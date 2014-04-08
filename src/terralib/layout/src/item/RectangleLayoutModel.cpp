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
  \file RectangleLayoutModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "RectangleLayoutModel.h"
#include "ContextLayoutItem.h"
#include "LayoutContext.h"
#include "../../../geometry/Envelope.h"
#include "../../../color/RGBAColor.h"
#include "../../../maptools/Canvas.h"

te::layout::RectangleLayoutModel::RectangleLayoutModel() 
{
  m_borderColor = te::color::RGBAColor(0, 0, 255, 255);
  m_backgroundColor = te::color::RGBAColor(0, 255, 0, 100);

  m_box = te::gm::Envelope(0., 0., 10., 10.);
}

te::layout::RectangleLayoutModel::~RectangleLayoutModel()
{

}

void te::layout::RectangleLayoutModel::draw( ContextLayoutItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  LayoutUtils* utils = LayoutContext::getInstance()->getUtils();

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);
  
  canvas->setPolygonContourColor(m_borderColor);
  canvas->setPolygonFillColor(m_backgroundColor);
  
  utils->drawRectW(m_box);
  
  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
  
  ContextLayoutItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

