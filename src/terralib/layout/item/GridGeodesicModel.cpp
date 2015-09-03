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
  \file GridGeodesicModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridGeodesicModel.h"

#include "../core/pattern/singleton/Context.h"
#include "../core/property/GridSettingsConfigProperties.h"
#include "../../common/StringUtils.h"
#include "../../common/UnitOfMeasure.h"
#include "../../srs/SpatialReferenceSystemManager.h"
/*
#include "../core/ContextItem.h"
#include "../../geometry/Envelope.h"
#include "../../geometry/Coord2D.h"
#include "../core/property/Property.h"
#include "../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../../srs/Config.h"
#include "../core/WorldTransformer.h"
#include "../core/enum/Enums.h"

// STL
#include <string>
#include <sstream> 
#include <cmath>
*/

te::layout::GridGeodesicModel::GridGeodesicModel() 
  : GridMapModel()
  , Observer()
  /*: GridMapModel(new GeodesicGridSettingsConfigProperties),
  m_pointTextSizeCorner(12),
  m_degreesText(true),
  m_minutesText(false),
  m_secondsText(false),
  m_lneCornerHrzDisplacement(10),
  m_lneCornerVrtDisplacement(10),
  m_lowerRightCornerText(false),
  m_upperRightCornerText(false),
  m_lowerLeftCornerText(false),
  m_upperLeftCornerText(false),
  m_visibleCornerTextsText(false),

  // Geodesic: Topographic Map

  m_defineScale(false),
  m_clip(false),
  m_scale(0.),
  m_lneX1(0.),
  m_lneX2(0.),
  m_lneY1(0.),
  m_lneY2(0.),
  m_lneX3(0.),
  m_lneX4(0.),
  m_lneY3(0.),
  m_lneY4(0.)
  */
{
  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getGridGeodesicItem());

  //m_lneVrtDisplacement = 2;
  //m_lneHrzDisplacement = 2;

  int srid = 0;
  te::gm::Envelope geographicBox(0, 0, 10000, 10000);
  bool showDegreesText = true;
  bool showMinutesText = false;
  bool showSecondsText = false;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties
  // Reference Settings
  {
    Property property(0);
    property.setName("geographic_box");
    property.setLabel("Geographic Box");
    property.setValue(geographicBox, dataType->getDataTypeEnvelope());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("srid");
    property.setLabel("Srid");
    property.setValue(srid, dataType->getDataTypeInt());
    m_properties.addProperty(property);
  }

  {
    std::string emptyString;

    Property property(0);
    property.setName("map_name");
    property.setLabel("Map Name");
    property.setValue(emptyString, dataType->getDataTypeStringList());

    Variant v;
    v.setValue(emptyString, dataType->getDataTypeString());
    property.addOption(v);
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("show_degrees_text");
    property.setLabel("Show Degrees Text");
    property.setValue(showDegreesText, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("show_minutes_text");
    property.setLabel("Show Minutes Text");
    property.setValue(showMinutesText, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("show_seconds_text");
    property.setLabel("Show Seconds Text");
    property.setValue(showSecondsText, dataType->getDataTypeBool());
    m_properties.addProperty(property);
  }
}

te::layout::GridGeodesicModel::~GridGeodesicModel()
{
  /*
  if(m_settingsConfig)
  {
    delete m_settingsConfig;
    m_settingsConfig = 0;
  }*/
}

void te::layout::GridGeodesicModel::update(const Subject* subject)
{
  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if(subjectModel == 0)
  {
    return;
  }

  //new properties
  const Property& pNewWidth = subjectModel->getProperty("width");
  const Property& pNewHeight = subjectModel->getProperty("height");
  const Property& pNewWorldBox = subjectModel->getProperty("world_box");
  const Property& pNewSrid = subjectModel->getProperty("srid");

  //current properties
  const Property& pCurrentWidth = this->getProperty("width");
  const Property& pCurrentHeight = this->getProperty("height");
  const Property& pCurrentGeographicBox = this->getProperty("geographic_box");
  const Property& pCurrentSrid = this->getProperty("srid");

  //new values
  double newWidth = pNewWidth.getValue().toDouble();
  double newHeight = pNewHeight.getValue().toDouble();
  int newSrid = pNewSrid.getValue().toInt();
  const te::gm::Envelope& newWorldBox = pNewWorldBox.getValue().toEnvelope();
  te::gm::Envelope newGeographicBox = getWorldBoxInGeographic(newWorldBox, newSrid);

  //current values
  double currentWidth = pCurrentWidth.getValue().toDouble();
  double currentHeight = pCurrentHeight.getValue().toDouble();
  int currentSrid = pCurrentSrid.getValue().toInt();
  te::gm::Envelope currentGeographicBox = pCurrentGeographicBox.getValue().toEnvelope();

  bool doUpdate = false;
  if(newWidth != currentWidth)
  {
    doUpdate = true;
  }
  else if(newHeight != currentHeight)
  {
    doUpdate = true;
  }
  else if(newGeographicBox.equals(currentGeographicBox) == false)
  {
    doUpdate = true;
  }

  if(doUpdate == true)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    GridSettingsConfigProperties settingsConfig;

    Properties properties("");
    properties.addProperty(pNewWidth);
    properties.addProperty(pNewHeight);

    {
      Property property(0);
      property.setName("geographic_box");
      property.setValue(newGeographicBox, dataType->getDataTypeEnvelope());
      properties.addProperty(property);
    }

    te::gm::Envelope defaultGeographicBox(0, 0, 10000, 10000);
    if(newGeographicBox.equals(defaultGeographicBox) == false)
    {
      {
        Property property(0);
        property.setName(settingsConfig.getInitialGridPointX());
        property.setValue(newGeographicBox.getLowerLeftX(), dataType->getDataTypeDouble());
        properties.addProperty(property);
      }
      {
        Property property(0);
        property.setName(settingsConfig.getInitialGridPointY());
        property.setValue(newGeographicBox.getLowerLeftY(), dataType->getDataTypeDouble());
        properties.addProperty(property);
      }
      {
        double horizontalGap = newGeographicBox.getWidth() / 4.;
        Property property(0);
        property.setName(settingsConfig.getLneHrzGap());
        property.setValue(horizontalGap, dataType->getDataTypeDouble());
        properties.addProperty(property);
      }
      {
        double verticalGap = newGeographicBox.getHeight() / 4.;
        Property property(0);
        property.setName(settingsConfig.getLneVrtGap());
        property.setValue(verticalGap, dataType->getDataTypeDouble());
        properties.addProperty(property);
      }
    }

    setProperties(properties);
  }
}

te::gm::Envelope te::layout::GridGeodesicModel::getWorldBoxInGeographic(const te::gm::Envelope& worldBox, int srid)
{
  te::gm::Envelope worldBoxGeographic = worldBox;

  //About units names (SI): terralib5\resources\json\uom.json 
  Utils* utils = Context::getInstance().getUtils();
  te::common::UnitOfMeasurePtr unitPtr = utils->unitMeasure(srid);

  if(!unitPtr)
    return worldBoxGeographic;
  
  std::string unitPtrStr = unitPtr->getName(); 
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if(unitPtrStr.compare("DEGREE") != 0)
  {
    std::string proj4 = utils->proj4DescToGeodesic();

    // Get the id of the projection of destination 
    std::pair<std::string, unsigned int> projGeographic = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4); 

    // Remapping 
    worldBoxGeographic.transform(srid, projGeographic.second);
  }

  return worldBoxGeographic;
}

/*
void te::layout::GridGeodesicModel::draw( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box, int srid )
{
  
}

void te::layout::GridGeodesicModel::drawVerticalLines(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box)
{

}

void te::layout::GridGeodesicModel::drawHorizontalLines(te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box)
{

}

void te::layout::GridGeodesicModel::calculateGaps( te::gm::Envelope box )
{
  te::gm::Coord2D init = box.getLowerLeft();
  te::gm::Coord2D end = box.getUpperRight();

  if(m_initialGridPointX == 0)
  {
    m_initialGridPointX = init.x;
  }
  if(m_initialGridPointY == 0)
  {
    m_initialGridPointY = init.y;
  }

  if(m_srid != TE_UNKNOWN_SRS)
  {
    if(m_lneHrzGap <= 0)
    {
      m_lneHrzGap = std::fabs(init.x - end.x);
      m_lneHrzGap /= 4;
    }
    if(m_lneVrtGap <= 0)
      m_lneVrtGap = m_lneHrzGap;
  }
  else
  {
    m_lneVrtGap = m_lneHrzGap = 0.001;
  }
}

void te::layout::GridGeodesicModel::setPlanarBox( te::gm::Envelope box )
{
  m_planarBox = box;
}

void te::layout::GridGeodesicModel::setVisibleAllTexts( bool visible )
{
  setVisibleCornerTextsText(visible);
}

void te::layout::GridGeodesicModel::setVisibleCornerTextsText( bool visible )
{
  m_visibleCornerTextsText = visible;

  m_lowerRightCornerText = visible;
  m_upperRightCornerText = visible;
  m_lowerLeftCornerText = visible;
  m_upperLeftCornerText = visible;
}

bool te::layout::GridGeodesicModel::isVisibleCornerTextsText()
{
  return m_visibleCornerTextsText;
}

te::gm::Envelope te::layout::GridGeodesicModel::getPlanarBox()
{
  return m_planarBox;
}

bool te::layout::GridGeodesicModel::isDegreesText()
{
  return m_degreesText;
}

bool te::layout::GridGeodesicModel::isMinutesText()
{
  return m_minutesText;
}

bool te::layout::GridGeodesicModel::isSecondsText()
{
  return m_secondsText;
}

void te::layout::GridGeodesicModel::setSRID( int srid )
{
  m_srid = srid;
}

*/