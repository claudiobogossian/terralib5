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
#include "../core/ContextItem.h"
#include "../../maptools/Canvas.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/enum/AbstractType.h"
#include "GridPlanarModel.h"
#include "GridGeodesicModel.h"
#include "../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../core/property/PlanarGridSettingsConfigProperties.h"
#include "../core/enum/Enums.h"
#include "../core/property/SharedProperties.h"

// STL
#include <vector>
#include <string>

te::layout::MapGridModel::MapGridModel() :
  MapModel(),
  m_gridPlanar(0),
  m_gridGeodesic(0),
  m_geodesicGridProperties(0),
  m_planarGridProperties(0),
  m_mapBorder(true)
{
  m_type = Enums::getInstance().getEnumObjectType()->getMapGridItem();

  m_gridPlanar = new GridPlanarModel;
  m_gridGeodesic = new GridGeodesicModel;

  m_geodesicGridProperties = new GeodesicGridSettingsConfigProperties;
  m_planarGridProperties = new PlanarGridSettingsConfigProperties;
  
  m_mapDisplacementX = 30;
  m_mapDisplacementY = 30;

  m_box = te::gm::Envelope(0., 0., 180., 150.);
  setBox(m_box); // Also update m_mapBox

  m_border = false;

  m_backgroundColor = te::color::RGBAColor(0, 0, 255, 0);

  m_mapbackgroundColor = te::color::RGBAColor(50, 205, 50, 255);

  m_properties->setHasWindows(true);
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
  
  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();

  if((!canvas) || (!utils))
    return;
  
  utils->configCanvas(m_box);

  drawBackground(context);

  canvas->setFontFamily("Arial");
  canvas->setTextPointSize(8);

  drawGrid(canvas, utils);
    
  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);
    
  context.setPixmap(pixmap);
  notifyAll(context);
}

void te::layout::MapGridModel::drawGrid(te::map::Canvas* canvas, Utils* utils)
{
  if(!m_layer)
    return;

  int srid = m_layer->getSRID();

  if(srid <= 0)
    return;

  double scale = getScale();

  // Planar Grid
  te::gm::Envelope planarBox = getWorldInMeters();
  m_gridPlanar->setMapScale(scale);
  m_gridPlanar->setBoxMapMM(m_mapBoxMM);
  m_gridPlanar->draw(canvas, utils, planarBox, srid);

  // Geodesic Grid
  te::gm::Envelope geoBox = getWorldInDegrees();
  m_gridGeodesic->setMapScale(scale);
  m_gridGeodesic->setBoxMapMM(m_mapBoxMM);
  /* Box necessário para desenhar a curvatura */
  te::gm::Envelope planarBoxGeodesic = geoBox;
  int zone = utils->calculatePlanarZone(geoBox);
  utils->remapToPlanar(&planarBoxGeodesic, zone);
  m_gridGeodesic->setPlanarBox(planarBoxGeodesic);
  m_gridGeodesic->draw(canvas, utils, geoBox, srid);
}

te::layout::Properties* te::layout::MapGridModel::getProperties() const
{
  MapModel::getProperties();

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  m_properties->removeProperty(m_sharedProps->getBackgroundcolor());

  Property pro_mapbackgroundcolor;
  pro_mapbackgroundcolor.setName("map_color");
  pro_mapbackgroundcolor.setId("unknown");
  pro_mapbackgroundcolor.setValue(m_mapbackgroundColor, dataType->getDataTypeColor());
  pro_mapbackgroundcolor.setMenu(true);
  m_properties->addProperty(pro_mapbackgroundcolor);

  Property pro_mapBorder;
  pro_mapBorder.setName("map_border");
  pro_mapBorder.setValue(m_mapBorder, dataType->getDataTypeBool());  
  m_properties->addProperty(pro_mapBorder);

  Property pro_grid;
  pro_grid.setName("grid");
  pro_grid.setLabel("Grid Settings");
  pro_grid.setId("unknown");
  std::string sValuePlanar = "Settings";
  pro_grid.setValue(sValuePlanar, dataType->getDataTypeGridSettings());
  pro_grid.setMenu(true);

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

  Property pro_mapbackgroundcolor = vectorProps->contains("map_color");
  if(!pro_mapbackgroundcolor.isNull())
  {
    m_mapbackgroundColor = pro_mapbackgroundcolor.getValue().toColor();
  }

  Property pro_mapBorder = vectorProps->contains("map_border");
  if(!pro_mapBorder.isNull())
  {
    m_mapBorder = pro_mapBorder.getValue().toBool();
  }

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

void te::layout::MapGridModel::generateSystematic( te::gm::Coord2D coord )
{
  MapModel::generateSystematic(coord);
}

te::layout::GridPlanarModel* te::layout::MapGridModel::getGridPlanar()
{
  return m_gridPlanar;
}

te::layout::GridGeodesicModel* te::layout::MapGridModel::getGridGeodesic()
{
  return m_gridGeodesic;
}

void te::layout::MapGridModel::drawBackground( ContextItem context )
{
  MapModel::drawBackground(context);

  te::map::Canvas* canvas = context.getCanvas();
  Utils* utils = context.getUtils();

  if((!canvas) || (!utils))
    return;

  if(m_mapBorder)
  {
    canvas->setPolygonContourWidth(1);
    canvas->setPolygonContourColor(te::color::RGBAColor(0, 0, 0, 255));
    canvas->setPolygonFillColor(te::color::RGBAColor(255,255,255, 0));
    utils->drawRectW(m_mapBoxMM);
  }  
}
