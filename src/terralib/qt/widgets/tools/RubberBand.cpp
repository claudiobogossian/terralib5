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
  \file terralib/qt/widgets/tools/RubberBand.cpp

  \brief This is a utility class tool that provides a rectangle that can indicate a boundary.
*/

// TerraLib
#include "../canvas/MapDisplay.h"
#include "RubberBand.h"

// Qt
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

te::qt::widgets::RubberBand::RubberBand(te::qt::widgets::MapDisplay* display, QObject* parent) 
  : AbstractTool(display, parent)
{
  // Setups the rubber band style
  m_pen.setStyle(Qt::SolidLine);
  m_pen.setColor(QColor(100, 177, 216));
  m_pen.setWidth(2);
  m_brush = QColor(100, 177, 216, 80);
}

te::qt::widgets::RubberBand::~RubberBand()
{
}

bool te::qt::widgets::RubberBand::mousePressEvent(QMouseEvent* e)
{
  m_origin = e->pos();
  return true;
}

bool te::qt::widgets::RubberBand::mouseMoveEvent(QMouseEvent* e)
{
  m_rect = QRect(m_origin, e->pos()).normalized();

  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  QPainter painter(draft);
  painter.setPen(m_pen);
  painter.setBrush(m_brush);
  painter.drawRect(m_rect);

  m_display->repaint();

  return true;
}

bool te::qt::widgets::RubberBand::mouseReleaseEvent(QMouseEvent* e)
{
  QPixmap* draft = m_display->getDraftPixmap();
  draft->fill(Qt::transparent);

  m_display->repaint();

  return true;
}
