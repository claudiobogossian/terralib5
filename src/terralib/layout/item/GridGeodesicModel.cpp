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
  
}

void te::layout::GridGeodesicModel::drawVerticalLines(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box)
{

}

void te::layout::GridGeodesicModel::drawHorizontalLines(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box)
{

}

void te::layout::GridGeodesicModel::calculateGaps( te::gm::Envelope box )
{
  te::gm::Coord2D init = box.getLowerLeft();
  te::gm::Coord2D end = box.getUpperRight();

  if(m_initialGridPointX == 0)
  {
    m_initialGridPointX = init.x;
  }
  if(m_initialGridPointY == 0)
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

