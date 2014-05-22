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
#include "LegendModel.h"
#include "LegendController.h"
#include "LegendItem.h"
#include "PaperModel.h"
#include "PaperController.h"
#include "PaperItem.h"
#include "Context.h"
#include "SharedProperties.h"
#include "ScaleModel.h"
#include "ScaleController.h"
#include "ScaleItem.h"
#include "Scene.h"
#include "ItemUtils.h"
#include "ItemGroupModel.h"
#include "ItemGroupController.h"
#include "ItemGroup.h"

// Qt
#include <QGraphicsItem>

// STL
#include <sstream>
#include <string>  

te::layout::BuildGraphicsItem::BuildGraphicsItem() :
  m_zValue(0),
  m_id(0),
  m_props(0),
  m_redraw(true),
  m_name("Unknown"),
  m_sharedProps(0),
  m_paperItem("PAPER_"),
  m_mapItem("MAP_"),
  m_mapGridItem("MAPGRID_"),
  m_textItem("TEXT_"),
  m_rectangleItem("RECTANGLE_"),
  m_legendItem("LEGEND_"),
  m_scaleItem("SCALE_"),
  m_horizontalRuler("HORIZONTAL_RULER_"),
  m_verticalRuler("VERTICAL_RULER_"),
  m_groupItem("ITEM_GROUP_")
{

}

te::layout::BuildGraphicsItem::~BuildGraphicsItem()
{
  if(m_sharedProps)
  {
    delete m_sharedProps;
    m_sharedProps = 0;
  }
}

QGraphicsItem* te::layout::BuildGraphicsItem::rebuildItem( te::layout::Properties* props, bool draw )
{
  QGraphicsItem* item = 0;

  if(!props)
    return item;

  clear();

  m_props = props;  
  m_coord = findCoordinate(props);
  m_zValue = findZValue(props);
  m_redraw = draw;

  te::layout::LayoutAbstractObjectType type = props->getTypeObj();

  switch(type)
  {
  case TPMapItem:
    item = createMap();
    break;
  case TPMapGridItem:
    item = createMapGrid();
    break;
  case TPText:
    item = createText();
    break;
  case TPRetangleItem:
    item = createRectangle();
    break;
  case TPLegendItem:
    item = createLegend();
    break;
  case TPPaperItem:
    item = createPaper();
    break;
  case TPScaleItem:
    item = createScale();
    break;
  case TPItemGroup:
    item = createItemGroup();
    break;
  default:
    item = 0;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createItem( te::layout::LayoutMode mode, const te::gm::Coord2D& coordinate, bool draw )
{
  QGraphicsItem* item = 0;

  m_coord = coordinate;
  clear();
  m_redraw = draw;

  switch(mode)
  {
  case TypeCreateMap:
    m_name = nameItem(m_mapItem, TPMapItem);
    item = createMap();
    break;
  case TypeCreateMapGrid:
    m_name = nameItem(m_mapGridItem, TPMapGridItem);
    item = createMapGrid();
    break;
  case TypeCreateText:
    m_name = nameItem(m_textItem, TPText);
    item = createText();
    break;
  case TypeCreateRectangle:
    m_name = nameItem(m_rectangleItem, TPRetangleItem);
    item = createRectangle();
    break;
  case TypeCreateLegend:
    m_name = nameItem(m_legendItem, TPLegendItem);
    item = createLegend();
    break;
  case TypeCreateScale:
    m_name = nameItem(m_scaleItem, TPScaleItem);
    item = createScale();
    break;
  case TypeCreateItemGroup:
    m_name = nameItem(m_groupItem, TPItemGroup);
    item = createItemGroup();
    break;
  default:
    item = 0;
  }

  return item;
}

te::gm::Coord2D te::layout::BuildGraphicsItem::findCoordinate( te::layout::Properties* props )
{
  /* Coordinate - x1, y1*/

  double x1 = 0;
  double y1 = 0;
  
  Property pro_x1 = props->contains(m_sharedProps->getX1());

  if(!pro_x1.isNull())
  {
    x1 = pro_x1.getValue().toDouble();
  }

  Property pro_y1 = props->contains(m_sharedProps->getY1());

  if(!pro_y1.isNull())
  {
    y1 = pro_y1.getValue().toDouble();
  }

  te::gm::Coord2D coord(x1, y1);
  return coord;
}

int te::layout::BuildGraphicsItem::findZValue( te::layout::Properties* props )
{
  int zValue = 0;
  
  Property pro_zValue = props->contains(m_sharedProps->getZValue());

  if(!pro_zValue.isNull())
  {
    zValue = pro_zValue.getValue().toInt();
  }

  return zValue;
}

std::string te::layout::BuildGraphicsItem::nameItem( std::string name, te::layout::LayoutAbstractObjectType type )
{
  AbstractScene* abstScene = Context::getInstance()->getScene();

  QList<QGraphicsItem*> graphicsItems;
  std::stringstream ss;//create a stringstream

  if(abstScene)
  {
    Scene* sc = dynamic_cast<Scene*>(abstScene);
    if(sc)
    {
      graphicsItems = sc->items();
    }
  }  

  m_id = te::layout::maxTypeId(graphicsItems, type);

  if(m_id < 0)
    m_id = 0;
  else
    m_id+= 1;

  ss << m_id;

  name+=ss.str();

  return name;
}

void te::layout::BuildGraphicsItem::clear()
{
  m_id = 0;
  m_name = "Unknown";
  m_zValue = 0;
  m_props = 0;
  m_redraw = true;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createPaper()
{
  //Paper
  QGraphicsItem* item = 0;

  PaperModel* model = new PaperModel(te::layout::Context::getInstance()->getPaperConfig());
  if(m_props)
  {
    model->updateProperties(m_props);
  }
  else
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  PaperController* controllerMap = new PaperController(model);
  ItemObserver* itemObs = (ItemObserver*)controllerMap->getView();

  PaperItem* view = dynamic_cast<PaperItem*>(itemObs);

  if(view)
  {
    view->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      view->setZValue(m_zValue);
    }
    if(m_redraw)
      itemObs->redraw();
    return view;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createMap()
{
  QGraphicsItem* item = 0;

  MapModel* model = new MapModel();
  if(m_props)
  {
    model->updateProperties(m_props);
  }
  else
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  MapController* controllerMap = new MapController(model);
  ItemObserver* itemObs = (ItemObserver*)controllerMap->getView();

  MapItem* qrectMap = dynamic_cast<MapItem*>(itemObs);
  
  if(qrectMap)
  {
    qrectMap->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      qrectMap->setZValue(m_zValue);
    }
    if(m_redraw)
      itemObs->redraw();
    return qrectMap;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createMapGrid()
{
  QGraphicsItem* item = 0;

  MapGridModel* model = new MapGridModel();
  if(m_props)
  {
    model->updateProperties(m_props);
  }
  else
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  MapGridController* controllerMapGrid = new MapGridController(model);
  ItemObserver* itemObs = (ItemObserver*)controllerMapGrid->getView();

  MapGridItem* qrectMapGrid = dynamic_cast<MapGridItem*>(itemObs);

  if(qrectMapGrid)
  {
    qrectMapGrid->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      qrectMapGrid->setZValue(m_zValue);
    }
    if(m_redraw)
      itemObs->redraw();
    return qrectMapGrid;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createText()
{
  QGraphicsItem* item = 0;

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createRectangle()
{
  QGraphicsItem* item = 0;

  RectangleModel* model = new RectangleModel();	
  if(m_props)
  {
    model->updateProperties(m_props);
  }
  else
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  RectangleController* controller = new RectangleController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  RectangleItem* rect = dynamic_cast<RectangleItem*>(itemObs); 

  if(rect)
  {
    rect->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      rect->setZValue(m_zValue);
    }
    if(m_redraw)
      itemObs->redraw();
    return rect;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createLegend()
{
  QGraphicsItem* item = 0;

  LegendModel* model = new LegendModel();	
  if(m_props)
  {
    model->updateProperties(m_props);
  }
  else
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  LegendController* controller = new LegendController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  LegendItem* legend = dynamic_cast<LegendItem*>(itemObs); 

  if(legend)
  {
    legend->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      legend->setZValue(m_zValue);
    }
    if(m_redraw)
      itemObs->redraw();
    return legend;
  }
  
  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createScale()
{
  QGraphicsItem* item = 0;

  ScaleModel* model = new ScaleModel();	
  if(m_props)
  {
    model->updateProperties(m_props);
  }
  else
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  ScaleController* controller = new ScaleController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  ScaleItem* scale = dynamic_cast<ScaleItem*>(itemObs); 

  if(scale)
  {
    scale->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      scale->setZValue(m_zValue);
    }
    if(m_redraw)
      itemObs->redraw();
    return scale;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createItemGroup()
{
  QGraphicsItem* item = 0;

  ItemGroupModel* model = new ItemGroupModel();	
  if(m_props)
  {
    model->updateProperties(m_props);
  }
  else
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  ItemGroupController* controller = new ItemGroupController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  ItemGroup* view = dynamic_cast<ItemGroup*>(itemObs); 

  if(view)
  {
    if(m_props)
    {
      view->setZValue(m_zValue);
    }
    if(m_redraw)
    {
      itemObs->redraw();
    }
    return view;
  }

  return item;
}
