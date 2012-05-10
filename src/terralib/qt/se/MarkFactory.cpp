/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/MarkFactory.cpp

  \brief This is the concrete factory based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.
 */

// TerraLib
#include "../../common/StringUtils.h"
#include "../../fe/Literal.h"
#include "../../maptools//Utils.h"
#include "../../se/Fill.h"
#include "../../se/Stroke.h"
#include "../../se/SvgParameter.h"
#include "MarkFactory.h"

// STL
#include <cassert>
#include <cstdlib>
#include <vector>

// Factory key
std::string te::qt::MarkFactory::sm_markFactoryKey("DefaultMarkFactory");

// Global Qt4 mark factory.
te::qt::MarkFactory* te::qt::MarkFactory::sm_factory(0);

// MarkMap::<name->MarkType>
std::map<std::string, te::qt::MarkFactory::MarkType> te::qt::MarkFactory::sm_markMap;

// PenCapMap::<type->Qt::PenCapStyle>
std::map<std::string, Qt::PenCapStyle> te::qt::MarkFactory::sm_penCapMap;

// PenJoinMap::<type->Qt::PenJoinStyle>
std::map<std::string, Qt::PenJoinStyle> te::qt::MarkFactory::sm_penJoinMap;

void te::qt::MarkFactory::initialize()
{
  finalize();
  sm_factory = new MarkFactory;
}

void te::qt::MarkFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::MarkFactory::~MarkFactory()
{
}

te::map::AbstractMarkFactory* te::qt::MarkFactory::build()
{
  return sm_factory;
}

te::color::RGBAColor** te::qt::MarkFactory::create(const te::se::Mark* mark, std::size_t size)
{
  assert(mark);

  // Supports the mark?
  const std::string* name = mark->getWellKnownName();
  std::string lname = te::common::Convert2LCase(*name);
  std::map<std::string, MarkType>::iterator it = sm_markMap.find(lname);
  if(it == sm_markMap.end()) // Not recognize mark!
    return 0;

  // Gets the correct mark type from mark name
  MarkType markType = it->second;

  // Creates the image that will represent the graphical mark pattern
  QImage* img = new QImage(size, size, QImage::Format_ARGB32_Premultiplied);
  img->fill(Qt::transparent);

  // Configuring visual...
  config(mark->getStroke());
  config(mark->getFill());

  // Let's draw the mark!
  switch(markType)
  {
    case te::qt::MarkFactory::Square:
      draw(img, m_squarePath);
    break;

    case te::qt::MarkFactory::Circle:
      draw(img, m_circlePath);
    break;

    case te::qt::MarkFactory::Triangle:
      draw(img, m_trianglePath);
    break;

    case te::qt::MarkFactory::Star:
      draw(img, m_starPath);
    break;

    case te::qt::MarkFactory::Cross:
      draw(img, m_crossPath);
    break;

    case te::qt::MarkFactory::X:
      draw(img, m_xPath);
    break;
  }

  // Converts QImage to te::color::RGBA**
  te::color::RGBAColor** rgba = convert(img);

  delete img;

  return rgba;
}

void te::qt::MarkFactory::getSupportedMarks(std::vector<std::string>& marks) const
{
  std::map<std::string, MarkType>::const_iterator it;
  for(it = sm_markMap.begin(); it != sm_markMap.end(); ++it)
    marks.push_back(it->first);
}

void te::qt::MarkFactory::buildMaps()
{
  // MarkMap
  sm_markMap["square"  ] = te::qt::MarkFactory::Square;
  sm_markMap["circle"  ] = te::qt::MarkFactory::Circle;
  sm_markMap["triangle"] = te::qt::MarkFactory::Triangle;
  sm_markMap["star"    ] = te::qt::MarkFactory::Star;
  sm_markMap["cross"   ] = te::qt::MarkFactory::Cross;
  sm_markMap["x"       ] = te::qt::MarkFactory::X;

  // PenCapMap
  sm_penCapMap[TE_SE_BUTT_CAP  ] = Qt::FlatCap;
  sm_penCapMap[TE_SE_ROUND_CAP ] = Qt::RoundCap;
  sm_penCapMap[TE_SE_SQUARE_CAP] = Qt::SquareCap;

  // PenJoinMap
  sm_penJoinMap[TE_SE_MITRE_JOIN] = Qt::MiterJoin;
  sm_penJoinMap[TE_SE_ROUND_JOIN] = Qt::RoundJoin;
  sm_penJoinMap[TE_SE_BEVEL_JOIN] = Qt::BevelJoin;
}

void te::qt::MarkFactory::buildPaths()
{
  // Local transformation matrix
  QTransform transform;

  // Square
  m_squarePath.addRect(-0.5, -0.5, 1.0, 1.0);

  // Circle
  m_circlePath.addEllipse(-0.5, -0.5, 1.0, 1.0);

  // Triangle
  m_trianglePath.moveTo(0.0, 1.25);
  m_trianglePath.lineTo(1.0, -0.75);
  m_trianglePath.lineTo(-1.0, -0.75);
  m_trianglePath.closeSubpath();
  m_trianglePath.translate(0, -0.25);
  m_trianglePath = transform.scale(0.5, 0.5).rotate(180).map(m_trianglePath);

  // Star
  m_starPath.moveTo(0.191, 0.0);
  m_starPath.lineTo(0.25, 0.344);
  m_starPath.lineTo(0.0, 0.588);
  m_starPath.lineTo(0.346, 0.638);
  m_starPath.lineTo(0.5, 1.0);
  m_starPath.lineTo(0.654, 0.638);
  m_starPath.lineTo(1.0, 0.588);
  m_starPath.lineTo(0.75, 0.344);
  m_starPath.lineTo(0.8, 0.0);
  m_starPath.lineTo(0.5, 0.162);
  m_starPath.closeSubpath();
  m_starPath.translate(-0.5, -0.5);
  transform.reset();
  m_starPath = transform.rotate(180).map(m_starPath);

  // Cross
  m_crossPath.moveTo(0.5, 0.125);
  m_crossPath.lineTo(0.125, 0.125);
  m_crossPath.lineTo(0.125, 0.5);
  m_crossPath.lineTo(-0.125, 0.5);
  m_crossPath.lineTo(-0.125, 0.125);
  m_crossPath.lineTo(-0.5, 0.125);
  m_crossPath.lineTo(-0.5, -0.125);
  m_crossPath.lineTo(-0.125, -0.125);
  m_crossPath.lineTo(-0.125, -0.5);
  m_crossPath.lineTo(0.125, -0.5);
  m_crossPath.lineTo(0.125, -0.125);
  m_crossPath.lineTo(0.5, -0.125);
  m_crossPath.closeSubpath();

  // X
  transform.reset();
  m_xPath = transform.rotate(-45).map(m_crossPath);
}

void te::qt::MarkFactory::setup(QImage* img)
{
  m_painter.begin(img);
  m_painter.setRenderHints(QPainter::Antialiasing);
  m_painter.setPen(m_pen);
  m_painter.setBrush(m_brush);
}

void te::qt::MarkFactory::end()
{
  m_painter.end();
  m_pen = QPen(Qt::black);
  m_brush = QBrush(Qt::lightGray, Qt::SolidPattern);
}

void te::qt::MarkFactory::setPenColor(const QColor& color)
{
  m_pen.setColor(color);
}

void te::qt::MarkFactory::setPenWidth(const unsigned int& width)
{
  m_pen.setWidth(width);
}

void te::qt::MarkFactory::setPenStyle(const std::vector<double>& pattern)
{
  QVector<qreal> dasharray;
  for(std::size_t i = 0; i < pattern.size(); ++i)
    dasharray << pattern[i];
  m_pen.setDashPattern(dasharray);
}

void te::qt::MarkFactory::setPenCapStyle(const Qt::PenCapStyle& cap)
{
  m_pen.setCapStyle(cap);
}

void te::qt::MarkFactory::setPenJoinStyle(const Qt::PenJoinStyle& join)
{
  m_pen.setJoinStyle(join);
}

void te::qt::MarkFactory::setBrushColor(const QColor& color)
{
  m_brush.setColor(color);
}

void te::qt::MarkFactory::draw(QImage* img, QPainterPath& path)
{
  setup(img);

  // Transformation parameters
  double s = img->width() - m_pen.width();
  double t = img->width() * 0.5;

  QTransform transform;
  QPainterPath transformedPath = transform.translate(t, t).scale(s, s).map(path);

  m_painter.drawPath(transformedPath);

  end();
}

te::color::RGBAColor** te::qt::MarkFactory::convert(QImage* img)
{
  int size = img->width();
  te::color::RGBAColor** rgba = new te::color::RGBAColor*[size];
  for(int i = 0; i < size; ++i)
  {
    te::color::RGBAColor* s = new te::color::RGBAColor[size];

    for(int j = 0; j < size; ++j)
      s[j] = img->pixel(j, i);

    rgba[i] = s;
  }

  return rgba;
}

void te::qt::MarkFactory::config(const te::se::Stroke* stroke)
{
  if(stroke == 0)
  {
    setPenColor(Qt::transparent); // no stroke
    return;
  }

  // Line Color Alpha
  int alpha = 255;
  const te::se::SvgParameter* opacity = stroke->getOpacity();
  if(opacity)
    alpha = (int)(te::map::GetDouble(opacity) * 255);

  // Line Color
  const te::se::SvgParameter* color = stroke->getColor();
  if(color)
  {
    te::color::RGBAColor rgba = te::map::GetColor(color);
    QColor qrgba(rgba.getRgba());
    qrgba.setAlpha(alpha);
    setPenColor(qrgba);
  }

  // Line Width
  const te::se::SvgParameter* width = stroke->getWidth();
  if(width)
    setPenWidth(te::map::GetInt(width));

  // Line Cap Style
  const te::se::SvgParameter* linecap = stroke->getLineCap();
  if(linecap)
  {
    std::map<std::string, Qt::PenCapStyle>::iterator it = sm_penCapMap.find(te::map::GetString(linecap));
    if(it != sm_penCapMap.end())
      setPenCapStyle(it->second);
  }
  
  // Line Join Style
  const te::se::SvgParameter* linejoin = stroke->getLineJoin();
  if(linejoin)
  {
    std::map<std::string, Qt::PenJoinStyle>::iterator it = sm_penJoinMap.find(te::map::GetString(linejoin));
    if(it != sm_penJoinMap.end())
      setPenJoinStyle(it->second);
  }

  // Line Dash Style
  const te::se::SvgParameter* dasharray = stroke->getDashArray();
  if(dasharray)
  {
    std::string value = te::map::GetString(dasharray);
    std::vector<double> pattern;
    te::map::GetDashStyle(value, pattern);
    setPenStyle(pattern);
  }

  /* TODO: Is necessary verify stroke-dashoffset, Graphic Stroke and Graphic Fill elements here?! */
}

void te::qt::MarkFactory::config(const te::se::Fill* fill)
{
  if(fill == 0)
  {
    setBrushColor(Qt::transparent);
    return;
  }

  int alpha = 255;
  const te::se::SvgParameter* opacity = fill->getOpacity();
  if(opacity)
    alpha = (int)(te::map::GetDouble(opacity) * 255);

  const te::se::SvgParameter* color = fill->getColor();
  if(color)
  {
    te::color::RGBAColor rgba = te::map::GetColor(color);
    QColor qrgba(rgba.getRgba());
    qrgba.setAlpha(alpha);
    setBrushColor(qrgba);
  }
}

te::qt::MarkFactory::MarkFactory()
  : te::map::AbstractMarkFactory(sm_markFactoryKey)
{
  m_brush.setStyle(Qt::SolidPattern);
  buildMaps();
  buildPaths();
}
