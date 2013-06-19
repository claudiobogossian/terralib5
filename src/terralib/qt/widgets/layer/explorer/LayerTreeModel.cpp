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

void RemoveLayerFromList(const te::map::AbstractLayer* layer, std::vector<te::qt::widgets::AbstractLayerTreeItem*>& layers)
{
  std::vector<te::qt::widgets::AbstractLayerTreeItem*>::iterator it;

  for(it=layers.begin(); it!=layers.end(); ++it)
    if((*it)->getLayer().get() == layer)
    {
      layers.erase(it);
      return;
    }
}

void RemoveLayer(const te::map::AbstractLayer* layer, std::vector<te::qt::widgets::AbstractLayerTreeItem*>& layers)
{
  if (layer->getParent() == 0)
    RemoveLayerFromList(layer, layers);
  else
  {
    size_t idx = layer->getIndex();
    layer->getParent()->remove(idx);
    ((te::map::AbstractLayer*)layer->getParent())->updateVisibility();
  }
}

size_t GetLayerPosition(const te::qt::widgets::AbstractLayerTreeItem* layer, std::vector<te::qt::widgets::AbstractLayerTreeItem*>& layers)
{
  for(size_t i=0; i< layers.size(); i++)
    if(layer == layers[i])
      return i;

  return -1;
}

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

  mimes << "application/x-terralib-items.list";

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

  QMimeData *mimeData = new QMimeData();
  QByteArray encodedData;
  QDataStream stream(&encodedData, QIODevice::WriteOnly);

  foreach (const QModelIndex &index, indexes) 
  {
    if (index.isValid()) 
    {
      if(!(flags(index) & Qt::ItemIsDragEnabled))
        continue;

      AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

      QString s;
      s.setNum((qulonglong)item);

      stream << s;
    }
  }

  mimeData->setData("application/x-terralib-items.list", encodedData);
  return mimeData;


  //for(it=indexes.begin(); it!=indexes.end(); ++it)
  //{
  //  const QModelIndex& lindex = *it;
  //
  //  if(!(flags(lindex) & Qt::ItemIsDragEnabled))
  //    continue;


  //}


  //if(!(flags(lindex) & Qt::ItemIsDragEnabled))
  //  return 0;

  //if(indexes.count() > 1)
  //{
  //  QMessageBox::warning(0, tr("Operation Not Allowed"), tr("Only one layer at a time can be dragged!"));
  //  return 0;
  //}

  //const QModelIndex& lindex = indexes.first();

  //if(!(flags(lindex) & Qt::ItemIsDragEnabled))
  //  return 0;

  //AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(lindex.internalPointer());

  //QString s;
  //s.setNum((qulonglong)item);

  //QByteArray encodedData(s.toStdString().c_str());

  //QMimeData* mimeData = new QMimeData();

  //mimeData->setData("application/x-terralib;value=\"AbstractLayerTreeItem\"", encodedData);

  //return mimeData;
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

  if(!data->hasFormat("application/x-terralib-items.list"))
    return false;

  QByteArray encodedData = data->data("application/x-terralib-items.list");
  QDataStream stream(&encodedData, QIODevice::ReadOnly);

  std::vector<AbstractLayerTreeItem*> lay_items;
  std::vector<AbstractLayerTreeItem*>::iterator it;

  while (!stream.atEnd()) 
  {
    QString sitem;
    stream >> sitem;
  
    qulonglong dataValue = sitem.toULongLong();
  
    AbstractLayerTreeItem* item = reinterpret_cast<AbstractLayerTreeItem*>(dataValue);
    lay_items.push_back(item);
  }

  beginResetModel();
  
  if(row == -1) // Clicked over a item.
  {
    if(parent.isValid())
    {
      AbstractLayerTreeItem* dropped_Layer = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());
      te::map::AbstractLayer* drop_l = dropped_Layer->getLayer().get();

      if(dropped_Layer->getLayer()->getType().compare("FOLDERLAYER") == 0)
      {
        for(it=lay_items.begin(); it!=lay_items.end(); ++it) // Adding layers to a group.
        {
          te::map::AbstractLayer* drag_l = (*it)->getLayer().get();

          RemoveLayer(drag_l, m_items);

          (*it)->setParent(dropped_Layer);
          drop_l->add((*it)->getLayer());
        }

        drop_l->updateVisibility();
      }
    }
    else
    {
      for(it=lay_items.begin(); it!=lay_items.end(); ++it) 
      {
        te::map::AbstractLayer* drag_l = (*it)->getLayer().get();

        RemoveLayer(drag_l, m_items);

        (*it)->setParent(0);
        m_items.push_back(*it);
      }
    }
  }
  else // Layers prority changes
  {
    if(lay_items.size() == 1)
    {

      AbstractLayerTreeItem* parent_item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());
      AbstractLayerTreeItem* drag_item = lay_items[0];

      if(parent_item == drag_item->parent())
      {
        int oldRow = drag_item->getLayer().get()->getIndex();

        if(parent.isValid())
        {
          drag_item->setParent(0);

          if(oldRow < row) // priority down
          {
            parent_item->getLayer()->insert(row, drag_item->getLayer());
            parent_item->getLayer()->remove(oldRow);
          }
          else        //priority up
          {
            parent_item->getLayer()->remove(oldRow);
            parent_item->getLayer()->insert(row, drag_item->getLayer());
          }

          drag_item->setParent(parent_item);
        }
        else
        {
          oldRow = GetLayerPosition(drag_item, m_items);

          if(oldRow < row) // priority down
          {
            m_items.insert(m_items.begin()+row, drag_item);
            RemoveLayer(drag_item->getLayer().get(), m_items);
          }
          else        //priority up
          {
            RemoveLayer(drag_item->getLayer().get(), m_items);
            m_items.insert(m_items.begin()+row, drag_item);
          }
        }
      }
    }
//    if(parent.isValid())
//    {
//      AbstractLayerTreeItem* parent_Layer = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());
//      te::map::AbstractLayer* parent_l = parent_Layer->getLayer().get();
//
//      for(it=lay_items.begin(); it!=lay_items.end(); ++it) 
//      {
//        te::map::AbstractLayer* drag_l = (*it)->getLayer().get();
//
//        RemoveLayer(drag_l, m_items);
//
//        int pos = 0; //it - lay_items.begin();
//
//   //     if(row > parent_l->getChildrenCount())
//     //     pos--;
//
//        (*it)->setParent(parent_Layer);
//        parent_l->add((*it)->getLayer());
////        parent_l->insert(row+pos, drag_l);
//      }
//    }
  }

  endResetModel();

  emit expand(parent);

  // Get the item that was dragged
  //QString sitem = data->data("application/x-terralib;value=\"AbstractLayerTreeItem\"");

  //if(sitem.isEmpty())
  //  return false;

  //qulonglong dataValue = sitem.toULongLong();

  //AbstractLayerTreeItem* draggedItem = reinterpret_cast<AbstractLayerTreeItem*>(dataValue);

  //// Set the row and the parent of the item to be inserted
  //int newItemRow;
  //QModelIndex newItemParentIndex;

  //QModelIndex dropIndex = parent; 
  //QModelIndex dropParentIndex = dropIndex.parent();

  //if(row < 0)
  //{
  //  if(dropParentIndex == QModelIndex())
  //  {
  //    // Drop item is a top level item
  //    if(dropIndex.row() == -1)
  //      newItemRow = m_items.size();   // The item was dropped after the last top item
  //    else        
  //      newItemRow = dropIndex.row();  // The item was dropped exactly on a top level item

  //    newItemParentIndex = QModelIndex();
  //  }
  //  else
  //  {
  //    // Drop item is not a top level item
  //    if(dropIndex.row() == -1)
  //      row = 0;
  //    else
  //      newItemRow = dropIndex.row();  // The item will be inserted in the position of the dropped item.

  //    newItemParentIndex = dropParentIndex;
  //  }

  //  // Check if the drop item is a folder item and if it has no children; in that case,
  //  // the dragged item will be made child of this folder item
  //  AbstractLayerTreeItem* dropItem = static_cast<AbstractLayerTreeItem*>(dropIndex.internalPointer());

  //  if(dropItem)
  //  {
  //    te::map::AbstractLayer* dropItemLayer = dropItem->getLayer().get();

  //    if(dropItemLayer->getType() == "FOLDERLAYER" && dropItemLayer->hasChildren() == false)
  //    {
  //      newItemRow = 0;
  //      newItemParentIndex = dropIndex;
  //    }
  //  }
  //}
  //else
  //{
  //  // row >= 0
  //  newItemRow = row;

  //  if(dropParentIndex == QModelIndex())
  //  {
  //    // Drop item is a top level item
  //    if(dropIndex.row() == -1)
  //      newItemParentIndex = QModelIndex();   // The item will be inserted as a top level item in the row-th position
  //    else
  //      newItemParentIndex = dropIndex;       // The item will be inserted in the row-th position as child of the dropped item
  //  }
  //  else
  //  {
  //    // Drop item is not a top level item
  //    if(dropIndex.row() == -1)
  //      newItemParentIndex = dropIndex.parent();
  //    else
  //      newItemParentIndex = dropIndex;
  //  }
  //}

  //// Create a new item to be inserted into the layer tree
  //AbstractLayerTreeItem* newItemParent = static_cast<AbstractLayerTreeItem*>(newItemParentIndex.internalPointer());

  //AbstractLayerTreeItem* newItem = AbstractLayerTreeItemFactory::make(draggedItem->getLayer(), 0);

  //// Insert the new item into the tree
  //beginInsertRows(newItemParentIndex, newItemRow, newItemRow);

  //if(!newItemParent)
  //  m_items.insert(m_items.begin() + newItemRow, newItem);
  //else
  //{
  //  // Insert the associated layer of the dragged item to the list of layers
  //  // of the new parent layer of the new item
  //  te::map::AbstractLayer* parentLayerOfNewItem = newItemParent->getLayer().get();
  //  parentLayerOfNewItem->insert(newItemRow, draggedItem->getLayer());

  //  // Get the children of the tree item and disconnect them from the tree item
  //  int numChildren = newItemParent->children().count();
  //  QList<QObject*> savedItemsList = newItemParent->children();

  //  for(int i = 0; i < numChildren; ++i)
  //    savedItemsList.at(i)->setParent(0);

  //  // Insert the given item into the saved tree
  //  savedItemsList.insert(newItemRow, newItem);

  //  // Reinsert the saved items into the tree
  //  for(int i = 0; i < savedItemsList.count(); ++i)
  //    savedItemsList.at(i)->setParent(newItemParent);

  //  // Adjust the visibility of the ascendent layers of the inserted layer
  //  newItem->getLayer().get()->updateVisibilityOfAncestors();
  //  emitDataChangedForAncestors(newItemParentIndex);

  //  // Adjust the visibility of the ascendent layers of the inserted layer

  //  // Emit the signal of visibility changed for the ancestors of the new item who had their visibility changed
  //  std::vector<AbstractLayerTreeItem*> ancestorItems = newItem->getAncestors();
  //  for(std::size_t i = 0; i < ancestorItems.size(); ++i)
  //  {
  //    te::map::AbstractLayer* ancestorLayer = ancestorItems[i]->getLayer().get();
  //    if(ancestorLayer->hasVisibilityChanged())
  //      emit visibilityChanged(ancestorItems[i]);
  //  }
  //}

  //endInsertRows();

  return true;
}

bool te::qt::widgets::LayerTreeModel::removeRows(int row, int count, const QModelIndex& parent)
{
//  beginRemoveRows(parent, row, row + count - 1);

  //if(!parent.isValid())
  //{
  //  delete m_items[row];
  //  m_items.erase(m_items.begin() + row);
  //}
  //else
  //{
  //  // Delete the item that is in the row-th position of the given parent
  //  QModelIndex removingIndex = parent.child(row, 0);
  //  AbstractLayerTreeItem* removingItem = static_cast<AbstractLayerTreeItem*>(removingIndex.internalPointer());

  //  // Get the item associated to the parent
  //  AbstractLayerTreeItem* removingItemParent = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  //  // First, remove the associated layer associated to the item
  //  te::map::AbstractLayer* removingItemParentLayer = static_cast<te::map::AbstractLayer*>(removingItemParent->getLayer().get());

  //  removingItemParentLayer->remove(row);

  //  // Finally, remove the item from the tree
  //  const QList<QObject*>& childrenList = removingItemParent->children();
  //  int numChildren = childrenList.count();

  //  if (numChildren == 0 || row < 0 || row >= numChildren)
  //    return false;

  //  removingItem->setParent(0);

  //  delete removingItem;

  //  // Emit the dataChanged signal for the ancestors of the parent of the item removed
  //  emitDataChangedForAncestors(parent);

  //  // Adjust the parent visibility of the removed layer and emit the signal 
  //  // of visibility changed, if its visibility has changed
  //  removingItemParentLayer->updateVisibility();

  //  if(removingItemParentLayer->hasVisibilityChanged())
  //    emit visibilityChanged(removingItemParent);

  //  // Adjust the visibility of the ancestors of the parent of the removed layer
  //  removingItemParentLayer->updateVisibilityOfAncestors();

  //  std::vector<AbstractLayerTreeItem*> ancestorItems = removingItemParent->getAncestors();
  //  for(std::size_t i = 0; i < ancestorItems.size(); ++i)
  //  {
  //    te::map::AbstractLayer* ancestorLayer = ancestorItems[i]->getLayer().get();
  //    if(ancestorLayer->hasVisibilityChanged())
  //      emit visibilityChanged(ancestorItems[i]);
  //  }
  //}

//  endRemoveRows();

  return QAbstractItemModel::removeRows(row, count, parent);
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

std::vector<te::map::AbstractLayerPtr> te::qt::widgets::LayerTreeModel::getTopLevelLayers()
{
  std::vector<te::map::AbstractLayerPtr> layers;
  std::vector<te::qt::widgets::AbstractLayerTreeItem*>::iterator it;
 
  for(it=m_items.begin(); it!=m_items.end(); ++it)
    layers.push_back((*it)->getLayer());

  return layers;
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
