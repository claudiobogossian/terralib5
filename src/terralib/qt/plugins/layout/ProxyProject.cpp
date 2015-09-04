/*  Copyright (C) 2013-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ProxyProject.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ProxyProject.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"

te::qt::plugins::layout::ProxyProject::ProxyProject()
{
  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);
}

te::qt::plugins::layout::ProxyProject::~ProxyProject()
{

}

std::list<te::map::AbstractLayerPtr> te::qt::plugins::layout::ProxyProject::getAllLayers( bool invalid /*= true*/ )
{
  te::qt::af::evt::GetAvailableLayers e;

  emit triggered(&e);

  return e.m_layers;
}

const std::list<te::map::AbstractLayerPtr> te::qt::plugins::layout::ProxyProject::getSelectedLayers( bool invalid /*= true*/ ) 
{
  te::qt::af::evt::GetLayerSelected e;

  emit triggered(&e);

  std::list<te::map::AbstractLayerPtr> layers;
  layers.push_back(e.m_layer);

  return layers;
}

te::map::AbstractLayerPtr te::qt::plugins::layout::ProxyProject::contains( std::string name )
{
  te::map::AbstractLayerPtr layer;

  std::list<te::map::AbstractLayerPtr> layers = getAllLayers();

  std::list<te::map::AbstractLayerPtr>::iterator it;

  for(it = layers.begin() ; it != layers.end() ; ++it)
  {
    te::map::AbstractLayerPtr layerPtr = (*it);
    if(!layerPtr)
      continue;

    if(layerPtr->getTitle().compare(name) == 0)
    {
      layer = layerPtr;
      break;
    }
  }
  return layer;
}
