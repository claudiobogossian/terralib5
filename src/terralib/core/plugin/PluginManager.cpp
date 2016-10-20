/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

  This file is part of the TerraLib - a Framework for building GIS enabled
  applications.

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
  \file terralib/common/plugin/PluginManager.cpp

  \brief A singleton for managing plugins.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

// TerraLib
#include "PluginManager.h"
#include "../translator/Translator.h"
#include "AbstractPlugin.h"
#include "AbstractPluginEngine.h"
#include "Exception.h"
#include "PluginEngineManager.h"

// STL
#include <map>

// Boost
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>

struct te::core::PluginManager::Impl
{
  /*! \brief The list of managed plugins: this will be needed to unload
     accordinly the plugins! */
  std::vector<std::shared_ptr<AbstractPlugin> > plugins;
  /*! \brief The list of plugins that are not loaded. */
  std::vector<PluginInfo> unloaded_plugins;
  /*! \brief The list of plugins that could not be loaded. */
  std::vector<PluginInfo> broken_plugins;
  /*! \brief The list of plugins dependencies */
  std::map<std::string, std::vector<std::string> > dependency_map;

  void move_from_unload_to_broken_list(std::size_t plugin_pos);
};

std::vector<std::string> te::core::PluginManager::getPlugins() const
{
  std::vector<std::string> plugins;

  // retrieve the list of loaded plugins
  for(std::shared_ptr<AbstractPlugin> p : m_pimpl->plugins)
  {
    plugins.push_back(p->info().name);
  }

  // retrieve the list of unloaded plugins
  for(const PluginInfo& p : m_pimpl->unloaded_plugins)
  {
    plugins.push_back(p.name);
  }

  // retrieve the list of broken plugins
  for(const PluginInfo& p : m_pimpl->broken_plugins)
  {
    plugins.push_back(p.name);
  }

  return plugins;
}

const te::core::PluginInfo& te::core::PluginManager::getPluginInfo(
    const std::string& name) const
{
  // check in the possible list of loaded plugins first
  for(const std::shared_ptr<AbstractPlugin>& p : m_pimpl->plugins)
  {
    if(p->info().name == name)
      return p->info();
  }

  // check in the list of plugins not loaded yet
  for(const PluginInfo& p : m_pimpl->unloaded_plugins)
  {
    if(p.name == name)
      return p;
  }

  // check in the list of "problematic"(or broken) plugins
  for(const PluginInfo& p : m_pimpl->broken_plugins)
  {
    if(p.name == name)
      return p;
  }

  throw OutOfRangeException() << ErrorDescription(
      (boost::format("could not find plugin: '%1%'.") % name).str());
}

std::vector<te::core::PluginInfo> te::core::PluginManager::getLoadedPlugins()
    const
{
  std::vector<te::core::PluginInfo> plugins;

  // retrieve the list of loaded plugins
  for(std::shared_ptr<AbstractPlugin> p : m_pimpl->plugins)
    plugins.push_back(p->info());

  return plugins;
}

std::vector<te::core::PluginInfo> te::core::PluginManager::getUnloadedPlugins()
    const
{
  return m_pimpl->unloaded_plugins;
}

std::vector<te::core::PluginInfo> te::core::PluginManager::getBrokenPlugins()
    const
{
  return m_pimpl->broken_plugins;
}

bool te::core::PluginManager::isBroken(const std::string& plugin_name) const
{
  return std::find_if(m_pimpl->broken_plugins.begin(),
                      m_pimpl->broken_plugins.end(),
                      [&plugin_name](const PluginInfo& p) {
                        return p.name == plugin_name;
                      }) != m_pimpl->broken_plugins.end();
}

bool te::core::PluginManager::isUnloaded(const std::string& plugin_name) const
{
  return std::find_if(m_pimpl->unloaded_plugins.begin(),
                      m_pimpl->unloaded_plugins.end(),
                      [&plugin_name](const PluginInfo& p) {
                        return p.name == plugin_name;
                      }) != m_pimpl->unloaded_plugins.end();
}

bool te::core::PluginManager::isLoaded(const std::string& plugin_name) const
{
  return std::find_if(m_pimpl->plugins.begin(), m_pimpl->plugins.end(),
                      [&plugin_name](const std::shared_ptr<AbstractPlugin>& p) {
                        return p->info().name == plugin_name;
                      }) != m_pimpl->plugins.end();
}

bool te::core::PluginManager::exists(const std::string& plugin_name) const
{
  return isBroken(plugin_name) || isUnloaded(plugin_name) ||
         isLoaded(plugin_name);
}

void te::core::PluginManager::insert(const PluginInfo& pinfo)
{
  if(isLoaded(pinfo.name))
  {
    boost::format err_msg(
        TE_TR("There is already a plugin registered with name: '%1%', already "
              "loaded."));

    throw InvalidArgumentException()
        << ErrorDescription((err_msg % pinfo.name).str());
  }

  if(isUnloaded(pinfo.name))
  {
    boost::format err_msg(TE_TR(
        "There is already a plugin registered with name: '%1%', but not loaded "
        "yet."));

    throw InvalidArgumentException()
        << ErrorDescription((err_msg % pinfo.name).str());
  }

  if(isBroken(pinfo.name))
  {
    boost::format err_msg(
        TE_TR("There is already a plugin registered with the name: '%1%', but "
              "presenting some problems to load or startup."));

    throw InvalidArgumentException()
        << ErrorDescription((err_msg % pinfo.name).str());
  }

  m_pimpl->unloaded_plugins.push_back(pinfo);
}

void te::core::PluginManager::remove(const std::string& plugin_name)
{
  // if plugin is loaded we need to unload it (and maybe propagate changes to
  // other dependent plugins)
  if(isLoaded(plugin_name))
  {
    // stop the plugin
    stop(plugin_name);
    unload(plugin_name);
    // now plugin belongs to the unloaded list!
  }

  if(isUnloaded(plugin_name))
  {
    for(std::vector<PluginInfo>::iterator it =
            m_pimpl->unloaded_plugins.begin();
        it != m_pimpl->unloaded_plugins.end(); ++it)
    {
      if(plugin_name == it->name)
      {
        m_pimpl->unloaded_plugins.erase(it);
        return;
      }
    }
  }

  if(isBroken(plugin_name))
  {
    for(std::vector<PluginInfo>::iterator it = m_pimpl->broken_plugins.begin();
        it != m_pimpl->broken_plugins.end(); ++it)
    {
      if(plugin_name == it->name)
      {
        m_pimpl->broken_plugins.erase(it);
        return;
      }
    }
  }
  boost::format err_msg(TE_TR("Could not find plugin: '%1%'."));

  throw OutOfRangeException()
      << ErrorDescription((err_msg % plugin_name).str());
}

void te::core::PluginManager::load(const std::string& plugin_name,
                                   const bool start)
{
  // if plugin is already loaded raise an exception
  if(isLoaded(plugin_name))
  {
    boost::format err_msg(TE_TR("The plugin '%1%' is already loaded."));

    throw InvalidArgumentException()
        << ErrorDescription((err_msg % plugin_name).str());
  }

  // if plugin is not in the broken list nor in the unloaded list raise an
  // exception
  if(!isBroken(plugin_name) && !isUnloaded(plugin_name))
  {
    boost::format err_msg(
        TE_TR("The plugin '%1%' is not registered in the manager."));

    throw InvalidArgumentException()
        << ErrorDescription((err_msg % plugin_name).str());
  }

  // in which list and position is the plugin_info?
  bool found_in_unloaded_list = false;
  bool found_in_broken_list = false;

  std::size_t plugin_pos = 0;

  for(; plugin_pos != m_pimpl->unloaded_plugins.size(); ++plugin_pos)
  {
    if(m_pimpl->unloaded_plugins[plugin_pos].name == plugin_name)
    {
      found_in_unloaded_list = true;
      break;
    }
  }

  if(!found_in_unloaded_list)
  {
    for(plugin_pos = 0; plugin_pos != m_pimpl->broken_plugins.size();
        ++plugin_pos)
    {
      if(m_pimpl->broken_plugins[plugin_pos].name == plugin_name)
      {
        found_in_broken_list = true;
        break;
      }
    }

    if(!found_in_broken_list)
    {
      boost::format err_msg(TE_TR("Could not find plugin: '%1%'."));

      throw OutOfRangeException()
          << ErrorDescription((err_msg % plugin_name).str());
    }
  }

  // get plugin_info (make a temp copy to avoid loose references!)
  PluginInfo pinfo = found_in_unloaded_list
                         ? m_pimpl->unloaded_plugins[plugin_pos]
                         : m_pimpl->broken_plugins[plugin_pos];

  // check if required plugins is already loaded
  for(const std::string& plugin_dependency : pinfo.dependencies)
  {
    if(!isLoaded(plugin_dependency))
    {
      if(found_in_unloaded_list)
        m_pimpl->move_from_unload_to_broken_list(plugin_pos);

      boost::format err_msg(
          TE_TR("Plugin '%1%' has the following dependency: '%2%'."));

      throw PluginLoadException() << ErrorDescription(
          (err_msg % plugin_name % plugin_dependency).str());
    }
  }

  std::unique_ptr<AbstractPlugin> plugin(nullptr);

  try
  {
    // if everything is ready for loading the plugin let's call the underlying
    // engine to load the plugin
    AbstractPluginEngine& engine =
        PluginEngineManager::instance().get(pinfo.engine);

    plugin = engine.load(pinfo);

    if(plugin.get() == nullptr)
    {
      boost::format err_msg(TE_TR("Could not load plugin: '%1%'."));

      throw PluginLoadException()
          << ErrorDescription((err_msg % plugin_name).str());
    }

    if(start)
      plugin->startup();

    m_pimpl->dependency_map[pinfo.name] = {};

    for(const std::string& plugin_dependency : pinfo.dependencies)
      m_pimpl->dependency_map[plugin_dependency].push_back(pinfo.name);

    m_pimpl->plugins.push_back(
        std::shared_ptr<AbstractPlugin>(plugin.release()));

    if(found_in_unloaded_list)
    {
      m_pimpl->unloaded_plugins.erase(m_pimpl->unloaded_plugins.begin() +
                                      plugin_pos);
    }
    else if(found_in_broken_list)
    {
      m_pimpl->broken_plugins.erase(m_pimpl->broken_plugins.begin() +
                                    plugin_pos);
    }
    else
    {
      boost::format err_msg(
          TE_TR("Unexpected error after loading plugin: '%1%'."));
      throw PluginLoadException()
          << ErrorDescription((err_msg % plugin_name).str());
    }
  }
  catch(const boost::exception& e)
  {
    // WARNING: if plugin goes out-of scope the exception thorwn exception is
    // not valid this is why we make a copy here
    if(found_in_unloaded_list)
      m_pimpl->move_from_unload_to_broken_list(plugin_pos);

    if(const std::string* d = boost::get_error_info<ErrorDescription>(e))
    {
      throw PluginLoadException() << ErrorDescription(*d);
    }
    else
    {
      boost::format err_msg(TE_TR("Unknown error loading plugin: %1%."));
      throw PluginLoadException()
          << ErrorDescription((err_msg % pinfo.name).str());
    }
  }
  catch(...)
  {
    boost::format err_msg(TE_TR("Unknown error loading plugin: %1%."));
    throw PluginLoadException()
        << ErrorDescription((err_msg % pinfo.name).str());
  }
}

void te::core::PluginManager::start(const std::string& plugin_name)
{
  for(std::shared_ptr<AbstractPlugin> p : m_pimpl->plugins)
  {
    if(p->info().name == plugin_name)
    {
      p->startup();
      return;
    }
  }

  boost::format err_msg(
      TE_TR("The plugin '%1%' is not loaded, load it before starting it."));

  throw PluginStartupException()
      << ErrorDescription((err_msg % plugin_name).str());
}

void te::core::PluginManager::stop(const std::string& plugin_name)
{
  const auto it = m_pimpl->dependency_map.find(plugin_name);

  // checks if the plugin has a dependent
  if(it->second.size() == 0)
  {
    for(std::shared_ptr<AbstractPlugin> p : m_pimpl->plugins)
    {
      if(p->info().name == plugin_name)
      {
        m_pimpl->dependency_map.erase(it);
        p->shutdown();
        return;
      }
    }
    boost::format err_msg(TE_TR(
        "The plugin '%1%' is not loaded, load it before trying to stop it."));

    throw PluginShutdownException()
        << ErrorDescription((err_msg % plugin_name).str());
  }

  boost::format err_msg(
      TE_TR("The plugin '%1%' cannot be stopped, because is required by: %2%"));

  std::string dependents = boost::algorithm::join(it->second, ", ");

  throw PluginShutdownException()
      << ErrorDescription((err_msg % plugin_name % dependents).str());
}

void te::core::PluginManager::unload(const std::string& plugin_name)
{
  for(std::size_t plugin_pos = 0; plugin_pos != m_pimpl->plugins.size();
      ++plugin_pos)
  {
    if(m_pimpl->plugins[plugin_pos]->info().name == plugin_name)
    {
      if(!m_pimpl->plugins[plugin_pos]->initialized())
      {
        // insert the plugin into the unloaded list
        m_pimpl->unloaded_plugins.push_back(
            m_pimpl->plugins[plugin_pos]->info());

        auto dependencies = m_pimpl->plugins[plugin_pos]->info().dependencies;
        for(std::string dependency : dependencies)
        {
          // remove the plugin assossiation with its dependencies
          m_pimpl->dependency_map[dependency].erase(std::find(
              m_pimpl->dependency_map[dependency].begin(),
              m_pimpl->dependency_map[dependency].end(), plugin_name));
        }

        // remove the plugin from the loaded list
        m_pimpl->plugins.erase(m_pimpl->plugins.begin() + plugin_pos);

        return;
      }

      boost::format err_msg(TE_TR(
          "The plugin '%1%' is initialized, stop it before trying to unload "
          "it."));

      throw PluginUnloadException()
          << ErrorDescription((err_msg % plugin_name).str());
    }
  }
}

void te::core::PluginManager::clear()
{
  for(auto it = m_pimpl->plugins.rbegin(); it != m_pimpl->plugins.rend(); ++it)
    remove((*it)->info().name);

  m_pimpl->broken_plugins.clear();
  m_pimpl->dependency_map.clear();
  m_pimpl->unloaded_plugins.clear();
}

te::core::PluginManager& te::core::PluginManager::instance()
{
  static PluginManager inst;

  return inst;
}

te::core::PluginManager::PluginManager() : m_pimpl(nullptr)
{
  m_pimpl = new Impl;
}

te::core::PluginManager::~PluginManager()
{
  delete m_pimpl;
}

void te::core::PluginManager::Impl::move_from_unload_to_broken_list(
    std::size_t plugin_pos)
{
  assert(plugin_pos < unloaded_plugins.size());

  PluginInfo pinfo = unloaded_plugins[plugin_pos];

  unloaded_plugins.erase(unloaded_plugins.begin() + plugin_pos);

  broken_plugins.push_back(pinfo);
}
