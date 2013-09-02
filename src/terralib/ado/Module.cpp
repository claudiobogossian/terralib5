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
#include "../dataaccess/query/BinaryOpEncoder.h"
#include "../dataaccess/query/FunctionEncoder.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/query/SQLFunctionEncoder.h"
#include "../dataaccess/query/TemplateEncoder.h"
#include "../dataaccess/query/UnaryOpEncoder.h"
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Module.h"

te::ado::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::ado::Module::~Module()
{
}

void te::ado::Module::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib PostGIS driver support
  TE_ADD_TEXT_DOMAIN(TE_ADO_TEXT_DOMAIN, TE_ADO_TEXT_DOMAIN_DIR, "UTF-8");

  // Register the data source factory
  te::da::DataSourceFactory::add("ADO", te::ado::Build);

  //DataSourceFactory::initialize();

  //#include "PostGISCapabilities.h"
  //#include "PostGISDialect.h"

  TE_LOG_TRACE(TR_ADO("TerraLib ADO driver support initialized!"));

  m_initialized = true;
}

void te::ado::Module::shutdown()
{
  if(!m_initialized)
    return;

  // Unregister the ADO factory support.
  te::da::DataSourceFactory::remove("ADO");

  //te::ado::DataSourceFactory::finalize();
  //DataSource::setDialect(0);

  // Free the PostGIS registered drivers
  //te::da::DataSourceManager::getInstance().detachAll(PGIS_DRIVER_IDENTIFIER);
 
  TE_LOG_TRACE(TR_ADO("TerraLib ADO driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::ado::Module)
