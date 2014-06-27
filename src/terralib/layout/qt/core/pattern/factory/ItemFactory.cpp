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
#include "RectangleItem.h"
#include "HorizontalRulerItem.h"
#include "VerticalRulerItem.h"
#include "ItemGroup.h"
#include "PaperItem.h"
#include "MapItem.h"
#include "Observer.h"
#include "MapGridItem.h"
#include "LegendItem.h"
#include "ScaleItem.h"

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
      return item;
      break;
    }
  case TPLegendItem:
    {
      LegendItem* legend = new LegendItem(params.getController(), params.getModel());
      item = (Observer*)legend;
      return item;
      break;
    }
  case TPHorizontalRuler:
    {
      HorizontalRulerItem* ruler = new HorizontalRulerItem(params.getController(), params.getModel());
      item = (Observer*)ruler;
      return item;
      break;
    }
  case TPVerticalRuler:
    {
      VerticalRulerItem* ruler = new VerticalRulerItem(params.getController(), params.getModel());
      item = (Observer*)ruler;
      return item;
      break;
    }
  case TPText:
    {
      break;
    }
  case TPItemGroup:
    {
      ItemGroup* group = new ItemGroup(params.getController(), params.getModel());
      item = (Observer*)group;
      return item;
      break;
    }
  case TPPaperItem:
    {
      PaperItem* paper = new PaperItem(params.getController(), params.getModel());
      item = (Observer*)paper;
      return item;
      break;
    }
  case TPMapItem:
    {
      MapItem* map = new MapItem(params.getController(), params.getModel());
      item = (Observer*)map;
      return item;
      break;
    }
  case TPMapGridItem:
    {
      MapGridItem* mapGrid = new MapGridItem(params.getController(), params.getModel());
      item = (Observer*)mapGrid;
      return item;
      break;
    }
  case TPScaleItem:
    {
      ScaleItem* scale = new ScaleItem(params.getController(), params.getModel());
      item = (Observer*)scale;
      return item;
      break;
    }
  }

  return 0;
}
