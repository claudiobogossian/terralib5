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
\file src/terralib/qt/plugins/mobile/core/form/Form.cpp

\brief This file defines the form class.
*/

//TerraLib
#include "../../../../../common/STLUtils.h"
#include "Form.h"

te::qt::plugins::terramobile::Form::Form():
  m_name("")
{

}

te::qt::plugins::terramobile::Form::~Form()
{
  te::common::FreeContents(m_items);
  m_items.clear();
}

te::qt::plugins::terramobile::AbstractFormItem* te::qt::plugins::terramobile::Form::getItem(std::string itemName)
{
  for (std::size_t t = 0; t < m_items.size(); ++t)
  {
    if (m_items[t]->getKey() == itemName)
      return m_items[t];
  }

  return 0;
}
