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
\file src/terralib/qt/plugins/mobile/core/form/DateFormItem.cpp

\brief This file defines the class for a date form item.
*/

//TerraLib
#include "DateFormItem.h"

te::qt::plugins::terramobile::DateFormItem::DateFormItem() : AbstractFormItem()
{
  m_type = DATEFORMITEMTYPE;
}

te::qt::plugins::terramobile::DateFormItem::~DateFormItem()
{
  delete m_value;
}

void te::qt::plugins::terramobile::DateFormItem::setValue(te::dt::DateTime* value) 
{ 
  m_value = value;

  m_hasDefaultValue = true;
}

te::dt::DateTime* te::qt::plugins::terramobile::DateFormItem::getValue() 
{ 
  return m_value; 
}

void te::qt::plugins::terramobile::DateFormItem::toString()
{
  m_defaultValue = m_value->toString();
}
