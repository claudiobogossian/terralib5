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
  \file terralib/gdal/Module.h
   
  \brief The TerraLib OGR driver is a plugin.
*/

// TerraLib
#include "../BuildConfig.h"
#include "../common/Logger.h"
#include "../plugin/PluginInfo.h"
#include "../common/Translator.h"
#include "../common/PlatformUtils.h"
#include "../Defines.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "Config.h"
#include "DataSourceFactory.h"
#include "Module.h"
#include "RasterFactory.h"
#include "Utils.h"

// STL
#include <cstdlib>
#include <iostream>

// GDAL
#include <gdal_priv.h>

te::gdal::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::gdal::Module::~Module()
{
}

void te::gdal::Module::startup()
{
  if(m_initialized)
    return;
// for all platforms, first look at an environment variable 
// defined by macro TERRALIB_GDAL_DATA.
// note: TERRALIB_GDAL_DATA is detected by CMAKE.
// note: installed versions on developers machine may look for this version of GDAL
  std::string gdal_data_dir(TERRALIB_GDAL_DATA);

// if the above variable is not set or it points to an invalid directory
  if(gdal_data_dir.empty() || !boost::filesystem::is_directory(gdal_data_dir))
  {
// search for GDAL in a PATH relative to TerraLib.
// note: each SO will look in a different folder
#if defined(TE_PLATFORM) && defined(TE_PLATFORMCODE_MSWINDOWS)
  #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
    gdal_data_dir = te::common::FindInTerraLibPath("/share/data");
  #elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
    gdal_data_dir = te::common::FindInTerraLibPath("/share/gdal");
  #elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
    gdal_data_dir= te::common::FindInTerraLibPath("/share/gdal");
  #else
    #error "unsupported plataform: please, contact terralib-team@terralib.org"
  #endif
#else 
  #error "the macro TE_PLATFORM is not set, please, contact terralib-team@terralib.org"
#endif
  }
  
  if(!gdal_data_dir.empty())
  {
    CPLSetConfigOption("GDAL_DATA", gdal_data_dir.c_str());
  }

  CPLSetConfigOption("GDAL_PAM_ENABLED", "NO");

  te::da::DataSourceFactory::add(TE_GDAL_DRIVER_IDENTIFIER, te::gdal::Build);

  GDALAllRegister();

  RasterFactory::initialize();

  // DataType Capabilites
  te::da::DataTypeCapabilities dataTypeCapabilities;
  dataTypeCapabilities.setSupportRaster(true);

  // DataSetType Capabilites
  te::da::DataSetTypeCapabilities dataSetTypeCapabilities; // All false

  // DataSet Capabilities
  te::da::DataSetCapabilities dataSetCapabilities;
  dataSetCapabilities.setSupportEfficientDataSetSize(true);

  // Query Capabilities
  te::da::QueryCapabilities queryCapabilities; // All false

  // DataSouce Capabilities
  te::da::DataSourceCapabilities capabilities;
  capabilities.setSupportAll();
  capabilities.setSupportTransactions(false);
  capabilities.setDataTypeCapabilities(dataTypeCapabilities);
  capabilities.setDataSetTypeCapabilities(dataSetTypeCapabilities);
  capabilities.setDataSetCapabilities(dataSetCapabilities);
  capabilities.setQueryCapabilities(queryCapabilities);
  capabilities.setAccessPolicy(te::common::RWAccess);
  
  // Supported file extensions capability
  
  std::set< std::string > supportedExtensionsSet;
  for( std::map< std::string, DriverMetadata >::const_iterator it = 
    GetGDALDriversMetadata().begin() ; it != 
    GetGDALDriversMetadata().end() ; ++it )
  {
    if( !it->second.m_extension.empty() )
    {
      if( supportedExtensionsSet.find( it->second.m_extension ) ==
        supportedExtensionsSet.end() )
      {
        supportedExtensionsSet.insert( it->second.m_extension );
      }
    }
  }
  
  std::string supportedExtensionsStr;
  for( std::set< std::string >::const_iterator it = supportedExtensionsSet.begin() ; 
    it != supportedExtensionsSet.end() ; ++it )
  {
    if( !supportedExtensionsStr.empty() )
    {
      supportedExtensionsStr.append( ";" );
    }
    supportedExtensionsStr.append( *it );
  }  

  capabilities.addSpecificCapability( "SUPPORTED_EXTENSIONS", supportedExtensionsStr );

  te::gdal::DataSource::setCapabilities(capabilities);
  
  // initializing the static mutex
  getStaticMutex();
  
  TE_LOG_TRACE(TE_TR("TerraLib GDAL driver startup!"));

  m_initialized = true;
}

void te::gdal::Module::shutdown()
{
  if(!m_initialized)
    return;

//! it finalizes the GDAL factory support.
 te::da::DataSourceFactory::remove(TE_GDAL_DRIVER_IDENTIFIER);

  RasterFactory::finalize();

//! free GDAL registered drivers
  te::da::DataSourceManager::getInstance().detachAll(TE_GDAL_DRIVER_IDENTIFIER);
 
  TE_LOG_TRACE(TE_TR("TerraLib GDAL driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::gdal::Module)

