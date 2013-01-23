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
  \file terralib/serialization/se/Extension.cpp
 
  \brief Support for Extension serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/All.h"
#include "../../xsd/Attribute.h"
#include "../../xsd/AttributeGroup.h"
#include "../../xsd/Choice.h"
#include "../../xsd/Extension.h"
#include "../../xsd/Group.h"
#include "../../xsd/Sequence.h"
#include "All.h"
#include "AnyAttribute.h"
#include "Attribute.h"
#include "AttributeGroup.h"
#include "Choice.h"
#include "Extension.h"
#include "Group.h"
#include "Sequence.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <set>
#include <string>

te::xsd::Extension* te::serialize::ReadExtension(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "extension");

  std::auto_ptr<te::xsd::Extension> extension(new te::xsd::Extension);

  // Id
  ReadIdentifiable(extension.get(), reader);

  // Base
  std::size_t pos = reader.getAttrPosition("base");
  if(pos != std::string::npos)
    extension->setBase(CreateQName(reader.getAttr(pos)));

  reader.next();

  /* Grammar: (annotation?,((group|all|choice|sequence)?,
              ((attribute|attributeGroup)*,anyAttribute?))) */

  // Annotation
  ReadAnnotated(extension.get(), reader);

  std::set<std::string> children;
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
    if(tag == "group")
    {
      extension->setContent(ReadGroup(reader));
      continue;
    }

    if(tag == "all")
    {
      extension->setContent(ReadAll(reader));
      continue;
    }

    if(tag == "choice")
    {
      extension->setContent(ReadChoice(reader));
      continue;
    }

    if(tag == "sequence")
    {
      extension->setContent(ReadSequence(reader));
      continue;
    }

    if(tag == "attribute")
    {
      extension->addAttribute(ReadAttribute(reader));
      continue;
    }

    if(tag == "attributeGroup")
    {
      extension->addAttribute(ReadAttributeGroup(reader));
      continue;
    }

    if(tag == "anyAttribute")
      extension->setAnyAttribute(ReadAnyAttribute(reader));
  }

  return extension.release();
}

void te::serialize::Save(te::xsd::Extension* extension, te::xml::Writer& writer)
{
}
