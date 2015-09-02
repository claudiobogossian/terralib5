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
  \file NorthItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "NorthItem.h"
#include "../../item/NorthModel.h"
#include "../../core/enum/EnumNorthArrowType.h"

te::layout::NorthItem::NorthItem( AbstractItemController* controller, AbstractItemModel* model, bool invertedMatrix ) :
  AbstractItem<QGraphicsItem>(controller, model)
{  
  //m_nameClass = std::string(this->metaObject()->className());
}

te::layout::NorthItem::~NorthItem()
{

}

void te::layout::NorthItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  const Property& property = m_controller->getProperty("northArrow_type");
  if(property.isNull() == false)
  {
    EnumNorthArrowType enumNorthArrowType;

    const std::string& label = property.getOptionByCurrentChoice().toString();
    EnumType* currentNorthArrowType = enumNorthArrowType.searchLabel(label);

    if(currentNorthArrowType == enumNorthArrowType.getNorthArrowType1())
    {
      drawNorthArrow1(painter);
    }
    else if (currentNorthArrowType == enumNorthArrowType.getNorthArrowType2())
    {
      drawNorthArrow2(painter);
    }
    else if(currentNorthArrowType == enumNorthArrowType.getNorthArrowType3())
    {
      drawNorthArrow3(painter);
    }
  }
}

void te::layout::NorthItem::drawNorthArrow1(QPainter * painter)
{
  painter->save();
  QColor cpen = setBrush(painter);
  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);
  QPointF p1 = QPointF(boundingRect().width() / 2.,boundingRect().center().y()+boundingRect().height()/4.);
  QPointF p2 = QPointF(boundingRect().bottomRight().x()-boundingRect().bottomRight().x()/4,boundingRect().top());
  QPointF p3 = QPointF(boundingRect().width() / 2.,boundingRect().center().y()-boundingRect().height()/4.);
  QPointF p4 = QPointF(boundingRect().bottomLeft().x()+boundingRect().bottomRight().x()/4,boundingRect().top());
  QPolygonF north;
  north<<p1<<p2<<p3<<p4;

  QPen pen(cpen, 0, Qt::SolidLine);
  pen.setWidth(1);
  QPointF p5 = QPointF(boundingRect().width()/2.2,(boundingRect().height()/3.+boundingRect().height()/2.)+boundingRect().height()/22.);
  QPointF p6 = QPointF(boundingRect().width()/2.2,(boundingRect().height()/3.+boundingRect().height()/2.)-boundingRect().height()/22.);
  QPointF p7 = QPointF(boundingRect().width()/2.2+boundingRect().width()/11,(boundingRect().height()/3.+boundingRect().height()/2.)+boundingRect().height()/22.);
  QPointF p8 = QPointF(boundingRect().width()/2.2+boundingRect().width()/11,(boundingRect().height()/3.+boundingRect().height()/2.)-boundingRect().height()/22.);

  painter->setPen(pen);
  painter->drawLine(p5,p6);
  painter->drawLine(p5,p8);
  painter->drawLine(p8,p7);

  painter->setPen(pn);
  painter->drawPolygon(north);
  painter->restore();
}
void te::layout::NorthItem::drawNorthArrow2(QPainter * painter)
{
  painter->save();
  QColor cpen = setBrush(painter);
  QPen pn(cpen, 0, Qt::SolidLine);
  
  QColor secondPolygon(255, 255, 255, 255);

  QPointF p1 = QPointF(boundingRect().width() / 2.,boundingRect().center().y()+boundingRect().height()/4.);
  QPointF p2 = QPointF(boundingRect().bottomRight().x()-boundingRect().bottomRight().x()/4,boundingRect().top());
  QPointF p3 = QPointF(boundingRect().width() / 2.,boundingRect().center().y()-boundingRect().height()/4.);
  QPointF p4 = QPointF(boundingRect().bottomLeft().x()+boundingRect().bottomRight().x()/4,boundingRect().top());

  QPen pen(cpen, 0, Qt::SolidLine);
  pen.setWidth(1);
  QPointF p5 = QPointF(boundingRect().width()/2.2,(boundingRect().height()/3.+boundingRect().height()/2.)+boundingRect().height()/22.);
  QPointF p6 = QPointF(boundingRect().width()/2.2,(boundingRect().height()/3.+boundingRect().height()/2.)-boundingRect().height()/22.);
  QPointF p7 = QPointF(boundingRect().width()/2.2+boundingRect().width()/11,(boundingRect().height()/3.+boundingRect().height()/2.)+boundingRect().height()/22.);
  QPointF p8 = QPointF(boundingRect().width()/2.2+boundingRect().width()/11,(boundingRect().height()/3.+boundingRect().height()/2.)-boundingRect().height()/22.);

  painter->setPen(pen);
  painter->drawLine(p5,p6);
  painter->drawLine(p5,p8);
  painter->drawLine(p8,p7);
  

  QPolygonF north2;
  north2<<p1<<p2<<p3;
  QPolygonF north3;
  north3<<p1<<p4<<p3;

  painter->setPen(pn);
  painter->drawPolygon(north2);
  painter->setBrush(QBrush(secondPolygon));
  painter->drawPolygon(north3);
  
  painter->restore();
}

void te::layout::NorthItem::drawNorthArrow3(QPainter * painter)
{
  painter->save();
  QColor cpen = setBrush(painter);
  QPen pn(cpen, 0, Qt::SolidLine);
  painter->setPen(pn);

  QPen pen(cpen, 0, Qt::SolidLine);
  pen.setWidth(1);
  QPointF p1 = QPointF(boundingRect().width()/2.2,(boundingRect().height()/2.5+boundingRect().height()/2.)+boundingRect().height()/22.);
  QPointF p2 = QPointF(boundingRect().width()/2.2,(boundingRect().height()/2.5+boundingRect().height()/2.)-boundingRect().height()/22.);
  QPointF p3 = QPointF(boundingRect().width()/2.2+boundingRect().width()/11,(boundingRect().height()/2.5+boundingRect().height()/2.)+boundingRect().height()/22.);
  QPointF p4 = QPointF(boundingRect().width()/2.2+boundingRect().width()/11,(boundingRect().height()/2.5+boundingRect().height()/2.)-boundingRect().height()/22.);

  painter->setPen(pen);
  painter->drawLine(p1,p2);
  painter->drawLine(p1,p4);
  painter->drawLine(p4,p3);

  QPointF p5 = QPointF(boundingRect().width()/2,boundingRect().height()/2+boundingRect().height()/2.7);
  QPointF p6 = QPointF(boundingRect().width()/2,boundingRect().height()/2-boundingRect().height()/2.5);
  QPointF p7 = QPointF(boundingRect().width()/2+boundingRect().width()/2.5,boundingRect().height()/2);
  QPointF p8 = QPointF(boundingRect().width()/2-boundingRect().width()/2.5,boundingRect().height()/2);
  QPointF p9 = QPointF(boundingRect().width()/2+boundingRect().width()/40,boundingRect().height()/2);
  QPointF p10 = QPointF(boundingRect().width()/2-boundingRect().width()/40,boundingRect().height()/2);
  QPointF p11 = QPointF(boundingRect().width()/2,boundingRect().height()/2+boundingRect().height()/40);
  QPointF p12 = QPointF(boundingRect().width()/2,boundingRect().height()/2-boundingRect().height()/40);

  QPolygonF north4;
  north4<<p5<<p9<<p10;
  north4<<p6<<p9<<p10;

  QPolygonF north5;
  north5<<p8<<p11<<p12;
  north5<<p7<<p11<<p12;

  painter->setPen(pn);
  painter->drawPolygon(north4);
  painter->drawPolygon(north5);
  painter->restore();
}

QColor te::layout::NorthItem::setBrush(QPainter* painter)
{
  const Property& colorProperty = m_controller->getProperty("color");
  const te::color::RGBAColor& color = colorProperty.getValue().toColor();
  QColor brushColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
  painter->setBrush(QBrush(brushColor));
  return brushColor;
}
