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
  \file QLayoutItemGroup.h
   
  \brief 

  \ingroup layout
*/

#ifndef	__TERRALIB_LAYOUT_INTERNAL_QLAYOUT_ITEMGROUP_H
#define __TERRALIB_LAYOUT_INTERNAL_QLAYOUT_ITEMGROUP_H

// Qt
#include <QGraphicsItemGroup>
#include <QPainter>
#include <QPixmap>

// TerraLib
#include "LayoutItemObserver.h"
#include "LayoutAbstractType.h"
#include "../../../../geometry/Envelope.h"

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class QString;

namespace te
{
  namespace layout
  {
    class QLayoutItemGroup : public QGraphicsItemGroup, public LayoutItemObserver
    {
      public:
        QLayoutItemGroup( LayoutItemController* controller, LayoutObservable* o );
        ~QLayoutItemGroup();

        virtual void updateObserver(ContextLayoutItem context);
        virtual te::gm::Coord2D getPosition();

        void setPixmap( const QPixmap& pixmap );
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        virtual void	addToGroup ( QGraphicsItem * item );
        virtual bool contains(const QPointF &point) const;

      protected:

        virtual void setPosition(const double& x, const double& y);
        void drawBackground( QPainter * painter );

        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );
        virtual void mousePressEvent ( QGraphicsSceneMouseEvent* event );
        virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent* event );

      protected:

        QPixmap m_pixmap;

    };
  }
}
#endif