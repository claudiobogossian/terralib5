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

/*!
  \file terralib/qt/widgets/tools/ZoomLeftAndRightClick.cpp

  \brief This class implements a concrete tool to geographic zoom operation using the left and right mouse clicks.
*/

// TerraLib
#include "MapDisplay.h"
#include "ZoomLeftAndRightClick.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Envelope.h"

// Qt
#include <QtGui/QMouseEvent>

te::qt::widgets::ZoomLeftAndRightClick::ZoomLeftAndRightClick(
  te::qt::widgets::MapDisplay* display, const double& zoomFactor,
  QObject* parent) 
  : AbstractTool(display, parent), m_zoomFactor( zoomFactor )
{
}

te::qt::widgets::ZoomLeftAndRightClick::~ZoomLeftAndRightClick()
{
}

bool te::qt::widgets::ZoomLeftAndRightClick::mousePressEvent(QMouseEvent* e)
{
  if(e->button() == Qt::LeftButton)
  {
    // Gets the current display extent
    const te::gm::Envelope* currentExtent = m_display->getExtent();
    if(currentExtent == 0)
      return false; 

    // If point is not null, the zoom extent will be centered on this point. Otherwise, keep the current center.
    te::gm::Coord2D center;
    QPointF point( m_display->transform( e->posF() ) );
    point.isNull() ? center = currentExtent->getCenter() : center = te::gm::Coord2D(point.x(), point.y());

    // Bulding the zoom extent based on zoom factor value and the given point
    double w = currentExtent->getWidth() * ( 1.0 / m_zoomFactor ) * 0.5;
    double h = currentExtent->getHeight() * ( 1.0 / m_zoomFactor ) * 0.5;

    te::gm::Envelope e(center.x - w, center.y - h, center.x + w, center.y + h);

    // Updates the map display with the new extent
    m_display->setExtent(e);
  }
  else if( e->button() == Qt::RightButton)
  {
    // Gets the current display extent
    const te::gm::Envelope* currentExtent = m_display->getExtent();
    if(currentExtent == 0)
      return false; 

    // If point is not null, the zoom extent will be centered on this point. Otherwise, keep the current center.
    te::gm::Coord2D center;
    QPointF point( m_display->transform( e->posF() ) );
    point.isNull() ? center = currentExtent->getCenter() : center = te::gm::Coord2D(point.x(), point.y());

    // Bulding the zoom extent based on zoom factor value and the given point
    double w = currentExtent->getWidth() * m_zoomFactor * 0.5;
    double h = currentExtent->getHeight() * m_zoomFactor * 0.5;

    te::gm::Envelope e(center.x - w, center.y - h, center.x + w, center.y + h);

    // Updates the map display with the new extent
    m_display->setExtent(e);
  }
  
  return false;
}
