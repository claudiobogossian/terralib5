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
  \file RectangleItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BalloonItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"

// STL
#include <algorithm>    // std::max and std::min

// Qt
#include <QTextDocument>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>
//
#include <QMessageBox>

te::layout::BalloonItem::BalloonItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o)
{      
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::BalloonItem::~BalloonItem()
{

}

void te::layout::BalloonItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /* = 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground(painter);
  
  drawBalloon(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::BalloonItem::drawBalloon( QPainter* painter )
{
  QPointF centerBoundRect = boundingRect().center();
  double xWidth = (boundingRect().width()/2) / 2;
  double yHeight = (boundingRect().height()/2) / 2;

  QRectF boundRect(centerBoundRect.x() - xWidth, centerBoundRect.y() - yHeight, boundingRect().width()/2, boundingRect().height()/2);

  painter->save();

  painter->drawRect(boundRect);

  QPainterPath balloonPath;

  double arrowWidth = 25.;
  double arrowHeight = 15;
  
  double x = m_endPoint.x();
  double y = m_endPoint.y();

  double centerX = boundRect.center().x();
  double centerY = boundRect.center().y();

  // to draw the point part that indicates who is speaking  
  QPolygonF poly;
  poly.append(QPointF(x, y));
  poly.append(QPointF(x - arrowWidth / 2, y - arrowHeight));
  poly.append(QPointF(x + arrowWidth / 2, y - arrowHeight));
  poly.append(QPointF(x, y));

  balloonPath.addPolygon(poly);

  //painter->drawPath(balloonPath);
  
  painter->restore();
}

void te::layout::BalloonItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  m_initPoint = QPointF(event->pos());
  QGraphicsItem::mousePressEvent(event);
}

void te::layout::BalloonItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
  QGraphicsItem::mouseMoveEvent(event);
}

void te::layout::BalloonItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{ 
  m_endPoint = QPointF(event->pos());
  QGraphicsItem::mouseReleaseEvent(event);
}





