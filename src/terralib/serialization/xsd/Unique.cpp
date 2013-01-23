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
  \file terralib/serialization/se/Unique.cpp
 
  \brief Support for Unique serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/Unique.h"
#include "Field.h"
#include "Selector.h"
#include "Unique.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <string>

te::xsd::Unique* te::serialize::ReadUnique(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "unique");

  std::auto_ptr<te::xsd::Unique> unique(new te::xsd::Unique(0));

  // Id
  ReadIdentifiable(unique.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  assert(pos != std::string::npos);
  unique->setName(new std::string(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?,(selector,field+))

  // Annotation
  ReadAnnotated(unique.get(), reader);

  // Selector
  if(reader.getElementLocalName() == "selector")
  {
    unique->setSelector(ReadSelector(reader));
    return unique.release();
  }

  // Fields
  while(reader.getNodeType() == te::xml::START_ELEMENT && reader.getElementLocalName() == "field")
    unique->addField(ReadField(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return unique.release();
}

void te::serialize::Save(te::xsd::Unique* unique, te::xml::Writer& writer)
{
}
