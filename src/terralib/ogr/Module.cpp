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
  \file terralib/ogr/Module.h
   
  \brief The TerraLib OGR driver is a plugin.
*/

// TerraLib
#include "../BuildConfig.h"
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/query/BinaryOpEncoder.h"
#include "../dataaccess/query/FunctionEncoder.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/query/SQLFunctionEncoder.h"
#include "../dataaccess/query/TemplateEncoder.h"
#include "../dataaccess/query/UnaryOpEncoder.h"
#include "DataSource.h"
#include "Globals.h"
#include "Module.h"

// OGR
#include <cpl_conv.h>
#include <ogr_api.h>

te::ogr::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::ogr::Module::~Module()
{
}

void te::ogr::Module::startup()
{
  if(m_initialized)
    return;

  std::string gdal_data;
  char* gdata = getenv("GDAL_DATA");

  if(gdata != 0)
    gdal_data = std::string(gdata);

  if(gdal_data.empty())
  {
    char* tDir = getenv("TERRALIB_HOME");
    std::string teDir;

    if(tDir != 0)
      teDir = std::string(tDir);

    if(!teDir.empty())
      gdal_data = teDir + "/share/gdal-data";
  }

  if(gdal_data.empty())
    gdal_data = TERRALIB_GDAL_DATA;

  if( !gdal_data.empty() )
  {
    CPLSetConfigOption("GDAL_DATA", gdal_data.c_str());
  }
  
  CPLSetConfigOption("GDAL_PAM_ENABLED", "NO");

// registers all format drivers built into OGR.
  OGRRegisterAll();

// it initializes the OGR Factory support
  te::da::DataSourceFactory::add(OGR_DRIVER_IDENTIFIER, te::ogr::Build);

  #include "OGRDialect.h"

#if GDAL_VERSION_NUM >= 1900
  const char* currentValue = CPLGetConfigOption("GDAL_FIX_ESRI_WKT", "");
  if(strcmp(currentValue, "") == 0) // to avoid override
    CPLSetConfigOption("GDAL_FIX_ESRI_WKT", "GEOGCS");
#endif

  TE_LOG_TRACE(TE_TR("TerraLib OGR driver startup!"));

  m_initialized = true;
}

void te::ogr::Module::shutdown()
{
  if(!m_initialized)
    return;

// it finalizes the OGR factory support.
  te::da::DataSourceFactory::remove(OGR_DRIVER_IDENTIFIER);

// free OGR registered drivers
  te::da::DataSourceManager::getInstance().detachAll(OGR_DRIVER_IDENTIFIER);
 
  OGRCleanupAll();

  DataSource::setDialect(0);

  TE_LOG_TRACE(TE_TR("TerraLib OGR driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::ogr::Module)

