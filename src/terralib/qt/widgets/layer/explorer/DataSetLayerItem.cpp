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
  \file terralib/qt/widgets/layer/explorer/DataSetLayerItem.cpp

  \brief The class that represents a dataset layer item in a LayerTreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../Exception.h"
#include "DataSetLayerItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::qt::widgets::DataSetLayerItem::DataSetLayerItem(const te::map::AbstractLayerPtr& l, QObject* parent)
  : AbstractLayerTreeItem(parent)
{
  m_layer = boost::dynamic_pointer_cast<te::map::DataSetLayer>(l);
}

te::qt::widgets::DataSetLayerItem::~DataSetLayerItem()
{
}

int te::qt::widgets::DataSetLayerItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::DataSetLayerItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("dataset-layer"));

  if(role == Qt::DisplayRole)
    return QVariant(QString::fromUtf8(m_layer->getTitle().c_str()));

  if(role == Qt::CheckStateRole)
    return QVariant(m_layer->getVisibility() == te::map::VISIBLE ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::DataSetLayerItem::getMenu(QWidget* /*parent*/) const
{
  //QMenu* m = new QMenu(parent);

  //QAction* aOpenDataSource = m->addAction(tr("&Open data source"));

  //connect(aOpenDataSource, SIGNAL(triggered()), this, SLOT(openDataSource()));

  //return m;
  return 0;
}

bool te::qt::widgets::DataSetLayerItem::canFetchMore() const
{
  return false;
}

Qt::ItemFlags te::qt::widgets::DataSetLayerItem::flags() const
{
  return Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;;
}

void te::qt::widgets::DataSetLayerItem::fetchMore()
{
}

bool te::qt::widgets::DataSetLayerItem::hasChildren() const
{
  return false;
}

bool te::qt::widgets::DataSetLayerItem::setData(const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole)
  {
    bool vis = value.toBool();

    m_layer->setVisibility(vis ? te::map::VISIBLE : te::map::NOT_VISIBLE);
    return true;
  }

  return false;
}

te::qt::widgets::AbstractLayerTreeItem* te::qt::widgets::DataSetLayerItem::clone(QObject* parent)
{
  return new DataSetLayerItem(m_layer, parent);
}

const te::map::DataSetLayerPtr& te::qt::widgets::DataSetLayerItem::getLayer() const
{
  return m_layer;
}
