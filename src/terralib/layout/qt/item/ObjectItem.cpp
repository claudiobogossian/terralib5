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
  m_nameClass = std::string(this->metaObject()->className());
  Context::getInstance().getScene()->insertItem((ItemObserver*)item);

  //If enabled is true, this item will accept hover events
  setAcceptHoverEvents(true);

  m_boxCopy = m_model->getBox();
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

void te::layout::ObjectItem::setPixmap( const QPixmap& pixmap )
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

  if(m_mousePressedCtrl)
    box = m_boxCopy;

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
  QRectF rtSource( 0, 0, m_pixmap.width(), m_pixmap.height() );
  painter->drawPixmap(boundRect, m_pixmap, rtSource);
  painter->restore();  
  
  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::ObjectItem::drawBackground( QPainter * painter )
{
  if ( !painter )
  {
    return;
  }

  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setRenderHint( QPainter::Antialiasing, true );
  painter->drawRect(QRectF( 0, 0, boundingRect().width(), boundingRect().height()));
  painter->restore();
}

void te::layout::ObjectItem::drawSelection( QPainter* painter )
{
  if(!painter)
  {
    return;
  }

  painter->save();

  qreal penWidth = painter->pen().widthF();

  const qreal adj = penWidth / 2;
  const QColor fgcolor(0,255,0);
  const QColor backgroundColor(0,0,0);

  QRectF rtAdjusted = boundingRect().adjusted(adj, adj, -adj, -adj);

  QPen penBackground(backgroundColor, 0, Qt::SolidLine);
  painter->setPen(penBackground);
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rtAdjusted);

  QPen penForeground(fgcolor, 0, Qt::DashLine);
  painter->setPen(penForeground);
  painter->setBrush(Qt::NoBrush);
  painter->drawRect(rtAdjusted);
  painter->restore();
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
    m_initialCoord = event->scenePos();
  }
}

void te::layout::ObjectItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{  
  QGraphicsItem::mouseReleaseEvent(event);

  m_finalCoord = event->scenePos();

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

    m_finalCoord = event->scenePos();

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
	setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPLowerLeft;
  }
  else if((x >= (lr.x() - margin) && x <= (lr.x() + margin))
    && (y >= (lr.y() - margin) && y <= (lr.y() + margin)))
  {
	setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPLowerRight;
  }
  else if((x >= (tl.x() - margin) && x <= (tl.x() + margin))
    && (y >= (tl.y() - margin) && y <= (tl.y() + margin)))
  {
	setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPTopLeft;
  }
  else if((x >= (tr.x() - margin) && x <= (tr.x() + margin))
    && (y >= (tr.y() - margin) && y <= (tr.y() + margin)))
  {
	setCursor(Qt::SizeFDiagCursor);
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
  QPointF pbxy1 = mapToScene(boundingRect().bottomLeft());
  QPointF pbxy2 = mapToScene(boundingRect().topRight());

  double dx = 0;
  double dy = 0;

  if(m_mousePressedCtrl && m_toResizeItem)
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
          setPos(QPointF(m_model->getBox().m_llx, m_model->getBox().m_lly));
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
          setPos(QPointF(m_finalCoord.x(), m_model->getBox().m_lly));
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
          setPos(QPointF(m_model->getBox().m_llx, m_finalCoord.y()));
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
          setPos(QPointF(m_finalCoord.x(), m_finalCoord.y()));
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

bool te::layout::ObjectItem::contains( const QPointF &point ) const
{
  te::gm::Coord2D coord(point.x(), point.y());
  return m_controller->contains(coord);
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

void te::layout::ObjectItem::applyRotation()
{
  if(!m_model)
    return;

  ItemModelObservable* model = dynamic_cast<ItemModelObservable*>(m_model);
  if(!model)
    return;

  double angle = model->getAngle();

  if(angle == model->getOldAngle())
    return;

  QPointF center = boundingRect().center();

  double centerX = center.x();
  double centerY = center.y();

  setTransform(QTransform().translate(centerX, centerY).rotate(angle).translate(-centerX, -centerY));
}

te::color::RGBAColor** te::layout::ObjectItem::getImage()
{
  QImage img = m_pixmap.toImage();
  te::color::RGBAColor** teImg = te::qt::widgets::GetImage(&img);
  return teImg;
}

QVariant te::layout::ObjectItem::itemChange( GraphicsItemChange change, const QVariant & value )
{
  if(change == QGraphicsItem::ItemPositionHasChanged)
  {
    refresh();
  }
  return QGraphicsItem::itemChange(change, value);
}
