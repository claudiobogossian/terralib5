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
#include "ZoomTool.h"
#include "../../../../geometry/Envelope.h"
#include "../View.h"
#include "../Scene.h"

// Qt
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

te::layout::ZoomTool::ZoomTool(View* view, const double& zoomFactor, const ZoomType& type, QObject* parent)
  : AbstractLayoutTool(view, parent),
  m_zoomFactor(zoomFactor),
  m_zoomType(type)
{

}

te::layout::ZoomTool::~ZoomTool()
{
}

void te::layout::ZoomTool::setZoomType(const ZoomType& type)
{
  m_zoomType = type;
}

void te::layout::ZoomTool::applyZoom(const QPointF& point)
{
  if(!m_view)
  {
    return;
  }

  // Calculates the extent translated
  Scene* scne = dynamic_cast<Scene*>(m_view->scene());

  if(!scne)
  {
    return;
  }
  
  //Conversion to world coordinates
  QPointF pt = m_view->mapToScene(point.toPoint());

  int newZoom = 100;
  if(m_zoomType == ZoomIn)
  {
    newZoom = m_view->getCurrentZoom() / m_zoomFactor;
  }
  else
  {
    newZoom = m_view->getCurrentZoom() * m_zoomFactor;
  }

  m_view->setZoom(newZoom);
  m_view->centerOn(pt);

  scne->update();
}
