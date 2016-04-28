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
  \file terralib/qt/plugins/datasource/wfs/Plugin.cpp

  \brief Plugin implementation for the OGC Web Feature Service (WFS) data source widget.
*/

// TerraLib
#include "../../../../common/Config.h"
#include "../../../../common/Translator.h"
#include "../../../../common/Logger.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../af/ApplicationController.h"
#include "../../../af/events/ApplicationEvents.h"
#include "../../../af/events/LayerEvents.h"
#include "../../../widgets/datasource/core/DataSourceTypeManager.h"
#include "../../../widgets/layer/explorer/LayerItem.h"
#include "../../../widgets/layer/explorer/LayerItemView.h"
#include "WFSItemDelegate.h"
#include "WFSType.h"
#include "Plugin.h"
#include "../../../../wfs/Utils.h"

void GetAllWFSLayers(te::qt::widgets::LayerItemView* view, const QModelIndex& parent,
                     QModelIndexList& layersIdx)
{
  QAbstractItemModel* model = view->model();
  int cS = model->rowCount(parent);

  for(int i = 0; i < cS; i++)
  {
    QModelIndex idx = model->index(i, 0, parent);

    te::qt::widgets::TreeItem* child =
      static_cast<te::qt::widgets::TreeItem*>(idx.internalPointer());

    if(child->getType() == "FOLDER")
      GetAllWFSLayers(view, idx, layersIdx);
    else if(child->getType() == "LAYER")
    {
      if(te::wfs::IsWFSLayer(*((te::qt::widgets::LayerItem*)child)->getLayer()))
        layersIdx.push_back(idx);
    }
  }
}

std::list<te::map::AbstractLayerPtr> GetLayers(const QModelIndexList& lst)
{
  std::list<te::map::AbstractLayerPtr> res;

  for(QModelIndexList::const_iterator it = lst.begin(); it != lst.end(); ++it)
  {
    QModelIndex idx = *it;
    te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>(idx.internalPointer());
    res.push_back(((te::qt::widgets::LayerItem*)item)->getLayer());
  }

  return res;
}


te::qt::plugins::wfs::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : QObject(),
  te::plugin::Plugin(pluginInfo),
  m_delegate(0)
{
}

te::qt::plugins::wfs::Plugin::~Plugin() 
{
}

void te::qt::plugins::wfs::Plugin::startup()
{
  if(m_initialized)
    return;

  te::qt::widgets::DataSourceTypeManager::getInstance().add(new WFSType);

  TE_LOG_TRACE(TE_TR("TerraLib Qt OGC Web Feature Service (WFS) widget startup!"));

  m_initialized = true;

  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);

  updateDelegate(true);
}

void te::qt::plugins::wfs::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  TE_LOG_TRACE(TE_TR("TerraLib Qt OGC Web Feature Service (WFS) widget shutdown!"));

  m_initialized = false;

  updateDelegate(false);

  QModelIndexList wls;

  te::qt::af::evt::GetLayerExplorer e;

  emit triggered(&e);

  if(e.m_layerExplorer == 0)
    return;

  GetAllWFSLayers(e.m_layerExplorer, QModelIndex(), wls);

  if(!wls.isEmpty())
  {
    std::list<te::map::AbstractLayerPtr> lst = GetLayers(wls);

    e.m_layerExplorer->removeItems(wls);
  }

  te::qt::af::AppCtrlSingleton::getInstance().removeListener(this);

  te::da::DataSourceInfoManager::getInstance().removeByType("WFS");
  te::qt::widgets::DataSourceTypeManager::getInstance().remove("WFS");
}

void te::qt::plugins::wfs::Plugin::updateDelegate(const bool& add)
{
  te::qt::af::evt::GetLayerExplorer e;

  emit triggered(&e);

  te::qt::widgets::LayerItemView* view = e.m_layerExplorer;

  if(view == 0)
    return;

  if(add)
  {
    m_delegate = new WFSItemDelegate((QStyledItemDelegate*)view->itemDelegate(), this);
    view->setItemDelegate(m_delegate);
  }
  else
  {
    view->removeDelegate(m_delegate);
    delete m_delegate;
    m_delegate = 0;
  }
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::wfs::Plugin)
