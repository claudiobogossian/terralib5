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
  \file terralib/qt/widgets/tools/CoordTracking.cpp

  \brief This class implements a concrete tool to geographic coordinate tracking on mouse move operation.
*/

// TerraLib
#include "../canvas/MapDisplay.h"
#include "CoordTracking.h"

// Qt
#include <QMouseEvent>

te::qt::widgets::CoordTracking::CoordTracking(te::qt::widgets::MapDisplay* display, QObject* parent) 
  : AbstractTool(display, parent)
{
  m_display->setMouseTracking(true);
}

te::qt::widgets::CoordTracking::~CoordTracking()
{
}

bool te::qt::widgets::CoordTracking::mouseMoveEvent(QMouseEvent* e)
{
#if (QT_VERSION >= 0x050000)
  QPointF p = e->localPos();
#else
  QPointF p = e->posF();
#endif
  
  p = m_display->transform(p);
  if(!p.isNull())
    emit coordTracked(p);

  return false;
}
