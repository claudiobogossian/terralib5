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
  \file QLayoutItemFactory.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "QLayoutItemFactory.h"
#include "QRectangleLayoutItem.h"
#include "QHorizontalRulerLayoutItem.h"
#include "QVerticalRulerLayoutItem.h"
#include "QLayoutItemGroup.h"
#include "QPaperLayoutItem.h"
#include "QMapLayoutItem.h"
#include "LayoutObserver.h"

te::layout::QLayoutItemFactory::QLayoutItemFactory()
{

}

te::layout::QLayoutItemFactory::~QLayoutItemFactory()
{

}

te::layout::LayoutObserver* te::layout::QLayoutItemFactory::make( LayoutAbstractObjectType type, LayoutItemParamsCreate params /*= LayoutItemParamsCreate()*/ )
{
  LayoutObserver* item = 0;

  switch (type)
  {

  case TPRetangleItem:
    {
      QRectangleLayoutItem* rectangle = new QRectangleLayoutItem(params.getController(), params.getModel());
      item = (LayoutObserver*)rectangle;
      return item;
      break;
    }
  case TPLegendItem:
    {
      break;
    }
  case TPHorizontalRuler:
    {
      QHorizontalRulerLayoutItem* ruler = new QHorizontalRulerLayoutItem(params.getController(), params.getModel());
      item = (LayoutObserver*)ruler;
      return item;
      break;
    }
  case TPVerticalRuler:
    {
      QVerticalRulerLayoutItem* ruler = new QVerticalRulerLayoutItem(params.getController(), params.getModel());
      item = (LayoutObserver*)ruler;
      return item;
      break;
    }
  case TPText:
    {
      break;
    }
  case TPItemGroup:
    {
      QLayoutItemGroup* group = new QLayoutItemGroup(params.getController(), params.getModel());
      item = (LayoutObserver*)group;
      return item;
      break;
    }
  case TPPaperItem:
    {
      QPaperLayoutItem* paper = new QPaperLayoutItem(params.getController(), params.getModel());
      item = (LayoutObserver*)paper;
      return item;
      break;
    }
  case TPMapItem:
    {
      QMapLayoutItem* map = new QMapLayoutItem(params.getController(), params.getModel());
      item = (LayoutObserver*)map;
      return item;
      break;
    }
  }

  return 0;
}
