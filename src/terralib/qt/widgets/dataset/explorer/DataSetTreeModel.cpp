/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/dataset/explorer/DataSetTreeModel.cpp

  \brief ????
*/

// TerraLib
#include "../../datasource/explorer/DataSetGroupItem.h"
#include "DataSetTreeModel.h"

// STL
#include <memory>

// Qt
#include <QtGui/QMessageBox>
#include <QtGui/QWidget>

te::qt::widgets::DataSetTreeModel::DataSetTreeModel(const te::da::DataSourceInfoPtr& datasource, QWidget* parent)
  : QAbstractItemModel(parent),
    m_checkable(false),
    m_forceCatalogCache(false)
{
  m_datasets.reset(new DataSetGroupItem(datasource, 0));
}

te::qt::widgets::DataSetTreeModel::~DataSetTreeModel()
{
}

bool te::qt::widgets::DataSetTreeModel::canFetchMore(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return false;

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(item == 0)
    return false;

  return item->canFetchMore();
}

int te::qt::widgets::DataSetTreeModel::columnCount(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return 1; // DataSetGroupItem is the root

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(item == 0)
    return 0;

  return item->columnCount();
}

QVariant te::qt::widgets::DataSetTreeModel::data(const QModelIndex& index, int role) const
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
                         tr("Unknown error in dataset explorer model!"));
  }

  return QVariant();
}

void te::qt::widgets::DataSetTreeModel::fetchMore(const QModelIndex& parent)
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
                         tr("Unknown error in dataset explorer model!"));
  }
}

Qt::ItemFlags te::qt::widgets::DataSetTreeModel::flags(const QModelIndex& index) const
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
                         tr("Unknown error in dataset explorer model!"));
  }

  return Qt::ItemIsEnabled;
}

bool te::qt::widgets::DataSetTreeModel::hasChildren(const QModelIndex& parent) const
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
                         tr("Unknown error in dataset explorer model!"));
  }

  return false;
}

QModelIndex te::qt::widgets::DataSetTreeModel::index(int row, int column, const QModelIndex& parent) const
{
  if(!parent.isValid()) // is it the top-level item? we have just one descendent from root!
  {
    if(m_datasets.get() == 0 || (row != 0))
      return QModelIndex();

    AbstractDataSourceTreeItem* item = m_datasets.get();

    return createIndex(row, column, item);
  }

  AbstractDataSourceTreeItem* parentItem = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(parentItem == 0 || parentItem->children().empty())  // if there isn't a child, return an invalid index
    return QModelIndex();

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parentItem->children().at(row));

  return createIndex(row, column, item);
}

QModelIndex te::qt::widgets::DataSetTreeModel::parent(const QModelIndex& index) const
{
  if(!index.isValid())
    return QModelIndex();

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0 || item->parent() == 0)
    return QModelIndex();

  return createIndex(index.row(), index.column(), static_cast<AbstractDataSourceTreeItem*>(item->parent()));
}

int te::qt::widgets::DataSetTreeModel::rowCount(const QModelIndex& parent) const
{
  if(!parent.isValid()) // if parent index is not valid we assume we are asking for root item
    return 1;

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(item == 0)
    return 0;

  return item->children().count();
}

bool te::qt::widgets::DataSetTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
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
                         tr("Unknown error in dataset explorer model!"));
  }

  return false;
}

void te::qt::widgets::DataSetTreeModel::setCheckable(const bool checkable)
{
  m_checkable = checkable;
}

bool te::qt::widgets::DataSetTreeModel::isCheckable() const
{
  return m_checkable;
}

