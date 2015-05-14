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
  \file terralib/se/Categorize.cpp
  
  \brief The transformation of continuous values to distinct values (Categorize function).
*/

// TerraLib
#include "../common/STLUtils.h"
#include "Categorize.h"
#include "ParameterValue.h"

te::se::Categorize::Categorize()
  : m_lookupValue(0),
    m_value(0),
    m_threshholdsBelongTo(NONE)
{
}

te::se::Categorize::Categorize(const Categorize& rhs)
  : m_lookupValue(0),
    m_value(0),
    m_threshholdsBelongTo(NONE)
{
  if(rhs.m_lookupValue)
  {
    m_lookupValue = rhs.m_lookupValue->clone();
  }

  if(rhs.m_value)
  {
    m_value = rhs.m_value->clone();
  }

  for(size_t t = 0; t < rhs.m_thresholds.size(); ++t)
  {
    m_thresholds.push_back(rhs.m_thresholds[t]->clone());
  }

  for(size_t t = 0; t < rhs.m_thresholdValues.size(); ++t)
  {
    m_thresholdValues.push_back(rhs.m_thresholdValues[t]->clone());
  }

  m_fallbackValue = rhs.getFallbackValue();

  m_threshholdsBelongTo = rhs.m_threshholdsBelongTo;
}

te::se::Categorize::~Categorize()
{
  delete m_lookupValue;
  delete m_value;
  te::common::FreeContents(m_thresholds);
  te::common::FreeContents(m_thresholdValues);
}

te::se::Categorize* te::se::Categorize::clone() const
{
  return new Categorize(*this);
}

void te::se::Categorize::setLookupValue(ParameterValue* v)
{
  delete m_lookupValue;
  m_lookupValue = v;
  
}

void te::se::Categorize::setValue(ParameterValue* v)
{
  delete m_value;
  m_value = v;
}

void te::se::Categorize::addThreshold(ParameterValue* v)
{
  m_thresholds.push_back(v);
}

void te::se::Categorize::addValue(ParameterValue* v)
{
  m_thresholdValues.push_back(v);
}

void te::se::Categorize::setThresholdsBelongTo(ThresholdsBelongToType t)
{
  m_threshholdsBelongTo = t;
}

const std::vector<te::se::ParameterValue*>& te::se::Categorize::getThresholds() const
{
  return m_thresholds;
}

const std::vector<te::se::ParameterValue*>& te::se::Categorize::getThresholdValues() const
{
  return m_thresholdValues;
}

