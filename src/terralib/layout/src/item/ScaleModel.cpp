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
  \file ScaleModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ScaleModel.h"
#include "Context.h"
#include "MapModel.h"
#include "Property.h"
#include "Properties.h"
#include "SharedProperties.h"
#include "../../../geometry/Polygon.h"

// STL
#include <string>
#include <sstream> // std::stringstream


te::layout::ScaleModel::ScaleModel() :
  m_mapName(""),
  m_mapScale(0),
  m_scaleGapX(20),
  m_scaleGapY(5)
{
  m_box = te::gm::Envelope(0., 0., 70., 30.);
}

te::layout::ScaleModel::~ScaleModel()
{

}

void te::layout::ScaleModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = Context::getInstance()->getCanvas();
  Utils* utils = Context::getInstance()->getUtils();
  
  if(context.isResizeCanvas())
    utils->configCanvas(m_box);

  drawScale(canvas, utils, m_box);

  canvas->setPolygonContourWidth(2);
  canvas->setPolygonContourColor(te::color::RGBAColor(0, 0, 0, 255));
  canvas->setPolygonFillColor(m_backgroundColor);

  utils->drawRectW(m_box);

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);

  ContextItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

void te::layout::ScaleModel::drawScale( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box )
{
  double			unit=1000.0;
  std::string strUnit="(Km)";

  if(m_mapScale < 1000)
  {
    unit = 1.0;
    strUnit="(m)";
  }
  else 
  {
    unit = 1000.0;
  }
  
  //convert millimeters to centimeters
  double mmToCm = m_scaleGapX/10;

  double spacing = m_mapScale/100;
  
  double value = 0.;
  double width = 0.;
  double x1 = box.getLowerLeftX();
  te::color::RGBAColor black(0, 0, 0, 255);
  te::color::RGBAColor white(255, 255, 255, 255);
  te::color::RGBAColor firtRect = black;
  te::color::RGBAColor secondRect = white;
  te::color::RGBAColor changeColor;
  te::gm::Envelope newBoxFirst;
  te::gm::Envelope newBoxSecond;
  canvas->setPolygonContourWidth(1);
  canvas->setPolygonContourColor(te::color::RGBAColor(0, 0, 0, 255));
  canvas->setTextColor(te::color::RGBAColor(0, 0, 0, 255));
  for( ; x1 < box.getUpperRightX() ; x1 += width)
  {
    //Up rect
    canvas->setPolygonFillColor(firtRect);
    newBoxFirst = te::gm::Envelope(x1, box.getUpperRightY(), x1 + m_scaleGapX, box.getUpperRightY() - m_scaleGapY);
    utils->drawRectW(newBoxFirst);

    //Down rect
    canvas->setPolygonFillColor(secondRect);
    newBoxSecond = te::gm::Envelope(x1, box.getUpperRightY() - (m_scaleGapY*2), x1 + m_scaleGapX, box.getUpperRightY() - m_scaleGapY);
    utils->drawRectW(newBoxSecond);

    if(width == 0)
      width = m_scaleGapX;
    else
      value += (spacing * mmToCm)/unit;

    std::stringstream ss_value;
    ss_value << value;

    std::string s_value = ss_value.str();
    canvas->drawText(x1, newBoxSecond.getLowerLeftY() - 5, s_value, 0);

    changeColor = firtRect;
    firtRect = secondRect;
    secondRect = changeColor;
  }

  //middle
  // Canvas - Necessário saber o tamanho do box do texto em mm: pendente;
  double centerX = m_box.getCenter().x;
  double centerY = m_box.getCenter().y;

  canvas->drawText(centerX, newBoxSecond.getLowerLeftY() - 15, strUnit, 0);

}

te::layout::Properties* te::layout::ScaleModel::getProperties() const
{
  ItemModelObservable::getProperties();

  Property pro_widthRectGap;
  pro_widthRectGap.setName("scale_width_rect_gap");
  pro_widthRectGap.setId("");
  pro_widthRectGap.setValue(m_scaleGapX, DataTypeDouble);
  m_properties->addProperty(pro_widthRectGap);

  Property pro_heightRectGap;
  pro_heightRectGap.setName("scale_height_rect_gap");
  pro_heightRectGap.setId("");
  pro_heightRectGap.setValue(m_scaleGapY, DataTypeDouble);
  m_properties->addProperty(pro_heightRectGap);

  Property pro_mapName;
  pro_mapName.setName(m_sharedProps->getMapName());
  pro_mapName.setId("");
  pro_mapName.setValue(m_mapName, DataTypeStringList);
  Variant v;
  v.setValue(m_mapName, DataTypeString);
  pro_mapName.addOption(v);
  m_properties->addProperty(pro_mapName);

  return m_properties;
}

void te::layout::ScaleModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_mapName = vectorProps->contains(m_sharedProps->getMapName());

  if(!pro_mapName.isNull())
  {
    m_mapName = pro_mapName.getOptionByCurrentChoice().toString();
  }

  Property pro_widthRectGap = vectorProps->contains("scale_width_rect_gap");

  if(!pro_widthRectGap.isNull())
  {
    m_scaleGapX = pro_widthRectGap.getValue().toDouble();
  }

  Property pro_heightRectGap = vectorProps->contains("scale_height_rect_gap");

  if(!pro_heightRectGap.isNull())
  {
    m_scaleGapY = pro_heightRectGap.getValue().toDouble();
  }
}

void te::layout::ScaleModel::visitDependent()
{
  MapModel* map = dynamic_cast<MapModel*>(m_visitable);

  if(map)
  {
    m_mapScale = map->getScale();

    ContextItem contx;

    draw(contx);

    ContextItem contextNotify;
    contextNotify.setWait(true);
    notifyAll(contextNotify);
  }	
}

void te::layout::ScaleModel::setScaleGapX( double x )
{
  m_scaleGapX = x;
}

double te::layout::ScaleModel::getScaleGapX()
{
  return m_scaleGapX;
}

void te::layout::ScaleModel::setScaleGapY( double y )
{
  m_scaleGapY = y;
}

double te::layout::ScaleModel::getScaleGapY()
{
  return m_scaleGapY;
}
