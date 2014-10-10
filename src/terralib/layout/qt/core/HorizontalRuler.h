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
  \file HorizontalRuler.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_HORIZONTAL_RULER_H
#define __TERRALIB_LAYOUT_INTERNAL_HORIZONTAL_RULER_H

// TerraLib
#include "AbstractRuler.h"

namespace te
{
  namespace layout
  {
    class HorizontalRuler : public AbstractRuler
    {
      public:

        HorizontalRuler(PaperConfig* paperConfig);

        virtual ~HorizontalRuler();

        virtual void drawRuler( QGraphicsView* view, QPainter* painter );

    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_HORIZONTALRULER_LAYOUTMODEL_H