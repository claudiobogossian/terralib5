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
  \file terralib/serialization/xsd/Utils.cpp
 
  \brief Utility methods for Schema serialization.
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "../../xml/Reader.h"
#include "../../xsd/Annotated.h"
#include "../../xsd/Identifiable.h"
#include "../../xsd/Occurs.h"
#include "../../xsd/QName.h"
#include "Annotation.h"
#include "Utils.h"

// STL
#include <cassert>
#include <string>
#include <vector>

void te::serialize::ReadIdentifiable(te::xsd::Identifiable* identifiable, te::xml::Reader& reader)
{
  assert(identifiable);

  std::size_t pos = reader.getAttrPosition("id");
  if(pos != std::string::npos)
    identifiable->setId(new std::string(reader.getAttr(pos)));
}

void te::serialize::ReadAnnotated(te::xsd::Annotated* annotated, te::xml::Reader& reader)
{
  assert(annotated);

  if((reader.getNodeType() != te::xml::START_ELEMENT) ||
     (reader.getElementLocalName() != "annotation"))
    return;

  te::xsd::Annotation* annotation = ReadAnnotation(reader);
  annotated->setAnnotation(annotation);
}

void te::serialize::ReadOccurs(te::xsd::Occurs* occurs, te::xml::Reader& reader)
{
  assert(occurs);

  // MinOccurs
  std::size_t pos = reader.getAttrPosition("minOccurs");
  if(pos != std::string::npos)
    occurs->setMinOccurs(static_cast<unsigned int>(reader.getAttrAsInt32(pos)));

  // MaxOccurs
  pos = reader.getAttrPosition("maxOccurs");
  if(pos != std::string::npos)
    occurs->setMaxOccurs(static_cast<unsigned int>(reader.getAttrAsInt32(pos)));
}

te::xsd::QName* te::serialize::CreateQName(const std::string& name)
{
  std::vector<std::string> tokens;
  te::common::Tokenize(name, tokens, ":");
  assert(tokens.size() >= 2);
  
  return new te::xsd::QName(tokens[0], tokens[1]);
}

te::xsd::FacetType te::serialize::GetFacetType(const std::string& name)
{
  if(name == "minExclusive")
    return te::xsd::MinExclusive;
  if(name == "minInclusive")
    return te::xsd::MinInclusive;
  if(name == "maxExclusive")
    return te::xsd::MaxExclusive;
  if(name == "maxInclusive")
    return te::xsd::MaxInclusive;
  if(name == "totalDigits")
    return te::xsd::TotalDigits;
  if(name == "fractionDigits")
    return te::xsd::FractionDigits;
  if(name == "length")
    return te::xsd::Length;
  if(name == "minLength")
    return te::xsd::MinLength;
  if(name == "maxLength")
    return te::xsd::MaxLength;
  if(name == "enumeration")
    return te::xsd::Enumeration;
  if(name == "whiteSpace")
    return te::xsd::WhiteSpace;

  return te::xsd::Pattern;
}
