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
  \file terralib/serialization/se/Displacement.cpp
 
  \brief Support for Displacement serialization.
*/

// TerraLib
#include "../../se/Displacement.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "Displacement.h"
#include "ParameterValue.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::Displacement* te::serialize::ReadDisplacement(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Displacement");

  reader.next();

  std::auto_ptr<te::se::Displacement> d(new te::se::Displacement);

  assert(reader.getElementLocalName() == "DisplacementX");
  reader.next();
  d->setDisplacementX(ReadParameterValue(reader));

  assert(reader.getElementLocalName() == "DisplacementY");
  reader.next();
  d->setDisplacementY(ReadParameterValue(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return d.release();
}

void te::serialize::Save(const te::se::Displacement* d, te::xml::Writer& writer)
{
  if(d == 0)
    return;

  writer.writeStartElement("Displacement");

  WriteParameterValuePtrHelper("DisplacementX", d->getDisplacementX(), writer);
  WriteParameterValuePtrHelper("DisplacementY", d->getDisplacementY(), writer);

  writer.writeEndElement("Displacement");
}
