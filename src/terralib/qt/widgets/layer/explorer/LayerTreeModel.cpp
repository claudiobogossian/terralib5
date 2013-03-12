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

bool te::qt::widgets::LayerTreeModel::dropMimeData(const QMimeData* data,
                                                   Qt::DropAction action, 
                                                   int row, int column,
                                                   const QModelIndex& parent)
{
  if(data == 0)
    return false;

//  if(action == Qt::IgnoreAction)
//    return true;
//
//  if(action != Qt::MoveAction)
//    return false;
//
//  if(column > 0)
//    return false;
//
//  if(!data->hasFormat("application/x-terralib;value=\"AbstractLayerTreeItem\""))
//    return false;
//
//// get the item that was dragged
//  QString sitem = data->data("application/x-terralib;value=\"AbstractLayerTreeItem\"");
//
//  if(sitem.isEmpty())
//    return false;
//
//  qulonglong dataValue = sitem.toULongLong();
//
//  AbstractLayerTreeItem* draggedItem = reinterpret_cast<AbstractLayerTreeItem*>(dataValue);
//
//// have we dropped the item in the top level?
//  if(!parent.isValid())
//  {
//// yes!
//    if(row < 0)
//      return false;
//
//    AbstractLayerTreeItem* nitem = draggedItem->clone(0);
//
//    m_items.insert(m_items.begin() + row, nitem);
//
//    return true;
//  }
//
//// get the parent item where the dragged one was dropped
//  if(parent.internalPointer() == 0)
//    return false;
//
//  AbstractLayerTreeItem* parentItem = reinterpret_cast<AbstractLayerTreeItem*>(parent.internalPointer());

// disconnect all the children
  //QList<QObject*> savedItemsList = parentItem->children();

  //for(QList<QObject*>::Iterator it = savedItemsList.begin(); it != savedItemsList.end(); ++it)
  //  (*it)->setParent(0);

  //AbstractLayerTreeItem* nitem = draggedItem->clone(0);

  //savedItemsList.insert(row, nitem);

  //// Reinsert the saved items into the tree
  //for(int i = 0; i < savedItemsList.count(); ++i)
  //  savedItemsList.at(i)->setParent(this);

  

//// check if the items involved in the drag and drop operation are siblings
//  bool areSiblings = draggedItem->parent() == droppedItem->parent();

  //// A new item will be generated(m_dndNewItem). Check where it will be placed, by setting
  //// its row(newItemRow) and its parent index(newItemParentIndex). Set also its new
  //// item parent(newItemParent).
  //int newItemRow = 0;
  //QModelIndex newItemParentIndex;
  //te::qt::widgets::AbstractTreeItem* newItemParent = 0;

  //if(dropIndex.isValid())
  //{
  //  if((dragRefLayer->getType() == "LAYER" && dropRefLayer->getType() == "LAYER") ||
  //     (dragRefLayer->getType() == "FOLDERLAYER" && dropRefLayer->getType() == "LAYER"))
  //  {
  //    newItemRow = dropIndex.row();
  //    newItemParentIndex = dropIndex.parent();
  //  }
  //  else if((dragRefLayer->getType() == "LAYER" && dropRefLayer->getType() == "FOLDERLAYER") ||
  //          (dragRefLayer->getType() == "FOLDERLAYER" && dropRefLayer->getType() == "FOLDERLAYER"))
  //  {
  //    if(areSiblings)
  //    {
  //      if(dropRefLayer->getChildrenCount() == 0)
  //      {
  //        newItemRow = 0;
  //        newItemParentIndex = dropIndex;
  //      }
  //      else
  //      {
  //        newItemRow = dropIndex.row();
  //        newItemParentIndex = dropIndex.parent();
  //      }
  //    }
  //    else
  //    {
  //      newItemRow = 0;
  //      newItemParentIndex = dropIndex;
  //    }
  //  }
  //  newItemParent = getItem(newItemParentIndex);
  //}
  //else
  //{
  //  newItemRow = m_rootItem->children().count();
  //  newItemParentIndex = QModelIndex();
  //  newItemParent = m_rootItem;
  //}

  //// Disconnect the reference layer from its parent
  //te::map::AbstractLayer* dragRefLayerParent = static_cast<te::map::AbstractLayer*>(dragRefLayer->getParent());
  //int dragRow = dragRefLayer->getIndex();
  //dragRefLayerParent->remove(dragRow);
  //
  //// * Under revision *
  //// Construct a dummy layer and insert it as a child of the dragged item parent in the drag position
  ////te::map::AbstractLayer* dummyLayer = new te::map::Layer("0", "DummyLayer", 0);
  ////dragRefLayerParent->insert(dragRow, dummyLayer);

  //// Create the new tree item
  //m_dndNewItem = new te::qt::widgets::LayerItem(dragRefLayer, 0);

  //// Adjust the newItemRow when it is immediately below the dragRow, and when
  //// the drag and drop items are siblings.
  //if(areSiblings && (newItemRow == dragRow + 1))
  //  ++newItemRow;

  //// Insert the new item into the tree
  //beginInsertRows(newItemParentIndex, newItemRow, newItemRow);
  //newItemParent->insertChild(newItemRow, m_dndNewItem);
  //endInsertRows();

  //// Update the visibility of the tree
  //Qt::CheckState state;
  //te::map::Visibility visibility = dragRefLayer->getVisibility();
  //if(visibility == te::map::NOT_VISIBLE)
  //  state = Qt::Unchecked;
  //else if(visibility == te::map::VISIBLE)
  //  state = Qt::Checked;
  //else
  //  state = Qt::PartiallyChecked;

  //setData(index(newItemRow, 0, newItemParentIndex), state, Qt::CheckStateRole);

  return true;
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

  if(!parent.isValid()) // is it a top-level item?
  {
// yes!
    AbstractLayerTreeItem* item = m_items[row];

    return createIndex(row, column, item);
  }

  AbstractLayerTreeItem* parentItem = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

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

