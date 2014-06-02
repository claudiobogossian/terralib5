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
  \file terralib/qt/widgets/datasource/explorer/UniqueKeyItem.cpp

  \brief A class that represents a unique key in a TreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/UniqueKey.h"
#include "../../Exception.h"
#include "PropertyItem.h"
#include "UniqueKeyItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::UniqueKeyItem::UniqueKeyItem(te::da::UniqueKey* uk, AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent),
    m_uk(uk)
{
}

te::qt::widgets::UniqueKeyItem::~UniqueKeyItem()
{
  //if(m_uk && (m_uk->getDataSetType() == 0))
  //  delete m_uk;
}

int te::qt::widgets::UniqueKeyItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::UniqueKeyItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("unique"));

  if(role == Qt::DisplayRole)
    return QVariant(m_uk->getName().c_str());

  return QVariant();
}

QMenu* te::qt::widgets::UniqueKeyItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenUniqueKey = m->addAction(tr("&Open unique-key"));

  connect(aOpenUniqueKey, SIGNAL(triggered()), this, SLOT(openUniqueKey()));

  return m;
}

bool te::qt::widgets::UniqueKeyItem::canFetchMore() const
{
  if(!children().empty())
    return false;

  return hasChildren();
}

Qt::ItemFlags te::qt::widgets::UniqueKeyItem::flags() const
{
  return Qt::NoItemFlags;
}

void te::qt::widgets::UniqueKeyItem::fetchMore()
{
  const std::size_t nproperties = m_uk->getProperties().size();

  for(std::size_t i = 0; i < nproperties; ++i)
    new PropertyItem(m_uk->getProperties()[i], this);
}

bool te::qt::widgets::UniqueKeyItem::hasChildren() const
{
  return !m_uk->getProperties().empty();
}

bool te::qt::widgets::UniqueKeyItem::setData(const QVariant& /*value*/, int /*role*/)
{
  return false;
}

te::da::UniqueKey* te::qt::widgets::UniqueKeyItem::getUniqueKey() const
{
  return m_uk;
}

