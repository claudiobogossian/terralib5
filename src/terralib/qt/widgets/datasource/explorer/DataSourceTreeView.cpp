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
  \file DataSourceTreeView.cpp

  \brief A tree view for the data sources of an application.
*/

// TerraLib
#include "AbstractDataSourceTreeItem.h"
#include "DataSourceTreeModel.h"
#include "DataSourceTreeView.h"

te::qt::widgets::DataSourceTreeView::DataSourceTreeView(QWidget* parent)
  : QTreeView(parent)
{
  connect(this, SIGNAL(activated(const QModelIndex&)), this, SLOT(itemActivated(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(itemClicked(const QModelIndex&)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(itemDoubleClicked(const QModelIndex&)));
  connect(this, SIGNAL(entered(const QModelIndex&)), this, SLOT(itemEntered(const QModelIndex&)));
  connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(itemPressed(const QModelIndex&)));
}

te::qt::widgets::DataSourceTreeView::~DataSourceTreeView()
{
}

void te::qt::widgets::DataSourceTreeView::itemActivated(const QModelIndex & index)
{
  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  emit activated(item);
}

void te::qt::widgets::DataSourceTreeView::itemClicked(const QModelIndex & index)
{
  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  emit clicked(item);

  DataSourceTreeModel* model = dynamic_cast<DataSourceTreeModel*>(this->model());

  if(!model->isCheckable())
    return;

  QVariant value = item->data(0, Qt::CheckStateRole);

  if(value.isNull())
    return;

  emit toggled(item, value.toBool());
}

void te::qt::widgets::DataSourceTreeView::itemDoubleClicked(const QModelIndex & index)
{
  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  emit doubleClicked(item);
}

void te::qt::widgets::DataSourceTreeView::itemEntered(const QModelIndex & index)
{
  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  emit entered(item);
}

void te::qt::widgets::DataSourceTreeView::itemPressed(const QModelIndex & index)
{
  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  emit pressed(item);
}

