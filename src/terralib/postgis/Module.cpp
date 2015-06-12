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

// TerraLib
#include "../common/Logger.h"
#include "../common/PlatformUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/query/BinaryOpEncoder.h"
#include "../dataaccess/query/FunctionEncoder.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/query/SQLFunctionEncoder.h"
#include "../dataaccess/query/TemplateEncoder.h"
#include "../dataaccess/query/UnaryOpEncoder.h"
#include "../dataaccess/serialization/xml/Serializer.h"
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"
#include "Module.h"

// Boost
#include <boost/filesystem.hpp>

te::pgis::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::pgis::Module::~Module()
{
}

void te::pgis::Module::startup()
{
  if(m_initialized)
    return;

// Register the data source factory
  te::da::DataSourceFactory::add("POSTGIS", te::pgis::Build);
  
// retrieve the Capabilities
  std::string capabilitiesFile = te::common::FindInTerraLibPath("share/terralib/plugins/postgis-capabilities.xml");

  te::pgis::Globals::sm_capabilities = new te::da::DataSourceCapabilities();
  te::pgis::Globals::sm_queryDialect = new te::da::SQLDialect();

  te::serialize::xml::Read(capabilitiesFile, *te::pgis::Globals::sm_capabilities, *te::pgis::Globals::sm_queryDialect);

  TE_LOG_TRACE(TE_TR("TerraLib PostGIS driver support initialized!"));

  m_initialized = true;
}

void te::pgis::Module::shutdown()
{
  if(!m_initialized)
    return;

// Unregister the PostGIS factory support.
  te::da::DataSourceFactory::remove("POSTGIS");

// Free the PostGIS registered drivers
  te::da::DataSourceManager::getInstance().detachAll(PGIS_DRIVER_IDENTIFIER);
 
  TE_LOG_TRACE(TE_TR("TerraLib PostGIS driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::pgis::Module)
