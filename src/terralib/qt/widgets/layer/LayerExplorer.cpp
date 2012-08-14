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
  \file LayerExplorer.cpp

  \brief It can be used to explore the layers belonging to a project in a QTreeView.
 */

// TerraLib
#include "LayerExplorer.h"

//Qt
#include <QtGui/QApplication>
#include <QtGui/QMenu>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QDragMoveEvent>

#include "../../../maptools/AbstractLayer.h"
#include "AbstractTreeItem.h"
#include "LayerExplorerModel.h"

te::qt::widgets::LayerExplorer::LayerExplorer(QWidget* parent)
  : QTreeView(parent),
    m_previousCurrentIndex(QModelIndex()),
    m_leftMouseButtonWasPressed(false)
{
  connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(pressedItem(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clickedItem(const QModelIndex&)));
}

te::qt::widgets::LayerExplorer::~LayerExplorer()
{
}

void te::qt::widgets::LayerExplorer::setActionsList(const QList<QAction*> actionsList)
{
  m_actionsList = actionsList;
}

void te::qt::widgets::LayerExplorer::updateCurrentIndex(const QModelIndex& dragIndex, const QModelIndex& dropIndex)
{
  setCurrentIndex(dropIndex);
}

void te::qt::widgets::LayerExplorer::pressedItem(const QModelIndex& index)
{
  if(m_mouseButton == Qt::RightButton)
  {
    emit contextMenuPressed(index, m_mousePos);
    m_leftMouseButtonWasPressed = false;
  }
  else
    m_leftMouseButtonWasPressed = true;
}

void te::qt::widgets::LayerExplorer::clickedItem(const QModelIndex& index)
{
  if(m_leftMouseButtonWasPressed == false)
  {
    if(index != m_previousCurrentIndex)
    {
      selectionModel()->select(index, QItemSelectionModel::Deselect);
      selectionModel()->setCurrentIndex(m_previousCurrentIndex, QItemSelectionModel::Select);
    }

    emit checkBoxWasClicked(index); // The checkbox of the item was clicked
  }
  else
  {
    // The item was pressed, its checkbox was not clicked
    if(index == m_previousCurrentIndex)
      selectionModel()->select(index, QItemSelectionModel::Select);
    else
    {
      selectionModel()->select(m_previousCurrentIndex, QItemSelectionModel::Deselect);
      selectionModel()->select(index, QItemSelectionModel::Select);
      m_previousCurrentIndex = index;
    }

    m_leftMouseButtonWasPressed = false;
  }
}

void te::qt::widgets::LayerExplorer::contextMenuEvent(QContextMenuEvent *e)
{
  QModelIndex index = indexAt(e->pos());
  if(index.isValid() == false)
    return;

  QMenu* menu = new QMenu(this);
  for(int i = 0; i < m_actionsList.count(); ++i)
    menu->addAction(m_actionsList[i]);

  menu->exec(QCursor::pos());
}

void te::qt::widgets::LayerExplorer::mousePressEvent(QMouseEvent *e)
{
  m_mouseButton = e->button();
  m_mousePos = e->globalPos();
  QTreeView::mousePressEvent(e);
}

void te::qt::widgets::LayerExplorer::dragMoveEvent(QDragMoveEvent * e)
{
  if(e->possibleActions() == (Qt::MoveAction | Qt::CopyAction))
  {
    Qt::KeyboardModifiers keyboardModifiers = QApplication::keyboardModifiers();
    if(keyboardModifiers == Qt::ControlModifier)
      e->setDropAction(Qt::CopyAction);
    else
      e->setDropAction(Qt::MoveAction);

    e->accept();
  }
}

void te::qt::widgets::LayerExplorer::dragDropEnded(const QModelIndex& dragIndex, const QModelIndex& dropIndex)
{
  setCurrentIndex(dropIndex);
  m_leftMouseButtonWasPressed = false;
}
