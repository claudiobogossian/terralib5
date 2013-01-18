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
  \file terralib/serialization/se/SimpleType.cpp
 
  \brief Support for SimpleType serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/List.h"
#include "../../xsd/Restriction4SimpleType.h"
#include "../../xsd/SimpleType.h"
#include "../../xsd/Union.h"
#include "List.h"
#include "SimpleType.h"
#include "Restriction4SimpleType.h"
#include "Union.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <string>

te::xsd::SimpleType* te::serialize::ReadSimpleType(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "simpleType");

  std::auto_ptr<te::xsd::SimpleType> st(new te::xsd::SimpleType);

  // Id
  ReadIdentifiable(st.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    st->setName(new std::string(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?,(restriction|list|union))

  // Annotation
  ReadAnnotated(st.get(), reader);
  
  if(reader.getElementLocalName() == "restriction")
    st->setConstructor(ReadRestriction4SimpleType(reader));
  else if(reader.getElementLocalName() == "list")
    st->setConstructor(ReadList(reader));
  else if(reader.getElementLocalName() == "union")
    st->setConstructor(ReadUnion(reader));
  else throw; // TODO: Add an exception here...

  return st.release();
}

void te::serialize::Save(te::xsd::SimpleType* st, te::xml::Writer& writer)
{
}
