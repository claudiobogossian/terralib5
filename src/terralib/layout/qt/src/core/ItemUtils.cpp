/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ItemUtils.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemUtils.h"
#include "ItemModelObservable.h"

// STL
#include <stddef.h>  // defines NULL

// Boost
#include "boost\foreach.hpp"

TELAYOUTEXPORT std::vector<te::layout::MapItem*> te::layout::getMapItemList( QList<QGraphicsItem*> graphicsItems )
{
  std::vector<te::layout::MapItem*> list;
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::ItemObserver* lItem = dynamic_cast<te::layout::ItemObserver*>(item);
    if(!lItem)
      continue;

    te::layout::MapItem* mit = dynamic_cast<te::layout::MapItem*>(lItem);
    if(!mit)
      continue;

    list.push_back(mit);
  }

  return list;
}

TELAYOUTEXPORT te::layout::MapItem* te::layout::getMapItem( QList<QGraphicsItem*> graphicsItems, std::string name )
{
  te::layout::MapItem* map = 0;

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::ItemObserver* lItem = dynamic_cast<te::layout::ItemObserver*>(item);
    if(!lItem)
      continue;

    te::layout::MapItem* mit = dynamic_cast<te::layout::MapItem*>(lItem);
    if(!mit)
      continue;

    if(mit->getName().compare(name) != 0)
      continue;

    map = mit;
  }

  return map;
}

TELAYOUTEXPORT std::vector<std::string> te::layout::mapNameList( QList<QGraphicsItem*> graphicsItems )
{
  std::vector<std::string> strList;

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::ItemObserver* lItem = dynamic_cast<te::layout::ItemObserver*>(item);
    if(!lItem)
      continue;

    te::layout::MapItem* mit = dynamic_cast<te::layout::MapItem*>(lItem);
    if(!mit)
      continue;

    std::string name = mit->getName();
    strList.push_back(name);
  }

  return strList;
}
