/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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

// TerraLib
#include "../../widgets/layer/explorer/AbstractTreeItem.h"
#include "../../widgets/layer/explorer/LayerExplorer.h"
#include "../../widgets/layer/explorer/LayerTreeModel.h"
#include "../../widgets/layer/explorer/LayerTreeView.h"
#include "../../widgets/layer/explorer/LegendItem.h"
#include "../events/Event.h"
#include "../events/ProjectEvents.h"
#include "../events/LayerEvents.h"
#include "../ApplicationController.h"
#include "../Project.h"
#include "LayerExplorer.h"

te::qt::af::LayerExplorer::LayerExplorer(te::qt::widgets::LayerExplorer* explorer, QObject* parent)
  : QObject(parent),
    m_explorer(explorer)
{
  assert(m_explorer);

  connect(m_explorer, SIGNAL(selectedLayersChanged(std::list<te::map::AbstractLayerPtr>)), SLOT(onSelectedLayersChanged(std::list<te::map::AbstractLayerPtr>)));
  connect(m_explorer, SIGNAL(visibilityChanged(te::map::AbstractLayerPtr)), SLOT(onLayerVisibilityChanged(te::map::AbstractLayerPtr)));
  connect(m_explorer, SIGNAL(layerOrderChanged()), SLOT(onLayerOrderChanged()));
  connect(m_explorer, SIGNAL(doubleClicked(te::qt::widgets::AbstractTreeItem*)), SLOT(onTreeItemDoubleClicked(te::qt::widgets::AbstractTreeItem*)));
}

te::qt::af::LayerExplorer::~LayerExplorer()
{
}

te::qt::widgets::LayerExplorer* te::qt::af::LayerExplorer::getExplorer() const
{
  return m_explorer;
}

void te::qt::af::LayerExplorer::onApplicationTriggered(te::qt::af::evt::Event* evt)
{
  switch(evt->m_id)
  {
    case te::qt::af::evt::PROJECT_ADDED:
    {
      assert(m_explorer);

      te::qt::af::evt::ProjectAdded* projectAdded = static_cast<te::qt::af::evt::ProjectAdded*>(evt);

      if(projectAdded == 0 || projectAdded->m_proj == 0)
        return;

      m_explorer->set(projectAdded->m_proj->getLayers());
    }
    break;

    case te::qt::af::evt::LAYER_ADDED:
    {
      te::qt::af::evt::LayerAdded* e = static_cast<te::qt::af::evt::LayerAdded*>(evt);

      m_explorer->add(e->m_layer);

      ApplicationController::getInstance().getProject()->add(e->m_layer);
    }
    break;

    case te::qt::af::evt::LAYER_ITEM_REMOVED:
    {
      te::qt::af::evt::LayerItemRemoved* e = static_cast<te::qt::af::evt::LayerItemRemoved*>(evt);

      te::qt::widgets::AbstractTreeItem* layerItem = e->m_layerItem;

      te::map::AbstractLayerPtr layer = layerItem->getLayer();

      // Remove the layer from the layer explorer
      m_explorer->remove(layerItem);

      // Remove the layer from the project
      ApplicationController::getInstance().getProject()->remove(layer);
    }
    break;

    case::te::qt::af::evt::LAYER_POPUP_ADD_ACTION:
    {
      te::qt::af::evt::LayerPopUpAddAction* e = static_cast<te::qt::af::evt::LayerPopUpAddAction*>(evt);
      
      QAction* action = e->m_action;

      m_explorer->add(action, "", "", m_explorer->getMenuType(e->m_menuType));
    }
    break;

    case::te::qt::af::evt::LAYER_POPUP_REMOVE_ACTION:
    {
      te::qt::af::evt::LayerPopUpRemoveAction* e = static_cast<te::qt::af::evt::LayerPopUpRemoveAction*>(evt);

      m_explorer->getTreeView()->remove(e->m_action);
    }
    break;

    case::te::qt::af::evt::GET_LAYER_SELECTED:
    {
      te::qt::af::evt::GetLayerSelected* e = static_cast<te::qt::af::evt::GetLayerSelected*>(evt);

      std::list<te::map::AbstractLayerPtr> list = m_explorer->getSelectedSingleLayers();

      if(list.empty() == false)
        e->m_layer = list.front();
    }
    break;

    default:
    break;
  }
}

void te::qt::af::LayerExplorer::onSelectedLayersChanged(const std::list<te::map::AbstractLayerPtr>& selectedLayers)
{
  if(selectedLayers.empty())
    return;

  std::list<te::map::AbstractLayerPtr>::const_iterator it;
  for(it = selectedLayers.begin(); it != selectedLayers.end(); ++it)
  {
    te::qt::af::evt::LayerSelected ev(*it);
    ApplicationController::getInstance().broadcast(&ev);
  }
}

void te::qt::af::LayerExplorer::onLayerVisibilityChanged(te::map::AbstractLayerPtr layer)
{
  te::qt::af::evt::LayerVisibilityChanged layerVisibilityChangedEvent(layer, layer->getVisibility());
  ApplicationController::getInstance().broadcast(&layerVisibilityChangedEvent);

  te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
  ApplicationController::getInstance().broadcast(&projectUnsavedEvent);
}

void te::qt::af::LayerExplorer::onLayerOrderChanged()
{
  ApplicationController::getInstance().getProject()->setTopLayers(m_explorer->getTopLayers());

  te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
  ApplicationController::getInstance().broadcast(&projectUnsavedEvent);
}

void te::qt::af::LayerExplorer::onTreeItemDoubleClicked(te::qt::widgets::AbstractTreeItem* item)
{
  te::qt::widgets::LegendItem* legendItem = dynamic_cast<te::qt::widgets::LegendItem*>(item);
  if(legendItem == 0)
    return;

  te::qt::widgets::AbstractTreeItem* layerItem = dynamic_cast<te::qt::widgets::AbstractTreeItem*>(item->parent());
  assert(layerItem);

  te::map::AbstractLayer* layer = layerItem->getLayer().get();
  assert(layer);

  te::qt::af::evt::LayerStyleSelected layerStyleSelected(layer);
  ApplicationController::getInstance().broadcast(&layerStyleSelected);
}
