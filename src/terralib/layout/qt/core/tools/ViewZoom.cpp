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
#include "ViewZoom.h"
#include "../../../../geometry/Envelope.h"
#include "../View.h"
#include "../Scene.h"
#include "../../../core/pattern/singleton/Context.h"
 
// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

te::layout::ViewZoom::ViewZoom(View* view, const double& zoomFactor, const ViewZoomType& type, QObject* parent) 
  : AbstractViewTool(view, parent),
  m_zoomFactor(zoomFactor),
  m_zoomType(type)
{

}

te::layout::ViewZoom::~ViewZoom()
{
}

void te::layout::ViewZoom::setZoomType(const ViewZoomType& type)
{
  m_zoomType = type;
}

void te::layout::ViewZoom::applyZoom(const QPointF& point)
{
  if(!m_view)
  {
    return;
  }

  // Calculates the extent translated
  Scene* scene = dynamic_cast<Scene*>(m_view->scene());

  if(!scene)
  {
    return;
  }
  
  QPointF pt = m_view->mapToScene(point.toPoint());

  double zoomFactor = Context::getInstance().getZoomFactor();
  double defaultZoomFactor = Context::getInstance().getDefaultZoomFactor();
  double newZoomFactor = zoomFactor;

  if(zoomFactor > defaultZoomFactor)
  {
    newZoomFactor = zoomFactor - 0.5;
    Context::getInstance().setZoomFactor(newZoomFactor);
  }
  else
  {
    return;
  }
    
  QRect rec(0, 0, m_view->width() * newZoomFactor, m_view->height() * newZoomFactor);
  rec.moveCenter(point.toPoint());

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
}
