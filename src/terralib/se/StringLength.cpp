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
  \file terralib/se/StringLength.cpp
  
  \brief Returns length of string (String formatting functions).
*/

// TerraLib
#include "ParameterValue.h"
#include "StringLength.h"

te::se::StringLength::StringLength()
  : m_stringValue(0)
{
}

te::se::StringLength::~StringLength()
{
  delete m_stringValue;
}

void te::se::StringLength::setStringValue(ParameterValue* v)
{
  delete m_stringValue;
  m_stringValue = v;
}

