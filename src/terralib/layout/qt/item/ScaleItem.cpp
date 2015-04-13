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
  \file LegendItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ScaleItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../core/Scene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/ScaleModel.h"

// STL
#include <string>
#include <sstream> // std::stringstream

// Qt
#include <QPixmap>

te::layout::ScaleItem::ScaleItem( ItemController* controller, Observable* o ) :
  ObjectItem(controller, o)
{  
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::ScaleItem::~ScaleItem()
{

}

void te::layout::ScaleItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */ )
{
  Q_UNUSED( option );
  Q_UNUSED( widget );
  if ( !painter )
  {
    return;
  }

  drawBackground(painter);

  ScaleModel* model = dynamic_cast<ScaleModel*>(m_model);

  if(model)
  {
    EnumScaleType* enumScale = model->getEnumScaleType();

    if(model->getCurrentScaleType() == enumScale->getDoubleAlternatingScaleBarType())
    {
      drawDoubleAlternatingScaleBar(painter);
    }
    if(model->getCurrentScaleType() == enumScale->getAlternatingScaleBarType())
    {
      drawAlternatingScaleBar(painter);
    }
    if(model->getCurrentScaleType() == enumScale->getHollowScaleBarType())
    {
      drawHollowScaleBar(painter);
    }
  }

  drawBorder(painter);

  //Draw Selection
  if (option->state & QStyle::State_Selected)
  {
    drawSelection(painter);
  }
}

void te::layout::ScaleItem::drawDoubleAlternatingScaleBar( QPainter * painter )
{
  QRectF boundRect = boundingRect();
  
  painter->save();

  double			unit=1000.0;
  std::string strUnit="(Km)";

  ScaleModel* model = dynamic_cast<ScaleModel*>(m_model);

  if(model->getMapScale() < 1000)
  {
    unit = 1.0;
    strUnit="(m)";
  }
  else 
  {
    unit = 1000.0;
  }

  //convert millimeters to centimeters
  double mmToCm = model->getScaleGapX()/10;

  double spacing = model->getMapScale()/100;
  
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

  double gapX = model->getScaleGapX();
  double gapY = model->getScaleGapY();
    
  for( ; x1 < boundRect.topRight().x(); x1 += width)
  {
    if(x1+gapX >= boundRect.topRight().x())
    {
      //Draw the remaining rects, near the end
      double dx = boundRect.width() - x1;
      gapX = dx;
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

  newBoxSecond = QRectF(boundRect.x(), boundRect.center().y() - gapY, boundRect.width(), gapY*2);

  //Rect around scale
  QPen penBackground(black, 0, Qt::SolidLine);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(penBackground);
  painter->drawRect(newBoxSecond);

  //middle-bottom text
  double centerX = newBoxSecond.center().x();  
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

  ScaleModel* model = dynamic_cast<ScaleModel*>(m_model);

  if(model->getMapScale() < 1000)
  {
    unit = 1.0;
    strUnit="(m)";
  }
  else 
  {
    unit = 1000.0;
  }

  //convert millimeters to centimeters
  double mmToCm = model->getScaleGapX()/10;

  double spacing = model->getMapScale()/100;

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

  double gapX = model->getScaleGapX();
  double gapY = model->getScaleGapY();

  for( ; x1 < boundRect.topRight().x(); x1 += width)
  {
    if(x1+gapX >= boundRect.topRight().x())
    {
      //Draw the remaining rects, near the end
      double dx = boundRect.width() - x1;
      gapX = dx;
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

  newBoxSecond = QRectF(boundRect.x(), boundRect.center().y() - gapY/2, boundRect.width(), gapY);

  //Rect around scale
  QPen penBackground(black, 0, Qt::SolidLine);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(penBackground);
  painter->drawRect(newBoxSecond);

  //middle-bottom text
  double centerX = newBoxSecond.center().x();  
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

  ScaleModel* model = dynamic_cast<ScaleModel*>(m_model);

  if(model->getMapScale() < 1000)
  {
    unit = 1.0;
    strUnit="(m)";
  }
  else 
  {
    unit = 1000.0;
  }

  //convert millimeters to centimeters
  double mmToCm = model->getScaleGapX()/10;

  double spacing = model->getMapScale()/100;

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

  double gapX = model->getScaleGapX();
  double gapY = model->getScaleGapY();
  
  //Rect around scale
  QPen pn(black, 0, Qt::SolidLine);

  for( ; x1 < boundRect.topRight().x(); x1 += width)
  {
    if(x1+gapX >= boundRect.topRight().x())
    {
      //Draw the remaining rects, near the end
      double dx = boundRect.width() - x1;
      gapX = dx;
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


  newBoxSecond = QRectF(boundRect.x(), boundRect.center().y() - gapY/2, boundRect.width(), gapY);

  QPen penBackground(black, 0, Qt::SolidLine);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(penBackground);
  painter->drawRect(newBoxSecond);

  //middle-bottom text
  double centerX = newBoxSecond.center().x();  
  painter->setPen(QPen(textColor));

  QPointF coordText(centerX, boundRect.topLeft().y() + 1); 
  drawText(coordText, painter, strUnit);

  painter->restore();
}




