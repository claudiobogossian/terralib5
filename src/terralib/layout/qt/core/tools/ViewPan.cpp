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
#include "ViewPan.h"
#include "../../../../geometry/Envelope.h"
#include "../View.h"
#include "../Scene.h"
#include "../../../core/pattern/singleton/Context.h"

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

te::layout::ViewPan::ViewPan(View* view, const QCursor& cursor, const QCursor& actionCursor, QObject* parent) 
  : AbstractViewTool(view, parent),
    m_panStarted(false),
    m_actionCursor(actionCursor)
{
  setCursor(cursor);
}

te::layout::ViewPan::~ViewPan()
{

}

bool te::layout::ViewPan::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  m_panStarted = true;
  m_origin = e->pos();
  m_delta *= 0;

  // Adjusting the action cursor
  if(m_actionCursor.shape() != Qt::BlankCursor)
    m_view->viewport()->setCursor(m_actionCursor);

  return true;
}

bool te::layout::ViewPan::mouseMoveEvent(QMouseEvent* e)
{
  if(!m_panStarted)
    return false;

  // Calculates the delta value
  m_delta = e->pos() - m_origin;
  
  return true;
}

bool te::layout::ViewPan::mouseReleaseEvent(QMouseEvent* e)
{
  m_panStarted = false;

  // Roll back the default tool cursor
  m_view->viewport()->setCursor(m_cursor);

  if(e->button() != Qt::LeftButton || m_delta.isNull())
    return false;

  // Calculates the extent translated
  Scene* scene = dynamic_cast<Scene*>(m_view->scene());

  if(!scene)
    return false;

  QRect rec(0, 0, m_view->width(), m_view->height());
  QPoint center = rec.center();
  center -= m_delta;
  rec.moveCenter(center);
  
  // Conversion to world coordinates
  QPolygonF poly = m_view->mapToScene(rec);

  // Updates the map display with the new extent
  QRectF bounding = poly.boundingRect();

  te::gm::Envelope* sceneBox = scene->getWorldBox();
  
  sceneBox->m_llx = bounding.x();
  sceneBox->m_lly = bounding.y();
  sceneBox->m_urx = bounding.x() + bounding.width();
  sceneBox->m_ury = bounding.y() + bounding.height();

  double zoomFactor = Context::getInstance()->getZoomFactor();

  scene->refresh(m_view, zoomFactor);
  scene->redrawRulers();
  scene->update();
  
  return true;
}
