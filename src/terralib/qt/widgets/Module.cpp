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
  \file Module.cpp
   
  \brief This singleton defines the TerraLib Qt Widgets module entry.
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/TerraLib.h"
#include "../../common/Translator.h"
#include "../../maptools/ChartRendererManager.h"
#include "../../maptools/ExternalGraphicRendererManager.h"
#include "../../maptools/MarkRendererManager.h"
#include "charts/ChartRenderer.h"
//#include "layer/explorer/AbstractTreeItemFactory.h"
#include "property/DateTimePropertyWidgetFactory.h"
#include "property/GeometryPropertyWidgetFactory.h"
#include "property/NumericPropertyWidgetFactory.h"
#include "property/StringPropertyWidgetFactory.h"
#include "se/BasicFillWidgetFactory.h"
#include "se/ExternalGraphicRenderer.h"
#include "se/GlyphGraphicWidgetFactory.h"
#include "se/GlyphMarkRenderer.h"
#include "se/GraphicFillWidgetFactory.h"
#include "se/LocalGraphicWidgetFactory.h"
#include "se/WellKnownGraphicWidgetFactory.h"
#include "se/WellKnownMarkRenderer.h"
#include "Config.h"
#include "Module.h"

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
}

te::qt::widgets::Module::~Module()
{
  TerraLib::getInstance().remove(TE_QTWIDGETS_MODULE_NAME);
}

void te::qt::widgets::Module::initialize()
{
#ifdef TERRALIB_MOD_MAPTOOLS_ENABLED
// Mark renderers
  te::map::MarkRendererManager::getInstance().add("", new WellKnownMarkRenderer);
  te::map::MarkRendererManager::getInstance().add("ttf", new GlyphMarkRenderer);

// ExternalGraphic renderer
  te::map::ExternalGraphicRendererManager::getInstance().add(new ExternalGraphicRenderer);

// Chart Renderer
  te::map::ChartRendererManager::getInstance().set(new ChartRenderer);

// It initializes the graphic widgets factories
  te::qt::widgets::WellKnownGraphicWidgetFactory::initialize();
  te::qt::widgets::GlyphGraphicWidgetFactory::initialize();
  te::qt::widgets::LocalGraphicWidgetFactory::initialize();

// It initializes the fill widgets factories
  te::qt::widgets::BasicFillWidgetFactory::initialize();
  te::qt::widgets::GraphicFillWidgetFactory::initialize();
  
// It initializes the layer item factory
//  AbstractTreeItemFactory::initialize();
#endif

#ifdef TERRALIB_MOD_DATAACCESS_ENABLED
// It initializes the property widgets factories
  te::qt::widgets::DateTimePropertyWidgetFactory::initialize();
  te::qt::widgets::GeometryPropertyWidgetFactory::initialize();
  te::qt::widgets::NumericPropertyWidgetFactory::initialize();
  te::qt::widgets::StringPropertyWidgetFactory::initialize();
#endif

  TE_LOG_TRACE(TE_TR("TerraLib Qt Widgets initialized!"));
}

void te::qt::widgets::Module::finalize()
{

#ifdef TERRALIB_MOD_DATAACCESS_ENABLED
  // It initializes the property widgets factories
  te::qt::widgets::DateTimePropertyWidgetFactory::finalize();
  te::qt::widgets::GeometryPropertyWidgetFactory::finalize();
  te::qt::widgets::NumericPropertyWidgetFactory::finalize();
  te::qt::widgets::StringPropertyWidgetFactory::finalize();
#endif

#ifdef TERRALIB_MOD_MAPTOOLS_ENABLED
// It finalizes the layer item factory
//  AbstractTreeItemFactory::finalize();

  te::map::MarkRendererManager::getInstance().clear();

// It finalizes the graphic widgets factories
  te::qt::widgets::WellKnownGraphicWidgetFactory::finalize();
  te::qt::widgets::GlyphGraphicWidgetFactory::finalize();
  te::qt::widgets::LocalGraphicWidgetFactory::finalize();

// It finalizes the fill widgets factories
  te::qt::widgets::BasicFillWidgetFactory::finalize();
  te::qt::widgets::GraphicFillWidgetFactory::finalize();
#endif

  TE_LOG_TRACE(TE_TR("TerraLib Qt Widgets finalized!"));
}

