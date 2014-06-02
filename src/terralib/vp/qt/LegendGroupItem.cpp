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
  \file terralib/vp/qt/LegendGroupItem.cpp

  \brief A class that represents a group of legends in a LayerTreeModel.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../datatype/Property.h"
#include "../../qt/widgets/layer/explorer/LegendItem.h"
#include "../../se/Rule.h"
#include "../Exception.h"
#include "LegendGroupItem.h"

// Qt
#include <QWidget>

te::vp::LegendGroupItem::LegendGroupItem(std::vector<te::se::Rule*> rules, AbstractTreeItem* parent)
  : AbstractTreeItem(parent)
{
  if(rules.empty())
    return;

  for(size_t i = 0; i < rules.size(); ++i)
  {
    te::se::Rule* rule = rules[i];


    te::qt::widgets::LegendItem* item = new te::qt::widgets::LegendItem(rule, this);
    item->setCheckable(true);

    m_items.push_back(item);
  }
}

te::vp::LegendGroupItem::~LegendGroupItem()
{
}

int te::vp::LegendGroupItem::columnCount() const
{
  return 2;
}

QVariant te::vp::LegendGroupItem::data(int column, int role) const
{
  if(role == Qt::DisplayRole && column == 0)
    return QVariant(TE_TR("Legends"));

  return QVariant();
}

QMenu* te::vp::LegendGroupItem::getMenu(QWidget* parent) const
{
  return 0;
}

bool te::vp::LegendGroupItem::canFetchMore() const
{
  if(!children().empty ())
    return false;

  return hasChildren();
}

Qt::ItemFlags te::vp::LegendGroupItem::flags() const
{
  return Qt::ItemIsUserCheckable;
}

void te::vp::LegendGroupItem::fetchMore()
{
}

bool te::vp::LegendGroupItem::hasChildren() const
{
  return m_items.size() > 0 ? true : false;
}

bool te::vp::LegendGroupItem::setData(int column, const QVariant& value, int role)
{
  return true;
}

te::map::AbstractLayerPtr te::vp::LegendGroupItem::getLayer() const
{
  return 0;
}

std::vector<te::qt::widgets::AbstractTreeItem*> te::vp::LegendGroupItem::getItems() const
{
  return m_items;
}

const std::string te::vp::LegendGroupItem::getItemType() const
{
  return "LEGEND_ITEM";
}
