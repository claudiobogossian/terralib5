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
#include "../../../color/RGBAColor.h"
#include "../../core/enum/EnumPointType.h"


// Qt
#include <QColor>
#include <QPen>
#include <QPolygonF>
#include <QPainterPath>
#include "geos/platform.h"

te::layout::PointItem::PointItem( AbstractItemController* controller, AbstractItemModel* model, bool invertedMatrix ) 
  : AbstractItem<QGraphicsItem>(controller, model)
{
}

te::layout::PointItem::~PointItem()
{

}

void te::layout::PointItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  const Property& property = m_model->getProperty("point_type");
  if(property.isNull() == false)
  {
    EnumPointType enumPointType;

    const std::string& label = property.getOptionByCurrentChoice().toString();
    EnumType* currentPointType = enumPointType.searchLabel(label);

    if(currentPointType == enumPointType.getStar1Type())
    {
      drawStar1(painter);
    }
    if(currentPointType == enumPointType.getStar2Type())
    {
      drawStar2(painter);
    }
    if(currentPointType == enumPointType.getStar3Type())
    {
      drawStar3(painter);
    }
    if(currentPointType == enumPointType.getStar4Type())
    {
      drawStar4(painter);
    }
    if(currentPointType == enumPointType.getCircleType())
    {
      drawCircle(painter);
    }
    if(currentPointType == enumPointType.getCrossType())
    {
      drawCross(painter);
    }
    if(currentPointType == enumPointType.getRhombusType())
    {
      drawRhombus(painter);
    }
    if(currentPointType == enumPointType.getSquareType())
    {
      drawSquare(painter);
    }
    if(currentPointType == enumPointType.getXType())
    {
      drawX(painter);
    }
  }
}

void te::layout::PointItem::drawStar1( QPainter * painter )
{
  painter->save();

  const Property& pFillColor = m_model->getProperty("fill_color");
  const Property& pContourColor = m_model->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );

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

  painter->drawPath(rhombus_path);
  painter->restore();
}

void te::layout::PointItem::drawStar2(QPainter * painter)
{
  painter->save();

  const Property& pFillColor = m_model->getProperty("fill_color");
  const Property& pContourColor = m_model->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );

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

  painter->drawPolygon(star);
  painter->restore();
}

void te::layout::PointItem::drawStar3(QPainter * painter)
{
  painter->save();

  const Property& pFillColor = m_model->getProperty("fill_color");
  const Property& pContourColor = m_model->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );

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

	painter->drawPolygon(star);
	painter->restore();
}

void te::layout::PointItem::drawStar4(QPainter * painter)
{
  painter->save();

  const Property& pFillColor = m_model->getProperty("fill_color");
  const Property& pContourColor = m_model->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );

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

	painter->drawPolygon(star);
	painter->restore();
}


void te::layout::PointItem::drawCircle( QPainter * painter )
{
  painter->save();

  const Property& pFillColor = m_model->getProperty("fill_color");
  const Property& pContourColor = m_model->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );

  double halfW = boundingRect().width() / 4.;
  double halfH = boundingRect().height() / 4.;

  double x = boundingRect().center().x() - halfW;
  double y = boundingRect().center().y() - halfH;
  double w = boundingRect().width() / 2.;
  double h = boundingRect().height() / 2.;

  QRectF pointRect(x, y, w, h);

  QPainterPath circle_path;
  circle_path.addEllipse(pointRect);
  painter->drawPath(circle_path);

  painter->restore();
}

void te::layout::PointItem::drawX( QPainter * painter )
{
  painter->save();

  const Property& pFillColor = m_model->getProperty("fill_color");
  const Property& pContourColor = m_model->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );
  
  double centerX = boundingRect().center().x();
  double centerY = boundingRect().center().y();

  double halfW = boundingRect().width() / 4.;
  double halfH = boundingRect().height() / 4.;
    
  QPainterPath rect_path;

  rect_path.moveTo(halfW, centerY + halfH);
  rect_path.lineTo(centerX + halfW, halfH);
  rect_path.moveTo(halfW, centerY - halfH);
  rect_path.lineTo(centerX + halfW, centerY + halfH);
 
  painter->drawPath(rect_path);

  painter->restore();
}

void te::layout::PointItem::drawSquare( QPainter * painter )
{
  painter->save();

  const Property& pFillColor = m_model->getProperty("fill_color");
  const Property& pContourColor = m_model->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );
  
  double halfW = boundingRect().width() / 4.;
  double halfH = boundingRect().height() / 4.;

  double x = boundingRect().center().x() - halfW;
  double y = boundingRect().center().y() - halfH;
  double w = boundingRect().width() / 2.;
  double h = boundingRect().height() / 2.;

  QRectF pointRect(x, y, w, h);

  QPainterPath rect_path;
  rect_path.addRect(pointRect);

  painter->drawPath(rect_path);

  painter->restore();
}

void te::layout::PointItem::drawRhombus( QPainter * painter )
{
  painter->save();

  const Property& pFillColor = m_model->getProperty("fill_color");
  const Property& pContourColor = m_model->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );

  double centerX = boundingRect().center().x();
  double centerY = boundingRect().center().y();

  double halfW = boundingRect().width() / 4.;
  double halfH = boundingRect().height() / 4.;
  
  QPolygonF poly;
  poly.push_back(QPointF(centerX, centerY + halfH));
  poly.push_back(QPointF(centerX + halfW, centerY));
  poly.push_back(QPointF(centerX, centerY - halfH));
  poly.push_back(QPointF(centerX - halfW, centerY));
  poly.push_back(QPointF(centerX, centerY + halfH));

  QPainterPath rhombus_path;
  rhombus_path.addPolygon(poly);

  painter->drawPath(rhombus_path);

  painter->restore();
}

void te::layout::PointItem::drawCross( QPainter * painter )
{
  painter->save();

  const Property& pFillColor = m_model->getProperty("fill_color");
  const Property& pContourColor = m_model->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );

  double centerX = boundingRect().center().x();
  double centerY = boundingRect().center().y();
  
  double halfW = boundingRect().width() / 4.;
  double halfH = boundingRect().height() / 4.;

  QPainterPath painterCross;
  
  painterCross.moveTo(centerX - halfW, centerY);
  painterCross.lineTo(centerX + halfW, centerY);
  painterCross.moveTo(centerX, centerY + halfH);
  painterCross.lineTo(centerX, centerY - halfH);
  
  painter->drawPath(painterCross);
  painter->restore();
}



