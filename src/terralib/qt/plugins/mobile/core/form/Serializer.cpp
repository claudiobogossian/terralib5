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
\file src/terralib/qt/plugins/mobile/core/form/Serializer.cpp

\brief This file defines the serializer for form classes.
*/

// TerraLib
#include "Serializer.h"

#include "AbstractFormItem.h"
#include "Form.h"
#include "Section.h"

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


std::string te::qt::plugins::terramobile::Write(te::qt::plugins::terramobile::Section* section)
{
  boost::property_tree::ptree sec;

  sec.put("sectionname", section->getName());
  sec.put("sectiondescription", section->getDescription());

  boost::property_tree::ptree forms;

  for (std::size_t t = 0; t < section->getForms().size(); ++t)
  {
    boost::property_tree::ptree form;

    te::qt::plugins::terramobile::Form* f = section->getForms()[t];

    form.put("formname", f->getName());

    boost::property_tree::ptree items;

    for (std::size_t p = 0; p < f->getItems().size(); ++p)
    {
      boost::property_tree::ptree item;

      te::qt::plugins::terramobile::AbstractFormItem* afi = f->getItems()[p];

      item.put("key", afi->getKey());
      item.put("label", afi->getLabel());

      if (!afi->getValues().empty())
      {
        boost::property_tree::ptree values;

        boost::property_tree::ptree itemsValues;

        for (std::size_t q = 0; q < afi->getValues().size(); ++q)
        {
          boost::property_tree::ptree val;
          val.put("item", afi->getValues()[q]);
          itemsValues.push_back(std::make_pair("", val));
        }

        values.add_child("items", itemsValues);

        item.push_back(std::make_pair("values", values));
      }

      item.put("value", afi->getStrDefaultValue());
      item.put("type", afi->getType());

      items.push_back(std::make_pair("", item));
    }

    form.add_child("formitems", items);

    forms.push_back(std::make_pair("", form));
  }

  sec.add_child("forms", forms);

  std::stringstream ss;

  boost::property_tree::json_parser::write_json(ss, sec);

  return ss.str();
}

te::qt::plugins::terramobile::Section* te::qt::plugins::terramobile::Read(std::string filePath)
{
  te::qt::plugins::terramobile::Section* section = 0;

  return section;
}

