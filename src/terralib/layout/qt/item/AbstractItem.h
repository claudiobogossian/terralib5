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
  \file AbstractItem.h
   
   \brief Abstract class that represents a graphic item.  
   Its coordinate system is the same of scene (millimeters). Knows rotate and resize. Stores a pixmap drawn by model.
   This is also son of ItemObserver, so it can become observer of a model (Observable). 
   This class will be inherited and will became the view part of the MVC component.
   Who inherits it is required the implementation of updateObserver(ContextItem context) method.
   Drawing starting point is llx, lly.
   Can't add signals and slots in this class because moc(Qt) doesn't support templates.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ITEM_H

// TerraLib
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/AbstractScene.h"
#include "../core/ContextObject.h"

//Qt
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>

class QWidget;

namespace te
{
  namespace layout
  {
    class AbstractItemController;

    /*!
    \brief Abstract class that represents a graphic item.  
      Its coordinate system is the same of scene (millimeters). Knows rotate and resize. Stores a pixmap drawn by model.
      This is also son of ItemObserver, so it can become observer of a model (Observable). 
      This class will be inherited and will became the view part of the MVC component.
      Who inherits it is required the implementation of updateObserver(ContextItem context) method.
      Drawing starting point is llx, lly.
      Can't add signals and slots in this class because moc(Qt) doesn't support templates.
      \ingroup layout

      \sa te::layout::ItemObserver
    */
    template <class T>
    class AbstractItem : public T, public AbstractItemView
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        AbstractItem(AbstractItemController* controller, AbstractItemModel* model);

        /*!
          \brief Destructor
        */
        virtual ~AbstractItem();

        virtual void update(const Subject* subject);

        /*!
          \brief Gets the bounding rect of the item, in local coordinate system
        */
        virtual QRectF boundingRect() const;

        /*!
          \brief Sets the bounding rect of the item, in local coordinate system
        */
        virtual void setBoundingRect(const QRectF& rect);

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    protected:

        /*!
          \brief  Gets the adjusted boundigned rectangle which considers the current state of the QPen that will be used to draw it. 
                  The returned rect will be in the item coordinate system.
         */
        virtual QRectF getAdjustedBoundingRect(QPainter* painter) const;

        /*!
          \brief For any specific drawing, the item must reimplement this function
         */
        virtual void drawItem( QPainter* painter ) = 0;

        /*!
          \brief Draws the background of the item
         */
        virtual void drawBackground( QPainter* painter );

        /*!
          \brief Draws the frame of the item
         */
        virtual void drawFrame(QPainter* painter);

        /*!
          \brief Draws the selection of the item
         */
        virtual void drawSelection(QPainter* painter);

        /*!
          \brief Draws the given text in the given localtion
         */
        virtual void drawText( const QPointF& point, QPainter* painter, const std::string& text );

    protected:

      QRectF                m_rect;//!< The bounding rect of the item, in local coordinate system
    };

    template <class T>
    inline te::layout::AbstractItem<T>::AbstractItem(AbstractItemController* controller, AbstractItemModel* model)
      : T()
      , AbstractItemView(controller, model)
    {
      T::setFlags(QGraphicsItem::ItemIsMovable
        | QGraphicsItem::ItemIsSelectable
        | QGraphicsItem::ItemSendsGeometryChanges
        | QGraphicsItem::ItemIsFocusable);
    }

    template <class T>
    inline te::layout::AbstractItem<T>::~AbstractItem()
    {
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::update(const Subject* subject)
    {
      T::update();
    }

    template <class T>
    inline QRectF te::layout::AbstractItem<T>::boundingRect() const
    {
      double x1 = m_model->getProperty("x1").getValue().toDouble();
      double y1 = m_model->getProperty("y1").getValue().toDouble();
      double width = m_model->getProperty("width").getValue().toDouble();
      double height = m_model->getProperty("height").getValue().toDouble();

      QRectF rect(x1, y1, width, height);
      return rect;
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::setBoundingRect(const QRectF& rect)
    {
      m_rect = rect;
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
    {
      Q_UNUSED( option );
      Q_UNUSED( widget );
      if ( painter == 0 )
      {
        return;
      }
      
      //Draws the background
      drawBackground( painter );

      //Draws the item
      drawItem( painter );

      //Draws the frame
      drawFrame(painter);

      //Draws the selection
      if (option->state & QStyle::State_Selected)
      {
        drawSelection(painter);
      }
    }

    template <class T>
    inline QRectF te::layout::AbstractItem<T>::getAdjustedBoundingRect(QPainter* painter) const
    {
      qreal penWidth = painter->pen().widthF();
      if(painter->pen().style() == Qt::NoPen)
      {
        penWidth = 0.;
      }

      QRectF bRect = boundingRect();
      bRect.setX(0.);
      bRect.setY(0.);

      const qreal adj = penWidth / 2.;
      QRectF rectAdjusted = bRect.adjusted(adj, adj, -adj, -adj);

      return rectAdjusted;
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawBackground( QPainter * painter )
    {
      if ( !painter )
      {
        return;
      }

      const Property& pBackgroundColor = m_model->getProperty("background_color");
      const te::color::RGBAColor& backgroundColor = pBackgroundColor.getValue().toColor();
      QColor qBackgroundColor(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());
      
      painter->save();
      painter->setPen(Qt::NoPen);
      painter->setBrush(QBrush(qBackgroundColor));
      painter->setBackground(QBrush(qBackgroundColor));
      painter->setRenderHint( QPainter::Antialiasing, true );

      //gets the adjusted boundigng rectangle based of the painter settings
      QRectF rectAdjusted = getAdjustedBoundingRect(painter);

      //draws the background
      painter->drawRect(rectAdjusted);

      painter->restore();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawFrame( QPainter * painter )
    {
      if ( !painter )
      {
        return;
      }

      if(m_model->getProperty("show_frame").getValue().toBool() == false)
      {
        return;
      }

      const Property& pFrameColor = m_model->getProperty("frame_color");
      const te::color::RGBAColor& frameColor = pFrameColor.getValue().toColor();
      QColor qFrameColor(frameColor.getRed(), frameColor.getGreen(), frameColor.getBlue(), frameColor.getAlpha());

      painter->save();
      QPen pen(qFrameColor, 0, Qt::SolidLine);
      painter->setPen(pen);
      painter->setBrush(Qt::NoBrush);
      painter->setRenderHint( QPainter::Antialiasing, true );

      //gets the adjusted boundigng rectangle based of the painter settings
      QRectF rectAdjusted = getAdjustedBoundingRect(painter);

      //draws the frame
      painter->drawRect(rectAdjusted);

      painter->restore();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawSelection( QPainter* painter )
    {
      if(!painter)
      {
        return;
      }

      painter->save();

      const QColor fgcolor(0,255,0);
      const QColor backgroundColor(0,0,0);

      QPen penBackground(backgroundColor, 0, Qt::SolidLine);
      painter->setPen(penBackground);
      painter->setBrush(Qt::NoBrush);

      //gets the adjusted boundigng rectangle based of the painter settings
      QRectF rectAdjusted = getAdjustedBoundingRect(painter);
      painter->drawRect(rectAdjusted);

      QPen penForeground(fgcolor, 0, Qt::DashLine);
      painter->setPen(penForeground);
      painter->setBrush(Qt::NoBrush);

      //gets the adjusted boundigng rectangle based of the painter settings
      rectAdjusted = getAdjustedBoundingRect(painter);
      painter->drawRect(rectAdjusted);

      painter->setPen(Qt::NoPen);
      QBrush brushEllipse(fgcolor);
      painter->setBrush(fgcolor);

      //gets the adjusted boundigng rectangle based of the painter settings
      rectAdjusted = getAdjustedBoundingRect(painter);

      double w = 2.0;
      double h = 2.0;
      double half = 1.0;

      painter->drawRect(rectAdjusted.center().x() - half, rectAdjusted.center().y() - half, w, h); // center
      painter->drawRect(rectAdjusted.bottomLeft().x(), rectAdjusted.bottomLeft().y() - h, w, h); // left-top
      painter->drawRect(rectAdjusted.bottomRight().x() - w, rectAdjusted.bottomRight().y() - h, w, h); // right-top
      painter->drawRect(rectAdjusted.topLeft().x(), rectAdjusted.topLeft().y(), w, h); // left-bottom
      painter->drawRect(rectAdjusted.topRight().x() - w, rectAdjusted.topRight().y(), w, h); // right-bottom

      painter->restore();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawText( const QPointF& point, QPainter* painter, const std::string& text )
    {
      painter->save();

      QTransform t = painter->transform();
      QPointF p = t.map(point);

      QGraphicsScene* scn = T::scene();
      AbstractScene* sc = dynamic_cast<AbstractScene*>(scn);
      ContextObject context = sc->getContext();

      int zoom = context.getZoom();
      double zoomFactor = zoom / 100.;

      QFont ft = painter->font();
      ft.setPointSize(ft.pointSize() * zoomFactor);
      painter->setFont(ft);

      //Keeps the size of the text.(Aspect)
      painter->setMatrixEnabled(false);
      painter->drawText(p, text.c_str());
      painter->setMatrixEnabled(true);

      painter->restore();
    }
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ITEM_H
