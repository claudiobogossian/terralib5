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
  \file terralib/qt/widgets/WellKnownMarkFactory.cpp

  \brief A concrete factory based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../maptools/Utils.h"
#include "../../../se/Mark.h"
#include "../Utils.h"
#include "WellKnownMarkFactory.h"
#include "Utils.h"

// STL
#include <vector>

// Factory key
std::string te::qt::widgets::WellKnownMarkFactory::sm_factoryKey(""); // Default Mark Factory

// Global factory
te::qt::widgets::WellKnownMarkFactory* te::qt::widgets::WellKnownMarkFactory::sm_factory(0);

// MarkMap::<name->MarkType>
std::map<std::string, te::qt::widgets::WellKnownMarkFactory::MarkType> te::qt::widgets::WellKnownMarkFactory::sm_markMap;

void te::qt::widgets::WellKnownMarkFactory::initialize()
{
  finalize();
  sm_factory = new WellKnownMarkFactory;
}

void te::qt::widgets::WellKnownMarkFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::qt::widgets::WellKnownMarkFactory::~WellKnownMarkFactory()
{
}

te::map::AbstractMarkFactory* te::qt::widgets::WellKnownMarkFactory::build()
{
  return sm_factory;
}

te::color::RGBAColor** te::qt::widgets::WellKnownMarkFactory::create(const te::se::Mark* mark, std::size_t size)
{
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
  te::qt::widgets::Config(m_pen, mark->getStroke());
  te::qt::widgets::Config(m_brush, mark->getFill());

  // Let's draw the mark!
  switch(markType)
  {
    case te::qt::widgets::WellKnownMarkFactory::Square:
      draw(img, m_squarePath);
    break;

    case te::qt::widgets::WellKnownMarkFactory::Circle:
      draw(img, m_circlePath);
    break;

    case te::qt::widgets::WellKnownMarkFactory::Triangle:
      draw(img, m_trianglePath);
    break;

    case te::qt::widgets::WellKnownMarkFactory::Star:
      draw(img, m_starPath);
    break;

    case te::qt::widgets::WellKnownMarkFactory::Cross:
      draw(img, m_crossPath);
    break;

    case te::qt::widgets::WellKnownMarkFactory::X:
      draw(img, m_xPath);
    break;

    case te::qt::widgets::WellKnownMarkFactory::Diamond:
      draw(img, m_diamondPath);
    break;

    case te::qt::widgets::WellKnownMarkFactory::Ellipse:
      draw(img, m_ellipsePath);
    break;

    case te::qt::widgets::WellKnownMarkFactory::Semicircle:
      draw(img, m_semiCirclePath);
    break;

    case te::qt::widgets::WellKnownMarkFactory::Pentagon:
      draw(img, m_pentagonPath);
    break;

    case te::qt::widgets::WellKnownMarkFactory::Hexagon:
      draw(img, m_hexagonPath);
    break;

    case te::qt::widgets::WellKnownMarkFactory::Octagon:
      draw(img, m_octagonPath);
    break;
  }

  // Converts QImage to te::color::RGBA**
  te::color::RGBAColor** rgba = te::qt::widgets::GetImage(img);

  delete img;

  return rgba;
}

void te::qt::widgets::WellKnownMarkFactory::getSupportedMarks(std::vector<std::string>& marks) const
{
  std::map<std::string, MarkType>::const_iterator it;
  for(it = sm_markMap.begin(); it != sm_markMap.end(); ++it)
    marks.push_back(it->first);
}

void te::qt::widgets::WellKnownMarkFactory::buildMaps()
{
  // MarkMap
  sm_markMap["square"    ] = te::qt::widgets::WellKnownMarkFactory::Square;
  sm_markMap["circle"    ] = te::qt::widgets::WellKnownMarkFactory::Circle;
  sm_markMap["triangle"  ] = te::qt::widgets::WellKnownMarkFactory::Triangle;
  sm_markMap["star"      ] = te::qt::widgets::WellKnownMarkFactory::Star;
  sm_markMap["cross"     ] = te::qt::widgets::WellKnownMarkFactory::Cross;
  sm_markMap["x"         ] = te::qt::widgets::WellKnownMarkFactory::X;
  sm_markMap["diamond"   ] = te::qt::widgets::WellKnownMarkFactory::Diamond;
  sm_markMap["ellipse"   ] = te::qt::widgets::WellKnownMarkFactory::Ellipse;
  sm_markMap["semicircle"] = te::qt::widgets::WellKnownMarkFactory::Semicircle;
  sm_markMap["pentagon"  ] = te::qt::widgets::WellKnownMarkFactory::Pentagon;
  sm_markMap["hexagon"   ] = te::qt::widgets::WellKnownMarkFactory::Hexagon;
  sm_markMap["octagon"   ] = te::qt::widgets::WellKnownMarkFactory::Octagon;
}

void te::qt::widgets::WellKnownMarkFactory::buildPaths()
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

  // Diamond
  transform.reset();
  m_diamondPath = transform.rotate(-45).scale(0.8, 0.8).map(m_squarePath);

  // Ellipse
  m_ellipsePath.addEllipse(-0.5, -0.3, 1.0, 0.7);

  // Semicircle
  m_semiCirclePath.arcTo(QRectF(-0.5, -0.5, 1.0, 1.0), 0.0, 180.0);
  m_semiCirclePath.closeSubpath();

  // Pentagon
  transform.reset();
  m_pentagonPath.moveTo(-0.6, -0.75);
  m_pentagonPath.lineTo(-1.0, 0.50);
  m_pentagonPath.lineTo(0.0, 1.25);
  m_pentagonPath.lineTo(1.0, 0.50);
  m_pentagonPath.lineTo(0.6, -0.75);
  m_pentagonPath.closeSubpath();
  m_pentagonPath.translate(0, -0.25);
  m_pentagonPath = transform.scale(0.5, 0.5).rotate(180).map(m_pentagonPath);

  // Hexagon
  transform.reset();
  m_hexagonPath.moveTo(-0.5, -0.5);
  m_hexagonPath.lineTo(-1.0, 0.4);
  m_hexagonPath.lineTo(-0.5, 1.1);
  m_hexagonPath.lineTo(0.5, 1.1);
  m_hexagonPath.lineTo(1.0, 0.4); 
  m_hexagonPath.lineTo(0.5, -0.5);
  m_hexagonPath.closeSubpath();
  m_hexagonPath.translate(0, -0.25);
  m_hexagonPath = transform.scale(0.5, 0.5).rotate(180).map(m_hexagonPath);

  // Octagon
  transform.reset();
  m_octagonPath.moveTo(-0.5, -0.75);
  m_octagonPath.lineTo(-1.0, -0.15);
  m_octagonPath.lineTo(-1.0, 0.75);
  m_octagonPath.lineTo(-0.5, 1.25);
  m_octagonPath.lineTo(0.5, 1.25);
  m_octagonPath.lineTo(1.0, 0.75);
  m_octagonPath.lineTo(1.0, -0.15); 
  m_octagonPath.lineTo(0.5, -0.75);
  m_octagonPath.closeSubpath();
  m_octagonPath.translate(0, -0.25);
  m_octagonPath = transform.scale(0.5, 0.5).rotate(180).map(m_octagonPath);
}

void te::qt::widgets::WellKnownMarkFactory::setup(QImage* img)
{
  m_painter.begin(img);
  m_painter.setRenderHints(QPainter::Antialiasing);
  m_painter.setPen(m_pen);
  m_painter.setBrush(m_brush);
}

void te::qt::widgets::WellKnownMarkFactory::end()
{
  m_painter.end();
  m_pen = QPen(QColor(TE_SE_DEFAULT_STROKE_BASIC_COLOR));
  m_brush = QBrush(QColor(TE_SE_DEFAULT_FILL_BASIC_COLOR), Qt::SolidPattern);
}

void te::qt::widgets::WellKnownMarkFactory::draw(QImage* img, QPainterPath& path)
{
  setup(img);

  // Transformation parameters
  double s = img->width() - m_pen.width() - 1;
  double t = img->width() * 0.5;

  QTransform transform;
  QPainterPath transformedPath = transform.translate(t, t).scale(s, s).map(path);

  m_painter.drawPath(transformedPath);

  end();
}

te::qt::widgets::WellKnownMarkFactory::WellKnownMarkFactory()
  : te::map::AbstractMarkFactory(sm_factoryKey)
{
  m_brush.setStyle(Qt::SolidPattern);
  m_brush.setColor(QColor(TE_SE_DEFAULT_FILL_BASIC_COLOR));
  buildMaps();
  buildPaths();
}
