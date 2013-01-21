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
  \file terralib/serialization/se/Restriction4ComplexContent.cpp
 
  \brief Support for Restriction on a complexContent serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/All.h"
#include "../../xsd/Attribute.h"
#include "../../xsd/AttributeGroup.h"
#include "../../xsd/Choice.h"
#include "../../xsd/Group.h"
#include "../../xsd/Restriction4ComplexContent.h"
#include "../../xsd/Sequence.h"
#include "All.h"
#include "AnyAttribute.h"
#include "Attribute.h"
#include "AttributeGroup.h"
#include "Choice.h"
#include "Group.h"
#include "Restriction4ComplexContent.h"
#include "Sequence.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <set>
#include <string>

te::xsd::Restriction4ComplexContent* te::serialize::ReadRestriction4ComplexContent(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "restriction");

  std::auto_ptr<te::xsd::Restriction4ComplexContent> restriction(new te::xsd::Restriction4ComplexContent(0, 0));

  // Id
  ReadIdentifiable(restriction.get(), reader);

  // Base
  std::size_t pos = reader.getAttrPosition("base");
  if(pos != std::string::npos)
    restriction->setBase(CreateQName(reader.getAttr(pos)));

  reader.next();

  /* Grammar: (annotation?,(group|all|choice|sequence)?,
              ((attribute|attributeGroup)*,anyAttribute?)) */

  std::set<std::string> children;
  children.insert("group");
  children.insert("all");
  children.insert("choice");
  children.insert("sequence");
  children.insert("attribute");
  children.insert("attributeGroup");
  children.insert("anyAttribute");

  // Annotation
  ReadAnnotated(restriction.get(), reader);

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;

    if(tag == "group")
    {
      restriction->setContent(ReadGroup(reader));
      continue;
    }

    if(tag == "all")
    {
      restriction->setContent(ReadAll(reader));
      continue;
    }

    if(tag == "choice")
    {
      restriction->setContent(ReadChoice(reader));
      continue;
    }

    if(tag == "sequence")
    {
      restriction->setContent(ReadSequence(reader));
      continue;
    }

    if(tag == "attribute")
    {
      restriction->addAttribute(ReadAttribute(reader));
      continue;
    }

    if(tag == "attributeGroup")
    {
      restriction->addAttribute(ReadAttributeGroup(reader));
      continue;
    }

    if(tag == "anyAttribute")
      restriction->setAnyAttribute(ReadAnyAttribute(reader));
  }

  return restriction.release();
}

void te::serialize::Save(te::xsd::Restriction4ComplexContent* restriction, te::xml::Writer& writer)
{
}
