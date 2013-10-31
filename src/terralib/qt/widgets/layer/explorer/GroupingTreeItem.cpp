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
  \file terralib/qt/widgets/layer/explorer/GroupingTreeItem.cpp

  \brief A class that represents a grouping of a layer in a LayerTreeModel.
*/

// TerraLib
#include "../../../../maptools/Grouping.h"
#include "GroupingItemTreeItem.h"
#include "GroupingTreeItem.h"

// Qt
#include <QtGui/QMenu>
#include <QtGui/QWidget>

te::qt::widgets::GroupingTreeItem::GroupingTreeItem(te::map::Grouping* grouping, QObject* parent)
  : AbstractTreeItem(parent),
    m_grouping(grouping),
    m_isCheckable(true),
    m_isChecked(true)
{
}

te::qt::widgets::GroupingTreeItem::~GroupingTreeItem()
{
}

int te::qt::widgets::GroupingTreeItem::columnCount() const
{
  return 1;
}

QVariant te::qt::widgets::GroupingTreeItem::data(int /*column*/, int role) const
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

QMenu* te::qt::widgets::GroupingTreeItem::getMenu(QWidget* /*parent*/) const
{
  return 0;
}

bool te::qt::widgets::GroupingTreeItem::canFetchMore() const
{
  return !m_grouping->getGroupingItems().empty() && children().isEmpty();
}

Qt::ItemFlags te::qt::widgets::GroupingTreeItem::flags() const
{
  return (m_isCheckable ? (Qt::ItemIsEnabled | Qt::ItemIsUserCheckable) : Qt::ItemIsEnabled);
}

void te::qt::widgets::GroupingTreeItem::fetchMore()
{
  if(!children().isEmpty())
    return;

  const std::vector<te::map::GroupingItem*> items = m_grouping->getGroupingItems();
  for(std::size_t i = 0; i < items.size(); ++i)
    new GroupingItemTreeItem(items[i], this);
}

bool te::qt::widgets::GroupingTreeItem::hasChildren() const
{
  return !m_grouping->getGroupingItems().empty();
}

bool te::qt::widgets::GroupingTreeItem::setData(int /*column*/, const QVariant& value, int role)
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

te::map::AbstractLayerPtr te::qt::widgets::GroupingTreeItem::getLayer() const
{
  return te::map::AbstractLayerPtr(0);
}

te::qt::widgets::AbstractTreeItem::TreeItemType te::qt::widgets::GroupingTreeItem::getType() const
{
  return GROUPINGTREEITEM;
}

void te::qt::widgets::GroupingTreeItem::setCheckable(bool checkable)
{
  m_isCheckable = checkable;
}

bool te::qt::widgets::GroupingTreeItem::getCheckable()
{
  return m_isCheckable;
}
