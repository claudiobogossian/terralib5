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
  \file AbstractRuler.h
   
  \brief Abstract class representing a ruler.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_RULER_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_RULER_H

// TerraLib
#include "../../core/Config.h"

// Qt
#include <QGraphicsView>
#include <QPainter>
#include <QColor>
#include <QFont>

namespace te
{
  namespace layout
  {
    /*!
	  \brief Abstract class representing a ruler.
	  
	  \ingroup layout
	  */
    class TELAYOUTEXPORT AbstractRuler
    {
      public:

        AbstractRuler();

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

        virtual void drawRuler(QGraphicsView* view, QPainter* painter, double scale) = 0;

      protected:

        virtual void drawText(QPoint p, std::string txt, double angle, QPainter* painter);
                
      protected:

        bool                m_visible;

        double	            m_longLine;
        double	            m_mediumLine;
        double              m_smallLine;
        int		              m_blockSize;
        int		              m_middleBlockSize;
        int                 m_smallBlockSize;
        double              m_height;
        double              m_cornerSize;
        double              m_spacingLineText;
        QFont               m_font;
        QColor              m_frontColor;
        QColor              m_middleRulerColor;
        QColor              m_backgroundRulerColor;
        QColor              m_penColor;
        double              m_penWidth;
    };
  }
}

#endif 

