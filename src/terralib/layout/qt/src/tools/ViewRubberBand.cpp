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

// TerraLib
#include "ViewRubberBand.h"
#include "../../../geometry/Envelope.h"
#include "View.h"
#include "Scene.h"
#include "Context.h"

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

te::layout::ViewRubberBand::ViewRubberBand(View* view, QObject* parent) : 
    AbstractViewTool(view, parent),
    m_started(false),
    m_draft(0)
{
  // Setups the rubber band style
  m_pen.setStyle(Qt::DashLine);
  m_pen.setColor(QColor(100, 177, 216));
  m_pen.setWidth(2);
  m_brush = QColor(100, 177, 216, 80);
}

te::layout::ViewRubberBand::~ViewRubberBand()
{

}

bool te::layout::ViewRubberBand::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  m_origin = e->pos();
  m_started = true;
  return true;
}

bool te::layout::ViewRubberBand::mouseMoveEvent(QMouseEvent* e)
{
  if(!m_started)
    return false;
  
  m_rect = QRect(m_origin, e->pos()).normalized();

  m_draft = new QPixmap(m_rect.width(), m_rect.height());
  m_draft->fill(Qt::transparent);

  QPainter painter(m_draft);
  painter.setPen(m_pen);
  painter.setBrush(m_brush);
  painter.drawRect(m_rect);

  return true;
}

bool te::layout::ViewRubberBand::mouseReleaseEvent(QMouseEvent* e)
{
  m_started = false;

  if(m_draft)
  {
    delete m_draft;
    m_draft = 0;
  }

  // Roll back the default tool cursor
  m_view->viewport()->setCursor(m_cursor);

  if(e->button() != Qt::LeftButton || m_origin.isNull())
    return false;  
  
  return true;
}
