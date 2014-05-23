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
  \file terralib/wfs/Module.cpp
   
  \brief The TerraLib WFS driver as a plugin.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/datasource/DataSourceManager.h"
#include "../dataaccess/query/BinaryOpEncoder.h"
#include "../dataaccess/query/FunctionEncoder.h"
#include "../dataaccess/query/SQLDialect.h"
#include "../dataaccess/query/UnaryOpEncoder.h"
#include "../plugin/PluginInfo.h"
#include "Config.h"
#include "DataSourceFactory.h"
#include "Module.h"

// OGR
#include <cpl_conv.h>
#include <ogr_api.h>

// Boost
#include <boost/functional/factory.hpp>
#include <boost/bind.hpp>

te::wfs::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::wfs::Module::~Module()
{
}

void te::wfs::Module::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib WFS driver support
  TE_ADD_TEXT_DOMAIN(TE_WFS_TEXT_DOMAIN, TE_WFS_TEXT_DOMAIN_DIR, "UTF-8");

  te::da::DataSourceFactory::add(TE_WFS_DRIVER_IDENTIFIER, te::wfs::Build);

  #include "WFSDialect.h"

  //CPLSetConfigOption("OGR_WFS_PAGING_ALLOWED", "YES");
  CPLSetConfigOption("OGR_WFS_USE_STREAMING", "NO");

// registers all format drivers built into OGR.
  OGRRegisterAll();

  TE_LOG_TRACE(TR_WFS("TerraLib WFS driver startup!"));

  m_initialized = true;
}

void te::wfs::Module::shutdown()
{
  if(!m_initialized)
    return;

  te::da::DataSourceFactory::remove(TE_WFS_DRIVER_IDENTIFIER);

  te::da::DataSourceManager::getInstance().detachAll(TE_WFS_DRIVER_IDENTIFIER);

  OGRCleanupAll();

  TE_LOG_TRACE(TR_WFS("TerraLib WFS driver shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::wfs::Module)
