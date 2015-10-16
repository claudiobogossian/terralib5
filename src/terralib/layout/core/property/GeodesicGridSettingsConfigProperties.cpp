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
  \file GeodesicGridSettingsConfigProperties.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GeodesicGridSettingsConfigProperties.h"

te::layout::GeodesicGridSettingsConfigProperties::GeodesicGridSettingsConfigProperties() :
  m_pointTextSizeCorner("grid_pointTextSizeCorner_geodesic"),
  m_fontTextCorner("grid_fontTextCorner_geodesic"),
  m_textColorCorner("grid_textColorCorner_geodesic"),
  m_degreesText("grid_degreesText_geodesic"),
  m_minutesText("grid_minutesText_geodesic"),
  m_secondsText("grid_secondsText_geodesic"),
  m_lneCornerHrzDisplacement("grid_lneCornerHrzDisplacement_geodesic"),
  m_lneCornerVrtDisplacement("grid_lneCornerVrtDisplacement_geodesic"),
  m_lowerRightCornerText("grid_lowerRightCornerText_geodesic"),
  m_upperRightCornerText("grid_upperRightCornerText_geodesic"),
  m_lowerLeftCornerText("grid_lowerLeftCornerText_geodesic"),
  m_upperLeftCornerText("grid_upperLeftCornerText_geodesic"),
  m_visibleCornerTextsText("grid_visibleCornerTextsText_geodesic"),

  /* Geodesic: Topographic Map */

  m_defineScale("grid_defineScale_geodesic"),
  m_clip("grid_clip_geodesic"),
  m_scale("grid_scale_geodesic"),
  m_lneX1("grid_lneX1_geodesic"),
  m_lneX2("grid_lneX2_geodesic"),
  m_lneY1("grid_lneY1_geodesic"),
  m_lneY2("grid_lneY2_geodesic"),
  m_lneX3("grid_lneX3_geodesic"),
  m_lneX4("grid_lneX4_geodesic"),
  m_lneY3("grid_lneY3_geodesic"),
  m_lneY4("grid_lneY4_geodesic")
{
  
}

te::layout::GeodesicGridSettingsConfigProperties::~GeodesicGridSettingsConfigProperties( void )
{

}

std::string te::layout::GeodesicGridSettingsConfigProperties::getPointTextSizeCorner()
{
  return m_pointTextSizeCorner;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getFontTextCorner()
{
  return m_fontTextCorner;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getTextColorCorner()
{
  return m_textColorCorner;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getDegreesText()
{
  return m_degreesText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getMinutesText()
{
  return m_minutesText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getSecondsText()
{
  return m_secondsText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneCornerHrzDisplacement()
{
  return m_lneCornerHrzDisplacement;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneCornerVrtDisplacement()
{
  return m_lneCornerVrtDisplacement;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLowerRightCornerText()
{
  return m_lowerRightCornerText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getUpperRightCornerText()
{
  return m_upperRightCornerText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLowerLeftCornerText()
{
  return m_lowerLeftCornerText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getUpperLeftCornerText()
{
  return m_upperLeftCornerText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getVisibleCornerTextsText()
{
  return m_visibleCornerTextsText;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getDefineScale()
{
  return m_defineScale;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getClip()
{
  return m_clip;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getScale()
{
  return m_scale;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneX1()
{
  return m_lneX1;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneX2()
{
  return m_lneX2;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneY1()
{
  return m_lneY1;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneY2()
{
  return m_lneY2;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneX3()
{
  return m_lneX3;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneX4()
{
  return m_lneX4;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneY3()
{
  return m_lneY3;
}

std::string te::layout::GeodesicGridSettingsConfigProperties::getLneY4()
{
  return m_lneY4;
}
