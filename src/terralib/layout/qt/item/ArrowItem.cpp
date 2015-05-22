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
  \file ArrowItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ArrowItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/ArrowModel.h"

#include <cmath>
#include "geos/platform.h"
#include "qgraphicsitem.h"
#include "qpoint.h"

te::layout::ArrowItem::ArrowItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o)
{
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::ArrowItem::~ArrowItem()
{

}

void te::layout::ArrowItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground(painter);

  drawRightArrow(painter);

  drawBorder(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::ArrowItem::drawLeftArrow( QPainter * painter )
{
	QRectF boundRect = boundingRect();

	QColor cblack(0,0,0);
	QPen pn(cblack, 0, Qt::SolidLine);
	painter->setPen(pn);

	double centerY = boundRect.center().y();
	QLineF lne(boundRect.bottomLeft().x(), centerY, boundRect.width(), centerY);

	painter->save();

	double h = boundRect.height();
	double Pi = 3.14;
	double sizeHead = 10;

	double angle = std::acos(lne.dx() / lne.length());
	if (lne.dy() >= 0)
		angle = (Pi * 2) - angle;

	QPointF arrowP1 = lne.p1() + QPointF(sin(angle + Pi / 3) * sizeHead,
		cos(angle + Pi / 3) * sizeHead);
	QPointF arrowP2 = lne.p1() + QPointF(sin(angle + Pi - Pi / 3) * sizeHead,
		cos(angle + Pi - Pi / 3) * sizeHead);

	qreal extra = (pn.width() + 20) / 2.0;

	QRectF laleska (lne.p1(), QSizeF(lne.p2().x() - lne.p1().x(),
	lne.p2().x() - lne.p1().y()));

	QPolygonF trianglePolygon;
	trianglePolygon << lne.p1() << arrowP1 << arrowP2;
	painter->drawPolygon(trianglePolygon);
	painter->drawRect(laleska);
	painter->restore();

}

void te::layout::ArrowItem::drawRightArrow( QPainter * painter )
{
	QRectF boundRect = boundingRect();



	double centerY = boundRect.center().y();
	QLineF lne(boundRect.bottomLeft().x(), centerY, boundRect.width(), centerY);
	double centerW = boundRect.width()/2;
	painter->save();

	QPointF p1 = QPointF(boundRect.bottomLeft().x()/2, centerW+10);
	QPointF p2 = QPointF(boundRect.width()/2+10, boundRect.width()/2+5);
	QPointF p3 = (boundRect.width()/2+5, boundRect.bottomRight()/2);
	QPointF p4 = (boundRect.bottomRight()/2,boundRect.bottomRight()/2);
	QPointF p5 = QPointF(boundRect.bottomRight().y()/2,boundRect.width()/2-5);
	QPointF p6 = QPointF(boundRect.width()/2-5, boundRect.width()/2-10);
	QPointF p7 = (boundRect.width()/2-10, boundRect.bottomLeft()/2);

	QPolygonF arrowPolygon;
	arrowPolygon<<p1<<p2<<p3<<p4<<p5<<p6<<p7;
	painter->drawPolygon(arrowPolygon);
	painter->restore();

}
