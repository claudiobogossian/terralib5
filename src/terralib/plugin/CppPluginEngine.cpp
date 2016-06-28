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
  \file terralib/plugin/CppPluginEngine.cpp

  \brief A plugin engine for plugins written in C++.
*/

// TerraLib
#include "../common/Exception.h"
#include "../core/lib/Library.h"
#include "../core/lib/LibraryManager.h"
#include "../core/logger/Logger.h"
#include "../core/translator/Translator.h"
#include "../common/PlatformUtils.h"
#include "../Defines.h"
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

te::plugin::CppPluginEngine::CppPluginEngine()
{
}

te::plugin::AbstractPlugin* te::plugin::CppPluginEngine::load(const PluginInfo& pInfo)
{
// get the plugin's shared library name in the resources list
  std::vector<PluginInfo::Resource>::const_iterator it = std::find_if(pInfo.m_resources.begin(),
                                                                      pInfo.m_resources.end(),
                                                                      PluginInfo::Finder1st("SharedLibraryName"));

  if(it == pInfo.m_resources.end())
  {
    std::string m  = TE_TR("Shared library name not informed for plugin: ");
                m += pInfo.m_name;
                m += "!";

    throw te::common::Exception(m);
  }

// get the adequate name depending on the platform and build type (release or debug)
  std::string libName = getPluginFileName(it->second);

// is there a library already loaded with this name?
  std::shared_ptr<te::core::Library> slib(new te::core::Library(libName));

  if(!slib->isLoaded())
    slib->load();
  else
  {
// if not loaded yet, load it!

// the plugin library file may be in a special dir informed by pInfo.m_folder

    std::string errorMessage;

    boost::filesystem::path pluginFile(libName);

    slib.reset(new te::core::Library(pluginFile.string(), true));

    try
    {
      slib->load();
    }
    catch(te::common::Exception& exc)
    {
      errorMessage = libName;
    }

    if(!slib->isLoaded())
    {
// search for alternative paths
      std::vector<std::string> alternative_paths;

#if TE_PLATFORM == TE_PLATFORMCODE_APPLE
      alternative_paths.push_back("./");
#endif

// 1st: in a place indicated by a environment variable defined in macro TERRALIB_DIR_VAR_NAME
      const char* home_dir = getenv(TERRALIB_DIR_VAR_NAME);

      if(home_dir != 0)
        alternative_paths.push_back(home_dir);

// 2nd: in the install prefix dir
      alternative_paths.push_back(TERRALIB_INSTALL_PREFIX_PATH);

      for(std::size_t i = 0; i != alternative_paths.size(); ++i)
      {
        try
        {
          pluginFile = alternative_paths[i];
          pluginFile /= libName;
          slib.reset(new te::core::Library(pluginFile.string(), true));
          slib->load();
          errorMessage.clear();
          break;
        }
        catch(te::common::Exception& exc)
        {
          errorMessage += '\n';
          errorMessage += pluginFile.string();
        }

        try
        {
          pluginFile = alternative_paths[i];
          pluginFile /= "lib";
          pluginFile /= libName;
          slib.reset(new te::core::Library(pluginFile.string(), true));
          slib->load();
          errorMessage.clear();
          break;
        }
        catch(te::common::Exception& exc)
        {
          errorMessage += '\n';
          errorMessage += pluginFile.string();
        }

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
        try
        {
          pluginFile = alternative_paths[i];
          pluginFile /= "Release";
          pluginFile /= libName;
          slib.reset(new te::core::Library(pluginFile.string(), true));
          slib->load();
          errorMessage.clear();
          break;
        }
        catch(te::common::Exception& exc)
        {
          errorMessage += '\n';
          errorMessage += pluginFile.string();
        }

        try
        {
          pluginFile = alternative_paths[i];
          pluginFile /= "Debug";
          pluginFile /= libName;
          slib.reset(new te::core::Library(pluginFile.string(), true));
          slib->load();
          errorMessage.clear();
          break;
        }
        catch(te::common::Exception& exc)
        {
          errorMessage += '\n';
          errorMessage += pluginFile.string();
        }
#endif
      }

      if(!slib->isLoaded())
      {
        std::string m  = TE_TR("Could not find shared library as: ");
        if (!errorMessage.empty())
          m = m + "\n" + errorMessage.c_str();
        else
          m = m + ' '+ TE_TR("described in ") + pInfo.m_name + '.';

        throw te::common::Exception(m);
      }
    }
  }

  if(slib.get() == 0)
  {
    std::string m  = TE_TR("Could not find shared library described in ");
                m += pInfo.m_name;
    throw te::common::Exception(m);
  }

// now we need to get the plugin constructor function address
  GetPluginFPtr getPluginFptr = (GetPluginFPtr) (slib->getAddress("CppPluginGetInstance"));

  if(getPluginFptr == NULL)
  {
    std::string m  = TE_TR("Could not find CppPluginGetInstance function into the plugin's code: ");
                m += pInfo.m_name;
                m += "!";

    throw te::common::Exception(m);
  }

  std::auto_ptr<Plugin> cppPlugin(getPluginFptr(pInfo));

  if(cppPlugin.get() == 0)
  {
    std::string m  = TE_TR("CppPluginGetInstance returned a null plugin instance: ");
                m += pInfo.m_name;
                m += "!";

    throw te::common::Exception(m);
  }

  CppPluginProxy* proxy = new CppPluginProxy(slib, cppPlugin.release());

  te::core::LibraryManager::instance().insert(libName, slib);

  return proxy;
}

void te::plugin::CppPluginEngine::unload(AbstractPlugin* plugin)
{
  assert(plugin && (plugin->getInfo().m_engine == "C++"));

  delete plugin;
}

std::string te::plugin::CppPluginEngine::getPluginFileName(const std::string& libName)
{
  std::string nativeName = te::core::Library::getNativeName(libName);

  return nativeName;
}

