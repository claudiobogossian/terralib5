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
  \file AbstractRuler.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_RULER_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_RULER_H

// Qt
#include <QGraphicsView>
#include <QPainter>

namespace te
{
  namespace layout
  {
    class PaperConfig;

    class AbstractRuler
    {
      public:

        AbstractRuler(PaperConfig* paperConfig);

        virtual ~AbstractRuler(void);
        
        virtual void setVisible(bool visible);
        
        virtual bool isVisible();

        virtual double getLongLine();

        virtual void setLongLine(double size);

        virtual double getMediumLine();

        virtual void setMediumLine(double size);

        virtual double getSmallLine();

        virtual void setSmallLine(double size);

        virtual double getBlockSize();

        virtual void setBlockSize(double size);

        virtual double getMiddleBlockSize();

        virtual void setMiddleBlockSize(double size);

        virtual double getSmallBlockSize();

        virtual void setSmallBlockSize(double size);

        virtual double getHeight();

        virtual void setHeight(double size);

        virtual double getCornerSize();

        virtual void setCornerSize(double size);

        virtual double getSpacingLineText();

        virtual void setSpacingLineText(double size);

      protected:

        virtual void drawRuler(QGraphicsView* view, QPainter* painter) = 0;

        virtual void drawText(QPoint p, std::string txt, double angle, QPainter* painter);
        
      protected:

        bool                m_visible;

        PaperConfig*        m_paperConfig;
        double	            m_longLine;
        double	            m_mediumLine;
        double              m_smallLine;
        int		              m_blockSize;
        int		              m_middleBlockSize;
        int                 m_smallBlockSize;
        double              m_height;
        double              m_cornerSize;
        double              m_spacingLineText;
    };
  }
}

#endif 
