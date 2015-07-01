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
  \file terralib/se/StringPosition.cpp
  
  \brief It returns the position of first occurence of a substring (String formatting functions).
*/

// TerraLib
#include "ParameterValue.h"
#include "StringPosition.h"

te::se::StringPosition::StringPosition()
  : m_lookupString(0),
    m_stringValue(0),
    m_searchDirection(FRONTTOBACK)
{
}

te::se::StringPosition::~StringPosition()
{
  delete m_lookupString;
  delete m_stringValue;
}

void te::se::StringPosition::setLookupString(ParameterValue* v)
{
  delete m_lookupString;
  m_lookupString = v;
}

void te::se::StringPosition::setStringValue(ParameterValue* v)
{
  delete m_stringValue;
  m_stringValue = v;
}

void te::se::StringPosition::setSearchDirection(SearchDirectionType t)
{
  m_searchDirection = t;
}

