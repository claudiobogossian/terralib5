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
  \file ForeignKeyGroupItem.cpp

  \brief A class used to group a set of foreign keys from a dataset in a DataSourceTreeModel.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/dataset/ForeignKey.h"
#include "../../Exception.h"
#include "DataSetItem.h"
#include "ForeignKeyGroupItem.h"
#include "ForeignKeyItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::qt::widgets::ForeignKeyGroupItem::ForeignKeyGroupItem(AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent)
{
}

te::qt::widgets::ForeignKeyGroupItem::~ForeignKeyGroupItem()
{
}

int te::qt::widgets::ForeignKeyGroupItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::ForeignKeyGroupItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("foreignkey"));

  if(role == Qt::DisplayRole)
    return QVariant(QString("foreign keys"));

  return QVariant();
}

QMenu* te::qt::widgets::ForeignKeyGroupItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenForeignKeys = m->addAction(tr("&Open Foreign keys"));

  connect(aOpenForeignKeys, SIGNAL(triggered()), this, SLOT(openForeignKeys()));

  return m;
}

bool te::qt::widgets::ForeignKeyGroupItem::canFetchMore() const
{
  if(!children().empty())
    return false;

  return hasChildren();
}

void te::qt::widgets::ForeignKeyGroupItem::fetchMore()
{
  DataSetItem* parentItem = dynamic_cast<DataSetItem*>(parent());

  if(parentItem == 0)
    return;

  te::da::DataSetType* dt = parentItem->getDataSet();

  if(dt == 0 || dt->getNumberOfForeignKeys() == 0)
    return;

  const std::size_t nfks = dt->getNumberOfForeignKeys();

  for(std::size_t i = 0; i < nfks; ++i)
    new ForeignKeyItem(dt->getForeignKey(i), this);
}

bool te::qt::widgets::ForeignKeyGroupItem::hasChildren() const
{
  DataSetItem* parentItem = dynamic_cast<DataSetItem*>(parent());

  if(parentItem == 0)
    return false;

  te::da::DataSetType* dt = parentItem->getDataSet();

  if(dt == 0)
    return false;

  return dt->getNumberOfForeignKeys() != 0;
}

