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
  \file terralib/se/MapItem.cpp
  
  \brief 
*/

// TerraLib
#include "MapItem.h"
#include "ParameterValue.h"

te::se::MapItem::MapItem()
  : m_data(0.0),
    m_value(0)
{
}

te::se::MapItem::~MapItem()
{
  delete m_value;
}

void te::se::MapItem::setData(const double& d)
{
  m_data = d;
}

void te::se::MapItem::setValue(ParameterValue* v)
{
  delete m_value;
  m_value = v;
}
