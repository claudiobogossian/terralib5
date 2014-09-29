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
  \file DefaultTextItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_DEFAULT_TEXT_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_DEFAULT_TEXT_ITEM_H

// TerraLib
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/enum/AbstractType.h"

// Qt
#include <QGraphicsTextItem>
#include <QPixmap>
#include <QVariant>
#include <QPainter>
#include <QTextTable>
#include <QVariant>

namespace te
{
  namespace layout
  {
    class Observable;

    class DefaultTextItem : public QGraphicsTextItem, public ItemObserver
    {
      Q_OBJECT //for slots/signals

      public:

        DefaultTextItem( ItemController* controller, Observable* o );
        virtual ~DefaultTextItem();
        
        virtual void updateObserver(ContextItem context);

        virtual te::gm::Coord2D getPosition();

        virtual int getZValueItem();

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

        virtual void init();

        void setTextInteraction(bool on, bool selectAll = false);

      protected slots:

        virtual void onContentsChange ( int position, int charsRemoved, int charsAdded );

      protected:

        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *evt);

        virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );

        virtual void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

        virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

        virtual void	hoverEnterEvent ( QGraphicsSceneHoverEvent * event );

        virtual void	hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

        virtual void	hoverMoveEvent ( QGraphicsSceneHoverEvent * event );

        virtual QVariant	itemChange ( GraphicsItemChange change, const QVariant & value );

        virtual void drawBackground( QPainter* painter );

        virtual void drawSelection(QPainter* painter);

        virtual void adjustSizeMM();

        virtual void refreshTable();

        virtual void refreshText();

        virtual void applyRotation();

        virtual bool checkTouchesCorner(const double& x, const double& y);

        virtual QPixmap calculateNewPixmap(const double& x, const double& y);

        virtual te::gm::Envelope createNewBoxInCoordScene(const double& x, const double& y);

      protected:

        QPixmap m_pixmap;
        QRectF  m_rect;//In local coordinate

        QTextTable*    m_table;
        double         m_oldAdjustSizeW;
        double         m_oldAdjustSizeH;

        //Resize
        te::gm::Envelope m_boxCopy;
        QPixmap m_clonePixmap;
        bool    m_mousePressedCtrl;
        QPointF m_initialCoord;
        QPointF m_finalCoord;
        bool    m_hoverAboveItem;
        bool    m_toResizeItem;
        LayoutAlign m_enumSides;
    };
  }
}

#endif
