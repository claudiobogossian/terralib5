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
  \file terralib/se/ChangeCase.cpp
  
  \brief Changes the case of strings (String formatting functions).
*/

// TerraLib
#include "ChangeCase.h"
#include "ParameterValue.h"

te::se::ChangeCase::ChangeCase()
  : m_stringValue(0),
    m_direction(TOUPPER)
{
}

te::se::ChangeCase::~ChangeCase()
{
  delete m_stringValue;
}

void te::se::ChangeCase::setStringValue(ParameterValue* v)
{
  delete m_stringValue;
  m_stringValue = v;
}

void te::se::ChangeCase::setDirection(CaseDirectionType cd)
{
  m_direction = cd;
}

