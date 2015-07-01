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
  \file terralib/qt/widgets/datasource/explorer/ForeignKeyItem.cpp

  \brief A class that represents a foreign key in a TreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/ForeignKey.h"
#include "../../Exception.h"
#include "ForeignKeyItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::ForeignKeyItem::ForeignKeyItem(te::da::ForeignKey* fk, AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent),
    m_fk(fk)
{
}

te::qt::widgets::ForeignKeyItem::~ForeignKeyItem()
{
  //if(m_fk && (m_fk->getDataSetType() == 0))
  //  delete m_fk;
}

int te::qt::widgets::ForeignKeyItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::ForeignKeyItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("foreignkey"));

  if(role == Qt::DisplayRole)
    return QVariant(m_fk->getName().c_str());

  return QVariant();
}

QMenu* te::qt::widgets::ForeignKeyItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenForeignKey = m->addAction(tr("&Open foreign-key"));

  connect(aOpenForeignKey, SIGNAL(triggered()), this, SLOT(openForeignKey()));

  return m;
}

bool te::qt::widgets::ForeignKeyItem::canFetchMore() const
{
  return false;
}

Qt::ItemFlags te::qt::widgets::ForeignKeyItem::flags() const
{
  return Qt::NoItemFlags;
}

void te::qt::widgets::ForeignKeyItem::fetchMore()
{
}

bool te::qt::widgets::ForeignKeyItem::hasChildren() const
{
  return false;
}

bool te::qt::widgets::ForeignKeyItem::setData(const QVariant& /*value*/, int /*role*/)
{
  return false;
}

te::da::ForeignKey* te::qt::widgets::ForeignKeyItem::getForeignKey() const
{
  return m_fk;
}

