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
  \file MapLayerChoiceOutsideControllerModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "MapLayerChoiceModel.h"
#include "../core/property/Properties.h"
#include "../core/enum/Enums.h"
#include "../core/property/GenericVariant.h"

// STL
#include <algorithm>

te::layout::MapLayerChoiceModel::MapLayerChoiceModel() :
	AbstractOutsideModel()
{
  m_type = Enums::getInstance().getEnumObjectType()->getMapLayerChoice();
  m_box = te::gm::Envelope(0., 0., 200., 200.);
}

te::layout::MapLayerChoiceModel::~MapLayerChoiceModel()
{

}

te::layout::Properties* te::layout::MapLayerChoiceModel::getProperties() const
{
  m_properties->clear();

  Property pro_name(m_hashCode);
  pro_name.setName(m_name);

  m_properties->addProperty(pro_name);

  m_properties->setTypeObj(m_type);
  return m_properties;
}

void te::layout::MapLayerChoiceModel::updateProperties( te::layout::Properties* properties, bool notify )
{

}

void te::layout::MapLayerChoiceModel::setPropertiesMaps( std::vector<te::layout::Properties*> properties )
{
  m_mapProperties = properties;
  m_selectedLayers = searchLayers();
}

void te::layout::MapLayerChoiceModel::setLayers( std::list<te::map::AbstractLayerPtr> layers )
{
  m_layers = layers;
}

std::list<te::map::AbstractLayerPtr> te::layout::MapLayerChoiceModel::getLayers()
{
  return m_layers;
}

std::list<te::map::AbstractLayerPtr> te::layout::MapLayerChoiceModel::getSelectedLayers()
{
  return m_selectedLayers;
}

std::list<te::map::AbstractLayerPtr> te::layout::MapLayerChoiceModel::searchLayers()
{
  std::list<te::map::AbstractLayerPtr> layers;

  if(m_mapProperties.empty())
  {
    return layers; 
  }
  
  std::vector<te::layout::Properties*>::const_iterator itProp;
  itProp = m_mapProperties.begin();

  for( ; itProp != m_mapProperties.end() ; ++itProp)
  {
    Properties* prop = (*itProp);
    Property pp = prop->getProperty("layers");

    if(pp.isNull())
    {
      continue;
    }

    m_layerProperties.push_back(pp);

    GenericVariant v = pp.getValue().toGenericVariant();

    std::list<te::map::AbstractLayerPtr> currentLayers = v.toLayerList();
    std::list<te::map::AbstractLayerPtr>::iterator itLayers = currentLayers.begin();
    while(itLayers != currentLayers.end())
    {
      layers.push_back(*itLayers);
      ++itLayers;
    }
  }

  return layers;
}

std::vector<te::layout::Property> te::layout::MapLayerChoiceModel::getLayerProperties()
{
  return m_layerProperties;
}

void te::layout::MapLayerChoiceModel::refresh()
{
  m_selectedLayers = searchLayers();
}






