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
  \file terralib/mongodb/Plugin.h

  \brief An utility class to load MongoDB driver as a plugin.
 */

// TerraLib
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "Config.h"
#include "DataSourceFactory.h"
#include "Exception.h"
#include "Globals.h"
#include "Plugin.h"

te::mongodb::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo),
    m_initialized(false)
{
}

te::mongodb::Plugin::~Plugin()
{
}

void te::mongodb::Plugin::startup() 
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib MongoDB driver
  TE_ADD_TEXT_DOMAIN(TE_MONGODB_TEXT_DOMAIN, TE_MONGODB_TEXT_DOMAIN_DIR, "UTF-8");

// initializes the MongoDB factory
  DataSourceFactory::initialize();

  TE_LOG_TRACE(TR_MONGODB("TerraLib MongoDB driver startup!"));

  m_initialized = true;
}

void te::mongodb::Plugin::shutdown() 
{
  if(!m_initialized)
    return;

// finalizes the MongoDB factory
  DataSourceFactory::finalize();

// free registered MongoDB data sources
  te::da::DataSourceManager::getInstance().detachAll(Globals::sm_driverIdentifier);

  TE_LOG_TRACE(TR_MONGODB("TerraLib MongoDB driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::mongodb::Plugin)

