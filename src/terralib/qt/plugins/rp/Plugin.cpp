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
  \file terralib/qt/plugins/rp/Plugin.cpp

  \brief Plugin implementation for the RP Qt Plugin widget.
*/

// TerraLib
#include "../../../common/Config.h"
#include "../../../common/Translator.h"
#include "../../../common/Logger.h"
#include "../../af/ApplicationController.h"
#include "../../af/events/LayerEvents.h"
#include "../../af/Utils.h"
#include "Plugin.h"

#ifdef TE_QT_PLUGIN_RP_HAVE_ARITHMETICOP
  #include "ArithmeticOpAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_CLASSIFIER
  #include "ClassifierAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_CLIPPING
  #include "ClippingAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_COLORTRANSFORM
  #include "ColorTransformAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_COMPOSEBANDS
  #include "ComposeBandsAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_CONTRAST
  #include "ContrastAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_FILTER
  #include "FilterAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_FUSION
  #include "FusionAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_MIXTUREMODEL
  #include "MixtureModelAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_MOSAIC
  #include "MosaicAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_SEGMENTER
  #include "SegmenterAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_REGISTER
  #include "RegisterAction.h"
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_VECTORIZATION
  #include "VectorizationAction.h"
#endif

// QT
#include <QAction>
#include <QMenu>
#include <QMenuBar>

te::qt::plugins::rp::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : QObject(),
  te::plugin::Plugin(pluginInfo), m_rpMenu(0)
{
}

te::qt::plugins::rp::Plugin::~Plugin() 
{
}

void te::qt::plugins::rp::Plugin::startup()
{
  if(m_initialized)
    return;

  te::qt::af::AppCtrlSingleton::getInstance().addListener(this, te::qt::af::SENDER);

// it initializes the Translator support for the TerraLib RP Qt Plugin
  //TE_ADD_TEXT_DOMAIN(TE_QT_PLUGIN_RP_TEXT_DOMAIN, TE_QT_PLUGIN_RP_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TE_TR("TerraLib Qt RP Plugin startup!"));

// add plugin menu
  QMenu* pluginMenu = te::qt::af::AppCtrlSingleton::getInstance().getMenu("Processing");
  m_rpMenu = new QMenu(pluginMenu);
  m_rpMenu->setIcon(QIcon::fromTheme("rp-rasterprocessing-icon"));

  // Insert action before plugin manager action
  QAction* pluginsSeparator = te::qt::af::AppCtrlSingleton::getInstance().findAction("ManagePluginsSeparator");

  pluginMenu->insertMenu(pluginsSeparator, m_rpMenu);

  m_rpMenu->setTitle(TE_TR("Raster Processing"));

// add pop up menu
  m_popupAction = new QAction(m_rpMenu);
  m_popupAction->setText(TE_TR("Raster Processing"));

  //add to application layer tree pop up menu
  //te::qt::af::evt::LayerPopUpAddAction evt(m_popupAction, "RASTER_LAYER_ITEM", 2 /*UNIQUE_ITEM_SELECTED*/);
  //te::qt::af::AppCtrlSingleton::getInstance().broadcast(&evt);

  m_rpPopupMenu = new QMenu(m_rpMenu);
  m_popupAction->setMenu(m_rpPopupMenu);

// register actions
  registerActions();

  m_initialized = true;
}

void te::qt::plugins::rp::Plugin::shutdown()
{
  if(!m_initialized)
    return;

//remove from application layer tree pop up menu
  //te::qt::af::evt::LayerPopUpRemoveAction evt(m_popupAction);
  //te::qt::af::AppCtrlSingleton::getInstance().broadcast(&evt);

// remove menu
  delete m_rpMenu;

// unregister actions
  unRegisterActions();

  TE_LOG_TRACE(TE_TR("TerraLib Qt RP Plugin shutdown!"));

  m_initialized = false;
}

void te::qt::plugins::rp::Plugin::registerActions()
{
#ifdef TE_QT_PLUGIN_RP_HAVE_ARITHMETICOP
  m_arithmeticOp = new te::qt::plugins::rp::ArithmeticOpAction(m_rpMenu, m_rpPopupMenu);
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_arithmeticOp->getAction());
  connect(m_arithmeticOp, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_CLASSIFIER
    m_classifier = new te::qt::plugins::rp::ClassifierAction(m_rpMenu, m_rpPopupMenu);
    te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_classifier->getAction());
    connect(m_classifier, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_CLIPPING
    m_clipping = new te::qt::plugins::rp::ClippingAction(m_rpMenu, m_rpPopupMenu);
    te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_clipping->getAction());
    connect(m_clipping, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_COLORTRANSFORM
    m_colorTrans = new te::qt::plugins::rp::ColorTransformAction(m_rpMenu, m_rpPopupMenu);
    te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_colorTrans->getAction());
    connect(m_colorTrans, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_COMPOSEBANDS
   m_composeBands = new te::qt::plugins::rp::ComposeBandsAction(m_rpMenu, m_rpPopupMenu);
   te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_composeBands->getAction());
   connect(m_composeBands, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_CONTRAST
    m_contrast = new te::qt::plugins::rp::ContrastAction(m_rpMenu, m_rpPopupMenu);
    te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_contrast->getAction());
    connect(m_contrast, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_FILTER
    m_filter = new te::qt::plugins::rp::FilterAction(m_rpMenu, m_rpPopupMenu);
    te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_filter->getAction());
    connect(m_filter, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_FUSION
    m_fusion = new te::qt::plugins::rp::FusionAction(m_rpMenu, m_rpPopupMenu);
    te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_fusion->getAction());
    connect(m_fusion, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_MIXTUREMODEL
  m_mixtureModel = new te::qt::plugins::rp::MixtureModelAction(m_rpMenu, m_rpPopupMenu);
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_mixtureModel->getAction());
  connect(m_mixtureModel, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

  #ifdef TE_QT_PLUGIN_RP_HAVE_MOSAIC
  m_mosaic = new te::qt::plugins::rp::MosaicAction(m_rpMenu, m_rpPopupMenu);
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_mosaic->getAction());
  connect(m_mosaic, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_REGISTER
  m_register = new te::qt::plugins::rp::RegisterAction(m_rpMenu, m_rpPopupMenu);
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_register->getAction());
  connect(m_register, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_SEGMENTER
    m_segmenter = new te::qt::plugins::rp::SegmenterAction(m_rpMenu, m_rpPopupMenu);
    te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_segmenter->getAction());
    connect(m_segmenter, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_VECTORIZATION
  m_vector = new te::qt::plugins::rp::VectorizationAction(m_rpMenu, m_rpPopupMenu);
  te::qt::af::AddActionToCustomToolbars(&te::qt::af::AppCtrlSingleton::getInstance(), m_vector->getAction());
  connect(m_vector, SIGNAL(triggered(te::qt::af::evt::Event*)), SIGNAL(triggered(te::qt::af::evt::Event*)));
#endif
}

void  te::qt::plugins::rp::Plugin::unRegisterActions()
{
#ifdef TE_QT_PLUGIN_RP_HAVE_ARITHMETICOP
    delete m_arithmeticOp;
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_CLASSIFIER
    delete m_classifier;
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_CLIPPING
    delete m_clipping;
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_COLORTRANSFORM
    delete m_colorTrans;
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_COMPOSEBANDS
    delete m_composeBands;
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_CONTRAST
    delete m_contrast;
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_FILTER
    delete m_filter;
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_FUSION
    delete m_fusion;
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_MIXTUREMODEL
    delete m_mixtureModel;
#endif

    #ifdef TE_QT_PLUGIN_RP_HAVE_MOSAIC
    delete m_mosaic;
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_REGISTER
  delete m_register;
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_SEGMENTER
    delete m_segmenter;
#endif

#ifdef TE_QT_PLUGIN_RP_HAVE_VECTORIZATION
  delete m_vector;
#endif
}

PLUGIN_CALL_BACK_IMPL(te::qt::plugins::rp::Plugin)
