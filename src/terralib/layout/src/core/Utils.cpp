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
  \file Utils.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Utils.h"
#include "Context.h"
#include "Scene.h"
#include "../../../color/RGBAColor.h"
#include "../../../geometry/Polygon.h"
#include "../../../geometry/Enums.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/Point.h"
#include "../../../qt/widgets/canvas/Canvas.h"

te::layout::Utils::Utils()
{

}

te::layout::Utils::~Utils()
{

}

void te::layout::Utils::drawRectW( te::gm::Envelope box )
{
  te::map::Canvas* canvas = Context::getInstance()->getCanvas();

  te::gm::Polygon* rect = new te::gm::Polygon(1, te::gm::PolygonType, 0, &box);
  
  te::gm::LinearRing* outRingPtr0 = new te::gm::LinearRing(5, te::gm::LineStringType);
  outRingPtr0->setPointN( 0, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));
  outRingPtr0->setPointN( 1, te::gm::Point(box.getUpperRightX(), box.getLowerLeftY())); 
  outRingPtr0->setPointN( 2, te::gm::Point(box.getUpperRightX(), box.getUpperRightY()));
  outRingPtr0->setPointN( 3, te::gm::Point(box.getLowerLeftX(), box.getUpperRightY())); 
  outRingPtr0->setPointN( 4, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY())); 

  rect->setRingN(0, outRingPtr0);
  canvas->draw(rect);
}

void te::layout::Utils::drawLineW( te::gm::LinearRing* line )
{
  te::map::Canvas* canvas = Context::getInstance()->getCanvas();

  canvas->draw(line);
}

te::gm::LinearRing* te::layout::Utils::createSimpleLine( te::gm::Envelope box )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  line->setNumCoordinates(3);

  line->setPointN(0, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));
  line->setPointN(1, te::gm::Point(box.getUpperRightX(), box.getUpperRightY()));
  line->setPointN(2, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));

  return line;
}

te::color::RGBAColor** te::layout::Utils::getImageW( te::gm::Envelope box )
{
  te::map::Canvas* canvas = Context::getInstance()->getCanvas();
  te::color::RGBAColor** pixmap = 0;

  te::gm::Envelope boxViewport = viewportBox(box);

  if(boxViewport.isValid())
    pixmap = canvas->getImage(0, 0, boxViewport.getWidth(), boxViewport.getHeight());
  return pixmap;
}

int te::layout::Utils::mm2pixel( double mm )
{
  //te::map::Canvas* canvas = LayoutContext::getInstance()->getCanvas();
  //te::qt::widgets::Canvas* canvascopy = static_cast<te::qt::widgets::Canvas*>(canvas);

  //int devDpi = canvascopy->getResolution();
  int devDpi = Context::getInstance()->getDpiX();
  int px = (mm * devDpi) / 25.4 ;
  return px;
}

void te::layout::Utils::configCanvas( te::gm::Envelope box )
{
  te::map::Canvas* canvas = Context::getInstance()->getCanvas();
  te::gm::Envelope boxViewport = viewportBox(box);

  //Transparent
  te::color::RGBAColor color(255,255,255, 0);
  canvas->setBackgroundColor(color);

  canvas->resize(boxViewport.getWidth(), boxViewport.getHeight());
  canvas->setWindow(box.getLowerLeftX(), box.getLowerLeftY(), 
    box.getUpperRightX(), box.getUpperRightY());  
}

te::gm::Envelope te::layout::Utils::viewportBox( te::gm::Envelope box )
{
  int pxwidth = mm2pixel(box.getWidth());
  int pxheight = mm2pixel(box.getHeight());
  
  // Adjust internal renderer transformer
  m_transformer.setTransformationParameters(box.getLowerLeftX(), box.getLowerLeftY(), 
    box.getUpperRightX(), box.getUpperRightY(), pxwidth, pxheight);

  double px1 = 0;
  double py1 = 0;
  double px2 = 0;
  double py2 = 0;
  double pycopy = 0;
  m_transformer.world2Device(box.getLowerLeftX(), box.getLowerLeftY(), px1, py1);
  m_transformer.world2Device(box.getUpperRightX(), box.getUpperRightY(), px2, py2);
  
  if(py1 > py2 )
  {
    pycopy = py1;
    py1 = py2;
    py2 = pycopy;
  }

  te::gm::Envelope boxViewport(px1, py1, px2, py2);
  return boxViewport;
}

te::gm::Envelope te::layout::Utils::viewportBoxFromScene( te::gm::Envelope box )
{
  te::gm::Envelope boxScene = Context::getInstance()->getScene()->getSceneBox();

  double px1 = 0;
  double py1 = 0;
  double px2 = 0;
  double py2 = 0;
  double pycopy = 0;

  px1 = boxScene.getLowerLeftX();
  py1 = boxScene.getLowerLeftY();
  px2 = boxScene.getUpperRightX();
  py2 = boxScene.getUpperRightY();

  if(py1 > py2 )
  {
    pycopy = py1;
    py1 = py2;
    py2 = pycopy;
  }

  te::gm::Envelope newBoxScene(px1, py1, px2, py2);

  int pxwidth = mm2pixel(newBoxScene.getWidth());
  int pxheight = mm2pixel(newBoxScene.getHeight());

  // Adjust internal renderer transformer
  m_transformer.setTransformationParameters(newBoxScene.getLowerLeftX(), newBoxScene.getLowerLeftY(), 
    newBoxScene.getUpperRightX(), newBoxScene.getUpperRightY(), pxwidth, pxheight);

  m_transformer.world2Device(box.getLowerLeftX(), box.getLowerLeftY(), px1, py1);
  m_transformer.world2Device(box.getUpperRightX(), box.getUpperRightY(), px2, py2);

  if(py1 > py2 )
  {
    pycopy = py1;
    py1 = py2;
    py2 = pycopy;
  }

  te::gm::Envelope boxViewport(px1, py1, px2, py2);
  return boxViewport;
}
