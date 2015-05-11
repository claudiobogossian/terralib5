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
  \file terralib/se/Trim.cpp
  
  \brief Strips off chars from a string value (String formatting functions).
*/

// TerraLib
#include "ParameterValue.h"
#include "Trim.h"

te::se::Trim::Trim()
  : m_stringValue(0),
    m_stripOffPosition(LEADING)
{
}

te::se::Trim::~Trim()
{
  delete m_stringValue;
}

void te::se::Trim::setStringValue(ParameterValue* v)
{
  delete m_stringValue;
  m_stringValue = v;
}

void te::se::Trim::setStripOffPosition(StripOffPositionType p)
{
  m_stripOffPosition = p;
}

void te::se::Trim::setStripOffChar(const std::string& v)
{
  m_stripOffChar = v;
}

