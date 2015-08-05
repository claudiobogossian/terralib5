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
  \file LegendItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ScaleItem.h"
#include "../../../color/RGBAColor.h"
#include "../../core/enum/EnumScaleType.h"

te::layout::ScaleItem::ScaleItem( AbstractItemController* controller, AbstractItemModel* model, bool invertedMatrix )
  : AbstractItem<QGraphicsItem>(controller, model)
{  
}

te::layout::ScaleItem::~ScaleItem()
{

}

void te::layout::ScaleItem::drawItem( QPainter * painter )
{
  const Property& property = m_controller->getProperty("scale_type");
  if(property.isNull() == false)
  {
    EnumScaleType enumScale;

    const std::string& label = property.getOptionByCurrentChoice().toString();
    EnumType* currentScaleType = enumScale.searchLabel(label);

    if(currentScaleType == enumScale.getDoubleAlternatingScaleBarType())
    {
      drawDoubleAlternatingScaleBar(painter);
    }
    if(currentScaleType == enumScale.getAlternatingScaleBarType())
    {
      drawAlternatingScaleBar(painter);
    }
    if(currentScaleType == enumScale.getHollowScaleBarType())
    {
      drawHollowScaleBar(painter);
    }
  }
}

void te::layout::ScaleItem::drawDoubleAlternatingScaleBar( QPainter * painter )
{
  QRectF boundRect = boundingRect();
  
  painter->save();

  double unit=1000.0;
  std::string strUnit="(Km)";

  const Property& pScale = m_controller->getProperty("scale");
  const Property& pScaleGapX = m_controller->getProperty("scale_width_rect_gap");
  const Property& pScaleGapY = m_controller->getProperty("scale_height_rect_gap");
  
  double scale = pScale.getValue().toDouble();
  double gapX = pScaleGapX.getValue().toDouble();
  double gapY = pScaleGapY.getValue().toDouble();

  if(scale < 1000)
  {
    unit = 1.0;
    strUnit="(m)";
  }
  else 
  {
    unit = 1000.0;
  }

  //convert millimeters to centimeters
  double mmToCm = gapX/10.;

  double spacing = scale/100.;
  
  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  QRectF newBoxFirst;
  QRectF newBoxSecond;

  for( ; x1 < boundRect.topRight().x(); x1 += width)
  {
    if(x1+gapX >= boundRect.topRight().x())
    {
      //No draw the remaining rects, near the end
      break;
    }

    painter->setPen(Qt::NoPen);

    //Down rect
    painter->setBrush(QBrush(secondRect));
    newBoxSecond = QRectF(x1, boundRect.center().y() - gapY, gapX, gapY);
    painter->drawRect(newBoxSecond);

    //Up rect
    painter->setBrush(QBrush(firstRect));
    newBoxFirst = QRectF(x1, boundRect.center().y(), gapX, gapY);
    painter->drawRect(newBoxFirst);

    if(width == 0)
      width = gapX;
    else
      value += (spacing * mmToCm)/unit;

    std::stringstream ss_value;
    ss_value << value;

    std::string s_value = ss_value.str();

    painter->setPen(QPen(textColor));
    QPointF coordText(x1, newBoxSecond.topLeft().y() - 5);

    drawText(coordText, painter, ss_value.str());

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;
  }
  
  QRectF rectScale = QRectF(boundRect.x(), boundRect.center().y() - gapY, boundRect.x() + newBoxSecond.right(), gapY*2);

  //Rect around scale
  QPen penBackground(black, 0, Qt::SolidLine);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(penBackground);
  painter->drawRect(rectScale);

  //middle-bottom text
  double centerX = rectScale.center().x();
  painter->setPen(QPen(textColor));
    
  QPointF coordText(centerX, boundRect.topLeft().y() + 1);
  drawText(coordText, painter, strUnit);

  painter->restore();
}

void te::layout::ScaleItem::drawAlternatingScaleBar( QPainter * painter )
{
  QRectF boundRect = boundingRect();

  painter->save();

  double			unit=1000.0;
  std::string strUnit="(Km)";

  const Property& pScale = m_controller->getProperty("scale");
  const Property& pScaleGapX = m_controller->getProperty("scale_width_rect_gap");
  const Property& pScaleGapY = m_controller->getProperty("scale_height_rect_gap");
  
  double scale = pScale.getValue().toDouble();
  double gapX = pScaleGapX.getValue().toDouble();
  double gapY = pScaleGapY.getValue().toDouble();

  if(scale < 1000)
  {
    unit = 1.0;
    strUnit="(m)";
  }
  else 
  {
    unit = 1000.0;
  }

  //convert millimeters to centimeters
  double mmToCm = gapX/10;

  double spacing = scale/100;

  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  QRectF newBoxFirst;
  QRectF newBoxSecond;

  for( ; x1 < boundRect.topRight().x(); x1 += width)
  {
    if(x1+gapX >= boundRect.topRight().x())
    {
      //No draw the remaining rects, near the end
      break;
    }

    painter->setPen(Qt::NoPen);

    painter->setBrush(QBrush(secondRect));
    newBoxSecond = QRectF(x1, boundRect.center().y() - gapY/2, gapX, gapY);
    painter->drawRect(newBoxSecond);


    if(width == 0)
      width = gapX;
    else
      value += (spacing * mmToCm)/unit;

    std::stringstream ss_value;
    ss_value << value;

    std::string s_value = ss_value.str();

    QPen pn(textColor);
    pn.setWidthF(0.5);
    painter->setPen(pn);
    QPointF coordText(x1, newBoxSecond.topLeft().y() - 5);

    drawText(coordText, painter, ss_value.str());

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;
  }

  QRectF rectScale = QRectF(boundRect.x(), boundRect.center().y() - gapY/2, boundRect.x() + newBoxSecond.right(), gapY);

  //Rect around scale
  QPen penBackground(black, 0, Qt::SolidLine);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(penBackground);
  painter->drawRect(rectScale);

  //middle-bottom text
  double centerX = rectScale.center().x();  
  painter->setPen(QPen(textColor));
  
  QPointF coordText(centerX, boundRect.topLeft().y() + 1); 
  drawText(coordText, painter, strUnit);

  painter->restore();
}

void te::layout::ScaleItem::drawHollowScaleBar( QPainter * painter )
{
  QRectF boundRect = boundingRect();

  painter->save();

  double			unit=1000.0;
  std::string strUnit="(Km)";

  const Property& pScale = m_controller->getProperty("scale");
  const Property& pScaleGapX = m_controller->getProperty("scale_width_rect_gap");
  const Property& pScaleGapY = m_controller->getProperty("scale_height_rect_gap");
  
  double scale = pScale.getValue().toDouble();
  double gapX = pScaleGapX.getValue().toDouble();
  double gapY = pScaleGapY.getValue().toDouble();

  if(scale < 1000)
  {
    unit = 1.0;
    strUnit="(m)";
  }
  else 
  {
    unit = 1000.0;
  }

  //convert millimeters to centimeters
  double mmToCm = gapX/10;

  double spacing = scale/100;

  double value = 0.;
  double width = 0.;
  double x1 = boundRect.bottomLeft().x();

  QColor black(0, 0, 0, 255);
  QColor white(255, 255, 255, 255);
  QColor firstRect = black;
  QColor secondRect = white;
  QColor changeColor;
  QColor textColor(0, 0, 0, 255);

  QRectF newBoxFirst;
  QRectF newBoxSecond;

  //Rect around scale
  QPen pn(black, 0, Qt::SolidLine);

  for( ; x1 < boundRect.topRight().x(); x1 += width)
  {
    if(x1+gapX >= boundRect.topRight().x())
    {
      //No draw the remaining rects, near the end
      break;
    }

    painter->setPen(Qt::NoPen);

    painter->setBrush(QBrush(white));
    newBoxSecond = QRectF(x1, boundRect.center().y() - gapY/2, gapX, gapY);
    painter->drawRect(newBoxSecond);

    painter->setPen(pn);

    QLineF lne(x1, boundRect.center().y(), gapX, boundRect.center().y());
    painter->drawLine(lne);

    if(width == 0)
      width = gapX;
    else
      value += (spacing * mmToCm)/unit;

    std::stringstream ss_value;
    ss_value << value;

    std::string s_value = ss_value.str();

    painter->setPen(QPen(textColor));
    QPointF coordText(x1, newBoxSecond.topLeft().y() - 5);

    drawText(coordText, painter, ss_value.str());

    changeColor = firstRect;
    firstRect = secondRect;
    secondRect = changeColor;
  }

  QRectF rectScale = QRectF(boundRect.x(), boundRect.center().y() - gapY/2, boundRect.x() + newBoxSecond.right(), gapY);

  QPen penBackground(black, 0, Qt::SolidLine);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(penBackground);
  painter->drawRect(rectScale);

  //middle-bottom text
  double centerX = rectScale.center().x();  
  painter->setPen(QPen(textColor));

  QPointF coordText(centerX, boundRect.topLeft().y() + 1); 
  drawText(coordText, painter, strUnit);

  painter->restore();
}
