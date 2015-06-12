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
  \file terralib/qt/widgets/layer/explorer/AbstractTreeItem.cpp

  \brief The class that represents an item in a LayerTreeModel.
*/

// TerraLib
#include "AbstractTreeItem.h"

te::qt::widgets::AbstractTreeItem::AbstractTreeItem(QObject* parent)
  : QObject(parent)
{
}

te::qt::widgets::AbstractTreeItem::~AbstractTreeItem()
{
}

std::vector<te::qt::widgets::AbstractTreeItem*> te::qt::widgets::AbstractTreeItem::getDescendants()
{
  std::vector<AbstractTreeItem*> descendantItems;
  std::vector<AbstractTreeItem*> childrenItems;

  if(hasChildren())
  {
    int numChildren = children().count();
    for(int i = 0; i < numChildren; ++i)
    {
      AbstractTreeItem* childItem = static_cast<AbstractTreeItem*>(children().at(i));
      descendantItems.push_back(childItem);
      childrenItems = childItem->getDescendants();
    }
  }

  for(std::size_t i = 0; i < childrenItems.size(); ++i)
    descendantItems.push_back(childrenItems[i]);

  return descendantItems;
}

std::vector<te::qt::widgets::AbstractTreeItem*> te::qt::widgets::AbstractTreeItem::getAncestors()
{
  std::vector<AbstractTreeItem*> ancestorLayers;

  AbstractTreeItem* itemParent = static_cast<AbstractTreeItem*>(parent());
  while(itemParent)
  {
    ancestorLayers.push_back(itemParent);
    itemParent = static_cast<AbstractTreeItem*>(itemParent->parent());
  }

  return ancestorLayers;
}
