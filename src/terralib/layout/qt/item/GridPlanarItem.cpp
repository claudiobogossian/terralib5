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
  \file GridPlanarItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridPlanarItem.h"

#include "../core/ItemUtils.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/property/PlanarGridSettingsConfigProperties.h"
#include "../../core/WorldTransformer.h"
#include "../../core/Utils.h"

te::layout::GridPlanarItem::GridPlanarItem(AbstractItemController* controller, bool invertedMatrix)
  : GridMapItem(controller, invertedMatrix)
{

}

te::layout::GridPlanarItem::~GridPlanarItem()
{

}

void te::layout::GridPlanarItem::drawGrid(QPainter* painter)
{
  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pStyle = m_controller->getProperty(settingsConfig.getStyle());
  
  const std::string& style = pStyle.getOptionByCurrentChoice().toString();
  EnumType* currentStyle = Enums::getInstance().getEnumGridStyleType()->getEnum(style);
  if (currentStyle != 0)
  {
    currentStyle = Enums::getInstance().getEnumGridStyleType()->searchLabel(style);
  }

  const Property& pFontFamily = m_controller->getProperty(settingsConfig.getFontText());
  const Property& pFontSize = m_controller->getProperty(settingsConfig.getPointTextSize());

  std::string fontFamily = pFontFamily.getValue().toString();
  int fontSize = pFontSize.getValue().toInt();

  ItemUtils::ConfigurePainterForTexts(painter, fontFamily, fontSize);
  
  EnumGridStyleType* gridStyle = Enums::getInstance().getEnumGridStyleType();
  if(!gridStyle)
  {
    return;
  }
  
  if(currentStyle == gridStyle->getStyleContinuous())
  {
    drawContinuousLines(painter);
  }
  else if(currentStyle == gridStyle->getStyleCross())
  {
    drawCrossLines(painter);
  }
}

void te::layout::GridPlanarItem::calculateGrid()
{
  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pPlanarBox = m_controller->getProperty("planar_box");
  const Property& pWidth = m_controller->getProperty("width");
  const Property& pHeight = m_controller->getProperty("height");
  const Property& pFrameThickness = m_controller->getProperty("frame_thickness");

  const te::gm::Envelope& planarBox = pPlanarBox.getValue().toEnvelope();
  double width = pWidth.getValue().toDouble();
  double height = pHeight.getValue().toDouble();
  double frameThickness = pFrameThickness.getValue().toDouble();

  te::gm::Envelope referenceBoxMM(0, 0, width, height);

  clear();

  const Property& pFontFamily = m_controller->getProperty(settingsConfig.getFontText());
  const Property& pFontSize = m_controller->getProperty(settingsConfig.getPointTextSize());

  std::string fontFamily = pFontFamily.getValue().toString();
  int fontSize = pFontSize.getValue().toInt();
  
  calculateVertical(planarBox, referenceBoxMM);
  calculateHorizontal(planarBox, referenceBoxMM);

  m_boundingBox = te::gm::Envelope(m_boundingBox.getLowerLeftX() - frameThickness, m_boundingBox.getLowerLeftY() - frameThickness, m_boundingBox.getUpperRightX() + frameThickness, m_boundingBox.getUpperRightY() + frameThickness);

  prepareGeometryChange();
}

void te::layout::GridPlanarItem::calculateVertical( const te::gm::Envelope& geoBox, const te::gm::Envelope& boxMM )
{
  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pFontFamily = m_controller->getProperty(settingsConfig.getFontText());
  const Property& pTextPointSize = m_controller->getProperty(settingsConfig.getPointTextSize());
  const Property& pVerticalGap = m_controller->getProperty(settingsConfig.getLneVrtGap());
  const Property& pVerticalDisplacement = m_controller->getProperty(settingsConfig.getLneVrtDisplacement());
  const Property& pUnit = m_controller->getProperty(settingsConfig.getUnit());
  const Property& pLeftRotate = m_controller->getProperty(settingsConfig.getLeftRotateText());
  const Property& pRightRotate = m_controller->getProperty(settingsConfig.getRightRotateText());

  std::string fontFamily = pFontFamily.getValue().toString();
  int textPointSize = pTextPointSize.getValue().toInt();
  double verticalGap = pVerticalGap.getValue().toDouble();
  double verticalDisplacement = pVerticalDisplacement.getValue().toDouble();
  LayoutUnit unit = (LayoutUnit)pUnit.getValue().toInt();
  bool bLeftRotate = pLeftRotate.getValue().toBool();
  bool bRightRotate = pRightRotate.getValue().toBool();

  Utils* utils = Context::getInstance().getUtils();
  ItemUtils* itemUtils = Context::getInstance().getItemUtils();

  // Draw a horizontal line and the y coordinate change(vertical)

  WorldTransformer transf = utils->getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double y1 = initVerticalLines(geoBox);
  
  m_boundingBox.Union(boxMM);
  
  QFont ft(fontFamily.c_str(), textPointSize);
  
  for( ; y1 <= geoBox.getUpperRightY() ; y1 += verticalGap)
  {
    if(y1 < geoBox.getLowerLeftY())
      continue;

    double llx = 0;
    double urx = 0;
    double y = 0;
    transf.system1Tosystem2(geoBox.getLowerLeftX(), y1, llx, y);
    transf.system1Tosystem2(geoBox.getUpperRightX(), y1, urx, y);
    
    QLineF line(llx, y, urx, y);
    m_horizontalLines.push_back(line);

    double number = y1 / (double)unit;
    QString convert = QString::number(number, 'f', 0);

    QRectF rectF = itemUtils->getMinimumTextBoundary(fontFamily, textPointSize, convert.toStdString());

    calculateLeft(line, rectF, convert, bLeftRotate, verticalDisplacement);
    calculateRight(line, rectF, convert, bRightRotate, verticalDisplacement);
  }
}

te::gm::Envelope te::layout::GridPlanarItem::calculateRight(QLineF line, QRectF textBoundingRect, QString text, bool rotate, double verticalDisplacement)
{
  double rightWtxt = textBoundingRect.width();
  double rightHtxt = textBoundingRect.height();

  double urx = line.p2().x();
  double y = line.p2().y();

  double yReference = y - textBoundingRect.y() - (textBoundingRect.height() / 2.);

  if (rotate)
  {
    rightWtxt = textBoundingRect.height();
    rightHtxt = textBoundingRect.width();
    urx += rightWtxt;
  }

  // text right
  QPointF ptRight(urx + verticalDisplacement, yReference);
  m_rightTexts[text.toStdString()] = ptRight;

  if (rotate)
  {
    ptRight.setX(ptRight.x() - rightWtxt);
  }

  te::gm::Envelope rightTextBox(ptRight.x(), ptRight.y(), ptRight.x() + rightWtxt, ptRight.y() + rightHtxt);
  m_boundingBox.Union(rightTextBox);

  return rightTextBox;
}

te::gm::Envelope te::layout::GridPlanarItem::calculateLeft(QLineF line, QRectF textBoundingRect, QString text, bool rotate, double verticalDisplacement)
{
  double leftWtxt = textBoundingRect.width();
  double leftHtxt = textBoundingRect.height();

  double llx = line.p1().x();
  double y = line.p1().y();

  double yReference = y - textBoundingRect.y() - (textBoundingRect.height() / 2.);

  if (rotate)
  {
    leftWtxt = textBoundingRect.height();
    leftHtxt = textBoundingRect.width();
    llx += leftWtxt;
  }

  // text left
  QPointF ptLeft(llx - leftWtxt - verticalDisplacement, yReference);
  m_leftTexts[text.toStdString()] = ptLeft;

  if (rotate)
  {
    ptLeft.setX(ptLeft.x() - leftWtxt);
  }

  te::gm::Envelope leftTextBox(ptLeft.x(), ptLeft.y(), ptLeft.x() + leftWtxt, ptLeft.y() + leftHtxt);
  m_boundingBox.Union(leftTextBox);

  return leftTextBox;
}

void te::layout::GridPlanarItem::calculateHorizontal( const te::gm::Envelope& geoBox, const te::gm::Envelope& boxMM )
{
  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pFontFamily = m_controller->getProperty(settingsConfig.getFontText());
  const Property& pTextPointSize = m_controller->getProperty(settingsConfig.getPointTextSize());
  const Property& pHorizontalGap = m_controller->getProperty(settingsConfig.getLneHrzGap());
  const Property& pHorizontalDisplacement = m_controller->getProperty(settingsConfig.getLneHrzDisplacement());
  const Property& pUnit = m_controller->getProperty(settingsConfig.getUnit());
  const Property& pTopRotate = m_controller->getProperty(settingsConfig.getTopRotateText());
  const Property& pBottomRotate = m_controller->getProperty(settingsConfig.getBottomRotateText());

  std::string fontFamily = pFontFamily.getValue().toString();
  int textPointSize = pTextPointSize.getValue().toInt();
  double horizontalGap = pHorizontalGap.getValue().toDouble();
  double horizontalDisplacement = pHorizontalDisplacement.getValue().toDouble();
  LayoutUnit unit = (LayoutUnit)pUnit.getValue().toInt();
  bool bTopRotate = pTopRotate.getValue().toBool();
  bool bBottomRotate = pBottomRotate.getValue().toBool();

  Utils* utils = Context::getInstance().getUtils();
  ItemUtils* itemUtils = Context::getInstance().getItemUtils();

  // Draw a vertical line and the x coordinate change(horizontal)

  WorldTransformer transf = utils->getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double x1 = initHorizontalLines(geoBox);

  utils = Context::getInstance().getUtils();
  
  m_boundingBox.Union(boxMM);
  
  QFont ft(fontFamily.c_str(), textPointSize);

  for( ; x1 <= geoBox.getUpperRightX() ; x1 += horizontalGap)
  {
    if(x1 < geoBox.getLowerLeftX())
      continue;

    double lly = 0;
    double ury = 0;
    double x = 0;
    transf.system1Tosystem2(x1, geoBox.getLowerLeftY(), x, lly);
    transf.system1Tosystem2(x1, geoBox.getUpperRightY(), x, ury);

    te::gm::Envelope newBox(x, lly, x, ury);

    if(lly > ury)
    {
      double ycopy = lly;
      lly = ury;
      ury = ycopy;
    }

    QLineF line(x, lly, x, ury);
    m_verticalLines.push_back(line);

    double number = x1 / (double)unit;
    QString convert = QString::number(number, 'f', 0);

    QRectF rectF = itemUtils->getMinimumTextBoundary(fontFamily, textPointSize, convert.toStdString());
    
    calculateTop(line, rectF, convert, bTopRotate, horizontalDisplacement);
    calculateBottom(line, rectF, convert, bBottomRotate, horizontalDisplacement);
  }
}

te::gm::Envelope te::layout::GridPlanarItem::calculateTop(QLineF line, QRectF textBoundingRect, QString text, bool rotate, double horizontalDisplacement)
{
  double topWtxt = textBoundingRect.width();
  double toptHtxt = textBoundingRect.height();

  double x = line.p2().x();
  double ury = line.p2().y();

  if (rotate)
  {
    topWtxt = textBoundingRect.height();
    toptHtxt = textBoundingRect.width();
    x += topWtxt / 2.;
  }
  
  // text top
  QPointF ptTop(x - (topWtxt / 2.), ury + horizontalDisplacement);
  m_topTexts[text.toStdString()] = ptTop;

  if (rotate)
  {
    ury += toptHtxt;
  }

  te::gm::Envelope topTextBox(ptTop.x(), ptTop.y(), ptTop.x() + topWtxt, ptTop.y() + toptHtxt);
  m_boundingBox.Union(topTextBox);

  return topTextBox;
}

te::gm::Envelope te::layout::GridPlanarItem::calculateBottom(QLineF line, QRectF textBoundingRect, QString text, bool rotate, double horizontalDisplacement)
{
  double bottomWtxt = textBoundingRect.width();
  double bottomHtxt = textBoundingRect.height();

  double x = line.p1().x();
  double lly = line.p1().y();

  if (rotate)
  {
    bottomWtxt = textBoundingRect.height();
    bottomHtxt = textBoundingRect.width();
    x += bottomWtxt / 2.;
  }

  // text bottom
  QPointF ptBottom(x - (bottomWtxt / 2.), lly - bottomHtxt - horizontalDisplacement);
  m_bottomTexts[text.toStdString()] = ptBottom;

  te::gm::Envelope bottomTextBox(ptBottom.x(), ptBottom.y(), ptBottom.x() + bottomWtxt, ptBottom.y() + bottomHtxt);
  m_boundingBox.Union(bottomTextBox);

  return bottomTextBox;
}

double te::layout::GridPlanarItem::initVerticalLines( const te::gm::Envelope& geoBox )
{
  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pInitialGridPointY = m_controller->getProperty(settingsConfig.getInitialGridPointY());
  const Property& pVerticalGap = m_controller->getProperty(settingsConfig.getLneVrtGap());

  double initialGridPointY = pInitialGridPointY.getValue().toDouble();
  double verticalGap = pVerticalGap.getValue().toDouble();

  double yInit = initialGridPointY;
  if(yInit < geoBox.getLowerLeftY())
  {
    double dify = geoBox.getLowerLeftY() - yInit;
    int nParts = (int)(dify/verticalGap);
    if(nParts == 0)
    {
      yInit = initialGridPointY;
    }
    else
    {
      yInit = yInit + (nParts * verticalGap);
    }
  }

  return yInit;
}

double te::layout::GridPlanarItem::initHorizontalLines( const te::gm::Envelope& geoBox )
{
  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pInitialGridPointX = m_controller->getProperty(settingsConfig.getInitialGridPointX());
  const Property& pHorizontalGap = m_controller->getProperty(settingsConfig.getLneHrzGap());

  double initialGridPointX = pInitialGridPointX.getValue().toDouble();
  double horizontalGap = pHorizontalGap.getValue().toDouble();

  double xInit = initialGridPointX;
  if(xInit < geoBox.getLowerLeftX())
  {
    double difx = geoBox.getLowerLeftX() - xInit;
    int nParts = (int)(difx/horizontalGap);
    if(nParts == 0)
    {
      xInit = initialGridPointX;
    }
    else
    {
      xInit = xInit + (nParts * horizontalGap);
    }
  }

  return xInit;
}
