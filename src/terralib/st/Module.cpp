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
  \file terralib/st/Module.cpp
   
  \brief It defines the TerraLib ST module as a plugin. 
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"

#include "loader/STDataLoaderFromDSFactory.h"
#include "loader/STDataLoaderFromMemDSFactory.h"
#include "Config.h"
#include "Module.h"

te::st::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo)
{
}

te::st::Module::~Module()
{
}

void te::st::Module::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib ST support
  TE_ADD_TEXT_DOMAIN(TE_ST_TEXT_DOMAIN, TE_ST_TEXT_DOMAIN_DIR, "UTF-8");

// it initializes the st data loader factories
  te::st::STDataLoaderFromDSFactory::initialize();
  te::st::STDataLoaderFromMemDSFactory::initialize();

  TE_LOG_TRACE(TR_ST("TerraLib ST module startup!"));

  m_initialized = true;
}

void te::st::Module::shutdown()
{
  if(!m_initialized)
    return;

// it finalizes the st data loader factory support.
  te::st::STDataLoaderFromDSFactory::finalize();
  te::st::STDataLoaderFromMemDSFactory::finalize();

  TE_LOG_TRACE(TR_ST("TerraLib ST module shutdown!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::st::Module)


/* TE_ST_MODULE_NAME */

