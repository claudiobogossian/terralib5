/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*! 
  \file terralib/qt/af/connectors/InterfaceController.cpp

  \brief A connector to controll all non modal interfaces.
*/

// TerraLib
#include "../../widgets/InterfaceController.h"
#include "../events/Event.h"
#include "../events/LayerEvents.h"
#include "../ApplicationController.h"
#include "InterfaceController.h"

te::qt::af::InterfaceController::InterfaceController(QObject* parent)
  : QObject(parent)
{
}

te::qt::af::InterfaceController::~InterfaceController()
{
  m_interfaces.clear();
}

void te::qt::af::InterfaceController::addInterface(te::qt::widgets::InterfaceController* i)
{
  std::set<te::qt::widgets::InterfaceController*>::iterator it = m_interfaces.find(i);

  if(it == m_interfaces.end())
    m_interfaces.insert(i);
}

void te::qt::af::InterfaceController::removeInteface(te::qt::widgets::InterfaceController* i)
{
  std::set<te::qt::widgets::InterfaceController*>::iterator it = m_interfaces.find(i);

  if(it != m_interfaces.end())
    m_interfaces.erase(it);
}

void te::qt::af::InterfaceController::onApplicationTriggered(te::qt::af::evt::Event* evt)
{
  switch(evt->m_id)
  {
    case te::qt::af::evt::LAYER_SELECTED:
    {
      te::qt::af::evt::LayerSelected* ev = static_cast<te::qt::af::evt::LayerSelected*>(evt);

      layerSelected( ev->m_layer);
    }
    break;

    case te::qt::af::evt::LAYER_ADDED:
    {
      te::qt::af::evt::LayerAdded* ev = static_cast<te::qt::af::evt::LayerAdded*>(evt);

      layerAdded( ev->m_layer);
    }
    break;

    case te::qt::af::evt::LAYER_REMOVED:
    {
      te::qt::af::evt::LayerRemoved* ev = static_cast<te::qt::af::evt::LayerRemoved*>(evt);

      for(std::list<te::map::AbstractLayerPtr>::iterator it = ev->m_layers.begin(); it != ev->m_layers.end(); ++it)
        layerRemoved(*it);
    }
    break;

    default:
      return;
  }
}

void te::qt::af::InterfaceController::layerSelected(te::map::AbstractLayerPtr layer)
{
  assert(layer);

  std::set<te::qt::widgets::InterfaceController*>::iterator it = m_interfaces.begin();

  while(it != m_interfaces.end())
  {
    (*it)->layerSelected(layer);
    ++it;
  }
}

void te::qt::af::InterfaceController::layerAdded(te::map::AbstractLayerPtr layer)
{
  assert(layer);

  std::set<te::qt::widgets::InterfaceController*>::iterator it = m_interfaces.begin();

  while(it != m_interfaces.end())
  {
    (*it)->layerAdded(layer);
    ++it;
  }
}

void te::qt::af::InterfaceController::layerRemoved(te::map::AbstractLayerPtr layer)
{
  assert(layer);

  std::set<te::qt::widgets::InterfaceController*>::iterator it = m_interfaces.begin();

  while(it != m_interfaces.end())
  {
    (*it)->layerRemoved(layer);
    ++it;
  }
}
