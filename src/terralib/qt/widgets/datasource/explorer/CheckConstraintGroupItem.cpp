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
  \file terralib/myqt/widgets/dataset/explorer/CheckConstraintGroupItem.cpp

  \brief A class used to group a set of check-constraints from a dataset in a TreeModel.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/CheckConstraint.h"
#include "../../../../dataaccess/dataset/DataSetType.h"
#include "../../Exception.h"
#include "CheckConstraintGroupItem.h"
#include "CheckConstraintItem.h"
#include "DataSetItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::qt::widgets::CheckConstraintGroupItem::CheckConstraintGroupItem(AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent)
{
}

te::qt::widgets::CheckConstraintGroupItem::~CheckConstraintGroupItem()
{
}

int te::qt::widgets::CheckConstraintGroupItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::CheckConstraintGroupItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("constraints"));
  
  if(role == Qt::DisplayRole)
    return QVariant(QString("check-constraints"));

  return QVariant();
}

QMenu* te::qt::widgets::CheckConstraintGroupItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenCheckConstraints = m->addAction(tr("&Open Check-constraints"));

  connect(aOpenCheckConstraints, SIGNAL(triggered()), this, SLOT(openCheckConstraints()));

  return m;
}

bool te::qt::widgets::CheckConstraintGroupItem::canFetchMore() const
{
  if(!children().empty())
    return false;

  return hasChildren();
}

void te::qt::widgets::CheckConstraintGroupItem::fetchMore()
{
  DataSetItem* parentItem = dynamic_cast<DataSetItem*>(parent());

  if(parentItem == 0)
    return;

  te::da::DataSetType* dt = parentItem->getDataSet();

  if(dt == 0 || dt->getNumberOfCheckConstraints() == 0)
    return;

  const std::size_t nccs = dt->getNumberOfCheckConstraints();

  for(std::size_t i = 0; i < nccs; ++i)
    new CheckConstraintItem(dt->getCheckConstraint(i), this);
}

bool te::qt::widgets::CheckConstraintGroupItem::hasChildren() const
{
  DataSetItem* parentItem = dynamic_cast<DataSetItem*>(parent());

  if(parentItem == 0)
    return false;

  te::da::DataSetType* dt = parentItem->getDataSet();

  if(dt == 0)
    return false;

  return dt->getNumberOfCheckConstraints() != 0;
}

