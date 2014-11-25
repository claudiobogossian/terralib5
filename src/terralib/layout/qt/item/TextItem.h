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
  \file TextItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXT_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_TEXT_ITEM_H

// TerraLib
#include "../../core/pattern/mvc/ItemObserver.h"
#include "ObjectItem.h"

// Qt
#include <QGraphicsTextItem>
#include <QImage>
#include <QColor>
#include <QVariant>

class QTextDocument;
class QTextTable;
class QGraphicsSceneMouseEvent;
class QKeyEvent;

namespace te
{
  namespace layout
  {
    class Observable;

    class TextItem : public QGraphicsTextItem, public ItemObserver
    {
      Q_OBJECT //for slots/signals

      public:

        TextItem( ItemController* controller, Observable* o );

        virtual ~TextItem();

        virtual void updateObserver( ContextItem context );

        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
        
        virtual QTextDocument* getDocument();

        virtual void refreshDocument();
                
        virtual bool isEditable();
        
      protected:

        virtual QVariant	itemChange ( GraphicsItemChange change, const QVariant & value );
        
        virtual void drawBackground( QPainter* painter );

        virtual void drawSelection(QPainter* painter);

        virtual void	keyPressEvent ( QKeyEvent * event );

        virtual void	mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );

        virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

        virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );

        virtual void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

        virtual void init();
        
        virtual QImage createImage();

        virtual te::gm::Coord2D getPosition();

        virtual te::color::RGBAColor** getImage();

        virtual int getZValueItem();

        virtual void applyRotation();

        virtual void getDocumentSizeMM(double &w, double &h);

        QTextDocument* m_document;
        QColor         m_backgroundColor;
        bool           m_editable;
        QTextTable*    m_table;
        bool           m_move;
    };
  }
}

#endif
