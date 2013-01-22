/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/Module.cpp
   
  \brief The TerraLib SQLite Data Access driver implements the plugin interface.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/query/SQLDialect.h"
//#include "../serialization/dataaccess/SQLDialect.h"
//#include "rlite/RasterFactory.h"
//#include "terralib/RasterFactory.h"
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"
#include "Module.h"
#include "Utils.h"

// Boost
#include <boost/filesystem.hpp>

// SQLite
#include <sqlite3.h>

#ifdef TE_ENABLE_SPATIALITE
// SpatiaLite
#include <spatialite.h>
#endif

#ifdef TE_ENABLE_RASTERLITE
// RasterLite
#include <rasterlite.h>
#endif

te::sqlite::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::sqlite::Module::~Module()
{
}

void te::sqlite::Module::startup() 
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib SQLite driver
  TE_ADD_TEXT_DOMAIN(TE_SQLITE_TEXT_DOMAIN, TE_SQLITE_TEXT_DOMAIN_DIR, "UTF-8");

// we need to initialize al the factories
  DataSourceFactory::initialize();
  //te::sqlite::rlite::RasterFactory::initialize();
  //te::sqlite::terralib::RasterFactory::initialize();

// it initializes the SQLite library
  sqlite3_initialize();
  
#ifdef TE_ENABLE_SPATIALITE
// it initializes the SpatiaLite library
  spatialite_init(0);
#endif

// retrieve the SQL dialect
  boost::filesystem::path driverpath(m_pluginInfo.m_folder);

  boost::filesystem::path spatialiteDialectFile = driverpath / "spatialite_dialect.xml";
  
  boost::filesystem::path nonspatialDialectFile = driverpath / "nonspatial_dialect.xml";

  //Globals::sm_spatiaLiteDialect = te::serialize::ReadDialect(spatialiteDialectFile.string());

  //Globals::sm_nonspatialDialect = te::serialize::ReadDialect(spatialiteDialectFile.string());

  TE_LOG_TRACE(TR_SQLITE("TerraLib SQLite driver startup!"));

  m_initialized = true;
}

void te::sqlite::Module::shutdown() 
{
  if(!m_initialized)
    return;

// we need to finalize all the factories registered by the driver
  te::sqlite::terralib::RasterFactory::finalize();
  te::sqlite::rlite::RasterFactory::finalize();  
  DataSourceFactory::finalize();

// we release all registered SQLite data sources
  te::da::DataSourceManager::getInstance().detachAll(Globals::sm_driverIdentifier);

// release SQL dialect
  delete Globals::sm_spatiaLiteDialect;
  Globals::sm_spatiaLiteDialect = 0;

  delete Globals::sm_nonspatialDialect;
  Globals::sm_nonspatialDialect = 0;

// shutdowns the SQLite library
  sqlite3_shutdown();

  TE_LOG_TRACE(TR_SQLITE("TerraLib SQLite driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::sqlite::Module)

