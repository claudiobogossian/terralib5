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
  \file terralib/qt/widgets/datasource/explorer/IndexItem.cpp

  \brief A class that represents an index in a DataSourceTreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/dataset/Index.h"
#include "../../Exception.h"
#include "IndexItem.h"
#include "PropertyItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::IndexItem::IndexItem(te::da::Index* idx, AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent),
    m_idx(idx)
{
}

te::qt::widgets::IndexItem::~IndexItem()
{
  //if(m_idx && (m_idx->getDataSetType() == 0))
  //  delete m_idx;
}

int te::qt::widgets::IndexItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::IndexItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("index"));

  if(role == Qt::DisplayRole)
    return QVariant(m_idx->getName().c_str());

  return QVariant();
}

QMenu* te::qt::widgets::IndexItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenIndex = m->addAction(tr("&Open index"));

  connect(aOpenIndex, SIGNAL(triggered()), this, SLOT(openIndex()));

  return m;
}

bool te::qt::widgets::IndexItem::canFetchMore() const
{
  if(!children().empty())
    return false;

  return hasChildren();
}

Qt::ItemFlags te::qt::widgets::IndexItem::flags() const
{
  return Qt::NoItemFlags;
}

void te::qt::widgets::IndexItem::fetchMore()
{
  const std::size_t nproperties = m_idx->getProperties().size();

  for(std::size_t i = 0; i < nproperties; ++i)
    new PropertyItem(m_idx->getProperties()[i], this);
}

bool te::qt::widgets::IndexItem::hasChildren() const
{
  return !m_idx->getProperties().empty();
}

bool te::qt::widgets::IndexItem::setData(const QVariant& /*value*/, int /*role*/)
{
  return false;
}

te::da::Index* te::qt::widgets::IndexItem::getIndex() const
{
  return m_idx;
}

