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
  \file terralib/serialization/se/Element.cpp
 
  \brief Support for Element serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/Element.h"
#include "../../xsd/QName.h"
#include "Element.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <set>
#include <string>

te::xsd::Element* te::serialize::ReadElement(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "element");

  std::auto_ptr<te::xsd::Element> element(new te::xsd::Element);

  // Id
  ReadIdentifiable(element.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    element->setName(new std::string(reader.getAttr(pos)));

  // Ref
  pos = reader.getAttrPosition("ref");
  if(pos != std::string::npos)
    element->setRef(CreateQName(reader.getAttr(pos)));

  // Type
  pos = reader.getAttrPosition("type");
  if(pos != std::string::npos)
    element->setType(CreateQName(reader.getAttr(pos)));

  // SubstitutionGroup
  pos = reader.getAttrPosition("substitutionGroup");
  if(pos != std::string::npos)
    element->setSubstitutionGroup(CreateQName(reader.getAttr(pos)));

  // Default
  pos = reader.getAttrPosition("default");
  if(pos != std::string::npos)
    element->setDefaultValue(new std::string(reader.getAttr(pos)));

  // Fixed
  pos = reader.getAttrPosition("fixed");
  if(pos != std::string::npos)
    element->setFixedValue(new std::string(reader.getAttr(pos)));

  // MinOccurs and MaxOccurs
  ReadOccurs(element.get(), reader);

  // Nillable
  pos = reader.getAttrPosition("nillable");
  if(pos != std::string::npos)
    element->setAsNillable(reader.getAttr(pos) == "true" ? true : false);

  // Abstract
  pos = reader.getAttrPosition("abstract");
  if(pos != std::string::npos)
    element->setAsAbstract(reader.getAttr(pos) == "true" ? true : false);

  // TODO: Block and Final ?

  reader.next();

  // Grammar: annotation?,((simpleType|complexType)?,(unique|key|keyref)*))

  ReadAnnotated(element.get(), reader);

  std::set<std::string> children;
  children.insert("simpleType");
  children.insert("complexType");
  children.insert("unique");
  children.insert("key");
  children.insert("keyref");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
  }

  return element.release();
}

void te::serialize::Save(te::xsd::Element* element, te::xml::Writer& writer)
{
}
