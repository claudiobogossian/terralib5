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
  \file terralib/serialization/se/All.cpp
 
  \brief Support for All serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/All.h"
#include "All.h"
#include "Element.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::xsd::All* te::serialize::ReadAll(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "all");

  std::auto_ptr<te::xsd::All> all(new te::xsd::All);

  // Id
  ReadIdentifiable(all.get(), reader);

  // MinOccurs and MaxOccurs
  ReadOccurs(all.get(), reader);

  reader.next();

  // Grammar: (annotation?,element*)

  // Annotation
  ReadAnnotated(all.get(), reader);

  // Elements
  while(reader.getNodeType() == te::xml::START_ELEMENT && (reader.getElementLocalName() == "element"))
    all->addElement(ReadElement(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return all.release();
}

void te::serialize::Save(te::xsd::All* all, te::xml::Writer& writer)
{
}
