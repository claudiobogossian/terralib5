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
  \file terralib/serialization/se/ComplexType.cpp
 
  \brief Support for ComplexType serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/All.h"
#include "../../xsd/Attribute.h"
#include "../../xsd/AttributeGroup.h"
#include "../../xsd/Choice.h"
#include "../../xsd/ComplexType.h"
#include "../../xsd/Group.h"
#include "../../xsd/Sequence.h"
#include "All.h"
#include "AnyAttribute.h"
#include "Attribute.h"
#include "AttributeGroup.h"
#include "Choice.h"
#include "ComplexContent.h"
#include "ComplexType.h"
#include "Group.h"
#include "Sequence.h"
#include "SimpleContent.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <set>
#include <string>

te::xsd::ComplexType* te::serialize::ReadComplexType(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "complexType");

  std::auto_ptr<te::xsd::ComplexType> ct(new te::xsd::ComplexType);

  // Id
  ReadIdentifiable(ct.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    ct->setName(new std::string(reader.getAttr(pos)));

  // Abstract
  pos = reader.getAttrPosition("abstract");
  if(pos != std::string::npos)
    ct->setAsAbstract(reader.getAttr(pos) == "true" ? true : false);

  // Mixed
  pos = reader.getAttrPosition("mixed");
  if(pos != std::string::npos)
    ct->setAsMixed(reader.getAttr(pos) == "true" ? true : false);

  // TODO: Block and Final ?

  reader.next();

  /* Grammar: (annotation?,(simpleContent|complexContent|((group|all|
              choice|sequence)?,((attribute|attributeGroup)*,anyAttribute?)))) */

  // Annotation
  ReadAnnotated(ct.get(), reader);

  std::set<std::string> children;
  children.insert("simpleContent");
  children.insert("complexContent");
  children.insert("group");
  children.insert("all");
  children.insert("choice");
  children.insert("sequence");
  children.insert("attribute");
  children.insert("attributeGroup");
  children.insert("anyAttribute");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "simpleContent")
    {
      ct->setSimpleContent(ReadSimpleContent(reader));
      continue;
    }

    if(tag == "complexContent")
    {
      ct->setComplexContent(ReadComplexContent(reader));
      continue;
    }

    if(tag == "group")
    {
      ct->setContent(ReadGroup(reader));
      continue;
    }

    if(tag == "all")
    {
      ct->setContent(ReadAll(reader));
      continue;
    }

    if(tag == "choice")
    {
      ct->setContent(ReadChoice(reader));
      continue;
    }

    if(tag == "sequence")
    {
      ct->setContent(ReadSequence(reader));
      continue;
    }

    if(tag == "attribute")
    {
      ct->addAttribute(ReadAttribute(reader));
      continue;
    }

    if(tag == "attributeGroup")
    {
      ct->addAttribute(ReadAttributeGroup(reader));
      continue;
    }

    if(tag == "anyAttribute")
      ct->setAnyAttribute(ReadAnyAttribute(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ct.release();
}

void te::serialize::Save(te::xsd::ComplexType* ct, te::xml::Writer& writer)
{
}
