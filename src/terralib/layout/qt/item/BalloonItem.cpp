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
#include <cmath>

// Qt
#include <QTextDocument>
#include <QStyleOptionGraphicsItem>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <QVector>

//
#include <QMessageBox>

te::layout::BalloonItem::BalloonItem( ItemController* controller, Observable* o ) :
ObjectItem(controller, o),
  m_rectangleItem(QPointF(0, 0), QPointF(0, 0)),
  m_firstQuadrant(QPointF(0, 0), QPointF(0, 0)),
  m_secondQuadrant(QPointF(0, 0), QPointF(0, 0)),
  m_thirdQuadrant(QPointF(0, 0), QPointF(0, 0)),
  m_fourthQuadrant(QPointF(0, 0), QPointF(0, 0)),
  m_pointClicked(boundingRect().center().x(), boundingRect().center().y()/2),
  m_angle(0)
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

  drawRectBalloon(painter);

  drawLineBalloon(painter/*, m_rectangleItem*/);


  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::BalloonItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
  m_pointClicked = event->pos();
}

void te::layout::BalloonItem::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{

}

void te::layout::BalloonItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{ 

}

void te::layout::BalloonItem::drawRectBalloon( QPainter * painter )
{
  painter->save();

  QColor cblack(0,0,0);
  QPen pn(cblack, 0, Qt::SolidLine);
  painter->setPen(pn);

  m_rectangleItem = QRectF((boundingRect().width()/4), (boundingRect().height()/4), (boundingRect().width()/2), (boundingRect().height()/2));
  m_firstQuadrant = QRectF(QPointF(boundingRect().width()/2, boundingRect().height()/2)/*topLeft*/, QPointF(boundingRect().width(), boundingRect().height())/*bottomRight*/);
  m_secondQuadrant = QRectF(QPointF(0, boundingRect().height()/2)/*topLeft*/, QPointF(boundingRect().width()/2, boundingRect().height())/*bottomRight*/);
  m_thirdQuadrant = QRectF(QPointF(0, 0)/*topLeft*/, QPointF(boundingRect().width()/2, boundingRect().height()/2)/*bottomRight*/);
  m_fourthQuadrant = QRectF(QPointF(boundingRect().width()/2, 0)/*topLeft*/, QPointF(boundingRect().width(), boundingRect().height()/2)/*bottomRight*/);

  painter->setRenderHint(QPainter::Antialiasing);  
  painter->drawRect(m_rectangleItem);

  painter->restore();
}

void te::layout::BalloonItem::drawLineBalloon( QPainter * painter/*, QRectF rectangle*/ )
{
  painter->save();

  QColor cblack(0,0,0);
  QPen pn(cblack, 0, Qt::SolidLine);
  painter->setPen(pn);

  m_angle = getAngle(boundingRect().center().x(), boundingRect().center().y(), m_pointClicked.x(), m_pointClicked.y());

  painter->setRenderHint(QPainter::Antialiasing);  

  painter->drawRect(m_rectangleItem);

  //QPointF baseLeft(boundingRect().center().x() - 1, boundingRect().center().y());
  //QPointF baseRight(boundingRect().center().x() + 1, boundingRect().center().y());
  //QPointF trianglePoint (m_pointClicked); //ponta aguda do triângulo até o centro do retangulo
  //QVector<QPointF> vectorTriangle;
  //vectorTriangle.append(m_pointClicked);
  //vectorTriangle.append(baseLeft);
  //vectorTriangle.append(baseRight);
  //QPolygonF balloon (vectorTriangle);

  //painter->drawPolygon(balloon);
  painter->drawLine(m_pointClicked, boundingRect().center());
  painter->translate(boundingRect().width(), boundingRect().height()/2);
  QPainterPath starPath;
  starPath.moveTo(boundingRect().center().x(), boundingRect().center().y());
  starPath.lineTo(boundingRect().center().x() + 40 * cos((m_pointClicked.x() - boundingRect().center().x()) * 3.14), boundingRect().center().y() + 40 * sin((m_pointClicked.y() - boundingRect().center().y()) * 3.14));
  painter->drawPath(starPath);

  starPath.closeSubpath();  
  //painter->rotate(m_angle);

  //m_rectangleItem = rectangle;

  painter->restore();
}

double te::layout::BalloonItem::getAngle( double centerX, double centerY, double clikedX, double clikedY )
{
  m_angle = std::atan2(clikedX - centerX, clikedY - centerY);
  m_angle = (m_angle * (180 / M_PI));
  return m_angle;
}
