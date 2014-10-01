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
  \file terralib/qt/widgets/datasource/explorer/DataSourceItem.cpp

  \brief A class that represents a data source in a DataSourceTreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../Exception.h"
#include "DataSetGroupItem.h"
#include "DataSourceItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::DataSourceItem::DataSourceItem(const te::da::DataSourceInfoPtr& ds, AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent),
    m_ds(ds)
{
}

te::qt::widgets::DataSourceItem::~DataSourceItem()
{
}

int te::qt::widgets::DataSourceItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::DataSourceItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
  {
    te::da::DataSourcePtr ds = te::da::DataSourceManager::getInstance().find(m_ds->getId());

    if(ds == 0 || !ds->isOpened())
      return QVariant(QIcon::fromTheme("closed-datasource"));
    else
      return QVariant(QIcon::fromTheme("datasource"));
  }

  if(role == Qt::DisplayRole)
    return QVariant(QString::fromStdString(m_ds->getTitle()));

  return QVariant();
}

QMenu* te::qt::widgets::DataSourceItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenDataSource = m->addAction(tr("&Open data source"));

  connect(aOpenDataSource, SIGNAL(triggered()), this, SLOT(openDataSource()));

  return m;
}

bool te::qt::widgets::DataSourceItem::canFetchMore() const
{
  if(m_ds.get() == 0)
    return false;

  if(!children().empty())
    return false;

  return true;
}

Qt::ItemFlags te::qt::widgets::DataSourceItem::flags() const
{
  return Qt::NoItemFlags;
}

void te::qt::widgets::DataSourceItem::fetchMore()
{
  if(m_ds.get() == 0)
    return;

  new DataSetGroupItem(m_ds, this); // add a child item
}

bool te::qt::widgets::DataSourceItem::hasChildren() const
{
  //return !children().empty();
  return true;
}

bool te::qt::widgets::DataSourceItem::setData(const QVariant& /*value*/, int /*role*/)
{
  return false;
}

te::da::DataSourceInfoPtr te::qt::widgets::DataSourceItem::getDataSource() const
{
  return m_ds;
}
