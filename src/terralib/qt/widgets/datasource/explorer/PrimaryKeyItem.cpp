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
  \file terralib/qt/widgets/datasource/explorer/PrimaryKeyItem.cpp

  \brief A class that represents a primary key in a TreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/PrimaryKey.h"
#include "../../Exception.h"
#include "PrimaryKeyItem.h"
#include "PropertyItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::PrimaryKeyItem::PrimaryKeyItem(te::da::PrimaryKey* pk, AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent),
    m_pk(pk)
{
}

te::qt::widgets::PrimaryKeyItem::~PrimaryKeyItem()
{
  //if(m_pk && (m_pk->getDataSetType() == 0))
  //  delete m_pk;
}

int te::qt::widgets::PrimaryKeyItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::PrimaryKeyItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("primarykey"));

  if(role == Qt::DisplayRole)
    return QVariant(m_pk->getName().c_str());

  return QVariant();
}

QMenu* te::qt::widgets::PrimaryKeyItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenPK = m->addAction(tr("&Open primary key"));

  connect(aOpenPK, SIGNAL(triggered()), this, SLOT(openPrimaryKey()));

  return m;
}

bool te::qt::widgets::PrimaryKeyItem::canFetchMore() const
{
  if(!children().empty())
    return false;

  return hasChildren();
}

Qt::ItemFlags te::qt::widgets::PrimaryKeyItem::flags() const
{
  return Qt::NoItemFlags;
}

void te::qt::widgets::PrimaryKeyItem::fetchMore()
{
  const std::size_t nproperties = m_pk->getProperties().size();

  for(std::size_t i = 0; i < nproperties; ++i)
    new PropertyItem(m_pk->getProperties()[i], this);
}

bool te::qt::widgets::PrimaryKeyItem::hasChildren() const
{
  return !m_pk->getProperties().empty();
}

bool te::qt::widgets::PrimaryKeyItem::setData(const QVariant& /*value*/, int /*role*/)
{
  return false;
}

te::da::PrimaryKey* te::qt::widgets::PrimaryKeyItem::getPrimaryKey() const
{
  return m_pk;
}

