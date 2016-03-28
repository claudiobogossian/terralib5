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
  \file MergeMemory.h

  \brief Merge Vector Processing functions.
*/

//Terralib
#include "../common/Exception.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/query_h.h"
#include "../dataaccess/utils/Utils.h"
#include "../geometry/GeometryProperty.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "MergeMemory.h"
#include "Utils.h"

// Boost
#include <boost/lexical_cast.hpp>

const int BLOCKSIZE = 10000;

te::vp::MergeMemory::MergeMemory()
{}

te::vp::MergeMemory::~MergeMemory()
{}

te::vp::MergeMemory::Strategy te::vp::MergeMemory::checkStrategy()
{
  te::da::PrimaryKey* firstPk = m_firstDst->getPrimaryKey();

  if ((m_outDsrc && m_outDsrc->getType() == "OGR") || m_firstSource->getType() == "OGR")
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



bool te::vp::MergeMemory::run() throw(te::common::Exception)
{
  std::auto_ptr<te::da::DataSourceTransactor> transactor;

  try{

    std::auto_ptr<te::da::DataSetType> dst;

    if (m_isUpdate)
    {
      transactor = m_firstSource->getTransactor();

      updateFirstDst(transactor.get());

      std::auto_ptr<te::da::DataSetType> aux = transactor->getDataSetType(m_firstDst->getName());

      std::auto_ptr<te::da::DataSetTypeConverter> firstConverter(new te::da::DataSetTypeConverter(aux.release(), m_firstSource->getCapabilities(), m_firstSource->getEncoding()));
      te::da::AssociateDataSetTypeConverterSRID(firstConverter.get(), m_firstSRID);

      te::da::DataSetType* dtClone = dynamic_cast<te::da::DataSetType*>(firstConverter->getResult()->clone());

      dst.reset(dtClone);
    }
    else
    {
      dst = getOutputDst();

      transactor = m_outDsrc->getTransactor();
    }

    Strategy s = checkStrategy();
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

    te::gm::GeometryProperty* fgGrop = te::da::GetFirstGeomProperty(m_firstDst);
    te::gm::GeometryProperty* sgGrop = te::da::GetFirstGeomProperty(m_secondDst);
    te::gm::GeometryProperty* outgGrop = te::da::GetFirstGeomProperty(dst.get());

    std::size_t fgPos = m_firstDst->getPropertyPosition(fgGrop);
    std::size_t sgPos = m_secondDst->getPropertyPosition(sgGrop);
    std::size_t outgPos = dst->getPropertyPosition(outgGrop);

    transactor->begin();

    int count = 0;

    m_firstDs->moveBeforeFirst();
    m_secondDs->moveBeforeFirst();

    std::map<std::string, std::string> op;

    if (!m_isUpdate)
    {
      while (m_firstDs->moveNext())
      {
        te::mem::DataSetItem* item = new te::mem::DataSetItem(ds);

        for (std::size_t i = 0; i < m_properties.size(); ++i)
        {
          if (!m_properties[i].first.empty())
          {
            if (isPrimaryKeyProperty(dst.get(), m_properties[i].first))
            {
              if (s != AUTOINCREMENT)
              {
                if (s == PUREMERGE)
                {
                  item->setValue(m_properties[i].first, m_firstDs->getValue(m_properties[i].first).release());
                }
                else if (s == TRYGETMAX)
                {
                  item->setValue(m_properties[i].first, m_firstDs->getValue(m_properties[i].first).release());
                }
              }
            }
            else
            {
              item->setValue(m_properties[i].first, m_firstDs->getValue(m_properties[i].first).release());
            }
          }
        }

        item->setGeometry(outgPos, m_firstDs->getGeometry(fgPos).release());

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

    while (m_secondDs->moveNext())
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
          if (!m_properties[i].first.empty())
          {
            if (s != TRYGETMAX && isPrimaryKeyProperty(dst.get(), m_properties[i].first))
            {
              if (s != AUTOINCREMENT)
              {
                if (s == PUREMERGE)
                {
                  item->setValue(m_properties[i].first, m_secondDs->getValue(m_properties[i].second).release());
                }
              }
            }
            else
            {
              item->setValue(m_properties[i].first, m_secondDs->getValue(m_properties[i].second).release());
            }
          }
          else
          {
            item->setValue(m_properties[i].second, m_secondDs->getValue(m_properties[i].second).release());
          }
        }
      }

      std::auto_ptr<te::gm::Geometry> geom = m_secondDs->getGeometry(sgPos);

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
  catch (const std::exception& e)
  {
    transactor->rollBack();
    std::string err = e.what();
    throw e;
  }

  return true;
}

int te::vp::MergeMemory::tryGetMax()
{
  te::dt::Property* pkProp = m_firstDst->getPrimaryKey()->getProperties()[0];

  te::da::FromItem* pFromItem = new te::da::DataSetName(m_firstDst->getName());

  te::da::From pFrom;
  pFrom.push_back(pFromItem);

  te::da::PropertyName pName(pkProp->getName());

  te::da::Max fMax(pName);

  te::da::Field* pField = new te::da::Field(fMax);

  te::da::Fields pFields;
  pFields.push_back(pField);

  te::da::Select pSelect(pFields, pFrom);

  std::auto_ptr<te::da::DataSet> result = m_firstSource->query(pSelect);

  result->moveFirst();

  std::string maxStr = result->getAsString(0);

  int maxInt = boost::lexical_cast<int>(maxStr);

  return maxInt;
}

bool te::vp::MergeMemory::isPrimaryKeyProperty(const te::da::DataSetType* dst, const std::string& p)
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

int te::vp::MergeMemory::getPropertyType(const te::dt::Property* p)
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
