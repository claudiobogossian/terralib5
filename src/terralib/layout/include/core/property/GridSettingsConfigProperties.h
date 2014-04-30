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
  \file GridSettingsConfigProperties.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_CONFIG_PROPERTIES_H
#define __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_CONFIG_PROPERTIES_H

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class GridSettingsConfigProperties
    {
      public:

        virtual ~GridSettingsConfigProperties(void){};
        
        std::string getName();
        std::string getVisible();
        std::string getLneHrzGap();
        std::string getLneVrtGap();
        std::string getInitialGridPointX();
        std::string getInitialGridPointY();
        std::string getStyle();
        std::string getLineStyle();
        std::string getLineColor();
        std::string getLineWidth();
        std::string getPointTextSize();
        std::string getFontText();
        std::string getTextColor();
        std::string getVisibleAllTexts();
        std::string getSuperscriptText();
        std::string getLneVrtDisplacement();
        std::string getLneHrzDisplacement();
        std::string getBottomText();
        std::string getLeftText();
        std::string getRightText();
        std::string getTopText();
        std::string getBottomRotateText();
        std::string getLeftRotateText();
        std::string getRightRotateText();
        std::string getTopRotateText();
              
      protected:

        std::string m_name;

        /* Grid */

        std::string m_visible;
        std::string m_lneHrzGap;
        std::string m_lneVrtGap;
        std::string m_initialGridPointX; 
        std::string m_initialGridPointY; 

        /* Just one is visible */
        std::string m_style;

        /* Line */
        std::string m_lineStyle;
        std::string m_lineColor;
        std::string m_lineWidth;

        /*Text: Basic Configuration*/
        std::string m_pointTextSize;
        std::string m_fontText;
        std::string m_textColor;

        /*Text: Advanced configuration*/
        std::string m_visibleAllTexts;
        std::string m_superscriptText;
        std::string m_lneVrtDisplacement;
        std::string m_lneHrzDisplacement;
        std::string m_bottomText;
        std::string m_leftText;
        std::string m_rightText;
        std::string m_topText;
        std::string m_bottomRotateText;
        std::string m_leftRotateText;
        std::string m_rightRotateText;
        std::string m_topRotateText;   
    };
  }
}

#endif