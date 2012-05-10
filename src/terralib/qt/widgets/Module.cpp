/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Module.cpp
   
  \brief This singleton defines the TerraLib Qt Widgets module entry.
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/TerraLib.h"
#include "../../common/Translator.h"
#include "layer/FolderLayerItemFactory.h"
#include "layer/LayerItemFactory.h"
#include "se/MarkFactory.h"
#include "Config.h"
#include "Module.h"

///*!
//  \brief This is a helper function that will be automatically called when the TerraLib Qt Widgets module is loaded.
//
//  \note This function doesn't perform operations for this module.
//*/
//static void TeQtWidgetsInitialize()
//{
//// It initializes the widgets factories
//  te::qt::widgets::FolderLayerItemFactory::initialize();
//  te::qt::widgets::LayerItemFactory::initialize();
//
//  TE_LOG_TRACE(TR_QT_WIDGETS("TerraLib Qt Widgets initialized!"));
//}
//
///*!
//  \brief This is a helper function that will be automatically called when the TerraLib Qt Widgets module is unloaded.
//
//  \note This function doesn't perform operations for this module.
//*/
//static void TeQtWidgetsFinalize()
//{
//// It finalizes the widgets factories
//  te::qt::widgets::FolderLayerItemFactory::finalize();
//  te::qt::widgets::LayerItemFactory::finalize();
//
//  TE_LOG_TRACE(TR_QT_WIDGETS("TerraLib Qt Widgets finalized!"));
//}

const te::qt::widgets::Module& sm_module = te::qt::widgets::Module::getInstance();

const char* te::qt::widgets::Module::sm_pngFmt = "PNG";
const char* te::qt::widgets::Module::sm_jpegFmt = "JPEG";
const char* te::qt::widgets::Module::sm_gifFmt = "GIF";
const char* te::qt::widgets::Module::sm_bmpFmt = "BMP";
const char* te::qt::widgets::Module::sm_xpmFmt = "XPM";
const char* te::qt::widgets::Module::sm_xbmFmt = "XBM";
const char* te::qt::widgets::Module::sm_unknownFmt = "XBM";

te::qt::widgets::Module::Module()
{
  TerraLib::Module m = { TE_QTWIDGETS_MODULE_NAME,
                         te::qt::widgets::Module::initialize,
                         te::qt::widgets::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib Qt Widgets support
  TE_ADD_TEXT_DOMAIN(TE_QT_WIDGETS_TEXT_DOMAIN, TE_QT_WIDGETS_TEXT_DOMAIN_DIR, "UTF-8");
}

te::qt::widgets::Module::~Module()
{
  TerraLib::getInstance().remove(TE_QTWIDGETS_MODULE_NAME);
}

void te::qt::widgets::Module::initialize()
{
// It initializes the widgets factories
  te::qt::widgets::FolderLayerItemFactory::initialize();
  te::qt::widgets::LayerItemFactory::initialize();

// It initializes the mark factory
  te::qt::MarkFactory::initialize();

  TE_LOG_TRACE(TR_QT_WIDGETS("TerraLib Qt Widgets initialized!"));
}

void te::qt::widgets::Module::finalize()
{
// It finalizes the widgets factories
  te::qt::widgets::FolderLayerItemFactory::finalize();
  te::qt::widgets::LayerItemFactory::finalize();

// It finalizes the mark factory
  te::qt::MarkFactory::finalize();

  TE_LOG_TRACE(TR_QT_WIDGETS("TerraLib Qt Widgets finalized!"));
}

//// TerraLib
//#include "../../common/ModuleUtils.h"
//
//TE_REGISTER_MODULE(TE_QTWIDGETS_MODULE_NAME, TeQtWidgetsInitialize, TeQtWidgetsFinalize)
