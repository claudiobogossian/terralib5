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
\file src/terralib/qt/plugins/mobile/core/form/AbstractFormItem.cpp

\brief This file defines the abstract class for a form item.
*/

//TerraLib
#include "AbstractFormItem.h"

te::qt::plugins::terramobile::AbstractFormItem::AbstractFormItem() :
  m_key(""),
  m_label(""),
  m_type(""),
  m_defaultValue(""),
  m_hasDefaultValue(false)
{

}

te::qt::plugins::terramobile::AbstractFormItem::~AbstractFormItem()
{
  m_values.clear();
}

std::string te::qt::plugins::terramobile::AbstractFormItem::getStrDefaultValue()
{
  //convert values to string
  if (m_hasDefaultValue)
    toString();

  return m_defaultValue;
}

std::vector<std::string> te::qt::plugins::terramobile::AbstractFormItem::getValues()
{ 
  toString();

  return m_values; 
}
