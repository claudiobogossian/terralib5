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

// TerraLib
#include "../../../se/ColorMap.h"
#include "../../../se/RasterSymbolizer.h"
#include "../../../se/Utils.h"
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

      m_explorer->set(projectAdded->m_proj->getTopLayers());
    }
    break;

    case te::qt::af::evt::LAYER_ADDED:
    {
      te::qt::af::evt::LayerAdded* e = static_cast<te::qt::af::evt::LayerAdded*>(evt);

      // Update the project
      te::map::AbstractLayerPtr parentLayer = e->m_parentLayer;

      // Fred: revisar
//      ApplicationController::getInstance().getProject()->add(e->m_layer, parentLayer);

      // Add the layer in the layer explorer
      te::qt::widgets::AbstractTreeItem* parentItem = m_explorer->getLayerItem(parentLayer);

      m_explorer->add(e->m_layer, parentItem);

      if(parentItem)
        m_explorer->expand(parentItem);

      te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
      emit triggered(&projectUnsavedEvent);
    }
    break;

    case te::qt::af::evt::LAYER_REMOVED:
    {
      te::qt::af::evt::LayerRemoved* e = static_cast<te::qt::af::evt::LayerRemoved*>(evt);

      te::map::AbstractLayerPtr layer = e->m_layer;

      // Remove the item from the layer explorer
      te::qt::widgets::AbstractTreeItem* layerItem = m_explorer->getLayerItem(layer);
      m_explorer->remove(layerItem);

      // Fred: revisar
//      ApplicationController::getInstance().getProject()->remove(layer);

      te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
      emit triggered(&projectUnsavedEvent);
    }
    break;

    case te::qt::af::evt::ITEM_OF_LAYER_REMOVED:
    {
      te::qt::af::evt::ItemOfLayerRemoved* e = static_cast<te::qt::af::evt::ItemOfLayerRemoved*>(evt);

      te::qt::widgets::AbstractTreeItem* item = e->m_item;

      te::map::AbstractLayerPtr layer = item->getLayer();

      te::qt::widgets::AbstractTreeItem* parentItem = static_cast<te::qt::widgets::AbstractTreeItem*>(item->parent());

      m_explorer->collapse(parentItem);

      te::map::AbstractLayerPtr parentLayer = parentItem->getLayer();

      if(item->getItemType() == "CHART_ITEM")
      {
        // If the item is a chart item, remove the chart from the layer associated to the parent of this chart item.
        parentLayer->setChart(0);
      }
      else if(item->getItemType() == "GROUPING_ITEM")
      {
        // If the item is a chart item, remove the chart from the layer associated to the parent of this chart item.
        parentLayer->setGrouping(0);
      }
      else if(item->getItemType() == "COLORMAP_ITEM")
      {
        // If the item is a color map item, remove the all style from the layer associated to the parent of this color map item.
        //parentLayer->setStyle(0);

        te::se::RasterSymbolizer* rs = te::se::GetRasterSymbolizer(parentLayer->getStyle());

        rs->setColorMap(0);
      }

      // Remove the item from the layer explorer
      m_explorer->remove(item);

      te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
      emit triggered(&projectUnsavedEvent);
    }
    break;

    case te::qt::af::evt::LAYER_POPUP_ADD_ACTION:
    {
      te::qt::af::evt::LayerPopUpAddAction* e = static_cast<te::qt::af::evt::LayerPopUpAddAction*>(evt);
      
      QAction* action = e->m_action;

      m_explorer->add(action, "", e->m_itemType.c_str(), m_explorer->getMenuSelectionType(e->m_menuSelectionType));
    }
    break;

    case te::qt::af::evt::LAYER_POPUP_REMOVE_ACTION:
    {
      te::qt::af::evt::LayerPopUpRemoveAction* e = static_cast<te::qt::af::evt::LayerPopUpRemoveAction*>(evt);

      m_explorer->getTreeView()->remove(e->m_action);
    }
    break;

    case te::qt::af::evt::GET_LAYER_SELECTED:
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

  // Fred: revisar
//  ApplicationController::getInstance().getProject()->setSelectedLayers(selectedLayers);

  std::list<te::map::AbstractLayerPtr>::const_iterator it;
  for(it = selectedLayers.begin(); it != selectedLayers.end(); ++it)
  {
    te::qt::af::evt::LayerSelected ev(*it);
    emit triggered(&ev);
  }
}

void te::qt::af::LayerExplorer::onLayerVisibilityChanged(te::map::AbstractLayerPtr layer)
{
  te::qt::af::evt::LayerVisibilityChanged layerVisibilityChangedEvent(layer, layer->getVisibility());
  emit triggered(&layerVisibilityChangedEvent);

  te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
  emit triggered(&projectUnsavedEvent);
}

void te::qt::af::LayerExplorer::onLayerOrderChanged()
{
  // Fred: revisar
//  ApplicationController::getInstance().getProject()->setTopLayers(m_explorer->getTopLayers());

  te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
  emit triggered(&projectUnsavedEvent);
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
  emit triggered(&layerStyleSelected);
}
