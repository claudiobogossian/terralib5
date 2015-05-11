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

/*!
  \file Module.cpp
   
  \brief The TerraLib V8 Plugin module is itself a plugin.
 */

// TerraLib
#include "../../../common/Logger.h"
#include "../../../common/Translator.h"
#include "Module.h"
#include "PluginEngineFactory.h"

te::v8::plugin::Module::Module(const te::plugin::PluginInfo& pluginInfo)
  : te::plugin::Plugin(pluginInfo),
    m_initialized(false)
{
}

te::v8::plugin::Module::~Module()
{
}

void te::v8::plugin::Module::startup()
{
  if(m_initialized)
    return;

// it initializes the Translator support for the TerraLib Java Plugin support
 TE_ADD_TEXT_DOMAIN(TE_V8PLUGIN_TEXT_DOMAIN, TE_V8PLUGIN_TEXT_DOMAIN_DIR, "UTF-8");

// it initializes the java plugin engine factory
  PluginEngineFactory::initialize();

  TE_LOG_TRACE(TR_V8PLUGIN("TerraLib's Google JavaScript V8 engine module initialized!"));

  m_initialized = true;
}

void te::v8::plugin::Module::shutdown()
{
  if(!m_initialized)
    return;

// it finalizes the plugin engine factory
  PluginEngineFactory::finalize();

  TE_LOG_TRACE(TR_V8PLUGIN("TerraLib's Google JavaScript V8 engine module finalized!"));

  m_initialized = false;
}

PLUGIN_CALL_BACK_IMPL(te::v8::plugin::Module)

