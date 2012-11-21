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
  \file terralib/serialization/se/GraphicStroke.cpp
 
  \brief Support for GraphicStroke serialization.
*/

// TerraLib
#include "../../se/GraphicStroke.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "Graphic.h"
#include "GraphicStroke.h"
#include "ParameterValue.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

te::se::GraphicStroke* te::serialize::ReadGraphicStroke(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "GraphicStroke");

  reader.next();

  std::auto_ptr<te::se::GraphicStroke> graphicStroke(new te::se::GraphicStroke);

  assert(reader.getElementLocalName() == "Graphic");
  graphicStroke->setGraphic(ReadGraphic(reader));

  // InitialGap
  if(reader.getElementLocalName() == "InitialGap")
  {
    reader.next();
    graphicStroke->setInitialGap(ReadParameterValue(reader));
  }

  // Gap
  if(reader.getElementLocalName() == "Gap")
  {
    reader.next();
    graphicStroke->setGap(ReadParameterValue(reader));
  }

  return graphicStroke.release();
}

void te::serialize::Save(const te::se::GraphicStroke* graphicStroke, te::xml::Writer& writer)
{
  if(graphicStroke == 0)
    return;

  writer.writeStartElement("GraphicStroke");

  const te::se::Graphic* graphic = graphicStroke->getGraphic();
  assert(graphic);
  Save(graphic, writer);

  WriteParameterValuePtrHelper("InitialGap", graphicStroke->getInitialGap(), writer);
  WriteParameterValuePtrHelper("Gap", graphicStroke->getGap(), writer);

  writer.writeEndElement("GraphicStroke");
}
