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
  \file EllipseModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EllipseModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../core/enum/Enums.h"

// STL
#include <cmath>

te::layout::EllipseModel::EllipseModel() 
{
  m_type = Enums::getInstance().getEnumObjectType()->getEllipseItem();

  m_box = te::gm::Envelope(0., 0., 22., 20.);
}

te::layout::EllipseModel::~EllipseModel()
{

}

void te::layout::EllipseModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();

  if((!canvas) || (!utils))
    return;

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);
  
  drawBackground(context);
  
  drawEllipse(canvas, utils);
    
  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
  
  context.setPixmap(pixmap);
  notifyAll(context);
}

void te::layout::EllipseModel::drawEllipse( te::map::Canvas* canvas, Utils* utils)
{
    double rx = (m_box.m_urx - m_box.m_llx) / 2.;
    double ry = (m_box.m_ury - m_box.m_lly) / 2.;
    double cx = (m_box.m_urx + m_box.m_llx) / 2.;
    double cy = (m_box.m_ury + m_box.m_lly) / 2.;

    double lastX = m_box.m_llx;
    double lastY = m_box.m_lly;

    double x = 0;
    double y = 0;
    double angle = 0;

    for(int i = 0 ; i < 360 ; ++i)
    {
      angle = i;
      x = cx + std::cos(angle) * rx;
      y = cy + std::sin(angle) * ry;

      if (angle > 0) 
      {
        te::gm::Envelope box(lastX, lastY, x, y);
        te::gm::LinearRing* line = utils->createSimpleLine(box);
        utils->drawLineW(line);
        if(line) delete line;        
      }

      lastX = x;
      lastY = y;
    }
}
