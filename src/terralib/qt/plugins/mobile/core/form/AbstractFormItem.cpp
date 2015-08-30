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
  m_defaultValue("")
{

}

te::qt::plugins::terramobile::AbstractFormItem::~AbstractFormItem()
{
  m_values.clear();
}

void te::qt::plugins::terramobile::AbstractFormItem::serialize(boost::property_tree::ptree& pt)
{
  //convert values to string
  toString();

  pt.put("key", m_key);
  pt.put("label", m_label);

  if (!m_values.empty())
  {
    boost::property_tree::ptree items;

    boost::property_tree::ptree values;

    for (std::size_t t = 0; t < m_values.size(); ++t)
    {
      boost::property_tree::ptree val;
      val.put("item", m_values[t]);
      values.push_back(std::make_pair("", val));
    }

    items.add_child("items", values);

    pt.push_back(std::make_pair("values", items));
  }

  pt.put("value", m_defaultValue);
  pt.put("type", m_type);
}
