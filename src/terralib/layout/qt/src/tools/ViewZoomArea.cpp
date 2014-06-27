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
#include "ViewZoomArea.h"
#include "../../../geometry/Envelope.h"
#include "View.h"
#include "Scene.h"
#include "Context.h"

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

te::layout::ViewZoomArea::ViewZoomArea(View* view, const QCursor& cursor, QObject* parent) 
  : ViewRubberBand(view, parent),
    m_zoomStarted(false)
{
  setCursor(cursor);
}

te::layout::ViewZoomArea::~ViewZoomArea()
{

}

bool te::layout::ViewZoomArea::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

  m_zoomStarted = true;
  m_rect = QRectF();

  return ViewRubberBand::mousePressEvent(e);
}

bool te::layout::ViewZoomArea::mouseMoveEvent(QMouseEvent* e)
{
  if(!m_zoomStarted)
    return false;

  return ViewRubberBand::mouseMoveEvent(e);
}

bool te::layout::ViewZoomArea::mouseReleaseEvent(QMouseEvent* e)
{
  m_zoomStarted = false;

  // Roll back the default tool cursor
  m_view->viewport()->setCursor(m_cursor);

  if(e->button() != Qt::LeftButton)
    return false;

  // Calculates the extent translated
  Scene* scene = dynamic_cast<Scene*>(m_view->scene());

  if(!scene)
    return false;

  ViewRubberBand::mouseReleaseEvent(e);

  if(m_rect.isNull()) // Zoom by click
  {

    QRectF scRct = scene->sceneRect();
    QPolygonF scRctViewport = m_view->mapFromScene(scRct);
    m_rect = QRectF(scRctViewport.boundingRect().topLeft() * 0.5, scRctViewport.boundingRect().bottomRight() * 0.5);
    m_rect.moveCenter(m_origin);
  }

  // Converts zoom boundary to world coordinates
  QPoint ll(m_rect.left(), m_rect.bottom());
  QPoint ur(m_rect.right(), m_rect.top());

  QPointF llworld = m_view->mapToScene(ll);
  QPointF urworld = m_view->mapToScene(ur);

  // Updates the map display with the new extent
  te::gm::Envelope envelope(ll.x(), ll.y(), ur.x(), ur.y());

  te::gm::Envelope* sceneBox = scene->getWorldBox();

  sceneBox->m_llx = ll.x();
  sceneBox->m_lly = ll.y();
  sceneBox->m_urx = ur.x();
  sceneBox->m_ury = ur.y();

  double zoomFactor = Context::getInstance()->getZoomFactor();

  /*scene->refresh(m_view, zoomFactor);
  scene->redrawRulers();
  scene->update();*/
  
  return true;
}
