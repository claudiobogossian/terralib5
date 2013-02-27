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
#include "../../maptools/MarkRendererManager.h"
#include "property/DateTimePropertyWidgetFactory.h"
#include "property/GeometryPropertyWidgetFactory.h"
#include "property/NumericPropertyWidgetFactory.h"
#include "property/StringPropertyWidgetFactory.h"
#include "se/BasicFillWidgetFactory.h"
#include "se/GlyphGraphicWidgetFactory.h"
#include "se/GlyphMarkRenderer.h"
#include "se/GraphicFillWidgetFactory.h"
#include "se/LocalGraphicWidgetFactory.h"
#include "se/WellKnownGraphicWidgetFactory.h"
#include "se/WellKnownMarkRenderer.h"
//#include "datasource/core/DataSourceTypeManager.h"
//#include "datasource/connector/gdal/GDALType.h"
//#include "datasource/connector/geofile/GeoFileType.h"
//#include "datasource/connector/ogr/OGRType.h"
//#include "datasource/connector/postgis/PostGISType.h"
//#include "datasource/connector/shp/ShapeFileType.h"
//#include "datasource/connector/wcs/WCSType.h"
//#include "datasource/connector/wfs/WFSType.h"
//#include "datasource/connector/wms/WMSType.h"
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
// Initializes the data source types
  //DataSourceTypeManager::getInstance().add(new GDALType);
  //DataSourceTypeManager::getInstance().add(new GeoFileType);
  //DataSourceTypeManager::getInstance().add(new OGRType);
  //DataSourceTypeManager::getInstance().add(new PostGISType);
  //DataSourceTypeManager::getInstance().add(new ShapeFileType);
  //DataSourceTypeManager::getInstance().add(new WCSType);
  //DataSourceTypeManager::getInstance().add(new WFSType);
  //DataSourceTypeManager::getInstance().add(new WMSType);

// Mark renderers
  te::map::MarkRendererManager::getInstance().add("", new WellKnownMarkRenderer);
  te::map::MarkRendererManager::getInstance().add("ttf", new GlyphMarkRenderer);

// It initializes the graphic widgets factories
  te::qt::widgets::WellKnownGraphicWidgetFactory::initialize();
  te::qt::widgets::GlyphGraphicWidgetFactory::initialize();
  te::qt::widgets::LocalGraphicWidgetFactory::initialize();

// It initializes the fill widgets factories
  te::qt::widgets::BasicFillWidgetFactory::initialize();
  te::qt::widgets::GraphicFillWidgetFactory::initialize();

// It initializes the property widgets factories
  te::qt::widgets::DateTimePropertyWidgetFactory::initialize();
  te::qt::widgets::GeometryPropertyWidgetFactory::initialize();
  te::qt::widgets::NumericPropertyWidgetFactory::initialize();
  te::qt::widgets::StringPropertyWidgetFactory::initialize();

  TE_LOG_TRACE(TR_QT_WIDGETS("TerraLib Qt Widgets initialized!"));
}

void te::qt::widgets::Module::finalize()
{
// Removes the data source types
  //DataSourceTypeManager::getInstance().remove("GDAL");
  //DataSourceTypeManager::getInstance().remove("GEOFILE");
  //DataSourceTypeManager::getInstance().remove("OGR");
  //DataSourceTypeManager::getInstance().remove("POSTGIS");
  //DataSourceTypeManager::getInstance().remove("SHAPEFILE");
  //DataSourceTypeManager::getInstance().remove("WCS");
  //DataSourceTypeManager::getInstance().remove("WFS");
  //DataSourceTypeManager::getInstance().remove("WMS");

// It finalizes the graphic widgets factories
  te::qt::widgets::WellKnownGraphicWidgetFactory::finalize();
  te::qt::widgets::GlyphGraphicWidgetFactory::finalize();
  te::qt::widgets::LocalGraphicWidgetFactory::finalize();

// It finalizes the fill widgets factories
  te::qt::widgets::BasicFillWidgetFactory::finalize();
  te::qt::widgets::GraphicFillWidgetFactory::finalize();

  TE_LOG_TRACE(TR_QT_WIDGETS("TerraLib Qt Widgets finalized!"));
}

//// TerraLib
//#include "../../common/ModuleUtils.h"
//
//TE_REGISTER_MODULE(TE_QTWIDGETS_MODULE_NAME, TeQtWidgetsInitialize, TeQtWidgetsFinalize)
