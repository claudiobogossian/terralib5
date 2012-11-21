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
  \file terralib/serialization/se/Fill.cpp
 
  \brief Support for Stroke serialization.
*/

// TerraLib
#include "../../se/Fill.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "Fill.h"
#include "Graphic.h"
#include "SvgParameter.h"

// STL
#include <cassert>
#include <memory>

te::se::Fill* te::serialize::ReadFill(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Fill");

  reader.next();

  std::auto_ptr<te::se::Fill> fill(new te::se::Fill);

  // GraphicFill
  if(reader.getElementLocalName() == "GraphicFill")
  {
    reader.next();
    fill->setGraphicFill(ReadGraphic(reader));
  }

  // SvgParameters
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
        reader.getElementLocalName() == "SvgParameter")
    fill->add(ReadSvgParameter(reader));

  return fill.release();
}

void te::serialize::Save(const te::se::Fill* fill, te::xml::Writer& writer)
{
  if(fill == 0)
    return;

  writer.writeStartElement("Fill");

  const te::se::Graphic* g = fill->getGraphicFill();
  if(g)
  {
    writer.writeStartElement("GraphicFill");
    Save(g, writer);
    writer.writeEndElement("GraphicFill");
  }
  else
  {
    Save(fill->getColor(), writer);
    Save(fill->getOpacity(), writer);
  }

  writer.writeEndElement("Fill");
}
