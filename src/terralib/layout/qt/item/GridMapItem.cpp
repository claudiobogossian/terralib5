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
  \file GridMapItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridMapItem.h"

#include "../../core/property/GridSettingsConfigProperties.h"
#include "../core/ItemUtils.h"

te::layout::GridMapItem::GridMapItem(AbstractItemController* controller, bool invertedMatrix)
  : AbstractItem<QGraphicsItem>(controller, invertedMatrix)
  , m_maxWidthTextMM(0)
  , m_maxHeigthTextMM(0)
  , m_onePointMM(0.3527777778)
  , m_changeSize(false)
{  
  //The text size or length that exceeds the sides will be cut
  setFlag(QGraphicsItem::ItemClipsToShape);
}

te::layout::GridMapItem::~GridMapItem()
{

}

void te::layout::GridMapItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  GridSettingsConfigProperties settingsConfig;

  const Property& pVisible = m_controller->getProperty(settingsConfig.getVisible());
  bool visible = pVisible.getValue().toBool();

  if(visible)
  {
    drawGrid(painter);
  }
  else
  {
    const Property& pSRID = m_controller->getProperty("srid");
    if (pSRID.getValue().toInt() <= 0)
    {
      drawDefaultGrid(painter);
    }
  }
}

void te::layout::GridMapItem::drawGrid( QPainter* painter )
{

}

void te::layout::GridMapItem::drawDefaultGrid( QPainter* painter )
{
  GridSettingsConfigProperties settingsConfig;

  //gets the properties
  const Property& pFontFamily = m_controller->getProperty(settingsConfig.getFontText());
  const Property& pTextPointSize = m_controller->getProperty(settingsConfig.getPointTextSize());

  std::string fontFamily = pFontFamily.getValue().toString();
  double pointTextSize = pTextPointSize.getValue().toDouble();

  painter->save();

  QRectF parentBound = boundingRect();
  
  QPainterPath gridMapPath;
  gridMapPath.setFillRule(Qt::WindingFill);

  int heightRect = (int)parentBound.height();
  int widgetRect = (int)parentBound.width();

  ItemUtils::ConfigurePainterForTexts(painter, fontFamily, pointTextSize);

  // PostScript to mm
  QFont ft(fontFamily.c_str(), pointTextSize);
  m_maxHeigthTextMM = m_onePointMM * ft.pointSize();

  QString text = "GRID";

  for (int i = 0; i <= heightRect; i+=10)
  {
    QLineF lineOne = QLineF(parentBound.topLeft().x(), parentBound.topLeft().y() + i, parentBound.topRight().x(), parentBound.topRight().y() + i);

    configTextPainter(painter);

    QPointF pointInit(parentBound.topLeft().x(), parentBound.topLeft().y() + i - (m_maxHeigthTextMM/2)); //left
    drawText(pointInit, painter, text.toStdString(), true);
    QPointF pointFinal(parentBound.topRight().x(), parentBound.topRight().y() + i  - (m_maxHeigthTextMM/2)); //right
    drawText(pointFinal, painter, text.toStdString());

    configPainter(painter);

    painter->drawLine(lineOne);

    for (int j = 0; j <= widgetRect; j+=10)
    {
      QLineF lineTwo = QLineF(parentBound.topLeft().x() + j, parentBound.topLeft().y(), parentBound.bottomLeft().x() + j, parentBound.bottomLeft().y());

      configTextPainter(painter);

      QPointF pointInit(parentBound.topLeft().x() + j + (m_maxWidthTextMM/2), boundingRect().topLeft().y() + (m_maxHeigthTextMM)); //lower
      drawText(pointInit, painter, text.toStdString(), true);
      QPointF pointFinal(parentBound.bottomLeft().x() + j  - (m_maxWidthTextMM/2), parentBound.bottomLeft().y()); //upper
      drawText(pointFinal, painter, text.toStdString());

      configPainter(painter);

      painter->drawLine(lineTwo);
    }    
  }

  painter->restore();
}

void te::layout::GridMapItem::drawText( const QPointF& point, QPainter* painter, const std::string& text, bool displacementLeft, bool displacementRight )
{
  const QFont& font = painter->font();

  ItemUtils* itemUtils = Context::getInstance().getItemUtils();
  QRectF rectF = itemUtils->getTextBoundary(font.family().toStdString(), font.pixelSize(), text);

  QPointF newPoint (point);
  if(displacementLeft)
  {
    newPoint.setX(newPoint.x() - rectF.width());
  }

  if(displacementRight)
  {
    newPoint.setX(newPoint.x() + rectF.width());
  }

  AbstractItem<QGraphicsItem>::drawText(newPoint, painter, text);
}

void te::layout::GridMapItem::configPainter( QPainter* painter )
{
  GridSettingsConfigProperties settingsConfig;

  const Property& pLineStyle = m_controller->getProperty(settingsConfig.getLineStyle());
  const Property& pLineColor = m_controller->getProperty(settingsConfig.getLineColor());
  const Property& pLineWidth = m_controller->getProperty(settingsConfig.getLineWidth());

  const std::string& lineStyleName = pLineStyle.getValue().toString();
  const te::color::RGBAColor& lineColor = pLineColor.getValue().toColor();
  double lineWidth = pLineWidth.getValue().toDouble();

  EnumLineStyleType* lineStyle = Enums::getInstance().getEnumLineStyleType();
  EnumType* currentLineStyle = Enums::getInstance().getEnumLineStyleType()->getEnum(lineStyleName);

  if(currentLineStyle == 0)
  {
    currentLineStyle = Enums::getInstance().getEnumLineStyleType()->searchLabel(lineStyleName);
  }

  QPen pen;

  if(currentLineStyle == lineStyle->getStyleSolid())
  {
    pen.setStyle(Qt::SolidLine);
  }
  else if(currentLineStyle == lineStyle->getStyleDot())
  {
    pen.setStyle(Qt::DotLine);
  }
  else if(currentLineStyle == lineStyle->getStyleDash())
  {
    pen.setStyle(Qt::DashLine);
  }
  else if(currentLineStyle == lineStyle->getStyleDashDot())
  {
    pen.setStyle(Qt::DashDotLine);
  }
  else if(currentLineStyle == lineStyle->getStyleDashDotDot())
  {
    pen.setStyle(Qt::DashDotDotLine);
  }

  QColor clrLine(lineColor.getRed(), lineColor.getGreen(), lineColor.getBlue(), lineColor.getAlpha());
  pen.setColor(clrLine);
  pen.setWidth(0);

  painter->setPen(pen);
}

void te::layout::GridMapItem::configTextPainter( QPainter* painter )
{
  GridSettingsConfigProperties settingsConfig;

  const Property& pTextPointSize = m_controller->getProperty(settingsConfig.getPointTextSize());
  const Property& pTextFontFamily = m_controller->getProperty(settingsConfig.getFontText());
  const Property& pTextColor = m_controller->getProperty(settingsConfig.getTextColor());

  int textPointSize = pTextPointSize.getValue().toInt();
  const std::string& fontFamily = pTextFontFamily.getValue().toString();
  const te::color::RGBAColor& textColor = pTextColor.getValue().toColor();

  QColor clrText(textColor.getRed(), textColor.getGreen(), textColor.getBlue(), textColor.getAlpha());

  ItemUtils::ConfigurePainterForTexts(painter, fontFamily, textPointSize);

  QPen pen (clrText);
  painter->setPen(pen);
}

void te::layout::GridMapItem::clear()
{
  m_verticalLines.clear();
  m_horizontalLines.clear();
  m_topTexts.clear();
  m_bottomTexts.clear();
  m_rightTexts.clear();
  m_leftTexts.clear();

  m_boundingBox = te::gm::Envelope();
}

void te::layout::GridMapItem::drawContinuousLines( QPainter* painter )
{
  painter->save();

  configPainter(painter);

  drawVerticalLines(painter);

  drawHorizontalLines(painter);

  drawTexts(painter);

  painter->restore();
}

void te::layout::GridMapItem::drawVerticalLines( QPainter* painter )
{
  QList<QLineF>::iterator it = m_verticalLines.begin();
  for( ; it != m_verticalLines.end() ; ++it )
  {
    QLineF line = (*it);
    painter->drawLine(line);
  }
}

void te::layout::GridMapItem::drawHorizontalLines( QPainter* painter )
{
  QList<QLineF>::iterator it = m_horizontalLines.begin();
  for( ; it != m_horizontalLines.end() ; ++it )
  {
    QLineF line = (*it);
    painter->drawLine(line);
  }
}

void te::layout::GridMapItem::drawTexts( QPainter* painter )
{
  GridSettingsConfigProperties settingsConfig;

  const Property& pVisibleAllTexts = m_controller->getProperty(settingsConfig.getVisibleAllTexts());
  const Property& pLeftText = m_controller->getProperty(settingsConfig.getLeftText());
  const Property& pRightText = m_controller->getProperty(settingsConfig.getRightText());
  const Property& pTopText = m_controller->getProperty(settingsConfig.getTopText());
  const Property& pBottomText = m_controller->getProperty(settingsConfig.getBottomText());

  bool visibleAllTexts = pVisibleAllTexts.getValue().toBool();
  bool leftText = pLeftText.getValue().toBool();
  bool rightText = pRightText.getValue().toBool();
  bool topText = pTopText.getValue().toBool();
  bool bottomText = pBottomText.getValue().toBool();

  if(visibleAllTexts == false)
  {
    return;
  }

  painter->save();

  configTextPainter(painter);
  
  if(leftText == true)
  {
    drawLeftTexts(painter);
  }

  if(rightText == true)
  {
    drawRightTexts(painter);
  }
  
  if(bottomText == true)
  {
    drawBottomTexts(painter);
  }

  if(topText == true)
  {
    drawTopTexts(painter);
  }

  painter->restore();  
}

void te::layout::GridMapItem::drawTopTexts( QPainter* painter )
{
  std::map<std::string, QPointF>::iterator it = m_topTexts.begin();
  for( ; it != m_topTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;
    drawText(pt, painter, txt);
  }
}

void te::layout::GridMapItem::drawBottomTexts( QPainter* painter )
{
  std::map<std::string, QPointF>::iterator it = m_bottomTexts.begin();
  for( ; it != m_bottomTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;
    drawText(pt, painter, txt);
  }
}

void te::layout::GridMapItem::drawLeftTexts( QPainter* painter )
{
  double width = 0;
  double height = 0;

  QFont ft = painter->font();

  std::map<std::string, QPointF>::iterator it = m_leftTexts.begin();
  for( ; it != m_leftTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;

    //checkMaxMapDisplacement(ft, txt, width, height);

    drawText(pt, painter, txt);
  }

  //changeMapDisplacement(width, height);
}

void te::layout::GridMapItem::drawRightTexts( QPainter* painter )
{
  std::map<std::string, QPointF>::iterator it = m_rightTexts.begin();
  for( ; it != m_rightTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;  
    drawText(pt, painter, txt);
  }
}

QRectF te::layout::GridMapItem::boundingRect() const
{
  if (m_currentAction == RESIZE_ACTION)
  {
    return AbstractItem<QGraphicsItem>::boundingRect();
  }

  if(m_boundingBox.isValid())
  {
    return QRectF(m_boundingBox.getLowerLeftX(), m_boundingBox.getLowerLeftY(), m_boundingBox.getWidth(), m_boundingBox.getHeight());
  }
  return AbstractItem<QGraphicsItem>::boundingRect();
}

void te::layout::GridMapItem::drawCrossLines(QPainter* painter)
{
  painter->save();

  configPainter(painter);

  GridSettingsConfigProperties settingsConfig;

  const Property& pCrossOffset = m_controller->getProperty(settingsConfig.getCrossOffset());
  double crossOffSet = pCrossOffset.getValue().toDouble();

  QList<QLineF>::iterator itv = m_verticalLines.begin();
  for( ; itv != m_verticalLines.end() ; ++itv )
  {
    QLineF vtrLine = (*itv);
    te::gm::Envelope vertical(vtrLine.x1(), vtrLine.y1(), vtrLine.x2(), vtrLine.y2());

    QList<QLineF>::iterator ith = m_horizontalLines.begin();
    for( ; ith != m_horizontalLines.end() ; ++ith )
    {
      QLineF hrzLine = (*ith);
      te::gm::Envelope horizontal(hrzLine.x1(), hrzLine.y1(), hrzLine.x2(), hrzLine.y2());

      // check intersection between two lines
      te::gm::Envelope result = vertical.intersection(horizontal);
      if(result.isValid())
      {
        QPointF pot(result.m_llx, result.m_lly);

        QLineF lneHrz(pot.x() - crossOffSet, pot.y(), pot.x() + crossOffSet, pot.y());
        QLineF lneVrt(pot.x(), pot.y() - crossOffSet, pot.x(), pot.y() + crossOffSet);
        
        if(drawCrossIntersectMapBorder(lneVrt, lneHrz, painter))
        {
          continue;
        }

        painter->drawLine(lneHrz);
        painter->drawLine(lneVrt);
      }
    }
  }

  configPainter(painter);
  
  drawTexts(painter);

  painter->restore();
}

bool te::layout::GridMapItem::drawCrossIntersectMapBorder( QLineF vrt, QLineF hrz, QPainter* painter )
{
  GridSettingsConfigProperties settingsConfig;

  const Property& pWidth = m_controller->getProperty("width");
  const Property& pHeight = m_controller->getProperty("height");
  const Property& pCrossOffset = m_controller->getProperty(settingsConfig.getCrossOffset());

  double width = pWidth.getValue().toDouble();
  double height = pHeight.getValue().toDouble();
  double crossOffSet = pCrossOffset.getValue().toDouble();

  bool result = false;

  painter->save();

  te::gm::Envelope boxMM(0, 0, width, height);

  te::gm::Envelope boxWithOffSet(boxMM.m_llx + crossOffSet, boxMM.m_lly + crossOffSet, boxMM.m_urx - crossOffSet, boxMM.m_ury - crossOffSet);
  
  te::gm::Envelope lneHrz(hrz.x1(), hrz.y1(), hrz.x2(), hrz.y2());
  te::gm::Envelope lneVrt(vrt.x1(), vrt.y1(), vrt.x2(), vrt.y2());

  QRectF recWithOffSet(boxMM.m_llx, boxMM.m_lly, boxMM.m_urx, boxMM.m_ury);

  painter->drawRect(recWithOffSet);

  bool resultMapLneTop = boxWithOffSet.touches(lneHrz);
  if(!resultMapLneTop)
  {
    return true;
  }

  painter->restore();

  return result;
}
