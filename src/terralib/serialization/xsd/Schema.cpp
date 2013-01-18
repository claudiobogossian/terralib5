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
  \file terralib/serialization/se/Schema.cpp
 
  \brief Support for Schema serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/Attribute.h"
#include "../../xsd/Schema.h"
#include "Annotation.h"
#include "Attribute.h"
#include "AttributeGroup.h"
#include "ComplexType.h"
#include "Element.h"
#include "Group.h"
#include "Import.h"
#include "Include.h"
#include "Redefine.h"
#include "Schema.h"
#include "SimpleType.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <set>
#include <string>

te::xsd::Schema* te::serialize::ReadSchema(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "schema");

  std::auto_ptr<te::xsd::Schema> schema(new te::xsd::Schema(0));

  // Namespaces
  std::size_t numberOfNamespaces = reader.getNumberOfNamespaces();
  for(std::size_t i = 0; i < numberOfNamespaces; ++i)
  {
    std::pair<std::string, std::string> ns;
    reader.getNamespace(i, ns);
    schema->getNamespaces().insert(boost::bimap<std::string, std::string>::value_type(ns.first, ns.second));
  }

  // Id
  ReadIdentifiable(schema.get(), reader);

  // AttributeFormDefault
  std::size_t pos = reader.getAttrPosition("attributeFormDefault");
  if(pos != std::string::npos)
    schema->setAttributeFormDefault(reader.getAttr(pos) == "qualified" ? te::xsd::Qualified : te::xsd::Unqualified);

  // ElementFormDefault
  pos = reader.getAttrPosition("elementFormDefault");
  if(pos != std::string::npos)
    schema->setElementFormDefault(reader.getAttr(pos) == "qualified" ? te::xsd::Qualified : te::xsd::Unqualified);

  // TODO: BlockDefault and FinalDefault ?

  // TargetNamespace
  pos = reader.getAttrPosition("targetNamespace");
  if(pos != std::string::npos)
    schema->setTargetNamespace(reader.getAttr(pos));

  // Version
  pos = reader.getAttrPosition("version");
  if(pos != std::string::npos)
    schema->setVersion(reader.getAttr(pos));

  reader.next();

  /* Grammar: ((include|import|redefine|annotation)*,(((simpleType|complexType|
               group|attributeGroup)|element|attribute|notation),annotation*)*) */

  /* TODO: Using a set to find the element's children. Temporary solution!
     Suggestion: we can put this information on a static member of te::xsd classes. - Uba, 2013 */
  std::set<std::string> children;
  children.insert("include");
  children.insert("import");
  children.insert("redefine");
  children.insert("annotation");
  children.insert("simpleType");
  children.insert("complexType"); 
  children.insert("group");
  children.insert("attributeGroup");
  children.insert("element");
  children.insert("attribute");
  children.insert("notation");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "include")
    {
      schema->addInclude(ReadInclude(reader));
      continue;
    }

    if(tag == "import")
    {
      schema->addImport(ReadImport(reader));
      continue;
    }

    if(tag == "redefine")
    {
      schema->addRedefine(ReadRedefine(reader));
      continue;
    }

    if(tag == "annotation")
    {
      schema->addAnnotation(ReadAnnotation(reader));
      continue;
    }

    if(tag == "simpleType")
    {
      schema->addSimpleType(ReadSimpleType(reader));
      continue;
    }

    if(tag == "complexType")
    {
      schema->addComplexType(ReadComplexType(reader));
      continue;
    }

    if(tag == "group")
    {
      schema->addGroup(ReadGroup(reader));
      continue;
    }

    if(tag == "attributeGroup")
    {
      schema->addAttributeGroup(ReadAttributeGroup(reader));
      continue;
    }

    if(tag == "element")
    {
      schema->addElement(ReadElement(reader));
      continue;
    }

    if(tag == "attribute")
      schema->addAttribute(ReadAttribute(reader));
  }

  return schema.release();
}

void te::serialize::Save(te::xsd::Schema* schema, te::xml::Writer& writer)
{
}
