/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/xsd/ReadXSD.cpp

  \brief Functions for reading an XSD.
*/

// TerraLib
#include "ReadXSD.h"  // tip: keep this order of include
#include "../xml/ReaderFactory.h"
#include "Exception.h"
#include "Schema.h"

// STL
#include <cassert>
#include <memory>

#if TE_XSD_BUILD_WITH_XML_PARSER

te::xsd::Schema* te::xsd::Read(const std::string& file_name)
{
  std::auto_ptr<te::xml::Reader> xml_reader(te::xml::ReaderFactory::make());

  xml_reader->setValidationScheme(false);

  xml_reader->read(file_name);

  assert(xml_reader->getNodeType() == te::xml::START_DOCUMENT);

// read schema element
  xml_reader->next();
  assert(xml_reader->getNodeType() == te::xml::START_ELEMENT);
  assert(xml_reader->getElementLocalName() == "schema");

  std::auto_ptr<te::xsd::Schema> my_schema(new te::xsd::Schema(0));

  std::size_t n_namespaces = xml_reader->getNumberOfNamespaces();

// get namespaces
  for(std::size_t i = 0; i != n_namespaces; ++i)
  {
    std::pair<std::string, std::string> ns;

    xml_reader->getNamespace(i, ns);

    my_schema->getNamespaces().insert(boost::bimap<std::string, std::string>::value_type(ns.first, ns.second));
  }

// get schema attributes
  {
    std::size_t pos = xml_reader->getAttrPosition("id");

    if(pos != std::string::npos)
      my_schema->setId(new std::string(xml_reader->getAttr(pos)));
  }

  {
    std::size_t pos = xml_reader->getAttrPosition("attributeFormDefault");

    if(pos != std::string::npos)
      my_schema->setAttributeFormDefault(xml_reader->getAttr(pos) == "qualified" ? Qualified : Unqualified);
  }

  {
    std::size_t pos = xml_reader->getAttrPosition("elementFormDefault");

    if(pos != std::string::npos)
      my_schema->setElementFormDefault(xml_reader->getAttr(pos) == "qualified" ? Qualified : Unqualified);
  }

  //{
  //  std::size_t pos = xml_reader->getAttrPosition("blockDefault");

  //  if(pos != std::string::npos)
  //    my_schema->setId(new std::string(xml_reader->getAttr(pos)));
  //}

  //{
  //  std::size_t pos = xml_reader->getAttrPosition("finalDefault");

  //  if(pos != std::string::npos)
  //    my_schema->setId(new std::string(xml_reader->getAttr(pos)));
  //}

  {
    std::size_t pos = xml_reader->getAttrPosition("targetNamespace");

    if(pos != std::string::npos)
      my_schema->setTargetNamespace(xml_reader->getAttr(pos));
  }

  {
    std::size_t pos = xml_reader->getAttrPosition("version");

    if(pos != std::string::npos)
      my_schema->setVersion(xml_reader->getAttr(pos));
  }

  return 0;
}

#endif // TE_XSD_BUILD_WITH_XML_PARSER
