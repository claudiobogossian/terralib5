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
  \file ArrowModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ArrowModel.h"
#include "../core/ContextItem.h"

// STL
#include <cmath>

te::layout::ArrowModel::ArrowModel() 
{
  m_box = te::gm::Envelope(0., 0., 10., 10.);
}

te::layout::ArrowModel::~ArrowModel()
{

}

void te::layout::ArrowModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();

  if((!canvas) || (!utils))
    return;

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);
  
  drawArrow(canvas, utils);
  
  canvas->setPolygonContourWidth(2);
  canvas->setPolygonContourColor(m_borderColor);
  canvas->setPolygonFillColor(m_backgroundColor);

  if(m_border)
  {
    utils->drawRectW(m_box);
  }
  
  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
  
  context.setPixmap(pixmap);
  notifyAll(context);
}

void te::layout::ArrowModel::drawArrow( te::map::Canvas* canvas, Utils* utils )
{
  canvas->setPolygonContourWidth(2);
  canvas->setPolygonContourColor(te::color::RGBAColor(0, 0, 0, 255));
  canvas->setPolygonFillColor(te::color::RGBAColor(0, 0, 0, 255));
  canvas->setLineColor(te::color::RGBAColor(0, 0, 0, 255));

  double y = m_box.getCenter().y;

  /* Draw Arrow */
  te::gm::Envelope box(m_box.m_llx, y, m_box.m_urx, y);
  
  te::gm::LinearRing* line = utils->createSimpleLine(box);
  utils->drawLineW(line);
  if(line) delete line;

  /* Draw Arrow Head */
  drawHeadArrow(canvas, utils, box);
}

void te::layout::ArrowModel::drawHeadArrow( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box )
{
  double w = m_box.getWidth();
  double h = m_box.getHeight();

  double initHead = box.m_urx - (w / 4.);
  double sizeHead = (h / 4.);

  double	slopy , cosy , siny;
  slopy = std::atan2((double)( box.getLowerLeftY() - box.getUpperRightY() ),(double)( box.getLowerLeftX() - box.getUpperRightX() ) );
  cosy = std::cos( slopy );
  siny = std::sin( slopy );

  double rotate1 = int(- sizeHead * cosy - (sizeHead / 2.0 * siny));
  double rotate2 = int(- sizeHead * siny + (sizeHead / 2.0 * cosy ));
  double rotate3 = int(- sizeHead * cosy + (sizeHead / 2.0 * siny));
  double rotate4 = int(sizeHead / 2.0 * cosy + sizeHead * siny);

  te::gm::Polygon* rect = new te::gm::Polygon(1, te::gm::PolygonType, 0, &box);

  te::gm::LinearRing* outRingPtr0 = new te::gm::LinearRing(4, te::gm::LineStringType);
  outRingPtr0->setPointN( 0, te::gm::Point(initHead, box.getLowerLeftY()));
  outRingPtr0->setPointN( 1, te::gm::Point(box.getUpperRightX() - rotate1, box.getLowerLeftY() - rotate2)); 
  outRingPtr0->setPointN( 2, te::gm::Point(box.getUpperRightX() - rotate3, box.getUpperRightY() + rotate4));
  outRingPtr0->setPointN( 3, te::gm::Point(initHead, box.getUpperRightY())); 

  rect->setRingN(0, outRingPtr0);

  canvas->draw(rect);
}
