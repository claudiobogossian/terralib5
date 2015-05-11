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
  \file terralib/qt/widgets/tools/AbstractTool.cpp

  \brief Abstract tool concept
*/

// TerraLib
#include "../canvas/MapDisplay.h"
#include "AbstractTool.h"

// Qt
#include <QMouseEvent>

// STL
#include <cassert>

te::qt::widgets::AbstractTool::AbstractTool(te::qt::widgets::MapDisplay* display, QObject* parent)
  : QObject(parent),
    m_display(display),
    m_cursor(Qt::BlankCursor)
{
  assert(m_display);
}

te::qt::widgets::AbstractTool::~AbstractTool()
{
  // Roll back the default map display cursor
  m_display->setCursor(Qt::ArrowCursor);
}

bool te::qt::widgets::AbstractTool::eventFilter(QObject* watched, QEvent* e)
{
  switch(e->type())
  {
    case QEvent::MouseButtonPress:
      return mousePressEvent(static_cast<QMouseEvent*>(e));

    case QEvent::MouseMove:
      return mouseMoveEvent(static_cast<QMouseEvent*>(e));

    case QEvent::MouseButtonRelease:
      return mouseReleaseEvent(static_cast<QMouseEvent*>(e));

    case QEvent::MouseButtonDblClick:
      return mouseDoubleClickEvent(static_cast<QMouseEvent*>(e));

    case QEvent::Enter:
    {
      if(m_cursor.shape() != Qt::BlankCursor)
        m_display->setCursor(m_cursor);
      return false;
    }

    default:
      return QObject::eventFilter(watched, e);
  }
}

bool te::qt::widgets::AbstractTool::mousePressEvent(QMouseEvent* e)
{
  return false;
}

bool te::qt::widgets::AbstractTool::mouseMoveEvent(QMouseEvent* e)
{
  return false;
}

bool te::qt::widgets::AbstractTool::mouseReleaseEvent(QMouseEvent* e)
{
  return false;
}

bool te::qt::widgets::AbstractTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  return false;
}

void te::qt::widgets::AbstractTool::setCursor(const QCursor& cursor)
{
  m_cursor = cursor;
}
