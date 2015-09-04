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
  \file terralib/qt/plugins/terralib4/Plugin.cpp

  \brief Plugin implementation for the TERRALIB4 Qt Plugin widget.
*/

// TerraLib
#include "../../../../common/Config.h"
#include "../../../../common/Translator.h"
#include "../../../../common/Logger.h"
#include "../../../af/ApplicationController.h"
#include "../../../af/events/ApplicationEvents.h"
#include "../../../af/Utils.h"
#include "TL4ConverterAction.h"
#include "TL4ConverterWizard.h"
#include "Plugin.h"

// QT
#include <QAction>
#include <QMenu>
#include <QMenuBar>

te::qt::plugins::terralib4::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : QObject(), te::plugin::Plugin(pluginInfo),
  m_showWindow(0)
{
}

te::qt::plugins::terralib4::Plugin::~Plugin() 
{
}

void te::qt::plugins::terralib4::Plugin::startup()
{
  if(m_initialized)
    return;

  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);

  TE_LOG_TRACE(TE_TR("TerraLib Qt TERRALIB4 Plugin startup!"));

// add plugin menu
  //m_terralib4Menu = te::qt::af::AppCtrlSingleton::getInstance().getMenu("Terralib4");

  te::qt::af::evt::NewActionsAvailable evt;
  evt.m_category = "Terralib4";
  evt.m_plgName = "Terralib4Converter";


// register actions
  //registerActions();
  //QMenu* mnu = te::qt::af::AppCtrlSingleton::getInstance().findMenu("Tools");
  //QAction* act = te::qt::af::AppCtrlSingleton::getInstance().findAction("Tools.Customize");

  //if(act)
  //{
    m_showWindow = new QAction(QIcon::fromTheme("tools-terralib4converter"), "TerraLib 4 Converter...", this);
    m_showWindow->setObjectName("Tools.TerraLib 4 Converter");
    evt.m_actions << m_showWindow;
    //mnu->insertAction(act, m_showWindow);
    //mnu->addSeparator();

    connect(m_showWindow, SIGNAL(triggered()), SLOT(showWindow()));

    emit triggered(&evt);
  //}

  //te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_showWindow);

  m_initialized = true;
}

void te::qt::plugins::terralib4::Plugin::shutdown()
{
  if(!m_initialized)
    return;

  delete m_showWindow;

  TE_LOG_TRACE(TE_TR("TerraLib Qt TERRALIB4 Plugin shutdown!"));

  m_initialized = false;

  te::qt::af::AppCtrlSingleton::getInstance().removeListener(this);
}

void te::qt::plugins::terralib4::Plugin::showWindow()
{
  QWidget* parent = te::qt::af::AppCtrlSingleton::getInstance().getMainWindow();
  te::qt::plugins::terralib4::TL4ConverterWizard dlg(parent);

  connect(&dlg, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));

  if(dlg.exec() != QDialog::Accepted)
    return;
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::terralib4::Plugin)
