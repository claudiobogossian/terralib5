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
  \file terralib/se/SvgParameter.cpp
  
  \brief A SvgParameter refers to an SVG/CSS graphical-formatting parameter.
*/

// TerraLib
#include "SvgParameter.h"

te::se::SvgParameter::SvgParameter(const std::string& name)
  : te::se::ParameterValue(),
    m_name(name)
{
}

te::se::SvgParameter::SvgParameter(const std::string& name, const std::string& value)
  : te::se::ParameterValue(value),
    m_name(name)
{
}

te::se::SvgParameter::SvgParameter(const SvgParameter& rhs)
  : te::se::ParameterValue(rhs),
    m_name(rhs.m_name)
{
}

te::se::SvgParameter::~SvgParameter()
{
}

void te::se::SvgParameter::setName(const std::string& name)
{
  m_name = name;
}

std::string te::se::SvgParameter::getName() const
{
  return m_name;
}

te::se::SvgParameter* te::se::SvgParameter::clone() const
{
  return new SvgParameter(*this);
}
