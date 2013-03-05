/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
#include "../../../../dataaccess/datasource/DataSource.h"
#include "DataSetGroupItem.h"
#include "DataSourceItem.h"
#include "DataSourceTreeModel.h"

// Qt
#include <QtGui/QMessageBox>
#include <QtGui/QWidget>

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
}

bool te::qt::widgets::DataSourceTreeModel::canFetchMore(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return false;

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(item == 0)
    return false;

  return item->canFetchMore();
}

int te::qt::widgets::DataSourceTreeModel::columnCount(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return 1; // DataSourceItem is the root

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(item == 0)
    return 0;

  return item->columnCount();
}

QVariant te::qt::widgets::DataSourceTreeModel::data(const QModelIndex& index, int role) const
{
  if(!index.isValid())
    return QVariant();

  if(role == Qt::CheckStateRole && !m_checkable)
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
    return Qt::ItemIsEnabled;

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0)
    return Qt::ItemIsEnabled;

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

  return Qt::ItemIsEnabled;
}

bool te::qt::widgets::DataSourceTreeModel::hasChildren(const QModelIndex& parent) const
{
  try
  {
    if(!parent.isValid())
      return true;        // root items are always expansibles!

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
  if(!parent.isValid()) // is it a top-level item?
  {
// yes!
    if(m_items.empty() || (static_cast<std::size_t>(row) >= m_items.size()))
      return QModelIndex();

    AbstractDataSourceTreeItem* item = const_cast<AbstractDataSourceTreeItem*>(&m_items[row]);

    return createIndex(row, column, item);
  }

  AbstractDataSourceTreeItem* parentItem = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(parentItem == 0 || parentItem->children().empty())  // if there isn't a child, return an invalid index
    return QModelIndex();

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parentItem->children().at(row));

  return createIndex(row, column, item);
}

QModelIndex te::qt::widgets::DataSourceTreeModel::parent(const QModelIndex& index) const
{
  if(!index.isValid())
    return QModelIndex();

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0 || item->parent() == 0)
    return QModelIndex();

  return createIndex(index.row(), index.column(), static_cast<AbstractDataSourceTreeItem*>(item->parent()));
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


