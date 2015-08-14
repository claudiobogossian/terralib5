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
#include "AbstractLayoutTool.h"
#include "../View.h"

// Qt
#include <QtGui/QMouseEvent>
#include <QSize>
#include <QIcon>
#include <QPixmap>

// STL
#include <cassert>

te::layout::AbstractLayoutTool::AbstractLayoutTool(View* view, QObject* parent)
  : QObject(parent),
    m_view(view),
    m_cursor(Qt::BlankCursor)
{
  assert(m_view);
}

te::layout::AbstractLayoutTool::~AbstractLayoutTool()
{
  // Roll back the default map display cursor
  m_view->viewport()->setCursor(Qt::ArrowCursor);
}

bool te::layout::AbstractLayoutTool::eventFilter(QObject* watched, QEvent* e)
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
        m_view->viewport()->setCursor(m_cursor);
      return false;
    }

    default:
      return QObject::eventFilter(watched, e);
  }
}

bool te::layout::AbstractLayoutTool::mousePressEvent(QMouseEvent* e)
{
  return false;
}

bool te::layout::AbstractLayoutTool::mouseMoveEvent(QMouseEvent* e)
{
  return false;
}

bool te::layout::AbstractLayoutTool::mouseReleaseEvent(QMouseEvent* e)
{
  return false;
}

bool te::layout::AbstractLayoutTool::mouseDoubleClickEvent(QMouseEvent* e)
{
  return false;
}

void te::layout::AbstractLayoutTool::setCursor(const QCursor& cursor)
{
  m_cursor = cursor;
}

QCursor te::layout::AbstractLayoutTool::createCursor(std::string pathIcon)
{
	QIcon ico(QIcon::fromTheme(pathIcon.c_str()));

	//search icon size
	QList<QSize> sizes = ico.availableSizes();
	int maximum = sizes[0].width();
	for (int i = 1; i < sizes.size(); ++i)
	{
		maximum = qMax(maximum, sizes[i].width());
	}

	QSize sz(maximum, maximum);
	QPixmap pixmap = ico.pixmap(sz);

	QCursor cur(pixmap);

	return cur;
}
