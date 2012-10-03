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
  \file terralib/qt/widgets/tools/ZoomWheel.cpp

  \brief This class implements a concrete tool to geographic zoom operation using the mouse wheel.
*/

// TerraLib
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Envelope.h"
#include "MapDisplay.h"
#include "ZoomWheel.h"

// Qt
#include <QtGui/QWheelEvent>

te::qt::widgets::ZoomWheel::ZoomWheel(te::qt::widgets::MapDisplay* display, QObject* parent) 
  : AbstractTool(display, parent)
{
}

te::qt::widgets::ZoomWheel::~ZoomWheel()
{
}

void te::qt::widgets::ZoomWheel::initialize()
{
}

void te::qt::widgets::ZoomWheel::finalize()
{
}

bool te::qt::widgets::ZoomWheel::eventFilter(QObject* watched, QEvent* e)
{
  if(watched != m_display)
    return false;

  switch(e->type())
  {
    case QEvent::Wheel:
    {
      QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(e);
      wheelEvent->delta() > 0 ? performsZoom(0.75) : performsZoom(1.25);

      return true;
    }

    default:
      return AbstractTool::eventFilter(watched, e);
  }
}

void te::qt::widgets::ZoomWheel::performsZoom(const double& t)
{
  const te::gm::Envelope* currentExtent = m_display->getExtent();
  if(currentExtent == 0)
    return;

  te::gm::Coord2D center = currentExtent->getCenter();
  double w = currentExtent->getWidth() * t * 0.5;
  double h = currentExtent->getHeight() * t * 0.5;

  te::gm::Envelope e(center.x - w, center.y - h, center.x + w, center.y + h);
  m_display->setExtent(e);
}
