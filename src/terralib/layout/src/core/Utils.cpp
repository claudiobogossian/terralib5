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

  if(!canvas)
  {
    return;
  }

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

  if(!canvas)
  {
    return;
  }
    
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

te::color::RGBAColor** te::layout::Utils::getImageW( te::gm::Envelope boxmm )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = Context::getInstance()->getCanvas();

  if(!canvas)
  {
    return pixmap;
  }

  te::gm::Envelope boxViewport = viewportBox(boxmm);

  if(boxViewport.isValid())
  {
    pixmap = canvas->getImage(0, 0, boxViewport.getWidth(), boxViewport.getHeight());
  }
  return pixmap;
}

int te::layout::Utils::mm2pixel( double mm )
{
  int devDpi = Context::getInstance()->getDpiX();
  int px = (mm * devDpi) / 25.4 ;
  return px;
}

void te::layout::Utils::configCanvas( te::gm::Envelope box, bool resize )
{
  te::gm::Envelope boxViewport = viewportBox(box);
  changeCanvas(boxViewport, box, resize); 
}

void te::layout::Utils::configGeoCanvas( te::gm::Envelope boxgeo, te::gm::Envelope boxmm, bool resize )
{
  te::gm::Envelope boxViewport = viewportBox(boxmm);
  changeCanvas(boxViewport, boxgeo, resize);
}

void te::layout::Utils::configGeoCanvasFromGeo( te::gm::Envelope boxgeo, te::gm::Envelope boxmm, bool resize )
{
  te::gm::Envelope boxViewport = viewportBoxFromGeo(boxgeo, boxmm);
  changeCanvas(boxViewport, boxgeo, resize);
}

void te::layout::Utils::changeCanvas( te::gm::Envelope viewport, te::gm::Envelope world, bool resize /*= true*/ )
{
  te::map::Canvas* canvas = Context::getInstance()->getCanvas();

  if(!canvas)
  {
    return;
  }

  if(resize)
  {
    //Transparent
    te::color::RGBAColor color(255,255,255, 0);
    canvas->setBackgroundColor(color);

    canvas->resize(viewport.getWidth(), viewport.getHeight());
  }
  
  canvas->setWindow(world.getLowerLeftX(), world.getLowerLeftY(), 
    world.getUpperRightX(), world.getUpperRightY()); 
}

te::gm::Envelope te::layout::Utils::viewportBox( te::gm::Envelope box )
{
  te::gm::Envelope boxViewport;
  
  if(!box.isValid())
    return boxViewport;

  boxViewport = viewportBoxFromMM(box);
  return boxViewport;
}

te::gm::Envelope te::layout::Utils::viewportBoxFromMM( te::gm::Envelope box )
{
  te::map::WorldDeviceTransformer transf; // World Device Transformer.

  int pxwidth = mm2pixel(box.getWidth());
  int pxheight = mm2pixel(box.getHeight());

  // Adjust internal renderer transformer
  transf.setTransformationParameters(box.getLowerLeftX(), box.getLowerLeftY(), 
    box.getUpperRightX(), box.getUpperRightY(), pxwidth, pxheight);

  te::gm::Envelope boxViewport = transformToViewport(transf, box);
  return boxViewport;
}

te::gm::Envelope te::layout::Utils::viewportBoxFromGeo( te::gm::Envelope boxgeo, te::gm::Envelope boxmm )
{
  te::gm::Envelope boxViewport;

  if(!boxgeo.isValid())
    return boxViewport;

  if(!boxmm.isValid())
    return boxViewport;

  te::map::WorldDeviceTransformer transf; // World Device Transformer.

  int pxwidth = mm2pixel(boxmm.getWidth());
  int pxheight = mm2pixel(boxmm.getHeight());

  // Adjust internal renderer transformer
  transf.setTransformationParameters(boxgeo.getLowerLeftX(), boxgeo.getLowerLeftY(), 
    boxgeo.getUpperRightX(), boxgeo.getUpperRightY(), pxwidth, pxheight);

  boxViewport = transformToViewport(transf, boxgeo);
  return boxViewport;
}

te::gm::Envelope te::layout::Utils::transformToViewport( te::map::WorldDeviceTransformer transf, te::gm::Envelope box )
{
  double px1 = 0;
  double py1 = 0;
  double px2 = 0;
  double py2 = 0;
  double pycopy = 0;
  transf.world2Device(box.getLowerLeftX(), box.getLowerLeftY(), px1, py1);
  transf.world2Device(box.getUpperRightX(), box.getUpperRightY(), px2, py2);

  if(py1 > py2 )
  {
    pycopy = py1;
    py1 = py2;
    py2 = pycopy;
  }

  te::gm::Envelope boxViewport(px1, py1, px2, py2);
  return boxViewport;
}

te::gm::LinearRing* te::layout::Utils::addCoordsInX( te::gm::Envelope box, double gap )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(te::gm::LineStringType);
  std::map<int, te::gm::Point> coords;

  int count = 1;
  for(double sub_x = box.getLowerLeftX(); sub_x < box.getUpperRightX(); sub_x +=(gap / 4.))
  {
    coords[count] = te::gm::Point(sub_x, box.getLowerLeftY());
    count+=1;
  }

  line->setNumCoordinates(count + 1);
  line->setPointN(0, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));

  for(int i = 1 ; i < count ; ++i)
  {
    line->setPointN(i, coords[i]);
  }

  line->setPointN(count, te::gm::Point(box.getUpperRightX(), box.getLowerLeftY()));

  return line;
}

te::gm::LinearRing* te::layout::Utils::addCoordsInY( te::gm::Envelope box, double gap )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(te::gm::LineStringType);
  std::map<int, te::gm::Point> coords;

  int count = 1;
  for(double sub_y = box.getLowerLeftY(); sub_y < box.getUpperRightY(); sub_y +=(gap / 4.))
  {
    coords[count] = te::gm::Point(box.getLowerLeftX(), sub_y);
    count+=1;
  }

  line->setNumCoordinates(count + 1);
  line->setPointN(0, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));

  for(int i = 1 ; i < count ; ++i)
  {
    line->setPointN(i, coords[i]);
  }

  line->setPointN(count, te::gm::Point(box.getUpperRightX(), box.getLowerLeftY()));

  return line;
}

void te::layout::Utils::textBoundingBox( double &w, double &h, std::string txt )
{
  te::map::Canvas* canvas = Context::getInstance()->getCanvas();

  if(!canvas)
  {
    return;
  }

  w = 0;
  h = 0;

  te::gm::Polygon* poly = canvas->getTextBoundary(0, 0, txt, 0);
  if(poly)
  {
    //Box = mbr: minimum bounding rectangle
    const te::gm::Envelope* env = poly->getMBR();
    te::gm::Envelope* box = 0;
    box = const_cast<te::gm::Envelope*>(env);
    if(box)
    {
      w = box->getWidth();
      h = box->getHeight();
    }
  }
}
