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
  \file BuildGraphicsItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildGraphicsItem.h"
#include "../../core/pattern/singleton/Context.h"
#include "pattern/factory/item/ItemFactoryParamsCreate.h"
#include "pattern/factory/item/NewItemFactory.h"
#include "pattern/command/AddCommand.h"
#include "Scene.h"
#include "ItemUtils.h"

// STL
#include <sstream>
#include <string>

// Qt
#include <QGraphicsItem>
#include <QUndoCommand> 

te::layout::BuildGraphicsItem::BuildGraphicsItem(Scene* scene ) :
  m_scene(scene)
{
 
}

te::layout::BuildGraphicsItem::~BuildGraphicsItem()
{
 
}

QGraphicsItem* te::layout::BuildGraphicsItem::rebuildItem( te::layout::Properties props, bool draw )
{
  QGraphicsItem* item = 0;

  if(props.getProperties().empty())
    return item;

  clear();

  m_props = props;  
  m_coord = findCoordinate(props);
  m_zValue = findZValue(props);
  
  EnumType* type = props.getTypeObj();

  item = createItem(type, draw);

  clear();
  
  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createItem(te::layout::EnumType* itemType, const te::gm::Coord2D& coordinate, 
    double width, double height, bool draw)
{
  QGraphicsItem* item = 0;

  clear();

  m_coord = coordinate;
  m_width = width;
  m_height = height;

  item = createItem(itemType, draw);

  clear();

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createItem(te::layout::EnumType* itemType, bool draw)
{
  QGraphicsItem* item = 0;

  if (!itemType)
  {
    return item;
  }

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  m_name = nameItem(itemType); // create item name, will be used if props is empty

  te::layout::EnumObjectType* obj = Enums::getInstance().getEnumObjectType();

  std::string strName = nameItem(itemType);

  ItemFactoryParamsCreate params(strName, m_zValue, m_id, m_coord, m_width, m_height);

  std::string name = itemType->getName();

  AbstractItemView* abstractItem = te::layout::NewItemFactory::make(name, params);
  item = dynamic_cast<QGraphicsItem*>(abstractItem);

  if (!item)
  {
    return item;
  }

  if (m_scene)
  {
    m_scene->insertItem(item);
  }

  afterBuild(item, draw);

  return item;
}

std::string te::layout::BuildGraphicsItem::nameItem( te::layout::EnumType* type )
{
  std::string name = "";
  
  QList<QGraphicsItem*> graphicsItems;
  std::stringstream ss;//create a stringstream

  if (m_scene)
  {
    graphicsItems = m_scene->items();
  }  

  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  if(!iUtils)
  {
    return name;
  }

  m_id = iUtils->maxTypeId(type);

  if(m_id < 0)
    m_id = 0;
  else
    m_id+= 1;

  ss << m_id;

  name = type->getName();
  name+="_";
  name+=ss.str();

  return name;
}

void te::layout::BuildGraphicsItem::afterBuild(QGraphicsItem* item, bool draw)
{
  if (!item)
  {
    return;
  }

  QPointF pointInSceneCS(m_coord.x, m_coord.y);

  // tool for create items could be with size
  if (m_width == 0 && m_height == 0)
  {
    double width = item->boundingRect().width();
    double height = item->boundingRect().height();

    QPointF pointInItemCS = item->mapFromScene(pointInSceneCS);
    pointInItemCS.setX(pointInItemCS.x() - (width / 2.));
    pointInItemCS.setY(pointInItemCS.y() - (height / 2.));
    pointInSceneCS = item->mapToScene(pointInItemCS);
  }

  item->setPos(pointInSceneCS);

  if (!m_props.getProperties().empty())
  {
    item->setZValue(m_zValue);
  }

  if (item)
  {
    if (m_scene)
    {
      QUndoCommand* command = new AddCommand(item);
      m_scene->addUndoStack(command);
    }
  }
}


