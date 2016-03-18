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
 \file MergeOp.cpp
 */

#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/SimpleProperty.h"
#include "MergeOp.h"

// Boost
#include <boost/lexical_cast.hpp>

te::vp::MergeOp::MergeOp()
  : m_firstDst(0),
    m_firstDs(0),
    m_secondDst(0),
    m_secondDs(0),
    m_outDset(""),
    m_isUpdate(false),
    m_firstSRID(0)
{
}

void te::vp::MergeOp::setInput(te::da::DataSourcePtr firstSource,
                               te::da::DataSetType* firstDst,
                               te::da::DataSet* firstDs,
                               int firstSRID,
                               te::da::DataSetType* secondDst,
                               te::da::DataSet* secondDs)
{
  m_firstSource = firstSource;

  m_firstDst = firstDst;
  m_firstDs = firstDs;
  m_secondDst = secondDst;
  m_secondDs = secondDs;

  m_firstSRID = firstSRID;
}

void te::vp::MergeOp::setParams(std::vector<std::pair<std::string, std::string> > properties, bool isUpdate)
{
  m_isUpdate = isUpdate;

  std::vector<std::string> invalid = checkAttrNames(properties);

  if (!invalid.empty())
  {
    std::string err = TE_TR("Some attributes has the same name:\n\n");
    for (std::size_t i = 0; i < invalid.size(); ++i)
    {
      err += " - " + invalid[i] + "\n";
    }

    throw te::common::Exception(err);
  }

  m_properties = properties;
}

void te::vp::MergeOp::setOutput(te::da::DataSourcePtr outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDset = dsname;
}

std::vector<std::string> te::vp::MergeOp::checkAttrNames(const std::vector<std::pair<std::string, std::string> >& properties)
{
  std::vector<std::string> result;

  std::map<std::string, int> occurence;
  std::map<std::string, int>::iterator it;

  for (std::size_t i = 0; i < properties.size(); ++i)
  {
    if (!properties[i].first.empty())
    {
      occurence[properties[i].first] = 1;
    }
    else if (!properties[i].second.empty())
    {
      it = occurence.find(properties[i].second);
      if (it == occurence.end())
      {
        occurence[properties[i].second] = 1;
      }
      else
      {
        occurence[properties[i].second] = ++occurence[properties[i].second];
        result.push_back(properties[i].second);
      }
    }
  }

  return result;
}

std::auto_ptr<te::da::DataSetType> te::vp::MergeOp::getOutputDst()
{
  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(m_outDset));

  for (std::size_t i = 0; i < m_properties.size(); ++i)
  {
    std::string fp = m_properties[i].first;
    std::string sp = m_properties[i].second;

    if (fp.empty() && sp.empty())
    {
      continue;
    }

    te::dt::Property* fProp = m_firstDst->getProperty(fp);
    te::dt::Property* sProp = m_secondDst->getProperty(sp);

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
    te::da::PrimaryKey* pk = m_firstDst->getPrimaryKey();

    std::vector<te::dt::Property*> pkProp = pk->getProperties();

    te::da::PrimaryKey* newPk = new te::da::PrimaryKey(dt->getName() + "_pk");

    for (std::size_t i = 0; i < pkProp.size(); ++i)
    {
      newPk->add(dt->getProperty(pkProp[i]->getName()));
    }

    dt->setPrimaryKey(newPk);
  }

  dt->add(te::da::GetFirstSpatialProperty(m_firstDst)->clone());

  return dt;
}

void te::vp::MergeOp::updateFirstDst(te::da::DataSourceTransactor* transactor)
{
  for (std::size_t i = 0; i < m_properties.size(); ++i)
  {
    std::string fp = m_properties[i].first;
    std::string sp = m_properties[i].second;

    if (fp.empty() && sp.empty())
    {
      throw te::common::Exception(TE_TR("An unexpected error occurred during the output dataset update!"));
    }

    te::dt::Property* fProp = m_firstDst->getProperty(fp);
    te::dt::Property* sProp = m_secondDst->getProperty(sp);

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

        transactor->changePropertyDefinition(m_firstDst->getName(), fProp->getName(), auxSimpleProp);
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

        transactor->changePropertyDefinition(m_firstDst->getName(), fProp->getName(), auxSimpleProp);
      }
    }
    else if (!sp.empty())
    {
      newProp = sProp->clone();

      te::dt::SimpleProperty* newSimpleProp = dynamic_cast<te::dt::SimpleProperty*>(newProp);

      newSimpleProp->setRequired(false);
      newSimpleProp->setAutoNumber(false);

      transactor->addProperty(m_firstDst->getName(), newProp);
    }
    else
    {
      throw te::common::Exception(TE_TR("An unexpected error occurred during the output dataset update!"));
    }
  }
}

bool te::vp::MergeOp::paramsAreValid()
{
  return true;
}