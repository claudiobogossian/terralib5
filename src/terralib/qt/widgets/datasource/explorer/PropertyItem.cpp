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
  \file terralib/qt/widgets/datasource/explorer/PropertyItem.cpp

  \brief A class that represents a dataset in a TreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../datatype/Property.h"
#include "../../Exception.h"
#include "PropertyGroupItem.h"
#include "PropertyItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::PropertyItem::PropertyItem(te::dt::Property* p, AbstractDataSourceTreeItem* parent)
  : AbstractDataSourceTreeItem(parent),
    m_property(p)
{
}

te::qt::widgets::PropertyItem::~PropertyItem()
{
  //if(m_property && (m_property->getParent() == 0))
  //  delete m_property;
}

int te::qt::widgets::PropertyItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::PropertyItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("property"));

  if(role == Qt::DisplayRole)
    return QVariant(m_property->getName().c_str());

  return QVariant();
}

QMenu* te::qt::widgets::PropertyItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenProperty = m->addAction(tr("&Open property"));

  connect(aOpenProperty, SIGNAL(triggered()), this, SLOT(openProperty()));

  return m;
}

bool te::qt::widgets::PropertyItem::canFetchMore() const
{
  return false;
}

Qt::ItemFlags te::qt::widgets::PropertyItem::flags() const
{
  return Qt::NoItemFlags;
}

void te::qt::widgets::PropertyItem::fetchMore()
{
}

bool te::qt::widgets::PropertyItem::hasChildren() const
{
  return false;
}

bool te::qt::widgets::PropertyItem::setData(const QVariant& /*value*/, int /*role*/)
{
  return false;
}

te::dt::Property* te::qt::widgets::PropertyItem::getProperty() const
{
  return m_property;
}
