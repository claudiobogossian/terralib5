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
  \file SVGViewModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SVGViewModel.h"
#include "../core/property/Property.h"
#include "../core/property/Properties.h"
#include "../core/enum/Enums.h"
#include "../core/property/GenericVariant.h"

// STL
#include <algorithm>

te::layout::SVGViewModel::SVGViewModel() 
{
  m_type = Enums::getInstance().getEnumObjectType()->getSVGView();
  m_box = te::gm::Envelope(0., 0., 200., 200.);
}

te::layout::SVGViewModel::~SVGViewModel()
{

}

te::layout::Properties* te::layout::SVGViewModel::getProperties() const
{
  m_properties->clear();

  Property pro_name(m_hashCode);
  pro_name.setName(m_name);

  m_properties->addProperty(pro_name);

  m_properties->setTypeObj(m_type);
  return m_properties;
}

void te::layout::SVGViewModel::updateProperties( te::layout::Properties* properties, bool notify )
{

}

void te::layout::SVGViewModel::setPropertiesMaps( std::vector<te::layout::Properties*> properties )
{
  m_mapProperties = properties;
  m_selectedLayers = searchLayers();
}

void te::layout::SVGViewModel::setLayers( std::list<te::map::AbstractLayerPtr> layers )
{
  m_layers = layers;
}

std::list<te::map::AbstractLayerPtr> te::layout::SVGViewModel::getLayers()
{
  return m_layers;
}

std::list<te::map::AbstractLayerPtr> te::layout::SVGViewModel::getSelectedLayers()
{
  return m_selectedLayers;
}

std::list<te::map::AbstractLayerPtr> te::layout::SVGViewModel::searchLayers()
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
	  Property pp = prop->contains("layers");

    if(pp.isNull())
    {
      continue;
    }

    m_layerProperties.push_back(pp);

	  GenericVariant v = pp.getValue().toGenericVariant();

	  std::list<te::map::AbstractLayerPtr> layerList = v.toLayerList();

    layerList.sort();
    layers.sort();

	  layers.merge(layerList);
  }

  return layers;
}

std::vector<te::layout::Property> te::layout::SVGViewModel::getLayerProperties()
{
  return m_layerProperties;
}

void te::layout::SVGViewModel::refresh()
{
  m_selectedLayers = searchLayers();
}






