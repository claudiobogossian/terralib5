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
  \file terralib/se/Substring.cpp
  
  \brief It returns the substring (String formatting functions).
*/

// TerraLib
#include "ParameterValue.h"
#include "Substring.h"

te::se::Substring::Substring()
  : m_stringValue(0),
    m_position(0),
    m_length(0)
{
}

te::se::Substring::~Substring()
{
  delete m_stringValue;
  delete m_position;
  delete m_length;
}

void te::se::Substring::setStringValue(ParameterValue* v)
{
  delete m_stringValue;
  m_stringValue = v;
}

void te::se::Substring::setPosition(ParameterValue* v)
{
  delete m_position;
  m_position = v;
}

void te::se::Substring::setLength(ParameterValue* v)
{
  delete m_length;
  m_length = v;
}

