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
  \file terralib/ogr/Module.h
   
  \brief The TerraLib OGR driver is a plugin.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "DataSource.h"
#include "DataSourceFactory.h"
#include "Globals.h"
#include "Module.h"

// OGR
#include <ogrsf_frmts.h>

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

// it initializes the Translator support for the TerraLib PostGIS driver support
  TE_ADD_TEXT_DOMAIN(TE_OGR_TEXT_DOMAIN, TE_OGR_TEXT_DOMAIN_DIR, "UTF-8");

// registers all format drivers built into OGR.
  OGRRegisterAll();

// it initializes the OGR Factory support
  te::ogr::DataSourceFactory::initialize();

  TE_LOG_TRACE(TR_OGR("TerraLib OGR driver startup!"));

  m_initialized = true;
}

void te::ogr::Module::shutdown()
{
  if(!m_initialized)
    return;

// it finalizes the OGR factory support.
  te::ogr::DataSourceFactory::finalize();

// free OGR registered drivers
  te::da::DataSourceManager::getInstance().detachAll(OGR_DRIVER_IDENTIFIER);
 
  TE_LOG_TRACE(TR_OGR("TerraLib OGR driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::ogr::Module)

