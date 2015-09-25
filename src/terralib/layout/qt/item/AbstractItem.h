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
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/AbstractScene.h"
#include "../../core/property/Property.h"
#include "../../core/pattern/singleton/Context.h"

//Qt
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QRectF>
#include <QPoint>
#include <QVariant>
#include <QEvent>
#include <QGraphicsScene>
#include <QColor>
#include <QGraphicsSceneHoverEvent>

class QWidget;

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    class AbstractItemModel;
    /*!
    \brief Abstract class that represents a graphic item.  
      Its coordinate system is the same of scene (millimeters). Knows rotate and resize. Stores a pixmap drawn by model.
      This is also son of AbstractItemView, so it can become observer of a model (Observable). 
      This class will be inherited and will became the view part of the MVC component.
      Who inherits it is required the implementation of updateObserver(ContextItem context) method.
      Drawing starting point is llx, lly.
      Can't add signals and slots in this class because moc(Qt) doesn't support templates.
      \ingroup layout

      \sa te::layout::AbstractItemView
    */
    template <class T>
    class AbstractItem : public T, public AbstractItemView
    {
      public:

        enum Action
        {
          NO_ACTION, RESIZE_ACTION, MOVE_ACTION
        };

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        AbstractItem(AbstractItemController* controller,  bool invertedMatrix = false);

        /*!
          \brief Destructor
        */
        virtual ~AbstractItem();

        /*!
          \brief Gets the bounding rect of the item, in local coordinate system
        */
        virtual QRectF boundingRect() const;

        virtual void refresh();

        virtual void contextUpdated(const ContextObject& context);

        /*!
          \brief Gets the rotation
        */ 
        virtual double getItemRotation() const;

        /*!
          \brief Implemented from AbstractItemView.
        */ 
        virtual void setItemRotation(double rotation);

        /*!
          \brief Implemented from AbstractItemView.
        */
        virtual void setItemPosition(double x, double y);

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        /*!
        \brief Reimplemented from QGraphicsItem. World coordinates(mm).
        */
        virtual bool  contains(const QPointF & point) const;

      protected:

        /*!
          \brief  Gets the adjusted boundigned rectangle which considers the current state of the QPen that will be used to draw it. 
                  The returned rect will be in the item coordinate system.
         */
        virtual QRectF getAdjustedBoundingRect(QPainter* painter) const;

        /*!
          \brief For any specific drawing, the item must reimplement this function
         */
        virtual void drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 ) = 0;

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

        /*!
          \brief Reimplemented from QGraphicsItem to capture changes in the item
         */
        virtual QVariant itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void hoverMoveEvent( QGraphicsSceneHoverEvent * event );

        virtual bool checkTouchesCorner( const double& x, const double& y );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

        virtual void calculateResize();

        virtual void setPixmap();

        virtual void drawItemResized( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        virtual void resized();

      protected:

        //resize
        QRectF            m_rect;
        QPixmap           m_clonePixmap;
        QPointF           m_initialCoord;
        QPointF           m_finalCoord;
        LayoutAlign       m_enumSides;
        Action            m_currentAction;
    };

    template <class T>
    inline te::layout::AbstractItem<T>::AbstractItem(AbstractItemController* controller, bool invertedMatrix)
      : T()
      , AbstractItemView(controller, invertedMatrix)
      , m_enumSides(TPNoneSide)
      , m_currentAction(NO_ACTION)
    {
      T::setFlags(QGraphicsItem::ItemIsMovable
        | QGraphicsItem::ItemIsSelectable
        | QGraphicsItem::ItemSendsGeometryChanges
        | QGraphicsItem::ItemIsFocusable);

      //If enabled is true, this item will accept hover events
      T::setAcceptHoverEvents(true);

      m_rect = boundingRect();
    }

    template <class T>
    inline te::layout::AbstractItem<T>::~AbstractItem()
    {
    }

    template <class T>
    inline QRectF te::layout::AbstractItem<T>::boundingRect() const
    {
      if (m_currentAction == RESIZE_ACTION)
      {
        return m_rect;
      }

      //models stores information in scene CS.
      //To ensure that everything works fine, we must convert the coordinates from scene CS to item CS
      double x = 0.;
      double y = 0.;
      double width = m_controller->getProperty("width").getValue().toDouble();
      double height = m_controller->getProperty("height").getValue().toDouble();

      QRectF rectSceneCS(x, y, width, height);
      QRectF rectItemCS = this->mapRectFromScene(rectSceneCS);

      QRectF boundingRect(0, 0, rectItemCS.width(), rectItemCS.height());
      return boundingRect;
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::refresh()
    {
      T::update();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::contextUpdated(const ContextObject& context)
    {
    }

    template <class T>
    inline double te::layout::AbstractItem<T>::getItemRotation() const
    {
      return T::rotation();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::setItemRotation(double angle)
    {
      double w = boundingRect().width();
      double h = boundingRect().height();

      QTransform transf = T::transform();

      if(m_invertedMatrix)
      {
        angle = -angle;
      }

      transf.translate(w/2, h/2);
      transf.rotate(angle);
      T::setTransform(transf);
      T::setRotation(angle);
      transf.translate(-(w/2), -(h/2));
      T::setTransform(transf);
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::setItemPosition(double x, double y)
    {
      T::setPos(x, y);
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

      if (m_currentAction == RESIZE_ACTION)
      {
        drawItemResized(painter, option, widget);
        drawFrame(painter);
        return;
      }

      painter->save();

      //Draws the background
      drawBackground( painter );

      //Draws the item
      drawItem( painter, option, widget );

      //Draws the frame
      drawFrame(painter);

      //Draws the selection
      if (option->state & QStyle::State_Selected)
      {
        drawSelection(painter);
      }

      painter->restore();
    }

    template <class T>
    inline bool te::layout::AbstractItem<T>::contains(const QPointF & point) const
    {
      te::gm::Coord2D coord(point.x(), point.y());
      return m_controller->contains(coord);
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

      qreal adj = penWidth / 2.;
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

      const Property& pBackgroundColor = m_controller->getProperty("background_color");
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

      if(m_controller->getProperty("show_frame").getValue().toBool() == false)
      {
        return;
      }

      const Property& pFrameColor = m_controller->getProperty("frame_color");
      const Property& pFrameThickness = m_controller->getProperty("frame_thickness");
      const te::color::RGBAColor& frameColor = pFrameColor.getValue().toColor();
      double frameThickness = pFrameThickness.getValue().toDouble();

      QColor qFrameColor(frameColor.getRed(), frameColor.getGreen(), frameColor.getBlue(), frameColor.getAlpha());

      painter->save();
      QPen pen(qFrameColor, frameThickness, Qt::SolidLine);
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

      const Property& pFrameThickness = m_controller->getProperty("frame_thickness");
      double frameThickness = pFrameThickness.getValue().toDouble();

      painter->save();

      const QColor fgcolor(0,255,0);
      const QColor backgroundColor(0,0,0);

      QPen penBackground(backgroundColor, frameThickness, Qt::SolidLine);
      painter->setPen(penBackground);
      painter->setBrush(Qt::NoBrush);

      //gets the adjusted boundigng rectangle based of the painter settings
      QRectF rectAdjusted = getAdjustedBoundingRect(painter);
      painter->drawRect(rectAdjusted);

      QPen penForeground(fgcolor, frameThickness, Qt::DashLine);
      painter->setPen(penForeground);
      painter->setBrush(Qt::NoBrush);

      //gets the adjusted boundigng rectangle based of the painter settings
      painter->drawRect(rectAdjusted);

      painter->setPen(Qt::NoPen);
      QBrush brushEllipse(fgcolor);
      painter->setBrush(fgcolor);

      double w = 2.;
      double h = 2.;
      double half = 1.;

      painter->drawRect(rectAdjusted.center().x() - half, rectAdjusted.center().y() - half, w, h); // center
      painter->drawRect(rectAdjusted.bottomLeft().x(), rectAdjusted.bottomLeft().y() - half, w, h); // left-top
      painter->drawRect(rectAdjusted.bottomRight().x() - half, rectAdjusted.bottomRight().y() - half, w, h); // right-top
      painter->drawRect(rectAdjusted.topLeft().x(), rectAdjusted.topLeft().y(), w, h); // left-bottom
      painter->drawRect(rectAdjusted.topRight().x() - half, rectAdjusted.topRight().y(), w, h); // right-bottom

      painter->restore();
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::drawText( const QPointF& point, QPainter* painter, const std::string& text )
    {
      QGraphicsScene* scene = T::scene();
      if(scene == 0)
      {
        return;
      }

      AbstractScene* myScene = dynamic_cast<AbstractScene*>(scene);
      if(myScene == 0)
      {
        return;
      }

      painter->save();

      QTransform transform;
      transform.translate(0., 2. * point.y());
      transform.scale(1., -1.);
      painter->setTransform(transform, true);

      painter->drawText(point, text.c_str());

      painter->restore();
    }

    template <class T>
    inline QVariant te::layout::AbstractItem<T>::itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value )
    {
      if (change == QGraphicsItem::ItemPositionChange && m_currentAction != MOVE_ACTION)
      {
        if (this->isInverted())
        {
          // value is the new position.
          QPointF newPos = value.toPointF();

          double tx = T::transform().dx();
          double ty = T::transform().dy();

          newPos.setX(newPos.x() - tx);
          newPos.setY(newPos.y() - ty);
          return newPos;
        }
      }
      else if (change == QGraphicsItem::ItemPositionHasChanged)
      {
        if(m_currentAction == NO_ACTION)
        {
          m_controller->itemPositionChanged(T::pos().x(), T::pos().y());
        }
      }
      return T::itemChange(change, value);
    }

    template <class T>
    inline void te::layout::AbstractItem<T>::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
    {
      if(m_controller->getProperty("resizable").getValue().toBool())
      {
        checkTouchesCorner(event->pos().x(), event->pos().y());
      }
      T::hoverMoveEvent(event);
    }
  }
}

template <class T>
inline bool te::layout::AbstractItem<T>::checkTouchesCorner( const double& x, const double& y )
{
  bool result = true;
  QRectF bRect = boundingRect();
  double margin = 2.; //precision

  QPointF ll = bRect.bottomLeft();
  QPointF lr = bRect.bottomRight();
  QPointF tl = bRect.topLeft();
  QPointF tr = bRect.topRight();

  if ((x >= (ll.x() - margin) && x <= (ll.x() + margin))
    && (y >= (ll.y() - margin) && y <= (ll.y() + margin)))
  {
    T::setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPTopLeft;
  }
  else if ((x >= (lr.x() - margin) && x <= (lr.x() + margin))
    && (y >= (lr.y() - margin) && y <= (lr.y() + margin)))
  {
    T::setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPTopRight;
  }
  else if ((x >= (tl.x() - margin) && x <= (tl.x() + margin))
    && (y >= (tl.y() - margin) && y <= (tl.y() + margin)))
  {
    T::setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPLowerLeft;
  }
  else if ((x >= (tr.x() - margin) && x <= (tr.x() + margin))
    && (y >= (tr.y() - margin) && y <= (tr.y() + margin)))
  {
    T::setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPLowerRight;
  }
  else
  {
    T::setCursor(Qt::ArrowCursor);
    m_enumSides = TPNoneSide;
    result = false;
  }

  return result;
}

template <class T>
inline void te::layout::AbstractItem<T>::mousePressEvent( QGraphicsSceneMouseEvent * event )
{  
  T::mousePressEvent(event);

  bool startResizing = checkTouchesCorner(event->pos().x(), event->pos().y());
  if (startResizing == true)
  {
    m_currentAction = RESIZE_ACTION;
    setPixmap();
    m_initialCoord = event->pos();
  }
}

template <class T>
inline void te::layout::AbstractItem<T>::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
  if(m_currentAction == RESIZE_ACTION)
  {
    T::setOpacity(0.5);
    m_finalCoord = event->pos();
    T::prepareGeometryChange();
    calculateResize();
  }
  else
  {
    if(event->buttons() == Qt::LeftButton)
    {
      m_currentAction = MOVE_ACTION;
    }

    T::mouseMoveEvent(event);
  }
}

template <class T>
inline void te::layout::AbstractItem<T>::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  if(m_currentAction == RESIZE_ACTION)
  {
    m_finalCoord = event->pos();
    calculateResize();
    QPointF newPos(m_rect.x(), m_rect.y());
    newPos = T::mapToScene(newPos);
    T::setPos(newPos);
    m_rect.moveTo(0, 0);
    T::setOpacity(1.);
    m_controller->resized(m_rect.width(), m_rect.height());
    resized();
  }
  else if(m_currentAction == MOVE_ACTION)
  {
    m_controller->itemPositionChanged(T::pos().x(), T::pos().y());
  }

  m_currentAction = NO_ACTION;

  T::mouseReleaseEvent(event);
}

template <class T>
inline void te::layout::AbstractItem<T>::calculateResize()
{
  double dx = 0;
  double dy = 0;

  if(m_currentAction == RESIZE_ACTION)
  {
    dx = m_finalCoord.x() - m_initialCoord.x();
    dy = m_finalCoord.y() - m_initialCoord.y();

    switch(m_enumSides)
    {
    case TPTopRight:
      {
        m_rect.setWidth(m_rect.width() + dx);
        m_rect.setHeight(m_rect.height() + dy);
        break;
      }
    case TPTopLeft:
      {
        m_rect.setX(m_rect.x() + dx);
        m_rect.setHeight(m_rect.height() + dy);
        break;
      }
    case TPLowerRight:
      {
        m_rect.setY(m_rect.y() + dy);
        m_rect.setWidth(m_rect.width() + dx);
        break;
      }
    case TPLowerLeft:
      {
        m_rect.setX(m_rect.x() + dx);
        m_rect.setY(m_rect.y() + dy);
        break;
      }
    default :
      {
        break;
      }
    }
    m_initialCoord = m_finalCoord;
  }
}

template <class T>
inline void te::layout::AbstractItem<T>::drawItemResized( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
  painter->save();
  painter->setClipRect(boundingRect());
  painter->drawPixmap(boundingRect(), m_clonePixmap, m_clonePixmap.rect());
  painter->restore();
}

template <class T>
inline void te::layout::AbstractItem<T>::setPixmap()
{
  Utils* utils = Context::getInstance().getUtils();
  QRectF itemBounding = boundingRect();
  te::gm::Envelope box(0, 0, itemBounding.width(), itemBounding.height());
  box = utils->viewportBox(box);
  m_clonePixmap = QPixmap(box.getWidth(), box.getHeight());
  QPainter p(&m_clonePixmap);
  double resX = box.getWidth() / itemBounding.width();
  double resY = box.getHeight() / itemBounding.height();
  QTransform transform;
  transform.scale(resX, -resY);
  transform.translate(-itemBounding.bottomLeft().x(), -itemBounding.bottomLeft().y());
  p.setTransform(transform);
  QStyleOptionGraphicsItem opt;
  this->drawItem(&p, &opt, 0);
  p.end();
  QImage image = m_clonePixmap.toImage();
  image = image.mirrored();
  m_clonePixmap = QPixmap::fromImage(image);
}

template <class T>
inline void te::layout::AbstractItem<T>::resized()
{

}

#endif //__TERRALIB_LAYOUT_INTERNAL_ABSTRACT_ITEM_H
