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

te::qt::widgets::LayerTreeModel::LayerTreeModel(const std::list<te::map::AbstractLayerPtr>& layers, QObject * parent)
  : QAbstractItemModel(parent),
    m_checkable(false)
{
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
    return false;

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  return item->canFetchMore();
}

int te::qt::widgets::LayerTreeModel::columnCount(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return 1; // item is the root

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  return item->columnCount();
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

  item->fetchMore();
}

Qt::ItemFlags te::qt::widgets::LayerTreeModel::flags(const QModelIndex& index) const
{
  if(!index.isValid())
    return Qt::ItemIsEnabled;

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  if(item == 0)
    return Qt::ItemIsEnabled;

  return QAbstractItemModel::flags(index) | item->flags();
}

bool te::qt::widgets::LayerTreeModel::hasChildren(const QModelIndex& parent) const
{
  if(parent.isValid())
  {
    AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

    if(item == 0)
      return false;

    return item->hasChildren();
  }

  return true;
}

QModelIndex te::qt::widgets::LayerTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if(!parent.isValid()) // is it a top-level item?
  {
// yes!
    AbstractLayerTreeItem* item = m_items[row];

    return createIndex(row, column, item);
  }

  AbstractLayerTreeItem* parentItem = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

  if(parentItem->children().empty())  // if there isn't a child, return an invalid index
    return QModelIndex();

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parentItem->children().at(row));

  return createIndex(row, column, item);
}

//bool te::qt::widgets::LayerTreeModel::insertRows(int row, int count, const QModelIndex & parent)
//{
//
//}

QModelIndex te::qt::widgets::LayerTreeModel::parent(const QModelIndex& index) const
{
  if(!index.isValid())
    return QModelIndex();

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(index.internalPointer());

  if(item == 0 || item->parent() == 0)
    return QModelIndex();

  return createIndex(index.row(), index.column(), static_cast<AbstractLayerTreeItem*>(item->parent()));
}

int te::qt::widgets::LayerTreeModel::rowCount(const QModelIndex& parent) const
{
  if(!parent.isValid()) // if parent index isnot valid we assume we are asking for root items
    return m_items.size();

  AbstractLayerTreeItem* item = static_cast<AbstractLayerTreeItem*>(parent.internalPointer());

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

  return retval;
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

