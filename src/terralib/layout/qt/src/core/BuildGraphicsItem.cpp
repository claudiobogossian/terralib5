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
  \file BuildGraphicsItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildGraphicsItem.h"
#include "ItemObserver.h"
#include "RectangleModel.h"
#include "RectangleController.h"
#include "RectangleItem.h"
#include "MapModel.h"
#include "MapController.h"
#include "MapItem.h"
#include "MapGridModel.h"
#include "MapGridController.h"
#include "MapGridItem.h"

// Qt
#include <QGraphicsItem>

te::layout::BuildGraphicsItem::BuildGraphicsItem()
{

}

te::layout::BuildGraphicsItem::~BuildGraphicsItem()
{

}

QGraphicsItem* te::layout::BuildGraphicsItem::createMap( const te::gm::Coord2D& coordinate )
{
  QGraphicsItem* item = 0;

  MapModel* modelMap = new MapModel();		  
  modelMap->setName("MAPA_01");

  MapController* controllerMap = new MapController(modelMap);
  ItemObserver* itemMap = (ItemObserver*)controllerMap->getView();

  MapItem* qrectMap = dynamic_cast<MapItem*>(itemMap);
  qrectMap->setPos(QPointF(coordinate.x, coordinate.y));
  itemMap->redraw();

  if(qrectMap)
    return qrectMap;

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createMapGrid( const te::gm::Coord2D& coordinate )
{
  QGraphicsItem* item = 0;

  MapGridModel* modelMapGrid = new MapGridModel();		  
  modelMapGrid->setName("MAPA_GRID_01");
  
  MapGridController* controllerMapGrid = new MapGridController(modelMapGrid);
  ItemObserver* itemMapGrid = (ItemObserver*)controllerMapGrid->getView();

  MapGridItem* qrectMapGrid = dynamic_cast<MapGridItem*>(itemMapGrid);
  qrectMapGrid->setPos(QPointF(coordinate.x, coordinate.y));
  itemMapGrid->redraw();

  if(qrectMapGrid)
    return qrectMapGrid;

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createText( const te::gm::Coord2D& coordinate )
{
  QGraphicsItem* item = 0;

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createRectangle( const te::gm::Coord2D& coordinate )
{
  QGraphicsItem* item = 0;

  //Retângulo: utilizando o canvas da Terralib 5
  RectangleModel* model = new RectangleModel();	
  model->setName("RECT_01");

  RectangleController* controller = new RectangleController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  RectangleItem* rect = dynamic_cast<RectangleItem*>(itemObs); 
  rect->setPos(QPointF(coordinate.x, coordinate.y));
  itemObs->redraw();
 
  if(rect)
    return rect;

  return item;
}
