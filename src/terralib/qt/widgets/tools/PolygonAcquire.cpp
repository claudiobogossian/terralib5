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
  \file terralib/qt/widgets/tools/PolygonAcquire.cpp

  \brief This class implements a concrete tool to acquire a polygon geometry
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/Polygon.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "PolygonAcquire.h"

// Qt
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

// STL
#include <cassert>

te::qt::widgets::PolygonAcquire::PolygonAcquire(te::qt::widgets::MapDisplay* display, QObject* parent) 
  : AbstractTool(display, parent),
    m_isFinished(false)
{
  // Setups the path style
  m_pen.setColor(QColor(0, 0, 0));
  m_pen.setWidth(2);
  m_brush = QColor(233, 88, 63, 80);

  // Signals & slots
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));
}

te::qt::widgets::PolygonAcquire::~PolygonAcquire()
{
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);
}

bool te::qt::widgets::PolygonAcquire::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  if(m_isFinished) // Is Finished?! So, start again...
  {
    clear();
    m_isFinished = false;
  }

  QPointF pw = m_display->transform(e->pos());
  m_coords.push_back(te::gm::Coord2D(pw.x(), pw.y()));

  return true;
}

bool te::qt::widgets::PolygonAcquire::mouseMoveEvent(QMouseEvent* e)
{
  if(m_coords.size() < 1 || m_isFinished)
    return false;

#if QT_VERSION >= 0x050000
  QPointF pw = m_display->transform(e->localPos());
#else
  QPointF pw = m_display->transform(e->posF());
#endif
  m_coords.push_back(te::gm::Coord2D(pw.x(), pw.y()));

#if QT_VERSION >= 0x050000
  QPointF pos = e->localPos() + QPointF(0.0001, 0.0001); // To avoid collinear points on polygon
#else
  QPointF pos = e->posF() + QPointF(0.0001, 0.0001); // To avoid collinear points on polygon
#endif

  pw = m_display->transform(pos);
  m_lastPos = te::gm::Coord2D(pw.x(), pw.y());

  drawGeometry();

  return false;
}

bool te::qt::widgets::PolygonAcquire::mouseReleaseEvent(QMouseEvent* e)
{
  return false;
}

bool te::qt::widgets::PolygonAcquire::mouseDoubleClickEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  if(m_coords.size() < 3) // Can not stop yet...
    return false;

  m_isFinished = true;

  // Build the geometry
  te::gm::LinearRing* ring = new te::gm::LinearRing(m_coords.size() + 1, te::gm::LineStringType);
  for(std::size_t i = 0; i < m_coords.size(); ++i)
    ring->setPoint(i, m_coords[i].x, m_coords[i].y);
  ring->setPoint(m_coords.size(), m_coords[0].x, m_coords[0].y); // Closing...

  te::gm::Polygon* polygon = new te::gm::Polygon(1, te::gm::PolygonType);
  polygon->setRingN(0, ring);
  polygon->setSRID(m_display->getSRID());

  emit polygonAquired(polygon);

  return true;
}

void te::qt::widgets::PolygonAcquire::drawGeometry()
{
  const te::gm::Envelope& env = m_display->getExtent();
  if(!env.isValid())
    return;

  // Clear!
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  // Prepares the canvas
  Canvas canvas(m_display->width(), m_display->height());
  canvas.setDevice(draft, false);
  canvas.setWindow(env.m_llx, env.m_lly, env.m_urx, env.m_ury);
  canvas.setRenderHint(QPainter::Antialiasing, true);

  // Let's draw!
  drawPolygon(canvas);

  m_coords.pop_back();

  m_display->repaint();
}

void te::qt::widgets::PolygonAcquire::drawLine(Canvas& canvas)
{
  // Build the geometry
  te::gm::LineString* line = new te::gm::LineString(m_coords.size(), te::gm::LineStringType);
  for(std::size_t i = 0; i < m_coords.size(); ++i)
    line->setPoint(i, m_coords[i].x, m_coords[i].y);

  // Setup canvas style
  canvas.setLineColor(m_pen.color().rgba());
  canvas.setLineWidth(m_pen.width());

  // Let's draw!
  canvas.draw(line);

  delete line;
}

void te::qt::widgets::PolygonAcquire::drawPolygon(Canvas& canvas)
{
  if(m_coords.size() < 3)
    return drawLine(canvas);

  // Build the geometry
  te::gm::LinearRing* ring = new te::gm::LinearRing(m_coords.size() + 1, te::gm::LineStringType);
  for(std::size_t i = 0; i < m_coords.size(); ++i)
    ring->setPoint(i, m_coords[i].x, m_coords[i].y);
  ring->setPoint(m_coords.size(), m_coords[0].x, m_coords[0].y); // Closing...

  te::gm::Polygon* polygon = new te::gm::Polygon(1, te::gm::PolygonType);
  polygon->setRingN(0, ring);

  // Setup canvas style
  canvas.setPolygonContourColor(m_pen.color().rgba());
  canvas.setPolygonContourWidth(m_pen.width());
  canvas.setPolygonFillColor(m_brush.color().rgba());

  // Let's draw!
  canvas.draw(polygon);

  delete polygon;
}

void te::qt::widgets::PolygonAcquire::clear()
{
  m_coords.clear();

  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);
    
  m_display->repaint();
}

void te::qt::widgets::PolygonAcquire::onExtentChanged()
{
  if(m_coords.empty())
    return;

  m_coords.push_back(m_lastPos);
  drawGeometry();
}
