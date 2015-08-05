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
#include "../../../color/RGBAColor.h"
#include "../../core/enum/EnumArrowType.h"

// Qt
#include <QPointF>
#include "qglobal.h"

te::layout::ArrowItem::ArrowItem( AbstractItemController* controller, AbstractItemModel* model, bool invertedMatrix ) 
  : AbstractItem<QGraphicsItem>(controller, model)
{
}

te::layout::ArrowItem::~ArrowItem()
{

}

void te::layout::ArrowItem::drawItem( QPainter * painter )
{
  const Property& property = m_controller->getProperty("arrow_type");
  if(property.isNull() == false)
  {
    EnumArrowType enumArrowType;

    const std::string& label = property.getOptionByCurrentChoice().toString();
    EnumType* currentArrowType = enumArrowType.searchLabel(label);

    if(currentArrowType == enumArrowType.getDoubleArrowType())
    {
      drawDoubleArrow(painter);
    }
    if(currentArrowType == enumArrowType.getRightArrowType())
    {
      drawRightArrow(painter);
    }
    if(currentArrowType == enumArrowType.getLeftArrowType())
    {
      drawLeftArrow(painter);
    }
  }
}

void te::layout::ArrowItem::drawRightArrow( QPainter * painter )
{
  painter->save();

  const Property& pFillColor = m_controller->getProperty("fill_color");
  const Property& pContourColor = m_controller->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );

  //gets the adjusted boundigng rectangle based of the painter settings
  QRectF rectAdjusted = getAdjustedBoundingRect(painter);

  double w1 = rectAdjusted.width() / 2.;
  double w2 = rectAdjusted.width() / 100.;
  double h1 = rectAdjusted.height() / 2.;
  double h2 = rectAdjusted.height() / 4.;
  double y1 = rectAdjusted.center().y()-h1;
  double y2 = rectAdjusted.center().y()+h1;
  double y3 = rectAdjusted.center().y()-h2;
  double y4 = rectAdjusted.center().y()+h2;
  double y5 = rectAdjusted.width() - w2;

  QPointF p1 = QPointF(w1,y2);
  QPointF p2 = QPointF(y5,h1);
  QPointF p3 = QPointF(w1,y1);
  QPointF p4 = QPointF(w1,y3);
  QPointF p5 = QPointF(w2,y3);
  QPointF p6 = QPointF(w2,y4);
  QPointF p7 = QPointF(w1,y4);
  QPolygonF arrowPolygon;
  arrowPolygon<<p1<<p2<<p3<<p4<<p5<<p6<<p7;

  //draws the item
  painter->drawPolygon(arrowPolygon);

  painter->restore();
}

void te::layout::ArrowItem::drawLeftArrow(QPainter * painter)
{
  painter->save();

  const Property& pFillColor = m_controller->getProperty("fill_color");
  const Property& pContourColor = m_controller->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );

  //gets the adjusted boundigng rectangle based of the painter settings
  QRectF rectAdjusted = getAdjustedBoundingRect(painter);

  double w2 = rectAdjusted.width() / 2.;
  double w3 = rectAdjusted.width() - rectAdjusted.width() / 100.;
  double w4 = rectAdjusted.width() / 100.;
  double h1 = rectAdjusted.height() / 2.;
  double h2 = rectAdjusted.height() / 4.;
  double y1 = rectAdjusted.center().y()-h1;
  double y2 = rectAdjusted.center().y()+h1;
  double y4 = rectAdjusted.center().y()+h2;
  double y5 = rectAdjusted.width() - w4;
  double y6 = rectAdjusted.center().y() + w4;
  
  QPointF p1 = QPointF(w3,y4);
  QPointF p2 = QPointF(w2,y4);
  QPointF p3 = QPointF(w2,y2);
  QPointF p4 = QPointF(w4,y6);
  QPointF p5 = QPointF(w2,y1);
  QPointF p6 = QPointF(w2,h2);
  QPointF p7 = QPointF(w3,h2);
  QPolygonF arrowPolygon;
  arrowPolygon<<p1<<p2<<p3<<p4<<p5<<p6<<p7;

  //draws the item
  painter->drawPolygon(arrowPolygon);

  painter->restore();
}

void te::layout::ArrowItem::drawDoubleArrow(QPainter * painter)
{
  painter->save();

  const Property& pFillColor = m_controller->getProperty("fill_color");
  const Property& pContourColor = m_controller->getProperty("contour_color");

  const te::color::RGBAColor& fillColor = pFillColor.getValue().toColor();
  const te::color::RGBAColor& contourColor = pContourColor.getValue().toColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->setRenderHint( QPainter::Antialiasing, true );

  //gets the adjusted boundigng rectangle based of the painter settings
  QRectF rectAdjusted = getAdjustedBoundingRect(painter);

  double w2 = rectAdjusted.width() / 4.;
  double w3 = rectAdjusted.width() - rectAdjusted.width() / 4.;
  double w4 = rectAdjusted.width() / 100.;
  double h1 = rectAdjusted.height() / 2.;
  double h2 = rectAdjusted.height() / 4.;
  double y1 = rectAdjusted.center().y()-h1;
  double y2 = rectAdjusted.center().y()+h1;
  double y4 = rectAdjusted.center().y()+h2;
  double y5 = rectAdjusted.width() - w4;
  double y6 = rectAdjusted.center().y() + w4;

  QPointF p1 = QPointF(w3,y2);
  QPointF p2 = QPointF(y5,h1);
  QPointF p3 = QPointF(w3,y1);
  QPointF p4 = QPointF(w3,h2);
  QPointF p5 = QPointF(w2,h2); 
  QPointF p6 = QPointF(w2,y1);
  QPointF p7 = QPointF(w4,y6);
  QPointF p8 = QPointF(w2,y2);
  QPointF p9 = QPointF(w2,y4);
  QPointF p10 = QPointF(w3,y4);
  QPolygonF arrowPolygon;
  arrowPolygon<<p1<<p2<<p3<<p4<<p5<<p6<<p7<<p8<<p9<<p10;

  //draws the item
  painter->drawPolygon(arrowPolygon);

  painter->restore();
}
