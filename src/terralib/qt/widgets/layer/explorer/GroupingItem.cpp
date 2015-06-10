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
  \file terralib/qt/widgets/layer/explorer/GroupingItem.cpp

  \brief A class that represents a grouping of a layer in a LayerTreeModel.
*/

// TerraLib
#include "../../../../maptools/Grouping.h"
#include "GroupingSliceItem.h"
#include "GroupingItem.h"

// Qt
#include <QMenu>
#include <QWidget>

te::qt::widgets::GroupingItem::GroupingItem(te::map::Grouping* grouping, QObject* parent)
  : AbstractTreeItem(parent),
    m_grouping(grouping),
    m_isCheckable(true),
    m_isChecked(true)
{
}

te::qt::widgets::GroupingItem::~GroupingItem()
{
}

int te::qt::widgets::GroupingItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::GroupingItem::data(int /*column*/, int role) const
{
  if(role == Qt::DecorationRole)
    return QVariant(QIcon::fromTheme("grouping"));

  if(role == Qt::DisplayRole)
  {
    QString text = tr("Classification by") + " " + m_grouping->getPropertyName().c_str();
    return QVariant(text);
  }

  if(role == Qt::CheckStateRole && m_isCheckable)
    return QVariant(m_isChecked ? Qt::Checked : Qt::Unchecked);

  return QVariant();
}

QMenu* te::qt::widgets::GroupingItem::getMenu(QWidget* /*parent*/) const
{
  return 0;
}

bool te::qt::widgets::GroupingItem::canFetchMore() const
{
  return !m_grouping->getGroupingItems().empty() && children().isEmpty();
}

Qt::ItemFlags te::qt::widgets::GroupingItem::flags() const
{
  return (m_isCheckable ? (Qt::ItemIsEnabled | Qt::ItemIsUserCheckable) : Qt::ItemIsEnabled);
}

void te::qt::widgets::GroupingItem::fetchMore()
{
  if(!children().isEmpty())
    return;

  const std::vector<te::map::GroupingItem*> items = m_grouping->getGroupingItems();
  for(std::size_t i = 0; i < items.size(); ++i)
    new GroupingSliceItem(items[i], this);
}

bool te::qt::widgets::GroupingItem::hasChildren() const
{
  return !m_grouping->getGroupingItems().empty();
}

bool te::qt::widgets::GroupingItem::setData(int /*column*/, const QVariant& value, int role)
{
  if(role == Qt::CheckStateRole && m_isCheckable)
  {
    bool ok = false;
    Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt(&ok));
    
    if(!ok)
      return false;

    m_isChecked = (checkState == Qt::Checked ? true : false);

    m_grouping->setVisibility(m_isChecked);

    return true;
  }

  return false;
}

te::map::AbstractLayerPtr te::qt::widgets::GroupingItem::getLayer() const
{
  return te::map::AbstractLayerPtr(0);
}

const std::string te::qt::widgets::GroupingItem::getItemType() const
{
  return "GROUPING_ITEM";
}

void te::qt::widgets::GroupingItem::setCheckable(bool checkable)
{
  m_isCheckable = checkable;
}

bool te::qt::widgets::GroupingItem::getCheckable()
{
  return m_isCheckable;
}
