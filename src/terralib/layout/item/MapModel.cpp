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

// STL
#include <vector>
#include <string>
#include <sstream> // std::stringstream

te::layout::MapModel::MapModel() :
  m_mapDisplacementX(15),
  m_mapDisplacementY(15)
{
  m_box = te::gm::Envelope(0., 0., 150., 120.);
  m_mapBoxMM = m_box;
}

te::layout::MapModel::~MapModel()
{

}

void te::layout::MapModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  context.setPixmap(pixmap);
  notifyAll(context);
}

void te::layout::MapModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

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

  return (1. / factor ) /(wMM / area);
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
