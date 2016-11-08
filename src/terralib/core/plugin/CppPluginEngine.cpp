/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/core/plugin/CppPluginEngine.cpp

  \brief A plugin engine for plugins written in C++.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

// TerraLib
#include "CppPluginEngine.h"
#include "../../BuildConfig.h"
#include "../lib/Library.h"
#include "../translator/Translator.h"
#include "CppPlugin.h"
#include "CppPluginProxy.h"
#include "Exception.h"

// STL
#include <algorithm>
#include <cassert>

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

struct te::core::CppPluginEngine::Impl
{
  std::string engine_id;
  std::string engine_name;
};

te::core::CppPluginEngine::CppPluginEngine()
  : m_pimpl(nullptr)
{
  m_pimpl = new Impl;
  m_pimpl->engine_id = "C++";
  m_pimpl->engine_name = "Engine for plugins written in C++";
}

te::core::CppPluginEngine::~CppPluginEngine()
{
  delete m_pimpl;
}

const std::string&
te::core::CppPluginEngine::id() const
{
  return m_pimpl->engine_id;
}

const std::string&
te::core::CppPluginEngine::name() const
{
  return m_pimpl->engine_name;
}

std::unique_ptr<te::core::AbstractPlugin>
te::core::CppPluginEngine::load(const PluginInfo& pinfo)
{
// get the plugin's shared library name in the resources list
  std::vector<Resource>::const_iterator it = std::find_if(pinfo.resources.begin(),
                                                          pinfo.resources.end(),
                                                          []
                                                          (const Resource& r)
                                                          {
                                                            return r.first == "shared_library_name";
                                                          });

  if(it == pinfo.resources.end())
  {
    boost::format err_msg(TE_TR("Shared library name not informed for plugin: %1%."));

    throw PluginEngineLoadException() << ErrorDescription((err_msg % pinfo.name).str());
  }

// get the adequate name depending on the platform and build type
  std::string slib_name = Library::getNativeName(it->second);

  boost::filesystem::path plugin_file(slib_name);

  std::shared_ptr<Library> slib(new Library(plugin_file.string(), true));

  std::string err_msg;
  
  try
  {
    slib->load();
  }
  catch(const te::core::Exception& e)
  {
    err_msg += plugin_file.string();
  }
  
#if (TE_PLATFORM == TE_PLATFORMCODE_APPLE) && defined(TERRALIB_BUILD_AS_BUNDLE)
  if(!slib->isLoaded())
  {
    
    try
    {
      plugin_file = "./";
      plugin_file /= slib_name;
      slib.reset(new te::core::Library(plugin_file.string(), true));
      slib->load();
      err_msg.clear();
    }
    catch(te::core::Exception& e)
    {
      err_msg += '\n';
      err_msg += plugin_file.string();
    }
    
    if(!slib->isLoaded())
    {
      try
      {
        plugin_file = "./";
        plugin_file /= "lib";
        plugin_file /= slib_name;
        slib.reset(new te::core::Library(plugin_file.string(), true));
        slib->load();
        err_msg.clear();
      }
      catch(te::core::Exception& exc)
      {
        err_msg += '\n';
        err_msg += plugin_file.string();
      }
    }
  }
#endif
  if(!slib->isLoaded())
  {
    boost::format err(TE_TR("Could not find shared library as:\n%1%"));
    
    throw PluginLoadException() << ErrorDescription((err % err_msg).str());
  }


// now we need to get the plugin constructor function address
  void* fptr = slib->getAddress("te_cpp_plugin_get_instance");

  te_get_plugin_fnct_t plugin_entry = reinterpret_cast<te_get_plugin_fnct_t>(fptr);

  if(plugin_entry == nullptr)
  {
    boost::format err_msg(TE_TR("Could not find entry point '%1%' for plugin '%2%'."));

    throw PluginEngineLoadException() << ErrorDescription((err_msg % "te_cpp_plugin_get_instance" % pinfo.name).str());
  }

  std::unique_ptr<CppPlugin> cpp_plugin(plugin_entry(pinfo));

  if(cpp_plugin == nullptr)
  {
    boost::format err_msg(TE_TR("Plugin '%1%' returned a null entry."));

    throw PluginEngineLoadException() << ErrorDescription((err_msg % pinfo.name).str());
  }

  std::unique_ptr<CppPluginProxy> proxy(new CppPluginProxy(slib, std::move(cpp_plugin)));

  return std::move(proxy);
}

void
te::core::CppPluginEngine::unload(std::unique_ptr<AbstractPlugin> plugin)
{
  if(plugin == nullptr)
    return;

  if(plugin->info().engine != "C++")
  {
    boost::format err_msg(TE_TR("Plugin '%1%' must be unloaded with plugin engine '%2%' not by C++ plugin engine."));

    throw PluginEngineUnloadException() << ErrorDescription((err_msg % plugin->info().name % plugin->info().engine).str());
  }

  plugin.reset();
}

