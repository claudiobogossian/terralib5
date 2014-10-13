/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file MapModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapModel.h"
#include "../core/ContextItem.h"
#include "../../maptools/Canvas.h"
#include "../../srs/SpatialReferenceSystemManager.h"
#include "../../common/StringUtils.h"
#include "../core/pattern/singleton/Context.h"
#include "../core/Systematic.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/WorldTransformer.h"
#include "../../common/STLUtils.h"
#include "../../geometry/Polygon.h"
#include "../../geometry/LinearRing.h"
#include "../core/enum/Enums.h"
#include "../core/property/SharedProperties.h"

// STL
#include <vector>
#include <string>
#include <sstream> 

te::layout::MapModel::MapModel() :
  m_mapDisplacementX(0),
  m_mapDisplacementY(0),
  m_systematic(0),
  m_fixedScale(false)
{
  m_type = Enums::getInstance().getEnumObjectType()->getMapItem();

  m_box = te::gm::Envelope(0., 0., 150., 120.);
  m_mapBoxMM = m_box;

  m_backgroundColor = te::color::RGBAColor(192, 192, 192, 255);

  m_mapbackgroundColor = te::color::RGBAColor(192, 192, 192, 255);
}

te::layout::MapModel::~MapModel()
{
  if(m_systematic)
  {
    delete m_systematic;
    m_systematic = 0;
  }
}

void te::layout::MapModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();
  
  if((!canvas) || (!utils))
    return;

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);

  drawBackground(context);
  
  context.setPixmap(pixmap);
  notifyAll(context);
}

te::layout::Properties* te::layout::MapModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pro_fixed;
  pro_fixed.setName("fixedScale");
  pro_fixed.setId("unknown");
  pro_fixed.setValue(m_fixedScale, dataType->getDataTypeBool());
  m_properties->addProperty(pro_fixed);

  Property pro_mapDisplacementX;
  pro_mapDisplacementX.setName("map_displacementX");
  pro_mapDisplacementX.setValue(m_mapDisplacementX, dataType->getDataTypeDouble());  
  m_properties->addProperty(pro_mapDisplacementX);

  Property pro_mapDisplacementY;
  pro_mapDisplacementY.setName("map_displacementY");
  pro_mapDisplacementY.setValue(m_mapDisplacementY, dataType->getDataTypeDouble());  
  m_properties->addProperty(pro_mapDisplacementY);

  return m_properties;
}

void te::layout::MapModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);
  
  Property pro_fixed = vectorProps->contains("fixedScale");
  if(!pro_fixed.isNull())
  {
    m_fixedScale = pro_fixed.getValue().toBool();
  }

  Property pro_mapDisplacementX = vectorProps->contains("map_displacementX");
  if(!pro_mapDisplacementX.isNull())
  {
    double d_differenceX = 0;
    if(m_mapDisplacementX < pro_mapDisplacementX.getValue().toDouble())
    {
      d_differenceX = pro_mapDisplacementX.getValue().toDouble() - m_mapDisplacementX;
      m_box.m_urx = m_box.m_urx + d_differenceX;
      m_mapBoxMM.m_urx = m_mapBoxMM.m_urx + d_differenceX;
    }
    else
    {
      d_differenceX = m_mapDisplacementX - pro_mapDisplacementX.getValue().toDouble();
      m_box.m_urx = m_box.m_urx - d_differenceX;
      m_mapBoxMM.m_urx = m_mapBoxMM.m_urx - d_differenceX;
    }
    m_mapDisplacementX = pro_mapDisplacementX.getValue().toDouble();
  }

  Property pro_mapDisplacementY = vectorProps->contains("map_displacementY");
  if(!pro_mapDisplacementY.isNull())
  {
    double d_differenceY = 0;
    if(m_mapDisplacementY < pro_mapDisplacementY.getValue().toDouble())
    {
      d_differenceY = pro_mapDisplacementY.getValue().toDouble() - m_mapDisplacementY; 
      m_box.m_ury = m_box.m_ury + d_differenceY;
      m_mapBoxMM.m_ury = m_mapBoxMM.m_ury + d_differenceY;
    }
    else
    {
      d_differenceY = m_mapDisplacementY - pro_mapDisplacementY.getValue().toDouble();
      m_box.m_ury = m_box.m_ury - d_differenceY;
      m_mapBoxMM.m_ury = m_mapBoxMM.m_ury - d_differenceY;
    }
    m_mapDisplacementY = pro_mapDisplacementY.getValue().toDouble();
  }

  if(m_box.equals(m_mapBoxMM))
  {
    m_mapbackgroundColor = m_backgroundColor;
  }
    
  updateVisitors();
}

bool te::layout::MapModel::refreshLayer( te::map::AbstractLayerPtr layer )
{
  if(m_layer)
  {
    if(m_layer->getId() == layer->getId())
      return false;
  }

  m_layer = layer;
  
  updateVisitors();

  return true;
}

te::map::AbstractLayerPtr te::layout::MapModel::getLayer()
{
  return m_layer;
}

double te::layout::MapModel::getScale()
{
  if(!m_layer)
    return 0;

  if(m_layer.get() == 0)
    return 0;

  // World box: coordinates in the same SRS as the layer
  te::gm::Envelope worldBox = m_layer->getExtent();
  int srid = m_layer->getSRID();
  
  //About units names (SI): terralib5\resources\json\uom.json 
  te::common::UnitOfMeasurePtr unitPtr = unitMeasureLayer();

  if(!unitPtr)
    return 0;

  std::string nameUnit = unitPtr->getName();
  nameUnit = te::common::Convert2UCase(nameUnit);

  double		fx;
  double		fy;
  double		wMM;
  double		factor;
  double		area;

  wMM = m_box.getWidth();
  fx = m_box.getWidth()/worldBox.getWidth();
  fy = m_box.getHeight()/worldBox.getHeight();

  if (fx > fy)
  {
    factor = fy;
    area = (int)(factor * worldBox.getWidth() + .5);
  }
  else
  {
    factor = fx;
    area = (int)(factor * worldBox.getHeight() + .5);
  }
  
  if (nameUnit.compare("METRE") == 0)
    wMM /= 1000.;
  else if (nameUnit.compare("KILOMETRE") == 0)
    wMM /= 1000000.;
  else if (nameUnit.compare("FOOT") == 0)
    wMM /= (12. * 25.4);
  else if (nameUnit.compare("DEGREE") == 0)
    wMM /= 110000000.;

  double scale = (1. / factor ) /(wMM / area);
  if(m_systematic && m_fixedScale)
  {
    scale = m_systematic->getScale();
  }
  return scale;
}

te::gm::Envelope te::layout::MapModel::getWorldInMeters()
{
  te::gm::Envelope worldBox;

  if(!m_layer)
    return worldBox;

  if(m_layer.get() == 0)
    return worldBox;

  // World box: coordinates in the same SRS as the layer
  worldBox = m_layer->getExtent();
  int srid = m_layer->getSRID();
  
  //About units names (SI): terralib5\resources\json\uom.json 
  te::common::UnitOfMeasurePtr unitPtr = unitMeasureLayer();
  
  if(!unitPtr)
    return worldBox;
  
  std::string unitPtrStr = unitPtr->getName(); 
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if(unitPtrStr.compare("DEGREE") == 0)
  {

    Utils* utils = Context::getInstance().getUtils();

    int zone = utils->calculatePlanarZone(worldBox);
    std::string proj4 = utils->proj4DescToPlanar(zone);
    
    // Get the id of the projection of destination 
    std::pair<std::string, unsigned int> projMeters = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4); 

    // Remapping 
    worldBox.transform(srid, projMeters.second);   
  }

  return worldBox;
}

te::common::UnitOfMeasurePtr te::layout::MapModel::unitMeasureLayer()
{
  te::common::UnitOfMeasurePtr unitPtr;

  if(!m_layer)
    return unitPtr;

  //About units names (SI): terralib5\resources\json\uom.json 
  
  int srid = m_layer->getSRID();

  Utils* utils = Context::getInstance().getUtils();
  unitPtr = utils->unitMeasure(srid);
  return unitPtr;
}

te::gm::Envelope te::layout::MapModel::getWorldInDegrees()
{
  te::gm::Envelope worldBox;
  
  if(!m_layer)
    return worldBox;

  if(m_layer.get() == 0)
    return worldBox;

  // World box: coordinates in the same SRS as the layer
  worldBox = m_layer->getExtent();
  int srid = m_layer->getSRID();
    
  //About units names (SI): terralib5\resources\json\uom.json 
  te::common::UnitOfMeasurePtr unitPtr = unitMeasureLayer();

  if(!unitPtr)
    return worldBox;

  std::string unitPtrStr = unitPtr->getName(); 
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if(unitPtrStr.compare("DEGREE") != 0)
  {
    Utils* utils = Context::getInstance().getUtils();
    std::string proj4 = utils->proj4DescToGeodesic();

    // Get the id of the projection of destination 
    std::pair<std::string, unsigned int> projMeters = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4); 

    // Remapping 
    worldBox.transform(srid, projMeters.second);
  }

  return worldBox;
}

void te::layout::MapModel::setBox( te::gm::Envelope box )
{
  ItemModelObservable::setBox(box);

  m_mapBoxMM.m_llx = box.m_llx + m_mapDisplacementX;
  m_mapBoxMM.m_lly = box.m_lly + m_mapDisplacementY;
  m_mapBoxMM.m_urx = box.m_urx - m_mapDisplacementX;
  m_mapBoxMM.m_ury = box.m_ury - m_mapDisplacementY;
}

te::gm::Envelope te::layout::MapModel::getMapBox()
{
  return m_mapBoxMM;
}

void te::layout::MapModel::setPosition( const double& x, const double& y )
{
  ItemModelObservable::setPosition(x, y);

  m_mapBoxMM.m_llx = m_box.m_llx + m_mapDisplacementX;
  m_mapBoxMM.m_lly = m_box.m_lly + m_mapDisplacementY;
  m_mapBoxMM.m_urx = m_box.m_urx - m_mapDisplacementX;
  m_mapBoxMM.m_ury = m_box.m_ury - m_mapDisplacementY;
}

double te::layout::MapModel::getDisplacementX()
{
  return m_mapDisplacementX;
}

double te::layout::MapModel::getDisplacementY()
{
  return m_mapDisplacementY;
}

void te::layout::MapModel::setSystematic( Systematic* systematic )
{
  if(m_systematic)
  {
    delete m_systematic;
    m_systematic = 0;
  }

  m_systematic = systematic;
}

void te::layout::MapModel::generateSystematic( te::gm::Coord2D coord )
{
  if(!m_systematic)
    return;

  if(!m_layer)
    return;

  if(m_layer.get() == 0)
    return;

  int srid = m_layer->getSRID();  
  if(srid == 0)
    return;

  setFixedScale(true);

  double			        height = 0.;
  double			        width = 0.;
  double			        x = coord.x;
  double			        y = coord.y;
  te::gm::Coord2D	    lowerLeft;
  te::gm::Coord2D     upperRight;

  height = m_systematic->getHeight();
  width = m_systematic->getWidth();

  te::gm::Envelope worldBox = getWorldInMeters();

  Utils* utils = Context::getInstance().getUtils();
  WorldTransformer transf = utils->getTransformGeo(worldBox, m_mapBoxMM);
  transf.setMirroring(false);

  transf.system2Tosystem1(x, y, x, y);

  x = floor(x/width);
  y = floor(y/height);

  lowerLeft = te::gm::Coord2D(x * width, y * height);
  upperRight = te::gm::Coord2D((x + 1) * width, (y + 1) * height);
    
  te::gm::LinearRing* lneOut0 = new te::gm::LinearRing(5, te::gm::LineStringType);
  lneOut0->setPointN(0, te::gm::Point(lowerLeft.x, upperRight.y));
  lneOut0->setPointN(1, te::gm::Point(lowerLeft.x, lowerLeft.y)); 
  lneOut0->setPointN(2, te::gm::Point(upperRight.x, lowerLeft.y));
  lneOut0->setPointN(3, te::gm::Point(upperRight.x, upperRight.y)); 
  lneOut0->setPointN(4, te::gm::Point(lowerLeft.x, upperRight.y)); 

  te::gm::Polygon* pol = new te::gm::Polygon(1, te::gm::PolygonType, 0, &worldBox);
  pol->setRingN(0, lneOut0);

  const te::gm::Envelope* polEnv = pol->getMBR();

  m_worldBox.m_llx = polEnv->m_llx;
  m_worldBox.m_lly = polEnv->m_lly;
  m_worldBox.m_urx = polEnv->m_urx;
  m_worldBox.m_ury = polEnv->m_ury;

  updateVisitors();
}

bool te::layout::MapModel::isFixedScale()
{
  return m_fixedScale;
}

void te::layout::MapModel::setFixedScale( bool fixed )
{
  m_fixedScale = fixed;
}

bool te::layout::MapModel::isPlanar()
{
  bool result = false;

  if(!m_layer)
    return result;

  if(m_layer.get() == 0)
    return result;
  
  //About units names (SI): terralib5\resources\json\uom.json 
  te::common::UnitOfMeasurePtr unitPtr = unitMeasureLayer();

  if(!unitPtr)
    return result;

  std::string unitPtrStr = unitPtr->getName(); 
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if(unitPtrStr.compare("DEGREE") != 0)
  {
    result = true;
  }

  return result;
}

te::gm::Envelope te::layout::MapModel::getWorldBox()
{
  te::gm::Envelope worldBox;

  if(!m_layer)
    return worldBox;

  if(m_layer.get() == 0)
    return worldBox;

  // World box: coordinates in the same SRS as the layer
  worldBox = m_layer->getExtent();
  int srid = m_layer->getSRID();

  if(!m_worldBox.isValid())
    return worldBox;

  //Planar World
  worldBox = m_worldBox;

  if(!isPlanar())
  {
    Utils* utils = Context::getInstance().getUtils();
    std::string proj4 = utils->proj4DescToGeodesic();

    // Get the id of the projection of destination 
    std::pair<std::string, unsigned int> projMeters = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4); 

    // Remapping 
    worldBox.transform(srid, projMeters.second); 
  }

  return worldBox;
}

std::map<te::gm::Point*, std::string> te::layout::MapModel::getTextMapAsObjectInfo()
{
  std::map<te::gm::Point*, std::string>  map;

  return map;
}

void te::layout::MapModel::setMapBackgroundColor( te::color::RGBAColor color )
{
  m_mapbackgroundColor = color;
}

te::color::RGBAColor te::layout::MapModel::getMapBackgroundColor()
{
  return m_mapbackgroundColor;
}
