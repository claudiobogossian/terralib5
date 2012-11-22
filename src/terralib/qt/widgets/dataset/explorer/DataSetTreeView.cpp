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
  \file terralib/qt/widgets/dataset/explorer/DataSetTreeView.cpp

  \brief A tree view for datasets of a data source.
*/

// TerraLib
#include "../../datasource/explorer/DataSetGroupItem.h"
#include "../../datasource/explorer/DataSetItem.h"
#include "DataSetTreeModel.h"
#include "DataSetTreeView.h"

// STL
#include <memory>

te::qt::widgets::DataSetTreeView::DataSetTreeView(QWidget* parent)
  : QTreeView(parent)
{
  connect(this, SIGNAL(activated(const QModelIndex&)), this, SLOT(onItemActivated(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onItemClicked(const QModelIndex&)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onItemDoubleClicked(const QModelIndex&)));
  connect(this, SIGNAL(entered(const QModelIndex&)), this, SLOT(onItemEntered(const QModelIndex&)));
  connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(onItemPressed(const QModelIndex&)));
}

te::qt::widgets::DataSetTreeView::~DataSetTreeView()
{
}

void te::qt::widgets::DataSetTreeView::set(const te::da::DataSourceInfoPtr& ds, bool useCheckableItems)
{
  std::auto_ptr<DataSetTreeModel> model(new DataSetTreeModel(ds, this));

  model->setCheckable(useCheckableItems);

  this->setModel(model.release());
}

std::list<te::qt::widgets::DataSetItem*> te::qt::widgets::DataSetTreeView::getSelectedDataSets() const
{
  std::list<DataSetItem*> ditems;

  QModelIndexList items = selectedIndexes();

  for(QModelIndexList::iterator it = items.begin(); it != items.end(); ++it)
  {
    AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(it->internalPointer());

    if(item == 0)
      continue;

    DataSetItem* ditem = dynamic_cast<DataSetItem*>(item);

    if(ditem)
      ditems.push_back(ditem);
  }

  return ditems;
}

bool te::qt::widgets::DataSetTreeView::hasSelectedDataSets() const
{
  QModelIndexList items = selectedIndexes();

  for(QModelIndexList::iterator it = items.begin(); it != items.end(); ++it)
  {
    AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(it->internalPointer());

    if(item == 0)
      continue;

    DataSetItem* ditem = dynamic_cast<DataSetItem*>(item);

    if(ditem)
      return true;
  }

  return false;
}

void te::qt::widgets::DataSetTreeView::onItemActivated(const QModelIndex & index)
{
  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0)
    return;

  DataSetItem* ditem = dynamic_cast<DataSetItem*>(item);

  if(ditem != 0)
  {
    emit activated(ditem);
    return;
  }

  DataSetGroupItem* dgitem = dynamic_cast<DataSetGroupItem*>(item);

  if(dgitem != 0)
    emit activated(dgitem);
}

void te::qt::widgets::DataSetTreeView::onItemClicked(const QModelIndex & index)
{
  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0)
    return;

  DataSetTreeModel* model = dynamic_cast<DataSetTreeModel*>(this->model());

  DataSetItem* ditem = dynamic_cast<DataSetItem*>(item);

  if(ditem != 0)
  {
    emit clicked(ditem);

    if(model && !model->isCheckable())
      return;

    QVariant value = item->data(0, Qt::CheckStateRole);

    if(value.isNull())
      return;

    emit toggled(ditem);
  }
  else
  {
    DataSetGroupItem* dgitem = dynamic_cast<DataSetGroupItem*>(item);

    if(dgitem != 0)
    {
      emit clicked(dgitem);

      if(model && !model->isCheckable())
        return;

      if(model && !model->isCheckable())
        return;

      QVariant value = item->data(0, Qt::CheckStateRole);

      if(value.isNull())
        return;

      emit toggled(dgitem);
    }
  }
}

void te::qt::widgets::DataSetTreeView::onItemDoubleClicked(const QModelIndex & index)
{
  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0)
    return;

  DataSetItem* ditem = dynamic_cast<DataSetItem*>(item);

  if(ditem != 0)
  {
    emit doubleClicked(ditem);
    return;
  }

  DataSetGroupItem* dgitem = dynamic_cast<DataSetGroupItem*>(item);

  if(dgitem != 0)
    emit doubleClicked(dgitem);
}

void te::qt::widgets::DataSetTreeView::onItemEntered(const QModelIndex & index)
{
  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0)
    return;

  DataSetItem* ditem = dynamic_cast<DataSetItem*>(item);

  if(ditem != 0)
  {
    emit entered(ditem);
    return;
  }

  DataSetGroupItem* dgitem = dynamic_cast<DataSetGroupItem*>(item);

  if(dgitem != 0)
    emit entered(dgitem);
}

void te::qt::widgets::DataSetTreeView::onItemPressed(const QModelIndex & index)
{
  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0)
    return;

  DataSetItem* ditem = dynamic_cast<DataSetItem*>(item);

  if(ditem != 0)
  {
    emit pressed(ditem);
    return;
  }

  DataSetGroupItem* dgitem = dynamic_cast<DataSetGroupItem*>(item);

  if(dgitem != 0)
    emit pressed(dgitem);
}

