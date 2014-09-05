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
#include "../../core/pattern/mvc/ItemModelObservable.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/enum/Enums.h"

// STL
#include <stddef.h>  // defines NULL

// Boost
#include <boost/foreach.hpp>

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

TELAYOUTEXPORT int te::layout::countType( QList<QGraphicsItem*> graphicsItems, te::layout::EnumType* type )
{
  int count = 0;

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    ItemObserver* obs = dynamic_cast<ItemObserver*>(item);
    if(!obs)
      continue;

    if(obs->getModel()->getType() == type)
    {
      count+=1;
    }
  }

  return count;
}

TELAYOUTEXPORT int te::layout::maxTypeId( QList<QGraphicsItem*> graphicsItems, te::layout::EnumType* type )
{
  int id = -1;

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    ItemObserver* obs = dynamic_cast<ItemObserver*>(item);
    if(!obs)
      continue;

    ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(obs->getModel());
    if(!model)
      continue;

    if(obs->getModel()->getType() == type)
    {
      if(id == -1)
      {
        id = model->getId();
      }
      else
      {
        if(model->getId() > id)
        {
          id = model->getId();
        }
      }
    }
  }

  return id;
}

TELAYOUTEXPORT bool te::layout::isCurrentMapTools()
{
  bool result = false;

  te::layout::EnumType* mode = te::layout::Context::getInstance().getMode();
  te::layout::EnumModeType* type = te::layout::Enums::getInstance().getEnumModeType();

  if(mode == type->getModeMapPan())
  {
    result = true;
  }
  else if(mode == type->getModeMapZoomIn())
  {
    result = true;
  }
  else if(mode == type->getModeMapZoomOut())
  {
    result = true;
  }
  return result;
}
