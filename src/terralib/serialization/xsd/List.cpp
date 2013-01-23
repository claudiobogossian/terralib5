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
  \file terralib/serialization/se/List.cpp
 
  \brief Support for List serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/List.h"
#include "List.h"
#include "SimpleType.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <string>

te::xsd::List* te::serialize::ReadList(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "list");

  std::auto_ptr<te::xsd::List> list(new te::xsd::List);

  // Id
  ReadIdentifiable(list.get(), reader);

  // ItemType
  std::size_t pos = reader.getAttrPosition("itemType");
  if(pos != std::string::npos)
    list->setItemType(CreateQName(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?,(simpleType?))

  // Annotation
  ReadAnnotated(list.get(), reader);

  // Simple Type
  if(reader.getElementLocalName() == "simpleType")
     list->setSimpleType(ReadSimpleType(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return list.release();
}

void te::serialize::Save(te::xsd::List* list, te::xml::Writer& writer)
{
}
