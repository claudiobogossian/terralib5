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
#include "../../../item/StringGridItem.h"
#include "../../../item/MultiLineTextItem.h"

te::layout::ItemFactory::ItemFactory()
{

}

te::layout::ItemFactory::~ItemFactory()
{

}

te::layout::Observer* te::layout::ItemFactory::make( LayoutAbstractObjectType type, ItemParamsCreate params /*= LayoutItemParamsCreate()*/ )
{
  Observer* item = 0;

  switch (type)
  {

  case TPRetangleItem:
    {
      RectangleItem* rectangle = new RectangleItem(params.getController(), params.getModel());
      item = (Observer*)rectangle;
      break;
    }
  case TPLegendItem:
    {
      LegendItem* legend = new LegendItem(params.getController(), params.getModel());
      item = (Observer*)legend;
      break;
    }
  case TPHorizontalRuler:
    {
      HorizontalRulerItem* ruler = new HorizontalRulerItem(params.getController(), params.getModel());
      item = (Observer*)ruler;
      break;
    }
  case TPVerticalRuler:
    {
      VerticalRulerItem* ruler = new VerticalRulerItem(params.getController(), params.getModel());
      item = (Observer*)ruler;
      break;
    }
  case TPTextItem:
    {
      DefaultTextItem* txt = new DefaultTextItem(params.getController(), params.getModel());
      item = (Observer*)txt;
      break;
    }
  case TPItemGroup:
    {
      ItemGroup* group = new ItemGroup(params.getController(), params.getModel());
      item = (Observer*)group;
      break;
    }
  case TPPaperItem:
    {
      PaperItem* paper = new PaperItem(params.getController(), params.getModel());
      item = (Observer*)paper;
      break;
    }
  case TPMapItem:
    {
      MapItem* map = new MapItem(params.getController(), params.getModel());
      item = (Observer*)map;
      break;
    }
  case TPMapGridItem:
    {
      MapGridItem* mapGrid = new MapGridItem(params.getController(), params.getModel());
      item = (Observer*)mapGrid;
      break;
    }
  case TPScaleItem:
    {
      ScaleItem* scale = new ScaleItem(params.getController(), params.getModel());
      item = (Observer*)scale;
      break;
    }
  case TPImageItem:
    {
      ImageItem* scale = new ImageItem(params.getController(), params.getModel());
      item = (Observer*)scale;
      break;
    }
  case TPPointItem:
    {
      PointItem* point = new PointItem(params.getController(), params.getModel());
      item = (Observer*)point;
      break;
    }
  case TPArrowItem:
    {
      ArrowItem* arrow = new ArrowItem(params.getController(), params.getModel());
      item = (Observer*)arrow;
      break;
    }
  case TPEllipseItem:
    {
      EllipseItem* ellipse = new EllipseItem(params.getController(), params.getModel());
      item = (Observer*)ellipse;
      break;
    }
  case TPTitleItem:
    {
      TitleItem* title = new TitleItem(params.getController(), params.getModel());
      item = (Observer*)title;
      break;
    }
  case TPStringGridItem:
    {
      StringGridItem* sGrid = new StringGridItem(params.getController(), params.getModel());
      item = (Observer*)sGrid;
      break;
    }
  case TPMultiLineTextItem:
    {
      MultiLineTextItem* mText = new MultiLineTextItem(params.getController(), params.getModel());
      item = (Observer*)mText;
      break;
    }
  default:
    item = 0;
  }

  return item;
}
