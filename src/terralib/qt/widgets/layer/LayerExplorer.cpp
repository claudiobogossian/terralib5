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
#include "AbstractTreeItem.h"
#include "LayerExplorer.h"
#include "LayerExplorerModel.h"
#include "../../../maptools/AbstractLayer.h"

//Qt
#include <QtGui/QApplication>
#include <QtGui/QMenu>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QDragMoveEvent>

te::qt::widgets::LayerExplorer::LayerExplorer(QWidget* parent)
  : QTreeView(parent),
    m_previousCurrentIndex(QModelIndex()),
    m_itemPressedWithLeftButton(false)
{
  connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(pressedItem(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clickedItem(const QModelIndex&)));
}

te::qt::widgets::LayerExplorer::LayerExplorer(LayerExplorerModel* model, QWidget* parent)
  : QTreeView(parent),
    m_previousCurrentIndex(QModelIndex()),
    m_itemPressedWithLeftButton(false)
{
  connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(pressedItem(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clickedItem(const QModelIndex&)));

  setModel(model);

  if(model)
    connect(model, SIGNAL(dragDropEnded(const QModelIndex&, const QModelIndex&)),
            this, SLOT(dragDropEnded(const QModelIndex&, const QModelIndex&)));
}

te::qt::widgets::LayerExplorer::~LayerExplorer()
{
}

QModelIndex te::qt::widgets::LayerExplorer::getPopupIndex() const
{
  return m_popupIndex;
}

void te::qt::widgets::LayerExplorer::pressedItem(const QModelIndex& index)
{
  m_itemPressedWithLeftButton = true;
}

void te::qt::widgets::LayerExplorer::clickedItem(const QModelIndex& index)
{
  if(m_itemPressedWithLeftButton == false)
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

      te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>(index.internalPointer());
      if(item->isLayerItem() == true)
      {
        selectionModel()->select(index, QItemSelectionModel::Select);
        m_previousCurrentIndex = index;
      }
      else
      {
        selectionModel()->select(index, QItemSelectionModel::Deselect);
        selectionModel()->select(m_previousCurrentIndex, QItemSelectionModel::Select);
      }
    }
  }

  m_itemPressedWithLeftButton = false;
}

void te::qt::widgets::LayerExplorer::mousePressEvent(QMouseEvent *e)
{
  m_itemPressedWithLeftButton = false;

  if(e->button() == Qt::RightButton)
  {
    m_popupIndex = indexAt(e->pos());
    emit contextMenuActivated(m_popupIndex, e->globalPos());
  }

  QTreeView::mousePressEvent(e);
}


void te::qt::widgets::LayerExplorer::dragMoveEvent(QDragMoveEvent* e)
{
  if(e->possibleActions() == (Qt::MoveAction | Qt::CopyAction))
  {
    Qt::KeyboardModifiers keyboardModifiers = QApplication::keyboardModifiers();
    if(keyboardModifiers == Qt::ControlModifier)
      e->setDropAction(Qt::CopyAction);
    else
      e->setDropAction(Qt::MoveAction);

    m_itemPressedWithLeftButton = false;

    te::qt::widgets::LayerExplorerModel* layerExplorerModel = static_cast<te::qt::widgets::LayerExplorerModel*>(model());
    
    QModelIndex dragIndex = layerExplorerModel->getDragIndex();

    te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>(dragIndex.internalPointer());

    if(item != 0 && item->isLayerItem() == true)
      e->accept();
    else
      e->ignore();
  }

  QTreeView::dragMoveEvent(e);
}

void te::qt::widgets::LayerExplorer::dragDropEnded(const QModelIndex& dragIndex, const QModelIndex& dropIndex)
{
  m_itemPressedWithLeftButton = false;
  
  // Update the current index
  int currentRow;

  int dragRow = dragIndex.row();

  int dropRow = dropIndex.row();
  QModelIndex dropParentIndex = dropIndex.parent();

  int prevCurrentRow = m_previousCurrentIndex.row();
  QModelIndex previousCurrentParentIndex = m_previousCurrentIndex.parent();

  if(dropParentIndex != previousCurrentParentIndex)
  {
    setCurrentIndex(m_previousCurrentIndex);
    return;
  }

  if(prevCurrentRow == -1)
    currentRow = prevCurrentRow;
  else if(dragRow < prevCurrentRow)
  {
    if(dropRow < prevCurrentRow)
      currentRow = prevCurrentRow;
    else if(dropRow >= prevCurrentRow)
      currentRow = prevCurrentRow - 1;
  }
  else if(dragRow == prevCurrentRow)
  {
    currentRow = dropRow;
  }
  else if(dragRow > prevCurrentRow)
  {
    if(dropRow <= prevCurrentRow)
      currentRow = prevCurrentRow + 1;
    else if(dropRow > prevCurrentRow)
      currentRow = prevCurrentRow;
  }

  QModelIndex newCurrentIndex = model()->index(currentRow, 0, previousCurrentParentIndex);
  setCurrentIndex(newCurrentIndex);

  m_previousCurrentIndex = newCurrentIndex;
}
