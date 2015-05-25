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
  \file GridPlanarModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridPlanarModel.h"
#include "../core/ContextItem.h"
#include "../core/pattern/singleton/Context.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Coord2D.h"
#include "../core/property/Property.h"
#include "../core/property/PlanarGridSettingsConfigProperties.h"
#include "../core/enum/Enums.h"

// STL
#include <string>
#include <sstream> 

te::layout::GridPlanarModel::GridPlanarModel() :
  GridMapModel(new PlanarGridSettingsConfigProperties),
  m_unit(StyleKilometer)
{
  m_type = Enums::getInstance().getEnumObjectType()->getGridPlanarItem();

  m_lneVrtDisplacement = 2;
  m_lneHrzDisplacement = 2;
}

te::layout::GridPlanarModel::~GridPlanarModel()
{
  if(m_settingsConfig)
  {
    delete m_settingsConfig;
    m_settingsConfig = 0;
  }
}

void te::layout::GridPlanarModel::calculateGaps( te::gm::Envelope box )
{
  if(m_lneHrzGap <= 0)
  {
    m_lneHrzGap = m_mapScale * 0.05;
  }

  if(m_lneVrtGap <= 0)
  {
    m_lneVrtGap = m_lneHrzGap;
  }

  if(m_initialGridPointX <= 0)
  {
    m_initialGridPointX = box.getLowerLeftX();
  }
  if(m_initialGridPointY <= 0)
  {
    m_initialGridPointY = box.getLowerLeftY();
  }
}

te::layout::LayoutUnit te::layout::GridPlanarModel::getUnit()
{
  return m_unit;
}

void te::layout::GridPlanarModel::draw( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box, int srid )
{

}

void te::layout::GridPlanarModel::drawVerticalLines( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box )
{

}

void te::layout::GridPlanarModel::drawHorizontalLines( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box )
{

}



