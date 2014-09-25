/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
#include "../../../item/RectangleItem.h"
#include "../../../item/HorizontalRulerItem.h"
#include "../../../item/VerticalRulerItem.h"
#include "../../../item/ItemGroup.h"
#include "../../../item/PaperItem.h"
#include "../../../item/MapItem.h"
#include "../../../item/MapGridItem.h"
#include "../../../item/LegendItem.h"
#include "../../../item/ScaleItem.h"
#include "../../../item/DefaultTextItem.h"
#include "../../../item/ImageItem.h"
#include "../../../item/PointItem.h"
#include "../../../item/ArrowItem.h"
#include "../../../item/EllipseItem.h"
#include "../../../item/TitleItem.h"
#include "../../../item/MultiLineTextItem.h"
#include "../../../item/TextGridItem.h"
#include "../../../../core/enum/Enums.h"

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

  if(type == enumObj->getRetangleItem())
  {
    RectangleItem* rectangle = new RectangleItem(params.getController(), params.getModel());
    item = (Observer*)rectangle;
  }
  else if(type == enumObj->getLegendItem())
  {
    LegendItem* legend = new LegendItem(params.getController(), params.getModel());
    item = (Observer*)legend;
  }
  else if(type == enumObj->getHorizontalRuler())
  {
    HorizontalRulerItem* ruler = new HorizontalRulerItem(params.getController(), params.getModel());
    item = (Observer*)ruler;
  }
  else if(type == enumObj->getVerticalRuler())
  {
    VerticalRulerItem* ruler = new VerticalRulerItem(params.getController(), params.getModel());
    item = (Observer*)ruler;
  }
  else if(type == enumObj->getDefaultTextItem())
  {
    DefaultTextItem* txt = new DefaultTextItem(params.getController(), params.getModel());
    txt->init();
    item = (Observer*)txt;
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
  else if(type == enumObj->getMapGridItem())
  {
    MapGridItem* mapGrid = new MapGridItem(params.getController(), params.getModel());
    item = (Observer*)mapGrid;
  }
  else if(type == enumObj->getScaleItem())
  {
    ScaleItem* scale = new ScaleItem(params.getController(), params.getModel());
    item = (Observer*)scale;
  }
  else if(type == enumObj->getImageItem())
  {
    ImageItem* scale = new ImageItem(params.getController(), params.getModel());
    item = (Observer*)scale;
  }
  else if(type == enumObj->getPointItem())
  {
    PointItem* point = new PointItem(params.getController(), params.getModel());
    item = (Observer*)point;
  }
  else if(type == enumObj->getArrowItem())
  {
    ArrowItem* arrow = new ArrowItem(params.getController(), params.getModel());
    item = (Observer*)arrow;
  }
  else if(type == enumObj->getEllipseItem())
  {
    EllipseItem* ellipse = new EllipseItem(params.getController(), params.getModel());
    item = (Observer*)ellipse;
  }
  else if(type == enumObj->getTitleItem())
  {
    TitleItem* title = new TitleItem(params.getController(), params.getModel());
    title->init();
    item = (Observer*)title;
  }
  else if(type == enumObj->getTextGridItem())
  {
    TextGridItem* txtGrid = new TextGridItem(params.getController(), params.getModel());
    txtGrid->init();
    item = (Observer*)txtGrid;
  }
  else if(type == enumObj->getMultiLineTextItem())
  {
    MultiLineTextItem* mText = new MultiLineTextItem(params.getController(), params.getModel());
    item = (Observer*)mText;
  }

  return item;
}
