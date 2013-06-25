/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/explorer/LayerTreeModel.cpp

  \brief This class defines the model used in the Qt Model/View architecture for the tree of layers.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../Exception.h"
#include "AbstractLayerTreeItem.h"
#include "AbstractLayerTreeItemFactory.h"
#include "FolderLayerItem.h"
#include "LayerTreeModel.h"

// Qt
#include <QtGui/QMessageBox>
#include <QtCore/QMimeData>
#include <QtCore/QStringList>

te::qt::widgets::LayerTreeModel::LayerTreeModel(QObject* parent)
  : QAbstractItemModel(parent),
    m_checkable(false)
{
  setSupportedDragActions(Qt::MoveAction);
}

te::qt::widgets::LayerTreeModel::LayerTreeModel(const std::list<te::map::AbstractLayerPtr>& layers, QObject * parent)
  : QAbstractItemModel(parent),
    m_checkable(false)
{
  setSupportedDragActions(Qt::MoveAction);
}

te::qt::widgets::LayerTreeModel::~LayerTreeModel()
{
  te::common::FreeContents(m_items);
}

void te::qt::widgets::LayerTreeModel::set(const std::list<te::map::AbstractLayerPtr>& layers)
{
  beginResetModel();

  te::common::FreeContents(m_items);

  m_items.clear();

  for(std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin(); it != layers.end(); ++it)
  {
    AbstractLayerTreeItem* litem = AbstractLayerTreeItemFactory::make(*it, 0);

    if(litem)
      m_items.push_back(litem);
  }

  endResetModel();
}

bool te::qt::widgets::LayerTreeModel::canFetchMore(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return !m_items.empty();

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  return item->canFetchMore();
}

void te::qt::widgets::LayerTreeModel::fetchMore(const QModelIndex& parent)
{
  if(!parent.isValid())
    return;

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TR_QT_WIDGETS("Invalid data associated to the layer model!"));

  item->fetchMore();
}

int te::qt::widgets::LayerTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
  return 1;
}

int te::qt::widgets::LayerTreeModel::rowCount(const QModelIndex& parent) const
{
  if(!parent.isValid()) // if parent index isnot valid we assume we are asking for root items
    return static_cast<int>(m_items.size());

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TR_QT_WIDGETS("Error: NULL layer item!"));

  return item->children().count();
}

QModelIndex te::qt::widgets::LayerTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if(m_items.empty())
    return QModelIndex();

  if(!parent.isValid())
  {
    if(static_cast<std::size_t>(row) >= m_items.size())
      return QModelIndex();

// row and column is about a top-level item?
    AbstractLayerTreeItem* item = m_items[row];

    return createIndex(row, column, item);
  }

  AbstractLayerTreeItem* parentItem = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  if(parentItem == 0)
    throw Exception(TR_QT_WIDGETS("Invalid data associated to the layer model!"));

  if(row >= parentItem->children().count())
    return QModelIndex();

  AbstractLayerTreeItem* item = dynamic_cast<AbstractLayerTreeItem*>(parentItem->children().at(row));

  if(item == 0)
    throw Exception(TR_QT_WIDGETS("The layer item is not an AbstractLayerTreeItem!"));

  return createIndex(row, column, item);
}

QModelIndex te::qt::widgets::LayerTreeModel::parent(const QModelIndex& index) const
{
  if(!index.isValid())
    return QModelIndex();

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  if(item == 0 || item->parent() == 0)
    return QModelIndex();

  AbstractLayerTreeItem* parentItem = dynamic_cast<AbstractLayerTreeItem*>(item->parent());

  if(parentItem == 0)
    throw Exception(TR_QT_WIDGETS("The layer item is not an AbstractLayerTreeItem!"));

  AbstractLayerTreeItem* grandParentItem = dynamic_cast<AbstractLayerTreeItem*>(parentItem->parent());

  if(grandParentItem == 0)
  {
// the parent is a top level item
    for(std::size_t i = 0; i != m_items.size(); ++i)
    {
      if(m_items[i] == parentItem)
        return createIndex(static_cast<int>(i), index.column(), parentItem);
    }
  }
  else
  {
// the parent has a grandparent
    const QObjectList& items = grandParentItem->children();

    int i = 0;

    for(QObjectList::const_iterator it = items.begin(); it != items.end(); ++it, ++i)
    {
      if((*it) == parentItem)
        return createIndex(i, index.column(), parentItem);
    }
  }

  throw Exception(TR_QT_WIDGETS("Could not determine the layer index in the model!"));
}

Qt::ItemFlags te::qt::widgets::LayerTreeModel::flags(const QModelIndex& index) const
{
  if(!index.isValid())
    return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  if(item == 0)
    throw Exception(TR_QT_WIDGETS("Invalid data associated to the layer model!"));

  return QAbstractItemModel::flags(index) | item->flags();
}

QVariant te::qt::widgets::LayerTreeModel::data(const QModelIndex& index, int role) const
{
  if(!index.isValid())
    return QVariant();

  if(role == Qt::CheckStateRole && !m_checkable)
    return QVariant();

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  if(item == 0)
    return QVariant();

  return item->data(index.column(), role);
}

bool te::qt::widgets::LayerTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if(!index.isValid())
    return false;

  if(role == Qt::CheckStateRole && !m_checkable)
    return false;

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  if(item == 0)
    return false;

  bool retVal;

  retVal = item->setData(index.column(), value, role);

  emit dataChanged(index, index);

  // Emit dataChanged signals for the descendants of this index (if any)
  emitDataChangedForDescendants(index);

    // Emit dataChanged signals for the descendants of this index (if any)
  emitDataChangedForAncestors(index);

  return retVal;
}

bool te::qt::widgets::LayerTreeModel::hasChildren(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return !m_items.empty();

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TR_QT_WIDGETS("Invalid data associated to the layer model!"));

  return item->hasChildren();
}

QStringList te::qt::widgets::LayerTreeModel::mimeTypes() const
{
  QStringList mimes;

  mimes << "application/x-terralib;value=\"DraggedItems\"";

  return mimes;
}

Qt::DropActions te::qt::widgets::LayerTreeModel::supportedDropActions() const
{ 
  return Qt::MoveAction | Qt::CopyAction;
}

QMimeData* te::qt::widgets::LayerTreeModel::mimeData(const QModelIndexList& indexes) const
{
  if(indexes.empty())
    return 0;

  int nItems = indexes.count();
  std::vector<AbstractLayerTreeItem*>* draggedItems = new std::vector<AbstractLayerTreeItem*>;

  for(int i = 0; i < indexes.count(); ++i)
  {
    //AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(indexes.at(i).internalPointer());
    draggedItems->push_back(static_cast<AbstractLayerTreeItem*>(indexes.at(i).internalPointer()));
  }

  QString s;
  s.setNum((qulonglong)draggedItems);

  QByteArray encodedData(s.toStdString().c_str());

  QMimeData* mimeData = new QMimeData();

  mimeData->setData("application/x-terralib;value=\"DraggedItems\"", encodedData);

  return mimeData;
}

bool te::qt::widgets::LayerTreeModel::dropMimeData(const QMimeData* data,
                                                   Qt::DropAction action, 
                                                   int row, int column,
                                                   const QModelIndex& parent)
{
  if(data == 0)
    return false;

  if(action == Qt::IgnoreAction)
    return true;

  if(action != Qt::MoveAction)
    return false;

  if(column > 0)
    return false;

  if(!data->hasFormat("application/x-terralib;value=\"DraggedItems\""))
    return false;

  // Get the item that was dragged
  QString sitem = data->data("application/x-terralib;value=\"DraggedItems\"");

  if(sitem.isEmpty())
    return false;

  qulonglong dataValue = sitem.toULongLong();

  // Get the layer items that were dragged
  bool ret = false;
  std::vector<AbstractLayerTreeItem*>* draggedItems = reinterpret_cast<std::vector<AbstractLayerTreeItem*>*>(dataValue);

  // Set the row and the parent of the item to be inserted
  int insertingRow;
  QModelIndex insertingItemParentIndex;

  QModelIndex dropIndex = parent; 
  AbstractLayerTreeItem* dropItem = static_cast<AbstractLayerTreeItem*>(dropIndex.internalPointer());

  te::map::AbstractLayer* dropItemLayer = 0;
  if(dropItem)
    dropItemLayer= dropItem->getLayer().get();

  // Check if the drop item is a folder item and if it has no children; in that case,
  // the dragged items will be made children of this folder item
  if(row < 0 && dropItem && dropItemLayer &&
     dropItemLayer->getType() == "FOLDERLAYER" &&
     dropItemLayer->hasChildren() == false)
  {
    for(std::size_t i = 0; i < draggedItems->size(); ++i)
    {
      AbstractLayerTreeItem* draggedItem = draggedItems->operator[](i);
      insertingRow = dropItemLayer->getChildrenCount();
      insertingItemParentIndex = dropIndex;
      ret = insert(draggedItem->getLayer().get(), insertingRow, insertingItemParentIndex);
    }
  }
  else
  {
    for(std::size_t i = 0; i < draggedItems->size(); ++i)
    {
      AbstractLayerTreeItem* draggedItem = draggedItems->operator[](i);
      QModelIndex dropParentIndex = dropIndex.parent();

      if(row < 0)
      {
        if(dropParentIndex == QModelIndex())
        {
          // Drop item is a top level item
          if(dropIndex.row() == -1)
            insertingRow = m_items.size();   // The item was dropped after the last top item
          else        
            insertingRow = dropIndex.row();  // The item was dropped exactly on a top level item

          insertingItemParentIndex = QModelIndex();
        }
        else
        {
          // Drop item is not a top level item
          if(dropIndex.row() == -1)
            row = 0;
          else
            insertingRow = dropIndex.row();  // The item will be inserted in the position of the dropped item.

          insertingItemParentIndex = dropParentIndex;
        }
      }
      else  // row >= 0
      { 
        insertingRow = row + i;

        if(dropParentIndex == QModelIndex())
        {
          // Drop item is a top level item
          if(dropIndex.row() == -1)
            insertingItemParentIndex = QModelIndex();   // The item will be inserted as a top level item in the row-th position
          else
            insertingItemParentIndex = dropIndex;       // The item will be inserted in the row-th position as child of the dropped item
        }
        else
        {
          // Drop item is not a top level item
          if(dropIndex.row() == -1)
            insertingItemParentIndex = dropIndex.parent();
          else
            insertingItemParentIndex = dropIndex;
        }
      }
      ret = insert(draggedItem->getLayer().get(), insertingRow, insertingItemParentIndex);
    }
  }

  return ret;
}

bool te::qt::widgets::LayerTreeModel::insertRows(int row, int count, const QModelIndex& parent)
{
  AbstractLayerTreeItem* parentItem = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());
  std::vector<AbstractLayerTreeItem*> insertingItems;

  beginInsertRows(parent, row, row + count - 1);
  if(!parentItem)
  {
    for(int i = row; i < row + count; ++i)
      insertingItems.push_back(m_items[i]);
  }
  else
  {
    for(int i = row; i < row + count; ++i)
    {
      te::map::AbstractLayer* refLayer = static_cast<te::map::AbstractLayer*>(parentItem->getLayer().get()->getChild(row).get());
      AbstractLayerTreeItem* insertingItem = AbstractLayerTreeItemFactory::make(refLayer, 0);

      // Insert the item into the row-th position
      // Get the children of the tree item and disconnect them from the tree item
      int numChildren = parentItem->children().count();
      QList<QObject*> savedItemsList = parentItem->children();

      for(int i = 0; i < numChildren; ++i)
        savedItemsList.at(i)->setParent(0);

      // Insert the given item into the saved tree
      savedItemsList.insert(row, insertingItem);

      // Reinsert the saved items into the tree
      for(int i = 0; i < savedItemsList.count(); ++i)
        savedItemsList.at(i)->setParent(parentItem);

      insertingItems.push_back(insertingItem);
    }
  }

  endInsertRows();

  // Adjust the visibility of the ascendent layers of the inserted items
  for(std::size_t i = 0; i < insertingItems.size(); ++i)
  {
    insertingItems[i]->getLayer().get()->updateVisibilityOfAncestors();
    emitDataChangedForAncestors(parent);

    // Emit the signal of visibility changed for the ancestors of the new item who had their visibility changed
    std::vector<AbstractLayerTreeItem*> ancestorItems = insertingItems[i]->getAncestors();
    for(std::size_t i = 0; i < ancestorItems.size(); ++i)
    {
      te::map::AbstractLayer* ancestorLayer = ancestorItems[i]->getLayer().get();
      if(ancestorLayer->hasVisibilityChanged())
        emit visibilityChanged(ancestorItems[i]);
    }
  }

  return true;
}

bool te::qt::widgets::LayerTreeModel::removeRows(int row, int count, const QModelIndex& parent)
{
  bool ret = false;

  beginRemoveRows(parent, row, row + count - 1);

  if(!parent.isValid())
  {
    delete m_items[row];
    m_items.erase(m_items.begin() + row);
  }
  else
  {
    // Delete the item that is in the row-th position of the given parent
    QModelIndex removingIndex = parent.child(row, 0);
    AbstractLayerTreeItem* removingItem = static_cast<AbstractLayerTreeItem*>(removingIndex.internalPointer());

    // Get the item associated to the parent
    AbstractLayerTreeItem* removingItemParent = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

    // First, remove the associated layer associated to the item
    te::map::AbstractLayer* removingItemParentLayer = static_cast<te::map::AbstractLayer*>(removingItemParent->getLayer().get());

    removingItemParentLayer->remove(row);

    // Finally, remove the item from the tree
    const QList<QObject*>& childrenList = removingItemParent->children();
    int numChildren = childrenList.count();

    if (numChildren == 0 || row < 0 || row >= numChildren)
      return false;

    removingItem->setParent(0);

    delete removingItem;

    // Emit the dataChanged signal for the ancestors of the parent of the item removed
    emitDataChangedForAncestors(parent);

    // Adjust the parent visibility of the removed layer and emit the signal 
    // of visibility changed, if its visibility has changed
    removingItemParentLayer->updateVisibility();

    if(removingItemParentLayer->hasVisibilityChanged())
      emit visibilityChanged(removingItemParent);

    // Adjust the visibility of the ancestors of the parent of the removed layer
    removingItemParentLayer->updateVisibilityOfAncestors();

    std::vector<AbstractLayerTreeItem*> ancestorItems = removingItemParent->getAncestors();
    for(std::size_t i = 0; i < ancestorItems.size(); ++i)
    {
      te::map::AbstractLayer* ancestorLayer = ancestorItems[i]->getLayer().get();
      if(ancestorLayer->hasVisibilityChanged())
        emit visibilityChanged(ancestorItems[i]);
    }
  }

  ret = true;

  endRemoveRows();

  return ret;
}

void te::qt::widgets::LayerTreeModel::setCheckable(const bool checkable)
{
  m_checkable = checkable;
}

bool te::qt::widgets::LayerTreeModel::isCheckable() const
{
  return m_checkable;
}

void te::qt::widgets::LayerTreeModel::add(const te::map::AbstractLayerPtr& layer)
{
  if(layer.get() == 0)
    return;

  te::map::AbstractLayer* layerParent = static_cast<te::map::AbstractLayer*>(layer.get()->getParent());

  if(!layerParent)
  {
    AbstractLayerTreeItem* layerItem = AbstractLayerTreeItemFactory::make(layer, 0);
    insert(layer, m_items.size(), QModelIndex());
  }
  else
  {
    int insertingRow = layerParent->getChildrenCount();

    // Create a index for the layer parent
    int parentRow = layerParent->getIndex();
    QModelIndex parentIndex = createIndex(parentRow, 0, layerParent);

    insertRows(insertingRow, 1, parentIndex);
  }
}

bool te::qt::widgets::LayerTreeModel::insert(const te::map::AbstractLayerPtr& layer, int row, const QModelIndex& parent)
{
  AbstractLayerTreeItem* itemParent = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  if(!itemParent)
  {
    AbstractLayerTreeItem* newItem = AbstractLayerTreeItemFactory::make(layer, 0);
    m_items.insert(m_items.begin() + row, newItem);
  }
  else
  {
    te::map::AbstractLayer* parentLayerOfNewItem = itemParent->getLayer().get();
    parentLayerOfNewItem->insert(row, layer);
  }

  return insertRows(row, 1, parent);
}

bool te::qt::widgets::LayerTreeModel::remove(AbstractLayerTreeItem* item)
{
  int itemRow;              // The item row
  QModelIndex parentIndex;  // The parent index of the item

  AbstractLayerTreeItem* parentItem = static_cast<AbstractLayerTreeItem*>(item->parent());
  if(!parentItem)
  {
    // The item is a top level item; get its row
    for(std::size_t i = 0; i < m_items.size(); ++i)
    {
      if(m_items[i] == item)
      {
        itemRow = i;
        break;
      }
    }
  }
  else
  {
    itemRow = parentItem->children().indexOf(item);

    QModelIndex itemIndex = createIndex(itemRow, 0, item);
    parentIndex = parent(itemIndex);
  }

  return removeRows(itemRow, 1, parentIndex);
}

void te::qt::widgets::LayerTreeModel::emitDataChangedForDescendants(const QModelIndex& parent)
{
  if(!parent.isValid())
    return;

  int rows = rowCount(parent);

  for(int i = 0; i != rows; ++i)
  {
    QModelIndex idx = index(i, 0, parent);

    emit dataChanged(idx, idx);

    if(hasChildren(idx))
      emitDataChangedForDescendants(idx);
  }
}

void te::qt::widgets::LayerTreeModel::emitDataChangedForAncestors(const QModelIndex& index)
{ 
  QModelIndex ancestorIndex = parent(index);
  if(parent(index).isValid())
  {
    // Emit the dataChanged signal for the ascendants indexes
    while(ancestorIndex.isValid())
    {
      emit dataChanged(ancestorIndex, ancestorIndex);
      ancestorIndex = parent(ancestorIndex);
    }
  }
}
