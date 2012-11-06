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
  \file Platform.h
   
  \brief A utility class to initialize and terminate TerraLib OGR driver support.  
 */

// TerraLib
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Platform.h"

// OGR
#include <ogrsf_frmts.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

bool  te::ogr::Platform::sm_ogrInitialized = false;

te::ogr::Platform::Platform(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::CppPlugin(pluginInfo)
{
}

te::ogr::Platform::~Platform()
{
}

void te::ogr::Platform::startup()
{
  te::ogr::Platform::initialize();
}

void te::ogr::Platform::shutdown()
{
  te::ogr::Platform::finalize();
}

void te::ogr::Platform::initialize()
{
  if(sm_ogrInitialized)
    return;

// it initializes the Translator support for the TerraLib OGR driver support
  TE_ADD_TEXT_DOMAIN(TE_OGR_TEXT_DOMAIN, TE_OGR_TEXT_DOMAIN_DIR, "UTF-8");

// it initializes Logger support
  //te::common::Logger::initialize(TE_OGR_LOGGER_NAME);

// registers all format drivers built into OGR.
  OGRRegisterAll();

// it initializes the OGR Factory support
  te::ogr::DataSourceFactory::initialize();

// if we are here... the common is initialized!
  sm_ogrInitialized = true;
}

void te::ogr::Platform::finalize()
{
  if(!sm_ogrInitialized)
    return;

// it finalizes the OGR factory support.
  te::ogr::DataSourceFactory::finalize();
// Free PostGIS resources
  te::da::DataSourceManager::getInstance().detachAll(getDriverID());
 
  sm_ogrInitialized = false;
}

const std::string& te::ogr::Platform::getDriverID()
{
  static const std::string driverIdentifier(OGR_DRIVER_IDENTIFIER);
  return driverIdentifier;
}


// this should be revised!
std::string te::ogr::Platform::getDriverName(const std::string& name)
{
  boost::filesystem::path mpath(name.c_str());
  std::string ext = mpath.extension().string();
  
  if (ext == ".shp" || ext == ".SHP")
    return std::string("ESRI Shapefile");
  if (ext == ".mid"|| ext == ".MID")
	  return std::string("Mapinfo File");
  return "";
}

PLUGIN_CALL_BACK_IMPL(te::ogr::Platform)
