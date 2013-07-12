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
  \file terralib/serialization/se/SimpleContent.cpp
 
  \brief Support for SimpleContent serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/Extension.h"
#include "../../xsd/Restriction4SimpleContent.h"
#include "../../xsd/SimpleContent.h"
#include "Extension.h"
#include "Restriction4SimpleContent.h"
#include "SimpleContent.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::xsd::SimpleContent* te::serialize::ReadSimpleContent(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "simpleContent");

  std::auto_ptr<te::xsd::SimpleContent> sc(new te::xsd::SimpleContent);

  // Id
  ReadIdentifiable(sc.get(), reader);

  reader.next();

  // Grammar: (annotation?,(restriction|extension))

  // Annotation
  ReadAnnotated(sc.get(), reader);

  if(reader.getElementLocalName() == "restriction")
    sc->setTypeDerivation(ReadRestriction4SimpleContent(reader));
  else if(reader.getElementLocalName() == "extension")
    sc->setTypeDerivation(ReadExtension(reader));
  else throw; // TODO: Add an exception here...

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return sc.release();
}

void te::serialize::Save(te::xsd::SimpleContent* sc, te::xml::Writer& writer)
{
}
