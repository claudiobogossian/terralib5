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
  , m_changeSize(false)
  , m_showDebugDrawings(false)
  , m_defaultRotate(90)
{  
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

  std::vector<TextPosition>::iterator it = m_topTexts.begin();
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

  std::vector<TextPosition>::iterator it = m_bottomTexts.begin();
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
  
  std::vector<TextPosition>::iterator it = m_leftTexts.begin();
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
  
  std::vector<TextPosition>::iterator it = m_rightTexts.begin();
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

    QList<QLineF>::iterator ith = m_horizontalLines.begin();
    for( ; ith != m_horizontalLines.end() ; ++ith )
    {
      QLineF hrzLine = (*ith);

      QPointF intersectonPoint;
      QLineF::IntersectType intersectionType = vtrLine.intersect(hrzLine, &intersectonPoint);

      if (intersectionType == QLineF::BoundedIntersection)
      {
        QLineF lneHrz(intersectonPoint.x() - crossOffSet, intersectonPoint.y(), intersectonPoint.x() + crossOffSet, intersectonPoint.y());
        QLineF lneVrt(intersectonPoint.x(), intersectonPoint.y() - crossOffSet, intersectonPoint.x(), intersectonPoint.y() + crossOffSet);
        
        if(drawCrossIntersectMapBorder(lneVrt, lneHrz, painter) == true)
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

  //if the cross intersects the border of the reference rect, a line must be drawn instead of the cross
  te::gm::Envelope boxMM(0, 0, width, height);

  QPointF intersectionPoint;

  QLineF bottomLine(boxMM.m_llx, boxMM.m_lly, boxMM.m_urx, boxMM.m_lly);
  QLineF topLine(boxMM.m_llx, boxMM.m_ury, boxMM.m_urx, boxMM.m_ury);
  QLineF leftLine(boxMM.m_llx, boxMM.m_lly, boxMM.m_llx, boxMM.m_ury);
  QLineF rightLine(boxMM.m_urx, boxMM.m_lly, boxMM.m_urx, boxMM.m_ury);

  bool intersects = false;
  if (bottomLine.intersect(vrt, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x(), intersectionPoint.y() + crossOffSet);
    intersects = true;

    painter->drawLine(borderLine);
  }
  if (topLine.intersect(vrt, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x(), intersectionPoint.y() - crossOffSet);
    intersects = true;

    painter->drawLine(borderLine);
  }
  if (leftLine.intersect(hrz, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x() + crossOffSet, intersectionPoint.y());
    intersects = true;

    painter->drawLine(borderLine);
  }
  if (rightLine.intersect(hrz, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x() - crossOffSet, intersectionPoint.y());
    intersects = true;

    painter->drawLine(borderLine);
  }

  return intersects;
}

void te::layout::GridMapItem::debugDrawTextRect(QPainter* painter, const QPointF& point, const std::string& text, int rotate)
{
  if (m_showDebugDrawings == false)
  {
    return;
  }

  if (scene() == 0)
  {
    return;
  }

  AbstractScene* myScene = dynamic_cast<AbstractScene*>(scene());
  if (myScene == 0)
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
  QPainterPath textObject = itemUtils->textToVector(text.c_str(), painter->font(), myScene->getContext().getDpiX(), point, rotate);

//draws the rect
  painter->save();

  QPen pen;
  pen.setWidth(0);
  pen.setColor(QColor(255, 0, 0));
  painter->setPen(pen);
  painter->setBrush(Qt::NoBrush);

  painter->drawRect(textObject.boundingRect());

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

te::gm::Envelope te::layout::GridMapItem::calculateTop(QLineF line, QRectF textBoundingRect, QString text, bool rotate, double horizontalDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  //if the 90 degrees rotation was applied, we transpose the bounding rect
  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double x = line.p2().x();
  double ury = line.p2().y();

  // text top
  QPointF ptTop(x - (textWidth / 2.), ury + horizontalDisplacement);
  m_topTexts.push_back(TextPosition(text.toStdString(), ptTop));

  te::gm::Envelope topTextBox(ptTop.x(), ptTop.y(), ptTop.x() + textWidth, ptTop.y() + textHeight);
  m_boundingBox.Union(topTextBox);

  return topTextBox;
}

te::gm::Envelope te::layout::GridMapItem::calculateBottom(QLineF line, QRectF textBoundingRect, QString text, bool rotate, double horizontalDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  //if the 90 degrees rotation was applied, we transpose the bounding rect
  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double x = line.p1().x();
  double lly = line.p1().y();

  // text bottom
  QPointF ptBottom(x - (textWidth / 2.), lly - textHeight - horizontalDisplacement);
  m_bottomTexts.push_back(TextPosition(text.toStdString(), ptBottom));

  te::gm::Envelope bottomTextBox(ptBottom.x(), ptBottom.y(), ptBottom.x() + textWidth, ptBottom.y() + textHeight);
  m_boundingBox.Union(bottomTextBox);

  return bottomTextBox;
}

te::gm::Envelope te::layout::GridMapItem::calculateRight(QLineF line, QRectF textBoundingRect, QString text, bool rotate, double verticalDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  double urx = line.p2().x();
  double y = line.p2().y();

  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double yReference = y + textY - (textHeight / 2.);

  // text right
  QPointF ptRight(urx + verticalDisplacement, yReference);
  m_rightTexts.push_back(TextPosition(text.toStdString(), ptRight));

  te::gm::Envelope rightTextBox(ptRight.x(), ptRight.y(), ptRight.x() + textWidth, ptRight.y() + textHeight);
  m_boundingBox.Union(rightTextBox);

  return rightTextBox;
}

te::gm::Envelope te::layout::GridMapItem::calculateLeft(QLineF line, QRectF textBoundingRect, QString text, bool rotate, double verticalDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  //if the 90 degrees rotation was applied, we transpose the bounding rect
  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double llx = line.p1().x();
  double y = line.p1().y();

  double yReference = y + textY - (textHeight / 2.);

  // text left
  QPointF ptLeft(llx - textX - textWidth - verticalDisplacement, yReference);
  m_leftTexts.push_back(TextPosition(text.toStdString(), ptLeft));

  te::gm::Envelope leftTextBox(ptLeft.x(), ptLeft.y(), ptLeft.x() + textWidth, ptLeft.y() + textHeight);
  m_boundingBox.Union(leftTextBox);

  return leftTextBox;
}