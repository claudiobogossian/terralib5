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
#include "../color/RGBAColor.h"
#include "Description.h"
#include "FeatureTypeStyle.h"
#include "Fill.h"
#include "Graphic.h"
#include "LineSymbolizer.h"
#include "Mark.h"
#include "ParameterValue.h"
#include "PointSymbolizer.h"
#include "PolygonSymbolizer.h"
#include "Rule.h"
#include "Stroke.h"
#include "TextSymbolizer.h"
#include "Utils.h"

// Boost
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

// STL
#include <cassert>
#include <ctime>

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

te::se::Font* te::se::CreateFont(const std::string& family, const std::string& size, const te::se::Font::FontStyleType& style, const te::se::Font::FontWeightType& weight)
{
  te::se::Font* font = new te::se::Font;
  font->setFamily(family);
  font->setSize(size);
  font->setStyle(style);
  font->setWeight(weight);

  return font;
}

te::se::TextSymbolizer* te::se::CreateTextSymbolizer(const std::string& label, te::se::Fill* fill, te::se::Font* font)
{
  te::se::TextSymbolizer* symbolizer = new te::se::TextSymbolizer;
  symbolizer->setLabel(new te::se::ParameterValue(label));
  symbolizer->setFill(fill);
  symbolizer->setFont(font);

  return symbolizer;
}

te::se::Description* te::se::CreateDescription(const std::string& title, const std::string& abst)
{
  te::se::Description* description = new te::se::Description;
  description->setTitle(title);
  description->setAbstract(abst);
  
  return description;
}

te::se::Symbolizer* te::se::CreateSymbolizer(const te::gm::GeomType& geomType)
{
  switch(geomType)
  {
    case te::gm::PolygonType:
    case te::gm::PolygonMType:
    case te::gm::PolygonZType:
    case te::gm::PolygonZMType:
    case te::gm::MultiPolygonType:
    case te::gm::MultiPolygonMType:
    case te::gm::MultiPolygonZType:
    case te::gm::MultiPolygonZMType:
    {
      te::se::Fill* fill = CreateFill(GenerateRandomColor(), "1.0");
      te::se::Stroke* stroke = CreateStroke("#000000", "1");
      te::se::PolygonSymbolizer* symbolizer = new te::se::PolygonSymbolizer;
      symbolizer->setFill(fill);
      symbolizer->setStroke(stroke);
      return symbolizer;
    }

    case te::gm::LineStringType:
    case te::gm::LineStringMType:
    case te::gm::LineStringZType:
    case te::gm::LineStringZMType:
    case te::gm::MultiLineStringType:
    case te::gm::MultiLineStringMType:
    case te::gm::MultiLineStringZType:
    case te::gm::MultiLineStringZMType:
    {
      te::se::Stroke* stroke = CreateStroke(GenerateRandomColor(), "1");
      te::se::LineSymbolizer* symbolizer = new te::se::LineSymbolizer;
      symbolizer->setStroke(stroke);
      return symbolizer;
    }

    case te::gm::PointType:
    case te::gm::PointMType:
    case te::gm::PointZType:
    case te::gm::PointZMType:
    case te::gm::MultiPointType:
    case te::gm::MultiPointMType:
    case te::gm::MultiPointZType:
    case te::gm::MultiPointZMType:
    {
      te::se::Fill* markFill = CreateFill(GenerateRandomColor(), "1.0");
      te::se::Stroke* markStroke = CreateStroke("#000000", "1");
      te::se::Mark* mark = CreateMark("circle", markStroke, markFill);
      te::se::Graphic* graphic = CreateGraphic(mark, "12", "", "");
      return CreatePointSymbolizer(graphic);
    }

    default:
      return 0;
  }
}

te::se::Style* te::se::CreateFeatureTypeStyle(const te::gm::GeomType& geomType)
{
  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(CreateSymbolizer(geomType));

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

std::string te::se::GenerateRandomColor()
{
  boost::random::mt19937 gen(static_cast<boost::uint32_t>(std::time(0)));
  boost::random::uniform_int_distribution<> dist(0, 128);
  // Creates random pastel colours
  te::color::RGBAColor color(dist(gen) + 127, dist(gen) + 127, dist(gen) + 127, TE_OPAQUE);
  return color.getColor();
}
