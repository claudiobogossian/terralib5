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
  \file ItemGroup.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemGroup.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/pattern/mvc/ItemModelObservable.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/ItemObserver.h"

// Qt
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

te::layout::ItemGroup::ItemGroup( ItemController* controller, Observable* o ) :
  QGraphicsItemGroup(0),
  ItemObserver(controller, o)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges);

  QGraphicsItem* item = this;
  Context::getInstance().getScene()->insertItem((ItemObserver*)item);

  m_nameClass = "ItemGroup";

  //If enabled is true, this item will accept hover events
  setAcceptHoverEvents(true);
}

te::layout::ItemGroup::~ItemGroup()
{

}

void te::layout::ItemGroup::updateObserver( ContextItem context )
{
  if(!m_model)
    return;

  te::color::RGBAColor** rgba = context.getPixmap();

  if(!rgba)
    return;
  
  Utils* utils = context.getUtils();

  if(!utils)
    return;

  te::gm::Envelope box = utils->viewportBox(m_model->getBox());

  if(!box.isValid())
    return;

  if(!box.isValid())
    return;

  QPixmap pixmp;
  QImage* img = 0;

  if(rgba)
  {
    img = te::qt::widgets::GetImage(rgba, box.getWidth(), box.getHeight());
    pixmp.fill(Qt::transparent);
    pixmp = QPixmap::fromImage(*img);
  }

  te::common::Free(rgba, box.getHeight());
  if(img)
    delete img;

  setPixmap(pixmp);
  update();
}

void te::layout::ItemGroup::setPosition( const double& x, const double& y )
{
  setPos(x,y);
}

te::gm::Coord2D te::layout::ItemGroup::getPosition()
{
  QPointF posF = scenePos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::ItemGroup::addToGroup( QGraphicsItem * item )
{
  QGraphicsItemGroup::addToGroup(item);

  QRectF chRect = childrenBoundingRect();

  ItemModelObservable* model = (ItemModelObservable*)m_controller->getModel();
  model->setBox(te::gm::Envelope(chRect.x(), chRect.y(), chRect.x() + childrenBoundingRect().width(), 
    chRect.y() + childrenBoundingRect().height()));
}

void te::layout::ItemGroup::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mouseMoveEvent(event);
}

void te::layout::ItemGroup::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mousePressEvent(event);

  QList<QGraphicsItem*> graphicsItems = childItems();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      if(item->contains(event->scenePos()))
      {

      }
    }
  }
}

void te::layout::ItemGroup::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
  QGraphicsItem::mouseReleaseEvent(event);
}

void te::layout::ItemGroup::setPixmap( const QPixmap& pixmap )
{
  m_pixmap = pixmap;
}

void te::layout::ItemGroup::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground( painter );
  ItemModelObservable* model = (ItemModelObservable*)m_controller->getModel();

  QRectF boundRect;
  boundRect = boundingRect();

  painter->save();
  painter->translate( model->getBox().getWidth() / 2.0, model->getBox().getHeight() / 2.0 );
  painter->translate( -boundRect.width() / 2.0, -boundRect.height() / 2.0 );
  QRectF rtSource( 0, 0, m_pixmap.width(), m_pixmap.height() );
  painter->drawPixmap(boundRect, m_pixmap, rtSource);
  painter->restore();

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }

}

void te::layout::ItemGroup::drawBackground( QPainter * painter )
{
  if (painter)
  {
    ItemModelObservable* model = (ItemModelObservable*)m_controller->getModel();
    //painter->setBrush( brush() );//this causes a problem in atlas generation
    painter->setPen( Qt::NoPen );
    painter->setRenderHint( QPainter::Antialiasing, true );
    painter->drawRect( QRectF( 0, 0, model->getBox().getWidth(), model->getBox().getHeight() ) );
  }
}

void te::layout::ItemGroup::drawSelection( QPainter* painter )
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

bool te::layout::ItemGroup::contains( const QPointF &point ) const
{
  te::gm::Coord2D coord(point.x(), point.y());
  return m_controller->contains(coord);
}

void te::layout::ItemGroup::setZValue( qreal z )
{
  QGraphicsItem::setZValue(z);
  setZValueItem(z);
}

int te::layout::ItemGroup::getZValueItem()
{
  return QGraphicsItem::zValue();
}

void te::layout::ItemGroup::applyRotation()
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

te::color::RGBAColor** te::layout::ItemGroup::getRGBAColorImage(int &w, int &h)
{
  QImage img = m_pixmap.toImage();
  te::color::RGBAColor** teImg = te::qt::widgets::GetImage(&img);
  return teImg;
}


