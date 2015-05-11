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
  \file terralib/qt/widgets/tools/Pan.cpp

  \brief This class implements a concrete tool to geographic pan operation.
*/

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../canvas/MapDisplay.h"
#include "Pan.h"

// Qt
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

te::qt::widgets::Pan::Pan(te::qt::widgets::MapDisplay* display, const QCursor& cursor, const QCursor& actionCursor, QObject* parent) 
  : AbstractTool(display, parent),
    m_panStarted(false),
    m_actionCursor(actionCursor)
{
  setCursor(cursor);
}

te::qt::widgets::Pan::~Pan()
{
}

bool te::qt::widgets::Pan::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  m_panStarted = true;
  m_origin = e->pos();
  m_delta *= 0;

  // Adjusting the action cursor
  if(m_actionCursor.shape() != Qt::BlankCursor)
    m_display->setCursor(m_actionCursor);

  return true;
}

bool te::qt::widgets::Pan::mouseMoveEvent(QMouseEvent* e)
{
  if(!m_panStarted)
    return false;

  // Calculates the delta value
  m_delta = e->pos() - m_origin;

  // Gets the draft map display pixmap
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill();

  // Gets the current result of map display, i.e. The draw layer composition.
  QPixmap* result = m_display->getDisplayPixmap();

  // Let's draw!
  QPainter painter(draft);
  painter.drawPixmap(0, 0, *result); // Draw the current result.
  painter.save();
  painter.setOpacity(0.3); // Adjusting transparency feedback.
  painter.drawPixmap(m_delta, *result); // Draw the current result translated.
  painter.restore();

  m_display->repaint();

  return true;
}

bool te::qt::widgets::Pan::mouseReleaseEvent(QMouseEvent* e)
{
  m_panStarted = false;

  // Roll back the default tool cursor
  m_display->setCursor(m_cursor);

  if(e->button() != Qt::LeftButton || m_delta.isNull())
    return false;

  // Clears the draft map display pixmap
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  // Calculates the extent translated
  QRect rec(0, 0, m_display->width(), m_display->height());
  QPoint center = rec.center();
  center -= m_delta;
  rec.moveCenter(center);

  // Conversion to world coordinates
  QPointF ll(rec.left(), rec.bottom());
  QPointF ur(rec.right(), rec.top());
  ll = m_display->transform(ll);
  ur = m_display->transform(ur);

  // Updates the map display with the new extent
  te::gm::Envelope envelope(ll.x(), ll.y(), ur.x(), ur.y());
  m_display->setExtent(envelope);

  return true;
}
