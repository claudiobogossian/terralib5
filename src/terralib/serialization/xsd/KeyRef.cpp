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
  \file terralib/serialization/se/KeyRef.cpp
 
  \brief Support for KeyRef serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/KeyRef.h"
#include "Field.h"
#include "KeyRef.h"
#include "Selector.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <string>

te::xsd::KeyRef* te::serialize::ReadKeyRef(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "keyref");

  std::auto_ptr<te::xsd::KeyRef> keyRef(new te::xsd::KeyRef(0, 0));

  // Id
  ReadIdentifiable(keyRef.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  assert(pos != std::string::npos);
  keyRef->setName(new std::string(reader.getAttr(pos)));

  // Refer
  pos = reader.getAttrPosition("refer");
  assert(pos != std::string::npos);
  keyRef->setRef(CreateQName(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?,(selector,field+))

  // Annotation
  ReadAnnotated(keyRef.get(), reader);

  // Selector
  if(reader.getElementLocalName() == "selector")
  {
    keyRef->setSelector(ReadSelector(reader));
    return keyRef.release();
  }

  // Fields
  while(reader.getNodeType() == te::xml::START_ELEMENT && reader.getElementLocalName() == "field")
    keyRef->addField(ReadField(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return keyRef.release();
}

void te::serialize::Save(te::xsd::KeyRef* keyRef, te::xml::Writer& writer)
{
}
