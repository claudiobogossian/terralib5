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
  \file GridGeodesicModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridGeodesicModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Coord2D.h"
#include "../core/property/Property.h"
#include "../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../../srs/Config.h"
#include "../core/WorldTransformer.h"
#include "../core/enum/Enums.h"

// STL
#include <string>
#include <sstream> 
#include <cmath>

te::layout::GridGeodesicModel::GridGeodesicModel() :
  GridMapModel(new GeodesicGridSettingsConfigProperties),
  m_pointTextSizeCorner(12),
  m_degreesText(true),
  m_minutesText(false),
  m_secondsText(false),
  m_lneCornerHrzDisplacement(10),
  m_lneCornerVrtDisplacement(10),
  m_lowerRightCornerText(false),
  m_upperRightCornerText(false),
  m_lowerLeftCornerText(false),
  m_upperLeftCornerText(false),
  m_visibleCornerTextsText(false),

  /* Geodesic: Topographic Map */

  m_defineScale(false),
  m_clip(false),
  m_scale(0.),
  m_lneX1(0.),
  m_lneX2(0.),
  m_lneY1(0.),
  m_lneY2(0.),
  m_lneX3(0.),
  m_lneX4(0.),
  m_lneY3(0.),
  m_lneY4(0.)
{
  m_type = Enums::getInstance().getEnumObjectType()->getGridGeodesicItem();

  m_lneVrtDisplacement = 2;
  m_lneHrzDisplacement = 2;
}

te::layout::GridGeodesicModel::~GridGeodesicModel()
{
  if(m_settingsConfig)
  {
    delete m_settingsConfig;
    m_settingsConfig = 0;
  }
}

void te::layout::GridGeodesicModel::draw( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box, int srid )
{
  if((!canvas) || (!utils))
    return;

  if(!box.isValid())
    return;

  m_srid = srid;

  calculateGaps(box);

  if(!m_visible)
    return;

  if(srid <= 0)
    return;
  
  te::color::RGBAColor color = te::color::RGBAColor(0, 0, 0, 255);
  canvas->setLineColor(color);

  canvas->setTextPointSize(m_pointTextSize);
  canvas->setFontFamily(m_fontText);

  gridTextFreeMemory();

  drawVerticalLines(canvas, utils, box);
  drawHorizontalLines(canvas, utils, box);
}

void te::layout::GridGeodesicModel::drawVerticalLines(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box)
{
  // Draw a horizontal line and the y coordinate change(vertical)
  
  double			y1;
  double			yInit;

  WorldTransformer transf = utils->getTransformGeo(m_planarBox, m_boxMapMM);
  transf.setMirroring(false);

  int zone = utils->calculatePlanarZone(box);
  
  yInit = m_initialGridPointY;
  if(yInit < box.getLowerLeftY())
  {
    double dify = box.getLowerLeftY() - yInit;
    int nParts = (int)(dify/m_lneVrtGap);
    if(nParts == 0)
    {
      yInit = m_initialGridPointY;
    }
    else
    {
      yInit = yInit + (nParts * m_lneVrtGap);
    }
  }

  y1 = yInit;
  for( ; y1 < box.getUpperRightY() ; y1 += m_lneVrtGap)
  {
    if(y1 < box.getLowerLeftY())
      continue;

    te::gm::Envelope env(box.getLowerLeftX(), y1, box.getUpperRightX(), y1);
    
    te::gm::LinearRing* line = 0;
    line = utils->addCoordsInX(env, y1, m_lneVrtGap);

    // Curvatura da linha: de latlong para planar;
    // Desenhar linha: de planar para milimetro

    utils->remapToPlanar(line, zone);
    utils->convertToMillimeter(transf, line);

    utils->drawLineW(line);

    std::string text = utils->convertDecimalToDegree(y1, m_degreesText, m_minutesText, m_secondsText);
    
    te::gm::Envelope* ev = const_cast<te::gm::Envelope*>(line->getMBR());
    
    if(m_visibleAllTexts)
    {
      if(m_leftText)
      {
        canvas->drawText(ev->getLowerLeftX() - m_lneHrzDisplacement, ev->getLowerLeftY(), text, 0);
        te::gm::Point* coordLeft = new te::gm::Point(ev->getLowerLeftX() - m_lneHrzDisplacement, ev->getLowerLeftY());
        m_gridTexts[coordLeft] = text;
      }

      if(m_rightText)
      {
        canvas->drawText(ev->getUpperRightX() + m_lneHrzDisplacement, ev->getUpperRightY(), text, 0);
        te::gm::Point* coordRight = new te::gm::Point(ev->getUpperRightX() + m_lneHrzDisplacement, ev->getUpperRightY());
        m_gridTexts[coordRight] = text;
      }
    }
    
    if(line)
    {
      delete line;
      line = 0;
    }
  }
}

void te::layout::GridGeodesicModel::drawHorizontalLines(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box)
{
  // Draw a vertical line and the x coordinate change(horizontal)
  
  double			x1;
  double			xInit;

  WorldTransformer transf = utils->getTransformGeo(m_planarBox, m_boxMapMM);
  transf.setMirroring(false);

  int zone = utils->calculatePlanarZone(box);

  xInit = m_initialGridPointX;
  if(xInit < box.getLowerLeftX())
  {
    double difx = box.getLowerLeftX() - xInit;
    int nParts = (int)(difx/m_lneHrzGap);
    if(nParts == 0)
    {
      xInit = m_initialGridPointX;
    }
    else
    {
      xInit = xInit + (nParts * m_lneHrzGap);
    }
  }

  x1 = xInit;

  for( ; x1 < box.getUpperRightX() ; x1 += m_lneHrzGap)
  {
    if(x1 < box.getLowerLeftX())
      continue;

    te::gm::Envelope env(x1, box.getLowerLeftY(), x1, box.getUpperRightY());
    te::gm::LinearRing* line = 0;
    line = utils->addCoordsInY(env, x1, m_lneHrzGap);

    // Curvatura da linha: de latlong para planar;
    // Desenhar linha: de planar para milimetro
    utils->remapToPlanar(line, zone);
    utils->convertToMillimeter(transf, line);

    utils->drawLineW(line);

    std::string text = utils->convertDecimalToDegree(x1, m_degreesText, m_minutesText, m_secondsText);

    te::gm::Envelope* ev = const_cast<te::gm::Envelope*>(line->getMBR());

    if(m_visibleAllTexts)
    {
      if(m_bottomText)
      {
        canvas->drawText(ev->getLowerLeftX(), ev->getLowerLeftX() - m_lneVrtDisplacement, text, 0);
        te::gm::Point* coordRight = new te::gm::Point(ev->getLowerLeftX(), ev->getLowerLeftX() - m_lneVrtDisplacement);
        m_gridTexts[coordRight] = text;
      }

      if(m_topText)
      {
        canvas->drawText(ev->getUpperRightX(), ev->getUpperRightY() + m_lneVrtDisplacement, text, 0);
        te::gm::Point* coordRight = new te::gm::Point(ev->getUpperRightX(), ev->getUpperRightY() + m_lneVrtDisplacement);
        m_gridTexts[coordRight] = text;
      }
    }

    if(line)
    {
      delete line;
      line = 0;
    }
  }
}

void te::layout::GridGeodesicModel::calculateGaps( te::gm::Envelope box )
{
  te::gm::Coord2D init = box.getLowerLeft();
  te::gm::Coord2D end = box.getUpperRight();

  if(m_initialGridPointX <= 0)
  {
    m_initialGridPointX = init.x;
  }
  if(m_initialGridPointY <= 0)
  {
    m_initialGridPointY = init.y;
  }

  if(m_srid != TE_UNKNOWN_SRS)
  {
    if(m_lneHrzGap <= 0)
    {
      m_lneHrzGap = std::fabs(init.x - end.x);
      m_lneHrzGap /= 4;
    }
    if(m_lneVrtGap <= 0)
      m_lneVrtGap = m_lneHrzGap;
  }
  else
  {
    m_lneVrtGap = m_lneHrzGap = 0.001;
  }
}

void te::layout::GridGeodesicModel::setPlanarBox( te::gm::Envelope box )
{
  m_planarBox = box;
}

void te::layout::GridGeodesicModel::setVisibleAllTexts( bool visible )
{
  setVisibleCornerTextsText(visible);
}

void te::layout::GridGeodesicModel::setVisibleCornerTextsText( bool visible )
{
  m_visibleCornerTextsText = visible;

  m_lowerRightCornerText = visible;
  m_upperRightCornerText = visible;
  m_lowerLeftCornerText = visible;
  m_upperLeftCornerText = visible;
}

bool te::layout::GridGeodesicModel::isVisibleCornerTextsText()
{
  return m_visibleCornerTextsText;
}

te::gm::Envelope te::layout::GridGeodesicModel::getPlanarBox()
{
  return m_planarBox;
}

bool te::layout::GridGeodesicModel::isDegreesText()
{
  return m_degreesText;
}

bool te::layout::GridGeodesicModel::isMinutesText()
{
  return m_minutesText;
}

bool te::layout::GridGeodesicModel::isSecondsText()
{
  return m_secondsText;
}

void te::layout::GridGeodesicModel::setSRID( int srid )
{
  m_srid = srid;
}

