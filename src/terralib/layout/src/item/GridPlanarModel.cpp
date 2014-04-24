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
  \file GridPlanarModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridPlanarModel.h"
#include "ContextItem.h"
#include "Context.h"
#include "../../../../../geometry/Envelope.h"
#include "../../../../../geometry/Coord2D.h"
#include "Property.h"

te::layout::GridPlanarModel::GridPlanarModel() 
{
  m_name = "GRID_PLANAR_MODEL";
  m_lneVrtDisplacement = 10;
  m_lneHrzDisplacement = 10;
}

te::layout::GridPlanarModel::~GridPlanarModel()
{

}

void te::layout::GridPlanarModel::draw( te::map::Canvas* canvas, te::gm::Envelope box )
{
  Utils* utils = Context::getInstance()->getUtils();

  te::color::RGBAColor color = te::color::RGBAColor(0, 0, 0, 255);
  canvas->setLineColor(color);

  drawVerticalLines(canvas, box);
  drawHorizontalLines(canvas, box);
}

void te::layout::GridPlanarModel::drawVerticalLines(te::map::Canvas* canvas, te::gm::Envelope box)
{
  Utils* utils = Context::getInstance()->getUtils();

  int totallines = (int)std::abs(std::ceil(box.getWidth() / m_lneVrtDisplacement));

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

    count+= m_lneVrtDisplacement;
  }
}

void te::layout::GridPlanarModel::drawHorizontalLines(te::map::Canvas* canvas, te::gm::Envelope box)
{
  Utils* utils = Context::getInstance()->getUtils();

  int totallines = (int)std::abs(std::ceil(box.getWidth() / m_lneHrzDisplacement));

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

    count+= m_lneHrzDisplacement;
  }
}

te::layout::Properties* te::layout::GridPlanarModel::getProperties() const
{
  return GridModel::getProperties();
}

void te::layout::GridPlanarModel::updateProperties( te::layout::Properties* properties )
{
  GridModel::updateProperties(properties);
}

