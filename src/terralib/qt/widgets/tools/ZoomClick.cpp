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
  \file terralib/qt/widgets/tools/ZoomClick.cpp

  \brief This class implements a concrete tool to geographic zoom operation using the mouse click.
*/

// TerraLib
#include "../canvas/MapDisplay.h"
#include "ZoomClick.h"

// Qt
#include <QMouseEvent>

te::qt::widgets::ZoomClick::ZoomClick(te::qt::widgets::MapDisplay* display, const QCursor& cursor, const double& zoomFactor, const ZoomType& type, QObject* parent) 
  : Zoom(display, zoomFactor, type, parent)
{
  setCursor(cursor);
}

te::qt::widgets::ZoomClick::~ZoomClick()
{
}

bool te::qt::widgets::ZoomClick::mousePressEvent(QMouseEvent* e)
{
  if(e->button() != Qt::LeftButton)
    return false;

#if QT_VERSION >= 0x050000
  applyZoom(m_display->transform(e->localPos()));
#else
  applyZoom(m_display->transform(e->posF()));
#endif

  return true;
}
