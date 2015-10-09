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
  , m_showDebugDrawings(false)
  , m_defaultRotate(90)
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
}

void te::layout::GridMapItem::drawGrid( QPainter* painter )
{

}

void te::layout::GridMapItem::configPainter( QPainter* painter )
{
  GridSettingsConfigProperties settingsConfig;

  const Property& pLineStyle = m_controller->getProperty(settingsConfig.getLineStyle());
  const Property& pLineColor = m_controller->getProperty(settingsConfig.getLineColor());
  const Property& pLineWidth = m_controller->getProperty(settingsConfig.getLineWidth());

  const std::string& lineStyleName = pLineStyle.getOptionByCurrentChoice().toString();
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
  pen.setWidthF(lineWidth);

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
    
#ifdef _DEBUG
    debugDrawLineEdges(painter, line);
#endif
  }
}

void te::layout::GridMapItem::drawHorizontalLines( QPainter* painter )
{
  QList<QLineF>::iterator it = m_horizontalLines.begin();
  for( ; it != m_horizontalLines.end() ; ++it )
  {
    QLineF line = (*it);
    painter->drawLine(line);

#ifdef _DEBUG
    debugDrawLineEdges(painter, line);
#endif
    
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
  GridSettingsConfigProperties settingsConfig;
  const Property& pTopRotateText = m_controller->getProperty(settingsConfig.getTopRotateText());
  bool bRotate = pTopRotateText.getValue().toBool();
  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotate;
  }

  std::map<std::string, QPointF>::iterator it = m_topTexts.begin();
  for( ; it != m_topTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;
    drawText(pt, painter, txt, iRotate);

#ifdef _DEBUG
    debugDrawTextRect(painter, pt, txt, iRotate);
#endif
  }
}

void te::layout::GridMapItem::drawBottomTexts( QPainter* painter )
{
  GridSettingsConfigProperties settingsConfig;
  const Property& pBottomRotateText = m_controller->getProperty(settingsConfig.getBottomRotateText());
  bool bRotate = pBottomRotateText.getValue().toBool();
  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotate;
  }

  std::map<std::string, QPointF>::iterator it = m_bottomTexts.begin();
  for( ; it != m_bottomTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;
    drawText(pt, painter, txt, iRotate);

#ifdef _DEBUG
    debugDrawTextRect(painter, pt, txt, iRotate);
#endif
  }
}

void te::layout::GridMapItem::drawLeftTexts( QPainter* painter )
{
  GridSettingsConfigProperties settingsConfig;
  const Property& pLeftRotateText = m_controller->getProperty(settingsConfig.getLeftRotateText());
  bool bRotate = pLeftRotateText.getValue().toBool();
  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotate;
  }

  double width = 0;
  double height = 0;
  
  std::map<std::string, QPointF>::iterator it = m_leftTexts.begin();
  for( ; it != m_leftTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;
    drawText(pt, painter, txt, iRotate);

#ifdef _DEBUG
    debugDrawTextRect(painter, pt, txt, iRotate);
#endif

  }
}

void te::layout::GridMapItem::drawRightTexts( QPainter* painter )
{
  GridSettingsConfigProperties settingsConfig;
  const Property& pRightRotateText = m_controller->getProperty(settingsConfig.getRightRotateText());
  bool bRotate = pRightRotateText.getValue().toBool();
  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotate;
  }
  
  std::map<std::string, QPointF>::iterator it = m_rightTexts.begin();
  for( ; it != m_rightTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;  
    drawText(pt, painter, txt, iRotate);

#ifdef _DEBUG
    debugDrawTextRect(painter, pt, txt, iRotate);
#endif
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

  painter->restore();

  bool resultMapLneTop = boxWithOffSet.touches(lneHrz);
  if(!resultMapLneTop)
  {
    return true;
  }

  return result;
}

void te::layout::GridMapItem::debugDrawTextRect(QPainter* painter, const QPointF& point, const std::string& text, int rotate)
{
  if (m_showDebugDrawings == false)
  {
    return;
  }

  GridSettingsConfigProperties settingsConfig;

  const Property& pTextPointSize = m_controller->getProperty(settingsConfig.getPointTextSize());
  const Property& pTextFontFamily = m_controller->getProperty(settingsConfig.getFontText());

  int textPointSize = pTextPointSize.getValue().toInt();
  const std::string& fontFamily = pTextFontFamily.getValue().toString();
  
  ItemUtils* itemUtils = Context::getInstance().getItemUtils();

 //creates the rect
  QRectF rectF = itemUtils->getMinimumTextBoundary(fontFamily, textPointSize, text);

//puts the rect in the correct position
  rectF.moveTo(point);

//draws the rect
  painter->save();

  QPen pen;
  pen.setWidth(0);
  pen.setColor(QColor(255, 0, 0));
  painter->setPen(pen);
  painter->setBrush(Qt::NoBrush);

  if (rotate != 0)
  {
    QTransform trf = painter->transform();
    trf.translate(point.x(), point.y());
    trf.rotate(rotate);
    trf.translate(-point.x(), -point.y());
    painter->setTransform(trf);
  }

  painter->drawRect(rectF);

  painter->restore();
}

void te::layout::GridMapItem::debugDrawLineEdges(QPainter* painter, const QLineF& line)
{
  if (m_showDebugDrawings == false)
  {
    return;
  }

  QPointF p1(line.p1());
  QPointF p2(line.p2());

  painter->save();

  QPen linepen(Qt::red);
  linepen.setCapStyle(Qt::RoundCap);
  linepen.setWidth(1);
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(linepen);

  painter->drawPoint(p1);
  painter->drawPoint(p2);

  painter->restore();
}

