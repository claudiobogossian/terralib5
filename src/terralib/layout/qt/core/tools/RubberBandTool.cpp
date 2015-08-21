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

// TerraLib
#include "RubberBandTool.h"
#include "../../../../geometry/Envelope.h"
#include "../View.h"
#include "../Scene.h"
#include "../../../core/pattern/singleton/Context.h"

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QRubberBand>

te::layout::RubberBandTool::RubberBandTool(View* view, QObject* parent) :
    AbstractLayoutTool(view, parent),
    m_started(false),
    m_draft(0),
    m_rubberBand(0)
{
  // Setups the rubber band style
  m_pen.setStyle(Qt::DashLine);
  m_pen.setColor(QColor(100, 177, 216));
  m_pen.setWidth(2);
  m_brush.setColor(QColor(100, 177, 216, 80));
}

te::layout::RubberBandTool::~RubberBandTool()
{

}

bool te::layout::RubberBandTool::mousePressEvent(QMouseEvent* e)
{
  if(!m_view)
    return false;

  if(e->button() != Qt::LeftButton)
    return false;

  m_origin = e->pos();
  m_started = true;
  m_rubberBand = new QRubberBand(QRubberBand::Rectangle, m_view->viewport());
  m_rubberBand->setGeometry(QRect(m_origin, QSize()));
  m_rubberBand->show();
  return true;
}

bool te::layout::RubberBandTool::mouseMoveEvent(QMouseEvent* e)
{
  if(!m_view)
    return false;

  if(!m_started)
    return false;
  
  m_rect = QRect(m_origin, e->pos()).normalized();

  if (m_rubberBand)
  {
    m_rubberBand->setGeometry(m_rect.toRect());
  }

  return true;
}

bool te::layout::RubberBandTool::mouseReleaseEvent(QMouseEvent* e)
{
  m_started = false;

  if(!m_view)
    return false;

  if(m_draft)
  {
    delete m_draft;
    m_draft = 0;
  }

  if(m_rubberBand)
  {
    m_rubberBand->hide();
    delete m_rubberBand;
    m_rubberBand=  0;
  }

  // Roll back the default tool cursor
  m_view->viewport()->setCursor(m_cursor);

  if(e->button() != Qt::LeftButton || m_origin.isNull())
    return false;  
  
  return true;
}
