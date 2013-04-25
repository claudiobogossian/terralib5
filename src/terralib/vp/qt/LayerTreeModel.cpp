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
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/layer/explorer/AbstractLayerTreeItem.h"
#include "../../qt/widgets/layer/explorer/AbstractLayerTreeItemFactory.h"
#include "../core/Exception.h"
#include "LayerItem.h"
#include "LayerTreeModel.h"

// Qt
#include <QtCore/QMimeData>
#include <QtCore/QStringList>

te::vp::LayerTreeModel::LayerTreeModel(const std::list<te::map::AbstractLayerPtr>& layers, QObject * parent)
  : QAbstractItemModel(parent)
{
  for(std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin(); it != layers.end(); ++it)
  {
    te::qt::widgets::AbstractLayerTreeItem* litem = te::qt::widgets::AbstractLayerTreeItemFactory::make(*it, 0);

    if(litem)
      m_items.push_back(litem);
  }
}

te::vp::LayerTreeModel::~LayerTreeModel()
{
  te::common::FreeContents(m_items);
}

bool te::vp::LayerTreeModel::canFetchMore(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return !m_items.empty();

  te::qt::widgets::AbstractLayerTreeItem* item = static_cast<te::qt::widgets::AbstractLayerTreeItem*>(parent.internalPointer());

  return item->canFetchMore();
}

int te::vp::LayerTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
  return 1;
}

QVariant te::vp::LayerTreeModel::data(const QModelIndex& index, int role) const
{
  if(!index.isValid())
    return QVariant();

  if(role == Qt::CheckStateRole)
    return QVariant();

  te::qt::widgets::AbstractLayerTreeItem* item = static_cast<te::qt::widgets::AbstractLayerTreeItem*>(index.internalPointer());

  if(item == 0)
    return QVariant();

  return item->data(index.column(), role);
}

void te::vp::LayerTreeModel::fetchMore(const QModelIndex& parent)
{
  if(!parent.isValid())
    return;

  te::qt::widgets::AbstractLayerTreeItem* item = static_cast<te::qt::widgets::AbstractLayerTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TR_VP("Invalid data associated to the layer model!"));

  item->fetchMore();
}

Qt::ItemFlags te::vp::LayerTreeModel::flags(const QModelIndex& index) const
{
  if(!index.isValid())
    return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

  te::qt::widgets::AbstractLayerTreeItem* item = static_cast<te::qt::widgets::AbstractLayerTreeItem*>(index.internalPointer());

  if(item == 0)
    throw Exception(TR_VP("Invalid data associated to the layer model!"));

  return QAbstractItemModel::flags(index) | item->flags();
}

bool te::vp::LayerTreeModel::hasChildren(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return !m_items.empty();

  te::qt::widgets::AbstractLayerTreeItem* item = static_cast<te::qt::widgets::AbstractLayerTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TR_VP("Invalid data associated to the layer model!"));

  return item->hasChildren();
}

QModelIndex te::vp::LayerTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if(m_items.empty())
    return QModelIndex();

  if(parent == QModelIndex() && static_cast<std::size_t>(row) >= m_items.size())
    return QModelIndex();

  te::qt::widgets::AbstractLayerTreeItem* parentItem = static_cast<te::qt::widgets::AbstractLayerTreeItem*>(parent.internalPointer());

  if(parentItem && row >= parentItem->children().count())
    return QModelIndex();

  if(!parent.isValid()) // is it a top-level item?
  {
// yes!
    te::qt::widgets::AbstractLayerTreeItem* item = m_items[row];

    return createIndex(row, column, item);
  }

  //te::qt::widgets::AbstractLayerTreeItem* parentItem = static_cast<te::qt::widgets::AbstractLayerTreeItem*>(parent.internalPointer());

  if(parentItem == 0)
    throw Exception(TR_VP("Invalid data associated to the layer model!"));

  if(parentItem->children().empty())
    throw Exception(TR_VP("The layer item in the model must have a child item!"));

  if(row >= parentItem->children().size())
    throw Exception(TR_VP("The row for the layer item is out of range!"));

  te::qt::widgets::AbstractLayerTreeItem* item = dynamic_cast<te::qt::widgets::AbstractLayerTreeItem*>(parentItem->children().at(row));

  if(item == 0)
    throw Exception(TR_VP("The layer item is not an te::qt::widgets::AbstractLayerTreeItem!"));

  return createIndex(row, column, item);
}

QModelIndex te::vp::LayerTreeModel::parent(const QModelIndex& index) const
{
  if(!index.isValid())
    return QModelIndex();

  te::qt::widgets::AbstractLayerTreeItem* item = static_cast<te::qt::widgets::AbstractLayerTreeItem*>(index.internalPointer());

  if(item == 0 || item->parent() == 0)
    return QModelIndex();

  te::qt::widgets::AbstractLayerTreeItem* parentItem = dynamic_cast<te::qt::widgets::AbstractLayerTreeItem*>(item->parent());

  if(parentItem == 0)
    throw Exception(TR_VP("The layer item is not an AbstractLayerTreeItem!"));

  te::qt::widgets::AbstractLayerTreeItem* grandParentItem = dynamic_cast<te::qt::widgets::AbstractLayerTreeItem*>(parentItem->parent());

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

  throw Exception(TR_VP("Could not determine the layer index in the model!"));
}

int te::vp::LayerTreeModel::rowCount(const QModelIndex& parent) const
{
  if(!parent.isValid()) // if parent index isnot valid we assume we are asking for root items
    return static_cast<int>(m_items.size());

  te::qt::widgets::AbstractLayerTreeItem* item = static_cast<te::qt::widgets::AbstractLayerTreeItem*>(parent.internalPointer());

  if(item == 0)
    throw Exception(TR_VP("Error: NULL layer item!"));

  return item->children().count();
}

bool te::vp::LayerTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if(!index.isValid())
    return false;

  if(role == Qt::CheckStateRole)
    return false;

  te::qt::widgets::AbstractLayerTreeItem* item = static_cast<te::qt::widgets::AbstractLayerTreeItem*>(index.internalPointer());

  if(item == 0)
    return false;

  bool retval = item->setData(value, role);

  emit dataChanged(index, index);

  if(role == Qt::CheckStateRole)
  {
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