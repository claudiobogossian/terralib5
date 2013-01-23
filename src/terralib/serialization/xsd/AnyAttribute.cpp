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
  \file terralib/serialization/se/AnyAttribute.cpp
 
  \brief Support for AnyAttribute serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/AnyAttribute.h"
#include "AnyAttribute.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <string>

te::xsd::AnyAttribute* te::serialize::ReadAnyAttribute(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "anyAttribute");

  std::auto_ptr<te::xsd::AnyAttribute> anyAttribute(new te::xsd::AnyAttribute);

  // Id
  ReadIdentifiable(anyAttribute.get(), reader);

  // Namespace
  std::size_t pos = reader.getAttrPosition("namespace");
  if(pos != std::string::npos)
    anyAttribute->setNamespace(new std::string(reader.getAttr(pos)));

  // ProcessContents
  pos = reader.getAttrPosition("processContents");
  if(pos != std::string::npos)
  {
    std::string value = reader.getAttr(pos);
    te::xsd::ProcessContents pc = te::xsd::LAX;
    value == "skip" ? pc = te::xsd::SKIP : pc = te::xsd::STRICT;
    anyAttribute->setProcessContents(pc);
  }

  reader.next();

  // Grammar: (annotation?)

  // Annotation
  ReadAnnotated(anyAttribute.get(), reader);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return anyAttribute.release();
}

void te::serialize::Save(te::xsd::AnyAttribute* anyAttribute, te::xml::Writer& writer)
{
}
