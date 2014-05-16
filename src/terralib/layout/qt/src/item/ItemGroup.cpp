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
  \file ItemGroup.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemGroup.h"
#include "Utils.h"
#include "Context.h"
#include "ItemModelObservable.h"
#include "ItemController.h"
#include "../../../color/RGBAColor.h"
#include "../../../../qt/widgets/Utils.h"
#include "../../../../geometry/Envelope.h"
#include "../../../../common/STLUtils.h"

// Qt
#include <QGraphicsSceneMouseEvent>

te::layout::ItemGroup::ItemGroup( ItemController* controller, Observable* o ) :
  QGraphicsItemGroup(0, 0),
  ItemObserver(controller, o)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges);

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
  
  Utils* utils = Context::getInstance()->getUtils();

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

  int indexItem = -1;

  QList<QGraphicsItem*> graphicsItems = childItems();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      if(item->contains(event->scenePos()))
      {
        int a = 0;
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
  painter->drawPixmap(boundRect, m_pixmap, QRectF( 0, 0, m_pixmap.width(), m_pixmap.height() ));
  painter->restore();

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

bool te::layout::ItemGroup::contains( const QPointF &point ) const
{
  ItemModelObservable* model = (ItemModelObservable*)m_controller->getModel();
  
  return model->contains(te::gm::Coord2D(point.x(), point.y()));
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

