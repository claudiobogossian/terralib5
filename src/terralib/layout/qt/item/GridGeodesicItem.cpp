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
  \file GridGeodesicItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridGeodesicItem.h"

#include "../core/ItemUtils.h"
#include "../../core/WorldTransformer.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../../../maptools/Utils.h"

te::layout::GridGeodesicItem::GridGeodesicItem(AbstractItemController* controller, bool invertedMatrix)
  : GridMapItem(controller, invertedMatrix)
{  
  
}

te::layout::GridGeodesicItem::~GridGeodesicItem()
{

}

void te::layout::GridGeodesicItem::drawGrid(QPainter* painter)
{
  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pGeographicBox = m_controller->getProperty("geographic_box");
  const Property& pStyle = m_controller->getProperty(settingsConfig.getStyle());

  const te::gm::Envelope& geographicBox = pGeographicBox.getValue().toEnvelope();

  const std::string& style = pStyle.getValue().toString();

  EnumType* currentStyle = Enums::getInstance().getEnumGridStyleType()->getEnum(style);
  if(currentStyle != 0)
  {
    currentStyle = Enums::getInstance().getEnumGridStyleType()->searchLabel(style);
  }
  
  Utils* utils = Context::getInstance().getUtils();

  // Box necessario para desenhar a curvatura
  te::gm::Envelope planarBox = geographicBox;
  int zone = te::map::CalculatePlanarZone(geographicBox);
  if(zone < 0 || zone > 60)
  {
    painter->drawRect(boundingRect());
    return;
  }
  
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

void te::layout::GridGeodesicItem::calculateGrid()
{
  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pGeographicBox = m_controller->getProperty("geographic_box");
  const Property& pWidth = m_controller->getProperty("width");
  const Property& pHeight = m_controller->getProperty("height");
  const Property& pStyle = m_controller->getProperty(settingsConfig.getStyle());

  const te::gm::Envelope& geographicBox = pGeographicBox.getValue().toEnvelope();
  double width = pWidth.getValue().toDouble();
  double height = pHeight.getValue().toDouble();
  const std::string& style = pStyle.getValue().toString();

  EnumType* currentStyle = Enums::getInstance().getEnumGridStyleType()->getEnum(style);
  if (currentStyle != 0)
  {
    currentStyle = Enums::getInstance().getEnumGridStyleType()->searchLabel(style);
  }

  te::gm::Envelope newBoxMM(0, 0, width, height);

  clear();

  Utils* utils = Context::getInstance().getUtils();

  // Box necessario para desenhar a curvatura
  te::gm::Envelope planarBox = geographicBox;
  int zone = te::map::CalculatePlanarZone(geographicBox);
  if (zone < 0 || zone > 60)
  {
    return;
  }

  utils->remapToPlanar(&planarBox, zone);

  calculateVertical(geographicBox, planarBox, newBoxMM);
  calculateHorizontal(geographicBox, planarBox, newBoxMM);

  prepareGeometryChange();
}

double te::layout::GridGeodesicItem::initVerticalLines( const te::gm::Envelope& geoBox )
{
  GeodesicGridSettingsConfigProperties settingsConfig;

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

double te::layout::GridGeodesicItem::initHorizontalLines( const te::gm::Envelope& geoBox )
{
  GeodesicGridSettingsConfigProperties settingsConfig;

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

void te::layout::GridGeodesicItem::calculateVertical(const te::gm::Envelope& geoBox, const te::gm::Envelope& planarBox, const te::gm::Envelope& boxMM )
{
  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pVerticalGap = m_controller->getProperty(settingsConfig.getLneVrtGap());
  const Property& pShowDegreesText = m_controller->getProperty(settingsConfig.getDegreesText());
  const Property& pShowMinutesText = m_controller->getProperty(settingsConfig.getMinutesText());
  const Property& pShowSecondsText = m_controller->getProperty(settingsConfig.getSecondsText());
  const Property& pHorizontalDisplacement = m_controller->getProperty(settingsConfig.getLneHrzDisplacement());

  double verticalGap = pVerticalGap.getValue().toDouble();
  bool showDegreesText = pShowDegreesText.getValue().toBool();
  bool showMinutesText = pShowMinutesText.getValue().toBool();
  bool showSecondsText = pShowSecondsText.getValue().toBool();
  double horizontalDisplacement = pHorizontalDisplacement.getValue().toDouble();

  // Draw a horizontal line and the y coordinate change(vertical)

  Utils* utils = Context::getInstance().getUtils();

  WorldTransformer transf = utils->getTransformGeo(planarBox, boxMM);
  transf.setMirroring(false);

  int zone = te::map::CalculatePlanarZone(geoBox);
  if(zone < 0 || zone > 60)
  {
    return;
  }

  double y1 = initVerticalLines(geoBox);

  for( ; y1 < geoBox.getUpperRightY() ; y1 += verticalGap)
  {
    if(y1 < geoBox.getLowerLeftY())
      continue;

    te::gm::Envelope env(geoBox.getLowerLeftX(), y1, geoBox.getUpperRightX(), y1);

    te::gm::LinearRing* line = 0;
    line = utils->addCoordsInX(env, y1, verticalGap);
    
    // Line curvature: of latlong to planar;
    // Draw line: planar to mm
    utils->remapToPlanar(line, zone);
    utils->convertToMillimeter(transf, line);

    te::gm::Envelope* ev = const_cast<te::gm::Envelope*>(line->getMBR());

    QLineF lne(ev->getLowerLeftX(), ev->getLowerLeftY(), ev->getUpperRightX(), ev->getUpperRightY());
    m_horizontalLines.push_back(lne);

    std::string text = utils->convertDecimalToDegree(y1, showDegreesText, showMinutesText, showSecondsText);

    // text left
    QPointF ptLeft(ev->getLowerLeftX() - horizontalDisplacement, ev->getLowerLeftY());
    m_leftTexts[text] =  ptLeft;

    // text right
    QPointF ptRight(ev->getUpperRightX() + horizontalDisplacement, ev->getUpperRightY());
    m_rightTexts[text] = ptRight;

    if(line)
    {
      delete line;
      line = 0;
    }
  }
}

void te::layout::GridGeodesicItem::calculateHorizontal( const te::gm::Envelope& geoBox, const te::gm::Envelope& planarBox, const te::gm::Envelope& boxMM )
{
  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pHorizontalGap = m_controller->getProperty(settingsConfig.getLneHrzGap());
  const Property& pShowDegreesText = m_controller->getProperty(settingsConfig.getDegreesText());
  const Property& pShowMinutesText = m_controller->getProperty(settingsConfig.getMinutesText());
  const Property& pShowSecondsText = m_controller->getProperty(settingsConfig.getSecondsText());
  const Property& pVerticalDisplacement = m_controller->getProperty(settingsConfig.getLneVrtDisplacement());
  
  double horizontalGap = pHorizontalGap.getValue().toDouble();
  bool showDegreesText = pShowDegreesText.getValue().toBool();
  bool showMinutesText = pShowMinutesText.getValue().toBool();
  bool showSecondsText = pShowSecondsText.getValue().toBool();
  double verticalDisplacement = pVerticalDisplacement.getValue().toDouble();

  // Draw a vertical line and the x coordinate change(horizontal)

  Utils* utils = Context::getInstance().getUtils();
  
  WorldTransformer transf = utils->getTransformGeo(planarBox, boxMM);
  transf.setMirroring(false);

  int zone = te::map::CalculatePlanarZone(geoBox);
  if(zone < 0 || zone > 60)
  {
    return;
  }

  double x1 = initHorizontalLines(geoBox);

  for( ; x1 < geoBox.getUpperRightX() ; x1 += horizontalGap)
  {
    if(x1 < geoBox.getLowerLeftX())
      continue;

    te::gm::Envelope env(x1, geoBox.getLowerLeftY(), x1, geoBox.getUpperRightY());
    te::gm::LinearRing* line = 0;
    line = utils->addCoordsInY(env, x1, horizontalGap);

    // Line curvature: of latlong to planar;
    // Draw line: planar to mm
    utils->remapToPlanar(line, zone);
    utils->convertToMillimeter(transf, line);

    te::gm::Envelope* ev = const_cast<te::gm::Envelope*>(line->getMBR());

    QLineF lne(ev->getLowerLeftX(), ev->getLowerLeftY(), ev->getUpperRightX(), ev->getUpperRightY());
    m_verticalLines.push_back(lne);

    std::string text = utils->convertDecimalToDegree(x1, showDegreesText, showMinutesText, showSecondsText);

    // text bottom
    QPointF ptBottom(ev->getLowerLeftX(), ev->getLowerLeftX() - verticalDisplacement);
    m_bottomTexts[text] = ptBottom;

    // text top
    QPointF ptTop(ev->getUpperRightX(), ev->getUpperRightY() + verticalDisplacement);
    m_topTexts[text] = ptTop;

    if(line)
    {
      delete line;
      line = 0;
    }
  }
}
