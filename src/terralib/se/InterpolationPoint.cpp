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
  \file terralib/se/InterpolationPoint.cpp
  
  \brief They are used to define a graph of points.
*/

// TerraLib
#include "InterpolationPoint.h"
#include "ParameterValue.h"

te::se::InterpolationPoint::InterpolationPoint()
  : m_data(0.0),
    m_value(0)
{
}

te::se::InterpolationPoint::InterpolationPoint(const InterpolationPoint& rhs)
  : m_data(0.0),
    m_value(0)
{
  if(rhs.m_value)
  {
    m_value = rhs.m_value->clone();
  }

  m_data = rhs.m_data;
}

te::se::InterpolationPoint::~InterpolationPoint()
{
  delete m_value;
}

te::se::InterpolationPoint* te::se::InterpolationPoint::clone() const
{
  return new InterpolationPoint(*this);
}

void te::se::InterpolationPoint::setData(const double& d)
{
  m_data = d;
}

double te::se::InterpolationPoint::getData() const
{
  return m_data;
}

void te::se::InterpolationPoint::setValue(ParameterValue* v)
{
  delete m_value;
  m_value = v;
}

te::se::ParameterValue* te::se::InterpolationPoint::getValue() const
{
  return m_value;
}

