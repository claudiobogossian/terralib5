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

// STL
#include <cmath>

// Qt
#include <QPointF>

te::layout::ArrowItem::ArrowItem( ItemController* controller, Observable* o, bool invertedMatrix ) :
  ObjectItem(controller, o, invertedMatrix)
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


	ArrowModel* model = dynamic_cast<ArrowModel*>(m_model);

	if(model)
	{
		EnumArrowType* enumScale = model->getEnumArrowType();

		if(model->getCurrentArrowType() == enumScale->getDoubleArrowType())
		{
			drawDoubleArrow(painter);
		}
		if(model->getCurrentArrowType() == enumScale->getRightArrowType())
		{
			drawRightArrow(painter);
		}
		
	}

  drawBorder(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::ArrowItem::drawRightArrow( QPainter * painter )
{
	painter->save();

	double w1 = boundingRect().width() / 2.;
	double w3 = boundingRect().width() / 40.;
	double h1 = boundingRect().height() / 2.;
	double h2 = boundingRect().height() / 4.;
	double y1 = boundingRect().center().y()-h1;
	double y2 = boundingRect().center().y()+h1;
	double y3 = boundingRect().center().y()-h2;
	double y4 = boundingRect().center().y()+h2;

	QPointF p1 = QPointF(w1,y2);
	QPointF p2 = QPointF(y2,w1);
	QPointF p3 = QPointF(w1,y1);
	QPointF p4 = QPointF(h1,y3);
	QPointF p5 = QPointF(w3,y3);
	QPointF p6 = QPointF(w3,y4);
	QPointF p7 = QPointF(h1,y4);
	QPolygonF arrowPolygon;
	arrowPolygon<<p1<<p2<<p3<<p4<<p5<<p6<<p7;
	painter->drawPolygon(arrowPolygon);
	painter->restore();
}

void te::layout::ArrowItem::drawDoubleArrow(QPainter * painter)
{
	double w1 = boundingRect().width() / 2.;
	double w2 = boundingRect().width() / 4.;
	double w3 = boundingRect().width() - boundingRect().width() / 4.;
	double h1 = boundingRect().height() / 2.;
	double h2 = boundingRect().height() / 4.;
	double y1 = boundingRect().center().y()-h1;
	double y2 = boundingRect().center().y()+h1;
	double y4 = boundingRect().center().y()+h2;
	painter->save();
	QPointF p1 = QPointF(w3,y2);
	QPointF p2 = QPointF(y2,w1);
	QPointF p3 = QPointF(w3,y1);
	QPointF p4 = QPointF(w3,h2);
	QPointF p5 = QPointF(h2,w2);
	QPointF p6 = QPointF(w2,y1);
	QPointF p7 = QPointF(y1,w1);
	QPointF p8 = QPointF(w2,y2);
	QPointF p9 = QPointF(h2,y4);
	QPointF p10 = QPointF(w3,y4);
	QPolygonF arrowPolygon;
	arrowPolygon<<p1<<p2<<p3<<p4<<p5<<p6<<p7<<p8<<p9<<p10;
	painter->drawPolygon(arrowPolygon);
	painter->restore();
}
