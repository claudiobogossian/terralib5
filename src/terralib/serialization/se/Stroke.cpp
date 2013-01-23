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
  \file terralib/serialization/se/Stroke.cpp
 
  \brief Support for Stroke serialization.
*/

// TerraLib
#include "../../se/Stroke.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "Graphic.h"
#include "GraphicStroke.h"
#include "Stroke.h"
#include "SvgParameter.h"

// STL
#include <cassert>
#include <memory>

te::se::Stroke* te::serialize::ReadStroke(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Stroke");

  reader.next();

  std::auto_ptr<te::se::Stroke> stroke(new te::se::Stroke);

  // GraphicFill
  if(reader.getElementLocalName() == "GraphicFill")
  {
    reader.next();
    stroke->setGraphicFill(ReadGraphic(reader));

    assert(reader.getNodeType() == te::xml::END_ELEMENT);
    reader.next();
  }
  // GraphicStroke
  else if(reader.getElementLocalName() == "GraphicStroke")
    stroke->setGraphicStroke(ReadGraphicStroke(reader));

  // SvgParameters
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "SvgParameter")
    stroke->add(ReadSvgParameter(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return stroke.release();
}

void te::serialize::Save(const te::se::Stroke* stroke, te::xml::Writer& writer)
{
  if(stroke == 0)
    return;

  writer.writeStartElement("Stroke");

  if(stroke->getGraphicFill())
  {
    writer.writeStartElement("GraphicFill");
    Save(stroke->getGraphicFill(), writer);
    writer.writeEndElement("GraphicFill");
  }
  else if(stroke->getGraphicStroke())
    Save(stroke->getGraphicStroke(), writer);

  Save(stroke->getColor(), writer);
  Save(stroke->getOpacity(), writer);
  Save(stroke->getWidth(), writer);
  Save(stroke->getLineJoin(), writer);
  Save(stroke->getLineCap(), writer);
  Save(stroke->getDashArray(), writer);
  Save(stroke->setDashOffset(), writer);

  writer.writeEndElement("Stroke");
}
