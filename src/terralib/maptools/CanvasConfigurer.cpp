/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/CanvasConfigurer.cpp
  
  \brief A Symbology Enconding visitor that configures a given canvas based on symbolizers elements.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/STLUtils.h"
#include "../fe/Literal.h"
#include "../se/ExternalGraphic.h"
#include "../se/Fill.h"
#include "../se/Font.h"
#include "../se/Graphic.h"
#include "../se/GraphicStroke.h"
#include "../se/Halo.h"
#include "../se/LineSymbolizer.h"
#include "../se/PointSymbolizer.h"
#include "../se/PolygonSymbolizer.h"
#include "../se/Stroke.h"
#include "../se/SvgParameter.h"
#include "../se/Symbolizer.h"
#include "../se/TextSymbolizer.h"
#include "../se/Utils.h"
#include "../xlink/SimpleLink.h"
#include "Canvas.h"
#include "CanvasConfigurer.h"
#include "ExternalGraphicRendererManager.h"
#include "MarkRendererManager.h"
#include "Utils.h"

// STL
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <vector>

std::map<std::string, te::map::LineCapStyle> te::map::CanvasConfigurer::sm_lineCapMap;
std::map<std::string, te::map::LineJoinStyle> te::map::CanvasConfigurer::sm_lineJoinMap;

te::map::CanvasConfigurer::CanvasConfigurer(te::map::Canvas* canvas)
  : m_canvas(canvas)
{
  // LineCapMap
  sm_lineCapMap[TE_SE_BUTT_CAP  ] = te::map::ButtCap;
  sm_lineCapMap[TE_SE_ROUND_CAP ] = te::map::RoundCap;
  sm_lineCapMap[TE_SE_SQUARE_CAP] = te::map::SquareCap;

  // LineJoinMap
  sm_lineJoinMap[TE_SE_MITRE_JOIN] = te::map::MiterJoin;
  sm_lineJoinMap[TE_SE_ROUND_JOIN] = te::map::RoundJoin;
  sm_lineJoinMap[TE_SE_BEVEL_JOIN] = te::map::BevelJoin;
}

te::map::CanvasConfigurer::~CanvasConfigurer()
{}

void te::map::CanvasConfigurer::config(const te::se::Symbolizer* symbolizer)
{
  symbolizer->accept(*this);
}

void te::map::CanvasConfigurer::visit(const te::se::Style& visited)
{
  // no need
}

void te::map::CanvasConfigurer::visit(const te::se::FeatureTypeStyle& visited)
{
// no need
}

void te::map::CanvasConfigurer::visit(const te::se::CoverageStyle& visited)
{
// no need
}

void te::map::CanvasConfigurer::visit(const te::se::Symbolizer& visited)
{
// no need
}

void te::map::CanvasConfigurer::visit(const te::se::PolygonSymbolizer& visited)
{
  // Default
  configDefaultPolygon();

  // Configuring the polygon stroke...
  const te::se::Stroke* stroke = visited.getStroke();
  if(stroke)
    config(stroke, false);
  else
    m_canvas->setPolygonContourColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT)); // no stroke

  // Configuring the polygon fill...
  const te::se::Fill* fill = visited.getFill();
  if(fill)
    config(fill);
  else
    m_canvas->setPolygonFillColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT)); // no fill
}

void te::map::CanvasConfigurer::visit(const te::se::LineSymbolizer& visited)
{
  // Configuring the line stroke...
  const te::se::Stroke* stroke = visited.getStroke();
  if(!stroke)
  {
    m_canvas->setLineColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT)); // no stroke
    return;
  }

  // Default
  configDefaultLine();

  config(stroke);
}

void te::map::CanvasConfigurer::visit(const te::se::PointSymbolizer& visited)
{
  // Default
  configDefaultPoint();

  const te::se::Graphic* graphic = visited.getGraphic();
  if(graphic)
    config(graphic, te::map::CanvasConfigurer::Point);
}

void te::map::CanvasConfigurer::visit(const te::se::TextSymbolizer& visited)
{
  // Default
  configDefaultText();

  // Color
  const te::se::Fill* fill = visited.getFill();
  te::color::RGBAColor color(TE_SE_DEFAULT_TEXT_COLOR, TE_OPAQUE);
  te::se::GetColor(fill, color);
  m_canvas->setTextColor(color);

  const te::se::Font* font = visited.getFont();
  if(font)
  {
    // Family
    const te::se::SvgParameter* family = font->getFamily();
    if(family)
      m_canvas->setFontFamily(te::se::GetString(family));

    // Size
    const te::se::SvgParameter* size = font->getSize();
    if(size)
      m_canvas->setTextPointSize(te::se::GetDouble(size));

    // Style - {normal, italic, and oblique}
    const te::se::SvgParameter* style = font->getStyle();
    if(style)
    {
      std::string value = te::se::GetString(style);
      //TODO: m_canvas->setTextStyle(need a map <std::string, te::FontStyle>);
    }

    // Weight - {normal, and bold}
    const te::se::SvgParameter* weight = font->getWeight();
    if(weight)
    {
      std::string value = te::se::GetString(weight);
      //TODO: m_canvas->setTextWeight(need a map <std::string, te::WeightStyle>);
    }
  }

  // Halo
  const te::se::Halo* halo = visited.getHalo();
  if(halo)
  {
    m_canvas->setTextContourEnabled(true);
    
    // Halo Color
    const te::se::Fill* haloFill = halo->getFill();
    te::color::RGBAColor haloColor(TE_SE_DEFAULT_HALO_COLOR, TE_OPAQUE);
    te::se::GetColor(haloFill, haloColor);
    m_canvas->setTextContourColor(haloColor);

    // Halo Radius
    int width = TE_SE_DEFAULT_HALO_RADIUS;
    const te::se::ParameterValue* radius = halo->getRadius();
    if(radius)
      width = te::se::GetInt(radius);
    m_canvas->setTextContourWidth(width);
  }

  // Property that will be labeled
  //const te::se::ParameterValue* label = visited.getLabel();
}

void te::map::CanvasConfigurer::visit(const te::se::RasterSymbolizer& visited)
{
// TODO
}

void te::map::CanvasConfigurer::config(const te::se::Stroke* stroke, const bool& fromLineSymbolizer)
{
  // Graphic Fill
  const te::se::Graphic* graphicFill = stroke->getGraphicFill();
  if(graphicFill)
    fromLineSymbolizer ? config(graphicFill, te::map::CanvasConfigurer::Line) : config(graphicFill, te::map::CanvasConfigurer::Contour);
  else
  {
    // Color
    te::color::RGBAColor color(TE_SE_DEFAULT_STROKE_BASIC_COLOR, TE_OPAQUE);
    te::se::GetColor(stroke, color);
    fromLineSymbolizer ? m_canvas->setLineColor(color) : m_canvas->setPolygonContourColor(color);
  }

  // Width
  const te::se::SvgParameter* width = stroke->getWidth();
  if(width)
    fromLineSymbolizer ? m_canvas->setLineWidth(te::se::GetInt(width)) : m_canvas->setPolygonContourWidth(te::se::GetInt(width));

  // LineCap
  const te::se::SvgParameter* linecap = stroke->getLineCap();
  if(linecap)
  {
    std::map<std::string, te::map::LineCapStyle>::iterator it = sm_lineCapMap.find(te::se::GetString(linecap));
    if(it != sm_lineCapMap.end())
      fromLineSymbolizer ? m_canvas->setLineCapStyle(it->second) : m_canvas->setPolygonContourCapStyle(it->second);
  }

  // LineJoin
  const te::se::SvgParameter* linejoin = stroke->getLineJoin();
  if(linejoin)
  {
    std::map<std::string, te::map::LineJoinStyle>::iterator it = sm_lineJoinMap.find(te::se::GetString(linejoin));
    if(it != sm_lineJoinMap.end())
      fromLineSymbolizer ? m_canvas->setLineJoinStyle(it->second) : m_canvas->setPolygonContourJoinStyle(it->second);
  }

  // Dasharray
  const te::se::SvgParameter* dasharray = stroke->getDashArray();
  if(dasharray)
  {
    std::string value = te::se::GetString(dasharray);
    std::vector<double> pattern;
    te::map::GetDashStyle(value, pattern);
    fromLineSymbolizer ? m_canvas->setLineDashStyle(pattern) : m_canvas->setPolygonContourDashStyle(pattern);
  }

  /* TODO: 1) stroke-dashoffset;
           2) Should be verified the GraphicStroke. */
}

void te::map::CanvasConfigurer::config(const te::se::Fill* fill)
{
  // Graphic Fill
  const te::se::Graphic* graphicFill = fill->getGraphicFill();
  if(graphicFill)
  {
    config(graphicFill, te::map::CanvasConfigurer::Fill);
    return;
  }

  // Color
  te::color::RGBAColor color(TE_SE_DEFAULT_FILL_BASIC_COLOR, TE_OPAQUE);
  te::se::GetColor(fill, color);
  m_canvas->setPolygonFillColor(color);
}

void te::map::CanvasConfigurer::config(const te::se::Graphic* graphic, te::map::CanvasConfigurer::ConfigStyle configStyle)
{
  // Gets the graphic size (it defines the height)
  const te::se::ParameterValue* size = graphic->getSize();
  std::size_t height = TE_SE_DEFAULT_GRAPHIC_SIZE;
  if(size)
    height = static_cast<std::size_t>(te::se::GetInt(size));

  // Gets the graphic rotation 
  const te::se::ParameterValue* rotation = graphic->getRotation();
  double angle = 0.0;
  if(rotation)
    angle = te::se::GetDouble(rotation);

  // Gets the graphic opacity
  int alpha = TE_OPAQUE;
  const te::se::ParameterValue* opacity = graphic->getOpacity();
  if(opacity)
    alpha = (int)(te::se::GetDouble(opacity) * TE_OPAQUE);

  std::size_t width = height;

  /* TODO: Here we have a set of marks and external graphics. Need review!
           Some questions:
           :: Create a Graphic2ImagePatternFactory? The factory will create an image pattern that represents the composition of marks and external graphics.
           :: Keep a reference to the dataset and make the draw here? 
           For while consider Mark or ExtrenalGraphic and use the first! */

  // The image pattern that will be used to configure the canvas
  te::color::RGBAColor** rgba = 0;

  // Generates the image pattern 
  //const std::vector<te::se::Mark*> marks = graphic->getMarks();
  //if(!marks.empty())
  //  rgba = te::map::MarkRendererManager::getInstance().render(marks[0], height);
  //else
  //{
  //  const std::vector<te::se::ExternalGraphic*> exgs = graphic->getExternalGraphics();
  //  if(!exgs.empty())
  //    rgba = te::map::ExternalGraphicRendererManager::getInstance().render(exgs[0], height, width);
  //}

  const std::vector<te::se::ExternalGraphic*> exgs = graphic->getExternalGraphics();
    
  if(!exgs.empty())
    rgba = te::map::ExternalGraphicRendererManager::getInstance().render(exgs[0], height, width);
  else
  {
    const std::vector<te::se::Mark*> marks = graphic->getMarks();
    if(!marks.empty())
      rgba = te::map::MarkRendererManager::getInstance().render(marks[0], height);
  }

  // Let's config the canvas!
  switch(configStyle)
  {
    case te::map::CanvasConfigurer::Fill:
      m_canvas->setPolygonFillColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT)); // It shouldn't be necessary! (Uba, Feb 2013)
      m_canvas->setPolygonFillPattern(rgba, width, height);
      m_canvas->setPolygonPatternRotation(angle);
      m_canvas->setPolygonPatternOpacity(alpha);
    break;

    case te::map::CanvasConfigurer::Contour:
      m_canvas->setPolygonContourColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT)); // It shouldn't be necessary! (Uba, Feb 2013)
      m_canvas->setPolygonContourPattern(rgba, width, height);
      m_canvas->setPolygonContourPatternRotation(angle);
      m_canvas->setPolygonContourPatternOpacity(alpha);
    break;

    case te::map::CanvasConfigurer::Line:
      m_canvas->setLineColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT)); // It shouldn't be necessary! (Uba, Feb 2013)
      m_canvas->setLinePattern(rgba, width, height);
      m_canvas->setLinePatternRotation(angle);
      m_canvas->setLinePatternOpacity(alpha);
    break;

    case te::map::CanvasConfigurer::Point:
      m_canvas->setPointColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT)); // It shouldn't be necessary! (Uba, Feb 2013)
      m_canvas->setPointPattern(rgba, width, height);
      m_canvas->setPointPatternRotation(angle);
      m_canvas->setPointPatternOpacity(alpha);
    break;
  }
  
  te::common::Free(rgba, height);
}

void te::map::CanvasConfigurer::configDefaultPolygon()
{
  m_canvas->setPolygonContourColor(te::color::RGBAColor(TE_SE_DEFAULT_STROKE_BASIC_COLOR, TE_OPAQUE));
  m_canvas->setPolygonContourWidth(TE_SE_DEFAULT_STROKE_BASIC_WIDTH);

  m_canvas->setPolygonContourDashStyle(te::map::SolidLine);
  
  m_canvas->setPolygonContourCapStyle(te::map::ButtCap);
  m_canvas->setPolygonContourJoinStyle(te::map::MiterJoin);

  m_canvas->setPolygonFillColor(te::color::RGBAColor(TE_SE_DEFAULT_FILL_BASIC_COLOR, TE_OPAQUE));
}

void te::map::CanvasConfigurer::configDefaultLine()
{
  m_canvas->setLineColor(te::color::RGBAColor(TE_SE_DEFAULT_STROKE_BASIC_COLOR, TE_OPAQUE));
  m_canvas->setLineWidth(TE_SE_DEFAULT_STROKE_BASIC_WIDTH);

  m_canvas->setLineDashStyle(te::map::SolidLine);

  m_canvas->setLineCapStyle(te::map::ButtCap);
  m_canvas->setLineJoinStyle(te::map::MiterJoin);
}

void te::map::CanvasConfigurer::configDefaultPoint()
{
  m_canvas->setPointColor(te::color::RGBAColor(0, 0, 0, TE_OPAQUE));
}

void te::map::CanvasConfigurer::configDefaultText()
{
  m_canvas->setTextColor(te::color::RGBAColor(TE_SE_DEFAULT_TEXT_COLOR, TE_OPAQUE));
  m_canvas->setTextPointSize(TE_SE_DEFAULT_FONT_SIZE);
}
