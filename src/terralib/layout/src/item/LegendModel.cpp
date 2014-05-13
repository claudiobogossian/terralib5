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
  \file LegendModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LegendModel.h"
#include "Context.h"
#include "../../../maptools/Canvas.h"
#include "MapModel.h"
#include "Property.h"
#include "Properties.h"
#include "SharedProperties.h"

te::layout::LegendModel::LegendModel() :
  m_mapName("")
{
  m_backgroundColor = te::color::RGBAColor(0, 183, 255, 255);
  m_box = te::gm::Envelope(0., 0., 70., 50.);
}

te::layout::LegendModel::~LegendModel()
{

}

void te::layout::LegendModel::draw( ContextItem context )
{
  te::color::RGBAColor** pixmap = 0;

  te::map::Canvas* canvas = Context::getInstance()->getCanvas();
  Utils* utils = Context::getInstance()->getUtils();

  if(context.isResizeCanvas())
    utils->configCanvas(m_box);

  canvas->setPolygonContourColor(te::color::RGBAColor(0, 0, 0, 255));
  canvas->setPolygonFillColor(m_backgroundColor);

  utils->drawRectW(m_box);

  if(context.isResizeCanvas())
    pixmap = utils->getImageW(m_box);

  ContextItem contextNotify;
  contextNotify.setPixmap(pixmap);
  notifyAll(contextNotify);
}

te::layout::Properties* te::layout::LegendModel::getProperties() const
{
  ItemModelObservable::getProperties();

  Property pro_mapName;
  pro_mapName.setName(m_sharedProps->getMapName());
  pro_mapName.setId("");
  pro_mapName.setValue(m_mapName, DataTypeStringList);
  Variant v;
  v.setValue(m_mapName, DataTypeString);
  pro_mapName.addOption(v);
  
  m_properties->addProperty(pro_mapName);

  return m_properties;
}

void te::layout::LegendModel::updateProperties( te::layout::Properties* properties )
{
  ItemModelObservable::updateProperties(properties);

  Properties* vectorProps = const_cast<Properties*>(properties);

  Property pro_mapName = vectorProps->contains(m_sharedProps->getMapName());

  if(!pro_mapName.isNull())
  {
    m_mapName = pro_mapName.getOptionByCurrentChoice().toString();
  }
}

void te::layout::LegendModel::visitDependent()
{
  MapModel* map = dynamic_cast<MapModel*>(m_visitable);

  /*te::map::AbstractLayerPtr layer = map->getLayer();

  layer->getGrouping();*/

  if(map)
  {
    m_mapName = map->getName();

    ContextItem contextNotify;
    contextNotify.setWait(true);
    notifyAll(contextNotify);
  }	
}
