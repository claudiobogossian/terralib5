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
  \file GridPlanarModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridPlanarModel.h"

#include "../core/pattern/singleton/Context.h"
#include "../../common/StringUtils.h"
#include "../../common/UnitOfMeasure.h"
#include "../../srs/SpatialReferenceSystemManager.h"

/*
#include "GridPlanarModel.h"
#include "../core/ContextItem.h"

#include "../../geometry/Envelope.h"
#include "../../geometry/Coord2D.h"
#include "../core/property/Property.h"
#include "../core/property/PlanarGridSettingsConfigProperties.h"
#include "../core/enum/Enums.h"

// STL
#include <string>
#include <sstream> 

*/


te::layout::GridPlanarModel::GridPlanarModel()
  : GridMapModel()
  , NewObserver()
{
  LayoutUnit unit(StyleKilometer);
  int srid = 0;
  te::gm::Envelope planarBox(0, 0, 10000, 10000);

  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getGridPlanarItem());

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  //adding properties
  {
    Property property(0);
    property.setName("unit");
    property.setLabel("Unit");
    property.setValue((int)unit, dataType->getDataTypeInt());
    m_properties.addProperty(property);
  }

  // Reference Settings
  {
    Property property(0);
    property.setName("planar_box");
    property.setLabel("Planar Box");
    property.setValue(planarBox, dataType->getDataTypeEnvelope());
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

//  m_type = ;

  //m_lneVrtDisplacement = 2;
  //m_lneHrzDisplacement = 2;
}

te::layout::GridPlanarModel::~GridPlanarModel()
{
  /*
  if(m_settingsConfig)
  {
    delete m_settingsConfig;
    m_settingsConfig = 0;
  }
  */
}

void te::layout::GridPlanarModel::update(const Subject* subject)
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
  const Property& pCurrentPlanarBox = this->getProperty("planar_box");
  const Property& pCurrentSrid = this->getProperty("srid");

  //new values
  double newWidth = pNewWidth.getValue().toDouble();
  double newHeight = pNewHeight.getValue().toDouble();
  int newSrid = pNewSrid.getValue().toInt();
  const te::gm::Envelope& newWorldBox = pNewWorldBox.getValue().toEnvelope();
  te::gm::Envelope newPlanarBox = getWorldBoxInPlanar(newWorldBox, newSrid);

  //current values
  double currentWidth = pCurrentWidth.getValue().toDouble();
  double currentHeight = pCurrentHeight.getValue().toDouble();
  int currentSrid = pCurrentSrid.getValue().toInt();
  te::gm::Envelope currentPlanarBox = pCurrentPlanarBox.getValue().toEnvelope();

  bool doUpdate = false;
  if(newWidth != currentWidth)
  {
    doUpdate = true;
  }
  else if(newHeight != currentHeight)
  {
    doUpdate = true;
  }
  else if(newPlanarBox.equals(currentPlanarBox) == false)
  {
    doUpdate = true;
  }

  if(doUpdate == true)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property property(0);
    property.setName("planar_box");
    property.setValue(newPlanarBox, dataType->getDataTypeEnvelope());

    Properties properties("");
    properties.addProperty(pNewWidth);
    properties.addProperty(pNewHeight);
    properties.addProperty(property);

    setProperties(properties);
  }
}

te::gm::Envelope te::layout::GridPlanarModel::getWorldBoxInPlanar(const te::gm::Envelope& worldBox, int srid)
{
  te::gm::Envelope worldBoxPlanar = worldBox;

  //About units names (SI): terralib5\resources\json\uom.json 
  Utils* utils = Context::getInstance().getUtils();
  te::common::UnitOfMeasurePtr unitPtr = utils->unitMeasure(srid);

  if(!unitPtr)
    return worldBoxPlanar;
  
  std::string unitPtrStr = unitPtr->getName(); 
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if(unitPtrStr.compare("DEGREE") == 0)
  {
    int zone = utils->calculatePlanarZone(worldBox);
    std::string proj4 = utils->proj4DescToPlanar(zone);
    
    // Get the id of the projection of destination 
    std::pair<std::string, unsigned int> projPlanar = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4); 

    // Remapping 
    worldBoxPlanar.transform(srid, projPlanar.second);
  }

  return worldBoxPlanar;
}

/*
void te::layout::GridPlanarModel::calculateGaps( te::gm::Envelope box )
{
  if(m_lneHrzGap <= 0)
  {
    m_lneHrzGap = m_mapScale * 0.05;
  }

  if(m_lneVrtGap <= 0)
  {
    m_lneVrtGap = m_lneHrzGap;
  }

  if(m_initialGridPointX == 0)
  {
    m_initialGridPointX = box.getLowerLeftX();
  }
  if(m_initialGridPointY == 0)
  {
    m_initialGridPointY = box.getLowerLeftY();
  }
}

te::layout::LayoutUnit te::layout::GridPlanarModel::getUnit()
{
  return m_unit;
}

void te::layout::GridPlanarModel::draw( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box, int srid )
{

}

void te::layout::GridPlanarModel::drawVerticalLines( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box )
{

}

void te::layout::GridPlanarModel::drawHorizontalLines( te::map::Canvas* canvas, Utils* utils, te::gm::Envelope box )
{

}
*/


