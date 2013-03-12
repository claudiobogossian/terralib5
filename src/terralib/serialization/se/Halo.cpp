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
  \file terralib/serialization/se/Halo.cpp
 
  \brief Support for Halo serialization.
*/

// TerraLib
#include "../../se/Halo.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "Fill.h"
#include "Halo.h"
#include "ParameterValue.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::Halo* te::serialize::ReadHalo(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Halo");

  reader.next();

  std::auto_ptr<te::se::Halo> halo(new te::se::Halo);

  // Radius
  if(reader.getElementLocalName() == "Radius")
  {
    reader.next();
    halo->setRadius(ReadParameterValue(reader));
  }
  
  // Fill
  if(reader.getElementLocalName() == "Fill")
    halo->setFill(ReadFill(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return halo.release();
}

void te::serialize::Save(const te::se::Halo* halo, te::xml::Writer& writer)
{
  if(halo == 0)
    return;

  writer.writeStartElement("se:Halo");

  WriteParameterValuePtrHelper("se:Radius", halo->getRadius(), writer);
  Save(halo->getFill(), writer);

  writer.writeEndElement("se:Halo");
}
