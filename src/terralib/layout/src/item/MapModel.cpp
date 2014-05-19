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
#include "ContextItem.h"
#include "Context.h"
#include "../../../maptools/Canvas.h"
#include "terralib4/terralib/kernel/TeDefines.h"
#include "../../../srs/SpatialReferenceSystemManager.h"
#include "../../../common/StringUtils.h"

// STL
#include <vector>
#include <string>
#include <sstream> // std::stringstream

te::layout::MapModel::MapModel()
{
  m_backgroundColor = te::color::RGBAColor(0, 0, 255, 255);
  m_box = te::gm::Envelope(0., 0., 150., 120.);
}

te::layout::MapModel::~MapModel()
{

}

void te::layout::MapModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  ContextItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
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
    double longitude = worldBox.getCenter().x;
    int meridiano = (int)(longitude / 6);
    meridiano = meridiano * 6;
    meridiano = abs(meridiano) + 3;

    double long0 = -meridiano * TeCDR;

    // TeUTM T4
    int zone = ((int)((long0*TeCRD+183.0)/6.0));

    std::string proj4 = proj4DescToPlanar(zone);
    
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

  // Checks if is Planar Geographic
  std::string authName = "EPSG"; // Now: So far it is the only one supported by TerraLib 5. Future: Review this line!
  te::srs::SpatialReferenceSystemManager::getInstance().isGeographic(srid, authName);
  unitPtr = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(srid, authName);

  return unitPtr;
}

te::gm::Envelope te::layout::MapModel::getWorldInDegrees()
{
  te::gm::Envelope box;
  
  if(!m_layer)
    return box;

  if(m_layer.get() == 0)
    return box;

  // World box: coordinates in the same SRS as the layer
  box = m_layer->getExtent();
  int srid = m_layer->getSRID();

  //About units names (SI): terralib5\resources\json\uom.json 
  te::common::UnitOfMeasurePtr unitPtr = unitMeasureLayer();

  if(!unitPtr)
    return box;

  std::string unitPtrStr = unitPtr->getName(); 
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if(unitPtrStr.compare("DEGREE") != 0)
  {
    /*TeCoord2D ll = _view->projection()->PC2LL(box.lowerLeft());
    TeCoord2D ur = _view->projection()->PC2LL(box.upperRight());

    ll.x_ = ll.x_ * TeCRD;
    ll.y_ = ll.y_ * TeCRD;

    ur.x_ = ur.x_ * TeCRD;
    ur.y_ = ur.y_ * TeCRD;

    box = TeBox(ll, ur);*/
  }

  return box;
}

std::string te::layout::MapModel::proj4DescToPlanar( int zone )
{
  /* 
  PROJ4
  +proj      Projection name
  +datum  Datum name
  +lat_0    Latitude of origin
  +lon_0   Central meridian 
  +x_0       False easting
  +y_0       False northing   
  +lat_1     Latitude of first standard parallel
  +lat_2     Latitude of second standard parallel
  +units     meters, US survey feet, etc.
  +lat_ts    Latitude of true scale
  +south   Denotes southern hemisphere UTM zone
  +no_defs Don't use the /usr/share/proj/proj_def.dat defaults file 
  */
  
  std::stringstream szone;
  szone << zone;

  std::string proj4 = "+proj=utm";
  proj4+= " +zone="+ szone.str();
  proj4+= " +south"; // pode ser +noth?
  proj4+= " +ellps=intl";
  proj4+= " +towgs84=-206,172,-6,0,0,0,0";
  proj4+= " +units=m"; 
  proj4+= " +no_defs ";
  
  return proj4;
}
