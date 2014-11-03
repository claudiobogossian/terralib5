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
#include "../../../../geometry/Envelope.h"
#include "../View.h"
#include "../Scene.h"
#include "../../../core/pattern/singleton/Context.h"

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QRubberBand>

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
    
  double zoomFactor = Context::getInstance().getZoomFactor();
  double newZoomFactor = zoomFactor + 0.5;
  Context::getInstance().setZoomFactor(newZoomFactor);

  QRect rec(m_rect.toRect());

  if(m_rect.isNull())
  {
    QRect rect = m_rubberBand->geometry().normalized();

    rec.setRect(0, 0, m_view->width() * 0.5, m_view->height() * 0.5);
    rec.moveCenter(m_origin);
  }

  // Conversion to world coordinates
  QPolygonF poly = m_view->mapToScene(rec);

  // Updates the map display with the new extent
  QRectF bounding = poly.boundingRect();

  te::gm::Envelope* sceneBox = scene->getWorldBox();

  sceneBox->m_llx = bounding.x();
  sceneBox->m_lly = bounding.y();
  sceneBox->m_urx = bounding.x() + bounding.width();
  sceneBox->m_ury = bounding.y() + bounding.height();

  scene->refresh(m_view, newZoomFactor);
  scene->redrawItems(true);
  scene->update();
  
  return true;
}
