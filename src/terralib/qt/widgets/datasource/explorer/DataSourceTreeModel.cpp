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
  \file terralib/qt/widgets/datasource/explorer/DataSourceTreeModel.cpp

  \brief ????
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "DataSetGroupItem.h"
#include "DataSourceItem.h"
#include "DataSourceTreeModel.h"

// Qt
#include <QMessageBox>
#include <QWidget>

te::qt::widgets::DataSourceTreeModel::DataSourceTreeModel(const std::vector<te::da::DataSourceInfoPtr>& datasources, QWidget* parent)
  : QAbstractItemModel(parent),
    m_checkable(false)
{
  for(std::size_t i = 0; i < datasources.size(); ++i)
  {
    if(datasources[i].get() != 0)
      m_items.push_back(new DataSourceItem(datasources[i], 0));
  }
}

te::qt::widgets::DataSourceTreeModel::DataSourceTreeModel(const te::da::DataSourceInfoPtr& ds, QObject* parent)
  : QAbstractItemModel(parent),
    m_checkable(false)
{
  m_items.push_back(new DataSetGroupItem(ds, 0));
}

te::qt::widgets::DataSourceTreeModel::~DataSourceTreeModel()
{
  te::common::FreeContents(m_items);
}

bool te::qt::widgets::DataSourceTreeModel::canFetchMore(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return !m_items.empty();

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(item == 0)
    return false;

  return item->canFetchMore();
}

int te::qt::widgets::DataSourceTreeModel::columnCount(const QModelIndex& /*parent*/) const
{
  return 1;
}

QVariant te::qt::widgets::DataSourceTreeModel::data(const QModelIndex& index, int role) const
{
  if(!index.isValid())
    return QVariant();

  if((role == Qt::CheckStateRole) && !m_checkable)
    return QVariant();

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0)
    return QVariant();

  try
  {
    return item->data(index.column(), role);
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(static_cast<QWidget*>(QObject::parent()),
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
  }
  catch(...)
  {
    QMessageBox::warning(static_cast<QWidget*>(QObject::parent()),
                         tr("TerraLib Qt Components"),
                         tr("Unknown error in data source explorer!"));
  }

  return QVariant();
}

void te::qt::widgets::DataSourceTreeModel::fetchMore(const QModelIndex& parent)
{
  if(!parent.isValid())
    return;

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(item == 0)
    return;

  try
  {
    item->fetchMore();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(static_cast<QWidget*>(QObject::parent()),
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
  }
  catch(...)
  {
    QMessageBox::warning(static_cast<QWidget*>(QObject::parent()),
                         tr("TerraLib Qt Components"),
                         tr("Unknown error in data source explorer!"));
  }
}

Qt::ItemFlags te::qt::widgets::DataSourceTreeModel::flags(const QModelIndex& index) const
{
  if(!index.isValid())
    return QAbstractItemModel::flags(index);

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0)
    return QAbstractItemModel::flags(index);

  try
  {
    return QAbstractItemModel::flags(index) | item->flags();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(static_cast<QWidget*>(QObject::parent()),
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
  }
  catch(...)
  {
    QMessageBox::warning(static_cast<QWidget*>(QObject::parent()),
                         tr("TerraLib Qt Components"),
                         tr("Unknown error in data source explorer!"));
  }

  return QAbstractItemModel::flags(index);
}

bool te::qt::widgets::DataSourceTreeModel::hasChildren(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return !m_items.empty();

  try
  {
    AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

    if(item == 0)
      return false;

    return item->hasChildren();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(static_cast<QWidget*>(QObject::parent()),
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
  }
  catch(...)
  {
    QMessageBox::warning(static_cast<QWidget*>(QObject::parent()),
                         tr("TerraLib Qt Components"),
                         tr("Unknown error in data source explorer!"));
  }

  return false;
}

QModelIndex te::qt::widgets::DataSourceTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if(m_items.empty())
    return QModelIndex();

  if(!parent.isValid()) // is it a top-level item?
  {
// yes!
    if(static_cast<std::size_t>(row) >= m_items.size())
      return QModelIndex();

    AbstractDataSourceTreeItem* item = m_items[row];

    return createIndex(row, column, item);
  }

  AbstractDataSourceTreeItem* parentItem = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(parentItem == 0 || parentItem->children().empty() || (row >= parentItem->children().size()))  // if there isn't a child, return an invalid index
    return QModelIndex();

  AbstractDataSourceTreeItem* item = dynamic_cast<AbstractDataSourceTreeItem*>(parentItem->children().at(row));

  if(item == 0)
    return QModelIndex();

  return createIndex(row, column, item);
}

QModelIndex te::qt::widgets::DataSourceTreeModel::parent(const QModelIndex& index) const
{
  if(!index.isValid())
    return QModelIndex();

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0 || item->parent() == 0)
    return QModelIndex();

  AbstractDataSourceTreeItem* parentItem = dynamic_cast<AbstractDataSourceTreeItem*>(item->parent());

  if(parentItem == 0)
    return QModelIndex();

  AbstractDataSourceTreeItem* grandParentItem = dynamic_cast<AbstractDataSourceTreeItem*>(parentItem->parent());

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

  return QModelIndex();
}

int te::qt::widgets::DataSourceTreeModel::rowCount(const QModelIndex& parent) const
{
  if(!parent.isValid()) // if parent index isnot valid we assume we are asking for root items
    return static_cast<int>(m_items.size());

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(item == 0)
    return 0;

  return item->children().count();
}

bool te::qt::widgets::DataSourceTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if(!index.isValid())
    return false;

  if(role == Qt::CheckStateRole && !m_checkable)
    return false;

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0)
    return false;

  try
  {
    bool retval = item->setData(value, role);

    emit dataChanged(index, index);

    return retval;
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(static_cast<QWidget*>(QObject::parent()),
                         tr("TerraLib Qt Components"),
                         tr(e.what()));
  }
  catch(...)
  {
    QMessageBox::warning(static_cast<QWidget*>(QObject::parent()),
                         tr("TerraLib Qt Components"),
                         tr("Unknown error in data source explorer!"));
  }

  return false;
}

void te::qt::widgets::DataSourceTreeModel::setCheckable(const bool checkable)
{
  m_checkable = checkable;
}

bool te::qt::widgets::DataSourceTreeModel::isCheckable() const
{
  return m_checkable;
}


