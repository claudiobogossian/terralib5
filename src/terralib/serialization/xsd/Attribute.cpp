/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/serialization/se/Attribute.cpp
 
  \brief Support for Attribute serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/Attribute.h"
#include "Attribute.h"
#include "SimpleType.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <string>

te::xsd::Attribute* te::serialize::ReadAttribute(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "attribute");

  std::auto_ptr<te::xsd::Attribute> attribute(new te::xsd::Attribute);

  // Id
  ReadIdentifiable(attribute.get(), reader);

  // Default
  std::size_t pos = reader.getAttrPosition("default");
  if(pos != std::string::npos)
    attribute->setDefault(new std::string(reader.getAttr(pos)));

  // Fixed
  pos = reader.getAttrPosition("fixed");
  if(pos != std::string::npos)
    attribute->setFixed(new std::string(reader.getAttr(pos)));

  // Form
  pos = reader.getAttrPosition("Form");
  if(pos != std::string::npos)
    attribute->setForm(reader.getAttr(pos) == "qualified" ? te::xsd::Qualified : te::xsd::Unqualified);

  // Name
  pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    attribute->setName(new std::string(reader.getAttr(pos)));

  // Ref
  pos = reader.getAttrPosition("ref");
  if(pos != std::string::npos)
    attribute->setRef(CreateQName(reader.getAttr(pos)));

  // Type
  pos = reader.getAttrPosition("type");
  if(pos != std::string::npos)
    attribute->setType(CreateQName(reader.getAttr(pos)));

  // Use
  pos = reader.getAttrPosition("use");
  if(pos != std::string::npos)
  {
    std::string value = reader.getAttr(pos);
    te::xsd::AttributeUse use = te::xsd::OPTIONAL;
    value == "prohibited" ? use = te::xsd::PROHIBITED : use = te::xsd::REQUIRED;
    attribute->setUse(use);
  }

  reader.next();

  // Grammar: (annotation?,(simpleType?))

  // Annotation
  ReadAnnotated(attribute.get(), reader);

  // Simple Type
  if(reader.getElementLocalName() == "simpleType")
    attribute->setInnerType(ReadSimpleType(reader));

  return attribute.release();
}

void te::serialize::Save(te::xsd::Attribute* attribute, te::xml::Writer& writer)
{
}
