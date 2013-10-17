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
  assert(m_explorer->getTreeView());
  assert(m_explorer->getTreeModel());
  
  connect(explorer->getTreeView(), SIGNAL(layerRemoved(te::map::AbstractLayerPtr)), SLOT(onLayerRemoved(te::map::AbstractLayerPtr)));
  connect(explorer->getTreeView(), SIGNAL(visibilityChanged(te::map::AbstractLayerPtr)), SLOT(onLayerVisibilityChanged(te::map::AbstractLayerPtr)));
  connect(explorer->getTreeModel(), SIGNAL(visibilityChanged(te::map::AbstractLayerPtr)), SLOT(onLayerVisibilityChanged(te::map::AbstractLayerPtr)));
  connect(explorer->getTreeModel(), SIGNAL(layerOrderChanged()), SLOT(onLayerOrderChanged()));
  //  connect(explorer->getTreeView(), SIGNAL(layersChanged(const std::vector<te::map::AbstractLayerPtr>&)), SLOT(layersChanged(const std::vector<te::map::AbstractLayerPtr>&)));
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

      te::qt::widgets::LayerTreeView* treeView = m_explorer->getTreeView();
      assert(treeView);

      QItemSelectionModel* layerTreeSelectionModel = treeView->selectionModel();
      assert(layerTreeSelectionModel);

      // Signals & slots
      connect(treeView, SIGNAL(doubleClicked(te::qt::widgets::AbstractTreeItem*)), SLOT(onAbstractTreeItemDoubleClicked(te::qt::widgets::AbstractTreeItem*)));
      //connect(layerTreeSelectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), SLOT(onSelectionChanged(const QItemSelection&, const QItemSelection&)));
      connect(treeView, SIGNAL(selectedLayersChanged(std::list<te::map::AbstractLayerPtr>)), SLOT(onLayerSelectionChanged(std::list<te::map::AbstractLayerPtr>)));
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

      // Remove the layer from the project
      ApplicationController::getInstance().getProject()->remove(layerItem->getLayer());

      // Remove the layer from the layer explorer
      m_explorer->remove(layerItem);
    }
    break;

    default:
    break;
  }
}

void te::qt::af::LayerExplorer::onLayerSelectionChanged(const std::list<te::map::AbstractLayerPtr>& selectedLayers)
{
  emit selectedLayersChanged(m_explorer->getSelectedLayers());

  if(selectedLayers.empty())
    return;

  std::list<te::map::AbstractLayerPtr>::const_iterator it;
  for(it = selectedLayers.begin(); it != selectedLayers.end(); ++it)
  {
    te::qt::af::evt::LayerSelected ev(*it);
    ApplicationController::getInstance().broadcast(&ev);
  }
}

//void te::qt::af::LayerExplorer::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
//{
//  emit selectedLayersChanged(m_explorer->getSelectedLayers());
//
//  QModelIndexList lst = selected.indexes();
//
//  if(lst.isEmpty())
//    return;
//
//  te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>((*lst.begin()).internalPointer());
//
//  if(item != 0)
//  {
//    te::map::AbstractLayerPtr itemLayer = item->getLayer();
//    if(itemLayer)
//    {
//      te::qt::af::evt::LayerSelected ls_ev(itemLayer);
//      ApplicationController::getInstance().broadcast(&ls_ev);
//    }
//  }
//}

void te::qt::af::LayerExplorer::onLayerVisibilityChanged(te::map::AbstractLayerPtr layer)
{
  te::qt::af::evt::LayerVisibilityChanged layerVisibilityChangedEvent(layer, layer->getVisibility());
  ApplicationController::getInstance().broadcast(&layerVisibilityChangedEvent);

  te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
  ApplicationController::getInstance().broadcast(&projectUnsavedEvent);
}

void te::qt::af::LayerExplorer::onLayerOrderChanged()
{
  ApplicationController::getInstance().getProject()->setLayers(m_explorer->getAllTopLevelLayers());

  te::qt::af::evt::ProjectUnsaved projectUnsavedEvent;
  ApplicationController::getInstance().broadcast(&projectUnsavedEvent);
}

void te::qt::af::LayerExplorer::onLayerRemoved(te::map::AbstractLayerPtr layer)
{
  te::qt::af::evt::LayerRemoved layerRemovedEvent(layer);
  ApplicationController::getInstance().broadcast(&layerRemovedEvent);
}

void te::qt::af::LayerExplorer::layersChanged(const std::vector<te::map::AbstractLayerPtr>& layers)
{
  te::qt::af::evt::LayersChanged evt(layers);
  ApplicationController::getInstance().broadcast(&evt);
}

void te::qt::af::LayerExplorer::onAbstractTreeItemDoubleClicked(te::qt::widgets::AbstractTreeItem* item)
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
