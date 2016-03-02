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
    m_outDset("")
{
}

void te::vp::MergeOp::setInput(te::da::DataSetType* firstDst, te::da::DataSet* firstDs, te::da::DataSetType* secondDst, te::da::DataSet* secondDs)
{
  m_firstDst = firstDst;
  m_firstDs = firstDs;
  m_secondDst = secondDst;
  m_secondDs = secondDs;
}

void te::vp::MergeOp::setParams(std::vector<std::pair<std::string, std::string> > properties)
{
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

  std::auto_ptr<te::dt::Property> pkProp = getNewPkProperty();
  
  te::da::PrimaryKey* pk = new te::da::PrimaryKey(m_outDset + "_pk", dt.get());
  pk->add(pkProp.get());
  dt->setPrimaryKey(pk);

  dt->add(pkProp.release());

  for (std::size_t i = 0; i < m_properties.size(); ++i)
  {
    std::string fp = m_properties[i].first;
    std::string sp = m_properties[i].second;

    if (!fp.empty())
    {
      dt->add(m_firstDst->getProperty(fp)->clone());
    }
    else if (!sp.empty())
    {
      dt->add(m_secondDst->getProperty(sp)->clone());
    }
  }

  dt->add(te::da::GetFirstSpatialProperty(m_firstDst)->clone());

  return dt;
}

std::auto_ptr<te::dt::Property> te::vp::MergeOp::getNewPkProperty()
{
  std::string pkName = "id";
  std::string auxName = pkName;

  std::size_t count = 0;

  while (!isValidName(auxName))
  {
    auxName = pkName + "_" + boost::lexical_cast<std::string>(count);
  }

  std::auto_ptr<te::dt::Property> result(new te::dt::SimpleProperty(auxName, te::dt::INT32_TYPE));

  return result;
}

bool te::vp::MergeOp::paramsAreValid()
{
  return true;
}

bool te::vp::MergeOp::isValidName(const std::string& name)
{
  for (std::size_t i = 0; i < m_properties.size(); ++i)
  {
    std::string fp = m_properties[i].first;
    std::string sp = m_properties[i].second;

    if (!fp.empty())
    {
      if (name == fp)
      {
        return false;
      }
    }
    else if (!sp.empty())
    {
      if (name == sp)
      {
        return false;
      }
    }
  }

  return true;
}