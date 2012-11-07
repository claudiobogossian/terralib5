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
  \file PluginManager.cpp
  
  \brief A singleton for managing plugins.
 */

// TerraLib
#include "../common/Exception.h"
#include "../common/Logger.h"
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "Plugin.h"
#include "PluginFactory.h"
#include "PluginManager.h"

te::plugin::PluginManager::~PluginManager()
{
  te::common::FreeContents(m_pluginsMap);
}

void te::plugin::PluginManager::loadPlugin(const PluginInfo& pluginInfo)
{
  Plugin* p = te::common::GetPValue(m_pluginsMap, pluginInfo.m_name);

  if(p)
  {
    std::string m =  TR_PLUGIN("There is already a plugin with the given name: ");
                m += pluginInfo.m_name;
                m += ".";

    throw te::common::Exception(m);
  }

// let's create a plugin instance and try to startup it
  p = PluginFactory::make(pluginInfo.m_type, pluginInfo);

  try
  {
    p->startup();  
  }
  catch(...)
  {
    delete p;
    throw;
  }

// if we are here, so plugin is already started and everything is ok!
  m_pluginsMap.insert(std::map<std::string, Plugin*>::value_type(pluginInfo.m_name, p));
}

void te::plugin::PluginManager::unloadPlugin(const std::string& name)
{
  Plugin* p = te::common::GetPValue(m_pluginsMap, name);

  if(p == 0)
  {
    std::string m =  TR_PLUGIN("There isn't a plugin with the given name: ");
                m += name;
                m += ".";

    throw te::common::Exception(m);
  }

  p->shutdown();

// if we are here, the plugin has shutdown ok!
// so... just delete the plugin and remove it from the map
  delete p;

  m_pluginsMap.erase(name);
}

void te::plugin::PluginManager::unloadAll()
{
  std::map<std::string, Plugin*>::iterator it = m_pluginsMap.begin();

  while(it != m_pluginsMap.end())
  {
    std::string pluginname = it->first;
    ++it;
    forceUnloadPlugin(pluginname);
  }
}

void te::plugin::PluginManager::forceUnloadPlugin(const std::string& name)
{
  Plugin* p = te::common::GetPValue(m_pluginsMap, name);

  if(p == 0)
  {
    std::string m =  TR_PLUGIN("There isn't a plugin with the given name: ");
                m += name;
                m += "!";

    TE_LOG_TRACE(m.c_str());

    return;
  }

  try
  {
    p->shutdown();
    delete p;
  }
  catch(...)
  {
    std::string m =  TR_PLUGIN("Forced unload of plugin ");
                m += name;
                m += TR_PLUGIN("has failed! We are just removing the plugin from the list of plugins... please, contact the plugin developer!");

    TE_LOG_TRACE(m.c_str());
  }

  m_pluginsMap.erase(name);
}

te::plugin::PluginManager::PluginManager()
{
}

