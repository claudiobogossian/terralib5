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

  drawBackground(painter);

  PointModel* model = dynamic_cast<PointModel*>(m_model);

  if(model)
  {
    EnumPointType* enumScale = model->getEnumPointType();

    if(model->getCurrentPointType() == enumScale->getStarType())
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
    }
  }

  drawBorder(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::PointItem::drawStar( QPainter * painter )
{
  PointModel* model = dynamic_cast<PointModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();

  double half = model->getShapeSize() / 2.;

  double centerX = boundingRect().center().x();
  double centerY = boundingRect().center().y();

  double x = centerX - half;
  double y = centerY + half;
  double w = model->getShapeSize();
  double h = model->getShapeSize();
  
  QPainterPath rhombus_path;

  QPolygonF poly;
  qreal const c = half;
  qreal const d = w;
  bool inner = true;
  QPointF pUnion;
  for ( qreal i = 0 ; i < 2*3.14; i += 3.14/5.0, inner=!inner ) {
    qreal const f = inner ? c : d;
    poly << QPointF( f * cos(i), f * sin(i) );
    if(i == 0)
    {
      pUnion = QPointF( f * cos(i), f * sin(i) );
    }
  }
  poly << pUnion;
  poly.translate(boundingRect().center());

  rhombus_path.addPolygon(poly);

  QColor cpen(0,0,0);
  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);

  te::color::RGBAColor clrBack = model->getBackgroundColor();

  QColor cbrush;
  cbrush.setRed(clrBack.getRed());
  cbrush.setGreen(clrBack.getGreen());
  cbrush.setBlue(clrBack.getBlue());
  cbrush.setAlpha(clrBack.getAlpha());

  painter->setBrush(cbrush);
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

  double half = model->getShapeSize() / 2.;

  double x = boundingRect().center().x() - half;
  double y = boundingRect().center().y() - half;
  double w = model->getShapeSize();
  double h = model->getShapeSize();

  QRectF pointRect(x, y, w, h);

  QPainterPath circle_path;
  circle_path.addEllipse(pointRect);

  QColor cpen(0,0,0);
  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);

  te::color::RGBAColor clrBack = model->getBackgroundColor();

  QColor cbrush;
  cbrush.setRed(clrBack.getRed());
  cbrush.setGreen(clrBack.getGreen());
  cbrush.setBlue(clrBack.getBlue());
  cbrush.setAlpha(clrBack.getAlpha());

  painter->setBrush(cbrush);
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

  double half = model->getShapeSize() / 2.;

  double x = boundingRect().center().x() - half;
  double y = boundingRect().center().y() + half;
  double w = model->getShapeSize();
  double h = model->getShapeSize();
  
  QFont ft = painter->font();
  ft.setPointSizeF(w);

  QPainterPath rect_path;
  rect_path.addText(x, y, ft, "X");

  QColor cpen(0,0,0);
  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);

  te::color::RGBAColor clrBack = model->getBackgroundColor();

  QColor cbrush;
  cbrush.setRed(clrBack.getRed());
  cbrush.setGreen(clrBack.getGreen());
  cbrush.setBlue(clrBack.getBlue());
  cbrush.setAlpha(clrBack.getAlpha());

  painter->setBrush(cbrush);
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

  double half = model->getShapeSize() / 2.;

  double x = boundingRect().center().x() - half;
  double y = boundingRect().center().y() - half;
  double w = model->getShapeSize();
  double h = model->getShapeSize();

  QRectF pointRect(x, y, w, h);

  QPainterPath rect_path;
  rect_path.addRect(pointRect);

  QColor cpen(0,0,0);
  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);

  te::color::RGBAColor clrBack = model->getBackgroundColor();

  QColor cbrush;
  cbrush.setRed(clrBack.getRed());
  cbrush.setGreen(clrBack.getGreen());
  cbrush.setBlue(clrBack.getBlue());
  cbrush.setAlpha(clrBack.getAlpha());

  painter->setBrush(cbrush);
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

  double half = model->getShapeSize() / 2.;

  double centerX = boundingRect().center().x();
  double centerY = boundingRect().center().y();

  double x = centerX - half;
  double y = centerY + half;
  double w = model->getShapeSize();
  double h = model->getShapeSize();
  
  QPolygonF poly;
  poly.push_back(QPoint(centerX, y));
  poly.push_back(QPoint(centerX + half, centerY));
  poly.push_back(QPoint(centerX, centerY - half));
  poly.push_back(QPoint(x, centerY));
  poly.push_back(QPoint(centerX, y));

  QPainterPath rhombus_path;
  rhombus_path.addPolygon(poly);

  QColor cpen(0,0,0);
  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);

  te::color::RGBAColor clrBack = model->getBackgroundColor();

  QColor cbrush;
  cbrush.setRed(clrBack.getRed());
  cbrush.setGreen(clrBack.getGreen());
  cbrush.setBlue(clrBack.getBlue());
  cbrush.setAlpha(clrBack.getAlpha());

  painter->setBrush(cbrush);
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

  double half = model->getShapeSize() / 2.;

  double centerX = boundingRect().center().x();
  double centerY = boundingRect().center().y();

  double x = centerX - half;
  double y = centerY + half;
  double w = model->getShapeSize();
  double h = model->getShapeSize();
    
  QColor cpen(0,0,0);
  QPen pn(cpen);
  painter->setPen(pn);

  te::color::RGBAColor clrBack = model->getBackgroundColor();

  QColor cbrush;
  cbrush.setRed(clrBack.getRed());
  cbrush.setGreen(clrBack.getGreen());
  cbrush.setBlue(clrBack.getBlue());
  cbrush.setAlpha(clrBack.getAlpha());

  painter->setBrush(cbrush);
  painter->drawLine(x, centerY, x + w, centerY);
  painter->drawLine(centerX, y, centerX, y - h);

  painter->restore();
}


