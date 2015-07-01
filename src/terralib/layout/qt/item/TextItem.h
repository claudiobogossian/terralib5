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
  This is also son of ItemObserver, so it can become observer of a model (Observable). "View" part of MVC component.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXT_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_TEXT_ITEM_H

// TerraLib
#include "../../core/pattern/mvc/ItemObserver.h"
#include "ObjectItem.h"
#include "../../core/Config.h"
#include "ParentItem.h"

// Qt
#include <QGraphicsTextItem>
#include <QImage>
#include <QColor>
#include <QVariant>
#include <QPointF>
#include <QRectF>
#include "QtPropertyBrowser/qtpropertybrowser.h"
#include "terralib/se/Font.h"

class QTextTable;
class QGraphicsSceneMouseEvent;
class QKeyEvent;
class QGraphicsSceneContextMenuEvent;

namespace te
{
  namespace layout
  {
    class Observable;

    /*!
    \brief Class that represents text. This object is of type QGraphicsTextItem. He is directly editable via user interaction. 
    His transformation matrix is inverted, that is, the inverse of the matrix of the scene, so its coordinate system is screen (pixel), 
    but its position in the scene remains in millimeters.
    Drawing starting point is llx, lly.
    He is also the son of ItemObserver, so it can become observer of a model (Observable). "View" part of MVC component.
	  
	  \ingroup layout

	  \sa te::layout::ItemObserver
	*/
    class TELAYOUTEXPORT TextItem : public ParentItem<QGraphicsTextItem>
    {
      Q_OBJECT //for slots/signals

      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        TextItem( ItemController* controller, Observable* o );

        /*!
          \brief Destructor
        */ 
        virtual ~TextItem();

        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual void updateObserver( ContextItem context );

        /*!
          \brief Reimplemented from QGraphicsTextItem
         */
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
                
        virtual void refreshDocument();
                
        /*!
          \brief 

          \return
         */
        virtual bool isEditable();

        /*!
          \brief 

          \param
         */
        virtual void setEditable(bool editable);

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual QRectF boundingRect() const;

        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual te::color::RGBAColor** getRGBAColorImage(int &w, int &h);
                
      protected:

        /*!
          \brief Reimplemented from QGraphicsTextItem
         */
        virtual QVariant itemChange ( GraphicsItemChange change, const QVariant & value );
                
        /*!
          \brief Reimplemented from QGraphicsTextItem
         */
        virtual void keyPressEvent ( QKeyEvent * event );

        /*!
          \brief Reimplemented from QGraphicsTextItem
         */
        virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsTextItem
         */
        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsTextItem
         */
        virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsTextItem
         */
        virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
                
        virtual void init();
        
        virtual QImage createImage();

        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual te::gm::Coord2D getPosition();
        
        virtual void getDocumentSizeMM(double &w, double &h);

        virtual void resetEdit();

        virtual void updateTextConfig();

        void setAlignment(Qt::Alignment alignment);

        virtual void drawAlignmentCenter(QPainter * painter);

        virtual void drawAlignmentLeft(QPainter * painter);

        virtual void drawAlignmentRight(QPainter * painter);

        QColor         m_backgroundColor;
        QColor         m_fontColor;
        bool           m_editable;
        QTextTable*    m_table;
        bool           m_move;
        bool           m_update;
    };
  }
}
#endif