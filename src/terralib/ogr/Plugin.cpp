/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

  This file is part of the TerraLib - a Framework for building GIS enabled
  applications.

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
  \file terralib/ogr/Plugin.cpp

  \brief Plugin interface for dynamic loading of OGR Driver for TerraLib.
*/

// TerraLib
#include "../BuildConfig.h"
#include "../Defines.h"
#include "../core/filesystem/FileSystem.h"
#include "../core/logger/Logger.h"
#include "../core/plugin/CppPlugin.h"
#include "../core/translator/Translator.h"
#include "../core/utils/Platform.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/query/BinaryOpEncoder.h"
#include "../dataaccess/query/FunctionEncoder.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/query/SQLFunctionEncoder.h"
#include "../dataaccess/query/TemplateEncoder.h"
#include "../dataaccess/query/UnaryOpEncoder.h"
#include "DataSource.h"
#include "Globals.h"
#include "Utils.h"

// Boost
#include <boost/filesystem.hpp>

// OGR
#include <cpl_conv.h>
#include <ogr_api.h>
namespace te
{
  namespace ogr
  {
    TERRALIB_CPP_PLUGIN_BEGIN(Plugin)

    TERRALIB_CPP_PLUGIN_STARTUP
    {
      if(m_initialized)
        return;

      // for all platforms, first look at an environment variable
      // defined by macro TERRALIB_GDAL_DATA.
      // note: TERRALIB_GDAL_DATA is detected by CMAKE.
      // note: installed versions on developers machine may look for this
      // version of GDAL
      std::string gdal_data_dir(TERRALIB_GDAL_DATA);

      // if the above variable is not set or it points to an invalid directory
      if(gdal_data_dir.empty() ||
         !te::core::FileSystem::isDirectory(gdal_data_dir))
      {
        // search for GDAL in a PATH relative to TerraLib.
        // note: each SO will look in a different folder
        #if defined(TE_PLATFORM) && defined(TE_PLATFORMCODE_MSWINDOWS)
        #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
                gdal_data_dir = te::core::FindInTerraLibPath("/share/data");
        #elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
                gdal_data_dir = te::core::FindInTerraLibPath("/share/gdal");
        #elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
                gdal_data_dir = te::core::FindInTerraLibPath("/share/gdal");
        #else
        #error "unsupported plataform: please, contact terralib-team@terralib.org"
        #endif
        #else
        #error \
            "the macro TE_PLATFORM is not set, please, contact terralib-team@terralib.org"
        #endif
      }

      if(!gdal_data_dir.empty())
      {
        CPLSetConfigOption("GDAL_DATA", gdal_data_dir.c_str());
      }

      CPLSetConfigOption("GDAL_DATA", gdal_data_dir.c_str());

      CPLSetConfigOption("GDAL_PAM_ENABLED", "NO");

      // registers all format drivers built into OGR.
      OGRRegisterAll();

      // it initializes the OGR Factory support
      te::da::DataSourceFactory::add(OGR_DRIVER_IDENTIFIER, te::ogr::Build);

      #include "OGRDialect.h"

      #if GDAL_VERSION_NUM >= 1900
      const char* currentValue = CPLGetConfigOption("GDAL_FIX_ESRI_WKT", "");
      if(strcmp(currentValue, "") == 0)  // to avoid override
        CPLSetConfigOption("GDAL_FIX_ESRI_WKT", "GEOGCS");
      #endif

      // initializing the static mutex
      getStaticMutex();

      TE_LOG_TRACE(TE_TR("TerraLib OGR driver startup!"));

      m_initialized = true;
    }

    TERRALIB_CPP_PLUGIN_SHUTDOWN
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
    TERRALIB_CPP_PLUGIN_END(te::ogr::Plugin)
  }
}
