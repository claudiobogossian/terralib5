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
#include "AbstractViewTool.h"
#include "View.h"

// Qt
#include <QtGui/QMouseEvent>

// STL
#include <cassert>

te::layout::AbstractViewTool::AbstractViewTool(View* view, QObject* parent)
  : QObject(parent),
    m_view(view),
    m_cursor(Qt::BlankCursor)
{
  assert(m_view);
}

te::layout::AbstractViewTool::~AbstractViewTool()
{
  // Roll back the default map display cursor
  m_view->setCursor(Qt::ArrowCursor);
}

bool te::layout::AbstractViewTool::eventFilter(QObject* watched, QEvent* e)
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
        m_view->setCursor(m_cursor);
      return false;
    }

    default:
      return QObject::eventFilter(watched, e);
  }
}

bool te::layout::AbstractViewTool::mousePressEvent(QMouseEvent* e)
{
  return false;
}

bool te::layout::AbstractViewTool::mouseMoveEvent(QMouseEvent* e)
{
  return false;
}

bool te::layout::AbstractViewTool::mouseReleaseEvent(QMouseEvent* e)
{
  return false;
}

bool te::layout::AbstractViewTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  return false;
}

void te::layout::AbstractViewTool::setCursor(const QCursor& cursor)
{
  m_cursor = cursor;
}
