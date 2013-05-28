/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
#include "../../widgets/layer/explorer/LayerExplorer.h"
#include "../../widgets/layer/explorer/LayerTreeModel.h"
#include "../../widgets/layer/explorer/LayerTreeView.h"
#include "../events/Event.h"
#include "../events/ProjectEvents.h"
#include "../events/LayerEvents.h"
#include "../ApplicationController.h"
#include "../LayerDecorator.h"
#include "../Project.h"
#include "LayerExplorer.h"

te::qt::af::LayerExplorer::LayerExplorer(te::qt::widgets::LayerExplorer* explorer, QObject* parent)
  : QObject(parent),
    m_explorer(explorer)
{
  assert(explorer);
  
  connect (explorer->getTreeView(), SIGNAL(toggled(te::qt::widgets::AbstractLayerTreeItem*, bool)), SLOT(layerVisibility(te::qt::widgets::AbstractLayerTreeItem*, bool)));
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

      te::qt::af::evt::ProjectAdded* nevt = static_cast<te::qt::af::evt::ProjectAdded*>(evt);

      if(nevt == 0 || nevt->m_proj == 0)
        return;

      std::list<te::map::AbstractLayerPtr> layers;
      std::list<te::map::AbstractLayerPtr>::iterator it;

      for(it = nevt->m_proj->getLayers().begin(); it != nevt->m_proj->getLayers().end(); ++it)
      {
        te::map::AbstractLayerPtr layer(((LayerDecorator*)it->get())->getDecorated());
        layers.push_back(layer);
      }

      m_explorer->set(layers);

      assert(m_explorer->getTreeView());
      assert(m_explorer->getTreeView()->selectionModel());

      QItemSelectionModel* layerTreeSelectionModel = m_explorer->getTreeView()->selectionModel();
      connect(layerTreeSelectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), SLOT(onSelectionChanged(const QItemSelection&, const QItemSelection&)));
    }
    break;

    case te::qt::af::evt::LAYER_ADDED:
    {
      te::qt::af::evt::LayerAdded* e = static_cast<te::qt::af::evt::LayerAdded*>(evt);

      m_explorer->add(e->m_layer);
    }
    break;

    default:
    break;
  }
}

void te::qt::af::LayerExplorer::onSelectionChanged(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
//        QModelIndexList lst = selected.indexes();
//
//        if(lst.isEmpty())
//          return;
//
//        te::qt::widgets::AbstractTreeItem* item = static_cast<te::qt::widgets::AbstractTreeItem*>((*lst.begin()).internalPointer());
//
//        if(item != 0 && item->isLayerItem())
//        {
//          te::map::AbstractLayerPtr abs_lay = item->getRefLayer();
//          te::qt::af::LayerSelected ls_ev(abs_lay.get());
//          ApplicationController::getInstance().broadcast(&ls_ev);
//        }
}

void te::qt::af::LayerExplorer::layerVisibility(te::qt::widgets::AbstractLayerTreeItem* item, bool checked)
{
  te::qt::af::evt::ProjectUnsaved evt;
  ApplicationController::getInstance().broadcast(&evt);
}

