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
  \file ItemFactory.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemFactory.h"
#include "../../../../core/pattern/mvc/Observer.h"
#include "../../../../core/enum/Enums.h"
#include "../../../item/RectangleItem.h"
#include "../../../item/ItemGroup.h"
#include "../../../item/PaperItem.h"
#include "../../../item/MapItem.h"
#include "../../../item/GridMapItem.h"
#include "../../../item/LegendItem.h"
#include "../../../item/ScaleItem.h"
#include "../../../item/ImageItem.h"
#include "../../../item/PointItem.h"
#include "../../../item/ArrowItem.h"
#include "../../../item/EllipseItem.h"
#include "../../../item/TitleItem.h"
#include "../../../item/TextGridItem.h"
#include "../../../item/LegendChildItem.h"
#include "../../../item/TextItem.h"
#include "../../../item/MovingItemGroup.h"
#include "../../../item/LineItem.h"
#include "../../../item/PolygonItem.h"
#include "../../../item/BalloonItem.h"
#include "../../../item/BarCodeItem.h"
#include "../../../item/GridPlanarItem.h"
#include "../../../item/GridGeodesicItem.h"
#include "../../../item/NorthItem.h"
#include "../../../item/MapLocationItem.h"
#include "../../../item/SVGItem.h"

te::layout::ItemFactory::ItemFactory()
{

}

te::layout::ItemFactory::~ItemFactory()
{

}

te::layout::Observer* te::layout::ItemFactory::make( EnumType* type, ItemParamsCreate params /*= LayoutItemParamsCreate()*/ )
{
  Observer* item = 0;

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

  if(type == enumObj->getLegendItem())
  {
    LegendItem* legend = new LegendItem(params.getController(), params.getModel());
    item = (Observer*)legend;
  }
  else if(type == enumObj->getItemGroup())
  {
    ItemGroup* group = new ItemGroup(params.getController(), params.getModel());
    item = (Observer*)group;
  }
  else if(type == enumObj->getPaperItem())
  {
    PaperItem* paper = new PaperItem(params.getController(), params.getModel());
    item = (Observer*)paper;
  }
  else if(type == enumObj->getMapItem())
  {
    MapItem* map = new MapItem(params.getController(), params.getModel());
    item = (Observer*)map;
  }
  else if(type == enumObj->getGridMapItem())
  {
    GridMapItem* gridMap = new GridMapItem(params.getController(), params.getModel());
    item = (Observer*)gridMap;
  }
  else if(type == enumObj->getLegendChildItem())
  {
    LegendChildItem* mText = new LegendChildItem(params.getController(), params.getModel());
    item = (Observer*)mText;
  }
  else if (type == enumObj->getMovingItemGroup())
  {
    MovingItemGroup* mItemGroup = new MovingItemGroup(params.getController(), params.getModel());
    item = (Observer*) mItemGroup;
  }
  else if(type == enumObj->getLineItem())
  {
    LineItem* line = new LineItem(params.getController(), params.getModel());
    item = (Observer*)line;
  }
  else if(type == enumObj->getPolygonItem())
  {
    PolygonItem* polygon = new PolygonItem(params.getController(), params.getModel());
    item = (Observer*)polygon;
  }
  else if(type == enumObj->getBalloonItem())
  {
    BalloonItem* balloon = new BalloonItem(params.getController(), params.getModel());
    item = (Observer*)balloon;
  }
  else if(type == enumObj->getGridPlanarItem())
  {
    GridPlanarItem* gridPlanar = new GridPlanarItem(params.getController(), params.getModel());
    item = (Observer*)gridPlanar;
  }
  else if(type == enumObj->getGridGeodesicItem())
  {
    GridGeodesicItem* gridGeodesic = new GridGeodesicItem(params.getController(), params.getModel());
    item = (Observer*)gridGeodesic;
  }
  else if(type == enumObj->getMapLocationItem())
  {
    MapLocationItem* location = new MapLocationItem(params.getController(), params.getModel());
    item = (Observer*)location;
  }
  else if(type == enumObj->getSVGItem())
  {
    SVGItem* point = new SVGItem(params.getController(), params.getModel());
    item = (Observer*)point;
  }
  return item;
}

te::layout::AbstractItemView* te::layout::ItemFactory::makeNew(EnumType* type, ItemParamsCreate params)
{
  AbstractItemView* item = 0;

  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

  if(type == enumObj->getRectangleItem())
  {
    item = new RectangleItem(params.m_newController, params.m_newModel);
  }
  else if(type == enumObj->getEllipseItem())
  {
    item = new EllipseItem(params.m_newController, params.m_newModel);
  }
  else if(type == enumObj->getArrowItem())
  {
    item = new ArrowItem(params.m_newController, params.m_newModel);
  }
  else if(type == enumObj->getScaleItem())
  {
    item = new ScaleItem(params.m_newController, params.m_newModel);
  }
  else if(type == enumObj->getPointItem())
  {
    item = new PointItem(params.m_newController, params.m_newModel);
  }
  else if(type == enumObj->getTextItem())
  {
    item = new TextItem(params.m_newController, params.m_newModel);
  }
  else if(type == enumObj->getTitleItem())
  {
    item = new TitleItem(params.m_newController, params.m_newModel);
  }
  else if(type == enumObj->getTextGridItem())
  {
    item = new TextGridItem(params.m_newController, params.m_newModel);
  }
  else if(type == enumObj->getBarCodeItem())
  {
    item = new BarCodeItem(params.m_newController, params.m_newModel);
  }
  else if(type == enumObj->getNorthItem())
  {
    item = new NorthItem(params.m_newController, params.m_newModel);
  }
  else if(type == enumObj->getImageItem())
  {
    item = new ImageItem(params.m_newController, params.m_newModel);
  }

  return item;
}

