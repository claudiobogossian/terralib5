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
#include "../events/NewProject.h"
#include "../Project.h"
#include "LayerExplorer.h"

te::qt::af::LayerExplorer::LayerExplorer(te::qt::widgets::LayerExplorer* explorer, QObject* parent)
  : QObject(parent),
    m_explorer(explorer)
{
  assert(explorer);
  
}

te::qt::af::LayerExplorer::~LayerExplorer()
{
}

void te::qt::af::LayerExplorer::onApplicationTriggered(te::qt::af::Event* evt)
{
  switch(evt->getId())
  {
    case te::qt::af::evt::NEW_PROJECT:
    {
      assert(m_explorer);

      te::qt::af::NewProject* nevt = dynamic_cast<te::qt::af::NewProject*>(evt);

      if(nevt == 0 || nevt->m_project == 0)
        return;

      m_explorer->setLayers(nevt->m_project->getLayers());

      assert(m_explorer->getTreeView());
      assert(m_explorer->getTreeView()->selectionModel());

      QItemSelectionModel* layerTreeSelectionModel = m_explorer->getTreeView()->selectionModel();
      connect(layerTreeSelectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), SLOT(onSelectionChanged(const QItemSelection&, const QItemSelection&)));
    }

//          case te::qt::af::evt::LAYER_ADDED:
//            {
//              LayerAdded* e = static_cast<LayerAdded*>(evt);
//              te::qt::widgets::LayerExplorerModel* model = static_cast<te::qt::widgets::LayerExplorerModel*>(m_explorer->model());
//
//              int idx = model->rowCount(QModelIndex());
//
//              model->insertItem(QModelIndex(), idx, e->m_layer);
//              model->resetModel();
//            }
//          break;
//
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

