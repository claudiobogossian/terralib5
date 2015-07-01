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
  \file terralib/qt/widgets/tools/ZoomArea.cpp

  \brief This class implements a concrete tool to geographic zoom in operation using a boundary rectangle.
*/

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../canvas/MapDisplay.h"
#include "ZoomArea.h"

// Qt
#include <QMouseEvent>

te::qt::widgets::ZoomArea::ZoomArea(te::qt::widgets::MapDisplay* display, const QCursor& cursor, QObject* parent) 
  : RubberBand(display, parent),
    m_zoomStarted(false)
{
  setCursor(cursor);
}

te::qt::widgets::ZoomArea::~ZoomArea()
{
}

bool te::qt::widgets::ZoomArea::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  m_zoomStarted = true;
  m_rect = QRectF();

  return RubberBand::mousePressEvent(e);
}

bool te::qt::widgets::ZoomArea::mouseMoveEvent(QMouseEvent* e)
{
  if(!m_zoomStarted)
    return false;

  return RubberBand::mouseMoveEvent(e);
}

bool te::qt::widgets::ZoomArea::mouseReleaseEvent(QMouseEvent* e)
{
  m_zoomStarted = false;

  if(e->button() != Qt::LeftButton)
    return false;

  RubberBand::mouseReleaseEvent(e);

  if(m_rect.isNull()) // Zoom by click
  {
    QRect displayRect = m_display->rect();
    m_rect = QRectF(displayRect.topLeft() * 0.5, displayRect.bottomRight() * 0.5);
    m_rect.moveCenter(m_origin);
  }

  // Converts zoom boundary to world coordinates
  QPointF ll(m_rect.left(), m_rect.bottom());
  QPointF ur(m_rect.right(), m_rect.top());
  ll = m_display->transform(ll);
  ur = m_display->transform(ur);

  // Updates the map display with the new extent
  te::gm::Envelope envelope(ll.x(), ll.y(), ur.x(), ur.y());
  m_display->setExtent(envelope);

  return true;
}
