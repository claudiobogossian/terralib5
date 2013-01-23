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
  \file terralib/serialization/se/Field.cpp
 
  \brief Support for Field serialization.
*/

// TerraLib
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../../xsd/Field.h"
#include "Field.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>
#include <string>

te::xsd::Field* te::serialize::ReadField(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "field");

  std::auto_ptr<te::xsd::Field> field(new te::xsd::Field(0));

  // Id
  ReadIdentifiable(field.get(), reader);

  // XPath
  std::size_t pos = reader.getAttrPosition("xpath");
  assert(pos != std::string::npos);
  field->setXPath(new std::string(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?)

  // Annotation
  ReadAnnotated(field.get(), reader);

  return field.release();
}

void te::serialize::Save(te::xsd::Field* field, te::xml::Writer& writer)
{
}
