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
    m_currentItem(0),
    m_itemPressedWithLeftButton(false)
{
  connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(pressedItem(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clickedItem(const QModelIndex&)));
}

te::qt::widgets::LayerExplorer::LayerExplorer(LayerExplorerModel* model, QWidget* parent)
  : QTreeView(parent),
    m_currentItem(0),
    m_itemPressedWithLeftButton(false)
{
  connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(pressedItem(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clickedItem(const QModelIndex&)));

  setModel(model);

  if(model)
    connect(model, SIGNAL(dragDropEnded(te::qt::widgets::AbstractTreeItem*, te::qt::widgets::AbstractTreeItem*)),
            this, SLOT(dragDropEnded(te::qt::widgets::AbstractTreeItem*, te::qt::widgets::AbstractTreeItem*)));
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
    // The checkbox of the item was clicked
    QModelIndex currentIndex = static_cast<te::qt::widgets::LayerExplorerModel*>(model())->getIndex(m_currentItem);
    setCurrentIndex(currentIndex);

    emit checkBoxWasClicked(index); 
  }
  else
  {
    // The item was pressed, its checkbox was not clicked
    te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>(index.internalPointer());
    const std::string itemType = item->getRefLayer()->getType();

    if(item->isLegendItem() == false)
      m_currentItem = item;
    else
    {
      QModelIndex index = static_cast<te::qt::widgets::LayerExplorerModel*>(model())->getIndex(m_currentItem);
      setCurrentIndex(index);
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
    e->accept();
  }

  return QTreeView::dragMoveEvent(e);
}

//void te::qt::widgets::LayerExplorer::dragMoveEvent(QDragMoveEvent* e)
//{
//  if(e->possibleActions() == (Qt::MoveAction | Qt::CopyAction))
//  {
//    Qt::KeyboardModifiers keyboardModifiers = QApplication::keyboardModifiers();
//    if(keyboardModifiers == Qt::ControlModifier)
//      e->setDropAction(Qt::CopyAction);
//    else
//      e->setDropAction(Qt::MoveAction);
//
//    m_itemPressedWithLeftButton = false;
//
//    te::qt::widgets::LayerExplorerModel* layerExplorerModel = static_cast<te::qt::widgets::LayerExplorerModel*>(model());
//    
//    QModelIndex dragIndex = layerExplorerModel->getDragIndex();
//
//    te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>(dragIndex.internalPointer());
//
//    if(item != 0 && item->isLayerItem() == true)
//      e->accept();
//    else
//      e->ignore();
//  }
//  return QTreeView::dragMoveEvent(e);
//}

void te::qt::widgets::LayerExplorer::dragDropEnded(te::qt::widgets::AbstractTreeItem* dragItem, te::qt::widgets::AbstractTreeItem* dragItemOldParent)
{
  m_itemPressedWithLeftButton = false;

  LayerExplorerModel* layerExploreModel = static_cast<LayerExplorerModel*>(model());

  QModelIndex index = layerExploreModel->getIndex(dragItem);
  setCurrentIndex(index);
  m_currentItem = dragItem;
}
