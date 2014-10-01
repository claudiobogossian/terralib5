/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/explorer/GroupingSliceItem.cpp

  \brief A class that represents a grouping item of a grouping in a LayerTreeModel.
*/

// TerraLib
#include "../../../../maptools/GroupingItem.h"
#include "../../se/SymbologyPreview.h"
#include "GroupingSliceItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::GroupingSliceItem::GroupingSliceItem(te::map::GroupingItem* item, QObject* parent)
  : AbstractTreeItem(parent),
    m_item(item),
    m_isCheckable(false),
    m_isChecked(false)
{
}

te::qt::widgets::GroupingSliceItem::~GroupingSliceItem()
{
}

int te::qt::widgets::GroupingSliceItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::GroupingSliceItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon(SymbologyPreview::build(m_item->getSymbolizers(), QSize(16, 16))));

  if(role == Qt::DisplayRole)
    return QVariant(m_item->getTitle().c_str());

  if(role == Qt::CheckStateRole && m_isCheckable)
    return QVariant(m_isChecked ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::GroupingSliceItem::getMenu(QWidget* /*parent*/) const
{
  return 0;
}

bool te::qt::widgets::GroupingSliceItem::canFetchMore() const
{
  return false;
}

Qt::ItemFlags te::qt::widgets::GroupingSliceItem::flags() const
{
  return (m_isCheckable ? (Qt::ItemIsEnabled | Qt::ItemIsUserCheckable) : Qt::ItemIsEnabled);
}

void te::qt::widgets::GroupingSliceItem::fetchMore()
{
}

bool te::qt::widgets::GroupingSliceItem::hasChildren() const
{
  return false;
}

bool te::qt::widgets::GroupingSliceItem::setData(int /*column*/, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole && m_isCheckable)
  {
    bool ok = false;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt(&ok));
    
    if(!ok)
      return false;

    m_isChecked = (checkState == Qt::Checked ? true : false);
  }

  return false;
}

te::map::AbstractLayerPtr te::qt::widgets::GroupingSliceItem::getLayer() const
{
  return te::map::AbstractLayerPtr(0);
}

const std::string te::qt::widgets::GroupingSliceItem::getItemType() const
{
  return "GROUPING_SLICE_ITEM";
}

void te::qt::widgets::GroupingSliceItem::setCheckable(bool checkable)
{
  m_isCheckable = checkable;
}

bool te::qt::widgets::GroupingSliceItem::getCheckable()
{
  return m_isCheckable;
}
