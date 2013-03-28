/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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

  \brief The class that defines the model used in the Qt Model/View architecture
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../Exception.h"
#include "AbstractLayerTreeItem.h"
#include "AbstractLayerTreeItemFactory.h"
#include "LayerTreeModel.h"

// Qt
#include <QtCore/QMimeData>
#include <QtCore/QStringList>

te::qt::widgets::LayerTreeModel::LayerTreeModel(const std::list<te::map::AbstractLayerPtr>& layers, QObject * parent)
  : QAbstractItemModel(parent),
    m_checkable(false)
{
  setSupportedDragActions(Qt::MoveAction);

  for(std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin(); it != layers.end(); ++it)
  {
    AbstractLayerTreeItem* litem = AbstractLayerTreeItemFactory::make(*it, 0);

    if(litem)
      m_items.push_back(litem);
  }
}

te::qt::widgets::LayerTreeModel::~LayerTreeModel()
{
  te::common::FreeContents(m_items);
}

bool te::qt::widgets::LayerTreeModel::canFetchMore(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return !m_items.empty();

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  return item->canFetchMore();
}

int te::qt::widgets::LayerTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
  return 1;
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

void te::qt::widgets::LayerTreeModel::fetchMore(const QModelIndex& parent)
{
  if(!parent.isValid())
    return;

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TR_QT_WIDGETS("Invalid data associated to the layer model!"));

  item->fetchMore();
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

bool te::qt::widgets::LayerTreeModel::hasChildren(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return !m_items.empty();

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TR_QT_WIDGETS("Invalid data associated to the layer model!"));

  return item->hasChildren();
}

QModelIndex te::qt::widgets::LayerTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if(m_items.empty())
    return QModelIndex();

  if(parent == QModelIndex() && static_cast<std::size_t>(row) >= m_items.size())
    return QModelIndex();

  AbstractLayerTreeItem* parentItem = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  if(parentItem && row >= parentItem->children().count())
    return QModelIndex();

  if(!parent.isValid()) // is it a top-level item?
  {
// yes!
    AbstractLayerTreeItem* item = m_items[row];

    return createIndex(row, column, item);
  }

  //AbstractLayerTreeItem* parentItem = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  if(parentItem == 0)
    throw Exception(TR_QT_WIDGETS("Invalid data associated to the layer model!"));

  if(parentItem->children().empty())
    throw Exception(TR_QT_WIDGETS("The layer item in the model must have a child item!"));

  if(row >= parentItem->children().size())
    throw Exception(TR_QT_WIDGETS("The row for the layer item is out of range!"));

  AbstractLayerTreeItem* item = dynamic_cast<AbstractLayerTreeItem*>(parentItem->children().at(row));

  if(item == 0)
    throw Exception(TR_QT_WIDGETS("The layer item is not an AbstractLayerTreeItem!"));

  return createIndex(row, column, item);
}

//bool te::qt::widgets::LayerTreeModel::insertRows(int row, int count, const QModelIndex & parent)
//{
//
//}

QMimeData* te::qt::widgets::LayerTreeModel::mimeData(const QModelIndexList& indexes) const
{
  if(indexes.empty())
    return 0;

  const QModelIndex& lindex = indexes.first();

  if(!(flags(lindex) & Qt::ItemIsDragEnabled))
    return 0;

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(lindex.internalPointer());

  QString s;
  s.setNum((qulonglong)item);

  QByteArray encodedData(s.toStdString().c_str());

  QMimeData* mimeData = new QMimeData();

  mimeData->setData("application/x-terralib;value=\"AbstractLayerTreeItem\"", encodedData);

  return mimeData;
}

QStringList te::qt::widgets::LayerTreeModel::mimeTypes() const
{
  QStringList mimes;

  mimes << "application/x-terralib;value=\"AbstractLayerTreeItem\"";

  return mimes;
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

  if(!data->hasFormat("application/x-terralib;value=\"AbstractLayerTreeItem\""))
    return false;

  // Get the item that was dragged
  QString sitem = data->data("application/x-terralib;value=\"AbstractLayerTreeItem\"");

  if(sitem.isEmpty())
    return false;

  qulonglong dataValue = sitem.toULongLong();

  AbstractLayerTreeItem* draggedItem = reinterpret_cast<AbstractLayerTreeItem*>(dataValue);

  // Set the row and the parent of the item to be inserted
  int newItemRow;
  QModelIndex newItemParentIndex;

  QModelIndex dropIndex = parent; 
  QModelIndex dropParentIndex = dropIndex.parent();

  if(row < 0)
  {
    if(dropParentIndex == QModelIndex())
    {
      // Drop item is a top level item
      if(dropIndex.row() == -1)
        newItemRow = m_items.size();   // The item was dropped after the last top item
      else        
        newItemRow = dropIndex.row();  // The item was dropped exactly on a top level item

      newItemParentIndex = QModelIndex();
    }
    else
    {
      // Drop item is not a top level item
      if(dropIndex.row() == -1)
        row = 0;
      else
        newItemRow = dropIndex.row();  // The item will be inserted in the position of the dropped item.

      newItemParentIndex = dropParentIndex;
    }
  }
  else
  {
    // row >= 0
    if(dropParentIndex == QModelIndex())
    {
      newItemRow = row;

      // Drop item is a top level item
      if(dropIndex.row() == -1)
        newItemParentIndex = QModelIndex();   // The item will be inserted as a top level item in the row-th position
      else
        newItemParentIndex = dropIndex;       // The item will be inserted in the row-th position as child of the dropped item
    }
    else
    {
      // Drop item is not a top level item
      if(dropIndex.row() == -1)
        newItemParentIndex = dropIndex.parent();
      else
        newItemParentIndex = dropIndex;
    }
  }

  // Create a new item to be inserted into the layer tree
  AbstractLayerTreeItem* newItemParent = static_cast<AbstractLayerTreeItem*>(newItemParentIndex.internalPointer());

  AbstractLayerTreeItem* newItem = AbstractLayerTreeItemFactory::make(draggedItem->getLayer(), 0);

  // Insert the new item into the tree
  beginInsertRows(newItemParentIndex, newItemRow, newItemRow);

  if(!newItemParent)
    m_items.insert(m_items.begin() + newItemRow, newItem);
  else
  {
    // Insert the associated layer of the dragged item to the list of layers
    // of the new item parent
    te::map::AbstractLayer* layer = newItemParent->getLayer().get();
    layer->insert(newItemRow, draggedItem->getLayer());

    // Get the children of the tree item and disconnect them from the tree item
    int numChildren = newItemParent->children().count();
    QList<QObject*> savedItemsList = newItemParent->children();

    for(int i = 0; i < numChildren; ++i)
      savedItemsList.at(i)->setParent(0);

    // Insert the given item into the saved tree
    savedItemsList.insert(newItemRow, newItem);

    // Reinsert the saved items into the tree
    for(int i = 0; i < savedItemsList.count(); ++i)
      savedItemsList.at(i)->setParent(newItemParent);
  }

  endInsertRows();

  return true;
}

bool te::qt::widgets::LayerTreeModel::removeRows(int row, int count, const QModelIndex& parent)
{
  AbstractLayerTreeItem* parentItem = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  beginRemoveRows(parent, row, row + count - 1);

  if(!parent.isValid())
  {
    delete m_items[row];
    m_items.erase(m_items.begin() + row);
  }
  else
  {
    // Delete the item that is in the row-th position of the given parent
    QModelIndex childIndex = parent.child(row, 0);
    AbstractLayerTreeItem* childItem = static_cast<AbstractLayerTreeItem*>(childIndex.internalPointer());

    // Get the item parent
    AbstractLayerTreeItem* itemParent = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

    // First, remove the associated layer associated to the item
    te::map::AbstractLayer* layer = childItem->getLayer().get();
    te::map::AbstractLayer* layerParent = static_cast<te::map::AbstractLayer*>(layer->getParent());

    // Get the index of the associated layer
    std::size_t i = layer->getIndex();

    if(layerParent)
      layerParent->remove(i);

    // Finally, remove the item from the tree
    const QList<QObject*>& childrenList = itemParent->children();
    int numChildren = childrenList.count();

    if (numChildren == 0 || row < 0 || row >= numChildren)
      return false;

    childItem->setParent(0);

    delete childItem;
  }

  endRemoveRows();

  return true;
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

int te::qt::widgets::LayerTreeModel::rowCount(const QModelIndex& parent) const
{
  if(!parent.isValid()) // if parent index isnot valid we assume we are asking for root items
    return static_cast<int>(m_items.size());

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TR_QT_WIDGETS("Error: NULL layer item!"));

  return item->children().count();
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

  bool retval = item->setData(value, role);

  emit dataChanged(index, index);

  if(role == Qt::CheckStateRole)
  {
    if(hasChildren(index))
      dataChangedForDescendants(index);

    if(parent(index).isValid())
    {
// emit the dataChanged signal for the ascendants indexes
      QModelIndex ascendentIndex = parent(index);

      while(ascendentIndex.isValid())
      {
        emit dataChanged(ascendentIndex, ascendentIndex);
        ascendentIndex = parent(ascendentIndex);
      }
    }
  }

  return retval;
}

Qt::DropActions te::qt::widgets::LayerTreeModel::supportedDropActions() const
{ 
  return Qt::MoveAction | Qt::CopyAction;
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

  AbstractLayerTreeItem* litem = AbstractLayerTreeItemFactory::make(layer, 0);

  if(litem == 0)
    throw Exception(TR_QT_WIDGETS("Could not add layer to the LayerTreeModel!"));

  QModelIndex parent;

  beginInsertRows(parent, m_items.size(), m_items.size());

  m_items.push_back(litem);

  endInsertRows();
}

void te::qt::widgets::LayerTreeModel::dataChangedForDescendants(const QModelIndex& parent)
{
  if(!parent.isValid())
    return;

  int rows = rowCount(parent);

  for(int i = 0; i != rows; ++i)
  {
    QModelIndex idx = index(i, 0, parent);

    emit dataChanged(idx, idx);

    if(hasChildren(idx))
      dataChangedForDescendants(idx);
  }
}
