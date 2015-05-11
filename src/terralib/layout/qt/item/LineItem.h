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
  \file LineItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LINE_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_LINE_ITEM_H

// TerraLib
#include "ObjectItem.h"
#include "../../../geometry/Point.h"

// Qt 
#include <QVector>
#include <QPolygonF>

class QGraphicsSceneMouseEvent;

namespace te
{
  namespace layout
  {
    class Observable;

    class LineItem : public ObjectItem
    {
      public:

        LineItem ( ItemController* controller, Observable* o );

        virtual ~LineItem ();

        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual void updateObserver(ContextItem context);

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
      
      protected:

        virtual void drawLine(QPainter * painter);

        QPolygonF   m_poly;
	  };
  }
}

#endif
