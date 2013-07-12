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
  \file terralib/serialization/se/Redefine.cpp
 
  \brief Support for Redefine serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/ComplexType.h"
#include "../../xsd/Redefine.h"
#include "../../xsd/SimpleType.h"
#include "Annotation.h"
#include "AttributeGroup.h"
#include "ComplexType.h"
#include "Group.h"
#include "Redefine.h"
#include "SimpleType.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <set>
#include <string>

te::xsd::Redefine* te::serialize::ReadRedefine(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "redefine");

  std::auto_ptr<te::xsd::Redefine> redefine(new te::xsd::Redefine(""));

  // Id
  ReadIdentifiable(redefine.get(), reader);

  // SchemaLocation
  std::size_t pos = reader.getAttrPosition("schemaLocation");
  if(pos != std::string::npos)
    redefine->setSchemaLocation(reader.getAttr(pos));

  reader.next();

  // Grammar: (annotation|(simpleType|complexType|group|attributeGroup))*

  std::set<std::string> children;
  children.insert("annotation");
  children.insert("simpleType");
  children.insert("complexType");
  children.insert("group");
  children.insert("attributeGroup");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "annotation")
    {
      redefine->addAnnotation(ReadAnnotation(reader));
      continue;
    }

    if(tag == "simpleType")
    {
      redefine->addType(ReadSimpleType(reader));
      continue;
    }

    if(tag == "complexType")
    {
      redefine->addType(ReadComplexType(reader));
      continue;
    }

    if(tag == "group")
    {
      redefine->addGroup(ReadGroup(reader));
      continue;
    }

    if(tag == "attributeGroup")
      redefine->addAttributeGroup(ReadAttributeGroup(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return redefine.release();
}

void te::serialize::Save(te::xsd::Redefine* redefine, te::xml::Writer& writer)
{
}
