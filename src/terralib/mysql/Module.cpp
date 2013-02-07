/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mysql/Module.cpp
   
  \brief An utility class to initialize and terminate TerraLib MySQL driver support.  
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../serialization/dataaccess/DataSourceCapabilities.h"
#include "DataSourceFactory.h"
#include "Globals.h"
#include "Module.h"
//#include "RasterFactory.h"
//#include "RasterTableFactory.h"

// Boost
#include <boost/filesystem.hpp>

te::mysql::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::mysql::Module::~Module()
{
}

void te::mysql::Module::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib MySQL driver
  TE_ADD_TEXT_DOMAIN(TE_MYSQL_TEXT_DOMAIN, TE_MYSQL_TEXT_DOMAIN_DIR, "UTF-8");

// initializes the factories
  DataSourceFactory::initialize();
  //RasterFactory::initialize();
  //RasterTableFactory::initialize();

// retrieve the Capabilities
  boost::filesystem::path driverpath(m_pluginInfo.m_folder);
  
  boost::filesystem::path capabilitiesFile = driverpath / "mysql-capabilities.xml";

  te::mysql::Globals::sm_capabilities = new te::da::DataSourceCapabilities();
  te::mysql::Globals::sm_queryDialect = new te::da::SQLDialect();

  te::serialize::Read(capabilitiesFile.string(), *te::mysql::Globals::sm_capabilities, *te::mysql::Globals::sm_queryDialect);

  TE_LOG_TRACE(TR_MYSQL("TerraLib MySQL driver startup!"));

  m_initialized = true;
}

void te::mysql::Module::shutdown()
{
  if(!m_initialized)
    return;

// finalizes the postgis factory
  //RasterTableFactory::finalize();
  //RasterFactory::finalize();
  DataSourceFactory::finalize();

// free registered MySQL data sources
  te::da::DataSourceManager::getInstance().detachAll(Globals::sm_driverIdentifier);

// release Query dialect
  delete Globals::sm_queryDialect;
  Globals::sm_queryDialect = 0;

  TE_LOG_TRACE(TR_MYSQL("TerraLib MySQL driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::mysql::Module)

