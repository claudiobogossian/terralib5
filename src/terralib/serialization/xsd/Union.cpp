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
  \file terralib/serialization/se/Union.cpp
 
  \brief Support for Union serialization.
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/Union.h"
#include "Annotation.h"
#include "Union.h"
#include "SimpleType.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <string>
#include <vector>

te::xsd::Union* te::serialize::ReadUnion(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "union");

  std::auto_ptr<te::xsd::Union> u(new te::xsd::Union);

  // Id
  ReadIdentifiable(u.get(), reader);

  // Member Types (List of QNames)
  std::size_t pos = reader.getAttrPosition("memberTypes");
  if(pos != std::string::npos)
  {
    std::string value = reader.getAttr(pos);
    std::vector<std::string> tokens;
    te::common::Tokenize(value, tokens, " ");
    for(std::size_t i = 0; i < tokens.size(); ++i)
      u->addMemberType(CreateQName(tokens[i]));
  }

  reader.next();

  // Grammar: (annotation?, (simpleType*))

  // Annotation
  ReadAnnotated(u.get(), reader);

  // SimpleTypes
  while(reader.getNodeType() == te::xml::START_ELEMENT && (reader.getElementLocalName() == "simpleType"))
    u->addSimpleType(ReadSimpleType(reader));

  return u.release();
}

void te::serialize::Save(te::xsd::Union* u, te::xml::Writer& writer)
{
}
