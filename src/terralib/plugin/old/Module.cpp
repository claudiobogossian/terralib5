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
  \file Module.cpp
   
  \brief This singleton defines the TerraLib Plugin module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
//#include "CppPluginEngineFactory.h"
#include "CppPluginProxyFactory.h"
#include "Module.h"
#include "PluginInfo.h"
#include "PluginManager.h"
//#include "Utils.h"

///*!
//  \brief This is a helper function that will be automatically called when the TerraLib Plugin module is loaded.
//
//  \note This function doesn't perform operations for this module.
//*/
//static void TePluginInitialize()
//{
//  te::plugin::CppPluginProxyFactory::initialize();
//
//  TE_LOG_TRACE(TR_PLUGIN("TerraLib Plugin module initialized!"));
//}
//
///*!
//  \brief This is a helper function that will be automatically called when the TerraLib Plugin module is unloaded.
//*/
//static void TePluginFinalize()
//{
//  te::plugin::CppPluginProxyFactory::finalize();
// 
//  TE_LOG_TRACE(TR_PLUGIN("TerraLib Plugin module finalized!"));
//}

const te::plugin::Module& sm_module = te::plugin::Module::getInstance();

te::plugin::Module::Module()
{
  TerraLib::Module m = { TE_PLUGIN_MODULE_NAME,
                         te::plugin::Module::initialize,
                         te::plugin::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib Plugin module
  TE_ADD_TEXT_DOMAIN(TE_PLUGIN_TEXT_DOMAIN, TE_PLUGIN_TEXT_DOMAIN_DIR, "UTF-8");

// just to make our singleton alive!
  PluginManager::getInstance();
}

te::plugin::Module::~Module()
{
}

void te::plugin::Module::initialize()
{
  te::plugin::CppPluginProxyFactory::initialize();

  TE_LOG_TRACE(TR_PLUGIN("TerraLib Plugin module initialized!"));
}

void te::plugin::Module::finalize()
{
  te::plugin::CppPluginProxyFactory::finalize();
 
  TE_LOG_TRACE(TR_PLUGIN("TerraLib Plugin module finalized!"));
}

//// TerraLib
//#include "../common/ModuleUtils.h"
//
//TE_REGISTER_MODULE(TE_PLUGIN_MODULE_NAME, TePluginInitialize, TePluginFinalize)





//#if TE_AUTOMATICALLY_LOAD_DEFAULT_XML_PLUGIN
//static te::plugin::AbstractPlugin* sg_xercesPlugins(0);
//#endif
//
///*!
//  \brief This is a helper function that will be automatically called when the TerraLib Plugin module is loaded.
//
//  \note This function doesn't perform operations for this module.
// */
//static void TePluginInitialize()
//{
//#if TE_AUTOMATICALLY_LOAD_DEFAULT_XML_PLUGIN
//  te::plugin::PluginInfo pinfo;
//  pinfo.m_name = "TERRALIB_XERCES";
//  pinfo.m_category = "XML";
//  pinfo.m_engine = TE_CPPPLUGINENGINE_CODE;
//  pinfo.m_folder = "";
//  pinfo.m_resources.push_back(te::plugin::PluginInfo::Resource("SharedLibraryName", "terralib_xerces"));
//
//  te::plugin::PluginManager::getInstance().load(pinfo, true);
//
//  sg_xercesPlugins = te::plugin::PluginManager::getInstance().detach("TERRALIB_XERCES");
//#endif
//  TE_LOG_TRACE(TR_PLUGIN("TerraLib Plugin module initialized!"));
//}
//
///*!
//  \brief This is a helper function that will be automatically called when the TerraLib Plugin module is unloaded.
// */
//static void TePluginFinalize()
//{
//  te::plugin::UnloadAllPluginsFromEngine(TE_CPPPLUGINENGINE_CODE);
//
//#if TE_AUTOMATICALLY_LOAD_DEFAULT_XML_PLUGIN
//  if(sg_xercesPlugins)
//  {
//    te::plugin::PluginManager::getInstance().unload(sg_xercesPlugins);
//  }
//#endif
//  
//  TE_LOG_TRACE(TR_PLUGIN("TerraLib Plugin module finalized!"));
//}
//
//const te::plugin::Module& sm_module = te::plugin::Module::getInstance();
//
//te::plugin::Module::Module()
//{
//  TerraLib::Module m = { TE_PLUGIN_MODULE_NAME,
//                         te::plugin::Module::initialize,
//                         te::plugin::Module::finalize
//                       };
//
//// initialize TerraLib singleton
//  TerraLib::getInstance().add(m);
//
//// it initializes the Translator support for the TerraLib Plugin module
//  TE_ADD_TEXT_DOMAIN(TE_PLUGIN_TEXT_DOMAIN, TE_PLUGIN_TEXT_DOMAIN_DIR, "UTF-8");
//
//// just to make our singleton alive!
//  PluginManager::getInstance();
//}
//
//te::plugin::Module::~Module()
//{
//// clear all registered plugins 
//  PluginManager::getInstance().unloadAll();
//
//  TerraLib::getInstance().remove(TE_PLUGIN_MODULE_NAME);
//}
//
//void te::plugin::Module::initialize()
//{
//}
//
//void te::plugin::Module::finalize()
//{
//}
//
//// TerraLib
//#include "../common/ModuleUtils.h"
//
//TE_REGISTER_MODULE(TE_PLUGIN_MODULE_NAME, TePluginInitialize, TePluginFinalize)
//
//