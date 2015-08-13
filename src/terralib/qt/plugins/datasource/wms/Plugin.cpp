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
  \file terralib/qt/plugins/datasource/wms/Plugin.cpp

  \brief Plugin implementation for the OGC Web Map Service (WMS) data source widget.
*/

// TerraLib
#include "../../../../common/Config.h"
#include "../../../../common/Translator.h"
#include "../../../../common/Logger.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../wms/qt/WMSLayerItem.h"
#include "../../../widgets/datasource/core/DataSourceTypeManager.h"
#include "../../../widgets/layer/explorer/AbstractTreeItemFactory.h"
#include "../../../widgets/layer/explorer/LayerExplorer.h"
#include "../../../widgets/layer/explorer/LayerTreeModel.h"
#include "../../../af/ApplicationController.h"
#include "../../../af/BaseApplication.h"
#include "../../../af/connectors/LayerExplorer.h"
#include "WMSType.h"
#include "Plugin.h"

// Qt
#include <QTreeView>

// Boost
#include <boost/functional/factory.hpp>
#include <boost/bind.hpp>

void GetWMSLayers(QAbstractItemModel* model, const QModelIndex& parent, std::list<te::qt::widgets::AbstractTreeItem*>& layers)
{
  int cc = model->rowCount(parent);

  for(int i = 0; i < cc; i++)
  {
    QModelIndex cIdx = model->index(i, 0, parent);

    te::qt::widgets::AbstractTreeItem* cItem = static_cast<te::qt::widgets::AbstractTreeItem*>(cIdx.internalPointer());

    if(cItem->getItemType() == "WMS_LAYER_ITEM")
      layers.push_back(cItem);
    else if(cItem->getItemType() == "FOLDER_LAYER_ITEM")
      GetWMSLayers(model, cIdx, layers);
  }
}


te::qt::plugins::wms::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::qt::plugins::wms::Plugin::~Plugin() 
{
}

void te::qt::plugins::wms::Plugin::startup()
{
  if(m_initialized)
    return;

  te::qt::widgets::AbstractTreeItemFactory::reg("WMSLAYER", boost::bind(boost::factory<te::wms::WMSLayerItem*>(), _1, _2));

  te::qt::widgets::DataSourceTypeManager::getInstance().add(new WMSType);

  TE_LOG_TRACE(TE_TR("TerraLib Qt OGC Web Map Service (WMS) widget startup!"));

  m_initialized = true;
}

void te::qt::plugins::wms::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  te::da::DataSourceInfoManager::getInstance().removeByType("WMS");
  te::qt::widgets::DataSourceTypeManager::getInstance().remove("WMS");

  TE_LOG_TRACE(TE_TR("TerraLib Qt OGC Web Map Service (WMS) widget shutdown!"));

  m_initialized = false;

  te::qt::af::LayerExplorer* exp = ((te::qt::af::BaseApplication*)te::qt::af::ApplicationController::getInstance().getMainWindow())->getLayerExplorer();

  std::list<te::qt::widgets::AbstractTreeItem*> wms;

  GetWMSLayers(exp->getExplorer()->getTreeModel(), QModelIndex(), wms);

  exp->removeLayers(wms);
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::wms::Plugin)
