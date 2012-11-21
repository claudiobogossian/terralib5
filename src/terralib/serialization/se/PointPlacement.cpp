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
  \file terralib/serialization/se/PointPlacement.cpp
 
  \brief Support for PointPlacement serialization.
*/

// TerraLib
#include "../../se/PointPlacement.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "AnchorPoint.h"
#include "Displacement.h"
#include "ParameterValue.h"
#include "PointPlacement.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::PointPlacement* te::serialize::ReadPointPlacement(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PointPlacement");

  reader.next();

  std::auto_ptr<te::se::PointPlacement> pp(new te::se::PointPlacement);

  // AnchorPoint
  if(reader.getElementLocalName() == "AnchorPoint")
    pp->setAnchorPoint(ReadAnchorPoint(reader));

  // Displacement
  if(reader.getElementLocalName() == "Displacement")
    pp->setDisplacement(ReadDisplacement(reader));

  // Rotation
  if(reader.getElementLocalName() == "Rotation")
  {
    reader.next();
    pp->setRotation(ReadParameterValue(reader));
  }

  return pp.release();
}

void te::serialize::Save(const te::se::PointPlacement* pp, te::xml::Writer& writer)
{
  if(pp == 0)
    return;

  writer.writeStartElement("PointPlacement");

  Save(pp->getAnchorPoint(), writer);
  Save(pp->getDisplacement(), writer);
  WriteParameterValuePtrHelper("Rotation", pp->getRotation(), writer);

  writer.writeEndElement("PointPlacement");
}
