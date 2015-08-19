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
  \file ParentItem.h
   
   \brief Abstract class that represents a graphic item.  
   Its coordinate system is the same of scene (millimeters). Knows rotate and resize. Stores a pixmap drawn by model.
   This is also son of ItemObserver, so it can become observer of a model (Observable). 
   This class will be inherited and will became the view part of the MVC component.
   Who inherits it is required the implementation of updateObserver(ContextItem context) method.
   Drawing starting point is llx, lly.
   Can't add signals and slots in this class because moc(Qt) doesn't support templates.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PARENT_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_PARENT_ITEM_H

// Qt
#include <QPixmap>
#include <QVariant>
#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QTransform>
#include <QColor>
#include <QGraphicsScene>

// TerraLib
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../../geometry/Envelope.h"
#include "../../core/enum/AbstractType.h"
#include "../../../color/RGBAColor.h"
#include "../../core/Config.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/ItemModelObservable.h"
#include "../core/Scene.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../core/ContextItem.h"
#include "../../../geometry/Coord2D.h"
#include "../../core/ContextObject.h"

// STL

#include <string>

namespace te
{
  namespace layout
  {
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
    class ParentItem : public T, public ItemObserver
    {
      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */
        ParentItem(ItemController* controller = 0, Observable* o = 0, bool inverted = false);

        /*!
          \brief Destructor
        */
        virtual ~ParentItem();

        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual void updateObserver(te::layout::ContextItem context);

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual te::gm::Coord2D getPosition();

        virtual void setPixmap( const QPixmap& pixmap );

        virtual QPixmap getPixmap();

        virtual QRectF boundingRect() const;

        virtual void setRect(QRectF rect);

        /*!
          \brief  Gets the adjusted boundigned rectangle which considers the current state of the QPen that will be used to draw it. 
                  The returned rect will be in the ITEM coordinate system.
         */
        virtual QRectF getAdjustedBoundingRect(QPainter* painter) const;

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
        virtual te::color::RGBAColor** getRGBAColorImage(int &w, int &h);

        /*!
          \brief Reimplemented from ItemObserver
         */
        virtual QImage getImage();

      protected:

        /*!
          \brief For any specific drawing, the item must reimplement this function
         */
        virtual void drawItem ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

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
          \brief Reimplemented from QGraphicsItem
         */
        virtual void	mousePressEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void	mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void	mouseMoveEvent ( QGraphicsSceneMouseEvent * event );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void	hoverEnterEvent ( QGraphicsSceneHoverEvent * event );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void	hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual void	hoverMoveEvent ( QGraphicsSceneHoverEvent * event );

        /*!
          \brief Reimplemented from QGraphicsItem
         */
        virtual QVariant	itemChange ( QGraphicsItem::GraphicsItemChange change, const QVariant & value );

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

        /*!
          \brief Draw a text. Converts boundingRect item coordinates (local coordinates) in pixel coordinates (screen coordinates) of the current device.

          \param point initial text coordinate (local coordinates from boundingRect)
          \param painter low-level painting on widgets and other paint devices
          \param text 
         */
        virtual void drawText(QPointF point, QPainter* painter, std::string text);

        virtual void createResizePixmap();

        virtual QColor rgbaToQColor(te::color::RGBAColor color);
        
      protected:

        QPixmap m_pixmap;
        QRectF  m_rect;//In local coordinate
        
        //Resize
        te::gm::Envelope    m_boxCopy; //!< box with resize
        QPixmap             m_clonePixmap;
        bool                m_mousePressedAlt; //!< mouse and active alt key
        QPointF             m_initialCoord;
        QPointF             m_finalCoord;
        bool                m_toResizeItem; //!< pixmap to perform the resize is not yet built
        LayoutAlign         m_enumSides;
        bool                m_resizeMode; //!< pixmap to perform the resize is already built 
    };

    template <class T>
    inline te::layout::ParentItem<T>::ParentItem( ItemController* controller, Observable* o, bool inverted ) :
      T(0),
      ItemObserver(controller, o),
      m_mousePressedAlt(false),
      m_toResizeItem(false),
      m_enumSides(TPNoneSide),
      m_resizeMode(false)
    {

      m_invertedMatrix = inverted;

      QGraphicsItem* item = this;
      Context::getInstance().getScene()->insertItem((ItemObserver*)item);

      this->setFlags(QGraphicsItem::ItemIsMovable
        | QGraphicsItem::ItemIsSelectable
        | QGraphicsItem::ItemSendsGeometryChanges
        | QGraphicsItem::ItemIsFocusable);

      //If enabled is true, this item will accept hover events
      QGraphicsItem::setAcceptHoverEvents(true);

      m_boxCopy = m_model->getBox();

      QRectF rect(0, 0, m_model->getBox().getWidth(), m_model->getBox().getHeight());
      setRect(rect);
    }

    template <class T>
    inline te::layout::ParentItem<T>::~ParentItem()
    {

    }
    
    template <class T>
    inline void te::layout::ParentItem<T>::updateObserver( te::layout::ContextItem context )
    {
      if(m_model)
      {
        if(m_rect.width() != m_model->getBox().getWidth() || 
          m_rect.height() != m_model->getBox().getHeight())
        {
          setRect(QRectF(0, 0, m_model->getBox().getWidth(), m_model->getBox().getHeight()));
        }
      }

      if(context.isChangePos())
      {
        double x = context.getPos().x;
        double y = context.getPos().y;
        QGraphicsItem::setPos(x, y);
      }

      refresh();

      QGraphicsItem::update();
    }

    template <class T>
    inline void te::layout::ParentItem<T>::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
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
      drawItem( painter, option, widget );

      //Draws the frame
      drawFrame(painter);

      //Draws the selection
      if (option->state & QStyle::State_Selected)
      {
        drawSelection(painter);
      }
    }

    template <class T>
    inline void te::layout::ParentItem<T>::drawText( QPointF point, QPainter* painter, std::string text )
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

    template <class T>
    inline te::gm::Coord2D te::layout::ParentItem<T>::getPosition()
    {
      QPointF posF = QGraphicsItem::scenePos();
      qreal valuex = posF.x();
      qreal valuey = posF.y();

      te::gm::Coord2D coordinate;
      coordinate.x = valuex;
      coordinate.y = valuey;

      return coordinate;
    }

    template <class T>
    inline void te::layout::ParentItem<T>::setPixmap( const QPixmap& pixmap )
    {
      if(pixmap.isNull())
        return;

      /* The model draws on Cartesian coordinates, millimeter. 
      The canvas of Terralib 5 works with the Y-Axis inverted, 
      so the pixmap generated is upside down.*/
      m_pixmap = pixmap;
      QImage img = m_pixmap.toImage();
      QImage image = img.mirrored(false, true);
      m_pixmap = QPixmap::fromImage(image);

      te::gm::Envelope box = m_model->getBox();

      if(m_mousePressedAlt)
        box = m_boxCopy;

      QGraphicsItem::prepareGeometryChange();
      setRect(QRectF(0, 0, box.getWidth(), box.getHeight()));
      QGraphicsItem::update();
    }

    template <class T>
    inline void te::layout::ParentItem<T>::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
    {
      if ( !painter )
      {
        return;
      }

      QRectF boundRect = boundingRect();

      painter->save();
      painter->translate( -boundRect.bottomLeft().x(), -boundRect.topRight().y() );  
      QRectF rtSource( 0, 0, m_clonePixmap.width(), m_clonePixmap.height() );
      painter->drawPixmap(boundRect, m_clonePixmap, rtSource);
      painter->restore();      
    }

    template <class T>
    inline void te::layout::ParentItem<T>::drawBackground( QPainter * painter )
    {
      if ( !painter )
      {
        return;
      }

      ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);

      if(!model)
        return;

      const te::color::RGBAColor& clrBack = model->getBackgroundColor();
      QColor backColor(clrBack.getRed(), clrBack.getGreen(), clrBack.getBlue(), clrBack.getAlpha());
      
      painter->save();
      painter->setPen(Qt::NoPen);
      painter->setBrush(QBrush(backColor));
      painter->setBackground(QBrush(backColor));
      painter->setRenderHint( QPainter::Antialiasing, true );

      //gets the adjusted boundigng rectangle based of the painter settings
      QRectF rectAdjusted = getAdjustedBoundingRect(painter);

      //draws the background
      painter->drawRect(rectAdjusted);

      painter->restore();
    }

     template <class T>
    inline void te::layout::ParentItem<T>::drawFrame( QPainter * painter )
    {
      if ( !painter )
      {
        return;
      }

      if(!m_model)
        return;

      ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
      if(!model)
        return;

      if(!model->isBorder())
        return;

      const te::color::RGBAColor& clrFrame = model->getFrameColor();
      QColor borderColor(clrFrame.getRed(), clrFrame.getGreen(), clrFrame.getBlue(), clrFrame.getAlpha());

      painter->save();
      QPen pen(borderColor, 0, Qt::SolidLine);
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
    inline void te::layout::ParentItem<T>::drawSelection( QPainter* painter )
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
    inline QRectF te::layout::ParentItem<T>::boundingRect() const
    {
      return m_rect;
    }

    template <class T>
    inline void te::layout::ParentItem<T>::setRect( QRectF rect )
    {
      if (rect.isEmpty() && !rect.isNull())
        return;

      m_rect = rect;
      T::update(rect);
    }

    template <class T>
    inline QRectF te::layout::ParentItem<T>::getAdjustedBoundingRect(QPainter* painter) const
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
    inline void te::layout::ParentItem<T>::mousePressEvent( QGraphicsSceneMouseEvent * event )
    {  
      QGraphicsItem::mousePressEvent(event);

      if(m_toResizeItem && m_model->isResizable())
      {
        m_clonePixmap = getPixmap();
        createResizePixmap();
        m_mousePressedAlt = true;
        m_initialCoord = event->scenePos();
      }
    }

    template <class T>
    inline void te::layout::ParentItem<T>::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
    {  
      QGraphicsItem::mouseReleaseEvent(event);

      m_finalCoord = event->scenePos();

      te::gm::Envelope boxScene = createNewBoxInCoordScene(event->scenePos().x(), event->scenePos().y());
      if(boxScene.isValid() && boxScene.getWidth() > 0 && boxScene.getHeight() > 0)
        m_controller->setBox(boxScene);

      m_mousePressedAlt = false;

      if(m_model->isResizable() && m_toResizeItem && boxScene.isValid())
      {
        m_clonePixmap = QPixmap();
        m_toResizeItem = false;
        m_resizeMode = false;
        redraw();
        T::setOpacity(1.);
      }

      refresh();
    }

    template <class T>
    inline void te::layout::ParentItem<T>::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
    {
      if(event->buttons() == Qt::LeftButton && m_toResizeItem && m_model->isResizable())
      {
        m_mousePressedAlt = true;
        T::setOpacity(0.5);

        m_finalCoord = event->scenePos();

        QPixmap pix = calculateNewPixmap(event->scenePos().x(), event->scenePos().y());
        setPixmap(pix);
        T::update();
      }
      else
      {
        if(!m_toResizeItem)
          T::setOpacity(1.);
        m_mousePressedAlt = false;
        QGraphicsItem::mouseMoveEvent(event);
      }
    }

    template <class T>
    inline void te::layout::ParentItem<T>::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
    {
      QGraphicsItem::hoverEnterEvent(event);
    }

    template <class T>
    inline void te::layout::ParentItem<T>::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
    {
      T::setCursor(Qt::ArrowCursor);
      QGraphicsItem::hoverLeaveEvent(event);
    }

    template <class T>
    inline void te::layout::ParentItem<T>::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
    {
      if(m_model->isResizable())
      {
        m_toResizeItem = checkTouchesCorner(event->pos().x(), event->pos().y());
      }
      QGraphicsItem::hoverMoveEvent(event);
    }

    template <class T>
    inline bool te::layout::ParentItem<T>::checkTouchesCorner( const double& x, const double& y )
    {
      bool result = true;
      QRectF bRect = boundingRect();
      double margin = 2.; //precision

      QPointF ll = bRect.bottomLeft();
      QPointF lr = bRect.bottomRight();
      QPointF tl = bRect.topLeft();
      QPointF tr = bRect.topRight();

      if((x >= (ll.x() - margin) && x <= (ll.x() + margin))
        && (y >= (ll.y() - margin) && y <= (ll.y() + margin)))
      {
        T::setCursor(Qt::SizeFDiagCursor);
        m_enumSides = TPLowerLeft;
      }
      else if((x >= (lr.x() - margin) && x <= (lr.x() + margin))
        && (y >= (lr.y() - margin) && y <= (lr.y() + margin)))
      {
        T::setCursor(Qt::SizeBDiagCursor);
        m_enumSides = TPLowerRight;
      }
      else if((x >= (tl.x() - margin) && x <= (tl.x() + margin))
        && (y >= (tl.y() - margin) && y <= (tl.y() + margin)))
      {
        T::setCursor(Qt::SizeBDiagCursor);
        m_enumSides = TPTopLeft;
      }
      else if((x >= (tr.x() - margin) && x <= (tr.x() + margin))
        && (y >= (tr.y() - margin) && y <= (tr.y() + margin)))
      {
        T::setCursor(Qt::SizeFDiagCursor);
        m_enumSides = TPTopRight;
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
    inline QPixmap te::layout::ParentItem<T>::calculateNewPixmap( const double& x, const double& y )
    {    
      te::gm::Envelope boxScene = createNewBoxInCoordScene(x, y);
      QPixmap pix = m_clonePixmap.scaled(boxScene.getWidth(), boxScene.getHeight());

      if(pix.isNull())
        pix = m_clonePixmap;

      return pix;
    }

    template <class T>
    inline te::gm::Envelope te::layout::ParentItem<T>::createNewBoxInCoordScene( const double& x, const double& y )
    {
      QPointF pbxy1 = T::mapToScene(boundingRect().bottomLeft());
      QPointF pbxy2 = T::mapToScene(boundingRect().topRight());

      double dx = 0;
      double dy = 0;

      if(m_mousePressedAlt && m_toResizeItem)
      {
        dx = m_finalCoord.x() - m_initialCoord.x();
        dy = m_finalCoord.y() - m_initialCoord.y();

        switch(m_enumSides)
        {
        case TPLowerRight :
          {
            if(m_finalCoord.x() > pbxy1.x() && m_finalCoord.y() > pbxy2.y())
            {
              m_boxCopy = te::gm::Envelope(m_model->getBox().m_llx, 
                m_model->getBox().m_lly, m_model->getBox().m_urx + dx, m_model->getBox().m_ury + dy);

              //In Parent Coordinates
              T::setPos(QPointF(m_model->getBox().m_llx, m_model->getBox().m_lly));
            }
            break;
          }
        case TPLowerLeft:
          {
            if(m_finalCoord.x() < pbxy2.x() && m_finalCoord.y() > pbxy2.y())
            {
              m_boxCopy = te::gm::Envelope(m_model->getBox().m_llx + dx, 
                m_model->getBox().m_lly - dy, m_model->getBox().m_urx, m_model->getBox().m_ury);

              //In Parent Coordinates
              T::setPos(QPointF(m_finalCoord.x(), m_model->getBox().m_lly));
            }
            break;
          }
        case TPTopRight:
          {
            if(m_finalCoord.x() > pbxy1.x() && m_finalCoord.y() < pbxy1.y())
            {
              m_boxCopy = te::gm::Envelope(m_model->getBox().m_llx, 
                m_model->getBox().m_lly, m_model->getBox().m_urx + dx, m_model->getBox().m_ury - dy);

              //In Parent Coordinates
              T::setPos(QPointF(m_model->getBox().m_llx, m_finalCoord.y()));
            }
            break;
          }
        case TPTopLeft :
          {  
            if(m_finalCoord.x() < pbxy2.x() && m_finalCoord.y() < pbxy1.y())
            {
              m_boxCopy = te::gm::Envelope(m_model->getBox().m_llx + dx, 
                m_model->getBox().m_lly + dy, m_model->getBox().m_urx, m_model->getBox().m_ury);

              //In Parent Coordinates
              T::setPos(QPointF(m_finalCoord.x(), m_finalCoord.y()));
            }
            break;
          }
        default :
          {
            m_boxCopy = m_model->getBox();
            break;
          }
        }
      }

      return m_boxCopy;
    }

    template <class T>
    inline bool te::layout::ParentItem<T>::contains( const QPointF &point ) const
    {
      te::gm::Coord2D coord(point.x(), point.y());
      return m_controller->contains(coord);
    }

    template <class T>
    inline QPixmap te::layout::ParentItem<T>::getPixmap()
    {
      return m_pixmap;
    }

    template <class T>
    inline void te::layout::ParentItem<T>::setZValue( qreal z )
    {
      QGraphicsItem::setZValue(z);
      setZValueItem(z);
    }

    template <class T>
    inline int te::layout::ParentItem<T>::getZValueItem()
    {
      return QGraphicsItem::zValue();
    }

    template <class T>
    inline void te::layout::ParentItem<T>::applyRotation()
    {
      if(!m_model)
        return;

      ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
      if(!model)
        return;

      double angle = model->getAngle();

      if(angle == model->getOldAngle())
        return;

      double w = boundingRect().width();
      double h = boundingRect().height();

      QTransform transf = T::transform();

      if(m_invertedMatrix)
      {
        angle = -angle;
      }

      transf.translate(w/2, h/2);
      T::setTransform(transf);
      T::setRotation(angle);
      transf.translate(-(w/2), -(h/2));
      T::setTransform(transf);
    }

    template <class T>
    inline te::color::RGBAColor** te::layout::ParentItem<T>::getRGBAColorImage(int &w, int &h)
    {
      w = 0;
      h = 0;
      te::color::RGBAColor** teImg = 0;

      QRectF rect = boundingRect();
      QStyleOptionGraphicsItem opt;
      QPixmap pix(rect.width(), rect.height());
      QPainter p(&pix);
      this->paint(&p, &opt, 0);
      QImage img = pix.toImage();

      if(img.isNull())
      {
        return teImg;
      }

      w = pix.width();
      h = pix.height();

      teImg = te::qt::widgets::GetImage(&img);
      return teImg;
    }

    template <class T>
    QImage te::layout::ParentItem<T>::getImage()
    {
      QRectF rect = boundingRect();
      QStyleOptionGraphicsItem opt;
      QPixmap pix(rect.width(), rect.height());
      QPainter p(&pix);
      this->paint(&p, &opt, 0);

      return pix.toImage();
    }

    template <class T>
    inline QVariant te::layout::ParentItem<T>::itemChange( QGraphicsItem::GraphicsItemChange change, const QVariant & value )
    {
      if(change == QGraphicsItem::ItemPositionHasChanged)
      {
        refresh();
      }

      if(change == QGraphicsItem::ItemChildAddedChange)
      {
        QGraphicsItem* item = value.value<QGraphicsItem*>();
        if(item)
        {
          ItemObserver* iOb = dynamic_cast<ItemObserver*>(item);
          if(iOb)
          {
            ItemModelObservable* itemModel = dynamic_cast<ItemModelObservable*>(m_model);
            if(itemModel)
            {
              itemModel->addChildren(iOb);
            }
          }
        }       
      }

      if(change == QGraphicsItem::ItemChildRemovedChange)
      {
        QGraphicsItem* item = value.value<QGraphicsItem*>();
        if(item)
        {
          ItemObserver* iOb = dynamic_cast<ItemObserver*>(item);
          if(iOb)
          {
            ItemModelObservable* itemModel = dynamic_cast<ItemModelObservable*>(m_model);
            if(itemModel)
            {
              itemModel->removeChildren(iOb->getModel()->getHashCode());
            }
          }
        } 
      }

      return QGraphicsItem::itemChange(change, value);
    }

    template <class T>
    inline void te::layout::ParentItem<T>::createResizePixmap()
    {
      QStyleOptionGraphicsItem opt;
      m_clonePixmap = QPixmap(m_rect.width(), m_rect.height());
      QPainter p(&m_clonePixmap);
      this->paint(&p, &opt, 0);

      if(!m_clonePixmap.isNull())
      {
        m_resizeMode = true;
      }
    }

    template <class T>
    inline QColor te::layout::ParentItem<T>::rgbaToQColor( te::color::RGBAColor color )
    {
      QColor clr;
      clr.setRed(color.getRed());
      clr.setGreen(color.getGreen());
      clr.setBlue(color.getBlue());
      clr.setAlpha(color.getAlpha());

      return clr;
    }
  }
}

#endif
