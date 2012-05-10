/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/se/Utils.cpp

  \brief Utility functions for Symbology Enconding module.
*/

// TerraLib
#include "Fill.h"
#include "Graphic.h"
#include "LineSymbolizer.h"
#include "Mark.h"
#include "ParameterValue.h"
#include "PointSymbolizer.h"
#include "PolygonSymbolizer.h"
#include "Stroke.h"
#include "Utils.h"

te::se::Stroke* te::se::CreateStroke(const std::string& color, const std::string& width)
{
  return te::se::CreateStroke(color, width, "");
}

te::se::Stroke* te::se::CreateStroke(const std::string& color, const std::string& width, const std::string& opacity)
{
  return te::se::CreateStroke(color, width, opacity, "");
}

te::se::Stroke* te::se::CreateStroke(const std::string& color, const std::string& width,
                                     const std::string& opacity, const std::string& dasharray)
{
  return te::se::CreateStroke(color, width, opacity, dasharray, "", "");
}

te::se::Stroke* te::se::CreateStroke(const std::string& color, const std::string& width,
                                     const std::string& opacity, const std::string& dasharray,
                                     const std::string& linecap, const std::string& linejoin)
{
  te::se::Stroke* stroke = te::se::CreateStroke(0, width, opacity, dasharray, linecap, linejoin);

  if(!color.empty())
    stroke->setColor(color);
  
  return stroke;
}

te::se::Stroke* te::se::CreateStroke(te::se::Graphic* graphicFill,
                                     const std::string& width, const std::string& opacity, 
                                     const std::string& dasharray, const std::string& linecap, const std::string& linejoin)
{
  te::se::Stroke* stroke = new te::se::Stroke;

  if(graphicFill)
    stroke->setGraphicFill(graphicFill);
  
  if(!width.empty())
    stroke->setWidth(width);
  
  if(!opacity.empty())
    stroke->setOpacity(opacity);

  if(!dasharray.empty())
    stroke->setDashArray(dasharray);

  if(!linecap.empty())
    stroke->setLineCap(linecap);

  if(!linejoin.empty())
    stroke->setLineJoin(linecap);

  return stroke;
}

te::se::Fill* te::se::CreateFill(const std::string& color, const std::string& opacity)
{
  te::se::Fill* fill = new te::se::Fill;

  if(!color.empty())
    fill->setColor(color);

  if(!opacity.empty())
    fill->setOpacity(opacity);

  return fill;
}

te::se::Fill* te::se::CreateFill(te::se::Graphic* graphicFill)
{
  te::se::Fill* fill = new te::se::Fill;
  fill->setGraphicFill(graphicFill);

  return fill;
}

te::se::Mark* te::se::CreateMark(const std::string& wellKnownName, te::se::Stroke* stroke, te::se::Fill* fill)
{
  std::string* name = new std::string(wellKnownName);

  te::se::Mark* mark = new te::se::Mark;
  mark->setWellKnownName(name);
  mark->setStroke(stroke);
  mark->setFill(fill);

  return mark;
}

te::se::Graphic* te::se::CreateGraphic(te::se::Mark* mark, const std::string& size, const std::string& rotation, const std::string& opacity)
{
  te::se::Graphic* graphic = new te::se::Graphic;
  graphic->add(mark);

  if(!size.empty())
    graphic->setSize(new te::se::ParameterValue(size));
  
  if(!rotation.empty())
    graphic->setRotation(new te::se::ParameterValue(rotation));

  if(!opacity.empty())
    graphic->setOpacity(new te::se::ParameterValue(opacity));

  return graphic;
}

te::se::PolygonSymbolizer* te::se::CreatePolygonSymbolizer(te::se::Stroke* stroke, te::se::Fill* fill)
{
  te::se::PolygonSymbolizer* symbolizer = new te::se::PolygonSymbolizer;

  if(fill)
    symbolizer->setFill(fill);

  if(stroke)
    symbolizer->setStroke(stroke);

  return symbolizer;
}

te::se::LineSymbolizer* te::se::CreateLineSymbolizer(te::se::Stroke* stroke)
{
  te::se::LineSymbolizer* symbolizer = new te::se::LineSymbolizer;

  if(stroke)
    symbolizer->setStroke(stroke);

  return symbolizer;
}

te::se::PointSymbolizer* te::se::CreatePointSymbolizer(te::se::Graphic* graphic)
{
  te::se::PointSymbolizer* symbolizer = new te::se::PointSymbolizer;
  symbolizer->setGraphic(graphic);

  return symbolizer;
}
