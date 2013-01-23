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
  \file terralib/serialization/se/Restriction4SimpleType.cpp
 
  \brief Support for Restriction on a simpleType serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/SimpleTypeConstructor.h"
#include "../../xsd/Restriction4SimpleType.h"
#include "Restriction4SimpleType.h"
#include "SimpleType.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <set>
#include <string>

te::xsd::Restriction4SimpleType* te::serialize::ReadRestriction4SimpleType(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "restriction");

  std::auto_ptr<te::xsd::Restriction4SimpleType> restriction(new te::xsd::Restriction4SimpleType);

  // Id
  ReadIdentifiable(restriction.get(), reader);

  // Base
  std::size_t pos = reader.getAttrPosition("base");
  if(pos != std::string::npos)
    restriction->setBase(CreateQName(reader.getAttr(pos)));

  reader.next();

  /* Grammar: (annotation?,(simpleType?,(minExclusive|minInclusive|maxExclusive|maxInclusive|
              totalDigits|fractionDigits|length|minLength|maxLength|enumeration|whiteSpace|pattern)*)) */

  std::set<std::string> children;
  children.insert("minExclusive");
  children.insert("minInclusive");
  children.insert("maxExclusive");
  children.insert("maxInclusive");
  children.insert("totalDigits");
  children.insert("fractionDigits");
  children.insert("length");
  children.insert("minLength");
  children.insert("maxLength");
  children.insert("enumeration");
  children.insert("whiteSpace");
  children.insert("pattern");

  // Annotation
  ReadAnnotated(restriction.get(), reader);

  // Simple Type
  if(reader.getElementLocalName() == "simpleType")
     restriction->setSimpleType(ReadSimpleType(reader));

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    // Facet
    pos = reader.getAttrPosition("value");
    assert(pos != std::string::npos);
    restriction->addFacet(GetFacetType(*it), reader.getAttr(pos));

    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of Facet

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return restriction.release();
}

void te::serialize::Save(te::xsd::Restriction4SimpleType* restriction, te::xml::Writer& writer)
{
}
