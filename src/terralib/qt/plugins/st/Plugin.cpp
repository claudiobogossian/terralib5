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
  \file terralib/qt/plugins/st/Plugin.cpp

  \brief Plugin implementation for the st Qt Plugin widget.
*/
#include "Plugin.h"

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/ApplicationEvents.h"
#include "../../af/events/LayerEvents.h"
#include "../../widgets/layer/explorer/LayerItem.h"
#include "../../widgets/layer/explorer/LayerItemView.h"

#include "STItemDelegate.h"

#ifdef TE_QT_PLUGIN_ST_HAVE_SLIDER
  #include "TimeSliderWidgetAction.h"
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_OBSERVATION
    #include "ObservationAction.h"
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TIMESERIES
    #include "TimeSeriesAction.h"
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TRAJECTORY
    #include "TrajectoryAction.h"
#endif

// QT
#include <QMenu>
#include <QMenuBar>



void GetAllSTLayers(te::qt::widgets::LayerItemView* view, const QModelIndex& parent, QModelIndexList& layersIdx)
{
  QAbstractItemModel* model = view->model();
  int cS = model->rowCount(parent);

  for(int i = 0; i < cS; i++)
  {
    QModelIndex idx = model->index(i, 0, parent);

    te::qt::widgets::TreeItem* child =
        static_cast<te::qt::widgets::TreeItem*>(idx.internalPointer());

    if(child->getType() == "FOLDER")
      GetAllSTLayers(view, idx, layersIdx);
    else if(child->getType() == "LAYER")
    {
      te::map::AbstractLayerPtr l = ((te::qt::widgets::LayerItem*)child)->getLayer();

      bool isSTLayer = (l->getType() == "TRAJECTORYDATASETLAYER" ||
                        l->getType() == "OBSERVATIONDATASETLAYER" ||
                        l->getType() == "TIMESERIESDATASETLAYER");

      if(isSTLayer)
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



te::qt::plugins::st::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : QObject(), te::plugin::Plugin(pluginInfo), m_stMenu(0), m_delegate(0)
{
}

te::qt::plugins::st::Plugin::~Plugin() 
{
}

void te::qt::plugins::st::Plugin::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib st Qt Plugin
  //TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_ST_TEXT_DOMAIN, TE_QT_PLUGIN_ST_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_TR("TerraLib Qt ST Plugin startup!"));

// add plugin menu
  m_stMenu = new QMenu("Project.Add Layer.Add Temporal Layer");

  m_stMenu->setTitle(tr("Add Temporal Layer"));

// register actions
  registerActions();

  m_initialized = true;

  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);

  updateDelegate(true);

  te::qt::af::evt::NewActionsAvailable e;

  e.m_category = "Dataaccess";
  e.m_plgName = "ST";
  e.m_actions <<m_stMenu->menuAction();

  emit triggered(&e);
}

void te::qt::plugins::st::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// unregister actions
  unRegisterActions();

// remove menu
  delete m_stMenu;

  TE_LOG_TRACE(TE_TR("TerraLib Qt ST Plugin shutdown!"));

  m_initialized = false;

  updateDelegate(false);

  QModelIndexList stls;

  te::qt::af::evt::GetLayerExplorer e;

  emit triggered(&e);

  if(e.m_layerExplorer == 0)
    return;

  GetAllSTLayers(e.m_layerExplorer, QModelIndex(), stls);

  if(!stls.isEmpty())
  {
    e.m_layerExplorer->removeItems(stls);
    te::qt::af::evt::LayerRemoved evt(GetLayers(stls));

    emit triggered(&evt);
  }

  te::qt::af::AppCtrlSingleton::getInstance().removeListener(this);
}

void te::qt::plugins::st::Plugin::registerActions()
{
#ifdef TE_QT_PLUGIN_ST_HAVE_SLIDER
    m_sliderAction = new te::qt::plugins::st::TimeSliderWidgetAction(te::qt::af::AppCtrlSingleton::getInstance().findMenu("View"));
    connect(m_sliderAction, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_OBSERVATION
    m_observactionAction = new te::qt::plugins::st::ObservationAction(m_stMenu);
    connect(m_observactionAction, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TIMESERIES
    m_timeSeriesAction = new te::qt::plugins::st::TimeSeriesAction(m_stMenu);
    connect(m_timeSeriesAction, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TRAJECTORY
    m_trajectoryAction = new te::qt::plugins::st::TrajectoryAction(m_stMenu);
    connect(m_trajectoryAction, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif
}

void  te::qt::plugins::st::Plugin::unRegisterActions()
{
#ifdef TE_QT_PLUGIN_ST_HAVE_SLIDER
    delete m_sliderAction;
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_OBSERVATION
    delete m_observactionAction;
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TIMESERIES
    delete m_timeSeriesAction;
#endif

#ifdef TE_QT_PLUGIN_ST_HAVE_TRAJECTORY
    delete m_trajectoryAction;
#endif
}

void te::qt::plugins::st::Plugin::updateDelegate(const bool& add)
{
  te::qt::af::evt::GetLayerExplorer e;

  emit triggered(&e);

  if(e.m_layerExplorer == 0)
    return;

  if(add)
  {
    m_delegate = new STItemDelegate((QStyledItemDelegate*)e.m_layerExplorer->itemDelegate(), this);
    e.m_layerExplorer->setItemDelegate(m_delegate);
  }
  else
  {
    e.m_layerExplorer->removeDelegate(m_delegate);
    delete m_delegate;
    m_delegate = 0;
  }
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::st::Plugin)
