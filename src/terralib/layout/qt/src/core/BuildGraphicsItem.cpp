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
#include "AbstractType.h"
#include "SharedProperties.h"
#include "ScaleModel.h"
#include "ScaleController.h"
#include "ScaleItem.h"

// Qt
#include <QGraphicsItem>

te::layout::BuildGraphicsItem::BuildGraphicsItem() :
  m_zValue(0)
{

}

te::layout::BuildGraphicsItem::~BuildGraphicsItem()
{

}

QGraphicsItem* te::layout::BuildGraphicsItem::rebuildItem( te::layout::Properties* props )
{
  QGraphicsItem* item = 0;

  if(!props)
    return item;

  m_props = props;  
  m_coord = findCoordinate(props);
  m_zValue = findZValue(props);

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
  default:
    item = 0;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createItem( te::layout::LayoutMode mode, const te::gm::Coord2D& coordinate )
{
  QGraphicsItem* item = 0;

  m_coord = coordinate;
  m_props = 0;

  switch(mode)
  {
  case TypeCreateMap:
    item = createMap();
    break;
  case TypeCreateMapGrid:
    item = createMapGrid();
    break;
  case TypeCreateText:
    item = createText();
    break;
  case TypeCreateRectangle:
    item = createRectangle();
    break;
  case TypeCreateLegend:
    item = createLegend();
    break;
  case TypeCreateScale:
    item = createScale();
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

  SharedProperties* sharedProps = new SharedProperties;

  Property pro_x1 = props->contains(sharedProps->getX1());

  if(!pro_x1.isNull())
  {
    x1 = pro_x1.getValue().toDouble();
  }

  Property pro_y1 = props->contains(sharedProps->getY1());

  if(!pro_y1.isNull())
  {
    y1 = pro_y1.getValue().toDouble();
  }

  if(sharedProps)
  {
    delete sharedProps;
    sharedProps = 0;
  }

  te::gm::Coord2D coord(x1, y1);
  return coord;
}

int te::layout::BuildGraphicsItem::findZValue( te::layout::Properties* props )
{
  int zValue = 0;

  SharedProperties* sharedProps = new SharedProperties;

  Property pro_zValue = props->contains(sharedProps->getZValue());

  if(!pro_zValue.isNull())
  {
    zValue = pro_zValue.getValue().toInt();
  }

  if(sharedProps)
  {
    delete sharedProps;
    sharedProps = 0;
  }

  return zValue;
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
    model->setName("PAPER_01");
  }

  PaperController* controllerMap = new PaperController(model);
  ItemObserver* itemPaper = (ItemObserver*)controllerMap->getView();

  PaperItem* view = dynamic_cast<PaperItem*>(itemPaper);

  if(view)
  {
    view->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      view->setZValue(m_zValue);
    }
    itemPaper->redraw();
    return view;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createMap()
{
  QGraphicsItem* item = 0;

  MapModel* modelMap = new MapModel();
  if(m_props)
  {
    modelMap->updateProperties(m_props);
  }
  else
  {
    modelMap->setName("MAPA_01");
  }

  MapController* controllerMap = new MapController(modelMap);
  ItemObserver* itemMap = (ItemObserver*)controllerMap->getView();

  MapItem* qrectMap = dynamic_cast<MapItem*>(itemMap);
  
  if(qrectMap)
  {
    qrectMap->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      qrectMap->setZValue(m_zValue);
    }
    itemMap->redraw();
    return qrectMap;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createMapGrid()
{
  QGraphicsItem* item = 0;

  MapGridModel* modelMapGrid = new MapGridModel();
  if(m_props)
  {
    modelMapGrid->updateProperties(m_props);
  }
  else
  {
    modelMapGrid->setName("MAPA_GRID_01");
  }

  MapGridController* controllerMapGrid = new MapGridController(modelMapGrid);
  ItemObserver* itemMapGrid = (ItemObserver*)controllerMapGrid->getView();

  MapGridItem* qrectMapGrid = dynamic_cast<MapGridItem*>(itemMapGrid);

  if(qrectMapGrid)
  {
    qrectMapGrid->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      qrectMapGrid->setZValue(m_zValue);
    }
    itemMapGrid->redraw();
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
    model->setName("RECT_01");
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
    model->setName("LEGEND_01");
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
    model->setName("SCALE_01");
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
    itemObs->redraw();
    return scale;
  }

  return item;
}
