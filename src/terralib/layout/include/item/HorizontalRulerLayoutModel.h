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
  \file HorizontalRulerLayoutModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_LAYOUTMODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_LAYOUTMODEL_H

// TerraLib
#include "AbstractRulerLayoutModel.h"
#include "ContextLayoutItem.h"
#include "LayoutUtils.h"
#include "LayoutMode.h"
#include "../../../qt/widgets/canvas/Canvas.h"
#include "../../../geometry/Envelope.h"

// STL
#include <vector>
#include <string>

namespace te
{
  namespace layout
  {
    class HorizontalRulerLayoutModel : public AbstractRulerLayoutModel
    {
      public:

        HorizontalRulerLayoutModel();
        ~HorizontalRulerLayoutModel();

        virtual void updateObserver(ContextLayoutItem context);

        virtual void draw( ContextLayoutItem context );
        
        virtual void setPaperBox(te::gm::Envelope	 box);
        virtual te::gm::Envelope	 getPaperBox();

        virtual void setBox(te::gm::Envelope box);
                
        virtual void setVisibleHorizontalRuler(bool visible);
        virtual bool isVisibleHorizontalRuler();

        virtual void invertedLines(bool invert);
        
      protected:

        virtual void drawHorizontalRuler(te::map::Canvas* canvas, LayoutUtils* utils);
        virtual void drawRuler(te::map::Canvas* canvas, LayoutUtils* utils);
        virtual void drawMarks(te::map::Canvas* canvas, LayoutUtils* utils, int marks);

        virtual void updateHorizontalListText();

      protected:

        te::gm::Envelope	 m_paperBox;
        te::gm::Envelope	 m_backEndBox;
        double	  m_longLine;
        double	  m_mediumLine;
        double    m_smallLine;
        double    m_spacing;
        double	  m_lineMargin;
        int		  m_horizontalBlockMarks;
        int		  m_blockSize;
        int		  m_middleBlockSize;
        double	  m_borderLineSize;
        bool	  m_visibleHorizontalRuler;
        double	  m_posCount;
        std::vector<std::string> m_horizontalTexts;
        bool      m_invertedLines;
        double m_backEndSpacing;
        double m_backEndMargin;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_LAYOUTMODEL_H