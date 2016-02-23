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
  \file terralib/qt/plugins/attributefill/AbstractAction.cpp

  \brief This file defines the abstract class AbstractAction
*/

// Terralib
#include "../../af/events/LayerEvents.h"
#include "../../af/ApplicationController.h"
#include "AbstractAction.h"

// STL
#include <cassert>

te::qt::plugins::attributefill::AbstractAction::AbstractAction(QMenu* menu): m_menu(menu), m_action(0)
{
}

te::qt::plugins::attributefill::AbstractAction::~AbstractAction()
{
}

void te::qt::plugins::attributefill::AbstractAction::createAction(std::string name, std::string pixmap)
{
  assert(m_menu);

  m_action = new QAction(m_menu);

  m_action->setText(name.c_str());

  if(pixmap.empty() == false)
    m_action->setIcon(QIcon::fromTheme(pixmap.c_str()));

  connect(m_action, SIGNAL(triggered(bool)), this, SLOT(onActionActivated(bool)));

  m_menu->addAction(m_action);
}

void te::qt::plugins::attributefill::AbstractAction::addNewLayer(te::map::AbstractLayerPtr layer)
{
  te::qt::af::evt::LayerAdded evt(layer.get());

  emit triggered(&evt);
}

std::list<te::map::AbstractLayerPtr> te::qt::plugins::attributefill::AbstractAction::getLayers()
{
  te::qt::af::evt::GetAvailableLayers e;
  te::qt::af::evt::GetLayerSelected ed;

  emit triggered(&e);
  emit triggered(&ed);

  std::list<te::map::AbstractLayerPtr> allLayers = e.m_layers;

  std::list<te::map::AbstractLayerPtr> layers;

  for (std::list<te::map::AbstractLayerPtr>::iterator it = allLayers.begin(); it != allLayers.end(); ++it)
  {
    if ((*it)->isValid())
      layers.push_back(*it);
  }

  te::map::AbstractLayerPtr selectedlayer = ed.m_layer;

  if (!selectedlayer || !selectedlayer->isValid())
    return layers;

  std::list<te::map::AbstractLayerPtr> result;

  result.push_back(selectedlayer);

  for (std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin(); it != layers.end(); ++it)
  {
    if ((*it)->getId() != selectedlayer->getId())
      result.push_back(*it);
  }

  return result;
}
