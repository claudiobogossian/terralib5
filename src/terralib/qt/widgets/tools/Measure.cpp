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
  \file terralib/qt/widgets/tools/Measure.cpp

  \brief This class implements a concrete tool to measure operation (distance, area, and angle).
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../common/UnitOfMeasure.h"
#include "../../../common/UnitsOfMeasureManager.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/LineString.h"
#include "../../../geometry/Point.h"
#include "../../../geometry/Polygon.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "Measure.h"

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

// STL
#include <cassert>

te::qt::widgets::Measure::Measure(te::qt::widgets::MapDisplay* display, const MeasureType& measureType, const QCursor& cursor, QObject* parent) 
  : AbstractTool(display, parent),
    m_measureType(measureType),
    m_isFinished(false),
    m_unit("")
{
  setCursor(cursor);

  // Setups the path style
  m_pen.setColor(QColor(100, 177, 216));
  m_pen.setWidth(3);
  m_brush = QColor(100, 177, 216, 80);
  
  if (m_display->getSRID()!= TE_UNKNOWN_SRS)
  {
    te::common::UnitOfMeasurePtr unit = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(m_display->getSRID());
    if (unit.get())
      m_unit = unit->getSymbol();
  }
  // Signals & slots
  connect(m_display, SIGNAL(extentChanged()), SLOT(onExtentChanged()));
}

te::qt::widgets::Measure::~Measure()
{
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);
}

bool te::qt::widgets::Measure::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  if(m_isFinished) // Is Finished?! So, start again...
  {
    clear();
    m_isFinished = false;
  }

  // Angle case: the maximum is 2 points collected...
  if(m_coords.size() == 2 && m_measureType == Angle)
    return false; // No collect more!

  QPointF pw = m_display->transform(e->pos());
  m_coords.push_back(te::gm::Coord2D(pw.x(), pw.y()));

  return true;
}

bool te::qt::widgets::Measure::mouseMoveEvent(QMouseEvent* e)
{
  if(m_coords.size() < 1 || m_isFinished)
    return false;

  QPointF pw = m_display->transform(e->posF());
  m_coords.push_back(te::gm::Coord2D(pw.x(), pw.y()));
  
  QPointF pos = e->posF() + QPointF(0.0001, 0.0001); // To avoid collinear points on polygon
  pw = m_display->transform(pos);
  m_lastPos = te::gm::Coord2D(pw.x(), pw.y());

  drawGeometry();

  return false;
}

bool te::qt::widgets::Measure::mouseReleaseEvent(QMouseEvent* e)
{
  return false;
}

bool te::qt::widgets::Measure::mouseDoubleClickEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  if(m_measureType == Area && m_coords.size() < 3) // Can not stop yet...
    return false;

  m_isFinished = true;

  return true;
}

void te::qt::widgets::Measure::drawGeometry()
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
  switch(m_measureType)
  {
    case Distance:
    case Angle:
      drawLine(canvas);
    break;

    case Area:
      drawPolygon(canvas);
    break;
  }

  m_coords.pop_back();

  m_display->repaint();
}

void te::qt::widgets::Measure::drawLine(Canvas& canvas)
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

  // Measuring and feedback...
  if(m_measureType == Distance)
  {
    drawText(canvas, (tr("Distance: ") + QString::number(calculateLength(line)) + " " + m_unit.c_str()).toStdString(), line->getEndPoint());
  }
  else if(m_measureType == Angle)
  {
    if(line->getNPoints() >= 3)
      drawText(canvas, (tr("Angle: ") + QString::number(calculateAngle(line)) + " deg").toStdString(), line->getPointN(1));
  }

  delete line;
}

void te::qt::widgets::Measure::drawPolygon(Canvas& canvas)
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

  // Measuring and feedback...
  const te::gm::Envelope* env = polygon->getMBR();
  te::gm::Point p(env->getCenter().x, env->getCenter().y);
  drawText(canvas, (tr("Area: ") + QString::number(polygon->getArea()) + (m_unit.empty() ? "" : (" " + m_unit + "^2").c_str())).toStdString(), &p);

  delete polygon;
}

void te::qt::widgets::Measure::drawText(Canvas& canvas, const std::string& text, te::gm::Point* p)
{
  // Testing...
  canvas.setTextColor(te::color::RGBAColor(0, 0, 0, TE_OPAQUE));
  canvas.setFontFamily("Arial");
  canvas.setTextWeight(te::at::Bold);
  canvas.setTextPointSize(9);
  canvas.setTextContourEnabled(true);
  canvas.setTextContourWidth(3);
  canvas.setTextContourColor(te::color::RGBAColor(255, 255, 255, TE_OPAQUE));
  canvas.drawText(p, text);
}

void te::qt::widgets::Measure::clear()
{
  m_coords.clear();

  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);
    
  m_display->repaint();
}

double te::qt::widgets::Measure::calculateLength(te::gm::LineString* line) const
{
  double length = 0.0;
  std::size_t n = line->getNPoints();
  for(std::size_t i = 0; i < n - 1; ++i)
    length += line->getPointN(i)->distance( line->getPointN(i + 1));

  return length;
}

double te::qt::widgets::Measure::calculateAngle(te::gm::LineString* line) const
{
  std::size_t n = line->getNPoints();
  assert(n >= 3);

  te::gm::Point* p1 = line->getPointN(0);
  te::gm::Point* p2 = line->getPointN(1);
  te::gm::Point* p3 = line->getPointN(2);

  QLineF line1(p2->getX(), p2->getY(), p1->getX(), p1->getY());
  QLineF line2(p2->getX(), p2->getY(), p3->getX(), p3->getY());

  return line1.angle(line2);
}

void te::qt::widgets::Measure::onExtentChanged()
{
  if (m_display->getSRID()!= TE_UNKNOWN_SRS)
  {
    te::common::UnitOfMeasurePtr unit = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(m_display->getSRID());
    if (unit.get())
      m_unit = unit->getSymbol();
    else
      m_unit = "";
  }
  else
    m_unit = "";
  
  if(m_coords.empty())
    return;

  m_coords.push_back(m_lastPos);
  drawGeometry();
}
