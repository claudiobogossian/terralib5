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
  \file AbstractBuildGraphicsItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "AbstractBuildGraphicsItem.h"
#include "property/SharedProperties.h"
#include "property/Property.h"
#include "property/Properties.h"

te::layout::AbstractBuildGraphicsItem::AbstractBuildGraphicsItem() :
  m_id(0),
  m_width(0),
  m_height(0),
  m_name("unknown")
{

}

te::layout::AbstractBuildGraphicsItem::~AbstractBuildGraphicsItem()
{
  
}

te::gm::Coord2D te::layout::AbstractBuildGraphicsItem::findCoordinate( te::layout::Properties props )
{
  /* Coordinate - x1, y1*/

  double x1 = 0;
  double y1 = 0;

  Property pro_x1 = props.getProperty("x");

  if(!pro_x1.isNull())
  {
    x1 = pro_x1.getValue().toDouble();
  }

  Property pro_y1 = props.getProperty("y");

  if(!pro_y1.isNull())
  {
    y1 = pro_y1.getValue().toDouble();
  }

  te::gm::Coord2D coord(x1, y1);
  return coord;
}

int te::layout::AbstractBuildGraphicsItem::findZValue( te::layout::Properties props )
{
  int zValue = -1;

  Property pro_zValue = props.getProperty("zValue");

  if(!pro_zValue.isNull())
  {
    zValue = pro_zValue.getValue().toInt();
  }

  return zValue;
}

std::string te::layout::AbstractBuildGraphicsItem::findName(te::layout::Properties props)
{
  std::string name = "";
  Property pro_name = props.getProperty("name");
  if (!pro_name.isNull())
  {
    name = pro_name.getValue().toString();
  }
  return name;
}

void te::layout::AbstractBuildGraphicsItem::clear()
{
  m_id = 0;
  m_props.clear();
  m_width = 0;
  m_height = 0;
  m_name = "unknown";
}


