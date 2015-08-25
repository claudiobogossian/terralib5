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
* \file layer/explorer/GroupingItem.cpp
*/
#include "GroupingItem.h"

// TerraLib
#include "../../../../color/RGBAColor.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../maptools/Grouping.h"
#include "../../../../maptools/GroupingItem.h"

#include "LayerItem.h"
#include "LegendItem.h"

// Qt
#include <QObject>

void AddSliceItems(te::qt::widgets::GroupingItem* item, const te::map::Grouping* g)
{
  const std::vector<te::map::GroupingItem*> items = g->getGroupingItems();

  for(std::vector<te::map::GroupingItem*>::const_iterator it = items.begin(); it != items.end(); ++it)
    item->addChild(new te::qt::widgets::LegendItem((*it)->getTitle(), (*it)->getSymbolizers()));
}

te::qt::widgets::GroupingItem::GroupingItem(te::map::Grouping* g) :
  TreeItem("GROUPING"),
  m_grouping(g)
{
  m_label = (QObject::tr("Classification by") + " " + m_grouping->getPropertyName().c_str()).toStdString();

  AddSliceItems(this, m_grouping);
}

te::qt::widgets::GroupingItem::~GroupingItem()
{
  if(m_parent == 0)
    return;

  te::map::AbstractLayerPtr layer = ((LayerItem*)m_parent)->getLayer();

  te::map::Grouping* group = layer->getGrouping();

  if(group == m_grouping)
    layer->setGrouping(0);
}

std::string te::qt::widgets::GroupingItem::getAsString() const
{
  return m_label;
}

te::qt::widgets::VISIBLE te::qt::widgets::GroupingItem::isVisible() const
{
  return m_grouping->isVisible() ? TOTALLY : NONE;
}

void te::qt::widgets::GroupingItem::setVisible(const VISIBLE& visible, const bool&, const bool&)
{
  m_grouping->setVisibility(visible == TOTALLY ? true : false);
}

Qt::ItemFlags te::qt::widgets::GroupingItem::flags()
{
  return TreeItem::flags() | Qt::ItemIsUserCheckable;
}
