/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
\file src/terralib/qt/plugins/mobile/core/form/BoolFormItem.cpp

\brief This file defines the class for a bool form item.
*/

//TerraLib
#include "BoolFormItem.h"

te::qt::plugins::terramobile::BoolFormItem::BoolFormItem() : AbstractFormItem()
{
  m_type = BOOLFORMITEMTYPE;
}

te::qt::plugins::terramobile::BoolFormItem::~BoolFormItem()
{
}

void te::qt::plugins::terramobile::BoolFormItem::setValue(bool value) 
{ 
  m_value = value;

  m_hasDefaultValue = true;
}

bool te::qt::plugins::terramobile::BoolFormItem::getValue() 
{ 
  return m_value; 
}

void te::qt::plugins::terramobile::BoolFormItem::toString()
{
  if (m_value)
    m_defaultValue = "TRUE";
  else
    m_defaultValue = "FALSE";
}
