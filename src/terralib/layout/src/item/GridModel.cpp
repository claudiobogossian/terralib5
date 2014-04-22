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
  \file GridModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridModel.h"
#include "ContextItem.h"
#include "Context.h"
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/Coord2D.h"
#include "Properties.h"
#include "Property.h"

te::layout::GridModel::GridModel() :
  m_gridPlanar(false),
  m_horizontalSeparationParameter(10), 
  m_verticalSeparationParameter(10),
  m_initialGridPointX(0),
  m_initialGridPointY(0),
  m_name("GRID_MODEL")
{
  m_properties = new te::layout::Properties(m_name, TPGrid);
}

te::layout::GridModel::~GridModel()
{

}

void te::layout::GridModel::draw( te::map::Canvas* canvas, te::gm::Envelope box )
{
  Utils* utils = Context::getInstance()->getUtils();

  te::color::RGBAColor color = te::color::RGBAColor(0, 0, 0, 255);
  canvas->setLineColor(color);

  drawVerticalLines(canvas, box);
  drawHorizontalLines(canvas, box);
}

void te::layout::GridModel::drawVerticalLines(te::map::Canvas* canvas, te::gm::Envelope box)
{
  Utils* utils = Context::getInstance()->getUtils();

  int totallines = (int)std::abs(std::ceil(box.getWidth() / m_verticalSeparationParameter));

  double count = 0;
  for(int i = 0 ; i < totallines ; i++)
  {
    te::gm::Envelope newBox(box.getLowerLeftX() + count, box.getLowerLeftY(), 
      box.getLowerLeftX() + count, box.getUpperRightY());
    te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);

    line = utils->createSimpleLine(newBox);
    utils->drawLineW(line);

    if(line)
    {
      delete line;
      line = 0;
    }

    count+= m_verticalSeparationParameter;
  }
}

void te::layout::GridModel::drawHorizontalLines(te::map::Canvas* canvas, te::gm::Envelope box)
{
  Utils* utils = Context::getInstance()->getUtils();

  int totallines = (int)std::abs(std::ceil(box.getWidth() / m_horizontalSeparationParameter));

  double count = 0;
  for(int i = 0 ; i < totallines ; i++)
  {
    te::gm::Envelope newBox(box.getLowerLeftX(), box.getLowerLeftY() + count, 
      box.getUpperRightX(), box.getLowerLeftY() + count);
    te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);

    line = utils->createSimpleLine(newBox);
    utils->drawLineW(line);

    if(line)
    {
      delete line;
      line = 0;
    }

    count+= m_horizontalSeparationParameter;
  }
}

bool te::layout::GridModel::isGridPlanar()
{
  return m_gridPlanar;
}

void te::layout::GridModel::setGridPlanar( bool planar )
{
  m_gridPlanar = planar;
}

te::map::LineDashStyle te::layout::GridModel::getPlanarLineStyle()
{
  return m_planarLineStyle;
}

void te::layout::GridModel::setPlanarLineStyle( te::map::LineDashStyle style )
{
  m_planarLineStyle = style;
}

te::layout::Properties* te::layout::GridModel::getProperties() const
{
  m_properties->clear();

  Property pro_name;
  pro_name.setName("name");
  pro_name.setId("unknown");
  pro_name.setValue(m_name, DataTypeString);

  m_properties->addProperty(pro_name);

  m_properties->setObjectName(m_name);
  m_properties->setTypeObj(TPGrid);
  return m_properties;
}

void te::layout::GridModel::updateProperties( te::layout::Properties* properties )
{

}

std::string te::layout::GridModel::getName()
{
  return m_name;
}

void te::layout::GridModel::setName( std::string name )
{
  m_name = name;
}



