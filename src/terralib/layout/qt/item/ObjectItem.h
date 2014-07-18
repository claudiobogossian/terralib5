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
  \file ObjectItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_OBJECT_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_OBJECT_ITEM_H

// Qt
#include <QGraphicsObject>
#include <QPixmap>
#include <QVariant>
#include <QPainter>

// TerraLib
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../../geometry/Envelope.h"
#include "../../core/enum/AbstractType.h"

class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;
class QStyleOptionGraphicsItem;
class QWidget;

namespace te
{
  namespace layout
  {
    class ObjectItem : public QGraphicsObject, public ItemObserver
    {
      Q_OBJECT //for slots/signals

      public:

        ObjectItem(ItemController* controller = 0, Observable* o = 0);
        virtual ~ObjectItem();

        virtual void updateObserver(ContextItem context) = 0;
        virtual te::gm::Coord2D getPosition();

        virtual void setPixmap( const QPixmap& pixmap );
        virtual QPixmap getPixmap();

        //Mandatory implementation methods
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
        virtual QRectF boundingRect() const;
        virtual void setRect(QRectF rect);

        //Override 
        /*World coordinates(mm)*/
        virtual bool contains(const QPointF &point) const;

        virtual void setPos(const QPointF &pos);

        void	setZValue ( qreal z );

      protected:

        virtual void drawBackground( QPainter* painter );
        virtual void drawSelection(QPainter* painter);

        //Resize
        virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );
        virtual void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
        virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
        virtual void	hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
        virtual void	hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
        virtual void	hoverMoveEvent ( QGraphicsSceneHoverEvent * event );

        virtual bool checkTouchesCorner(const double& x, const double& y);
        virtual QPixmap calculateNewPixmap(const double& x, const double& y);

        virtual te::gm::Envelope createNewBoxInCoordScene(const double& x, const double& y);
        
        virtual int getZValueItem();

      protected:

        QPixmap m_pixmap;
        QRectF  m_rect;//In local coordinate

        //Resize
        QPixmap m_clonePixmap;
        bool    m_mousePressedCtrl;
        QPointF m_initialCoord;
        QPointF m_finalCoord;
        bool    m_hoverAboveItem;
        bool    m_toResizeItem;
        LayoutSides m_enumSides;
    };
  }
}

#endif //QRECT_LAYOUTITEM_H