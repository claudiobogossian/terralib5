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
  \file terralib/serialization/se/Graphic.cpp
 
  \brief Support for Graphic serialization.
*/

// TerraLib
#include "../../se/Graphic.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "AnchorPoint.h"
#include "Displacement.h"
#include "ExternalGraphic.h"
#include "Graphic.h"
#include "Mark.h"
#include "ParameterValue.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::Graphic* te::serialize::ReadGraphic(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Graphic");

  reader.next();

  std::auto_ptr<te::se::Graphic> graphic(new te::se::Graphic);

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        (reader.getElementLocalName() == "Mark" ||
        reader.getElementLocalName() == "ExternalGraphic"))
    reader.getElementLocalName() == "Mark" ? graphic->add(ReadMark(reader)) : graphic->add(ReadExternalGraphic(reader));

  // Opacity
  if(reader.getElementLocalName() == "Opacity")
  {
    reader.next();
    graphic->setOpacity(ReadParameterValue(reader));
  }

  // Size
  if(reader.getElementLocalName() == "Size")
  {
    reader.next();
    graphic->setSize(ReadParameterValue(reader));
  }

  // Rotation
  if(reader.getElementLocalName() == "Rotation")
  {
    reader.next();
    graphic->setRotation(ReadParameterValue(reader));
  }

  // AnchorPoint
  if(reader.getElementLocalName() == "AnchorPoint")
    graphic->setAnchorPoint(ReadAnchorPoint(reader));

  // Displacement
  if(reader.getElementLocalName() == "Displacement")
    graphic->setDisplacement(ReadDisplacement(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return graphic.release();
}

void te::serialize::Save(const te::se::Graphic* graphic, te::xml::Writer& writer)
{
  if(graphic == 0)
    return;

  writer.writeStartElement("se:Graphic");

  const std::vector<te::se::Mark*> marks = graphic->getMarks();
  for(std::size_t i = 0; i < marks.size(); ++i)
    Save(marks[i], writer);

  const std::vector<te::se::ExternalGraphic*> egs = graphic->getExternalGraphics();
  for(std::size_t i = 0; i < egs.size(); ++i)
    Save(egs[i], writer);

  WriteParameterValuePtrHelper("se:Opacity", graphic->getOpacity(), writer);
  WriteParameterValuePtrHelper("se:Size", graphic->getSize(), writer);
  WriteParameterValuePtrHelper("se:Rotation", graphic->getRotation(), writer);

  Save(graphic->getAnchorPoint(), writer);
  Save(graphic->getDisplacement(), writer);

  writer.writeEndElement("se:Graphic");
}
