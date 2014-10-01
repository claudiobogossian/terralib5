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
  \file terralib/qt/widgets/layer/explorer/ChartItem.cpp

  \brief A class that represents a chart of a layer in a LayerTreeModel.
*/

// TerraLib
#include "../../../../maptools/Chart.h"
#include "../../Utils.h"
#include "ChartItem.h"
#include "ChartSliceItem.h"

// Qt
#include <QMenu>

// STL
#include <vector>

te::qt::widgets::ChartItem::ChartItem(te::map::Chart* chart, QObject* parent)
  : AbstractTreeItem(parent),
    m_chart(chart),
    m_isCheckable(true),
    m_isChecked(true)
{
}

te::qt::widgets::ChartItem::~ChartItem()
{
}

int te::qt::widgets::ChartItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::ChartItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("chart-pie"));

  if(role == Qt::DisplayRole)
  {
    switch(m_chart->getType())
    {
      case te::map::Pie:
        return QVariant(tr("Pie Chart"));
      
      case te::map::Bar:
        return QVariant(tr("Bar Chart"));

      default:
        return QVariant(tr("Chart"));
    }
  }

  if(role == Qt::CheckStateRole && m_isCheckable)
    return QVariant(m_isChecked ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::ChartItem::getMenu(QWidget* /*parent*/) const
{
  return 0;
}

bool te::qt::widgets::ChartItem::canFetchMore() const
{
  return !m_chart->getProperties().empty() && children().isEmpty();
}

Qt::ItemFlags te::qt::widgets::ChartItem::flags() const
{
  return (m_isCheckable ? (Qt::ItemIsEnabled | Qt::ItemIsUserCheckable) : Qt::ItemIsEnabled);
}

void te::qt::widgets::ChartItem::fetchMore()
{
   if(!children().isEmpty())
    return;

   int contourWidth = static_cast<int>(m_chart->getContourWidth());
   QColor contourColor = Convert2Qt(m_chart->getContourColor());

   const std::vector<std::string>& properties = m_chart->getProperties();

   for(std::size_t i = 0; i < properties.size(); ++i)
   {
     QColor color = Convert2Qt(m_chart->getColor(i));
     new ChartSliceItem(properties[i].c_str(), color, contourColor, contourWidth, this);
   }
}

bool te::qt::widgets::ChartItem::hasChildren() const
{
  return !m_chart->getProperties().empty();
}

bool te::qt::widgets::ChartItem::setData(int /*column*/, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole && m_isCheckable)
  {
    bool ok = false;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt(&ok));
    
    if(!ok)
      return false;

    m_isChecked = (checkState == Qt::Checked ? true : false);

    m_chart->setVisibility(m_isChecked);

    return true;
  }

  return false;
}

te::map::AbstractLayerPtr te::qt::widgets::ChartItem::getLayer() const
{
  return te::map::AbstractLayerPtr(0);
}

const std::string te::qt::widgets::ChartItem::getItemType() const
{
  return "CHART_ITEM";
}

void te::qt::widgets::ChartItem::setCheckable(bool checkable)
{
  m_isCheckable = checkable;
}

bool te::qt::widgets::ChartItem::getCheckable()
{
  return m_isCheckable;
}
