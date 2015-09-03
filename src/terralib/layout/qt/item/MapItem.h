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
  \file MapItem.h
   
   \brief This class is a proxy MapDisplay. This makes it possible to add a MapDisplay as item of a scene. 
   This object is of type QGraphicsProxyWidget. He have a directly interaction by user. 
   His transformation matrix is inverted, that is, the inverse of the matrix of the scene, so its coordinate system is screen (pixel), 
   but its position in the scene remains in millimeters.
   This is also son of AbstractItem, so it can become observer of a model (Observable). "View" part of MVC component.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_MAP_ITEM_H

// TerraLib
#include "AbstractItem.h"
#include "../../core/Config.h"

// Qt
#include <QGraphicsObject>

//class QGraphicsSceneMouseEvent;
//class QMimeData;
class QVariant;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
      class Pan;
      class ZoomWheel;
    }
  }

  namespace layout
  {
    /*!
    \brief This class is a proxy MapDisplay. This makes it possible to add a MapDisplay as item of a scene. 
    This object is of type QGraphicsProxyWidget. He have a directly interaction by user. 
    His transformation matrix is inverted, that is, the inverse of the matrix of the scene, so its coordinate system is screen (pixel), 
    but its position in the scene remains in millimeters.
    Drawing starting point is llx, lly.
    He is also the son of AbstractItem, so it can become observer of a model (Observable). "View" part of MVC component.
    
    \ingroup layout

    \sa te::layout::AbstractItem
  */
    class TELAYOUTEXPORT MapItem : public AbstractItem<QGraphicsObject>
    {
      Q_OBJECT //for slots/signals

      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        MapItem( AbstractItemController* controller, AbstractItemModel* model );

        /*!
          \brief Destructor
        */ 
        virtual ~MapItem();

        virtual te::qt::widgets::MapDisplay* getMapDisplay();

        virtual void contextUpdated(const ContextObject& context);

        /*!
          \brief For any specific drawing, the item must reimplement this function
        */
        virtual void drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        /*!
          \brief Reimplemented from QGraphicsItem to capture changes in the item
        */
        virtual QVariant itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value );

        /*!
           \brief Reimplemented from QGraphicsProxyWidget
        */
        virtual void  mousePressEvent ( QGraphicsSceneMouseEvent * event );

        /*!
            \brief Reimplemented from QGraphicsProxyWidget
          */
        virtual void  mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

        /*!
            \brief Reimplemented from QGraphicsProxyWidget
          */
        virtual void  mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsProxyWidget
        */
        virtual void  dragEnterEvent ( QGraphicsSceneDragDropEvent * event );

        /*!
          \brief Reimplemented from QGraphicsProxyWidget
         */
        virtual void  dropEvent ( QGraphicsSceneDragDropEvent * event );
        
        /*!
          \brief Reimplemented from QGraphicsProxyWidget
        */
        virtual void  wheelEvent ( QGraphicsSceneWheelEvent * event );

        virtual void doRefresh();

      protected:

        virtual void enterEditionMode();

        virtual void leaveEditionMode();

        virtual QPointF remapPointToViewport(const QPointF& point, const QRectF& item, const QRectF& widget) const;

    protected slots:

        void extentChanged();

        virtual void  dragLeaveEvent ( QGraphicsSceneDragDropEvent * event );

        virtual void  dragMoveEvent ( QGraphicsSceneDragDropEvent * event );

      protected:

        virtual void resized();

        te::qt::widgets::MapDisplay*  m_mapDisplay;
        te::qt::widgets::Pan*         m_pan;
        te::qt::widgets::ZoomWheel*   m_zoomWheel;
    };
  }
}

#endif


