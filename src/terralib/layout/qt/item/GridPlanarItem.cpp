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
#include "../../core/property/GridSettingsConfigProperties.h"
#include "../../core/WorldTransformer.h"
#include "../../core/Utils.h"

te::layout::GridPlanarItem::GridPlanarItem(AbstractItemController* controller, bool invertedMatrix)
  : GridMapItem(controller, invertedMatrix)
{  
  
}

te::layout::GridPlanarItem::~GridPlanarItem()
{

}

void te::layout::GridPlanarItem::drawGrid( QPainter* painter )
{
  GridSettingsConfigProperties settingsConfig;

  const Property& pPlanarBox = m_controller->getProperty("planar_box");
  const Property& pWidth = m_controller->getProperty("width");
  const Property& pHeight = m_controller->getProperty("height");
  const Property& pStyle = m_controller->getProperty(settingsConfig.getStyle());

  const te::gm::Envelope& planarBox = pPlanarBox.getValue().toEnvelope();
  double width = pWidth.getValue().toDouble();
  double height = pHeight.getValue().toDouble();

  const std::string& style = pStyle.getValue().toString();
  EnumType* currentStyle = Enums::getInstance().getEnumGridStyleType()->getEnum(style);
  if(currentStyle != 0)
  {
    currentStyle = Enums::getInstance().getEnumGridStyleType()->searchLabel(style);
  }

  te::gm::Envelope referenceBoxMM(0, 0, width, height);

  clear();

  //double scale = mapModel->getCurrentScale();
  //te::gm::Envelope boxMM = mapModel->getMapBox();

  //double wMargin = mapModel->getDisplacementX();
  //double hMargin = mapModel->getDisplacementY();

  //te::gm::Envelope referenceBoxMM(wMargin, hMargin, boxMM.getWidth() + wMargin, boxMM.getHeight() + hMargin);

  //model->setMapScale(scale); // put visit
  //model->calculateGaps(planarBox); ///TODO

  calculateVertical(planarBox, referenceBoxMM);
  calculateHorizontal(planarBox, referenceBoxMM);

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

void te::layout::GridPlanarItem::calculateVertical( const te::gm::Envelope& geoBox, const te::gm::Envelope& boxMM )
{
  GridSettingsConfigProperties settingsConfig;

  const Property& pFontFamily = m_controller->getProperty(settingsConfig.getFontText());
  const Property& pTextPointSize = m_controller->getProperty(settingsConfig.getPointTextSize());
  const Property& pVerticalGap = m_controller->getProperty(settingsConfig.getLneVrtGap());
  const Property& pUnit = m_controller->getProperty("unit");

  std::string fontFamily = pFontFamily.getValue().toString();
  int textPointSize = pTextPointSize.getValue().toInt();
  double verticalGap = pVerticalGap.getValue().toDouble();
  LayoutUnit unit = (LayoutUnit)pUnit.getValue().toInt();


  Utils* utils = Context::getInstance().getUtils();
  ItemUtils* itemUtils = Context::getInstance().getItemUtils();

  // Draw a horizontal line and the y coordinate change(vertical)

  WorldTransformer transf = utils->getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double y1 = initVerticalLines(geoBox);

  double wtxt = 0;
  double htxt = 0;

  m_boundingBox.Union(boxMM);
  
  QFont ft(fontFamily.c_str(), textPointSize);
  
  for( ; y1 < geoBox.getUpperRightY() ; y1 += verticalGap)
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

    std::ostringstream convert;
    convert.precision(10);
    double number = y1 / (double)unit;
    convert << number;

    itemUtils->getTextBoundary(ft, wtxt, htxt,convert.str());

    // text left
    QPointF ptLeft(llx - wtxt, y);
    m_leftTexts[convert.str()] = ptLeft;

    // text right
    QPointF ptRight(urx, y);
    m_rightTexts[convert.str()] = ptRight;

    te::gm::Envelope leftTextBox(ptLeft.x(), ptLeft.y(), ptLeft.x() + wtxt, ptLeft.y() + htxt);
    te::gm::Envelope rightTextBox(ptRight.x(), ptRight.y(), ptRight.x() + wtxt, ptRight.y() + htxt);

    m_boundingBox.Union(leftTextBox);
    m_boundingBox.Union(rightTextBox);
  }
}

void te::layout::GridPlanarItem::calculateHorizontal( const te::gm::Envelope& geoBox, const te::gm::Envelope& boxMM )
{
  GridSettingsConfigProperties settingsConfig;

  const Property& pFontFamily = m_controller->getProperty(settingsConfig.getFontText());
  const Property& pTextPointSize = m_controller->getProperty(settingsConfig.getPointTextSize());
  const Property& pHorizontalGap = m_controller->getProperty(settingsConfig.getLneHrzGap());
  const Property& pUnit = m_controller->getProperty("unit");

  std::string fontFamily = pFontFamily.getValue().toString();
  int textPointSize = pTextPointSize.getValue().toInt();
  double horizontalGap = pHorizontalGap.getValue().toDouble();
  LayoutUnit unit = (LayoutUnit)pUnit.getValue().toInt();

  Utils* utils = Context::getInstance().getUtils();
  ItemUtils* itemUtils = Context::getInstance().getItemUtils();

  // Draw a vertical line and the x coordinate change(horizontal)

  WorldTransformer transf = utils->getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double x1 = initHorizontalLines(geoBox);

  utils = Context::getInstance().getUtils();

  double wtxt = 0;
  double htxt = 0;

  m_boundingBox.Union(boxMM);
  
  QFont ft(fontFamily.c_str(), textPointSize);

  for( ; x1 < geoBox.getUpperRightX() ; x1 += horizontalGap)
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

    std::ostringstream convert;
    convert.precision(10);
    double number = x1 / (double)unit;
    convert << number;

    itemUtils->getTextBoundary(ft, wtxt, htxt, convert.str());

    // text bottom
    QPointF ptBottom(x - (wtxt/2.), lly - htxt);
    m_bottomTexts[convert.str()] = ptBottom;

    // text top
    QPointF ptTop(x - (wtxt/2.), ury);
    m_topTexts[convert.str()] = ptTop;

    te::gm::Envelope bottomTextBox(ptBottom.x(), ptBottom.y(), ptBottom.x() + wtxt, ptBottom.y() + htxt);
    te::gm::Envelope topTextBox(ptTop.x(), ptTop.y(), ptTop.x() + wtxt, ptTop.y() + htxt);

    m_boundingBox.Union(bottomTextBox);
    m_boundingBox.Union(topTextBox);
  }
}

double te::layout::GridPlanarItem::initVerticalLines( const te::gm::Envelope& geoBox )
{
  GridSettingsConfigProperties settingsConfig;

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
  GridSettingsConfigProperties settingsConfig;

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


/*
void te::layout::GridPlanarItem::calculateVertical( te::gm::Envelope geoBox, te::gm::Envelope boxMM, double scale )
{
  GridPlanarModel* model = dynamic_cast<GridPlanarModel*>(m_model);
  if(!model)
  {
    return;
  }

  if(!hasLayer())
  {
    return;
  }

  Utils* utils = Context::getInstance().getUtils();
  ItemUtils* itemUtils = Context::getInstance().getItemUtils();

  // Draw a horizontal line and the y coordinate change(vertical)

  WorldTransformer transf = utils->getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double y1 = initVerticalLines(geoBox);

  double wtxt = 0;
  double htxt = 0;
  
  QFont ft(model->getFontFamily().c_str(), model->getTextPointSize());
  
  for( ; y1 < geoBox.getUpperRightY() ; y1 += model->getLneVrtGap())
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

    std::ostringstream convert;
    convert.precision(10);
    double number = y1 / (double)model->getUnit();
    convert << number;

    itemUtils->getTextBoundary(ft, wtxt, htxt,convert.str());
        
    // text left
    QPointF ptLeft(llx - wtxt, y);
    m_leftTexts[convert.str()] = ptLeft;

    // text right
    QPointF ptRight(urx, y);
    m_rightTexts[convert.str()] = ptRight;
  }
}

void te::layout::GridPlanarItem::calculateHorizontal( te::gm::Envelope geoBox, te::gm::Envelope boxMM, double scale )
{
  GridPlanarModel* model = dynamic_cast<GridPlanarModel*>(m_model);
  if(!model)
  {
    return;
  }

  if(!hasLayer())
  {
    return;
  }

  Utils* utils = Context::getInstance().getUtils();
  ItemUtils* itemUtils = Context::getInstance().getItemUtils();

  // Draw a vertical line and the x coordinate change(horizontal)

  WorldTransformer transf = utils->getTransformGeo(geoBox, boxMM);
  transf.setMirroring(false);

  double x1 = initHorizontalLines(geoBox);

  utils = Context::getInstance().getUtils();

  double wtxt = 0;
  double htxt = 0;
  
  QFont ft(model->getFontFamily().c_str(), model->getTextPointSize());

  for( ; x1 < geoBox.getUpperRightX() ; x1 += model->getLneHrzGap())
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

    std::ostringstream convert;
    convert.precision(10);
    double number = x1 / (double)model->getUnit();
    convert << number;

    itemUtils->getTextBoundary(ft, wtxt, htxt, convert.str());

    // text bottom
    QPointF ptBottom(x - (wtxt/2.), lly - htxt);
    m_bottomTexts[convert.str()] = ptBottom;

    // text top
    QPointF ptTop(x - (wtxt/2.), ury);
    m_topTexts[convert.str()] = ptTop;
  }
}


*/



