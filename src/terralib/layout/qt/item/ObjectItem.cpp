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
  \file ObjectItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ObjectItem.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/ItemModelObservable.h"
#include "../core/Scene.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"

// Qt
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QStyleOptionGraphicsItem>

te::layout::ObjectItem::ObjectItem( ItemController* controller, Observable* o ) :
  QGraphicsObject(0),
  ItemObserver(controller, o),
  m_mousePressedCtrl(false),
  m_hoverAboveItem(false),
  m_toResizeItem(false),
  m_enumSides(TPNoneSide)
{
  QGraphicsItem* item = this;
  Context::getInstance().getScene()->insertItem((ItemObserver*)item);

  //If enabled is true, this item will accept hover events
  setAcceptHoverEvents(true);
}

te::layout::ObjectItem::~ObjectItem()
{

}

te::gm::Coord2D te::layout::ObjectItem::getPosition()
{
  QPointF posF = scenePos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::ObjectItem::setPos( const QPointF &pos )
{
  QGraphicsItem::setPos(pos);

  refresh();
}

void te::layout::ObjectItem::setPixmap( const QPixmap& pixmap )
{
  m_pixmap = pixmap;

  if(m_pixmap.isNull())
    return;

  QPointF point = pos();

  te::gm::Envelope box = m_model->getBox();

  //If you modify the boundingRect value, you need to inform Graphics View about it by calling QGraphicsItem::prepareGeometryChange();
  QGraphicsObject::prepareGeometryChange();
  //The rect is in coordinate location of the item, because the bounding rect is 
  //updated with this value, and will be used by the scene for drawing.
  //Rect values ​​do not influence the actual position of the object.
  setRect(QRectF(0, 0, box.getWidth(), box.getHeight()));
  update();
}

void te::layout::ObjectItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground( painter );

  QRectF boundRect;
  boundRect = boundingRect();
  	
  painter->save();
  painter->translate( -boundRect.bottomLeft().x(), -boundRect.topRight().y() );
  painter->drawPixmap(boundRect, m_pixmap, QRectF( 0, 0, m_pixmap.width(), m_pixmap.height() ));
  painter->restore();  

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::ObjectItem::drawBackground( QPainter * painter )
{
  if (painter)
  {
    //painter->setBrush(brush());
    painter->setPen(Qt::NoPen);
    painter->setRenderHint( QPainter::Antialiasing, true );
    painter->drawRect(QRectF( 0, 0, boundingRect().width(), boundingRect().height()));
  }
}

void te::layout::ObjectItem::drawSelection( QPainter* painter )
{
  if(!painter)
  {
    return;
  }

  qreal penWidth = painter->pen().widthF();

  const qreal adj = penWidth / 2;
  const QColor fgcolor(255,255,255);
  const QColor backgroundColor(0,0,0);

  painter->setPen(QPen(backgroundColor, 0, Qt::SolidLine));
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(boundingRect().adjusted(adj, adj, -adj, -adj));

  painter->setPen(QPen(fgcolor, 0, Qt::DashLine));
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(boundingRect().adjusted(adj, adj, -adj, -adj));
}

QRectF te::layout::ObjectItem::boundingRect() const
{
  return m_rect;
}

void te::layout::ObjectItem::setRect( QRectF rect )
{
  if (rect.isEmpty() && !rect.isNull())
    return;

  m_rect = rect;
  update(rect);
}

void te::layout::ObjectItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{  
  QGraphicsItem::mousePressEvent(event);

  if(event->modifiers() == Qt::AltModifier && m_toResizeItem)
  {
    m_clonePixmap = getPixmap();
    m_mousePressedCtrl = true;
  }
}

void te::layout::ObjectItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{  
  QGraphicsItem::mouseReleaseEvent(event);

  te::gm::Envelope boxScene = createNewBoxInCoordScene(event->scenePos().x(), event->scenePos().y());
  if(boxScene.isValid() && boxScene.getWidth() > 0 && boxScene.getHeight() > 0)
    m_controller->setBox(boxScene);

  m_mousePressedCtrl = false;

  if(m_toResizeItem && boxScene.isValid())
  {
    m_toResizeItem = false;
    //Antes é necessário saber se o pixmap continua o mesmo, ou foi modificado.
    //Só chamará o redraw se foi modificado.

    redraw();
    setOpacity(1.);
  }

  refresh();
}

void te::layout::ObjectItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
  if(event->modifiers() == Qt::AltModifier && event->buttons() == Qt::LeftButton && m_toResizeItem)
  {
    m_mousePressedCtrl = true;
    setOpacity(0.5);

    QPixmap pix = calculateNewPixmap(event->scenePos().x(), event->scenePos().y());
    setPixmap(pix);
    update();
  }
  else
  {
    if(!m_toResizeItem)
      setOpacity(1.);
    m_mousePressedCtrl = false;
    QGraphicsItem::mouseMoveEvent(event);
  }
}

void te::layout::ObjectItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
  QGraphicsItem::hoverEnterEvent(event);
}

void te::layout::ObjectItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
  m_hoverAboveItem = false;
  setCursor(Qt::ArrowCursor);
  QGraphicsItem::hoverLeaveEvent(event);
}

void te::layout::ObjectItem::hoverMoveEvent( QGraphicsSceneHoverEvent * event )
{
  m_hoverAboveItem = true;
  m_toResizeItem = checkTouchesCorner(event->pos().x(), event->pos().y());
  QGraphicsItem::hoverMoveEvent(event);
}

bool te::layout::ObjectItem::checkTouchesCorner( const double& x, const double& y )
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
    setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPLowerLeft;
  }
  else if((x >= (lr.x() - margin) && x <= (lr.x() + margin))
    && (y >= (lr.y() - margin) && y <= (lr.y() + margin)))
  {
    setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPLowerRight;
  }
  else if((x >= (tl.x() - margin) && x <= (tl.x() + margin))
    && (y >= (tl.y() - margin) && y <= (tl.y() + margin)))
  {
    setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPTopLeft;
  }
  else if((x >= (tr.x() - margin) && x <= (tr.x() + margin))
    && (y >= (tr.y() - margin) && y <= (tr.y() + margin)))
  {
    setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPTopRight;
  }
  else
  {
    setCursor(Qt::ArrowCursor);
    m_enumSides = TPNoneSide;
    result = false;
  }

  return result;
}

QPixmap te::layout::ObjectItem::calculateNewPixmap( const double& x, const double& y )
{    
  te::gm::Envelope boxScene = createNewBoxInCoordScene(x, y);
  QPixmap pix = getPixmap().scaled(boxScene.getWidth(), boxScene.getHeight());

  if(pix.isNull())
    pix = getPixmap();

  return pix;
}

te::gm::Envelope te::layout::ObjectItem::createNewBoxInCoordScene( const double& x, const double& y )
{
  te::gm::Envelope boxScene = m_model->getBox();;

  QPointF posItem = this->scenePos();
  QPointF posAtual(x, y);

  double xTranslation = 0;
  double yTranslation = 0;

  double x1 = posItem.x() < posAtual.x() ? posItem.x() : posAtual.x();
  double y1 = posItem.y() < posAtual.y() ? posItem.y() : posAtual.y();
  double x2 = posItem.x() > posAtual.x() ? posItem.x() : posAtual.x();
  double y2 = posItem.y() > posAtual.y() ? posItem.y() : posAtual.y();

  double dx = x2 - x1;
  double dy = y2 - y1;

  if(m_mousePressedCtrl && m_toResizeItem)
  {
    switch(m_enumSides)
    {
    case TPLowerRight :
      {
        boxScene = te::gm::Envelope(posItem.x(), posItem.y() - dy, posItem.x() + dx, posItem.y());
        break;
      }
    case TPLowerLeft:
      {
        dx = posItem.x() - posAtual.x();
        dy = posItem.y() - posAtual.y();

        QPointF pbxy2 = mapToScene(boundingRect().topRight());
        if(posAtual.x() < pbxy2.x() && posAtual.y() < posItem.y())
        {
          boxScene = te::gm::Envelope(posItem.x() - dx, posItem.y() - dy, pbxy2.x(), posItem.y());

          QPointF p_f = mapFromScene(QPointF(posItem.x() - dx, posItem.y()));
          QPointF p_ff = mapToParent(p_f);                                  
          xTranslation = p_ff.x();
          yTranslation = p_ff.y();

          //In Parent Coordinates
          setPos( QPointF(xTranslation, yTranslation) );
        }
        break;
      }
    case TPTopRight:
      {

        QRectF bond = boundingRect();

        QPointF pbxy2 = mapToScene(boundingRect().topRight());
        QPointF pbxy1 = mapToScene(boundingRect().bottomLeft());
        
        dx = posAtual.x() - pbxy2.x();
        dy = posAtual.y() - pbxy2.y();

        if(posAtual.x() > posItem.x() && posAtual.y() < pbxy1.y())
        {
          boxScene = te::gm::Envelope(boxScene.getLowerLeftX(), 
            boxScene.getLowerLeftY(), boxScene.getUpperRightX() + dx, boxScene.getUpperRightY() + dy);

          QPointF p_f = mapFromScene(QPointF(posItem.x(), posItem.y() + dy));
          QPointF p_ff = mapToParent(p_f);
          xTranslation = p_ff.x();
          yTranslation = p_ff.y();

          setRect(QRectF(0, 0, boxScene.getWidth(), boxScene.getHeight()));
          
          //In Parent Coordinates
          setPos( QPointF(xTranslation, yTranslation) );
        }
        break;
      }
    case TPTopLeft :
      {  
        QPointF pbxy2 = mapToScene(boundingRect().topLeft());

        dx = posAtual.x() - pbxy2.x();
        dy = posAtual.y() - pbxy2.y();

        QPointF pbxy1 = mapToScene(boundingRect().bottomRight());
        if(posAtual.x() < pbxy1.x() && posAtual.y() > pbxy1.y())
        {
          boxScene = te::gm::Envelope(posItem.x() + dx, pbxy1.y(), pbxy1.x(), posItem.y() + dy);

          QPointF p_f = mapFromScene(QPointF(posItem.x() + dx, posItem.y() + dy));
          QPointF p_ff = mapToParent(p_f);
          xTranslation = p_ff.x();
          yTranslation = p_ff.y();

          //In Parent Coordinates
          setPos( QPointF(xTranslation, yTranslation) );
        }
        break;
      }
    default :
      {
        boxScene = te::gm::Envelope(posItem.x(), posItem.y() - boundingRect().height(), 
          posItem.x() + boundingRect().width(), posItem.y());
        break;
      }
    }
  }

  return boxScene;
}

bool te::layout::ObjectItem::contains( const QPointF &point ) const
{
  return m_controller->contains(te::gm::Coord2D(point.x(), point.y()));
}

QPixmap te::layout::ObjectItem::getPixmap()
{
  return m_pixmap;
}

void te::layout::ObjectItem::setZValue( qreal z )
{
  QGraphicsItem::setZValue(z);
  setZValueItem(z);
}

int te::layout::ObjectItem::getZValueItem()
{
  return QGraphicsItem::zValue();
}

