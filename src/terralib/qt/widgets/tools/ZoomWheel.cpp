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
#include "../canvas/MapDisplay.h"
#include "ZoomWheel.h"

// Qt
#include <QPixmap>
#include <QWheelEvent>

te::qt::widgets::ZoomWheel::ZoomWheel(te::qt::widgets::MapDisplay* display, const double& zoomFactor, QObject* parent) 
  : Zoom(display, zoomFactor, In, parent)
{
}

te::qt::widgets::ZoomWheel::~ZoomWheel()
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
      wheelEvent->delta() > 0 ? setZoomType(In) : setZoomType(Out);
      
      QPixmap* draft = m_display->getDraftPixmap();
      draft->fill(Qt::transparent);

      applyZoom();

      return true;
    }

    default:
      return Zoom::eventFilter(watched, e);
  }
}
