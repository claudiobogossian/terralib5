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
  \file terralib/serialization/se/Any.cpp
 
  \brief Support for Any serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/Any.h"
#include "Any.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <string>

te::xsd::Any* te::serialize::ReadAny(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "any");

  std::auto_ptr<te::xsd::Any> any(new te::xsd::Any);

  // Id
  ReadIdentifiable(any.get(), reader);

  // MinOccurs and MaxOccurs
  ReadOccurs(any.get(), reader);

  // Namespace
  std::size_t pos = reader.getAttrPosition("namespace");
  if(pos != std::string::npos)
    any->setNamespace(new std::string(reader.getAttr(pos)));

  // ProcessContents
  pos = reader.getAttrPosition("processContents");
  if(pos != std::string::npos)
  {
    std::string value = reader.getAttr(pos);
    te::xsd::ProcessContents pc = te::xsd::LAX;
    value == "skip" ? pc = te::xsd::SKIP : pc = te::xsd::STRICT;
    any->setProcessContents(pc);
  }

  reader.next();

  // Grammar: (annotation?)

  // Annotation
  ReadAnnotated(any.get(), reader);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return any.release();
}

void te::serialize::Save(te::xsd::Any* any, te::xml::Writer& writer)
{
}
