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
  \file GridModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridModel.h"
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Coord2D.h"
#include "../core/property/Property.h"
#include "../../srs/Config.h"
#include "../core/enum/Enums.h"

te::layout::GridModel::GridModel() :
  m_name("GRID_MODEL"),
  m_visible(false),
  m_lneHrzGap(0),
  m_lneVrtGap(0),
  m_initialGridPointX(0),
  m_initialGridPointY(0),
  m_gridStyle(0),
  m_lineStyle(0),
  m_lineWidth(1),
  m_pointTextSize(12),
  m_fontText("Arial"),
  m_textColor(),
  m_visibleAllTexts(true),
  m_superscriptText(false),
  m_lneVrtDisplacement(7),
  m_lneHrzDisplacement(7),
  m_bottomText(true),
  m_leftText(true),
  m_rightText(true),
  m_topText(true),
  m_bottomRotateText(false),
  m_leftRotateText(false),
  m_rightRotateText(false),
  m_topRotateText(false),
  m_srid(TE_UNKNOWN_SRS)
{
  m_gridStyle = Enums::getInstance().getEnumGridStyleType()->getStyleNone();
  m_lineStyle = Enums::getInstance().getEnumLineStyleType()->getStyleNone();
}

te::layout::GridModel::~GridModel()
{

}

std::string te::layout::GridModel::getName()
{
  return m_name;
}

void te::layout::GridModel::setName( std::string name )
{
  m_name = name;
}

bool te::layout::GridModel::isVisible()
{
  return m_visible;
}

void te::layout::GridModel::setVisible( bool visible )
{
  m_visible = visible;
}

void te::layout::GridModel::setMapScale( double scale )
{
  m_mapScale = scale;
}

void te::layout::GridModel::setWorldBox( te::gm::Envelope box )
{
  m_worldBox = box;
}

void te::layout::GridModel::setBoxMapMM( te::gm::Envelope box )
{
  m_boxMapMM = box;
}

void te::layout::GridModel::setBoundingBoxItemMM( te::gm::Envelope box )
{
  m_boundingBoxItemMM = box;
}

void te::layout::GridModel::setSystematic( Systematic* sys )
{
  m_systematic = sys;
}

void te::layout::GridModel::setVisibleAllTexts( bool visible )
{
  m_visibleAllTexts = visible;

  m_bottomText = visible;
  m_leftText = visible;
  m_rightText = visible;
  m_topText = visible;
}

bool te::layout::GridModel::isVisibleAllTexts()
{
  return m_visibleAllTexts;
}

void te::layout::GridModel::gridTextFreeMemory()
{
  std::map<te::gm::Point*, std::string>::iterator it;
  for(it = m_gridTexts.begin() ; it != m_gridTexts.end() ; ++it)
  {
    te::gm::Point* p = it->first;
    if(p)
    {
      delete p;
      p = 0;
    }
  }
  m_gridTexts.clear();
}

std::map<te::gm::Point*, std::string> te::layout::GridModel::getGridInfo()
{
  return m_gridTexts;
}
