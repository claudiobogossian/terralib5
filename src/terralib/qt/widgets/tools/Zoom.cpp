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

/*!
  \file terralib/qt/widgets/tools/Zoom.cpp

  \brief This is a utility class to geographic zoom operation.
*/

// TerraLib
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Envelope.h"
#include "../canvas/MapDisplay.h"
#include "Zoom.h"

te::qt::widgets::Zoom::Zoom(te::qt::widgets::MapDisplay* display, const double& zoomFactor, const ZoomType& type, QObject* parent) 
  : AbstractTool(display, parent),
    m_zoomFactor(zoomFactor),
    m_zoomType(type)
{
}

te::qt::widgets::Zoom::~Zoom()
{
}

void te::qt::widgets::Zoom::setZoomType(const ZoomType& type)
{
  m_zoomType = type;
}

void te::qt::widgets::Zoom::applyZoom(const QPointF& point, bool centralize)
{
  // Gets the current display extent
  const te::gm::Envelope& currentExtent = m_display->getExtent();
  if(!currentExtent.isValid())
    return;

  // Adjusting zoom factor based on zoomType
  double factor = m_zoomFactor;
  if(m_zoomType == In)
    factor = 1 / factor;

  // If point is not null, the zoom extent will be centered on this point. Otherwise, keep the current center.
  te::gm::Envelope extent;
  if(point.isNull() == true)
  {
    te::gm::Coord2D center;
    point.isNull() ? center = currentExtent.getCenter() : center = te::gm::Coord2D(point.x(), point.y());
  
    // Bulding the zoom extent based on zoom factor value and the given point
    double w = currentExtent.getWidth() * factor * 0.5;
    double h = currentExtent.getHeight() * factor * 0.5;

    extent.init(center.x - w, center.y - h, center.x + w, center.y + h);
  }
  else
  {
    te::gm::Coord2D reference = te::gm::Coord2D(point.x(), point.y());
    
    double dxPercentage = (point.x() - currentExtent.getLowerLeftX()) / currentExtent.getWidth();
    double dyPercentage = (point.y() - currentExtent.getLowerLeftY()) / currentExtent.getHeight();

    // Bulding the zoom extent based on zoom factor value and the given point
    double w = currentExtent.getWidth() * factor;
    double h = currentExtent.getHeight() * factor;

    double leftWidth = w * dxPercentage;
    double rightWidth = w * (1. - dxPercentage);

    double lowerHeight = h * dyPercentage;
    double upperHeight = h * (1. - dyPercentage);

    extent.init(reference.x - leftWidth, reference.y - lowerHeight, reference.x + rightWidth, reference.y + upperHeight);
  }

  // Updates the map display with the new extent
  m_display->setExtent(extent);
}
