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
  \file terralib/serialization/se/ComplexContent.cpp
 
  \brief Support for ComplexType serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/ComplexContent.h"
#include "../../xsd/Extension.h"
#include "../../xsd/Restriction4ComplexContent.h"
#include "ComplexContent.h"
#include "Extension.h"
#include "Restriction4ComplexContent.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::xsd::ComplexContent* te::serialize::ReadComplexContent(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "complexContent");

  std::auto_ptr<te::xsd::ComplexContent> cc(new te::xsd::ComplexContent);

  // Id
  ReadIdentifiable(cc.get(), reader);

  // Mixed
  std::size_t pos = reader.getAttrPosition("mixed");
  if(pos != std::string::npos)
    cc->setAsMixed(reader.getAttr(pos) == "true" ? true : false);

  reader.next();

  /* Grammar: (annotation?,(restriction|extension)) */

  // Annotation
  ReadAnnotated(cc.get(), reader);

  // Restriction
  if(reader.getElementLocalName() == "restriction")
  {
    cc->setTypeDerivation(ReadRestriction4ComplexContent(reader));
    return cc.release();
  }

  assert(reader.getElementLocalName() == "extension");
  cc->setTypeDerivation(ReadExtension(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return cc.release();
}

void te::serialize::Save(te::xsd::ComplexContent* cc, te::xml::Writer& writer)
{
}
