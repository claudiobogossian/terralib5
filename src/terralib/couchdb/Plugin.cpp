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
  \file terralib/couchdb/Plugin.h

  \brief An utility class to load CouchDB driver as a plugin.
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

// libcURL
#include <curl/curl.h>

te::couchdb::Plugin::Plugin(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo),
    m_initialized(false)
{
}

te::couchdb::Plugin::~Plugin()
{
}

void te::couchdb::Plugin::startup() 
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib CouchDB driver
  TE_ADD_TEXT_DOMAIN(TE_COUCHDB_TEXT_DOMAIN, TE_COUCHDB_TEXT_DOMAIN_DIR, "UTF-8");

// initializes the CouchDB factory
  DataSourceFactory::initialize();

// initializes cURL
  CURLcode code = curl_global_init(CURL_GLOBAL_ALL);

  if(code)
    throw Exception(TR_COUCHDB("Could not initialize libcURL!"));


  TE_LOG_TRACE(TR_COUCHDB("TerraLib CouchDB driver startup!"));

  m_initialized = true;
}

void te::couchdb::Plugin::shutdown() 
{
  if(!m_initialized)
    return;

// finalizes the CouchDB factory
  DataSourceFactory::finalize();

// free registered CouchDB data sources
  te::da::DataSourceManager::getInstance().detachAll(Globals::sm_driverIdentifier);

// finalizes libcURL
  curl_global_cleanup();

  TE_LOG_TRACE(TR_COUCHDB("TerraLib CouchDB driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::couchdb::Plugin)

