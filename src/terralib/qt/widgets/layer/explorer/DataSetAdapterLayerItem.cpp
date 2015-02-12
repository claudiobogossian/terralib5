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
  \file terralib/qt/widgets/layer/explorer/DataSetAdapterLayerItem.cpp

  \brief The class that represents a dataset layer item in a LayerTreeModel.
*/

// TerraLib
#include "../../../../common/Translator.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../maptools/DataSetAdapterLayer.h"
#include "../../../../se/Style.h"
#include "../../Exception.h"
#include "ChartItem.h"
#include "DataSetAdapterLayerItem.h"
#include "GroupingItem.h"
#include "LegendItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::DataSetAdapterLayerItem::DataSetAdapterLayerItem(const te::map::AbstractLayerPtr& l, QObject* parent)
  : AbstractTreeItem(parent)
{
  m_layer = boost::dynamic_pointer_cast<te::map::DataSetAdapterLayer>(l);
}

te::qt::widgets::DataSetAdapterLayerItem::~DataSetAdapterLayerItem()
{
}

int te::qt::widgets::DataSetAdapterLayerItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::DataSetAdapterLayerItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
  {
    if(m_layer->isValid())
      return QVariant(QIcon::fromTheme("dataset-layer-tabular"));
    else
      return QVariant(QIcon::fromTheme("dataset-layer-invalid"));
  }

  if(role == Qt::DisplayRole)
    return QVariant(QString::fromStdString(m_layer->getTitle()));

  if(role == Qt::CheckStateRole)
    return QVariant(m_layer->getVisibility() == te::map::VISIBLE ? Qt::Checked : Qt::Unchecked);

  if(role == Qt::ToolTipRole)
    return buildToolTip();

  return QVariant();
}

QMenu* te::qt::widgets::DataSetAdapterLayerItem::getMenu(QWidget* /*parent*/) const
{
  return 0;
}

bool te::qt::widgets::DataSetAdapterLayerItem::canFetchMore() const
{
  return (((m_layer->getStyle() != 0) && (!m_layer->getStyle()->getRules().empty())) || m_layer->getGrouping() != 0 || m_layer->getChart() != 0);
}

Qt::ItemFlags te::qt::widgets::DataSetAdapterLayerItem::flags() const
{
  return Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

void te::qt::widgets::DataSetAdapterLayerItem::fetchMore()
{
  if(m_layer->getStyle() && children().empty())
  {
    const std::vector<te::se::Rule*>& rules = m_layer->getStyle()->getRules();

    for(std::size_t i = 0; i != rules.size(); ++i)
      new LegendItem(rules[i], this);
  }

  if(m_layer->getGrouping() && !hasGroupingItem())
    new GroupingItem(m_layer->getGrouping(), this);

  if(m_layer->getChart() && !hasChartItem())
    new ChartItem(m_layer->getChart(), this);
}

bool te::qt::widgets::DataSetAdapterLayerItem::hasChildren() const
{
  return ((m_layer->getStyle() != 0) && (!m_layer->getStyle()->getRules().empty())) || m_layer->getGrouping() != 0 || m_layer->getChart() != 0;
}

bool te::qt::widgets::DataSetAdapterLayerItem::setData(int column, const QVariant& value, int role)
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

te::map::AbstractLayerPtr te::qt::widgets::DataSetAdapterLayerItem::getLayer() const
{
  return m_layer;
}

const std::string te::qt::widgets::DataSetAdapterLayerItem::getItemType() const
{
  return "DATASET_ADAPTER_LAYER_ITEM";
}

bool te::qt::widgets::DataSetAdapterLayerItem::hasGroupingItem() const
{
  GroupingItem* groupingItem = findChild<GroupingItem*>();

  return groupingItem != 0;
}

bool te::qt::widgets::DataSetAdapterLayerItem::hasChartItem() const
{
  ChartItem* chartItem = findChild<ChartItem*>();

  return chartItem != 0;
}

QString te::qt::widgets::DataSetAdapterLayerItem::buildToolTip() const
{
  if(!m_layer->isValid())
    return tr("Invalid Layer");

  QString toolTip;

  // Gets the data set name
  toolTip += tr("DataSet") + ": " + m_layer->getDataSetName().c_str() + "\n";

  // Gets the connection info
  const std::string& id = m_layer->getDataSourceId();
  te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(id);
  const std::map<std::string, std::string>& connInfo = info->getConnInfo();

  toolTip += tr("Connection Info") + ":\n";

  std::size_t i = 0;
  std::map<std::string, std::string>::const_iterator it;
  for(it = connInfo.begin(); it != connInfo.end(); ++it)
  {
    toolTip += it->first.c_str();
    toolTip += ": ";
    toolTip += it->second.c_str();
    ++i;
    if(i != connInfo.size())
      toolTip += "\n";
  }

  return toolTip;
}