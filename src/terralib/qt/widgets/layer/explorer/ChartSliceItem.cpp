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
  \file terralib/qt/widgets/layer/explorer/ChartSliceItem.cpp

  \brief A class that represents a chart slice in a LayerTreeModel.
*/

// TerraLib
#include "../../Utils.h"
#include "ChartSliceItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::ChartSliceItem::ChartSliceItem(const QString& propertyName, const QColor& color,
                                                const QColor& contourColor, int contourWidth,
                                                QObject* parent)
  : AbstractTreeItem(parent),
    m_propertyName(propertyName),
    m_color(color),
    m_contourColor(contourColor),
    m_contourWidth(contourWidth),
    m_size(16),
    m_isCheckable(false),
    m_isChecked(false)
{
}

te::qt::widgets::ChartSliceItem::~ChartSliceItem()
{
}

int te::qt::widgets::ChartSliceItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::ChartSliceItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon(CreatePixmapIcon(m_size, m_contourColor, m_color, m_contourWidth)));

  if(role == Qt::DisplayRole)
    return QVariant(tr("Property") + ": " + m_propertyName);

  if(role == Qt::CheckStateRole && m_isCheckable)
    return QVariant(m_isChecked ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::ChartSliceItem::getMenu(QWidget* /*parent*/) const
{
  return 0;
}

bool te::qt::widgets::ChartSliceItem::canFetchMore() const
{
  return false;
}

Qt::ItemFlags te::qt::widgets::ChartSliceItem::flags() const
{
  return (m_isCheckable ? (Qt::ItemIsEnabled | Qt::ItemIsUserCheckable) : Qt::ItemIsEnabled);
}

void te::qt::widgets::ChartSliceItem::fetchMore()
{
}

bool te::qt::widgets::ChartSliceItem::hasChildren() const
{
  return false;
}

bool te::qt::widgets::ChartSliceItem::setData(int /*column*/, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole && m_isCheckable)
  {
    bool ok = false;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt(&ok));
    
    if(!ok)
      return false;

    m_isChecked = (checkState == Qt::Checked ? true : false);

    return true;
  }

  return false;
}

te::map::AbstractLayerPtr te::qt::widgets::ChartSliceItem::getLayer() const
{
  return te::map::AbstractLayerPtr(0);
}

const std::string te::qt::widgets::ChartSliceItem::getItemType() const
{
  return "CHART_SLICE_ITEM";
}

void te::qt::widgets::ChartSliceItem::setCheckable(bool checkable)
{
  m_isCheckable = checkable;
}

bool te::qt::widgets::ChartSliceItem::getCheckable()
{
  return m_isCheckable;
}
