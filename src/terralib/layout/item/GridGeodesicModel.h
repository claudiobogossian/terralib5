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
  \file GridGeodesicModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRID_GEODESIC_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_GRID_GEODESIC_MODEL_H

// TerraLib
#include "GridModel.h"
#include "../core/Config.h"
#include "GridMapModel.h"

namespace te
{
  namespace layout
  {
    class TELAYOUTEXPORT GridGeodesicModel: public GridMapModel 
    {
      public:

        GridGeodesicModel();

        virtual ~GridGeodesicModel();

        virtual void draw(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box, int srid);
        
        virtual void setPlanarBox(te::gm::Envelope box);
        
        virtual void setVisibleAllTexts(bool visible);

        virtual void setVisibleCornerTextsText(bool visible);

        virtual bool isVisibleCornerTextsText();

       protected:

         virtual void drawVerticalLines(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box);

         virtual void drawHorizontalLines(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box);  

         virtual void calculateGaps(te::gm::Envelope box);

       protected:
         
         te::gm::Envelope m_planarBox;

         /*Text: Basic Configuration*/
         int    m_pointTextSizeCorner;
         std::string m_fontTextCorner;
         te::color::RGBAColor  m_textColorCorner;

         bool m_degreesText;
         bool m_minutesText;
         bool m_secondsText;
         double m_lneCornerHrzDisplacement;
         double m_lneCornerVrtDisplacement;
         bool m_lowerRightCornerText;
         bool m_upperRightCornerText;
         bool m_lowerLeftCornerText;
         bool m_upperLeftCornerText;
         bool m_visibleCornerTextsText;

         /* Geodesic: Topographic Map */

         bool m_defineScale;
         bool m_clip;
         double m_scale;
         double m_lneX1;
         double m_lneX2;
         double m_lneY1;
         double m_lneY2;
         double m_lneX3;
         double m_lneX4;
         double m_lneY3;
         double m_lneY4;
};
  }
}

#endif 

