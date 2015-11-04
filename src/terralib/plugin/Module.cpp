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
  \file terralib/plugin/Module.cpp

  \brief This singleton defines the TerraLib Plugin module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "AbstractPlugin.h"
#include "CppPluginEngineFactory.h"
#include "Module.h"
#include "PluginInfo.h"
#include "PluginManager.h"
#include "Utils.h"

#if TE_AUTOMATICALLY_LOAD_DEFAULT_XML_PLUGIN
static te::plugin::AbstractPlugin* sg_xercesPlugins(0);
#endif

const te::plugin::Module& sm_module = te::plugin::Module::getInstance();

te::plugin::Module::Module()
{
  TerraLib::Module m = { TE_PLUGIN_MODULE_NAME,
                         te::plugin::Module::initialize,
                         te::plugin::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// just to make our singleton alive!
  PluginManager::getInstance();
}

te::plugin::Module::~Module()
{
  TerraLib::getInstance().remove(TE_PLUGIN_MODULE_NAME);
}

void te::plugin::Module::initialize()
{
#if TE_AUTOMATICALLY_LOAD_DEFAULT_XML_PLUGIN
  te::plugin::PluginInfo pinfo;
  pinfo.m_name = "TERRALIB_XERCES";
  pinfo.m_category = "XML";
  pinfo.m_engine = TE_CPPPLUGINENGINE_CODE;
  //pinfo.m_folder = "."; //the default search directories will be used
  pinfo.m_resources.push_back(te::plugin::PluginInfo::Resource("SharedLibraryName", "terralib_mod_xerces"));

  te::plugin::PluginManager::getInstance().load(pinfo, true);

  sg_xercesPlugins = te::plugin::PluginManager::getInstance().detach("TERRALIB_XERCES");
#endif
  TE_LOG_TRACE(TE_TR("TerraLib Plugin module initialized!"));
}

void te::plugin::Module::finalize()
{
  te::plugin::UnloadAllPluginsFromEngine(TE_CPPPLUGINENGINE_CODE);

// then, clear all registered plugins 
  PluginManager::getInstance().unloadAll();

#if TE_AUTOMATICALLY_LOAD_DEFAULT_XML_PLUGIN
  if(sg_xercesPlugins)
  {
    sg_xercesPlugins->shutdown();
  }
#endif

  TE_LOG_TRACE(TE_TR("TerraLib Plugin module finalized!"));
}



