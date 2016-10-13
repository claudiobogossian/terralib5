/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
 \file Difference.cpp
 */

#include "../core/translator/Translator.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../dataaccess/datasource/DataSourceInfoManager.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/query_h.h"
#include "../dataaccess/utils/Utils.h"

#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../datatype/StringProperty.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "AlgorithmParams.h"
#include "ComplexData.h"
#include "Merge.h"
#include "Utils.h"

// BOOST
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

const int BLOCKSIZE = 10000;

te::vp::Merge::Merge()
: m_inputDst(0),
  m_inputDs(0),
  m_mergeDst(0),
  m_mergeDs(0),
  m_outDsetName(""),
  m_isUpdate(false),
  m_inputSRID(0)
{
}

bool te::vp::Merge::executeMemory(te::vp::AlgorithmParams* mainParams)
{
  te::vp::ValidateAlgorithmParams(mainParams, te::vp::MEMORY);

  m_inputSource = mainParams->getInputParams()[0].m_inputDataSource;
  m_mergeSource = mainParams->getInputParams()[1].m_inputDataSource;
  m_inputDst = mainParams->getInputParams()[0].m_inputDataSetType;
  m_inputDs = mainParams->getInputParams()[0].m_inputDataSet;
  m_mergeDst = mainParams->getInputParams()[1].m_inputDataSetType;
  m_mergeDs = mainParams->getInputParams()[1].m_inputDataSet;
  m_outDsetName = mainParams->getOutputDataSetName();
  m_outDsrc = mainParams->getOutputDataSource();

  m_isUpdate = isUpdate(mainParams->getSpecificParams());

  te::gm::GeometryProperty* gpInput = te::da::GetFirstGeomProperty(m_inputDst);

  m_inputSRID = gpInput->getSRID();

  m_properties = getPropNames(mainParams->getSpecificParams());

  ///////////////////////////////////////////////////////////////////////////////////////////////

  std::auto_ptr<te::da::DataSourceTransactor> transactor;

  try {

    std::auto_ptr<te::da::DataSetType> dst;

    if (m_isUpdate)
    {
      transactor = m_inputSource->getTransactor();

      updateInputDst(transactor.get());

      std::auto_ptr<te::da::DataSetType> aux = transactor->getDataSetType(m_inputDst->getName());

      std::auto_ptr<te::da::DataSetTypeConverter> firstConverter(new te::da::DataSetTypeConverter(aux.release(), m_inputSource->getCapabilities(), m_inputSource->getEncoding()));
      te::da::AssociateDataSetTypeConverterSRID(firstConverter.get(), m_inputSRID);

      te::da::DataSetType* dtClone = dynamic_cast<te::da::DataSetType*>(firstConverter->getResult()->clone());

      dst.reset(dtClone);
    }
    else
    {
      dst = getOutputDataSetType(mainParams);

      transactor = m_outDsrc->getTransactor();
    }

    MergeStrategy s = checkStrategy();
    int pkMax = -1;
    std::size_t pkPos;
    int pkType;
    std::string pkName;

    if (s == AUTOINCREMENT)
    {
      te::dt::Property* pkProp = dst->getPrimaryKey()->getProperties()[0];
      pkName = pkProp->getName();
    }

    if (s == TRYGETMAX)
    {
      te::dt::Property* pkProp = dst->getPrimaryKey()->getProperties()[0];
      pkMax = tryGetMax();
      pkPos = dst->getPropertyPosition(pkProp);
      pkType = getPropertyType(pkProp);
    }

    te::mem::DataSet* ds = new te::mem::DataSet(dst.get());

    te::gm::GeometryProperty* fgGrop = te::da::GetFirstGeomProperty(m_inputDst);
    te::gm::GeometryProperty* sgGrop = te::da::GetFirstGeomProperty(m_mergeDst);
    te::gm::GeometryProperty* outgGrop = te::da::GetFirstGeomProperty(dst.get());

    std::size_t fgPos = m_inputDst->getPropertyPosition(fgGrop);
    std::size_t sgPos = m_mergeDst->getPropertyPosition(sgGrop);
    std::size_t outgPos = dst->getPropertyPosition(outgGrop);

    transactor->begin();

    int count = 0;

    m_inputDs->moveBeforeFirst();
    m_mergeDs->moveBeforeFirst();

    std::map<std::string, std::string> op;

    if (!m_isUpdate)
    {
      while (m_inputDs->moveNext())
      {
        te::mem::DataSetItem* item = new te::mem::DataSetItem(ds);

        for (std::size_t i = 0; i < m_properties.size(); ++i)
        {
          if (m_inputDs->isNull(m_properties[i].second))
            continue;

          if (!m_properties[i].first.empty())
          {
            if (isPrimaryKeyProperty(dst.get(), m_properties[i].first))
            {
              if (s != AUTOINCREMENT)
              {
                if (s == PUREMERGE)
                {
                  item->setValue(m_properties[i].first, m_inputDs->getValue(m_properties[i].first).release());
                }
                else if (s == TRYGETMAX)
                {
                  item->setValue(m_properties[i].first, m_inputDs->getValue(m_properties[i].first).release());
                }
              }
            }
            else
            {
              item->setValue(m_properties[i].first, m_inputDs->getValue(m_properties[i].first).release());
            }
          }
        }

        item->setGeometry(outgPos, m_inputDs->getGeometry(fgPos).release());

        ds->add(item);

        if (count == 0)
        {
          if (s == AUTOINCREMENT)
          {
            std::vector<std::string> pkNameVec;
            pkNameVec.push_back(pkName);

            std::auto_ptr<te::da::DataSet> adaptDs = te::da::HideColumns(ds, dst.get(), pkNameVec);

            transactor->createDataSet(dst.get(), op);
            transactor->add(dst->getName(), adaptDs.get(), op);

          }
          else
          {
            transactor->createDataSet(dst.get(), op);
            transactor->add(dst->getName(), ds, op);
          }

          delete ds;

          ds = new te::mem::DataSet(dst.get());
        }

        if ((count / BLOCKSIZE) >= 1)
        {
          if (s == AUTOINCREMENT)
          {
            std::vector<std::string> pkNameVec;
            pkNameVec.push_back(pkName);

            std::auto_ptr<te::da::DataSet> adaptDs = te::da::HideColumns(ds, dst.get(), pkNameVec);

            transactor->add(dst->getName(), adaptDs.get(), op);
          }
          else
          {
            transactor->add(dst->getName(), ds, op);
          }

          delete ds;

          ds = new te::mem::DataSet(dst.get());
        }

        ++count;
      }
    }

    while (m_mergeDs->moveNext())
    {
      te::mem::DataSetItem* item = new te::mem::DataSetItem(ds);

      if (s == TRYGETMAX)
      {
        ++pkMax;

        if (pkType == te::dt::INT16_TYPE)
        {
          item->setInt16(pkPos, pkMax);
        }
        else if (pkType == te::dt::INT32_TYPE)
        {
          item->setInt32(pkPos, pkMax);
        }
        else
        {
          item->setInt64(pkPos, pkMax);
        }
      }

      for (std::size_t i = 0; i < m_properties.size(); ++i)
      {
        if (!m_properties[i].second.empty())
        {
          if (m_mergeDs->isNull(m_properties[i].second))
            continue;

          if (!m_properties[i].first.empty())
          {
            if (s != TRYGETMAX && isPrimaryKeyProperty(dst.get(), m_properties[i].first))
            {
              if (s != AUTOINCREMENT)
              {
                if (s == PUREMERGE)
                {
                  item->setValue(m_properties[i].first, m_mergeDs->getValue(m_properties[i].second).release());
                }
              }
            }
            else
            {
              item->setValue(m_properties[i].first, m_mergeDs->getValue(m_properties[i].second).release());
            }
          }
          else
          {
            item->setValue(m_properties[i].second, m_mergeDs->getValue(m_properties[i].second).release());
          }
        }
      }

      std::auto_ptr<te::gm::Geometry> geom = m_mergeDs->getGeometry(sgPos);

      if (fgGrop->getSRID() != sgGrop->getSRID())
      {
        geom->transform(fgGrop->getSRID());
      }

      item->setGeometry(outgPos, geom.release());

      ds->add(item);

      if ((count / BLOCKSIZE) >= 1)
      {
        if (s == AUTOINCREMENT)
        {
          std::vector<std::string> pkNameVec;
          pkNameVec.push_back(pkName);

          std::auto_ptr<te::da::DataSet> adaptDs = te::da::HideColumns(ds, dst.get(), pkNameVec);

          transactor->add(dst->getName(), adaptDs.get(), op);
        }
        else
        {
          transactor->add(dst->getName(), ds, op);
        }

        delete ds;

        ds = new te::mem::DataSet(dst.get());
      }

      ++count;
    }

    if (ds)
    {
      if (s == AUTOINCREMENT)
      {
        std::vector<std::string> pkNameVec;
        pkNameVec.push_back(pkName);

        std::auto_ptr<te::da::DataSet> adaptDs = te::da::HideColumns(ds, dst.get(), pkNameVec);

        transactor->add(dst->getName(), adaptDs.get(), op);
      }
      else
      {
        transactor->add(dst->getName(), ds, op);
      }

      delete ds;
    }

    transactor->commit();

  }
  catch (std::exception& e)
  {
    transactor->rollBack();
    throw e;
  }

  return true;
}

bool te::vp::Merge::isUpdate(const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  std::map<std::string, te::dt::AbstractData*>::const_iterator it = specificParams.begin();

  bool isUpdate;

  while (it != specificParams.end())
  {
    if (it->first != "ISUPDATE")
    {
      ++it;
      continue;
    }

    te::dt::AbstractData* abData = it->second;

    te::vp::ComplexData<bool>* cd = dynamic_cast<te::vp::ComplexData<bool>* >(abData);

    if (cd)
      isUpdate = cd->getValue();

    ++it;
  }

  return isUpdate;
}

std::vector<std::pair<std::string, std::string> > te::vp::Merge::getPropNames(const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  std::map<std::string, te::dt::AbstractData*>::const_iterator it = specificParams.begin();

  std::vector<std::pair<std::string, std::string> > propNames;

  while (it != specificParams.end())
  {
    if (it->first != "ATTRIBUTES")
    {
      ++it;
      continue;
    }

    te::dt::AbstractData* abData = it->second;

    te::vp::ComplexData<std::vector<std::pair<std::string, std::string> > >* cd = dynamic_cast<te::vp::ComplexData<std::vector<std::pair<std::string, std::string> > >* >(abData);

    if (cd)
      propNames = cd->getValue();

    ++it;
  }

  return propNames;
}

std::auto_ptr<te::da::DataSetType> te::vp::Merge::getOutputDataSetType(te::vp::AlgorithmParams* mainParams)
{
  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(m_outDsetName));

  for (std::size_t i = 0; i < m_properties.size(); ++i)
  {
    std::string fp = m_properties[i].first;
    std::string sp = m_properties[i].second;

    if (fp.empty() && sp.empty())
    {
      continue;
    }

    te::dt::Property* fProp = m_inputDst->getProperty(fp);
    te::dt::Property* sProp = m_mergeDst->getProperty(sp);

    te::dt::Property* newProp = 0;

    if ((!fp.empty()) && (!sp.empty()))
    {
      te::dt::SimpleProperty* fSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(fProp);
      te::dt::SimpleProperty* sSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(sProp);

      if (fSimpleProp->isRequired() != sSimpleProp->isRequired())
      {
        newProp = fProp->clone();

        te::dt::SimpleProperty* newSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(newProp);

        newSimpleProp->setRequired(false);
      }
      else
      {
        newProp = fProp->clone();
      }
    }
    else if (!fp.empty())
    {
      newProp = fProp->clone();

      te::dt::SimpleProperty* newSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(newProp);

      newSimpleProp->setRequired(false);
    }
    else if (!sp.empty())
    {
      newProp = sProp->clone();

      te::dt::SimpleProperty* newSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(newProp);

      newSimpleProp->setRequired(false);
      newSimpleProp->setAutoNumber(false);
    }
    else
    {
      throw te::common::Exception(TE_TR("An unexpected error occurred during the output dataset creation!"));
    }

    dt->add(newProp);
  }

  if (m_outDsrc->getType() != "OGR")
  {
    te::da::PrimaryKey* pk = m_inputDst->getPrimaryKey();

    std::vector<te::dt::Property*> pkProp = pk->getProperties();

    te::da::PrimaryKey* newPk = new te::da::PrimaryKey(dt->getName() + "_pk");

    for (std::size_t i = 0; i < pkProp.size(); ++i)
    {
      newPk->add(dt->getProperty(pkProp[i]->getName()));
    }

    dt->setPrimaryKey(newPk);
  }

  dt->add(te::da::GetFirstSpatialProperty(m_inputDst)->clone());

  return dt;
}

te::vp::Merge::MergeStrategy te::vp::Merge::checkStrategy()
{
  te::da::PrimaryKey* firstPk = m_inputDst->getPrimaryKey();

  if ((m_outDsrc && m_outDsrc->getType() == "OGR") || m_inputSource->getType() == "OGR")
    return PUREMERGE;

  if (firstPk)
  {
    std::vector<te::dt::Property*> firstProps = firstPk->getProperties();

    if (firstProps.size() > 1)
    {
      return PUREMERGE;
    }
    else
    {
      te::dt::Property* prop = firstProps[0];

      if ((prop->getType() != te::dt::INT16_TYPE) && (prop->getType() != te::dt::INT32_TYPE) && (prop->getType() != te::dt::INT64_TYPE))
      {
        return PUREMERGE;
      }
      else
      {
        te::dt::SimpleProperty* simpleProp = dynamic_cast<te::dt::SimpleProperty*>(prop);

        if (simpleProp)
        {
          if (simpleProp->isAutoNumber())
          {
            return AUTOINCREMENT;
          }
          else
          {
            return TRYGETMAX;
          }
        }
        else
        {
          throw te::common::Exception(TE_TR("Merge error: Property type invalid."));
        }

      }
    }
  }
  else
    return PUREMERGE;

}

int te::vp::Merge::tryGetMax()
{
  te::dt::Property* pkProp = m_inputDst->getPrimaryKey()->getProperties()[0];

  te::da::FromItem* pFromItem = new te::da::DataSetName(m_inputDst->getName());

  te::da::From pFrom;
  pFrom.push_back(pFromItem);

  te::da::PropertyName pName(pkProp->getName());

  te::da::Max fMax(pName);

  te::da::Field* pField = new te::da::Field(fMax);

  te::da::Fields pFields;
  pFields.push_back(pField);

  te::da::Select pSelect(pFields, pFrom);

  std::auto_ptr<te::da::DataSet> result = m_inputSource->query(pSelect);

  result->moveFirst();

  std::string maxStr = result->getAsString(0);

  int maxInt = boost::lexical_cast<int>(maxStr);

  return maxInt;
}

bool te::vp::Merge::isPrimaryKeyProperty(const te::da::DataSetType* dst, const std::string& p)
{
  te::da::PrimaryKey* pk = dst->getPrimaryKey();
  if (!pk)
    return false;

  std::vector<te::dt::Property*> props = pk->getProperties();

  for (std::size_t i = 0; i < props.size(); ++i)
  {
    if (props[i]->getName() == p)
      return true;
  }

  return false;
}

void te::vp::Merge::updateInputDst(te::da::DataSourceTransactor* transactor)
{
  for (std::size_t i = 0; i < m_properties.size(); ++i)
  {
    std::string fp = m_properties[i].first;
    std::string sp = m_properties[i].second;

    if (fp.empty() && sp.empty())
    {
      throw te::common::Exception(TE_TR("An unexpected error occurred during the output dataset update!"));
    }

    te::dt::Property* fProp = m_inputDst->getProperty(fp);
    te::dt::Property* sProp = m_mergeDst->getProperty(sp);

    te::dt::Property* newProp = 0;

    if ((!fp.empty()) && (!sp.empty()))
    {
      te::dt::SimpleProperty* fSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(fProp);
      te::dt::SimpleProperty* sSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(sProp);

      if (fSimpleProp->isRequired() != sSimpleProp->isRequired())
      {
        te::dt::Property* auxProp = fSimpleProp->clone();
        te::dt::SimpleProperty* auxSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(auxProp);
        auxSimpleProp->setRequired(false);

        transactor->changePropertyDefinition(m_inputDst->getName(), fProp->getName(), auxSimpleProp);
      }
    }
    else if (!fp.empty())
    {
      te::dt::SimpleProperty* fSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(fProp);

      if (fSimpleProp->isRequired())
      {
        te::dt::Property* auxProp = fSimpleProp->clone();
        te::dt::SimpleProperty* auxSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(auxProp);
        auxSimpleProp->setRequired(false);

        transactor->changePropertyDefinition(m_inputDst->getName(), fProp->getName(), auxSimpleProp);
      }
    }
    else if (!sp.empty())
    {
      newProp = sProp->clone();

      te::dt::SimpleProperty* newSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(newProp);

      newSimpleProp->setRequired(false);
      newSimpleProp->setAutoNumber(false);

      transactor->addProperty(m_inputDst->getName(), newProp);
    }
    else
    {
      throw te::common::Exception(TE_TR("An unexpected error occurred during the output dataset update!"));
    }
  }
}

int te::vp::Merge::getPropertyType(const te::dt::Property* p)
{
  if (p->getType() == te::dt::INT16_TYPE || p->getType() == te::dt::UINT16_TYPE)
    return te::dt::INT16_TYPE;
  else if (p->getType() == te::dt::INT32_TYPE || p->getType() == te::dt::UINT32_TYPE)
    return te::dt::INT32_TYPE;
  else if (p->getType() == te::dt::INT64_TYPE || p->getType() == te::dt::UINT64_TYPE)
    return te::dt::INT64_TYPE;
  else
    throw; //Throw!!!

}

bool te::vp::Merge::executeQuery(te::vp::AlgorithmParams* mainParams)
{
  return true;
}