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
  \file terralib/qt/widgets/datasource/explorer/IndexGroupItem.cpp

  \brief A class used to group a set of indexes from a dataset in a TreeModel.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/dataset/Index.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../Exception.h"
#include "DataSetItem.h"
#include "IndexGroupItem.h"
#include "IndexItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::IndexGroupItem::IndexGroupItem(AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent)
{
}

te::qt::widgets::IndexGroupItem::~IndexGroupItem()
{
}

int te::qt::widgets::IndexGroupItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::IndexGroupItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("indexes"));

  if(role == Qt::DisplayRole)
    return QVariant(QString("indexes"));

  return QVariant();
}

QMenu* te::qt::widgets::IndexGroupItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenIndexes = m->addAction(tr("&Open indexes"));

  connect(aOpenIndexes, SIGNAL(triggered()), this, SLOT(openIndexes()));

  return m;
}

bool te::qt::widgets::IndexGroupItem::canFetchMore() const
{
  if(!children().empty())
    return false;

  return hasChildren();
}

Qt::ItemFlags te::qt::widgets::IndexGroupItem::flags() const
{
  return Qt::NoItemFlags;
}

void te::qt::widgets::IndexGroupItem::fetchMore()
{
  if(parent() == 0)
    return;

  DataSetItem* parentItem = dynamic_cast<DataSetItem*>(parent());

  if(parentItem == 0)
    return;

  const te::da::DataSetTypePtr& dt = parentItem->getDataSet();

  if(dt.get() == 0)
    return;

  //if(!dt->isFullLoaded() && (parentItem->getDataSource() != 0))
  //  te::da::LoadFull(dt.get(), parentItem->getDataSource());

// create new items
  const std::size_t nidxs = dt->getNumberOfIndexes();

  for(std::size_t i = 0; i < nidxs; ++i)
    new IndexItem(dt->getIndex(i), this);
}

bool te::qt::widgets::IndexGroupItem::hasChildren() const
{
  if(parent() == 0)
    return false;

  DataSetItem* parentItem = dynamic_cast<DataSetItem*>(parent());

  if(parentItem == 0)
    return false;

  const te::da::DataSetTypePtr& dt = parentItem->getDataSet();

  if(dt == 0)
    return false;

  //if(!dt->isFullLoaded() && (parentItem->getDataSource() != 0))
  //  te::da::LoadFull(dt.get(), parentItem->getDataSource());

  //return (dt->isFullLoaded() && (dt->getNumberOfIndexes() != 0));
  return dt->getNumberOfIndexes() != 0;
}

bool te::qt::widgets::IndexGroupItem::setData(const QVariant& /*value*/, int /*role*/)
{
  return false;
}

