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
  \file MapGridModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapGridModel.h"
#include "ContextItem.h"
#include "Context.h"
#include "../../../maptools/Canvas.h"
#include "Property.h"
#include "Properties.h"
#include "AbstractType.h"
#include "GridPlanarModel.h"
#include "GridGeodesicModel.h"

// STL
#include <vector>
#include <string>
#include "GeodesicGridSettingsConfigProperties.h"
#include "PlanarGridSettingsConfigProperties.h"

te::layout::MapGridModel::MapGridModel() :
  MapModel(),
  m_gridPlanar(0),
  m_gridGeodesic(0),
  m_planarGridProperties(0),
  m_geodesicGridProperties(0)
{
  m_backgroundColor = te::color::RGBAColor(0, 255, 255, 200);

  m_gridPlanar = new GridPlanarModel;
  m_gridGeodesic = new GridGeodesicModel;

  m_geodesicGridProperties = new GeodesicGridSettingsConfigProperties;
  m_planarGridProperties = new PlanarGridSettingsConfigProperties;

  m_properties->setHasGridWindows(true);
}

te::layout::MapGridModel::~MapGridModel()
{
  if(m_gridPlanar)
  {
    delete m_gridPlanar;
    m_gridPlanar = 0;
  }

  if(m_gridGeodesic)
  {
    delete m_gridGeodesic;
    m_gridGeodesic = 0;
  }

  if(m_planarGridProperties)
  {
    delete m_planarGridProperties;
    m_planarGridProperties = 0;
  }

  if(m_geodesicGridProperties)
  {
    delete m_geodesicGridProperties;
    m_geodesicGridProperties = 0;
  }
}

void te::layout::MapGridModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;
  
  te::map::Canvas* canvas = Context::getInstance()->getCanvas();
  Utils* utils = Context::getInstance()->getUtils();

  utils->configCanvas(m_box);

  drawGrid(canvas, utils);

  utils->configCanvas(m_box, false);

  te::color::RGBAColor colorp6(0,0,0, TE_OPAQUE);
  canvas->setLineColor(colorp6);

  canvas->setPolygonContourColor(m_borderColor);
  canvas->setPolygonFillColor(m_backgroundColor);

  //utils->drawRectW(m_box);
  
  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
    
  ContextItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

void te::layout::MapGridModel::drawGrid(te::map::Canvas* canvas, Utils* utils)
{
  if(!m_layer)
    return;

  int srid = m_layer->getSRID();
  double scale = getScale();

  // Planar Grid
  te::gm::Envelope planarBox = getWorldInMeters();
  utils->configGeoCanvas(planarBox, m_box, false);

  m_gridPlanar->setMapScale(scale);
  m_gridPlanar->draw(canvas, planarBox, srid);

  // Geodesic Grid
  te::gm::Envelope geoBox = getWorldInDegrees();
  utils->configGeoCanvas(geoBox, m_box, false);

  m_gridGeodesic->setMapScale(scale);
  m_gridGeodesic->draw(canvas, geoBox, srid);
}

te::layout::Properties* te::layout::MapGridModel::getProperties() const
{
  MapModel::getProperties();

  Property pro_grid;
  pro_grid.setName("grid");
  pro_grid.setId("unknown");
  std::string sValuePlanar = "Settings";
  pro_grid.setValue(sValuePlanar, DataTypeGridSettings);

  if(m_gridPlanar)
  {
    Property propPlanar = m_gridPlanar->getProperty();
    pro_grid.addSubProperty(propPlanar);
  }

  if(m_gridGeodesic)
  {
    Property propGeodesic = m_gridGeodesic->getProperty();
    pro_grid.addSubProperty(propGeodesic);
  }

  m_properties->addProperty(pro_grid);

  return m_properties;
}

void te::layout::MapGridModel::updateProperties( te::layout::Properties* properties )
{
  MapModel::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_grid = vectorProps->contains("grid");

  Property pro_grid_planar = pro_grid.containsSubProperty(m_planarGridProperties->getName());

  if(!pro_grid_planar.isNull())
  {
    m_gridPlanar->updateProperty(pro_grid_planar);
  }

  Property pro_grid_geodesic = pro_grid.containsSubProperty(m_geodesicGridProperties->getName());

  if(!pro_grid_geodesic.isNull())
  {
    m_gridGeodesic->updateProperty(pro_grid_geodesic);
  }
}
