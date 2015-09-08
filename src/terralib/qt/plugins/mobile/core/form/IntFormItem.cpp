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
\file src/terralib/qt/plugins/mobile/core/form/IntFormItem.cpp

\brief This file defines the class for a int form item.
*/

//TerraLib
#include "../../../../../common/StringUtils.h"
#include "IntFormItem.h"

te::qt::plugins::terramobile::IntFormItem::IntFormItem() : AbstractFormItem()
{
  m_type = INTFORMITEMTYPE;
}

te::qt::plugins::terramobile::IntFormItem::~IntFormItem()
{
}

void te::qt::plugins::terramobile::IntFormItem::setValue(int value) 
{ 
  m_value = value;

  m_hasDefaultValue = true;
}

int te::qt::plugins::terramobile::IntFormItem::getValue() 
{
  return m_value; 
}

void te::qt::plugins::terramobile::IntFormItem::toString()
{
  m_defaultValue = te::common::Convert2String(m_value);
}
