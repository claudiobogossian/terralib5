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
#include "../../../geometry/Utils.h"

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
  const Property& pFrameThickness = m_controller->getProperty("frame_thickness");

  const te::gm::Envelope& geographicBox = pGeographicBox.getValue().toEnvelope();
  double width = pWidth.getValue().toDouble();
  double height = pHeight.getValue().toDouble();
  const std::string& style = pStyle.getValue().toString();
  double frameThickness = pFrameThickness.getValue().toDouble();

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

  m_boundingBox = te::gm::Envelope(m_boundingBox.getLowerLeftX() - frameThickness, m_boundingBox.getLowerLeftY() - frameThickness, m_boundingBox.getUpperRightX() + frameThickness, m_boundingBox.getUpperRightY() + frameThickness);

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

  const Property& pFontFamily = m_controller->getProperty(settingsConfig.getFontText());
  const Property& pTextPointSize = m_controller->getProperty(settingsConfig.getPointTextSize());
  const Property& pVerticalGap = m_controller->getProperty(settingsConfig.getLneVrtGap());
  const Property& pHorizontalGap = m_controller->getProperty(settingsConfig.getLneHrzGap());
  const Property& pShowDegreesText = m_controller->getProperty(settingsConfig.getDegreesText());
  const Property& pShowMinutesText = m_controller->getProperty(settingsConfig.getMinutesText());
  const Property& pShowSecondsText = m_controller->getProperty(settingsConfig.getSecondsText());
  const Property& pHorizontalDisplacement = m_controller->getProperty(settingsConfig.getLneHrzDisplacement());
  const Property& pVerticalDisplacement = m_controller->getProperty(settingsConfig.getLneVrtDisplacement());
  const Property& pLeftRotate = m_controller->getProperty(settingsConfig.getLeftRotateText());
  const Property& pRightRotate = m_controller->getProperty(settingsConfig.getRightRotateText());

  std::string fontFamily = pFontFamily.getValue().toString();
  int textPointSize = pTextPointSize.getValue().toInt();
  double verticalGap = pVerticalGap.getValue().toDouble();
  double horizontalGap = pHorizontalGap.getValue().toDouble();
  bool showDegreesText = pShowDegreesText.getValue().toBool();
  bool showMinutesText = pShowMinutesText.getValue().toBool();
  bool showSecondsText = pShowSecondsText.getValue().toBool();
  double horizontalDisplacement = pHorizontalDisplacement.getValue().toDouble();
  double verticalDisplacement = pVerticalDisplacement.getValue().toDouble();
  bool bLeftRotate = pLeftRotate.getValue().toBool();
  bool bRightRotate = pRightRotate.getValue().toBool();

  // Draw a horizontal line and the y coordinate change(vertical)

  Utils* utils = Context::getInstance().getUtils();
  ItemUtils* itemUtils = Context::getInstance().getItemUtils();

  int dpi = 72.;
  if (scene() != 0)
  {
    AbstractScene* myScene = dynamic_cast<AbstractScene*>(scene());
    if (myScene != 0)
    {
      dpi = myScene->getContext().getDpiX();
    }
  }

  WorldTransformer transf = utils->getTransformGeo(planarBox, boxMM);
  transf.setMirroring(false);

  int zone = te::map::CalculatePlanarZone(geoBox);
  if(zone < 0 || zone > 60)
  {
    return;
  }

  QFont ft(fontFamily.c_str(), textPointSize);

  double y1 = initVerticalLines(geoBox);
  double y2 = geoBox.getUpperRightY();

  y1 -= 3 * verticalGap;
  y2 += 3 * verticalGap;

  for( ; y1 <= y2 ; y1 += verticalGap)
  {
    te::gm::Envelope env(geoBox.getLowerLeftX() - 3 * horizontalGap, y1, geoBox.getUpperRightX() + 3 * horizontalGap, y1);

    te::gm::LinearRing* line = 0;
    line = utils->addCoordsInX(env, y1, verticalGap);
    
    // Line curvature: of latlong to planar;
    // Draw line: planar to mm
    utils->remapToPlanar(line, zone);
    utils->convertToMillimeter(transf, line);

    //here we clip the line using the boundaries of the item in MM
    te::gm::Geometry* rectPolygon = te::gm::GetGeomFromEnvelope(&boxMM, line->getSRID());
    te::gm::Geometry* clippedGeometry = line->intersection(rectPolygon);
    if (clippedGeometry == 0 || clippedGeometry->isValid() == false)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    te::gm::LineString* lineString = dynamic_cast<te::gm::LineString*>(clippedGeometry);
    if (lineString == 0)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    te::gm::Envelope* ev = const_cast<te::gm::Envelope*>(lineString->getMBR());

    //QLineF lne(ev->getLowerLeftX(), ev->getLowerLeftY(), ev->getUpperRightX(), ev->getUpperRightY());
    size_t numCoords = lineString->size();
    te::gm::Point* firstPoint = lineString->getPointN(0);
    te::gm::Point* lastPoint = lineString->getPointN(numCoords - 1);


    QLineF gridLine(firstPoint->getX(), firstPoint->getY(), lastPoint->getX(), lastPoint->getY());
    QLineF leftLine(boxMM.getLowerLeftX(), boxMM.getLowerLeftY(), boxMM.getLowerLeftX(), boxMM.getUpperRightY());
    QLineF rightLine(boxMM.getUpperRightX(), boxMM.getLowerLeftY(), boxMM.getUpperRightX(), boxMM.getUpperRightY());

    m_horizontalLines.push_back(gridLine);

    std::string text = utils->convertDecimalToDegree(y1, showDegreesText, showMinutesText, showSecondsText);
    QString qText = text.c_str();

    QPainterPath textObject = itemUtils->textToVector(qText, ft, dpi, QPointF(), 0);

    QRectF rectF(textObject.boundingRect());

    QPointF intersectonPoint;
    if (gridLine.intersect(leftLine, &intersectonPoint) == QLineF::BoundedIntersection)
    {
      calculateLeft(gridLine, rectF, qText, bLeftRotate, verticalDisplacement);
    }
    if (gridLine.intersect(rightLine, &intersectonPoint) == QLineF::BoundedIntersection)
    {
      calculateRight(gridLine, rectF, qText, bRightRotate, verticalDisplacement);
    }

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

  const Property& pFontFamily = m_controller->getProperty(settingsConfig.getFontText());
  const Property& pTextPointSize = m_controller->getProperty(settingsConfig.getPointTextSize());
  const Property& pHorizontalGap = m_controller->getProperty(settingsConfig.getLneHrzGap());
  const Property& pVerticalGap = m_controller->getProperty(settingsConfig.getLneVrtGap());
  const Property& pShowDegreesText = m_controller->getProperty(settingsConfig.getDegreesText());
  const Property& pShowMinutesText = m_controller->getProperty(settingsConfig.getMinutesText());
  const Property& pShowSecondsText = m_controller->getProperty(settingsConfig.getSecondsText());
  const Property& pVerticalDisplacement = m_controller->getProperty(settingsConfig.getLneVrtDisplacement());
  const Property& pHorizontalDisplacement = m_controller->getProperty(settingsConfig.getLneHrzDisplacement());
  const Property& pTopRotate = m_controller->getProperty(settingsConfig.getTopRotateText());
  const Property& pBottomRotate = m_controller->getProperty(settingsConfig.getBottomRotateText());
  
  std::string fontFamily = pFontFamily.getValue().toString();
  int textPointSize = pTextPointSize.getValue().toInt();
  double horizontalGap = pHorizontalGap.getValue().toDouble();
  double verticalGap = pVerticalGap.getValue().toDouble();
  bool showDegreesText = pShowDegreesText.getValue().toBool();
  bool showMinutesText = pShowMinutesText.getValue().toBool();
  bool showSecondsText = pShowSecondsText.getValue().toBool();
  double verticalDisplacement = pVerticalDisplacement.getValue().toDouble();
  double horizontalDisplacement = pHorizontalDisplacement.getValue().toDouble();
  bool bTopRotate = pTopRotate.getValue().toBool();
  bool bBottomRotate = pBottomRotate.getValue().toBool();

  // Draw a vertical line and the x coordinate change(horizontal)

  Utils* utils = Context::getInstance().getUtils();
  ItemUtils* itemUtils = Context::getInstance().getItemUtils();

  int dpi = 72.;
  if (scene() != 0)
  {
    AbstractScene* myScene = dynamic_cast<AbstractScene*>(scene());
    if (myScene != 0)
    {
      dpi = myScene->getContext().getDpiX();
    }
  }
  
  WorldTransformer transf = utils->getTransformGeo(planarBox, boxMM);
  transf.setMirroring(false);

  int zone = te::map::CalculatePlanarZone(geoBox);
  if(zone < 0 || zone > 60)
  {
    return;
  }

  QFont ft(fontFamily.c_str(), textPointSize);

  //as the geographic grid comes from the result of a reprojection, the geographic box sometimes can be smaller than the planar box, maninly when the distance from equator gets higher.
  //when this happens, these calculated lines do not reach the boundaries of the item, and consequently the texts are not drawn in the correct position
  //to fix this, we calculate a grid bigger than we need, and then we clip the lines by making an intertection operation with the item
  double x1 = initHorizontalLines(geoBox);
  double x2 = geoBox.getUpperRightX();

  x1 -= 3 * horizontalGap;
  x2 += 3 * horizontalGap;

  for( ; x1 <= x2 ; x1 += horizontalGap)
  {
    te::gm::Envelope env(x1, geoBox.getLowerLeftY() - 3 * verticalGap, x1, geoBox.getUpperRightY() + 3 * verticalGap);
    te::gm::LinearRing* line = 0;
    line = utils->addCoordsInY(env, x1, horizontalGap);

    // Line curvature: of latlong to planar;
    // Draw line: planar to mm
    utils->remapToPlanar(line, zone);
    utils->convertToMillimeter(transf, line);

    //here we clip the line using the boundaries of the item in MM
    te::gm::Geometry* rectPolygon = te::gm::GetGeomFromEnvelope(&boxMM, line->getSRID());
    te::gm::Geometry* clippedGeometry = line->intersection(rectPolygon);
    if (clippedGeometry == 0 || clippedGeometry->isValid() == false)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    te::gm::LineString* lineString = dynamic_cast<te::gm::LineString*>(clippedGeometry);
    if (lineString == 0)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    te::gm::Envelope* ev = const_cast<te::gm::Envelope*>(lineString->getMBR());
    
    size_t numCoords = lineString->size();
    te::gm::Point* firstPoint = lineString->getPointN(0);
    te::gm::Point* lastPoint = lineString->getPointN(numCoords - 1);


    QLineF gridLine(firstPoint->getX(), firstPoint->getY(), lastPoint->getX(), lastPoint->getY());
    QLineF topLine(boxMM.getLowerLeftX(), boxMM.getUpperRightY(), boxMM.getUpperRightX(), boxMM.getUpperRightY());
    QLineF bottonLine(boxMM.getLowerLeftX(), boxMM.getLowerLeftY(), boxMM.getUpperRightX(), boxMM.getLowerLeftY());
    
    
    m_verticalLines.push_back(gridLine);

    std::string text = utils->convertDecimalToDegree(x1, showDegreesText, showMinutesText, showSecondsText);
    QString qText = text.c_str();

    QPainterPath textObject = itemUtils->textToVector(qText, ft, dpi, QPointF(), 0);
    QRectF rectF(textObject.boundingRect());

    //as the grid lines an be curved, texts must only de drawn in the cases that the grid line reaches the top or the botton of the item bounding rect

    QPointF intersectonPoint;
    if (gridLine.intersect(topLine, &intersectonPoint) == QLineF::BoundedIntersection)
    {
      calculateTop(gridLine, rectF, qText, bTopRotate, horizontalDisplacement);
    }
    if (gridLine.intersect(bottonLine, &intersectonPoint) == QLineF::BoundedIntersection)
    {
      calculateBottom(gridLine, rectF, qText, bBottomRotate, horizontalDisplacement);
    }

    if(line)
    {
      delete line;
      line = 0;
    }

    delete rectPolygon;
    delete clippedGeometry;
  }
}
