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
\file src/terralib/qt/plugins/mobile/core/form/StringComboFormItem.cpp

\brief This file defines the class for a string combo form item.
*/

//TerraLib
#include "StringComboFormItem.h"

te::qt::plugins::terramobile::StringComboFormItem::StringComboFormItem() : AbstractFormItem()
{
  m_type = STRINGCOMBOFORMITEMTYPE;
}

te::qt::plugins::terramobile::StringComboFormItem::~StringComboFormItem()
{
}

void te::qt::plugins::terramobile::StringComboFormItem::setValue(std::string value) 
{ 
  m_value = value; 

  m_hasDefaultValue = true;
}

std::string te::qt::plugins::terramobile::StringComboFormItem::getValue() 
{ 
  return m_value; 
}

void te::qt::plugins::terramobile::StringComboFormItem::setValues(std::vector<std::string> values) 
{ 
  m_itemValues = values; 
}

std::vector<std::string> te::qt::plugins::terramobile::StringComboFormItem::getValues() 
{ 
  return m_itemValues; 
}

void te::qt::plugins::terramobile::StringComboFormItem::toString()
{
  m_defaultValue = m_value;

  m_values = m_itemValues;
}
