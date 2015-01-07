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
   
   \brief Abstract class that represents a graphic item. This object is of type QGraphicsObject. 
   Its coordinate system is the same of scene (millimeters). Knows rotate and resize. Stores a pixmap drawn by model.
   He is also the son of ItemObserver, so it can become observer of a model (Observable). 
   This class will be inherited and will became the view part of the MVC component.

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
#include "../../../color/RGBAColor.h"
#include "../../core/Config.h"

class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;
class QStyleOptionGraphicsItem;
class QWidget;

namespace te
{
  namespace layout
  {
    /*!
    \brief Abstract class that represents a graphic item. This object is of type QGraphicsObject. 
    Its coordinate system is the same of scene (millimeters). Knows rotate and resize. Stores a pixmap drawn by model.
    He is also the son of ItemObserver, so it can become observer of a model (Observable). 
    This class will be inherited and will became the view part of the MVC component.
    Who inherits it is required the implementation of updateObserver(ContextItem context) method.
    Drawing starting point is llx, lly.
	  
	  \ingroup layout

	  \sa te::layout::ItemObserver
	*/
    class TELAYOUTEXPORT ObjectItem : public QGraphicsObject, public ItemObserver
    {
      Q_OBJECT //for slots/signals

      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        ObjectItem(ItemController* controller = 0, Observable* o = 0);

        /*!
          \brief Destructor
        */
        virtual ~ObjectItem();

        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual void updateObserver(ContextItem context) = 0;

        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual te::gm::Coord2D getPosition();

        virtual void setPixmap( const QPixmap& pixmap );

        virtual QPixmap getPixmap();

        /*!
          \brief Mandatory implementation from QGraphicsObject
         */
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        virtual QRectF boundingRect() const;

        virtual void setRect(QRectF rect);

        //Override 
        /*World coordinates(mm)*/
        virtual bool contains(const QPointF &point) const;
        
        /*!
          \brief The Z value decides the stacking order of drawing.

          \param drawing order
        */
        void	setZValue ( qreal z );

        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual te::color::RGBAColor** getImage();

      protected:

        virtual void drawBackground( QPainter* painter );

        virtual void drawSelection(QPainter* painter);

        /*!
          \brief Reimplemented from QGraphicsObject
         */
        virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsObject
         */
        virtual void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsObject
         */
        virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsObject
         */
        virtual void	hoverEnterEvent ( QGraphicsSceneHoverEvent * event );

        /*!
          \brief Reimplemented from QGraphicsObject
         */
        virtual void	hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

        /*!
          \brief Reimplemented from QGraphicsObject
         */
        virtual void	hoverMoveEvent ( QGraphicsSceneHoverEvent * event );

        /*!
          \brief Reimplemented from QGraphicsObject
         */
        virtual QVariant	itemChange ( GraphicsItemChange change, const QVariant & value );

        virtual bool checkTouchesCorner(const double& x, const double& y);

        virtual QPixmap calculateNewPixmap(const double& x, const double& y);

        virtual te::gm::Envelope createNewBoxInCoordScene(const double& x, const double& y);
        
        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual int getZValueItem();

        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual void applyRotation();

      protected:

        QPixmap m_pixmap;
        QRectF  m_rect;//In local coordinate
        
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
