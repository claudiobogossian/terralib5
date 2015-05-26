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
#include <algorithm>

te::layout::MapModel::MapModel() :
  m_mapDisplacementX(10),
  m_mapDisplacementY(10),
  m_systematic(0),
  m_fixedScale(false),
  m_loadedLayer(false)
{
  m_type = Enums::getInstance().getEnumObjectType()->getMapItem();

  m_box = te::gm::Envelope(0., 0., 150., 120.);
  m_mapBoxMM = m_box;

  //will calculated map box with displacement
  setBox(m_box);

  m_backgroundColor = te::color::RGBAColor(255, 255, 255, 0);

  m_mapbackgroundColor = te::color::RGBAColor(255, 255, 255, 0);
  m_enableChildren = true;
}

te::layout::MapModel::~MapModel()
{
  m_layers.clear();

  if(m_systematic)
  {
    delete m_systematic;
    m_systematic = 0;
  }
}

te::layout::Properties* te::layout::MapModel::getProperties() const
{
  ItemModelObservable::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
    return m_properties;

  std::string name = "Choice";

  Property pro_mapchoice(m_hashCode);
  pro_mapchoice.setName("mapChoice");
  pro_mapchoice.setValue(name, dataType->getDataTypeMapChoice());
  pro_mapchoice.setMenu(true);
  m_properties->addProperty(pro_mapchoice);

  Property pro_layersNames = getLayerNamesProperty();
  m_properties->addProperty(pro_layersNames);

  Property pro_layers = getLayersGenericVariant();
  m_properties->addProperty(pro_layers);

  Property pro_mapbackgroundcolor(m_hashCode);
  pro_mapbackgroundcolor.setName("map_color");
  pro_mapbackgroundcolor.setValue(m_mapbackgroundColor, dataType->getDataTypeColor());
  pro_mapbackgroundcolor.setMenu(true);
  m_properties->addProperty(pro_mapbackgroundcolor);

  Property pro_fixed(m_hashCode);
  pro_fixed.setName("fixedScale");
  pro_fixed.setValue(m_fixedScale, dataType->getDataTypeBool());
  m_properties->addProperty(pro_fixed);

  Property pro_mapDisplacementX(m_hashCode);
  pro_mapDisplacementX.setName("map_displacementX");
  pro_mapDisplacementX.setValue(m_mapDisplacementX, dataType->getDataTypeDouble());  
  m_properties->addProperty(pro_mapDisplacementX);

  Property pro_mapDisplacementY(m_hashCode);
  pro_mapDisplacementY.setName("map_displacementY");
  pro_mapDisplacementY.setValue(m_mapDisplacementY, dataType->getDataTypeDouble());  
  m_properties->addProperty(pro_mapDisplacementY);

  return m_properties;
}

void te::layout::MapModel::updateProperties( te::layout::Properties* properties, bool notify )
{
  ItemModelObservable::updateProperties(properties, false);

  Properties* vectorProps = const_cast<Properties*>(properties);  
  
  Property pro_layerNames = vectorProps->contains("layerNames");
  if(!pro_layerNames.isNull())
  {
    m_layerNames.clear();
    std::vector<Variant> names = pro_layerNames.getOptionChoices();
    std::vector<Variant>::const_iterator it = names.begin();
    for( ; it != names.end() ; ++it)
    {
      Variant v = (*it);
      std::string name = v.toString();
      m_layerNames.push_back(name);
    }
  }

  Property pro_layers = vectorProps->contains("layers");
  if(!pro_layers.isNull())
  {
    GenericVariant v = pro_layers.getValue().toGenericVariant();
    m_layers = v.toLayerList();
  }

  Property pro_mapbackgroundcolor = vectorProps->contains("map_color");
  if(!pro_mapbackgroundcolor.isNull())
  {
    m_mapbackgroundColor = pro_mapbackgroundcolor.getValue().toColor();
  }

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
    
  updateVisitors();

  if(notify)
  {
    ContextItem context;
    notifyAll(context);
  }  
}

double te::layout::MapModel::getScale()
{
  if(m_layers.empty())
  {
    return 0;
  }

  std::list<te::map::AbstractLayerPtr>::iterator it;
  it = m_layers.begin();

  te::map::AbstractLayerPtr layer = (*it);

  if(layer.get() == 0)
    return 0;

  // World box: coordinates in the same SRS as the layer
  te::gm::Envelope worldBox = layer->getExtent();
  
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

  if(m_layers.empty())
  {
    return worldBox;
  }

  std::list<te::map::AbstractLayerPtr>::iterator it;
  it = m_layers.begin();

  te::map::AbstractLayerPtr layer = (*it);
  
  if(layer.get() == 0)
    return worldBox;

  // World box: coordinates in the same SRS as the layer
  worldBox = layer->getExtent();
  int srid = layer->getSRID();
  
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

  if(m_layers.empty())
  {
    return unitPtr;
  }

  std::list<te::map::AbstractLayerPtr>::iterator it;
  it = m_layers.begin();

  te::map::AbstractLayerPtr layer = (*it);
  
  //About units names (SI): terralib5\resources\json\uom.json 
  
  int srid = layer->getSRID();

  Utils* utils = Context::getInstance().getUtils();
  unitPtr = utils->unitMeasure(srid);
  return unitPtr;
}

te::gm::Envelope te::layout::MapModel::getWorldInDegrees()
{
  te::gm::Envelope worldBox;
  
  if(m_layers.empty())
  {
    return worldBox;
  }

  std::list<te::map::AbstractLayerPtr>::iterator it;
  it = m_layers.begin();

  te::map::AbstractLayerPtr layer = (*it);
  
  if(layer.get() == 0)
    return worldBox;

  // World box: coordinates in the same SRS as the layer
  worldBox = layer->getExtent();
  int srid = layer->getSRID();
    
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

  recalculateMapBoxMM();
}

te::gm::Envelope te::layout::MapModel::getMapBox()
{
  return m_mapBoxMM;
}

void te::layout::MapModel::setPosition( const double& x, const double& y )
{
  ItemModelObservable::setPosition(x, y);

  recalculateMapBoxMM();
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

  if(m_layers.empty())
  {
    return;
  }

  std::list<te::map::AbstractLayerPtr>::iterator it;
  it = m_layers.begin();

  te::map::AbstractLayerPtr layer = (*it);
  
  if(layer.get() == 0)
    return;

  int srid = layer->getSRID();  
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

  if(m_layers.empty())
  {
    return result;
  }

  std::list<te::map::AbstractLayerPtr>::iterator it;
  it = m_layers.begin();

  te::map::AbstractLayerPtr layer = (*it);
  
  if(layer.get() == 0)
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

  if(m_layers.empty())
  {
    return worldBox;
  }

  std::list<te::map::AbstractLayerPtr>::iterator it;
  it = m_layers.begin();

  te::map::AbstractLayerPtr layer = (*it);

  if(layer.get() == 0)
    return worldBox;

  // World box: coordinates in the same SRS as the layer
  worldBox = layer->getExtent();
  int srid = layer->getSRID();

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

bool te::layout::MapModel::isLoadedLayer()
{
  return m_loadedLayer;
}

void te::layout::MapModel::setDisplacementX( double displacement )
{
  m_mapDisplacementX = displacement;

  recalculateMapBoxMM();
}

void te::layout::MapModel::setDisplacementY( double displacement )
{
  m_mapDisplacementY = displacement;

  recalculateMapBoxMM();
}

void te::layout::MapModel::recalculateMapBoxMM()
{
  m_mapBoxMM.m_llx = m_box.m_llx + m_mapDisplacementX;
  m_mapBoxMM.m_lly = m_box.m_lly + m_mapDisplacementY;
  m_mapBoxMM.m_urx = m_box.m_urx - m_mapDisplacementX;
  m_mapBoxMM.m_ury = m_box.m_ury - m_mapDisplacementY;
}

bool te::layout::MapModel::addLayer( te::map::AbstractLayerPtr layer )
{
  if(!layer)
    return false;

  std::list<te::map::AbstractLayerPtr>::iterator it;
  it = std::find(m_layers.begin(), m_layers.end(), layer);
  if(it != m_layers.end())
  {
    if((*it)->getId() == layer->getId())
      return false;
  }

  m_loadedLayer = true;

  m_layers.push_back(layer);

  updateVisitors();

  return true;
}

bool te::layout::MapModel::removeLayer( te::map::AbstractLayerPtr layer )
{
  bool result = false;
  std::list<te::map::AbstractLayerPtr>::iterator it;
  it = std::find(m_layers.begin(), m_layers.end(), layer);
  if(it != m_layers.end())
  {
    m_layers.erase(it);
    result = true;
  }

  if(m_layers.empty())
  {
    m_loadedLayer = false;
  }
  return result;
}

void te::layout::MapModel::clear()
{
  m_layers.clear();
  m_loadedLayer = false;
}

te::layout::Property te::layout::MapModel::getLayerNamesProperty() const
{
  Property prop;
  prop.setName("layerNames");

  if(m_layers.empty())
  {
    return prop;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  std::vector<std::string> stringList = findLayerNames();

  std::vector<std::string>::const_iterator it;
  it = stringList.begin();

  for( ; it != stringList.end() ; ++it)
  {
    std::string name = (*it);

    Variant v;
    v.setValue(name, dataType->getDataTypeString());
    prop.addOption(v);
  }

  prop.setEditable(false);
  prop.setVisible(false);
  return prop;
}

te::layout::Property te::layout::MapModel::getLayersGenericVariant() const
{
  Property prop;
  prop.setName("layers");
  
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  GenericVariant gv;
  gv.setList(m_layers, dataType->getDataTypeLayerList());

  prop.setValue(gv, dataType->getDataTypeGenericVariant());

  prop.setEditable(false);
  prop.setVisible(false);
  return prop;
}

std::list<te::map::AbstractLayerPtr> te::layout::MapModel::getLayers()
{
  return m_layers;
}

std::vector<std::string> te::layout::MapModel::getLayerNames()
{
  return m_layerNames;
}

std::vector<std::string> te::layout::MapModel::findLayerNames() const
{
  std::vector<std::string> stringList;

  std::list<te::map::AbstractLayerPtr>::const_iterator it;
  it = m_layers.begin();

  for( ; it != m_layers.end() ; ++it)
  {
    te::map::AbstractLayerPtr layer = (*it);
    std::string name = layer->getTitle();
    stringList.push_back(name);
  }

  return stringList;
}

te::gm::Envelope te::layout::MapModel::maxLayerExtent()
{
  te::gm::Envelope box;

  if(m_layers.empty())
  {
    return box;
  }
  
  std::list<te::map::AbstractLayerPtr>::const_iterator it;
  it = m_layers.begin();

  te::map::AbstractLayerPtr layer = (*it);

  box = layer->getExtent();

  return box;
}


