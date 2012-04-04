/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file HeaderView.cpp

  \brief It is a reimplementation of the horizontal and vertical QHeaderView of a QTableView.
 */

// TerraLib
#include "../../../maptools/DataGridOperation.h"

#include "HeaderView.h"

// STL
#include <algorithm>

//Qt
#include <QtGui/QApplication>
#include <QtGui/QHeaderView>
#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>

te::qt::widgets::HeaderView::HeaderView(Qt::Orientation orientation, QWidget * parent)
  : m_dataGridOp(0),
    m_resized(false),
    m_visualColumnPressed(-1),
    m_contextVisualColumnClicked(-1),
    m_horizHeaderMenu(0),
    m_vertHeaderMenu(0),
    QHeaderView(orientation, parent)
{
  QHeaderView* headerView = static_cast<QHeaderView*>(this);
  connect(headerView, SIGNAL(sectionResized(int, int, int)), this, SLOT(sectionResized(int, int, int)));
}

te::qt::widgets::HeaderView::~HeaderView()
{
}

void te::qt::widgets::HeaderView::setDataGridOperation(te::map::DataGridOperation* dataGridOp)
{
  m_dataGridOp = dataGridOp;
}

int te::qt::widgets::HeaderView::getContextVisualColumnClicked() const
{
  return m_contextVisualColumnClicked;
}

void te::qt::widgets::HeaderView::setHorizontalHeaderMenu(QMenu* menu)
{
  if (m_horizHeaderMenu)
    delete m_horizHeaderMenu;

  m_horizHeaderMenu = menu;
}

void te::qt::widgets::HeaderView::setVerticalHeaderMenu(QMenu* menu)
{
  if (m_vertHeaderMenu)
    delete m_vertHeaderMenu;

  m_vertHeaderMenu = menu;
}

void te::qt::widgets::HeaderView::sectionResized(int, int, int)
{
  m_resized = true;

  QApplication::restoreOverrideCursor();
}

void te::qt::widgets::HeaderView::mousePressEvent(QMouseEvent* e)
{
  m_resized = false;

  QPoint pos = e->pos();
  if (orientation() == Qt::Horizontal)
  {
    m_visualColumnPressed = visualIndex(logicalIndexAt(pos));

    if (e->button() == Qt::LeftButton)
      QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
  }

  QHeaderView::mousePressEvent(e);
}

void te::qt::widgets::HeaderView::mouseReleaseEvent(QMouseEvent* e)
{
  if (m_resized == true)
  {
    m_resized = false;
    return;
  }

  if (e->button() == Qt::LeftButton)
  {
    QPoint pos = e->pos();
    int section = visualIndex(logicalIndexAt(pos));

    if (section < 0)
    {
      QHeaderView::mouseReleaseEvent(e);
      return;
    }

    if (orientation() == Qt::Horizontal)
    {
      int visualColumnReleased = section;

      if (m_visualColumnPressed == visualColumnReleased)
      {
        emit QHeaderView::sectionClicked(section);

        QApplication::restoreOverrideCursor();

        return QHeaderView::mouseReleaseEvent(e);
      }

      reset();

      emit columnMoved(m_visualColumnPressed, visualColumnReleased);

      m_visualColumnPressed = -1;

      QApplication::restoreOverrideCursor();

      return QHeaderView::mouseReleaseEvent(e);
    }

    emit QHeaderView::sectionClicked(section);
  }

   m_visualColumnPressed = -1;

  QHeaderView::mouseReleaseEvent(e);
}

void te::qt::widgets::HeaderView::contextMenuEvent(QContextMenuEvent* e)
{
  if (e->reason() != QContextMenuEvent::Mouse)
    return QHeaderView::contextMenuEvent(e);

  const QPoint& p = e->pos();
  if (orientation() == Qt::Horizontal)
    m_contextVisualColumnClicked = visualIndex(logicalIndexAt(p));

  if (orientation() == Qt::Horizontal)
  {
    if (m_horizHeaderMenu != 0)
      m_horizHeaderMenu->exec(e->globalPos());
  }
  else
  {
    if (m_vertHeaderMenu != 0)
     m_vertHeaderMenu->exec(e->globalPos());
  }
}
