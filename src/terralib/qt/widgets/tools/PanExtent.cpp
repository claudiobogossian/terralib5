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
  \file terralib/qt/widgets/tools/PanExtent.h

  \brief This class implements a concrete tool to pan a user defined extent.
*/

// TerraLib
#include "../../../geometry/Geometry.h"
#include "../../../geometry/Utils.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "PanExtent.h"

// Qt
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

te::qt::widgets::PanExtent::PanExtent(te::qt::widgets::MapDisplay* display, const QCursor& cursor, const QCursor& actionCursor, QObject* parent) 
  : AbstractTool(display, parent),
    m_panStarted(false),
    m_actionCursor(actionCursor)
{
  setCursor(cursor);

  m_pen.setColor(QColor(0, 0, 0));
  m_pen.setWidth(2);
  m_brush = QColor(233, 88, 63, 80);
}

te::qt::widgets::PanExtent::~PanExtent()
{
}

bool te::qt::widgets::PanExtent::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  m_panStarted = true;

  // Adjusting the action cursor
  if(m_actionCursor.shape() != Qt::BlankCursor)
    m_display->setCursor(m_actionCursor);

  return true;
}

bool te::qt::widgets::PanExtent::mouseMoveEvent(QMouseEvent* e)
{
  if(!m_panStarted)
    return false;

  QPointF p = m_display->transform(e->pos());

  double w = m_currentExtent.getWidth() * 0.5;
  double h = m_currentExtent.getHeight() * 0.5;

  te::gm::Envelope auxExt;

  auxExt.m_llx = p.x() - w;
  auxExt.m_lly = p.y() - h;
  auxExt.m_urx = p.x() + w;
  auxExt.m_ury = p.y() + h;

  setCurrentExtent(auxExt);

  return true;
}

bool te::qt::widgets::PanExtent::mouseReleaseEvent(QMouseEvent* e)
{
  m_panStarted = false;

  //emit signal
  emit extentMoved(m_currentExtent);

  return true;
}

void te::qt::widgets::PanExtent::setCurrentExtent(const te::gm::Envelope& e)
{
  m_currentExtent = e;

  drawCurrentExtent();
}

void te::qt::widgets::PanExtent::drawCurrentExtent()
{
  m_display->getDraftPixmap()->fill(QColor(0, 0, 0, 0).rgba());
  te::qt::widgets::Canvas canvasInstance(m_display->getDraftPixmap());
  const te::gm::Envelope& mapExt = m_display->getExtent();
  canvasInstance.setWindow(mapExt.m_llx, mapExt.m_lly, mapExt.m_urx, mapExt.m_ury);

  canvasInstance.setPolygonContourColor(m_pen.color().rgba());
  canvasInstance.setPolygonContourWidth(m_pen.width());
  canvasInstance.setPolygonFillColor(m_brush.color().rgba());

  te::gm::Geometry* geom = te::gm::GetGeomFromEnvelope(&m_currentExtent, m_display->getSRID());

  canvasInstance.draw(geom);

  m_display->repaint();

  delete geom;
}
