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
  \file terralib/qt/plugins/edit/Plugin.cpp

  \brief Plugin implementation for the TerraLib Edit Qt Plugin.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../../edit/qt/Renderer.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/ApplicationEvents.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/events/MapEvents.h"
#include "../../widgets/canvas/MapDisplay.h"
#include "../../widgets/canvas/Canvas.h"
#include "../../widgets/layer/explorer/LayerItem.h"
#include "../../widgets/layer/explorer/LayerItemView.h"
#include "ApplicationListener.h"
#include "Plugin.h"
#include "ToolBar.h"
#include "Stasher.h"
#include "EditedDelegate.h"

// QT
#include <QAction>
#include <QObject>
#include <QMenu>
#include <QMenuBar>

std::list<te::map::AbstractLayer*> GetVisibleLayers(const std::list<te::map::AbstractLayerPtr>& ls)
{
  std::list<te::map::AbstractLayer*> res;

  for(std::list<te::map::AbstractLayerPtr>::const_iterator it = ls.begin(); it != ls.end(); ++it)
    if((*it)->getVisibility() == te::map::VISIBLE)
      res.push_back((*it).get());

  return res;
}

void UpdateTreeView(QTreeView* view)
{
  QModelIndex idx = *view->selectionModel()->selectedIndexes().begin();

  if(view != 0 && idx.isValid())
  {
    QVector<int> roles;
    roles << Qt::DecorationRole;

    view->dataChanged(idx, idx, roles);
  }
}

te::map::AbstractLayer* GetSelectedLayer(QTreeView* view)
{
  QModelIndexList idxs = view->selectionModel()->selectedIndexes();

  if(idxs.isEmpty() || idxs.size() > 1)
    return 0;

  te::qt::widgets::TreeItem* item = static_cast<te::qt::widgets::TreeItem*>((*idxs.begin()).internalPointer());

  if(item->getType() != "LAYER")
    return 0;

  return ((te::qt::widgets::LayerItem*)item)->getLayer().get();
}

te::qt::plugins::edit::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : QObject(),
  te::plugin::Plugin(pluginInfo),
    m_toolbar(0),
    m_menu(0),
    m_action(0)
{
}

te::qt::plugins::edit::Plugin::~Plugin() 
{
}

void te::qt::plugins::edit::Plugin::startup()
{
  if(m_initialized)
    return;

  // Create the main toolbar
  m_toolbar = new ToolBar(this);

  //m_toolbar->initialize();

  // Register the application framework listener
  te::qt::af::AppCtrlSingleton::getInstance().addListener(this);

  updateDelegate(true);

  connect(m_toolbar, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
  connect(m_toolbar, SIGNAL(stashed(te::map::AbstractLayer*)), SLOT(onStashedLayer(te::map::AbstractLayer*)));

  connect(m_toolbar, SIGNAL(geometriesEdited()), SLOT(onGeometriesChanged()));

  // Add plugin toolbar
//  te::qt::af::AppCtrlSingleton::getInstance().addToolBar("PluginEditionToolBar", m_toolbar->get());

  // Get plugins menu
  QMenu* pluginsMenu = te::qt::af::AppCtrlSingleton::getInstance().getMenu("Plugins");

  // Create the main menu
  m_menu = new QMenu(pluginsMenu);
  m_menu->setTitle(TE_TR("Edit"));
  m_menu->setIcon(QIcon::fromTheme("layer-edit"));

  // Insert menu before plugins last action
  QAction* lastAction = te::qt::af::AppCtrlSingleton::getInstance().findAction("ManagePluginsSeparator");
  pluginsMenu->insertMenu(lastAction, m_menu);

  m_action = new QAction(m_menu);
  //m_action->setObjectName("Layer.Edit");
  m_action->setText("Enable Edit Buttons");
  m_menu->addAction(m_action);

  connect(m_action, SIGNAL(triggered(bool)), this, SLOT(onActionActivated(bool)));

  TE_LOG_TRACE(TE_TR("TerraLib Edit Qt Plugin startup!"));

  m_initialized = true;

  te::qt::af::evt::NewActionsAvailable e;
  e.m_toolbar = m_toolbar->get();
  e.m_category = "Edition";

  emit triggered(&e);

  std::set<std::string> sts = GetStashedLayers();

  for(std::set<std::string>::iterator it = sts.begin(); it != sts.end(); ++it)
    m_delegate->addStashed(*it);
}

void te::qt::plugins::edit::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  updateDelegate(false);

  delete m_toolbar;

  delete m_menu;

  TE_LOG_TRACE(TE_TR("TerraLib Edit Qt Plugin shutdown!"));

  m_initialized = false;

  te::qt::af::AppCtrlSingleton::getInstance().removeListener(this);
}

void te::qt::plugins::edit::Plugin::onActionActivated(bool checked)
{
  if(checked != m_toolbar->get()->isEnabled())
    m_toolbar->onEditActivated(checked);
}

void te::qt::plugins::edit::Plugin::onApplicationTriggered(te::qt::af::evt::Event* e)
{
  switch(e->m_id)
  {
    case te::qt::af::evt::DRAWING_FINISHED:
    {
      drawStashed();
    }
      break;

    case te::qt::af::evt::LAYER_SELECTED:
    {
      te::qt::af::evt::LayerSelected* evt = static_cast<te::qt::af::evt::LayerSelected*>(e);

      te::map::AbstractLayer* l = GetSelectedLayer(getLayerExplorer());

      if(l == 0)
      {
        m_toolbar->updateLayer(0, false);
        return;
      }

      te::qt::af::evt::GetMapDisplay de;
      emit triggered(&de);

      //m_toolbar->get()->setEnabled(evt->m_layer->getSRID() != 0);
      m_toolbar->updateLayer(evt->m_layer.get(), m_delegate->isStached(evt->m_layer->getTitle()));
    }
      break;

    default:
      break;
  }
}

void te::qt::plugins::edit::Plugin::onStashedLayer(te::map::AbstractLayer* layer)
{
  m_delegate->addStashed(layer->getTitle());

  UpdateTreeView(getLayerExplorer());
}

void te::qt::plugins::edit::Plugin::onGeometriesChanged()
{
  m_delegate->addEdited(m_toolbar->getSelectedLayer()->getTitle());

  UpdateTreeView(getLayerExplorer());
}

void te::qt::plugins::edit::Plugin::drawStashed()
{
  te::qt::af::evt::GetMapDisplay e;
  emit triggered(&e);

  // Initialize the renderer
  te::qt::widgets::MapDisplay* display = e.m_display->getDisplay();

  if(display == 0)
    return;

  QPixmap* draft = display->getDraftPixmap();
  draft->fill(Qt::transparent);

  te::map::AbstractLayer* l = GetSelectedLayer(getLayerExplorer());

  if(l != 0)
  {
    te::gm::Envelope env = display->getExtent();

    te::edit::Renderer& renderer = te::edit::Renderer::getInstance();
    renderer.begin(draft, env, display->getSRID());

    // Draw the layer edited geometries
    renderer.drawRepository(l->getId(), env, display->getSRID());

    renderer.end();
  }

  display->repaint();
}

void te::qt::plugins::edit::Plugin::updateDelegate(const bool& add)
{
  te::qt::widgets::LayerItemView* view = getLayerExplorer();

  if(view == 0)
    return;

  if(add)
  {
    m_delegate = new EditDelegate((QStyledItemDelegate*)view->itemDelegate(), this);
    view->setItemDelegate(m_delegate);
  }
  else
  {
    view->removeDelegate(m_delegate);
    delete m_delegate;
    m_delegate = 0;
  }
}

te::qt::widgets::LayerItemView*te::qt::plugins::edit::Plugin::getLayerExplorer()
{
  te::qt::af::evt::GetLayerExplorer e;

  emit triggered(&e);

  return e.m_layerExplorer;
}


PLUGIN_CALL_BACK_IMPL(te::qt::plugins::edit::Plugin)
