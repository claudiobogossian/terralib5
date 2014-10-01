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
#include "../../datasource/explorer/DataSetCategoryGroupItem.h"
#include "../../datasource/explorer/DataSetItem.h"
//#include "DataSetTreeModel.h"
#include "DataSetCategoryModel.h"
#include "DataSetTreeModel.h"
#include "DataSetTreeView.h"

// STL
#include <memory>

// Qt
#include <QMenu>

te::qt::widgets::DataSetTreeView::DataSetTreeView(QWidget* parent)
  : QTreeView(parent),
    m_categoryModel(0),
    m_treeModel(0),
    m_ds(te::da::DataSourceInfoPtr()),
    m_isCategoryModel(true),
    m_useCheckableItems(true)
{
  this->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(this, SIGNAL(activated(const QModelIndex&)), this, SLOT(onItemActivated(const QModelIndex&)));
  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onItemClicked(const QModelIndex&)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onItemDoubleClicked(const QModelIndex&)));
  connect(this, SIGNAL(entered(const QModelIndex&)), this, SLOT(onItemEntered(const QModelIndex&)));
  connect(this, SIGNAL(pressed(const QModelIndex&)), this, SLOT(onItemPressed(const QModelIndex&)));
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(customContextMenu(const QPoint&)));
}

te::qt::widgets::DataSetTreeView::~DataSetTreeView()
{
}

void te::qt::widgets::DataSetTreeView::set(const te::da::DataSourceInfoPtr& ds, bool useCheckableItems)
{
  m_ds = ds;

  m_treeModel.reset(new DataSetTreeModel(ds, this));
  m_categoryModel.reset(new DataSetCategoryModel(ds, this));

  m_useCheckableItems = useCheckableItems;

  m_treeModel->setCheckable(m_useCheckableItems);
  m_categoryModel->setCheckable(m_useCheckableItems);
  
  this->setModel(m_treeModel.get());

  m_isCategoryModel = true;
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

  DataSetCategoryGroupItem* categoryItem = dynamic_cast<DataSetCategoryGroupItem*>(item);

  if(categoryItem != 0)
  {
    emit activated(categoryItem);
    return;
  }

  DataSetGroupItem* groupItem = dynamic_cast<DataSetGroupItem*>(item);

  if(groupItem != 0)
    emit activated(groupItem);
}

void te::qt::widgets::DataSetTreeView::onItemClicked(const QModelIndex & index)
{
  AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(index.internalPointer());

  if(item == 0)
    return;

  QAbstractItemModel* model = this->model();

  DataSetItem* ditem = dynamic_cast<DataSetItem*>(item);

  if(ditem != 0)
  {
    emit clicked(ditem);

    if(model && !m_useCheckableItems)
      return;

    QVariant value = item->data(0, Qt::CheckStateRole);

    if(value.isNull())
      return;

    emit toggled(ditem);

    return;
  }

  DataSetCategoryGroupItem* categoryItem = dynamic_cast<DataSetCategoryGroupItem*>(item);

  if(categoryItem != 0)
  {
    emit clicked(categoryItem);

    if(model && !m_useCheckableItems)
      return;

    if(model && !m_useCheckableItems)
      return;

    QVariant value = item->data(0, Qt::CheckStateRole);

    if(value.isNull())
      return;

    emit toggled(categoryItem);

    return;
  }

  DataSetGroupItem* groupItem = dynamic_cast<DataSetGroupItem*>(item);

  if(groupItem != 0)
  {
    emit clicked(groupItem);

    if(model && !m_useCheckableItems)
      return;

    if(model && !m_useCheckableItems)
      return;

    QVariant value = item->data(0, Qt::CheckStateRole);

    if(value.isNull())
      return;

    emit toggled(groupItem);

    return;
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

  DataSetCategoryGroupItem* categoryItem = dynamic_cast<DataSetCategoryGroupItem*>(item);

  if(categoryItem != 0)
  {
    emit doubleClicked(categoryItem);
    return;
  }

  DataSetGroupItem* groupItem = dynamic_cast<DataSetGroupItem*>(item);

  if(groupItem != 0)
    emit doubleClicked(groupItem);
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

  DataSetCategoryGroupItem* categoryItem = dynamic_cast<DataSetCategoryGroupItem*>(item);

  if(categoryItem != 0)
  {
    emit entered(categoryItem);
    return;
  }

  DataSetGroupItem* groupItem = dynamic_cast<DataSetGroupItem*>(item);

  if(groupItem != 0)
    emit entered(groupItem);
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

  DataSetCategoryGroupItem* categoryItem = dynamic_cast<DataSetCategoryGroupItem*>(item);

  if(categoryItem != 0)
  {
    emit pressed(categoryItem);
    return;
  }

  DataSetGroupItem* groupItem = dynamic_cast<DataSetGroupItem*>(item);

  if(groupItem != 0)
    emit pressed(groupItem);
}

void te::qt::widgets::DataSetTreeView::customContextMenu(const QPoint &point)
{
  QMenu *menu = new QMenu("", this);

  QAction * organize = new QAction(tr("Organize by category"), menu);
  organize->setCheckable(true);

  if(m_isCategoryModel)
    organize->setChecked(true);

  connect(organize, SIGNAL(toggled(bool)), this, SLOT(onModelToggled(bool)));
  menu->addAction(organize);
  menu->exec(QCursor::pos());
}

void te::qt::widgets::DataSetTreeView::onModelToggled(bool checked)
{
  if(checked)
    this->setModel(m_categoryModel.get());
  else
    this->setModel(m_treeModel.get());

  m_isCategoryModel = checked;

  QAbstractItemModel* nmodel = model();

  QModelIndex idx = nmodel->index(0, 0);

  expand(idx);

  if(m_isCategoryModel)
  {
    AbstractDataSourceTreeItem* item = static_cast<AbstractDataSourceTreeItem*>(idx.internalPointer());

    for(int i = 0; i < item->children().size(); i++)
    {
      QModelIndex idxChild = nmodel->index(i, 0, idx);
      expand(idxChild);
    }
  }
}