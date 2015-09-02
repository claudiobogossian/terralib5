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
  \file TextItem.h
   
  \brief Class that represents text. This object is of type QGraphicsTextItem. He is directly editable via user interaction. 
  His transformation matrix is inverted, that is, the inverse of the matrix of the scene, so its coordinate system is screen (pixel), 
  but its position in the scene remains in millimeters.
  This is also son of AbstractItem, so it can become observer of a model (Observable). "View" part of MVC component.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXT_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_TEXT_ITEM_H

// TerraLib
#include "AbstractItem.h"
#include "../../core/Config.h"

// Qt
#include <QGraphicsTextItem>

class QKeyEvent;

namespace te
{
  namespace layout
  {
    /*!
    \brief Class that represents text. This object is of type QGraphicsTextItem. He is directly editable via user interaction. 
    His transformation matrix is inverted, that is, the inverse of the matrix of the scene, so its coordinate system is screen (pixel), 
    but its position in the scene remains in millimeters.
    Drawing starting point is llx, lly.
    He is also the son of AbstractItem, so it can become observer of a model (Observable). "View" part of MVC component.
    
    \ingroup layout

    \sa te::layout::AbstractItem
  */
    class TELAYOUTEXPORT TextItem : public AbstractItem<QGraphicsTextItem>
    {
      Q_OBJECT

      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        TextItem( AbstractItemController* controller, AbstractItemModel* model, bool invertedMatrix = false );

        /*!
          \brief Destructor
        */ 
        virtual ~TextItem();

        /*!
          \brief For any specific drawing, the item must reimplement this function
        */
        virtual void drawItem ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        /*!
          \brief Reimplemented from QGraphicsItem to capture changes in the item
        */
        virtual QVariant itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value );

        /*!
          \brief Reimplemented from QGraphicsItem
        */
        virtual QRectF boundingRect() const;

      protected:

        /*!
          \brief Reimplemented from QGraphicsTextItem
         */
        virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );

        /*!
        \brief Reimplemented from QGraphicsTextItem
        */
        virtual void keyPressEvent(QKeyEvent * event);

        /*!
          \brief Enters the edition mode
         */
        void enterEditionMode();

        /*!
          \brief Leaves edition mode
         */
        void leaveEditionMode();

      protected slots:

        virtual void updateGeometry( int position, int charsRemoved, int charsAdded );
    };
  }
}
#endif
