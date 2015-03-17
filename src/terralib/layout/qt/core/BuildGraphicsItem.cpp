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
#include "Scene.h"
#include "ItemUtils.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/property/SharedProperties.h"
#include "../../item/RectangleModel.h"
#include "../../item/RectangleController.h"
#include "../item/RectangleItem.h"
#include "../../item/MapModel.h"
#include "../../item/MapController.h"
#include "../item/MapItem.h"
#include "../../item/MapGridModel.h"
#include "../../item/MapGridController.h"
#include "../item/MapGridItem.h"
#include "../../item/LegendModel.h"
#include "../../item/LegendController.h"
#include "../item/LegendItem.h"
#include "../../item/PaperModel.h"
#include "../../item/PaperController.h"
#include "../item/PaperItem.h"
#include "../../item/ScaleModel.h"
#include "../../item/ScaleController.h"
#include "../item/ScaleItem.h"
#include "../../item/ItemGroupModel.h"
#include "../../item/ItemGroupController.h"
#include "../item/ItemGroup.h"
#include "../../item/ImageModel.h"
#include "../../item/ImageController.h"
#include "../item/ImageItem.h"
#include "../../item/ArrowModel.h"
#include "../../item/ArrowController.h"
#include "../item/ArrowItem.h"
#include "../../item/EllipseModel.h"
#include "../../item/EllipseController.h"
#include "../item/EllipseItem.h"
#include "../../item/PointModel.h"
#include "../../item/PointController.h"
#include "../item/PointItem.h"
#include "../../item/TitleModel.h"
#include "../../item/TitleController.h"
#include "../item/TitleItem.h"
#include "../../item/TextGridModel.h"
#include "../../item/TextGridController.h"
#include "../item/TextGridItem.h"
#include "../../core/enum/Enums.h"
#include "../../item/LegendChildModel.h"
#include "../../item/LegendChildController.h"
#include "../item/LegendChildItem.h"
#include "../../item/TextController.h"
#include "../item/TextItem.h"
#include "../../item/TextModel.h"
#include "../../core/property/Properties.h"
#include "../item/MovingItemGroup.h"
#include "../../item/MovingItemGroupController.h"
#include "../../item/MovingItemGroupModel.h"
#include "../../item/LineModel.h"
#include "../../item/LineController.h"
#include "../item/LineItem.h"
#include "../../item/PolygonModel.h"
#include "../../item/PolygonController.h"
#include "../item/PolygonItem.h"
#include "../../item/BalloonModel.h"
#include "../../item/BalloonController.h"
#include "../item/BalloonItem.h"
#include "../../item/BarCodeModel.h"
#include "../../item/BarCodeController.h"
#include "../item/BarCodeItem.h"

// Qt
#include <QGraphicsItem>

// STL
#include <sstream>
#include <string>  

te::layout::BuildGraphicsItem::BuildGraphicsItem() :
  m_paperItem("PAPER_"),
  m_mapItem("MAP_"),
  m_mapGridItem("MAPGRID_"),
  m_textItem("TEXT_"),
  m_rectangleItem("RECTANGLE_"),
  m_legendItem("LEGEND_"),
  m_scaleItem("SCALE_"),
  m_groupItem("ITEM_GROUP_"),
  m_imageItem("IMAGE_"),
  m_arrowItem("ARROW_"),
  m_ellipseItem("ELLIPSE_"),
  m_pointItem("POINT_"),
  m_textGridItem("TEXT_GRID_"),
  m_titleItem("TITLE_"),
  m_legendChildItem("LEGEND_CHILD_"),
  m_movingGroupItem("MOVING_ITEM_GROUP_"),
  m_lineItem("LINE_"), 
  m_polygonItem("POLYGON_"), 
  m_balloonItem("BALLOON_"),
  m_barCodeItem("BARCODE_")
{
 
}

te::layout::BuildGraphicsItem::~BuildGraphicsItem()
{
 
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

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

  EnumType* type = props->getTypeObj();

  if(type == enumObj->getMapItem())
  {
    item = createMap();
  }
  else if(type == enumObj->getMapGridItem())
  {
    item = createMapGrid();
  }
  else if(type == enumObj->getTextItem())
  {
    item = createText();
  }
  else if(type == enumObj->getImageItem())
  {
    item = createImage();
  }
  else if(type == enumObj->getRectangleItem())
  {
    item = createRectangle();
  }
  else if(type == enumObj->getLegendItem())
  {
    item = createLegend();
  }
  else if(type == enumObj->getPaperItem())
  {
    item = createPaper();
  }
  else if(type == enumObj->getScaleItem())
  {
    item = createScale();
  }
  else if(type == enumObj->getItemGroup())
  {
    item = createItemGroup();
  }
  else if(type == enumObj->getArrowItem())
  {
    item = createArrow();
  }
  else if(type == enumObj->getEllipseItem())
  {
    item = createEllipse();
  }
  else if(type == enumObj->getPointItem())
  {
    item = createPoint();
  }
  else if(type == enumObj->getTextGridItem())
  {
    item = createTextGrid();
  }
  else if(type == enumObj->getTitleItem())
  {
    item = createTitle();
  }
  else if(type == enumObj->getLegendChildItem())
  {
    item = createLegendChild();
  }
  else if(type == enumObj->getLineItem())
  {
    item = createLine();
  }
  else if(type == enumObj->getPolygonItem())
  {
    item = createPolygon();
  }
  else if(type == enumObj->getBalloonItem())
  {
    item = createBalloon();
  }
  else if(type == enumObj->getBarCodeItem())
  {
    item = createBarCode();
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createItem( te::layout::EnumType* mode, const te::gm::Coord2D& coordinate, bool draw )
{
  QGraphicsItem* item = 0;

  m_coord = coordinate;
  clear();
  m_redraw = draw;

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  EnumModeType* enumMode = Enums::getInstance().getEnumModeType();

  if(mode == enumMode->getModeCreateMap())
  {
    m_name = nameItem(m_mapItem, enumObj->getMapItem());
    item = createMap();
  }
  else if(mode == enumMode->getModeCreateMapGrid())
  {
    m_name = nameItem(m_mapGridItem, enumObj->getMapGridItem());
    item = createMapGrid();
  }
  else if(mode == enumMode->getModeCreateText())
  {
    m_name = nameItem(m_textItem, enumObj->getTextItem());
    item = createText();
  }
  else if(mode == enumMode->getModeCreateImage())
  {
    m_name = nameItem(m_imageItem, enumObj->getImageItem());
    item = createImage();
  }
  else if(mode == enumMode->getModeCreateRectangle())
  {
    m_name = nameItem(m_rectangleItem, enumObj->getRectangleItem());
    item = createRectangle();
  }
  else if(mode == enumMode->getModeCreateLegend())
  {
    m_name = nameItem(m_legendItem, enumObj->getLegendItem());
    item = createLegend();
  }
  else if(mode == enumMode->getModeCreateScale())
  {
    m_name = nameItem(m_scaleItem, enumObj->getScaleItem());
    item = createScale();
  }
  else if(mode == enumMode->getModeCreateItemGroup())
  {
    m_name = nameItem(m_groupItem, enumObj->getItemGroup());
    item = createItemGroup();
  }
  else if(mode == enumMode->getModeCreatePoint())
  {
    m_name = nameItem(m_pointItem, enumObj->getPointItem());
    item = createPoint();
  }
  else if(mode == enumMode->getModeCreateArrow())
  {
    m_name = nameItem(m_arrowItem, enumObj->getArrowItem());
    item = createArrow();
  }
  else if(mode == enumMode->getModeCreateEllipse())
  {
    m_name = nameItem(m_ellipseItem, enumObj->getEllipseItem());
    item = createEllipse();
  }
  else if(mode == enumMode->getModeCreateTitle())
  {
    m_name = nameItem(m_titleItem, enumObj->getTitleItem());
    item = createTitle();
  }
  else if(mode == enumMode->getModeCreateTextGrid())
  {
    m_name = nameItem(m_textGridItem, enumObj->getTextGridItem());
    item = createTextGrid();
  }
  else if(mode == enumMode->getModeCreateLegendChild())
  {
    m_name = nameItem(m_legendChildItem, enumObj->getLegendChildItem());
    item = createLegendChild();
  }
  else if (mode == enumMode->getModeCreateLine()) 
  {
    m_name = nameItem(m_lineItem, enumObj->getLineItem());
    item = createLine();
  }
  else if (mode == enumMode->getModeCreatePolygon()) 
  {
    m_name = nameItem(m_polygonItem, enumObj->getPolygonItem());
    item = createPolygon();
  }
  else if (mode == enumMode->getModeCreateBalloon()) 
  {
    m_name = nameItem(m_balloonItem, enumObj->getBalloonItem());
    item = createBalloon();
  }
  else if (mode == enumMode->getModeCreateBarCode())
  {
    m_name = nameItem(m_barCodeItem, enumObj->getBarCodeItem());
    item = createBarCode();
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createItem( te::layout::EnumType* type )
{
  QGraphicsItem* item = 0;

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

  if (type == enumObj->getMovingItemGroup())
  {
    item = createMovingItemGroup();
  }
  else if(type == enumObj->getMapItem())
  {
    item = createMap();
  }
  else if(type == enumObj->getMapGridItem())
  {
    item = createMapGrid();
  }
  else if(type == enumObj->getTextItem())
  {
    item = createText();
  }
  else if(type == enumObj->getImageItem())
  {
    item = createImage();
  }
  else if(type == enumObj->getRectangleItem())
  {
    item = createRectangle();
  }
  else if(type == enumObj->getLegendItem())
  {
    item = createLegend();
  }
  else if(type == enumObj->getPaperItem())
  {
    item = createPaper();
  }
  else if(type == enumObj->getScaleItem())
  {
    item = createScale();
  }
  else if(type == enumObj->getItemGroup())
  {
    item = createItemGroup();
  }
  else if(type == enumObj->getArrowItem())
  {
    item = createArrow();
  }
  else if(type == enumObj->getEllipseItem())
  {
    item = createEllipse();
  }
  else if(type == enumObj->getPointItem())
  {
    item = createPoint();
  }
  else if(type == enumObj->getTextGridItem())
  {
    item = createTextGrid();
  }
  else if(type == enumObj->getTitleItem())
  {
    item = createTitle();
  }
  else if(type == enumObj->getLegendChildItem())
  {
    item = createLegendChild();
  }
  else if(type == enumObj->getLineItem())
  {
    item = createLine();
  }
  else if(type == enumObj->getPolygonItem())
  {
    item = createPolygon();
  }
  else if(type == enumObj->getBalloonItem())
  {
    item = createBalloon();
  }
  else if(type == enumObj->getBarCodeItem())
  {
    item = createBarCode();
  }

  return item;
}

std::string te::layout::BuildGraphicsItem::nameItem( std::string name, te::layout::EnumType* type )
{
  AbstractScene* abstScene = Context::getInstance().getScene();

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

  ItemUtils* iUtils = Context::getInstance().getItemUtils();
  m_id = iUtils->maxTypeId(type);

  if(m_id < 0)
    m_id = 0;
  else
    m_id+= 1;

  ss << m_id;

  name+=ss.str();

  return name;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createPaper()
{
  //Paper
  QGraphicsItem* item = 0;

  PaperModel* model = new PaperModel(te::layout::Context::getInstance().getPaperConfig());
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  PaperController* controllerMap = new PaperController(model);
  ItemObserver* itemObs = (ItemObserver*)controllerMap->getView();

  PaperItem* view = dynamic_cast<PaperItem*>(itemObs);

  if(m_props)
  {
    view->updateProperties(m_props);
  }

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
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  MapController* controllerMap = new MapController(model);
  ItemObserver* itemObs = (ItemObserver*)controllerMap->getView();

  MapItem* qrectMap = dynamic_cast<MapItem*>(itemObs);

  if(m_props)
  {
    qrectMap->updateProperties(m_props);
  }
    
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
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  MapGridController* controllerMapGrid = new MapGridController(model);
  ItemObserver* itemObs = (ItemObserver*)controllerMapGrid->getView();

  MapGridItem* qrectMapGrid = dynamic_cast<MapGridItem*>(itemObs);

  if(m_props)
  {
    qrectMapGrid->updateProperties(m_props);
  }
  
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

  TextModel* model = new TextModel();	
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  TextController* controller = new TextController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  TextItem* txt = dynamic_cast<TextItem*>(itemObs); 
  if(m_props)
  {
    txt->updateProperties(m_props);
  }

  if(txt)
  {
    txt->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      txt->setZValue(m_zValue);
    }
    if(m_redraw)
      itemObs->redraw();
    return txt;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createRectangle()
{
  QGraphicsItem* item = 0;

  RectangleModel* model = new RectangleModel();	
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  RectangleController* controller = new RectangleController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  RectangleItem* rect = dynamic_cast<RectangleItem*>(itemObs); 
  if(m_props)
  {
    rect->updateProperties(m_props);
  }

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
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  LegendController* controller = new LegendController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  LegendItem* legend = dynamic_cast<LegendItem*>(itemObs); 
  if(m_props)
  {
    legend->updateProperties(m_props);
  }

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
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  ScaleController* controller = new ScaleController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  ScaleItem* scale = dynamic_cast<ScaleItem*>(itemObs); 
  if(m_props)
  {
    scale->updateProperties(m_props);
  }

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
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  ItemGroupController* controller = new ItemGroupController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  ItemGroup* view = dynamic_cast<ItemGroup*>(itemObs); 
  if(m_props)
  {
    view->updateProperties(m_props);
  }

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

QGraphicsItem* te::layout::BuildGraphicsItem::createMovingItemGroup()
{
  QGraphicsItem* item = 0;

  MovingItemGroupModel* model = new MovingItemGroupModel();	
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  MovingItemGroupController* controller = new MovingItemGroupController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  MovingItemGroup* view = dynamic_cast<MovingItemGroup*>(itemObs); 
  if(m_props)
  {
    view->updateProperties(m_props);
  }

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

QGraphicsItem* te::layout::BuildGraphicsItem::createImage()
{
  QGraphicsItem* item = 0;

  ImageModel* model = new ImageModel();	
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  ImageController* controller = new ImageController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  ImageItem* img = dynamic_cast<ImageItem*>(itemObs); 
  if(m_props)
  {
    img->updateProperties(m_props);
  }

  if(img)
  {
    img->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      img->setZValue(m_zValue);
    }
    if(m_redraw)
      itemObs->redraw();
    return img;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createArrow()
{
  QGraphicsItem* item = 0;

  ArrowModel* model = new ArrowModel();	
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  ArrowController* controller = new ArrowController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  ArrowItem* view = dynamic_cast<ArrowItem*>(itemObs); 
  if(m_props)
  {
    view->updateProperties(m_props);
  }

  if(view)
  {
    view->setPos(QPointF(m_coord.x, m_coord.y));
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

QGraphicsItem* te::layout::BuildGraphicsItem::createEllipse()
{
  QGraphicsItem* item = 0;

  EllipseModel* model = new EllipseModel();	
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  EllipseController* controller = new EllipseController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  EllipseItem* view = dynamic_cast<EllipseItem*>(itemObs); 
  if(m_props)
  {
    view->updateProperties(m_props);
  }

  if(view)
  {
    view->setPos(QPointF(m_coord.x, m_coord.y));
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

QGraphicsItem* te::layout::BuildGraphicsItem::createPoint()
{
  QGraphicsItem* item = 0;

  PointModel* model = new PointModel();	
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  PointController* controller = new PointController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  PointItem* view = dynamic_cast<PointItem*>(itemObs); 
  if(m_props)
  {
    view->updateProperties(m_props);
  }

  if(view)
  {
    view->setPos(QPointF(m_coord.x, m_coord.y));
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

QGraphicsItem* te::layout::BuildGraphicsItem::createTextGrid()
{
  QGraphicsItem* item = 0;

  TextGridModel* model = new TextGridModel();	
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  TextGridController* controller = new TextGridController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  TextGridItem* view = dynamic_cast<TextGridItem*>(itemObs); 
  if(m_props)
  {
    view->updateProperties(m_props);
  }

  if(view)
  {
    view->setPos(QPointF(m_coord.x, m_coord.y));
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

QGraphicsItem* te::layout::BuildGraphicsItem::createTitle()
{
  QGraphicsItem* item = 0;

  TitleModel* model = new TitleModel();	
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  TitleController* controller = new TitleController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  TitleItem* view = dynamic_cast<TitleItem*>(itemObs); 
  if(m_props)
  {
    view->updateProperties(m_props);
  }

  if(view)
  {
    view->setPos(QPointF(m_coord.x, m_coord.y));
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

QGraphicsItem* te::layout::BuildGraphicsItem::createLegendChild()
{
  QGraphicsItem* item = 0;

  LegendChildModel* model = new LegendChildModel();	
  if(!m_props)
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  LegendChildController* controller = new LegendChildController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  LegendChildItem* view = dynamic_cast<LegendChildItem*>(itemObs); 
  if(m_props)
  {
    view->updateProperties(m_props);
  }

  if(view)
  {
    view->setPos(QPointF(m_coord.x, m_coord.y));
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

QGraphicsItem* te::layout::BuildGraphicsItem::createLine() 
{
  QGraphicsItem* item = 0;

  LineModel* model = new LineModel();	
  if(m_props)
  {
    model->updateProperties(m_props);
  }
  else
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  LineController* controller = new LineController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  LineItem* line = dynamic_cast<LineItem*>(itemObs); 

  if(line)
  {
    line->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      line->setZValue(m_zValue);
    }
    if(m_redraw)
      itemObs->redraw();
    return line;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createPolygon() 
{
  QGraphicsItem* item = 0;

  PolygonModel* model = new PolygonModel();	
  if(m_props)
  {
    model->updateProperties(m_props);
  }
  else
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  PolygonController* controller = new PolygonController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  PolygonItem* polygon = dynamic_cast<PolygonItem*>(itemObs); 

  if(polygon)
  {
    polygon->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      polygon->setZValue(m_zValue);
    }
    if(m_redraw)
      itemObs->redraw();
    return polygon;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createBalloon() 
{
  QGraphicsItem* item = 0;

  BalloonModel* model = new BalloonModel();	
  if(m_props)
  {
    model->updateProperties(m_props);
  }
  else
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  BalloonController* controller = new BalloonController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  BalloonItem* balloon = dynamic_cast<BalloonItem*>(itemObs); 

  if(balloon)
  {
    balloon->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_props)
    {
      balloon->setZValue(m_zValue);
    }
    if(m_redraw)
      itemObs->redraw();
    return balloon;
  }

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createBarCode()
{
  QGraphicsItem* item = 0;

  BarCodeModel* model = new BarCodeModel;
  if(m_props)
  {
    model->updateProperties(m_props);
  }
  else
  {
    model->setId(m_id);
    model->setName(m_name);
  }

  BarCodeController* controller = new BarCodeController(model);
  ItemObserver* itemObs = (ItemObserver*)controller->getView();

  BarCodeItem* barCode = dynamic_cast<BarCodeItem*>(itemObs);

  if(barCode)
  {
    barCode->setPos(QPointF(m_coord.x, m_coord.y));
    if(m_redraw)
      itemObs->redraw();
    return barCode;
  }

  return item;
}