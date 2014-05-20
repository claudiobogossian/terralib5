/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/query/SQLDialect.h"
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"
#include "Module.h"

#ifdef WIN32
  // Terralib 4.x
  #include <terralib4/drivers/ado/TeAdoDB.h>
#endif

// Boost
#include <boost/filesystem.hpp>

terralib4::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

terralib4::Module::~Module()
{
}

void terralib4::Module::startup()
{
  if(m_initialized)
    return;

  // Register the data source factory
  te::da::DataSourceFactory::add("TERRALIB4", terralib4::Build);

  terralib4::Globals::sm_capabilities = new te::da::DataSourceCapabilities();
  terralib4::Globals::sm_queryDialect = new te::da::SQLDialect();

  TE_LOG_TRACE(TE_TR("TerraLib Terralib 4 driver support initialized!"));

  m_initialized = true;
}

void terralib4::Module::shutdown()
{
  if(!m_initialized)
    return;

  te::da::DataSourceFactory::remove("TERRALIB4");

  te::da::DataSourceManager::getInstance().detachAll(TERRALIB4_DRIVER_IDENTIFIER);
 
  TE_LOG_TRACE(TE_TR("TerraLib Terralib 4 driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(terralib4::Module)
