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
  \file terralib/qt/widgets/layer/explorer/TimeSeriesLayerItem.cpp

  \brief The class that represents a dataset layer item in a LayerTreeModel.
*/

// TerraLib
#include "../../../common/Translator.h"
#include "../../../se/Style.h"
#include "../../../qt/widgets/Exception.h"
#include "../../../qt/widgets/layer/explorer/ChartItem.h"
#include "../../../qt/widgets/layer/explorer/GroupingItem.h"
#include "../../../qt/widgets/layer/explorer/LegendItem.h"
#include "TimeSeriesLayerItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::plugins::st::TimeSeriesLayerItem::TimeSeriesLayerItem(const te::map::AbstractLayerPtr& l, QObject* parent)
  : te::qt::widgets::AbstractTreeItem(parent)
{
  m_layer = boost::dynamic_pointer_cast<te::st::TimeSeriesDataSetLayer>(l);
}

te::qt::plugins::st::TimeSeriesLayerItem::~TimeSeriesLayerItem()
{
}

int te::qt::plugins::st::TimeSeriesLayerItem::columnCount() const
{
  return 1;
}

QVariant te::qt::plugins::st::TimeSeriesLayerItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("timeseries-layer"));

  if(role == Qt::DisplayRole)
    return QVariant(QString::fromStdString(m_layer->getTitle()));

  if(role == Qt::CheckStateRole)
    return QVariant(m_layer->getVisibility() == te::map::VISIBLE ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::plugins::st::TimeSeriesLayerItem::getMenu(QWidget* /*parent*/) const
{
  return 0;
}

bool te::qt::plugins::st::TimeSeriesLayerItem::canFetchMore() const
{
  return (((m_layer->getStyle() != 0) && (!m_layer->getStyle()->getRules().empty())) || m_layer->getGrouping() != 0 || m_layer->getChart() != 0);
}

Qt::ItemFlags te::qt::plugins::st::TimeSeriesLayerItem::flags() const
{
  return Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

void te::qt::plugins::st::TimeSeriesLayerItem::fetchMore()
{
  if(m_layer->getStyle() && children().empty())
  {
    const std::vector<te::se::Rule*>& rules = m_layer->getStyle()->getRules();

    for(std::size_t i = 0; i != rules.size(); ++i)
      new te::qt::widgets::LegendItem(rules[i], this);
  }

  if(m_layer->getGrouping() && !hasGroupingItem())
    new te::qt::widgets::GroupingItem(m_layer->getGrouping(), this);

  if(m_layer->getChart() && !hasChartItem())
    new te::qt::widgets::ChartItem(m_layer->getChart(), this);
}

bool te::qt::plugins::st::TimeSeriesLayerItem::hasChildren() const
{
  return ((m_layer->getStyle() != 0) && (!m_layer->getStyle()->getRules().empty())) || m_layer->getGrouping() != 0 || m_layer->getChart() != 0;
}

bool te::qt::plugins::st::TimeSeriesLayerItem::setData(int column, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole)
  {
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt());

    if(checkState == Qt::Checked)
      m_layer->setVisibility(te::map::VISIBLE);
    else if(checkState == Qt::Unchecked)
      m_layer->setVisibility(te::map::NOT_VISIBLE);

    m_layer->updateVisibilityOfAncestors();

    return true;
  }

  return false;
}

te::map::AbstractLayerPtr te::qt::plugins::st::TimeSeriesLayerItem::getLayer() const
{
  return m_layer;
}

const std::string te::qt::plugins::st::TimeSeriesLayerItem::getItemType() const
{
  return "TimeSeries_LAYER_ITEM";
}

bool te::qt::plugins::st::TimeSeriesLayerItem::hasGroupingItem() const
{
  te::qt::widgets::GroupingItem* groupingItem = findChild<te::qt::widgets::GroupingItem*>();

  return groupingItem != 0;
}

bool te::qt::plugins::st::TimeSeriesLayerItem::hasChartItem() const
{
  te::qt::widgets::ChartItem* chartItem = findChild<te::qt::widgets::ChartItem*>();

  return chartItem != 0;
}
