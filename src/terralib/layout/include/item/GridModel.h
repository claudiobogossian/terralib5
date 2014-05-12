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
  \file GridModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRID_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_GRID_MODEL_H

// TerraLib
#include "Properties.h"
#include "../../../maptools/Canvas.h"
#include "../../../maptools/Enums.h"

namespace te
{
  namespace layout
  {
    class GridModel 
    {
      public:

        GridModel();
        virtual ~GridModel();

        virtual void draw(te::map::Canvas* canvas, te::gm::Envelope box) = 0;

        virtual te::layout::Properties* getProperties() const = 0;
        virtual void updateProperties(te::layout::Properties* properties) = 0;

        virtual std::string getName();
        virtual void setName(std::string name);

        virtual bool isVisible();
        virtual void setVisible(bool visible);

        virtual void setMapScale(double scale);

       protected:

         virtual void drawVerticalLines(te::map::Canvas* canvas, te::gm::Envelope box) = 0;

         virtual void drawHorizontalLines(te::map::Canvas* canvas, te::gm::Envelope box) = 0;
         
         virtual void calculateGaps(te::gm::Envelope box) = 0;

       protected:

         te::layout::Properties* m_properties;
         double m_mapScale;
         std::string           m_name;

         /* Grid */
         bool   m_visible;
         double m_lneHrzGap;
         double m_lneVrtGap;
         double m_initialGridPointX; 
         double m_initialGridPointY;

         /* Just one is visible */
         LayoutGridStyle m_gridStyle;

         /* Line */
        te::map::LineDashStyle  m_lineStyle;
        te::color::RGBAColor    m_lineColor;
        int                     m_lineWidth;

        /* Text: Basic Configuration */
        int    m_pointTextSize;
        std::string m_fontText;
        te::color::RGBAColor  m_textColor;

        /* Text: Advanced configuration */
        bool   m_visibleAllTexts;
        bool m_superscriptText;
        double m_lneVrtDisplacement;
        double m_lneHrzDisplacement;
        bool   m_bottomText;
        bool   m_leftText;
        bool   m_rightText;
        bool   m_topText;
        bool   m_bottomRotateText;
        bool   m_leftRotateText;
        bool   m_rightRotateText;
        bool   m_topRotateText;       
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_GRID_MODEL_H