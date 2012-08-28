/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/se/Interpolate.cpp
  
  \brief The transformation of continuous values to a number of values (Interpolate function).
*/

// TerraLib
#include "../common/STLUtils.h"
#include "Interpolate.h"
#include "InterpolationPoint.h"
#include "ParameterValue.h"

te::se::Interpolate::Interpolate()
  : m_lookupValue(0),
    m_mode(LINEAR),
    m_method(NUMERIC)
{
}

te::se::Interpolate::~Interpolate()
{
  delete m_lookupValue;
  te::common::FreeContents(m_interpolationPoints);
}

void te::se::Interpolate::setLookupValue(ParameterValue* v)
{
  delete m_lookupValue;
  m_lookupValue = v;
}

void te::se::Interpolate::add(InterpolationPoint* i)
{
  m_interpolationPoints.push_back(i);
}

void te::se::Interpolate::setModeType(ModeType t)
{
  m_mode = t;
}

void te::se::Interpolate::setMethodType(MethodType t)
{
  m_method = t;
}

std::vector<te::se::InterpolationPoint*>& te::se::Interpolate::getInterpolationPoints()
{
  return m_interpolationPoints;
}
