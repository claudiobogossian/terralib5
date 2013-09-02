/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/stmemory/Module.cpp
   
  \brief It defines the TerraLib ST module as a plugin. 
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "../dataaccess2/datasource/DataSourceFactory.h"

#include "Config.h"
#include "DataSourceFactory.h"
#include "Module.h"


te::stmem::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::stmem::Module::~Module()
{
}

void te::stmem::Module::startup()
{
  if(m_initialized)
    return;

  // register data source factory
  te::da::DataSourceFactory::add(TE_STMEMORY_DRIVER_IDENTIFIER, te::stmem::Build);
    
  // it initializes the Translator support for the TerraLib ST support
  TE_ADD_TEXT_DOMAIN(TE_STMEMORY_TEXT_DOMAIN, TE_STMEMORY_TEXT_DOMAIN_DIR, "UTF-8");

  TE_LOG_TRACE(TR_STMEMORY("TerraLib ST memory module startup!"));

  m_initialized = true;
}

void te::stmem::Module::shutdown()
{
  if(!m_initialized)
    return;

  TE_LOG_TRACE(TR_STMEMORY("TerraLib ST module shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::stmem::Module)


/* TE_ST_MODULE_NAME */

