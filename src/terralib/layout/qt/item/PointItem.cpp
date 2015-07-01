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
  \file PointItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PointItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/PointModel.h"
#include "../../core/enum/EnumPointType.h"

#include <cmath>
#include "qmath.h"

te::layout::PointItem::PointItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o)
{
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::PointItem::~PointItem()
{

}

void te::layout::PointItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  if(m_resizeMode)
  {
    ObjectItem::paint(painter, option, widget);
    return;
  }

  drawBackground(painter);

  PointModel* model = dynamic_cast<PointModel*>(m_model);

  if(model)
  {
    EnumPointType* enumScale = model->getEnumPointType();

    if(model->getCurrentPointType() == enumScale->getStar1Type())
    {
      drawStar1(painter);
    }

    if(model->getCurrentPointType() == enumScale->getStar2Type())
    {
      drawStar2(painter);
    }

    if(model->getCurrentPointType() == enumScale->getStar3Type())
    {
      drawStar3(painter);
    }

    if(model->getCurrentPointType() == enumScale->getStar4Type())
    {
      drawStar4(painter);
    }

    if(model->getCurrentPointType() == enumScale->getCircleType())
    {
      drawCircle(painter);
    }
    if(model->getCurrentPointType() == enumScale->getXType())
    {
      drawX(painter);
    }
    if(model->getCurrentPointType() == enumScale->getSquareType())
    {
      drawSquare(painter);
    }
    if(model->getCurrentPointType() == enumScale->getRhombusType())
    {
      drawRhombus(painter);
    }
    if(model->getCurrentPointType() == enumScale->getCrossType())
    {
      drawCross(painter);
    }
  }

  drawBorder(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::PointItem::drawStar1( QPainter * painter )
{
  PointModel* model = dynamic_cast<PointModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();

  double halfW = boundingRect().width() / 4.;

  double w = boundingRect().width() / 2.;
  
  QPainterPath rhombus_path;

  QPolygonF poly;
  qreal const c = halfW;
  qreal const d = w;
  bool inner = true;
  QPointF pUnion;
  for ( qreal i = 0 ; i < 2*3.14; i += 3.14/5.0, inner=!inner ) {
    qreal const f = inner ? c : d;
    poly << QPointF( f * std::cos(i), f * std::sin(i) );
    if(i == 0)
    {
      pUnion = QPointF( f * std::cos(i), f * std::sin(i) );
    }
  }
  poly << pUnion;
  poly.translate(boundingRect().center());

  rhombus_path.addPolygon(poly);

  te::color::RGBAColor clrPoint = model->getPointColor();

  QColor pointColor;
  pointColor.setRed(clrPoint.getRed());
  pointColor.setGreen(clrPoint.getGreen());
  pointColor.setBlue(clrPoint.getBlue());
  pointColor.setAlpha(clrPoint.getAlpha());

  QPen pn(pointColor, 0, Qt::SolidLine);
  painter->setPen(pn);

  painter->setBrush(pointColor);
  painter->drawPath(rhombus_path);

  painter->restore();
}

void te::layout::PointItem::drawStar2(QPainter * painter)
{
  PointModel* model = dynamic_cast<PointModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();

  double halfW = boundingRect().width() / 4.;
  double w = boundingRect().width() / 2.;
  QPolygonF star;
  qreal const c = halfW;
  qreal const d = w;
  bool inner = true;
  QPointF center;
  for ( qreal i = 0 ; i < 2*M_PI; i += M_PI/6.0, inner=!inner ) {
    qreal const f = inner ? c : d;
    star << QPointF( f * std::cos(i), f * std::sin(i) );
    if(i == 0)
    {
      center = QPointF( f * std::cos(i), f * std::sin(i) );
    }
  }
  star << center;
  star.translate(boundingRect().center());

  te::color::RGBAColor clrPoint = model->getPointColor();

  QColor pointColor;
  pointColor.setRed(clrPoint.getRed());
  pointColor.setGreen(clrPoint.getGreen());
  pointColor.setBlue(clrPoint.getBlue());
  pointColor.setAlpha(clrPoint.getAlpha());

  QPen pn(pointColor, 0, Qt::SolidLine);
  painter->setPen(pn);
  painter->setBrush(pointColor);
  painter->drawPolygon(star);
  painter->restore();
}

void te::layout::PointItem::drawStar3(QPainter * painter)
{
  PointModel* model = dynamic_cast<PointModel*>(m_model);
	if(!model)
	{
		return;
	}

	painter->save();

	QPolygonF star;
	double halfW = boundingRect().width() / 4.;
	double w = boundingRect().width() / 2.;
	qreal const c = halfW;
	qreal const d = w;
	bool inner = true;
	for ( qreal i=0; i<2*M_PI; i+=M_PI/7.0, inner=!inner ) {
		qreal const f = inner ? c : d;
		star << QPointF( f * cos(i), f * sin(i) );
	}
	star.translate(boundingRect().center());

	te::color::RGBAColor clrPoint = model->getPointColor();

	QColor pointColor;
	pointColor.setRed(clrPoint.getRed());
	pointColor.setGreen(clrPoint.getGreen());
	pointColor.setBlue(clrPoint.getBlue());
	pointColor.setAlpha(clrPoint.getAlpha());

	QPen pn(pointColor, 0, Qt::SolidLine);
	painter->setPen(pn);

	painter->setBrush(pointColor);
	painter->drawPolygon(star);
	painter->restore();
}

void te::layout::PointItem::drawStar4(QPainter * painter)
{
	PointModel* model = dynamic_cast<PointModel*>(m_model);
	if(!model)
	{
		return;
	}

	painter->save();

	QPolygonF star;
	double halfW = boundingRect().width() / 4.;
	double w = boundingRect().width() / 2.;
	qreal const c = halfW;
	qreal const d = w;
	bool inner = true;
	for ( qreal i=0; i<2*M_PI; i+=M_PI/8.0, inner=!inner ) {
		qreal const f = inner ? c : d;
		star << QPointF( f * cos(i), f * sin(i) );
	}
	star.translate(boundingRect().center());

	te::color::RGBAColor clrPoint = model->getPointColor();

	QColor pointColor;
	pointColor.setRed(clrPoint.getRed());
	pointColor.setGreen(clrPoint.getGreen());
	pointColor.setBlue(clrPoint.getBlue());
	pointColor.setAlpha(clrPoint.getAlpha());

	QPen pn(pointColor, 0, Qt::SolidLine);
	painter->setPen(pn);

	painter->setBrush(pointColor);
	painter->drawPolygon(star);
	painter->restore();
}


void te::layout::PointItem::drawCircle( QPainter * painter )
{
  PointModel* model = dynamic_cast<PointModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();

  double halfW = boundingRect().width() / 4.;
  double halfH = boundingRect().height() / 4.;

  double x = boundingRect().center().x() - halfW;
  double y = boundingRect().center().y() - halfH;
  double w = boundingRect().width() / 2.;
  double h = boundingRect().height() / 2.;

  QRectF pointRect(x, y, w, h);

  QPainterPath circle_path;
  circle_path.addEllipse(pointRect);

  te::color::RGBAColor clrPoint = model->getPointColor();

  QColor pointColor;
  pointColor.setRed(clrPoint.getRed());
  pointColor.setGreen(clrPoint.getGreen());
  pointColor.setBlue(clrPoint.getBlue());
  pointColor.setAlpha(clrPoint.getAlpha());

  QPen pn(pointColor, 0, Qt::SolidLine);
  painter->setPen(pn);

  painter->setBrush(pointColor);
  painter->drawPath(circle_path);

  painter->restore();
}

void te::layout::PointItem::drawX( QPainter * painter )
{
  PointModel* model = dynamic_cast<PointModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();
  
  double halfW = boundingRect().width() / 4.;
  double halfH = boundingRect().height() / 4.;

  double x = boundingRect().center().x() - halfW;
  double y = boundingRect().center().y() + halfH;
  double w = boundingRect().width() / 2.;

  QFont ft = painter->font();
  ft.setPointSizeF(w);

  QPainterPath rect_path;
  rect_path.addText(x, y, ft, "X");

  te::color::RGBAColor clrPoint = model->getPointColor();

  QColor pointColor;
  pointColor.setRed(clrPoint.getRed());
  pointColor.setGreen(clrPoint.getGreen());
  pointColor.setBlue(clrPoint.getBlue());
  pointColor.setAlpha(clrPoint.getAlpha());

  QPen pn(pointColor, 0, Qt::SolidLine);
  painter->setPen(pn);

  painter->setBrush(pointColor);
  painter->drawPath(rect_path);

  painter->restore();
}

void te::layout::PointItem::drawSquare( QPainter * painter )
{
  PointModel* model = dynamic_cast<PointModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();
  
  double halfW = boundingRect().width() / 4.;
  double halfH = boundingRect().height() / 4.;

  double x = boundingRect().center().x() - halfW;
  double y = boundingRect().center().y() - halfH;
  double w = boundingRect().width() / 2.;
  double h = boundingRect().height() / 2.;

  QRectF pointRect(x, y, w, h);

  QPainterPath rect_path;
  rect_path.addRect(pointRect);

  te::color::RGBAColor clrPoint = model->getPointColor();

  QColor pointColor;
  pointColor.setRed(clrPoint.getRed());
  pointColor.setGreen(clrPoint.getGreen());
  pointColor.setBlue(clrPoint.getBlue());
  pointColor.setAlpha(clrPoint.getAlpha());

  QPen pn(pointColor, 0, Qt::SolidLine);
  painter->setPen(pn);

  painter->setBrush(pointColor);
  painter->drawPath(rect_path);

  painter->restore();
}

void te::layout::PointItem::drawRhombus( QPainter * painter )
{
  PointModel* model = dynamic_cast<PointModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();
  
  double centerX = boundingRect().center().x();
  double centerY = boundingRect().center().y();

  double halfW = boundingRect().width() / 4.;
  double halfH = boundingRect().height() / 4.;

  double x = centerX - halfW;
  double y = centerY + halfH;
  
  QPolygonF poly;
  poly.push_back(QPoint(centerX, y));
  poly.push_back(QPoint(centerX + halfW, centerY));
  poly.push_back(QPoint(centerX, centerY - halfH));
  poly.push_back(QPoint(x, centerY));
  poly.push_back(QPoint(centerX, y));

  QPainterPath rhombus_path;
  rhombus_path.addPolygon(poly);

  te::color::RGBAColor clrPoint = model->getPointColor();

  QColor pointColor;
  pointColor.setRed(clrPoint.getRed());
  pointColor.setGreen(clrPoint.getGreen());
  pointColor.setBlue(clrPoint.getBlue());
  pointColor.setAlpha(clrPoint.getAlpha());

  QPen pn(pointColor, 0, Qt::SolidLine);
  painter->setPen(pn);

  painter->setBrush(pointColor);
  painter->drawPath(rhombus_path);

  painter->restore();
}

void te::layout::PointItem::drawCross( QPainter * painter )
{
  PointModel* model = dynamic_cast<PointModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();
  
  double centerX = boundingRect().center().x();
  double centerY = boundingRect().center().y();
  
  double halfW = boundingRect().width() / 4.;
  double halfH = boundingRect().height() / 4.;

  double x = centerX - halfW;
  double y = centerY + halfH;
  double w = boundingRect().width() / 2.;
  double h = boundingRect().height() / 2.;
    
  te::color::RGBAColor clrPoint = model->getPointColor();

  QColor pointColor;
  pointColor.setRed(clrPoint.getRed());
  pointColor.setGreen(clrPoint.getGreen());
  pointColor.setBlue(clrPoint.getBlue());
  pointColor.setAlpha(clrPoint.getAlpha());

  QPen pn(pointColor, 0, Qt::SolidLine);
  painter->setPen(pn);

  painter->setBrush(pointColor);
  painter->drawLine(x, centerY, x + w, centerY);
  painter->drawLine(centerX, y, centerX, y - h);

  painter->restore();
}



