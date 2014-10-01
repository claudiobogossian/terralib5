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
  \file AbstractRulerModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACTRULER_MODEL_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACTRULER_MODEL_H

// TerraLib
#include "ItemModelObservable.h"
#include "Utils.h"
#include "../../../qt/widgets/canvas/Canvas.h"
#include "../../../color/RGBAColor.h"

namespace te
{
  namespace layout
  {
    class AbstractRulerModel : public ItemModelObservable
    {
      public:

        AbstractRulerModel();
        virtual ~AbstractRulerModel(void);

        virtual void setPaperBox(te::gm::Envelope	 box);
        virtual te::gm::Envelope	 getPaperBox();

        virtual void setVisible(bool visible);
        virtual bool isVisible();

      protected:

        virtual void drawRuler(te::map::Canvas* canvas, Utils* utils) = 0;

        virtual void drawLineW(te::gm::Envelope box, Utils* utils);

        virtual void drawRectW(te::gm::Envelope box, te::color::RGBAColor color, te::map::Canvas* canvas, Utils* utils);

      protected:

        bool m_visible;

        te::gm::Envelope	 m_paperBox;
        te::gm::Envelope	 m_backEndBox;
        double	  m_longLine;
        double	  m_mediumLine;
        double    m_smallLine;
        double    m_spacing;
        double	  m_lineMargin;
        int		  m_blockSize;
        int		  m_middleBlockSize;
        int     m_smallBlockSize;
        double	  m_borderLineSize;
        double m_backEndSpacing;
        double m_backEndMargin;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_LAYOUTMODEL_H