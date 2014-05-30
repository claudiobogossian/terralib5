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
  \file terralib/qt/widgets/dataset/explorer/DataSetCategoryModel.cpp

  \brief ????
*/

// TerraLib
#include "../../../../dataaccess/Enums.h"
#include "../../datasource/explorer/DataSetCategoryGroupItem.h"
#include "DataSetCategoryModel.h"

// STL
#include <memory>

// Qt
#include <QMessageBox>
#include <QWidget>

te::qt::widgets::DataSetCategoryModel::DataSetCategoryModel(const te::da::DataSourceInfoPtr& datasource, QWidget* parent)
  : QAbstractItemModel(parent),
    m_datasets(0),
    m_checkable(false),
    m_forceCatalogCache(false)
{
  if(datasource.get() != 0)
    m_datasets = new DataSetCategoryGroupItem(datasource, 0);
}

te::qt::widgets::DataSetCategoryModel::~DataSetCategoryModel()
{
  delete m_datasets;
}

bool te::qt::widgets::DataSetCategoryModel::canFetchMore(const QModelIndex& parent) const
{
  if(!parent.isValid())
    return false;

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(item == 0)
    return false;

  return item->canFetchMore();
}

int te::qt::widgets::DataSetCategoryModel::columnCount(const QModelIndex& parent) const
{
  return 1;
}

QVariant te::qt::widgets::DataSetCategoryModel::data(const QModelIndex& index, int role) const
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

void te::qt::widgets::DataSetCategoryModel::fetchMore(const QModelIndex& parent)
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

Qt::ItemFlags te::qt::widgets::DataSetCategoryModel::flags(const QModelIndex& index) const
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
                         tr("Unknown error in dataset explorer model!"));
  }

  return QAbstractItemModel::flags(index);
}

bool te::qt::widgets::DataSetCategoryModel::hasChildren(const QModelIndex& parent) const
{
  try
  {
    if(!parent.isValid())
      return true;

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

QModelIndex te::qt::widgets::DataSetCategoryModel::index(int row, int column, const QModelIndex& parent) const
{
  if(m_datasets == 0)
    return QModelIndex();
  
  if(!parent.isValid()) // is it the top-level item? we have just one descendent from root!
  {
    if(row != 0)
      return QModelIndex();

    AbstractDataSourceTreeItem* item = m_datasets;

    return createIndex(row, column, item);
  }

  AbstractDataSourceTreeItem* parentItem = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(parentItem == 0 || parentItem->children().empty())  // if there isn't a child, return an invalid index
    return QModelIndex();

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parentItem->children().at(row));

  if(item == 0)
    return QModelIndex();

  return createIndex(row, column, item);
}

QModelIndex te::qt::widgets::DataSetCategoryModel::parent(const QModelIndex& index) const
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
    return createIndex(0, index.column(), parentItem);
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

int te::qt::widgets::DataSetCategoryModel::rowCount(const QModelIndex& parent) const
{
  if(!parent.isValid()) // if parent index is not valid we assume we are asking for root item
    return 1;

  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(parent.internalPointer());

  if(item == 0)
    return 0;

  return item->children().count();
}

bool te::qt::widgets::DataSetCategoryModel::setData(const QModelIndex& index, const QVariant& value, int role)
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

void te::qt::widgets::DataSetCategoryModel::setCheckable(const bool checkable)
{
  m_checkable = checkable;
}

bool te::qt::widgets::DataSetCategoryModel::isCheckable() const
{
  return m_checkable;
}

