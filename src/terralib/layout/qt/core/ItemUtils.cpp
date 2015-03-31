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
#include "../item/MapItem.h"
#include "../item/LegendItem.h"
#include "../../item/LegendModel.h"
#include "../../item/MapModel.h"
#include "../../item/MapGridModel.h"
#include "../item/TextItem.h"
#include "../../item/TextModel.h"
#include "../item/LegendChildItem.h"
#include "../../item/LegendChildModel.h"
#include "../../core/pattern/derivativevisitor/VisitorUtils.h"
#include "../item/MapGridItem.h"
#include "../../item/GridGeodesicModel.h"
#include "../../item/GridPlanarModel.h"
#include "Scene.h"

// STL
#include <stddef.h>  // defines NULL

// Boost
#include <boost/foreach.hpp>

// Qt
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTextDocument>
#include <QFont>

te::layout::ItemUtils::ItemUtils( QGraphicsScene* scene ) :
  m_scene(scene)
{

}

te::layout::ItemUtils::~ItemUtils()
{

}

std::vector<te::layout::MapItem*> te::layout::ItemUtils::getMapItemList(bool selected)
{
  std::vector<te::layout::MapItem*> list;

  QList<QGraphicsItem*> graphicsItems = getItems(selected);
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

te::layout::MapItem* te::layout::ItemUtils::getMapItem( std::string name )
{
  te::layout::MapItem* map = 0;

  QList<QGraphicsItem*> graphicsItems = getItems(false);
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

std::vector<std::string> te::layout::ItemUtils::mapNameList(bool selected)
{
  std::vector<std::string> strList;

  QList<QGraphicsItem*> graphicsItems = getItems(selected);
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

int te::layout::ItemUtils::countType( te::layout::EnumType* type )
{
  int count = 0;

  QList<QGraphicsItem*> graphicsItems = getItems();
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

int te::layout::ItemUtils::maxTypeId( te::layout::EnumType* type )
{
  int id = -1;

  QList<QGraphicsItem*> graphicsItems = getItems();
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

bool te::layout::ItemUtils::isCurrentMapTools()
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

QList<QGraphicsItem*> te::layout::ItemUtils::getItems( bool selected )
{
  QList<QGraphicsItem*> graphicsItems;

  if(selected)
  {
    graphicsItems = m_scene->selectedItems();
  }
  else
  {
    graphicsItems = m_scene->items();
  }

  return graphicsItems;
}

void te::layout::ItemUtils::setCurrentToolInSelectedMapItems( EnumType* mode )
{
  if(!mode)
    return;

  EnumModeType* type = Enums::getInstance().getEnumModeType();

  if(mode == type->getModeNone())
    return;

  if(!isCurrentMapTools())
    return;

  QList<QGraphicsItem*> graphicsItems = getItems(true);
  foreach(QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::ItemObserver* lItem = dynamic_cast<te::layout::ItemObserver*>(item);
    if(!lItem)
      continue;

    te::layout::MapItem* mit = dynamic_cast<te::layout::MapItem*>(lItem);
    if(!mit)
      continue;

    mit->changeCurrentTool(mode);
  }
}

void te::layout::ItemUtils::createTextGridAsObject()
{
  QFont* ft = new QFont;
  QGraphicsItem *item = m_scene->selectedItems().first();
  if(item)
  {
    ItemObserver* it = dynamic_cast<ItemObserver*>(item);
    if(it)
    {
      MapGridItem* mt = dynamic_cast<MapGridItem*>(it);
      if(mt)
      {
        MapGridModel* model = dynamic_cast<MapGridModel*>(mt->getModel());

        GridGeodesicModel* gridGeo = dynamic_cast<GridGeodesicModel*>(model->getGridGeodesic());
        if(model->getGridGeodesic()->isVisible())
        {
          model->getGridGeodesic()->setVisibleAllTexts(false);
          std::map<te::gm::Point*, std::string> mapGeo = gridGeo->getGridInfo();
          gridGeo->setVisibleAllTexts(false);
          ft->setFamily(gridGeo->getFontFamily().c_str());
          ft->setPointSize(gridGeo->getTextPointSize());
          createTextItemFromObject(mapGeo, ft);
        }

        GridPlanarModel* gridPlanar = dynamic_cast<GridPlanarModel*>(model->getGridPlanar());
        if(model->getGridPlanar()->isVisible())
        {
          model->getGridGeodesic()->setVisibleAllTexts(false);
          std::map<te::gm::Point*, std::string> mapPlanar = gridPlanar->getGridInfo();
          gridPlanar->setVisibleAllTexts(false);
          ft->setFamily(gridPlanar->getFontFamily().c_str());
          ft->setPointSize(gridPlanar->getTextPointSize());
          createTextItemFromObject(mapPlanar, ft);
        }   
      }
      it->redraw();
    }
  }
}

void te::layout::ItemUtils::createTextMapAsObject()
{
  QGraphicsItem *item = m_scene->selectedItems().first();
  if(item)
  {
    ItemObserver* it = dynamic_cast<ItemObserver*>(item);
    if(it)
    {
      MapItem* mt = dynamic_cast<MapItem*>(it);
      if(mt)
      {
        MapModel* model = dynamic_cast<MapModel*>(mt->getModel());
        std::map<te::gm::Point*, std::string> map = model->getTextMapAsObjectInfo();
        createTextItemFromObject(map);
      }
    }
  }
}

void te::layout::ItemUtils::createLegendChildAsObject()
{
  QGraphicsItem *item = m_scene->selectedItems().first();
  if(item)
  {
    ItemObserver* it = dynamic_cast<ItemObserver*>(item);
    if(it)
    {
      LegendItem* lit = dynamic_cast<LegendItem*>(it);
      if(lit)
      {
        LegendModel* model = dynamic_cast<LegendModel*>(lit->getModel());
        MapModel* visitable = dynamic_cast<MapModel*>(model->getVisitable());

        std::map<te::gm::Point*, std::string> coord = model->getCoordChildren();
        createLegendChildItemFromLegend(coord, visitable);
      }
      it->redraw();
    }
  }
}

void te::layout::ItemUtils::createTextItemFromObject( std::map<te::gm::Point*, std::string> map, QFont* ft )
{
  Scene* scne = dynamic_cast<Scene*>(m_scene);

  if(!scne)
    return;

  EnumModeType* mode = Enums::getInstance().getEnumModeType();

  std::map<te::gm::Point*, std::string>::iterator it;
  
  for (it = map.begin(); it != map.end(); ++it) 
  {
    te::gm::Point* pt = it->first;
    std::string text = it->second;

    Context::getInstance().setMode(mode->getModeCreateText());

    QGraphicsItem* item = 0;
    
    te::gm::Coord2D coord(pt->getX(), pt->getY());
    item = scne->createItem(coord);
    if(!item)
      continue;

    TextItem* txtItem = dynamic_cast<TextItem*>(item);
    if(txtItem)
    {
      TextModel* model = dynamic_cast<TextModel*>(txtItem->getModel());
      if(model)
      {
        if(ft)
        {
          txtItem->setFont(*ft);
          Font fnt = model->getFont();
          fnt.setFamily(ft->family().toStdString());
          fnt.setPointSize(ft->pointSize());
          model->setFont(fnt);
        }        
        model->setText(text);
        txtItem->document()->setPlainText(text.c_str());
      }
    }
  }

  Context::getInstance().setMode(mode->getModeNone());
}

void te::layout::ItemUtils::createLegendChildItemFromLegend( std::map<te::gm::Point*, std::string> map, te::layout::MapModel* visitable )
{
  Scene* scne = dynamic_cast<Scene*>(m_scene);

  if(!scne)
    return;

  if(!visitable)
    return;

  EnumModeType* mode = Enums::getInstance().getEnumModeType();

  std::map<te::gm::Point*, std::string>::iterator it;

  for (it = map.begin(); it != map.end(); ++it) 
  {
    te::gm::Point* pt = it->first;
    std::string text = it->second;

    Context::getInstance().setMode(mode->getModeCreateLegendChild());

    QGraphicsItem* item = 0;
    te::gm::Coord2D coord(pt->getX(), pt->getY());
    item = scne->createItem(coord);
    if(!item)
      continue;

    LegendChildItem* lgItem = dynamic_cast<LegendChildItem*>(item);
    if(lgItem)
    {
      QList<QGraphicsItem*> legends;
      legends.push_back(lgItem);
      te::layout::VisitorUtils::getInstance().changeMapVisitable(legends, visitable);
    }
  }

  Context::getInstance().setMode(mode->getModeNone());
}
