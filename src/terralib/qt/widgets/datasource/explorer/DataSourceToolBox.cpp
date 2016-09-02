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
  \file DataSourceToolBox.cpp

  \brief A tool box to help explorer data sources.
*/

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../core/DataSourceType.h"
#include "../core/DataSourceTypeManager.h"
#include "DataSourceToolBox.h"
#include "DataSourceTreeModel.h"
#include "DataSourceTreeView.h"

te::qt::widgets::DataSourceToolBox::DataSourceToolBox(QWidget * parent, Qt::WindowFlags f)
  : QToolBox(parent, f)
{
  DataSourceTypeManager::const_iterator it = DataSourceTypeManager::getInstance().begin();

  DataSourceTypeManager::const_iterator itend = DataSourceTypeManager::getInstance().end();

  while(it != itend)
  {
    DataSourceTreeView* treeView = new DataSourceTreeView(this);
    
    treeView->setHeaderHidden(true);
    treeView->setIndentation(TE_QTWIDGETS_DEFAULT_TREEVIEW_IDENTATION);

    std::vector<te::da::DataSourceInfoPtr> datasources;

    te::da::DataSourceInfoManager::getInstance().getByType(it->first, datasources);

    DataSourceTreeModel* treeModel = new DataSourceTreeModel(datasources, this);

    treeView->setModel(treeModel);

    QString dsTypeTitle = QString::fromUtf8(it->second->getTitle().c_str());

    addItem(treeView, dsTypeTitle);

    connect(treeView, SIGNAL(activated(AbstractDataSourceTreeItem*)), this, SLOT(itemActivated(AbstractDataSourceTreeItem*)));
    connect(treeView, SIGNAL(clicked(AbstractDataSourceTreeItem*)), this, SLOT(itemClicked(AbstractDataSourceTreeItem*)));
    connect(treeView, SIGNAL(doubleClicked(AbstractDataSourceTreeItem*)), this, SLOT(itemDoubleClicked(AbstractDataSourceTreeItem*)));
    connect(treeView, SIGNAL(entered(AbstractDataSourceTreeItem*)), this, SLOT(itemEntered(AbstractDataSourceTreeItem*)));
    connect(treeView, SIGNAL(pressed(AbstractDataSourceTreeItem*)), this, SLOT(itemPressed(AbstractDataSourceTreeItem*)));

    ++it;
  }
}

te::qt::widgets::DataSourceToolBox::~DataSourceToolBox()
{
  int nmodels = count();

  for(int i = 0; i< nmodels; ++i)
  {
    QWidget* w = widget(i);

    DataSourceTreeView* tview = dynamic_cast<DataSourceTreeView*>(w);

    if(tview == 0)
      continue;

    QAbstractItemModel* model = tview->model();

    tview->setModel(0);

    delete model;
  }
}

void te::qt::widgets::DataSourceToolBox::itemActivated(AbstractDataSourceTreeItem* item)
{
  emit activated(item);
}

void te::qt::widgets::DataSourceToolBox::itemClicked(AbstractDataSourceTreeItem* item)
{
  emit clicked(item);
}

void te::qt::widgets::DataSourceToolBox::itemDoubleClicked(AbstractDataSourceTreeItem* item)
{
  emit doubleClicked(item);
}

void te::qt::widgets::DataSourceToolBox::itemEntered(AbstractDataSourceTreeItem* item)
{
  emit entered(item);
}

void te::qt::widgets::DataSourceToolBox::itemPressed(AbstractDataSourceTreeItem* item)
{
  emit pressed(item);
}

