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
  \file terralib/plugin/CppPluginEngine.cpp
  
  \brief A plugin engine for plugins written in C++.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/Library.h"
#include "../common/LibraryManager.h"
#include "../common/Logger.h"
#include "../common/Translator.h"
#include "CppPluginEngine.h"
#include "CppPluginProxy.h"
#include "Plugin.h"
#include "PluginInfo.h"

// STL
#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>

// Boost
#include <boost/filesystem.hpp>

te::plugin::AbstractPlugin* te::plugin::CppPluginEngine::load(const PluginInfo& pInfo)
{
// get the plugin's shared library name in the resources list
  std::vector<PluginInfo::Resource>::const_iterator it = std::find_if(pInfo.m_resources.begin(),
                                                                      pInfo.m_resources.end(),
                                                                      PluginInfo::Finder1st("SharedLibraryName"));

  if(it == pInfo.m_resources.end())
  {
    std::string m  = TR_PLUGIN("Shared library name not informed for plugin: ");
                m += pInfo.m_name;
                m += "!";

    throw te::common::Exception(m);
  }

// get the adequate name depending on the platform and build type (release or debug)
  std::string libName = getPluginFileName(it->second);

// is there a library already loaded with this name?
  te::common::LibraryPtr slib(te::common::LibraryManager::getInstance().find(libName));

  if(slib.get() == 0)
  {
// if not loaded, load it!
    
// the plugin library file may be in a special dir informed by pInfo.m_folder
    boost::filesystem::path pluginFile(pInfo.m_folder);

    pluginFile /= libName;

// create shared library entry but doesn't load it yet!
    slib.reset(new te::common::Library(pluginFile.string(), true));
  }
  
  if(!slib->isLoaded())
    slib->load();

// now we need to get the plugin constructor function address
  GetPluginFPtr getPluginFptr = (GetPluginFPtr) (slib->getAddress("CppPluginGetInstance"));

  if(getPluginFptr == NULL)
  {
    std::string m  = TR_PLUGIN("Could not find CppPluginGetInstance function into the plugin's code: ");
                m += pInfo.m_name;
                m += "!";

    throw te::common::Exception(m);
  }

  std::auto_ptr<Plugin> cppPlugin(getPluginFptr(pInfo));

  if(cppPlugin.get() == 0)
  {
    std::string m  = TR_PLUGIN("CppPluginGetInstance returned a null plugin instance: ");
                m += pInfo.m_name;
                m += "!";

    throw te::common::Exception(m);
  }

  CppPluginProxy* proxy = new CppPluginProxy(slib, cppPlugin.release());

  te::common::LibraryManager::getInstance().add(libName, slib);

  return proxy;
}

void te::plugin::CppPluginEngine::unload(AbstractPlugin* plugin)
{
  assert(plugin && (plugin->getInfo().m_engine == "C++"));

  delete plugin;
}

std::string te::plugin::CppPluginEngine::getPluginFileName(const std::string& libName)
{
#ifdef NDEBUG
  std::string nativeName = libName;
#else
  std::string nativeName = libName + "_d";  // add a suffix _d to the library name in debug mode
#endif
  nativeName = te::common::Library::getNativeName(nativeName);

  return nativeName;
}

