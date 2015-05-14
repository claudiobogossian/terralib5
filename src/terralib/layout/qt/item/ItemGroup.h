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
  \file ItemGroup.h
   
   \brief Class that represents the grouping of objects of type QGraphicsItem, MVC components.
   Its coordinate system is the same of scene (millimeters). 
   He is also the son of ItemObserver and ObjectItem, so it can become observer of a model (Observable). 

  \ingroup layout
*/

#ifndef	__TERRALIB_LAYOUT_INTERNAL_ITEMGROUP_H
#define __TERRALIB_LAYOUT_INTERNAL_ITEMGROUP_H

// Qt
#include <QGraphicsItemGroup>
#include <QPainter>
#include <QPixmap>

// TerraLib
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/enum/AbstractType.h"
#include "../../../geometry/Envelope.h"
#include "../../../color/RGBAColor.h"
#include "../../core/Config.h"

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class QString;

namespace te
{
  namespace layout
  {
    /*!
    \brief Class that represents the grouping of objects of type QGraphicsItem, MVC components.
        Its coordinate system is the same of scene (millimeters). 
        He is also the son of ItemObserver and ObjectItem, so it can become observer of a model (Observable).  
	  
	    \ingroup layout

	    \sa te::layout::ItemObserver
	  */
    class TELAYOUTEXPORT ItemGroup : public QGraphicsItemGroup, public ItemObserver
    {
      public:

        ItemGroup( ItemController* controller, Observable* o );

        virtual ~ItemGroup();

        virtual void updateObserver(ContextItem context);

        virtual te::gm::Coord2D getPosition();

        void setPixmap( const QPixmap& pixmap );

        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        virtual void	addToGroup ( QGraphicsItem * item );

        virtual bool contains(const QPointF &point) const;

        void	setZValue ( qreal z );

        virtual te::color::RGBAColor** getRGBAColorImage(int &w, int &h);

      protected:

        virtual void setPosition(const double& x, const double& y);

        void drawBackground( QPainter * painter );

        void drawSelection( QPainter* painter );

        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent* event );

        virtual void mousePressEvent ( QGraphicsSceneMouseEvent* event );

        virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent* event );
        
        virtual int getZValueItem();

        virtual void applyRotation();
        
      protected:

        QPixmap m_pixmap;
    };
  }
}
#endif
