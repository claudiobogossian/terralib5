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
  \file UniqueKeyGroupItem.cpp

  \brief A class used to group a set of unique keys from a dataset in a DataSourceTreeModel.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/dataset/UniqueKey.h"
#include "../../Exception.h"
#include "DataSetItem.h"
#include "UniqueKeyGroupItem.h"
#include "UniqueKeyItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::qt::widgets::UniqueKeyGroupItem::UniqueKeyGroupItem(AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent)
{
}

te::qt::widgets::UniqueKeyGroupItem::~UniqueKeyGroupItem()
{
}

int te::qt::widgets::UniqueKeyGroupItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::UniqueKeyGroupItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("key"));

  if(role == Qt::DisplayRole)
    return QVariant(QString("unique keys"));

  return QVariant();
}

QMenu* te::qt::widgets::UniqueKeyGroupItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenUniqueKeys = m->addAction(tr("&Open Unique keys"));

  connect(aOpenUniqueKeys, SIGNAL(triggered()), this, SLOT(openUniqueKeys()));

  return m;
}

bool te::qt::widgets::UniqueKeyGroupItem::canFetchMore() const
{
  if(!children().empty())
    return false;

  return hasChildren();
}

void te::qt::widgets::UniqueKeyGroupItem::fetchMore()
{
  DataSetItem* parentItem = dynamic_cast<DataSetItem*>(parent());

  if(parentItem == 0)
    return;

  te::da::DataSetTypePtr dt = parentItem->getDataSet();

  if(dt.get() == 0)
    return;

  const std::size_t nuks = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < nuks; ++i)
    new UniqueKeyItem(dt->getUniqueKey(i), this);
}

bool te::qt::widgets::UniqueKeyGroupItem::hasChildren() const
{
  DataSetItem* parentItem = dynamic_cast<DataSetItem*>(parent());

  if(parentItem == 0)
    return false;

  te::da::DataSetTypePtr dt = parentItem->getDataSet();

  if(dt.get() == 0)
    return false;

  return dt->getNumberOfUniqueKeys() != 0;
}

