/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/qt/LayerItem.cpp

  \brief ????
*/

// TerraLib
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../qt/widgets/datasource/explorer/PropertyItem.h"
#include "LayerItem.h"
#include "PropertyItem.h"

#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::vp::LayerItem::LayerItem(te::map::AbstractLayerPtr layer, QObject* parent)
  : te::qt::widgets::AbstractLayerTreeItem(parent),
    m_layer(layer)
{
}

te::vp::LayerItem::~LayerItem()
{

}

int te::vp::LayerItem::columnCount() const
{
  return 1;
}

QVariant te::vp::LayerItem::data(int column, int role) const
{
  if(role == Qt::DisplayRole)
    return m_layer->getTitle().c_str();
}

QMenu* te::vp::LayerItem::getMenu(QWidget* parent) const
{
  QMenu* m = new QMenu(parent);

  QAction* aOpenDataSource = m->addAction(tr("&Open layer"));

  connect(aOpenDataSource, SIGNAL(triggered()), this, SLOT(openDataSource()));

  return m;
}

bool te::vp::LayerItem::canFetchMore() const
{
  return m_layer->hasChildren();
}

void te::vp::LayerItem::fetchMore()
{
  if(parent() == 0)
    return;

// if parent is a dataset item we can get more data otherwise we can do nothing
  LayerItem* parentItem = dynamic_cast<LayerItem*>(parent());

  if(parentItem == 0)
    return;

  const te::map::LayerSchema* schema = parentItem->getLayer()->getSchema();

  if(schema == 0)
    return;

  const std::size_t nproperties = schema->size();

  for(std::size_t i = 0; i < nproperties; ++i)
  {
    te::dt::Property* p = schema->getProperty(i);

    if(p == 0)
      continue;

    new PropertyItem(p, this);
  }
}

Qt::ItemFlags te::vp::LayerItem::flags() const
{
  return 0;
}

bool te::vp::LayerItem::hasChildren() const
{
  return m_layer->hasChildren();
}

bool te::vp::LayerItem::setData(const QVariant& value, int role)
{
  return true;
}

te::map::AbstractLayerPtr te::vp::LayerItem::getLayer() const
{
  return m_layer;
}