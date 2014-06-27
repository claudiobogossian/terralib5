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
#include "../../../geometry/Envelope.h"
#include "View.h"
#include "Scene.h"
#include "Context.h"
 
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
  // Gets the current display extent
  //const te::gm::Envelope& currentExtent = m_display->getExtent();
  //if(!currentExtent.isValid())
  //  return;

  //// Adjusting zoom factor based on zoomType
  //double factor = m_zoomFactor;
  //if(m_zoomType == In)
  //  factor = 1 / factor;

  //// If point is not null, the zoom extent will be centered on this point. Otherwise, keep the current center.
  //te::gm::Coord2D center;
  //point.isNull() ? center = currentExtent.getCenter() : center = te::gm::Coord2D(point.x(), point.y());

  //// Bulding the zoom extent based on zoom factor value and the given point
  //double w = currentExtent.getWidth() * factor * 0.5;
  //double h = currentExtent.getHeight() * factor * 0.5;

  //te::gm::Envelope e(center.x - w, center.y - h, center.x + w, center.y + h);

  //// Updates the map display with the new extent
  //m_display->setExtent(e);
}
