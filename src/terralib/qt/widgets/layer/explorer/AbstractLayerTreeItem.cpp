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
  \file terralib/qt/widgets/layer/explorer/AbstractLayerTreeItem.cpp

  \brief The class that represents an item in a LayerTreeModel.
*/

// TerraLib
#include "AbstractLayerTreeItem.h"

te::qt::widgets::AbstractLayerTreeItem::AbstractLayerTreeItem(QObject* parent)
  : QObject(parent)
{
}

te::qt::widgets::AbstractLayerTreeItem::~AbstractLayerTreeItem()
{
}

std::vector<te::qt::widgets::AbstractLayerTreeItem*> te::qt::widgets::AbstractLayerTreeItem::getDescendants()
{
  std::vector<AbstractLayerTreeItem*> descendantItems;
  std::vector<AbstractLayerTreeItem*> childrenItems;

  if(hasChildren())
  {
    int numChildren = children().count();
    for(int i = 0; i < numChildren; ++i)
    {
      AbstractLayerTreeItem* childItem = static_cast<AbstractLayerTreeItem*>(children().at(i));
      descendantItems.push_back(childItem);
      childrenItems = childItem->getDescendants();
    }
  }

  for(std::size_t i = 0; i < childrenItems.size(); ++i)
    descendantItems.push_back(childrenItems[i]);

  return descendantItems;
}

std::vector<te::qt::widgets::AbstractLayerTreeItem*> te::qt::widgets::AbstractLayerTreeItem::getAncestors()
{
  std::vector<AbstractLayerTreeItem*> ancestorLayers;

  AbstractLayerTreeItem* itemParent = static_cast<AbstractLayerTreeItem*>(parent());
  while(itemParent)
  {
    ancestorLayers.push_back(itemParent);
    itemParent = static_cast<AbstractLayerTreeItem*>(itemParent->parent());
  }

  return ancestorLayers;
}
