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
#include "ViewZoomArea.h"
#include "../../../../geometry/Envelope.h"
#include "../View.h"
#include "../Scene.h"

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

  if(!m_view)
    return false;

  //Roll back the default tool cursor
  m_view->viewport()->setCursor(m_cursor);

  if(e->button() != Qt::LeftButton)
    return false;

  Scene* scne = dynamic_cast<Scene*>(m_view->scene());

  if(!scne)
    return false;

  if(m_rubberBand == 0)
  {
    return false;
  }

  QRect rect = m_rubberBand->geometry().normalized();

  ViewRubberBand::mouseReleaseEvent(e);

  //Conversion to world coordinates
  QPolygonF poly = m_view->mapToScene(rect);

  //Updates 
  QRectF bounding = poly.boundingRect();
  if(bounding.width() == 0. || bounding.height() == 0.)
  {
    View* view = dynamic_cast<View*>(m_view);
    if(view)
    {
      int newZoom = view->getCurrentZoom() * 1.5;
      view->setZoom(newZoom);
      return true;
    }
    return false;
  }
  
  /*
  Zoom In Area:
  Scales the view matrix. The view is scaled according to aspectRatioMode.
  Ensure that the scene rectangle rect fits inside the viewport
  */
  m_view->fitZoom(bounding);

  scne->update();

  return true;
}
