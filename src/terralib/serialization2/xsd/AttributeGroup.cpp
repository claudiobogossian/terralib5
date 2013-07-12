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
  \file terralib/serialization/se/AttributeGroup.cpp
 
  \brief Support for AttributeGroup serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/Attribute.h"
#include "../../xsd/AttributeGroup.h"
#include "AnyAttribute.h"
#include "Attribute.h"
#include "AttributeGroup.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <set>
#include <string>

te::xsd::AttributeGroup* te::serialize::ReadAttributeGroup(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "attributeGroup");

  std::auto_ptr<te::xsd::AttributeGroup> attributeGroup(new te::xsd::AttributeGroup);

  // Id
  ReadIdentifiable(attributeGroup.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    attributeGroup->setName(new std::string(reader.getAttr(pos)));

  // Ref
  pos = reader.getAttrPosition("ref");
  if(pos != std::string::npos)
    attributeGroup->setRef(CreateQName(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?),((attribute|attributeGroup)*,anyAttribute?))

  // Annotation
  ReadAnnotated(attributeGroup.get(), reader);

  std::set<std::string> children;
  children.insert("attribute");
  children.insert("attributeGroup");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "attribute")
    {
      attributeGroup->addAttribute(ReadAttribute(reader));
      continue;
    }

    attributeGroup->addAttribute(ReadAttributeGroup(reader));
  }

  // AnyAttribute
  if(reader.getElementLocalName() == "anyAttribute")
    attributeGroup->setAnyAttribute(ReadAnyAttribute(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return attributeGroup.release();
}

void te::serialize::Save(te::xsd::AttributeGroup* attributeGroup, te::xml::Writer& writer)
{
}
