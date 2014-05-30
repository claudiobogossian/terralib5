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
  \file terralib/qt/widgets/datasource/explorer/PropertyGroupItem.cpp

  \brief A class used to group a set of properties from a dataset in a TreeModel.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../../../datatype/Property.h"
#include "../../Exception.h"
#include "DataSetItem.h"
#include "PropertyGroupItem.h"
#include "PropertyItem.h"

// STL
#include <memory>

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::PropertyGroupItem::PropertyGroupItem(AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent)
{
}

te::qt::widgets::PropertyGroupItem::~PropertyGroupItem()
{
}

int te::qt::widgets::PropertyGroupItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::PropertyGroupItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("properties"));

  if(role == Qt::DisplayRole)
    return QVariant(QString("properties"));

  return QVariant();
}

QMenu* te::qt::widgets::PropertyGroupItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenProperties = m->addAction(tr("&Open properties"));

  connect(aOpenProperties, SIGNAL(triggered()), this, SLOT(openProperties()));

  return m;
}

bool te::qt::widgets::PropertyGroupItem::canFetchMore() const
{
  if(!children().empty())
    return false;

  return hasChildren();
}

Qt::ItemFlags te::qt::widgets::PropertyGroupItem::flags() const
{
  return Qt::NoItemFlags;
}

void te::qt::widgets::PropertyGroupItem::fetchMore()
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

  const std::size_t nproperties = dt->size();

  for(std::size_t i = 0; i < nproperties; ++i)
  {
    te::dt::Property* p = dt->getProperty(i);

    if(p == 0)
      continue;

    new PropertyItem(p, this);
  }
}

bool te::qt::widgets::PropertyGroupItem::hasChildren() const
{
  if(parent() == 0)
    return false;

  DataSetItem* parentItem = dynamic_cast<DataSetItem*>(parent());

  if(parentItem == 0)
    return false;

  const te::da::DataSetTypePtr& dt = parentItem->getDataSet();

  if(dt.get() == 0)
    return false;

  return dt->size() != 0;
}

bool te::qt::widgets::PropertyGroupItem::setData(const QVariant& /*value*/, int /*role*/)
{
  return false;
}

