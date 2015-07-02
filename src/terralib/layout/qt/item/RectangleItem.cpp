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
  \file RectangleItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "RectangleItem.h"
#include "../../core/pattern/mvc/ItemController.h"
#include "../../core/AbstractScene.h"
#include "../../core/pattern/mvc/Observable.h"
#include "../../../color/RGBAColor.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../geometry/Envelope.h"
#include "../../../common/STLUtils.h"
#include "../../item/RectangleModel.h"
#include "../../core/enum/EnumRectangleType.h"

// Qt
#include <QColor>
#include <QPen>
#include <QRectF>

te::layout::RectangleItem::RectangleItem( ItemController* controller, Observable* o, bool invertedMatrix ) :
  ObjectItem(controller, o, invertedMatrix)
{  
  m_nameClass = std::string(this->metaObject()->className());
}

te::layout::RectangleItem::~RectangleItem()
{

}

void te::layout::RectangleItem::drawItem( QPainter * painter )
{
  RectangleModel* model = dynamic_cast<RectangleModel*>(m_model);

  if(model)
  {
    EnumRectangleType* enumScale = model->getEnumRectangleType();

    if(model->getCurrentRectangleType() == enumScale->getSimpleRectangleType())
    {
      drawRectangle(painter);
    }
    if(model->getCurrentRectangleType() == enumScale->getRoundedRetangleType())
    {
      drawRoundedRectangle(painter);
    }

    if(model->getCurrentRectangleType() == enumScale->getSingleCornerTrimmedRectangleType())
    {
      drawSingleCornerTrimmedRectangle(painter);
    }
  }
}

void te::layout::RectangleItem::drawRectangle( QPainter * painter )
{
  RectangleModel* model = dynamic_cast<RectangleModel*>(m_model);
  if(!model)
  {
    return;
  }

  painter->save();

  const te::color::RGBAColor& fillColor = model->getBackgroundColor();
  const te::color::RGBAColor& contourColor = model->getFrameColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);

  //gets the adjusted boundigng rectangle based of the painter settings
  QRectF rectAdjusted = getAdjustedBoundingRect(painter);

  QPainterPath rect_path;
  rect_path.addRect(rectAdjusted);

  //draws the item
  painter->drawPath(rect_path);

  painter->restore();
}

void te::layout::RectangleItem::drawRoundedRectangle(QPainter * painter)
{
  RectangleModel* model = dynamic_cast<RectangleModel*>(m_model);
  if(!model)
  {
    return;
  }
  painter->save();

  const te::color::RGBAColor& fillColor = model->getBackgroundColor();
  const te::color::RGBAColor& contourColor = model->getFrameColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);

  //gets the adjusted boundigng rectangle based of the painter settings
  QRectF rectAdjusted = getAdjustedBoundingRect(painter);

  QPainterPath rect_path;
  rect_path.addRoundRect(rectAdjusted,30,30);

  //draws the item
  painter->drawPath(rect_path);

  painter->restore();
}

void te::layout::RectangleItem::drawSingleCornerTrimmedRectangle(QPainter * painter)
{
  RectangleModel* model = dynamic_cast<RectangleModel*>(m_model);
  if(!model)
  {
    return;
  }
  painter->save();

  const te::color::RGBAColor& fillColor = model->getBackgroundColor();
  const te::color::RGBAColor& contourColor = model->getFrameColor();

  QColor qFillColor(fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
  QColor qContourColor(contourColor.getRed(), contourColor.getGreen(), contourColor.getBlue(), contourColor.getAlpha());

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);

  //gets the adjusted boundigng rectangle based of the painter settings
  QRectF rectAdjusted = getAdjustedBoundingRect(painter);

  QPointF p1 = QPointF(rectAdjusted.width() - rectAdjusted.width() / 4., rectAdjusted.center().y()+ rectAdjusted.height() / 2.);
  QPointF p2 = QPointF(rectAdjusted.height()-rectAdjusted.topRight().y(),rectAdjusted.topRight().x()-rectAdjusted.width()/4);
  QPointF p3 = QPointF(rectAdjusted.bottomRight().y(),rectAdjusted.top());
  QPointF p4 = QPointF(rectAdjusted.bottomLeft().x(),rectAdjusted.top());
  QPointF p5 = QPointF(rectAdjusted.bottomLeft().x(),rectAdjusted.bottom());

  QPolygonF rect;
  rect<<p1<<p2<<p3<<p4<<p5;

  //draws the item
  painter->drawPolygon(rect);

  painter->restore();
}
