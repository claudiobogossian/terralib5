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
  \file terralib/qt/widgets/datasource/explorer/ConstraintsItem.h

  \brief A class used to group the set of constraints of a dataset in a TreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceCatalog.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../datatype/Property.h"
#include "../../Exception.h"
#include "DataSetItem.h"
#include "CheckConstraintItem.h"
#include "ConstraintsItem.h"
#include "PrimaryKeyItem.h"
#include "PropertyItem.h"
#include "UniqueKeyItem.h"

// STL
#include <memory>

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::ConstraintsItem::ConstraintsItem(AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent)
{
}

te::qt::widgets::ConstraintsItem::~ConstraintsItem()
{
}

int te::qt::widgets::ConstraintsItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::ConstraintsItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("constraints"));

  if(role == Qt::DisplayRole)
    return QVariant(QString("constraints"));

  return QVariant();
}

QMenu* te::qt::widgets::ConstraintsItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenProperties = m->addAction(tr("&Open constraints"));

  connect(aOpenProperties, SIGNAL(triggered()), this, SLOT(openProperties()));

  return m;
}

bool te::qt::widgets::ConstraintsItem::canFetchMore() const
{
  if(!children().empty())
    return false;

  return hasChildren();
}

Qt::ItemFlags te::qt::widgets::ConstraintsItem::flags() const
{
  return Qt::NoItemFlags;
}

void te::qt::widgets::ConstraintsItem::fetchMore()
{
  if(parent() == 0)
    return;

// if parent is a dataset item we can get more data otherwise we can do nothing
  DataSetItem* parentItem = dynamic_cast<DataSetItem*>(parent());

  if(parentItem == 0)
    return;

  const te::da::DataSetTypePtr& dt = parentItem->getDataSet();

  if(dt.get() == 0)
    return;

  //if(!dt->isFullLoaded() && (parentItem->getDataSource() != 0))
  //  te::da::LoadFull(dt.get(), parentItem->getDataSource());

  if(dt->getPrimaryKey() != 0)
    new PrimaryKeyItem(dt->getPrimaryKey(), this);

  const std::size_t nuks = dt->getNumberOfUniqueKeys();

  for(std::size_t i = 0; i < nuks; ++i)
    new UniqueKeyItem(dt->getUniqueKey(i), this);

  const std::size_t nccs = dt->getNumberOfCheckConstraints();

  for(std::size_t i = 0; i < nccs; ++i)
    new CheckConstraintItem(dt->getCheckConstraint(i), this);
}

bool te::qt::widgets::ConstraintsItem::hasChildren() const
{
  if(parent() == 0)
    return false;

  DataSetItem* parentItem = dynamic_cast<DataSetItem*>(parent());

  if(parentItem == 0)
    return false;

  const te::da::DataSetTypePtr& dt = parentItem->getDataSet();

  if(dt.get() == 0)
    return false;

  //if(!dt->isFullLoaded() && (parentItem->getDataSource() != 0))
  //  te::da::LoadFull(dt.get(), parentItem->getDataSource());

  return (dt->getPrimaryKey() != 0) ||
         (dt->getNumberOfUniqueKeys() != 0) ||
         (dt->getNumberOfCheckConstraints() != 0);
}

bool te::qt::widgets::ConstraintsItem::setData(const QVariant& /*value*/, int /*role*/)
{
  return false;
}

