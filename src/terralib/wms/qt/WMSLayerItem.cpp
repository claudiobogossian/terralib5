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
  \file terralib/wms/qt/WMSLayerItem.cpp

  \brief The class that represents a WMS layer item in a LayerTreeModel.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../qt/widgets/layer/explorer/ColorMapItem.h"
#include "../../qt/widgets/layer/explorer/LegendItem.h"
#include "../../se/RasterSymbolizer.h"
#include "../../se/Style.h"
#include "../../se/Utils.h"
#include "../Exception.h"
#include "WMSLayerItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>

// STL
#include <map>

te::wms::WMSLayerItem::WMSLayerItem(const te::map::AbstractLayerPtr& l, QObject* parent)
  : te::qt::widgets::AbstractTreeItem(parent)
{
  m_layer = boost::dynamic_pointer_cast<WMSLayer>(l);
}

te::wms::WMSLayerItem::~WMSLayerItem()
{
}

int te::wms::WMSLayerItem::columnCount() const
{
  return 1;
}

QVariant te::wms::WMSLayerItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("wms-layer"));

  if(role == Qt::DisplayRole)
    return QVariant(QString::fromStdString(m_layer->getTitle()));

  if(role == Qt::CheckStateRole)
    return QVariant(m_layer->getVisibility() == te::map::VISIBLE ? Qt::Checked : Qt::Unchecked);

  if(role == Qt::ToolTipRole)
    return buildToolTip();

  return QVariant();
}

QMenu* te::wms::WMSLayerItem::getMenu(QWidget* /*parent*/) const
{
  return 0;
}

bool te::wms::WMSLayerItem::canFetchMore() const
{
  return ((m_layer->getStyle() != 0) && (!m_layer->getStyle()->getRules().empty()));
}

Qt::ItemFlags te::wms::WMSLayerItem::flags() const
{
  return Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

void te::wms::WMSLayerItem::fetchMore()
{
  if(m_layer->getStyle() && children().empty())
  {
    const std::vector<te::se::Rule*>& rules = m_layer->getStyle()->getRules();

    for(std::size_t i = 0; i != rules.size(); ++i)
      new te::qt::widgets::LegendItem(rules[i], this);
  }

  if(m_layer->getStyle())
  {
    te::se::RasterSymbolizer* rs = te::se::GetRasterSymbolizer(m_layer->getStyle());

    if(rs && rs->getColorMap() && !hasColorMapItem())
      new te::qt::widgets::ColorMapItem(rs->getColorMap(), this);
  }
}

bool te::wms::WMSLayerItem::hasChildren() const
{
  return (m_layer->getStyle() != 0) && (!m_layer->getStyle()->getRules().empty());
}

bool te::wms::WMSLayerItem::setData(int column, const QVariant& value, int role)
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

te::map::AbstractLayerPtr te::wms::WMSLayerItem::getLayer() const
{
  return m_layer;
}

const std::string te::wms::WMSLayerItem::getItemType() const
{
  return "WMS_LAYER_ITEM";
}

bool te::wms::WMSLayerItem::hasColorMapItem() const
{
  te::qt::widgets::ColorMapItem* cmi = findChild<te::qt::widgets::ColorMapItem*>();

  return cmi != 0;
}

QString te::wms::WMSLayerItem::buildToolTip() const
{
  // Gets the connection info
  const std::string& id = m_layer->getDataSourceId();
  te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(id);
  const std::map<std::string, std::string>& connInfo = info->getConnInfo();

  QString toolTip;

  std::size_t i = 0;
  std::map<std::string, std::string>::const_iterator it;
  for(it = connInfo.begin(); it != connInfo.end(); ++it)
  {
    toolTip += it->first.c_str();
    toolTip += " = ";
    toolTip += it->second.c_str();
    ++i;
    if(i != connInfo.size())
      toolTip += "\n";
  }

  return toolTip;
}
