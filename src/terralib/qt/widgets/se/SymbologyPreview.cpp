/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/SymbologyPreview.cpp

  \brief Static class used to generate preview of Symbology elements.
*/

// TerraLib
#include "../../../geometry/Polygon.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/Point.h"
#include "../../../maptools/CanvasConfigurer.h"
#include "../../../se/Rule.h"
#include "../../../se/Symbolizer.h"
#include "../canvas/Canvas.h"
#include "SymbologyPreview.h"
#include "Symbol.h"

// Qt
#include <QIcon>

// STL
#include <cassert>

QPixmap te::qt::widgets::SymbologyPreview::build(const te::se::Symbolizer* symb, const QSize& size)
{
  double offset = 2.0;

  te::gm::Geometry* geom = 0;
  if(symb->getType() == "PolygonSymbolizer")
  {
    te::gm::Polygon* polygon = new te::gm::Polygon(1, te::gm::PolygonType);
    te::gm::LinearRing* ring = new te::gm::LinearRing(5, te::gm::LineStringType);
    ring->setPoint(0, offset, offset);
    ring->setPoint(1, size.width() - offset, offset);
    ring->setPoint(2, size.width() - offset, size.height() - offset);
    ring->setPoint(3, offset, size.height() - offset);
    ring->setPoint(4, offset, offset);
    polygon->setRingN(0, ring);
    geom = polygon;
  } 
  else if(symb->getType() == "LineSymbolizer")
  {
    te::gm::LineString* line = new te::gm::LineString(2, te::gm::LineStringType);
    line->setPoint(0, offset, size.height() * 0.5);
    line->setPoint(1, size.width() - offset, size.height() * 0.5);
    geom = line;
  }
  else if(symb->getType() == "PointSymbolizer")
  {
    geom = new te::gm::Point(size.width() * 0.5, size.height() * 0.5);
  }
  else if(symb->getType() == "RasterSymbolizer")
  {
    QIcon raster = QIcon::fromTheme("raster-symbolizer");
    return raster.pixmap(size);
  }

  QPixmap result = build(symb, geom, size);

  delete geom;

  return result;
}

QPixmap te::qt::widgets::SymbologyPreview::build(const std::vector<te::se::Symbolizer*>& symbs, const QSize& size)
{
  QPixmap result(size);
  result.fill(Qt::transparent);

  // Let's draw!
  QPainter painter(&result);
  for(std::size_t i = 0; i < symbs.size(); ++i)
  {
    QPixmap pix = build(symbs[i], size);
    painter.drawPixmap(0, 0, pix);
  }

  return result;
}

QPixmap te::qt::widgets::SymbologyPreview::build(const te::se::Rule* rule, const QSize& size)
{
  assert(rule);

  return build(rule->getSymbolizers(), size);
}

QPixmap te::qt::widgets::SymbologyPreview::build(const te::se::Symbolizer* symb, const te::gm::Geometry* geom, const QSize& size)
{
  assert(symb);

  if(symb->getType() == "RasterSymbolizer")
    return QPixmap();

  assert(geom);
  assert(!size.isEmpty());

  // Creating a canvas...
  te::qt::widgets::Canvas canvas(size.width(), size.height());
  canvas.setWindow(0.0, 0.0, size.width(), size.height());
  canvas.setBackgroundColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT));

  // Configuring...
  te::map::CanvasConfigurer cc(&canvas);
  cc.config(symb);

  // Let's draw!
  canvas.draw(geom);

  return *canvas.getPixmap();
}

QPixmap te::qt::widgets::SymbologyPreview::build(const std::vector<te::se::Symbolizer*>& symbs, const te::gm::Geometry* geom, const QSize& size)
{
  QPixmap result(size);
  result.fill(Qt::transparent);

  // Let's draw!
  QPainter painter(&result);
  for(std::size_t i = 0; i < symbs.size(); ++i)
  {
    QPixmap pix = build(symbs[i], geom, size);
    painter.drawPixmap(0, 0, pix);
  }

  return result;
}

QPixmap te::qt::widgets::SymbologyPreview::build(const te::se::Rule* rule, const te::gm::Geometry* geom, const QSize& size)
{
  assert(rule);

  return build(rule->getSymbolizers(), geom, size);
}

QPixmap te::qt::widgets::SymbologyPreview::build(const te::qt::widgets::Symbol* symbol, const te::gm::Geometry* geom, const QSize& size)
{
  assert(symbol);

  return build(symbol->getSymbolizers(), geom, size);
}
