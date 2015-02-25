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
  \file LineModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LineModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../../geometry/Point.h"
#include "../core/pattern/singleton/Context.h"
#include "../core/Utils.h"
#include "../core/pattern/mvc/ItemObserver.h"

te::layout::LineModel::LineModel() 
{
  m_type = Enums::getInstance().getEnumObjectType()->getLineItem();

  m_borderColor = te::color::RGBAColor(255, 255, 255, 0);
  m_box = te::gm::Envelope(0., 0., 20., 20.);
}

te::layout::LineModel::~LineModel()
{

}

void te::layout::LineModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;
  m_context = context;
  
  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();
    
  if((!canvas) || (!utils))
    return;

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);
  
  drawBackground(context);
  drawCoords(canvas, utils);

 if(context.isResizeCanvas())
  pixmap = utils->getImageW(m_box);
    
 context.setPixmap(pixmap);
 notifyAll(context);
}

void te::layout::LineModel::setCoords( std::vector<te::gm::Point*> coords )
{
  m_coords = coords;

  int sizeMCoords = 0;
  sizeMCoords = m_coords.size();

  te::gm::LinearRing *lineOfPoints = new te::gm::LinearRing(sizeMCoords, te::gm::LineStringType);
  
  for(int i = 0; i < sizeMCoords; ++i)
  {
    lineOfPoints->setPointN( i, te::gm::Point(m_coords[i]->getX(), m_coords[i]->getY()));    
  }

  const te::gm::Envelope *returnBox = lineOfPoints->getMBR();
  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;
  x1 = returnBox->getLowerLeftX();
  y1 = returnBox->getLowerLeftY();
  x2 = returnBox->getUpperRightX();
  y2 = returnBox->getUpperRightY();

  m_box = te::gm::Envelope (x1-1, y1-1, x2+1, y2+1);

  if (lineOfPoints)
  {
    delete lineOfPoints;
    lineOfPoints = 0;
  }  
}

void te::layout::LineModel::drawCoords( te::map::Canvas* canvas, Utils* utils )
{
  if (m_coords.empty())
    return;

  te::gm::LinearRing *lineOfPoints = new te::gm::LinearRing(m_coords.size(), te::gm::LineStringType);
  
  for (unsigned int i = 0; m_coords.size() > i; ++i)
  {
    lineOfPoints->setPointN( i, te::gm::Point( m_coords[i]->getX(), m_coords[i]->getY() ) );   
  }

  canvas->setLineColor(te::color::RGBAColor(0, 0, 0, 255));
  canvas->draw(lineOfPoints);

  if (lineOfPoints)
  {
    delete lineOfPoints;
    lineOfPoints = 0;
  }
}
