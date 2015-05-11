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
  \file terralib/qt/widgets/layer/explorer/RasterLayerItem.cpp

  \brief The class that represents a dataset layer item in a LayerTreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../maptools/RasterLayer.h"
#include "../../../../se/Style.h"
#include "../../Exception.h"
#include "RasterLayerItem.h"
#include "LegendItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::RasterLayerItem::RasterLayerItem(const te::map::AbstractLayerPtr& l, QObject* parent)
  : AbstractTreeItem(parent)
{
  m_layer = boost::dynamic_pointer_cast<te::map::RasterLayer>(l);
}

te::qt::widgets::RasterLayerItem::~RasterLayerItem()
{
}

int te::qt::widgets::RasterLayerItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::RasterLayerItem::data(int /*column*/, int role) const
{
  //if(role == Qt::DecorationRole)
  //  return QVariant(QIcon::fromTheme("dataset-layer"));

  if(role == Qt::DisplayRole)
    return QVariant(QString::fromStdString(m_layer->getTitle()));

  if(role == Qt::CheckStateRole)
    return QVariant(m_layer->getVisibility() == te::map::VISIBLE ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::RasterLayerItem::getMenu(QWidget* /*parent*/) const
{
  //QMenu* m = new QMenu(parent);

  //QAction* aOpenDataSource = m->addAction(tr("&Open data source"));

  //connect(aOpenDataSource, SIGNAL(triggered()), this, SLOT(openDataSource()));

  //return m;
  return 0;
}

bool te::qt::widgets::RasterLayerItem::canFetchMore() const
{
  return children().isEmpty();
}

Qt::ItemFlags te::qt::widgets::RasterLayerItem::flags() const
{
  return Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;;
}

void te::qt::widgets::RasterLayerItem::fetchMore()
{
  if(!children().isEmpty())
    return;

  //if(m_layer->getStyle())
  //{
  //  const std::vector<te::se::Rule*>& rules = m_layer->getStyle()->getRules();

  //  for(std::size_t i = 0; i != rules.size(); ++i)
  //    new LegendItem(rules[i], this);
  //}
}

bool te::qt::widgets::RasterLayerItem::hasChildren() const
{
  return false;
}

bool te::qt::widgets::RasterLayerItem::setData(int column, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole)
  {
    bool ok = false;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt(&ok));

    if(!ok)
      return false;

    if(checkState == Qt::Checked)
      m_layer->setVisibility(te::map::VISIBLE);
    else if(checkState == Qt::Unchecked)
      m_layer->setVisibility(te::map::NOT_VISIBLE);

    return true;
  }

  return false;
}

te::map::AbstractLayerPtr te::qt::widgets::RasterLayerItem::getLayer() const
{
  return m_layer;
}

const std::string te::qt::widgets::RasterLayerItem::getItemType() const
{
  return "RASTER_LAYER_ITEM";
}
