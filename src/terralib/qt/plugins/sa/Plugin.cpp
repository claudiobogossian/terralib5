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
  \file terralib/qt/plugins/sa/Plugin.cpp

  \brief Plugin implementation for the SA Qt Plugin widget.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/Utils.h"
#include "Plugin.h"

#ifdef TE_QT_PLUGIN_SA_HAVE_BAYESGLOBAL
  #include "BayesGlobalAction.h"
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_BAYESLOCAL
  #include "BayesLocalAction.h"
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_KERNELMAP
  #include "KernelMapAction.h"
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_KERNELRATIO
  #include "KernelRatioAction.h"
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_GEOSTATISTICALMETHODS
  #include "GeostatisticalMethodsAction.h"
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_PROXIMITYMATRIXCREATOR
  #include "ProximityMatrixCreatorAction.h"
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_SAMPLEPOINTSGENERATOR
  #include "SamplePointsGeneratorAction.h"
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_SKATER
  #include "SkaterAction.h"
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_SPATIALSTATISTICS
  #include "SpatialStatisticsAction.h"
#endif


// QT
#include <QMenu>
#include <QMenuBar>

te::qt::plugins::sa::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : QObject(),
  te::plugin::Plugin(pluginInfo), m_saMenu(0)
{
}

te::qt::plugins::sa::Plugin::~Plugin() 
{
}

void te::qt::plugins::sa::Plugin::startup()
{
  if(m_initialized)
    return;

  TE_LOG_TRACE(TE_TR("TerraLib Qt SA Plugin startup!"));

  // add sa entry in plugin menu
  QMenu* pluginMenu = te::qt::af::AppCtrlSingleton::getInstance().getMenu("Processing");
  m_saMenu = new QMenu(pluginMenu);
  m_saMenu->setIcon(QIcon::fromTheme("sa-spatialanalysis-icon"));

  // Insert action before plugin manager action
  QAction* pluginsSeparator = te::qt::af::AppCtrlSingleton::getInstance().findAction("ManagePluginsSeparator");

  pluginMenu->insertMenu(pluginsSeparator, m_saMenu);

  m_saMenu->setTitle(TE_TR("Spatial Analysis"));

// add pop up menu
  m_popupAction = new QAction(m_saMenu);
  m_popupAction->setText(TE_TR("Spatial Analysis"));

// register actions
  registerActions();

  m_initialized = true;

  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);
}

void te::qt::plugins::sa::Plugin::shutdown()
{
  if(!m_initialized)
    return;

// remove menu
  delete m_saMenu;

// unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_TR("TerraLib Qt SA Plugin shutdown!"));

  m_initialized = false;

  te::qt::af::AppCtrlSingleton::getInstance().removeListener(this);
}

void te::qt::plugins::sa::Plugin::registerActions()
{

#ifdef TE_QT_PLUGIN_SA_HAVE_PROXIMITYMATRIXCREATOR
  m_proxMatrixCreator = new te::qt::plugins::sa::ProximityMatrixCreatorAction(m_saMenu);
  te::qt::af::AddActionToCustomToolbars(m_proxMatrixCreator->getAction());
  connect(m_proxMatrixCreator, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_SPATIALSTATISTICS
  m_saMenu->addSeparator();
  m_spatialStatistics = new te::qt::plugins::sa::SpatialStatisticsAction(m_saMenu);
  te::qt::af::AddActionToCustomToolbars(m_spatialStatistics->getAction());
  connect(m_spatialStatistics, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_BAYESGLOBAL
  m_saMenu->addSeparator();
  m_bayesGlobal = new te::qt::plugins::sa::BayesGlobalAction(m_saMenu);
  te::qt::af::AddActionToCustomToolbars(m_bayesGlobal->getAction());
  connect(m_bayesGlobal, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_BAYESLOCAL
    m_bayesLocal = new te::qt::plugins::sa::BayesLocalAction(m_saMenu);
    te::qt::af::AddActionToCustomToolbars(m_bayesLocal->getAction());
    connect(m_bayesLocal, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_KERNELMAP
  m_saMenu->addSeparator();
  m_kernelMap = new te::qt::plugins::sa::KernelMapAction(m_saMenu);
  te::qt::af::AddActionToCustomToolbars(m_kernelMap->getAction());
  connect(m_kernelMap, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_KERNELRATIO
  m_kernelRatio = new te::qt::plugins::sa::KernelRatioAction(m_saMenu);
  te::qt::af::AddActionToCustomToolbars(m_kernelRatio->getAction());
  connect(m_kernelRatio, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_SKATER
  m_saMenu->addSeparator();
  m_skater = new te::qt::plugins::sa::SkaterAction(m_saMenu);
  te::qt::af::AddActionToCustomToolbars(m_skater->getAction());
  connect(m_skater, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_GEOSTATISTICALMETHODS
  m_saMenu->addSeparator();
  m_geostatistics = new te::qt::plugins::sa::GeostatisticalMethodsAction(m_saMenu);
  te::qt::af::AddActionToCustomToolbars(m_geostatistics->getAction());
  connect(m_geostatistics, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_SAMPLEPOINTSGENERATOR
  m_saMenu->addSeparator();
  m_samplePointsGenerator = new te::qt::plugins::sa::SamplePointsGeneratorAction(m_saMenu);
  te::qt::af::AddActionToCustomToolbars(m_samplePointsGenerator->getAction());
  connect(m_samplePointsGenerator, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

}

void  te::qt::plugins::sa::Plugin::unRegisterActions()
{

#ifdef TE_QT_PLUGIN_SA_HAVE_BAYESGLOBAL
    delete m_bayesGlobal;
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_BAYESLOCAL
    delete m_bayesLocal;
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_KERNELMAP
    delete m_kernelMap;
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_KERNELRATIO
    delete m_kernelRatio;
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_GEOSTATISTICALMETHODS
    delete m_geostatistics;
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_PROXIMITYMATRIXCREATOR
    delete m_proxMatrixCreator;
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_SAMPLEPOINTSGENERATOR
  delete m_samplePointsGenerator;
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_SKATER
  delete m_skater;
#endif

#ifdef TE_QT_PLUGIN_SA_HAVE_SPATIALSTATISTICS
    delete m_spatialStatistics;
#endif

}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::sa::Plugin)
