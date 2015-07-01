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
  \file SVGItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SVGItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/PointModel.h"
#include "../../core/enum/EnumPointType.h"

// STL
#include <cmath>

// Qt
#include <QColor>
#include <QPen>
#include <QPolygonF>
#include <QPainterPath>

te::layout::SVGItem::SVGItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o)
{
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::SVGItem::~SVGItem()
{

}

void te::layout::SVGItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
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

    /*if(model->getCurrentPointType() == enumScale->getStarType())
    {
      drawStar(painter);
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
    }*/
  }

  drawFrame(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

/*void te::layout::PointItem::drawStar( QPainter * painter )
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
  
  double centerX = boundingRect().center().x();
  double centerY = boundingRect().center().y();

  double halfW = boundingRect().width() / 4.;
  double halfH = boundingRect().height() / 4.;
    
  QPainterPath rect_path;

  rect_path.moveTo(halfW, centerY + halfH);
  rect_path.lineTo(centerX + halfW, halfH);
  rect_path.moveTo(halfW, centerY - halfH);
  rect_path.lineTo(centerX + halfW, centerY + halfH);
  
  te::color::RGBAColor clrPoint = model->getPointColor();

  QColor pointColor;
  pointColor.setRed(clrPoint.getRed());
  pointColor.setGreen(clrPoint.getGreen());
  pointColor.setBlue(clrPoint.getBlue());
  pointColor.setAlpha(clrPoint.getAlpha());

  QPen pn(pointColor, 1, Qt::SolidLine);
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
  
  QPolygonF poly;
  poly.push_back(QPointF(centerX, centerY + halfH));
  poly.push_back(QPointF(centerX + halfW, centerY));
  poly.push_back(QPointF(centerX, centerY - halfH));
  poly.push_back(QPointF(centerX - halfW, centerY));
  poly.push_back(QPointF(centerX, centerY + halfH));

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
    
  te::color::RGBAColor clrPoint = model->getPointColor();

  QPainterPath painterCross;

  QColor pointColor;
  pointColor.setRed(clrPoint.getRed());
  pointColor.setGreen(clrPoint.getGreen());
  pointColor.setBlue(clrPoint.getBlue());
  pointColor.setAlpha(clrPoint.getAlpha());

  QPen pn(pointColor, 0, Qt::SolidLine);
  painter->setPen(pn);

  painter->setBrush(pointColor);
  
  painterCross.moveTo(centerX - halfW, centerY);
  painterCross.lineTo(centerX + halfW, centerY);
  painterCross.moveTo(centerX, centerY + halfH);
  painterCross.lineTo(centerX, centerY - halfH);
  
  painter->drawPath(painterCross);
  painter->restore();
}*/


