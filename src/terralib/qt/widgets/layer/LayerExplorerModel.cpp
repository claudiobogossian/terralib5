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
  \file LayerExplorerModel.cpp

  \brief It provides an implementation for an item model classes based on te::qt::widgets::AbstractTreeItem.
 */

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../../maptools/Layer.h"
#include "LayerExplorerModel.h"
#include "LayerItem.h"
#include "LegendItem.h"

// Qt
#include <QtCore/QByteArray>
#include <QtCore/QMimeData>
#include <QtGui/QApplication>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

te::qt::widgets::LayerExplorerModel::LayerExplorerModel(te::map::AbstractLayer* rootLayer, QObject* parent)
  : QAbstractItemModel(parent),
    m_rootItem(0),
    m_dragItem(0),
    m_dndOperation(false)
{
  m_rootItem = new te::qt::widgets::LayerItem(rootLayer, 0);
}

te::qt::widgets::LayerExplorerModel::~LayerExplorerModel()
{
  delete m_rootItem;
}

QModelIndex te::qt::widgets::LayerExplorerModel::index(int row, int column, const QModelIndex& parent) const
{
  if(!hasIndex(row, column, parent))
    return QModelIndex();

  te::qt::widgets::AbstractTreeItem* parentItem = 0;

  if(parent.isValid())
    parentItem = static_cast<te::qt::widgets::AbstractTreeItem*>(parent.internalPointer());
  else
    parentItem = m_rootItem;

  if (parentItem == 0)
    return QModelIndex();

  te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>(parentItem->getChildren().at(row));

  if(item)
    return createIndex(row, column, item);

  return QModelIndex();
}

QModelIndex te::qt::widgets::LayerExplorerModel::parent(const QModelIndex& index) const
{
  if(!index.isValid())
    return QModelIndex();

  te::qt::widgets::AbstractTreeItem* childItem = static_cast<te::qt::widgets::AbstractTreeItem*>(index.internalPointer());

  if (childItem == 0)
    return QModelIndex();

  te::qt::widgets::AbstractTreeItem* parentItem = static_cast<te::qt::widgets::AbstractTreeItem*>(childItem->parent());

  if(parentItem == m_rootItem)
    return QModelIndex();

  if (parentItem == 0)
    return QModelIndex();

  size_t row = parentItem->getIndex();

  return createIndex(row, 0, parentItem);
}

int te::qt::widgets::LayerExplorerModel::rowCount(const QModelIndex& parent) const
{
  if(parent.column() > 0)
    return 0;

  te::qt::widgets::AbstractTreeItem* parentItem = 0;

 if(parent.isValid())
    parentItem = static_cast<te::qt::widgets::AbstractTreeItem*>(parent.internalPointer());
  else
    parentItem = m_rootItem;

 if (parentItem == 0)
   return 0;

  return parentItem->getChildren().count();
}

int te::qt::widgets::LayerExplorerModel::columnCount(const QModelIndex& /*parent*/) const
{
 return 1;
}

QVariant te::qt::widgets::LayerExplorerModel::data(const QModelIndex& index, int role) const
{
  if(!index.isValid())
    return QVariant();

  te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>(index.internalPointer());
  
  if(item == 0)
    return QVariant();

  return item->data(role);
}

bool te::qt::widgets::LayerExplorerModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if((!index.isValid()) || (index.column() > 0))
    return false;

  te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>(index.internalPointer());

  if(item == 0)
    return false;

  if(role == Qt::EditRole)
  {
    item->getRefLayer()->setTitle(value.toString().toStdString());
    emit dataChanged(index, index);
    return true;
  }
  else if(role == Qt::CheckStateRole)
  {
    // Get item visibility
    bool ok = false;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt(&ok));

    if(!ok)
      return false;

    if (item->getRefLayer() == 0)
      return false;

    // Set the reference layer visibility associated to the item
    if(checkState == Qt::Checked)
      item->getRefLayer()->setVisibility(te::map::VISIBLE);
    else if(checkState == Qt::Unchecked)
      item->getRefLayer()->setVisibility(te::map::NOT_VISIBLE);
    else if (checkState == Qt::PartiallyChecked)
      item->getRefLayer()->setVisibility(te::map::PARTIALLY_VISIBLE);

    emit dataChanged(index, index);

    // Emit the dataChanged signal for the ascendants indexes
    QModelIndex ascendentIndex = parent(index);
    while(ascendentIndex.isValid())
    {
      emit dataChanged(ascendentIndex, ascendentIndex);
      ascendentIndex = parent(ascendentIndex);
    }

    // Emit the dataChanged signal for the descendants indexes
    dataChangedForDescendantsIndexes(index);

    return true;
  }

  return true;
}

QVariant te::qt::widgets::LayerExplorerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
  {
    if(section == 0)
    {
      te::qt::widgets::AbstractTreeItem* root = static_cast<te::qt::widgets::AbstractTreeItem*>(m_rootItem);

      if (root == 0 || root->getRefLayer() == 0)
        return QVariant();
      
      return QVariant(root->getRefLayer()->getTitle().c_str());
    }
  }

  return QVariant();
}

Qt::ItemFlags te::qt::widgets::LayerExplorerModel::flags(const QModelIndex &index) const
{
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);

  if(index.isValid())
  {
    if(index == m_dragIndex)
    {
      te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>(index.internalPointer());

      if(item->isLayerItem() == true)     
        flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled |
             Qt::ItemIsUserCheckable;
      else
        flags = Qt::ItemIsEnabled;
    }
    else
      flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled |
             Qt::ItemIsDropEnabled | Qt::ItemIsUserCheckable;
  }

  return flags;
}

Qt::DropActions te::qt::widgets::LayerExplorerModel::supportedDragActions() const
{ 
  return Qt::MoveAction | Qt::CopyAction;
}

Qt::DropActions te::qt::widgets::LayerExplorerModel::supportedDropActions() const
{ 
  return Qt::MoveAction | Qt::CopyAction;
}

QStringList te::qt::widgets::LayerExplorerModel::mimeTypes() const
{
  QStringList types;
  types << "application/layer-explorer";
  return types;
}

QMimeData* te::qt::widgets::LayerExplorerModel::mimeData(const QModelIndexList& indexes) const
{
  m_dragIndex = indexes[0];

  QMimeData *mimeData = new QMimeData();
  te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>(m_dragIndex.internalPointer());
  te::map::AbstractLayer* refLayer = item->getRefLayer();

  QString s;
  s.setNum((qulonglong)refLayer);

  QByteArray encodedData(s.toStdString().c_str());
  mimeData->setData("application/layer-explorer", encodedData);

  return mimeData;
}

bool te::qt::widgets::LayerExplorerModel::dropMimeData(const QMimeData* data, Qt::DropAction action, 
                  int row, int column, const QModelIndex& dropIndex)
{
  m_dndOperation = true;

  if(action == Qt::IgnoreAction)
    return true;

  if(column > 0)
    return false;

  if(!data->hasFormat("application/layer-explorer"))
    return false;

  // Get the drop item
  te::qt::widgets::AbstractTreeItem* dropItem = getItem(dropIndex);

  // Get the drop parent item
  te::qt::widgets::AbstractTreeItem* dropParentItem = static_cast<te::qt::widgets::AbstractTreeItem*>(dropItem->parent());

  if(dropParentItem == 0)
    return false;

  // Get the drag parent item
  te::qt::widgets::AbstractTreeItem* dragParentItem = getItem(m_dragIndex.parent());

  // Get the drag row
  int dragRow = m_dragIndex.row();
  
  // Get the item that was dragged
  m_dragItem = getItem(m_dragIndex);

  if(m_dragItem->isLayerItem() == false)
    return true;

  // Get the drop row
  int dropRow = dropIndex.row();

  // Check if the parent of the drag item is the same as the drop item parent
  if(dragParentItem == dropParentItem)
  {
    if(dragRow == dropRow)
      return true;

    // Remove the dragged item
    beginRemoveRows(m_dragIndex.parent(), dragRow, dragRow);
    dragParentItem->removeChildren(dragRow, 1);
    endRemoveRows();

     // Insert the dragged item at the dropped item position
    insertRows(dropRow, 1, dropIndex.parent());

    emit dragDropEnded(m_dragIndex, dropIndex);
  }

  return true;
}

bool te::qt::widgets::LayerExplorerModel::insertRows(int row, int count, const QModelIndex& parent)
{
  te::qt::widgets::AbstractTreeItem* parentLayerItem = getItem(parent);

  beginInsertRows(parent, row, row + count - 1);
  te::qt::widgets::AbstractTreeItem* layerItem;

  for (int i = 0; i < count; ++i)
  {
    layerItem = m_dragItem;
    parentLayerItem->addChild(row++, layerItem);
  }

  endInsertRows();
  return true;
}

bool	te::qt::widgets::LayerExplorerModel::removeRows(int row, int count, const QModelIndex& parent)
{
  if(m_dndOperation == true)
  {
    m_dndOperation = false;
    return true;
  }

  te::qt::widgets::AbstractTreeItem* parentLayerItem = getItem(parent);
 
  beginRemoveRows(parent, row, row+count-1);
  parentLayerItem->removeChildren(row, count);
  endRemoveRows();
  return true;
}

te::qt::widgets::AbstractTreeItem* te::qt::widgets::LayerExplorerModel::getRootItem() const
{
  return m_rootItem;
}

te::qt::widgets::AbstractTreeItem* te::qt::widgets::LayerExplorerModel::getItem(const QModelIndex& index)
{
  if(index.isValid())
    return static_cast<te::qt::widgets::AbstractTreeItem*>(index.internalPointer());
  else
    return m_rootItem;
}

QModelIndex te::qt::widgets::LayerExplorerModel::getDragIndex() const
{
  return m_dragIndex;
}

void te::qt::widgets::LayerExplorerModel::removeLegend(const QModelIndex& index)
{
  if(!index.isValid())
    return;

  te::qt::widgets::AbstractTreeItem* item = getItem(index);

  if(item->getRefLayer()->getType() != "LAYER")
    return;

  if(item->getRefLayer()->hasLegend() == false)
    return;

  te::qt::widgets::LayerItem* layerItem = static_cast<te::qt::widgets::LayerItem*>(item);

  beginRemoveRows(index, 0, item->children().count() - 1);
  layerItem->removeLegend();
  endRemoveRows();
}

void te::qt::widgets::LayerExplorerModel::addLegend(const QModelIndex& index, const std::vector<te::map::LegendItem*>& legend)
{
  if(!index.isValid())
    return;

  te::qt::widgets::AbstractTreeItem* item = getItem(index);

  if(item->getRefLayer()->getType() != "LAYER")
    return;

  removeLegend(index);

  te::qt::widgets::LayerItem* layerItem = static_cast<te::qt::widgets::LayerItem*>(item);

  beginInsertRows(index, 0, legend.size() - 1);
  layerItem->insertLegend(legend);
  endInsertRows();
}

QModelIndex te::qt::widgets::LayerExplorerModel::insertItem(const QModelIndex& parent, int insertRow, te::map::AbstractLayer* refLayer)
{
  te::qt::widgets::AbstractTreeItem* parentItem = getItem(parent);

  if(parentItem->isLayerItem() == false)
    return QModelIndex();

  te::qt::widgets::LayerItem* layerItem = new te::qt::widgets::LayerItem(refLayer, 0);
  beginInsertRows(parent, insertRow, insertRow);
  parentItem->addChild(insertRow, layerItem);
  endInsertRows();

  QModelIndex newIndex = createIndex(insertRow, 0, layerItem);

  setData(newIndex, 0, Qt::CheckStateRole);

  return newIndex;
}

te::map::AbstractLayer* te::qt::widgets::LayerExplorerModel::removeItem(const QModelIndex& index)
{
  if(!index.isValid())
    return 0;

  te::qt::widgets::AbstractTreeItem* item = getItem(index);

  if(item->isLayerItem() == false)
    return 0;

  te::qt::widgets::LayerItem* layerItem = static_cast<te::qt::widgets::LayerItem*>(item);
  te::qt::widgets::LayerItem* layerParentItem = static_cast<te::qt::widgets::LayerItem*>(layerItem->parent());

  // Adjust the visibility of the folder layers
  setData(index, 0, Qt::CheckStateRole);

  int row = index.row();
  QModelIndex parentIndex = index.parent();
  beginRemoveRows(parentIndex, row, row);
  te::map::AbstractLayer* itemRefLayer = layerParentItem->removeChild(row);
  endRemoveRows();

  return itemRefLayer;
}

void te::qt::widgets::LayerExplorerModel::resetModel()
{
  reset();
}

void te::qt::widgets::LayerExplorerModel::dataChangedForDescendantsIndexes(const QModelIndex& parentIndex)
{
  if (parentIndex.isValid() == false)
    return;

  int rows = rowCount(parentIndex);

  for (int i = 0; i < rows; ++i)
  {
    QModelIndex idx = index(i, 0, parentIndex);

    emit dataChanged(idx, idx);
    dataChangedForDescendantsIndexes(idx);
  }
}
