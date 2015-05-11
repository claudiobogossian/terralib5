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
  \file PlanarGridSettingsConfigProperties.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PlanarGridSettingsConfigProperties.h"

te::layout::PlanarGridSettingsConfigProperties::PlanarGridSettingsConfigProperties() :
  m_unit("grid_unitPlanar_planar")
{
  m_name = "grid_name_planar";
  m_visible = "grid_visible_planar";
  m_lneHrzGap = "grid_lneHrzGap_planar";
  m_lneVrtGap = "grid_lneVrtGap_planar";
  m_initialGridPointX = "grid_initialGridPointX_planar";
  m_initialGridPointY = "grid_initialGridPointY_planar";
  m_style = "grid_style_planar";
  m_lineStyle = "grid_lineStyle_planar";
  m_lineColor = "grid_lineColor_planar";
  m_lineWidth = "grid_lineWidth_planar";
  m_pointTextSize = "grid_pointTextSize_planar";
  m_fontText = "grid_fontText_planar";
  m_textColor = "grid_textColor_planar";
  m_visibleAllTexts = "grid_visibleAllTexts_planar";
  m_superscriptText = "grid_superscriptText_planar";
  m_lneVrtDisplacement = "grid_lneVrtDisplacement_planar";
  m_lneHrzDisplacement = "grid_lneHrzDisplacement_planar";
  m_bottomText = "grid_bottomText_planar";
  m_leftText = "grid_leftText_planar";
  m_rightText = "grid_rightText_planar";
  m_topText = "grid_topText_planar";
  m_bottomRotateText = "grid_bottomRotateText_planar";
  m_leftRotateText = "grid_leftRotateText_planar";
  m_rightRotateText = "grid_rightRotateText_planar";
  m_topRotateText = "grid_topRotateText_planar";
}

te::layout::PlanarGridSettingsConfigProperties::~PlanarGridSettingsConfigProperties( void )
{

}

std::string te::layout::PlanarGridSettingsConfigProperties::getUnit()
{
  return m_unit;
}
