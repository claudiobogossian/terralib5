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
#include "../../../../../se/serialization/xml/Style.h"
#include "../../../../../se/Style.h"
#include "../../../../../xml/AbstractWriter.h"
#include "../../../../../xml/AbstractWriterFactory.h"
#include "../../../../../xml/Reader.h"
#include "../../../../../xml/ReaderFactory.h"
#include "Serializer.h"

#include "AbstractFormItem.h"
#include "Form.h"
#include "Section.h"

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem/operations.hpp>

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

std::string te::qt::plugins::terramobile::WriteStyle(const te::se::Style* style, std::string path)
{
  boost::filesystem::wpath file = boost::filesystem::absolute(path);
  std::string xml;
  {
    std::auto_ptr<te::xml::AbstractWriter> writer(te::xml::AbstractWriterFactory::make());

    writer->setURI(file.string());
    writer->writeStartDocument("UTF-8", "no");

    writer->writeStartElement("StyledLayerDescriptor");

    writer->writeAttribute("xmlns", "http://www.opengis.net/sld");
    writer->writeAttribute("xmlns:ogc", "http://www.opengis.net/ogc");
    writer->writeAttribute("xmlns:se", "http://www.opengis.net/se");
    writer->writeAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");
    writer->writeAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    writer->writeAttribute("xsi:schemaLocation", "http://www.opengis.net/sld StyledLayerDescriptor.xsd");

    writer->writeAttribute("version", "1.0");

    writer->writeStartElement("NamedLayer");
    writer->writeStartElement("UserStyle");

    te::se::serialize::Style::getInstance().write(style, *writer.get());

    writer->writeEndElement("UserStyle");
    writer->writeEndElement("NamedLayer");

    writer->writeEndElement("StyledLayerDescriptor");
    writer->writeToFile();

    std::ifstream t(file.string());
    std::stringstream buffer;
    buffer << t.rdbuf();

    xml = buffer.str();
  }

  boost::filesystem::remove(file);
  return xml;
}

std::auto_ptr<te::se::Style> te::qt::plugins::terramobile::ReadStyle(std::string xml)
{
  std::auto_ptr<te::se::Style> style;
  {
    std::ofstream ot("style.xml");
    ot << xml;
    ot.close();
  }

  boost::filesystem::wpath file = boost::filesystem::absolute("style.xml");
  {
    std::auto_ptr<te::xml::Reader> reader(te::xml::ReaderFactory::make());
    reader->setValidationScheme(false);

    reader->read(file.string());
    reader->next();

    if ((reader->getNodeType() == te::xml::START_ELEMENT) &&
      (reader->getElementLocalName() == "StyledLayerDescriptor"))
      reader->next();

    if ((reader->getNodeType() == te::xml::START_ELEMENT) &&
      (reader->getElementLocalName() == "NamedLayer"))
      reader->next();

    if ((reader->getNodeType() == te::xml::START_ELEMENT) &&
      (reader->getElementLocalName() == "UserStyle"))
      reader->next();

    if ((reader->getNodeType() == te::xml::START_ELEMENT) &&
      (reader->getElementLocalName() == "FeatureTypeStyle"))
    {
      if (reader->getNodeType() == te::xml::START_ELEMENT)
        style.reset(te::se::serialize::Style::getInstance().read(*reader.get()));
    }
  }

  boost::filesystem::remove(file);
  return std::move(style);
}

