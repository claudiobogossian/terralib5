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
  \file terralib/gdal/Module.h
   
  \brief The TerraLib OGR driver is a plugin.
*/

// TerraLib
#include "../common/Logger.h"
#include "../plugin/PluginInfo.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "Config.h"
#include "DataSourceFactory.h"
#include "Module.h"
#include "RasterFactory.h"

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

  te::gdal::DataSource::setCapabilities(capabilities);
  
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

