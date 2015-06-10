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
  \file GeodesicGridSettingsConfigProperties.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GEODESIC_GRID_SETTINGS_CONFIG_PROPERTIES_H
#define __TERRALIB_LAYOUT_INTERNAL_GEODESIC_GRID_SETTINGS_CONFIG_PROPERTIES_H

// TerraLib
#include "GridSettingsConfigProperties.h"
#include "../Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class TELAYOUTEXPORT GeodesicGridSettingsConfigProperties: public GridSettingsConfigProperties
    {
      public:

        GeodesicGridSettingsConfigProperties();
        virtual ~GeodesicGridSettingsConfigProperties(void);
      
        std::string getPointTextSizeCorner();
        std::string getFontTextCorner();
        std::string getTextColorCorner();
        std::string getDegreesText();
        std::string getMinutesText();
        std::string getSecondsText();
        std::string getLneCornerHrzDisplacement();
        std::string getLneCornerVrtDisplacement();
        std::string getLowerRightCornerText();
        std::string getUpperRightCornerText();
        std::string getLowerLeftCornerText();
        std::string getUpperLeftCornerText();
        std::string getVisibleCornerTextsText();
        std::string getDefineScale();
        std::string getClip();
        std::string getScale();
        std::string getLneX1();
        std::string getLneX2();
        std::string getLneY1();
        std::string getLneY2();
        std::string getLneX3();
        std::string getLneX4();
        std::string getLneY3();
        std::string getLneY4();

      protected:

        /* Geodesic */

        std::string m_pointTextSizeCorner;
        std::string m_fontTextCorner;
        std::string m_textColorCorner;

        std::string m_degreesText;
        std::string m_minutesText;
        std::string m_secondsText;
        std::string m_lneCornerHrzDisplacement;
        std::string m_lneCornerVrtDisplacement;
        std::string m_lowerRightCornerText;
        std::string m_upperRightCornerText;
        std::string m_lowerLeftCornerText;
        std::string m_upperLeftCornerText;
        std::string m_visibleCornerTextsText;

        /* Geodesic: Topographic Map */

        std::string m_defineScale;
        std::string m_clip;
        std::string m_scale;
        std::string m_lneX1;
        std::string m_lneX2;
        std::string m_lneY1;
        std::string m_lneY2;
        std::string m_lneX3;
        std::string m_lneX4;
        std::string m_lneY3;
        std::string m_lneY4;
    };
  }
}

#endif