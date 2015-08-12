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
  \file terralib/qt/widgets/layer/explorer/LayerTreeModel.cpp

  \brief This class defines the model used in the Qt Model/View architecture for the tree of layers.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../Exception.h"
#include "AbstractTreeItem.h"
#include "AbstractTreeItemFactory.h"
#include "FolderLayerItem.h"
#include "LayerTreeModel.h"

// Qt
#include <QMessageBox>
#include <QMimeData>
#include <QStringList>

te::qt::widgets::LayerTreeModel::LayerTreeModel(QObject* parent)
  : QAbstractItemModel(parent),
    m_checkable(false)
{
#if (QT_VERSION < 0x050000)
  setSupportedDragActions(Qt::MoveAction | Qt::CopyAction  | Qt::LinkAction);
#endif
}

te::qt::widgets::LayerTreeModel::LayerTreeModel(const std::list<te::map::AbstractLayerPtr>& layers, QObject * parent)
  : QAbstractItemModel(parent),
    m_checkable(false)
{
#if (QT_VERSION < 0x050000)
  setSupportedDragActions(Qt::MoveAction | Qt::CopyAction | Qt::LinkAction);
#endif
}

te::qt::widgets::LayerTreeModel::~LayerTreeModel()
{
//  te::common::FreeContents(m_items);
}

void te::qt::widgets::LayerTreeModel::set(const std::list<te::map::AbstractLayerPtr>& layers)
{
  beginResetModel();

  te::common::FreeContents(m_items);

  m_items.clear();

  m_layers.clear();

  for(std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin(); it != layers.end(); ++it)
  {
    AbstractTreeItem* litem = AbstractTreeItemFactory::make(*it, 0);

    if(litem)
      m_items.push_back(litem);

    m_layers.push_back(*it);
  }

  endResetModel();
}

const std::vector<te::map::AbstractLayerPtr>& te::qt::widgets::LayerTreeModel::getTopLayers() const
{
  return m_layers;
}

const std::vector<te::qt::widgets::AbstractTreeItem*>& te::qt::widgets::LayerTreeModel::getTopLayerItems() const
{
  return m_items;
}

bool te::qt::widgets::LayerTreeModel::canFetchMore(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return !m_items.empty();

  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(parent.internalPointer());

  return item->canFetchMore();
}

void te::qt::widgets::LayerTreeModel::fetchMore(const QModelIndex& parent)
{
  if(!parent.isValid())
    return;

  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TE_TR("Invalid data associated to the layer model!"));

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

  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TE_TR("Error: NULL layer item!"));

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
    AbstractTreeItem* item = m_items[row];

    return createIndex(row, column, item);
  }

  AbstractTreeItem* parentItem = static_cast<AbstractTreeItem*>(parent.internalPointer());

  if(parentItem == 0)
    throw Exception(TE_TR("Invalid data associated to the layer model!"));

  if(row >= parentItem->children().count())
    return QModelIndex();

  AbstractTreeItem* item = dynamic_cast<AbstractTreeItem*>(parentItem->children().at(row));

  if(item == 0)
    throw Exception(TE_TR("The layer item is not an AbstractTreeItem!"));

  return createIndex(row, column, item);
}

QModelIndex te::qt::widgets::LayerTreeModel::parent(const QModelIndex& index) const
{
  if(!index.isValid())
    return QModelIndex();

  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());

  if(item == 0 || item->parent() == 0)
    return QModelIndex();

  AbstractTreeItem* parentItem = dynamic_cast<AbstractTreeItem*>(item->parent());

  if(parentItem == 0)
    throw Exception(TE_TR("The layer item is not an AbstractTreeItem!"));

  AbstractTreeItem* grandParentItem = dynamic_cast<AbstractTreeItem*>(parentItem->parent());

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

  throw Exception(TE_TR("Could not determine the layer index in the model!"));
}

Qt::ItemFlags te::qt::widgets::LayerTreeModel::flags(const QModelIndex& index) const
{
  if(!index.isValid())
    return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());

  if(item == 0)
    throw Exception(TE_TR("Invalid data associated to the layer model!"));

  return QAbstractItemModel::flags(index) | item->flags();
}

QVariant te::qt::widgets::LayerTreeModel::data(const QModelIndex& index, int role) const
{
  if(!index.isValid())
    return QVariant();

  if(role == Qt::CheckStateRole && !m_checkable)
    return QVariant();

  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());

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

  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(index.internalPointer());

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

  AbstractTreeItem* item = static_cast<AbstractTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TE_TR("Invalid data associated to the layer model!"));

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
  //return Qt::MoveAction | Qt::CopyAction;
  return Qt::MoveAction | Qt::CopyAction | Qt::LinkAction;
}

#if (QT_VERSION >= 0x050000)
Qt::DropActions te::qt::widgets::LayerTreeModel::supportedDragActions() const
{
  return Qt::MoveAction | Qt::CopyAction | Qt::LinkAction;
}
#endif

QMimeData* te::qt::widgets::LayerTreeModel::mimeData(const QModelIndexList& indexes) const
{
  if(indexes.empty())
    return 0;
  
  std::vector<AbstractTreeItem*>* draggedItems = new std::vector<AbstractTreeItem*>;

  for(int i = 0; i < indexes.count(); ++i)
    draggedItems->push_back(static_cast<AbstractTreeItem*>(indexes.at(i).internalPointer()));

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
  if(data == 0 || column > 0 || action != Qt::MoveAction)
    return false;

  if(action == Qt::IgnoreAction)
    return true;

  if(!data->hasFormat("application/x-terralib;value=\"DraggedItems\""))
    return false;

  // Get the item that was dragged
  QString sitem = data->data("application/x-terralib;value=\"DraggedItems\"");

  if(sitem.isEmpty())
    return false;

  qulonglong dataValue = sitem.toULongLong();

  // Get the layer items that were dragged
  std::vector<AbstractTreeItem*>* draggedItems = reinterpret_cast<std::vector<AbstractTreeItem*>*>(dataValue);
  int count = draggedItems->size();

  m_insertingLayers.clear();
  for(int i = 0; i < count; ++i)
    m_insertingLayers.push_back(draggedItems->operator[](i)->getLayer());

  // Get the drop item and its associated layer
  QModelIndex dropIndex = parent; 
  AbstractTreeItem* dropItem = static_cast<AbstractTreeItem*>(dropIndex.internalPointer());

  te::map::AbstractLayerPtr dropLayer = 0;
  if(dropItem)
    dropLayer= dropItem->getLayer();

  // Get the parent index of the dropped item and its associated layer
  QModelIndex dropParentIndex = dropIndex.parent();
  AbstractTreeItem* dropParentItem = static_cast<AbstractTreeItem*>(dropParentIndex.internalPointer());

  te::map::AbstractLayerPtr dropParentLayer = 0;
  if(dropParentItem)
    dropParentLayer = dropParentItem->getLayer();

  // Set the row and the parent where the dragged items will be inserted
  int insertingRow;
  QModelIndex insertingItemParentIndex;

  removeLayerFromParentChildrenList(m_insertingLayers);

  // Check if the drop item is a folder item and if it has no children; in that case,
  // the dragged items will be made children of this folder item
  //bool folderItemWasEmpty = false;

  if(row < 0 && dropItem && dropLayer.get() &&
     dropLayer->getType() == "FOLDERLAYER" &&
     dropLayer->hasChildren() == false)
  {
    insertingRow = 0;
    insertingItemParentIndex = dropIndex;

    // Insert the dragged layers into its new parent
    for(int i = 0; i < count; ++i)
      dropLayer->insert(i, m_insertingLayers[i]);

    //folderItemWasEmpty = true;
  }
  else
  {
    // Get the index of the parent of the dropped item
    QModelIndex dropParentIndex = dropIndex.parent();

    if(row < 0)
    {
      // The dragged items were dragged exactly on the dropped item
      if(dropParentIndex == QModelIndex())
      {
        // Drop item is a top level item
        if(dropIndex.row() == -1)
          insertingRow = m_items.size();   // The items were dropped after the last top level item
        else
          insertingRow = dropIndex.row();  // The item was dropped exactly on a top level item

        // Insert the dragged layers into its new parent
        if(insertingRow >= (int)m_layers.size())
        {
          for(int i = 0; i < count; ++i)
            m_layers.push_back(m_insertingLayers[i]);
        }
        else
        {
          for(int i = 0; i < count; ++i)
            m_layers.insert(m_layers.begin() + insertingRow + i, m_insertingLayers[i]);
        }
      }
      else
      {
        // Drop item is not a top level item
        insertingRow = dropIndex.row();  // The item will be inserted in the position of the dropped item.
        insertingItemParentIndex = dropParentIndex;

        // Insert the dragged layers into its new parent
        int k = insertingRow;
        for(int i = 0; i < count; ++i, ++k)
          dropParentLayer->insert(k, m_insertingLayers[i]);
      }
    }   //end of row < 0
    else
    { // begin of row >= 0
      // The item will be inserted as the last top level item
      // The result of this operation has no effect because
      // the item is inserted and removed afterwards
      insertingRow = m_layers.size();

      // Insert the dragged layers into its new parent
      for(int i = 0; i < count; ++i)
        m_layers.insert(m_layers.begin() + insertingRow + i, m_insertingLayers[i]);
    } // end of row >= 0
  }

  bool ret = insertRows(insertingRow, count, insertingItemParentIndex);

  // If the dropped item is a folder item that was empty emit a signal for expand it.
  //if(folderItemWasEmpty)
  //  emit expandItem(dropItem);

  // Emit a signal to indicate that the order of the layers were changed inside the tree
  if(ret)
    emit layerOrderChanged();

  return ret;
}

bool te::qt::widgets::LayerTreeModel::insertRows(int row, int count, const QModelIndex& parent)
{
  AbstractTreeItem* parentItem = static_cast<AbstractTreeItem*>(parent.internalPointer());

  beginInsertRows(parent, row, row + count - 1);

  if(!parentItem)
  {
    if(row == m_items.size())
    {
      for(int i = 0; i < count; ++i)
      {
        AbstractTreeItem* newItem = AbstractTreeItemFactory::make(m_insertingLayers[i], 0);
        m_items.push_back(newItem);
      }
    }
    else
    {
      int k = 0;
      for(int i = row; i < row + count; ++i, ++k)
      {
        AbstractTreeItem* newItem = AbstractTreeItemFactory::make(m_insertingLayers[k], 0);
        m_items.insert(m_items.begin() + i, newItem);
      }
    }
  }
  else
  {
    // Get the children of the parent item and disconnect them from the tree item
    int numChildren = parentItem->children().count();
    QList<QObject*> savedItemsList = parentItem->children();

    for(int i = 0; i < numChildren; ++i)
     savedItemsList.at(i)->setParent(0);

    // Insert the items into the list
    int k = 0;
    for(int i = row; i < row + count; ++i, ++k)
    {
      AbstractTreeItem* newItem = AbstractTreeItemFactory::make(m_insertingLayers[k], 0);
      savedItemsList.insert(i, newItem);
    }

    // Reinsert the saved items into the tree
    for(int i = 0; i < savedItemsList.count(); ++i)
      savedItemsList.at(i)->setParent(parentItem);
  }

  endInsertRows();

  // Adjust the visibility of the ancestors that are folder layers of the inserted items
  m_insertingLayers[0]->updateVisibilityOfAncestors();
  emitDataChangedForAncestors(parent);

  return true;
}

bool te::qt::widgets::LayerTreeModel::removeRows(int row, int count, const QModelIndex& parent)
{
  bool ret = false;

  beginRemoveRows(parent, row, row + count - 1);

  if(!parent.isValid())
  {
    for(int i = row; i < row + count; ++i)
      delete m_items[i];

    m_items.erase(m_items.begin() + row, m_items.begin() + row + count);
  }
  else
  {
    // Get the parent of the items to be removed
    AbstractTreeItem* parentItem = static_cast<AbstractTreeItem*>(parent.internalPointer());

    // Get the parent layer associated to the items to be removed
    te::map::AbstractLayerPtr parentLayer = parentItem->getLayer();

    // Finally, remove the item from the tree
    const QList<QObject*>& childrenList = parentItem->children();

    std::vector<QObject*> items;
    for(int i = row; i < row + count; ++i)
    {
      QObject* item = childrenList.at(i);
      items.push_back(item);
    }

    for(std::size_t i = 0; i < items.size(); ++i)
      delete items[i];

    // Emit the dataChanged signal for the ancestors of the parent of the item removed
    emitDataChangedForAncestors(parent);

    // Adjust the parent visibility of the removed layer and of their ancestors
    parentLayer->updateVisibility();

    // Adjust the visibility of the ancestors of the parent of the removed layer
    parentLayer->updateVisibilityOfAncestors();
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

QModelIndex te::qt::widgets::LayerTreeModel::getIndex(AbstractTreeItem* item)
{
  if(!item)
    return QModelIndex();

  int itemRow = 0;              // The item row
  QModelIndex parentIndex;  // The parent index of the item

  AbstractTreeItem* parentItem = static_cast<AbstractTreeItem*>(item->parent());

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
    itemRow = parentItem->children().indexOf(item);

  return createIndex(itemRow, 0, item);
}

te::qt::widgets::AbstractTreeItem* te::qt::widgets::LayerTreeModel::getLayerItem(const te::map::AbstractLayerPtr& layer)
{
  if(!layer)
    return 0;

  te::map::AbstractLayerPtr parentLayer = static_cast<te::map::AbstractLayer*>(layer->getParent());

  if(!parentLayer)
  {
    // The layer is a top layer
    for(std::size_t i = 0; i < m_layers.size(); ++i)
    {
      if(m_layers[i] == layer)
        return m_items[i];
    }
  }

  // Get the list of rows of the ancestors of the given layer,
  // and the index of its top level ancestor layer.
  std::vector<std::size_t> rows;
  QModelIndex topLayerIndex;

  te::map::AbstractLayerPtr auxLayer = layer;

  while(parentLayer)
  {
    rows.push_back(auxLayer->getIndex());
    auxLayer = parentLayer;
    parentLayer = static_cast<te::map::AbstractLayer*>(parentLayer->getParent());
  }

  te::map::AbstractLayerPtr topLayer = auxLayer;
  te::qt::widgets::AbstractTreeItem* topItem = 0;

  for(std::size_t i = 0; i < m_layers.size(); ++i)
  {
    if(m_layers[i] == topLayer)
    {
      topItem = m_items[i];
      topLayerIndex = index(i, 0, QModelIndex());
      break;
    }
  }

  QModelIndex inLayerIndex = topLayerIndex;

  std::vector<std::size_t>::reverse_iterator it;
  for(it = rows.rbegin(); it != rows.rend(); ++it)
    inLayerIndex = index(*it, 0, inLayerIndex);

  return static_cast<AbstractTreeItem*>(inLayerIndex.internalPointer());
}

void te::qt::widgets::LayerTreeModel::add(const te::map::AbstractLayerPtr& layer, te::qt::widgets::AbstractTreeItem* parentItem)
{
  std::size_t row = 0;

  QModelIndex parentIndex;

  if(!parentItem)
  {
    row = m_layers.size();
    m_layers.push_back(layer);
  }
  else
  {
    row = parentItem->children().size();
    parentIndex = getIndex(parentItem);
  }

  m_insertingLayers.clear();
  m_insertingLayers.push_back(layer);

  insertRows(row, 1, parentIndex);
}

bool te::qt::widgets::LayerTreeModel::remove(AbstractTreeItem* item)
{
  QModelIndex itemIndex = getIndex(item);

  int itemRow = itemIndex.row();
  QModelIndex parentIndex = parent(itemIndex);

  AbstractTreeItem* parentItem = static_cast<AbstractTreeItem*>(item->parent());

  if(item->getLayer())
  {
    // If the item is a single layer item or a folder layer item, remove the layer associated to the item.
    if(!parentItem)
    {
      // The item is a top level item; get its row
      for(std::size_t i = 0; i < m_items.size(); ++i)
      {
        if(m_items[i] == item)
        {
          m_layers.erase(m_layers.begin() + i);
          break;
        }
      }
    }
  }

  return removeRows(itemRow, 1, parentIndex);
}

void te::qt::widgets::LayerTreeModel::removeLayerFromParentChildrenList(std::vector<te::map::AbstractLayerPtr>& layers)
{
  for(std::size_t i = 0; i < layers.size(); ++i)
  {
    te::map::AbstractLayerPtr layer = layers[i];

    te::map::AbstractLayerPtr parentLayer = static_cast<te::map::AbstractLayer*>(layer->getParent());

    if(!parentLayer)
    {
      for(std::size_t j = 0; j < m_layers.size(); ++j)
      {
        if(m_layers[j] == layer)
          m_layers.erase(m_layers.begin() + j);
      }
    }
    else
      parentLayer->remove(layer->getIndex());
  }
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
