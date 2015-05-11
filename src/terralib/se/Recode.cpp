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
  \file terralib/se/Recode.cpp
  
  \brief Transformation of discrete values to other values.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "MapItem.h"
#include "Recode.h"
#include "ParameterValue.h"

te::se::Recode::Recode()
  : m_lookupValue(0)
{
}

te::se::Recode::~Recode()
{
  delete m_lookupValue;
  te::common::FreeContents(m_mapItems);
}

void te::se::Recode::setLookupValue(ParameterValue* v)
{
  delete m_lookupValue;
  m_lookupValue = v;
}

void te::se::Recode::add(MapItem* m)
{
  m_mapItems.push_back(m);
}
