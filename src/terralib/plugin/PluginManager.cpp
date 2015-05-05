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
  \file terralib/plugin/PluginManager.cpp

  \brief A singleton for managing plugins.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "AbstractFinder.h"
#include "AbstractPlugin.h"
#include "AbstractPluginEngine.h"
#include "DefaultFinder.h"
#include "PluginEngineFactory.h"
#include "PluginInfo.h"
#include "PluginManager.h"

// STL
#include <algorithm>
#include <cassert>
#include <memory>

// Boost
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/exception.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

std::vector<std::string>
te::plugin::PluginManager::getPlugins() const
{
  std::vector<std::string> plugins;
  
// retrieve the list of loaded plugins
  for(std::vector<AbstractPlugin*>::const_iterator it = m_plugins.begin(); it != m_plugins.end(); ++it)
    plugins.push_back((*it)->getInfo().m_name);

// retrieve the list of unloaded plugins
  for(boost::ptr_vector<PluginInfo>::const_iterator it = m_unloadedPlugins.begin(); it != m_unloadedPlugins.end(); ++it)
    plugins.push_back(it->m_name);

// retrieve the list of broken plugins
  for(boost::ptr_vector<PluginInfo>::const_iterator it = m_brokenPlugins.begin(); it != m_brokenPlugins.end(); ++it)
    plugins.push_back(it->m_name);
  
  return plugins;
}

const te::plugin::PluginInfo& te::plugin::PluginManager::getPlugin(const std::string& name) const
{
// check in the loaded list first
  {
    std::map<std::string, AbstractPlugin*>::const_iterator it = m_pluginsMap.find(name);

    if(it != m_pluginsMap.end())
      return it->second->getInfo();
  }

// check in not-loaded list
  for(boost::ptr_vector<PluginInfo>::const_iterator it = m_unloadedPlugins.begin(); it != m_unloadedPlugins.end(); ++it)
    if(it->m_name == name)
      return *it;

// check in the broken list
  for(boost::ptr_vector<PluginInfo>::const_iterator it = m_brokenPlugins.begin(); it != m_brokenPlugins.end(); ++it)
    if(it->m_name == name)
      return *it;

  throw Exception((boost::format("Could not find plugin %1%") % name).str());
}

const boost::ptr_vector<te::plugin::PluginInfo>& te::plugin::PluginManager::getUnloadedPlugins() const
{
  return m_unloadedPlugins;
}

void te::plugin::PluginManager::setUnloadedPlugins(const boost::ptr_vector<te::plugin::PluginInfo> unloadedPlugins)
{
  m_unloadedPlugins = unloadedPlugins;
}

const boost::ptr_vector<te::plugin::PluginInfo>& te::plugin::PluginManager::getBrokenPlugins() const
{
  return m_brokenPlugins;
}

void te::plugin::PluginManager::setBrokenPlugins(const boost::ptr_vector<te::plugin::PluginInfo> brokenPlugins)
{
  m_brokenPlugins = brokenPlugins;
}

bool te::plugin::PluginManager::isBrokenPlugin(const std::string& pluginName) const
{
  for(boost::ptr_vector<PluginInfo>::const_iterator it = m_brokenPlugins.begin(); it != m_brokenPlugins.end(); ++it)
    if(it->m_name == pluginName)
      return true;

  return false;
}

bool te::plugin::PluginManager::isUnloadedPlugin(const std::string& pluginName) const
{
  for(boost::ptr_vector<PluginInfo>::const_iterator it = m_unloadedPlugins.begin(); it != m_unloadedPlugins.end(); ++it)
    if(it->m_name == pluginName)
      return true;

  return false;
}

bool te::plugin::PluginManager::isLoaded(const std::string& pname) const
{
  return m_pluginsMap.find(pname) != m_pluginsMap.end();
}

void te::plugin::PluginManager::loadAll(const bool start)
{
// unload all plugins before loading all again!
  unloadAll();

  boost::ptr_vector<PluginInfo> plugins;

// have we already load plugins?
  if(m_unloadedPlugins.empty())
  {
// if yes, then we can find and load new plugins
    if(m_finders.empty())
    {
      DefaultFinder f;
      f.getPlugins(plugins);
    }
    else
    {
      const std::size_t nfinders = m_finders.size();

      for(std::size_t i = 0; i < nfinders; ++i)
        m_finders[i]->getPlugins(plugins);
    }
  }
  else
  {
// use last unloaded list
    plugins = m_unloadedPlugins;
  }

  load(plugins, start);
}

void te::plugin::PluginManager::unloadAll()
{
// start by the last loaded plugin
  std::vector<AbstractPlugin*>::reverse_iterator it = m_plugins.rbegin();

  while(it != m_plugins.rend())
  {
    std::auto_ptr<PluginInfo> pinfo(new PluginInfo((*it)->getInfo()));
    unload(*it);
    m_unloadedPlugins.push_back(pinfo.release());
    it = m_plugins.rbegin();  // always get last loaded plugin
  }

// some assertions
  assert(m_pluginCategoryMap.empty());
  assert(m_plugins.empty());
  assert(m_pluginsMap.empty());
}

void te::plugin::PluginManager::clear()
{
  unloadAll();
  m_unloadedPlugins.clear();
  m_brokenPlugins.clear();
}

void te::plugin::PluginManager::load(boost::ptr_vector<PluginInfo>& plugins, const bool start)
{
  sort(plugins);

  bool hasException = false;

  const std::size_t nplugins = plugins.size();

  std::string exceptionPlugins;

  for(std::size_t i = 0; i < nplugins; ++i)
  {
    const PluginInfo& pInfo = plugins[i];

    try
    {
      load(pInfo, start);
    }
    catch(...)
    {
      hasException = true;
      exceptionPlugins += "\n" + pInfo.m_name;
    }
  }

  if(hasException || !m_brokenPlugins.empty())
    throw Exception(TE_TR("\n\nPlugins not loaded:" ) + exceptionPlugins);
}

void te::plugin::PluginManager::load(const PluginInfo& pInfo, const bool start)
{
  if(isLoaded(pInfo.m_name))
    throw Exception((boost::format("Plugin %1% is already loaded") % pInfo.m_name).str());

// check if required plugins is already loaded
  if(!isLoaded(pInfo.m_requiredPlugins))
  {
    moveToBrokenList(pInfo);
    throw Exception(TE_TR("A required plugin is not loaded!"));
  }

  std::auto_ptr<AbstractPlugin> plugin;

/////////////////////////////
// how many modules were loaded before the plugin?
  std::size_t nmodules = TerraLib::getInstance().getNumRegModules();
/////////////////////////////

  try
  {
// if everything is ready for loading the plugin let's call its engine
    std::auto_ptr<AbstractPluginEngine> engine(PluginEngineFactory::make(pInfo.m_engine));

    if(engine.get() == 0)
      throw Exception((boost::format("Could not determine plugin's language type for %1%!") % pInfo.m_name).str());

    plugin.reset(engine->load(pInfo));

    if(plugin.get() == 0)
      throw Exception((boost::format("Could not load plugin %1%!") % pInfo.m_name).str());
  }
  catch(const std::exception& e)
  {
    moveToBrokenList(pInfo);
    Exception ee(e.what());
    throw ee;
  }
  catch(...)
  {
    moveToBrokenList(pInfo);
    throw;
  }

  try
  {
/////////////////////////////
// if after loading the plugin there are more modules than before we need to initilize the new loaded modules
    if(TerraLib::getInstance().getNumRegModules() > nmodules)
    {
      std::size_t nnmodules = TerraLib::getInstance().getNumRegModules();

      for(std::size_t i = nmodules; i < nnmodules; ++i)
        TerraLib::getInstance().initialize(i);
    }
/////////////////////////////

    if(start)
      plugin->startup();

    std::string plg_name = plugin->getInfo().m_name;
// register plugin in the map and add it to its category
    m_pluginCategoryMap[pInfo.m_category].push_back(plugin.get());
    m_plugins.push_back(plugin.get());
    m_pluginsMap[plg_name] = plugin.release();

// remove plugin from broken or unloaded list
    removeFromBrokenList(pInfo);
    removeFromUnloadedList(pInfo);

    updateDependents(plg_name);
  }
  catch(const std::exception& e)
  {
    moveToBrokenList(pInfo);
    Exception ee(e.what());
    throw ee;
  }
  catch(...)
  {
    moveToBrokenList(pInfo);
    throw;  // sorry, but maybe this will cause the code to crash due to the unload of plugin module!
  }
}

void te::plugin::PluginManager::load(const std::string& pluginName)
{
  load(getPlugin(pluginName));
}

void te::plugin::PluginManager::unload(const std::string& name)
{
  std::map<std::string, AbstractPlugin*>::iterator it = m_pluginsMap.find(name);

  if(it == m_pluginsMap.end())
    throw Exception((boost::format("Plugin %1% is not loaded!") % name).str());

  AbstractPlugin* plugin = it->second;

  if(plugin == 0)
    throw Exception((boost::format("Plugin %1% is NULL!") % name).str());

  unload(plugin);
}

void te::plugin::PluginManager::unload(AbstractPlugin* plugin)
{
  const std::string pluginName = plugin->getInfo().m_name;
  const std::string pluginCategory = plugin->getInfo().m_category;

// check dependency
  if(hasDependents(pluginName))
    moveDependentsToBrokenList(pluginName);
//    throw Exception((boost::format("Could not unload plugin %1% because other plugins depends on it!") % pluginName).str());

// shutdown plugin!
  if(plugin->isStarted())
    plugin->shutdown();

// locate plugin engine and unload it!
  std::auto_ptr<AbstractPluginEngine> engine(PluginEngineFactory::make(plugin->getInfo().m_engine));

  if(engine.get() == 0)
    throw Exception((boost::format("Could not determine plugin %1% language type!") % pluginName).str());

  std::auto_ptr<PluginInfo> pInfo(new PluginInfo(plugin->getInfo()));

  engine->unload(plugin); // the plugin pointer is already invalidated!

  m_unloadedPlugins.push_back(pInfo);
  
// remove plugin from manager if it is managed by it
  removeFromCategory(plugin, pluginCategory);

  {
    std::vector<AbstractPlugin*>::iterator it = std::find(m_plugins.begin(), m_plugins.end(), plugin);

    if(it != m_plugins.end())
      m_plugins.erase(it);
  }

  {
    std::map<std::string, AbstractPlugin*>::iterator it = m_pluginsMap.find(pluginName);

    if(it != m_pluginsMap.end())
      m_pluginsMap.erase(it);
  }
}

void te::plugin::PluginManager::shutdownAll()
{
// start by the last loaded plugin
  std::vector<AbstractPlugin*>::reverse_iterator it = m_plugins.rbegin();

  while(it != m_plugins.rend())
  {
    if((*it)->isStarted())
      (*it)->shutdown();

    ++it;
  }
}

std::size_t te::plugin::PluginManager::getNumPlugins() const
{
  return m_pluginsMap.size() + m_unloadedPlugins.size() + m_brokenPlugins.size();
}

bool te::plugin::PluginManager::isLoaded(const std::vector<std::string>& plugins) const
{
  const std::size_t size = plugins.size();

  for(std::size_t i = 0; i < size; ++i)
    if(!isLoaded(plugins[i]))
      return false; // there is a missing plugin!

  return true;
}

void te::plugin::PluginManager::add(const PluginInfo& plugin)
{
  add(new PluginInfo(plugin));
}

void te::plugin::PluginManager::add(PluginInfo* plugin)
{
  //try
  //{
  //  if(!isLoaded(plugin->m_requiredPlugins))
  //    throw Exception("Missing requirement");

    m_unloadedPlugins.push_back(plugin);
  //}
  //catch(Exception&)
  //{
  //  m_brokenPlugins.push_back(plugin);
  //}
}

void te::plugin::PluginManager::remove(const std::string& plugin)
{
  PluginInfo info = getPlugin(plugin);

  if(isLoaded(plugin))
  {
    AbstractPlugin* plg = detach(plugin);
    plg->shutdown();
    delete plg;

    return;
  }

  if(isUnloadedPlugin(plugin))
  {
    removeFromUnloadedList(info);
    return;
  }

  removeFromBrokenList(info);
}

std::vector<std::string> te::plugin::PluginManager::getDependents(const std::string& pluginName) const
{
  std::vector<std::string> dependents;

  const std::size_t nplugins = m_plugins.size();

  for(std::size_t i = 0; i < nplugins; ++i)
  {
    if(pluginName == m_plugins[i]->getInfo().m_name)
      continue;

    const std::vector<std::string>& requiredPlugins = m_plugins[i]->getInfo().m_requiredPlugins;

    const std::size_t size = requiredPlugins.size();

    for(std::size_t j = 0; j < size; ++j)
    {
// is the checked plugin a required plugin?
      if(pluginName == requiredPlugins[j])
      {
        dependents.push_back(m_plugins[i]->getInfo().m_name);  // it->first contains the name of the plugin that requires pluginName!
        break;
      }
    }
  }

  return dependents;
}

bool te::plugin::PluginManager::hasDependents(const std::string& pluginName) const
{
  return getDependents(pluginName).empty() ? false : true;
}

te::plugin::AbstractPlugin* te::plugin::PluginManager::detach(const std::string& name)
{
// find plugin
  std::map<std::string, AbstractPlugin*>::iterator it = m_pluginsMap.find(name);

  if(it == m_pluginsMap.end())
    throw Exception((boost::format(TE_TR("Could not find the given plugin (%1%) in order to detach it from PluginManager!")) % name).str());

  AbstractPlugin* p = it->second;

  if(p == 0)
    throw Exception((boost::format(TE_TR("Could not detach a NULL plugin (%1%) from PluginManager!")) % name).str());

// check if it doesn't have dependents plugins
  if(hasDependents(name))
    moveDependentsToBrokenList(name);
//    throw Exception((boost::format(TE_TR("There are some plugins that depends on %1%!")) % name).str());

// see if we must destroy the plugin category index: in the case the detached plugin being the only plugin in its category
  removeFromCategory(p, p->getInfo().m_category);

// remove plugin from manager
  std::vector<AbstractPlugin*>::iterator itv = std::find(m_plugins.begin(), m_plugins.end(), p);

  if(itv == m_plugins.end())
    throw Exception(TE_TR("PluginManager has lost the synchronization between its internal indexes!"));

  m_plugins.erase(itv);

  m_pluginsMap.erase(name);

// finally... just return the plugin pointer and loose its ownership
  return p;
}

void te::plugin::PluginManager::getCategories(std::vector<std::string>& categories) const
{
  std::map<std::string, std::vector<AbstractPlugin*> >::const_iterator it = m_pluginCategoryMap.begin();

  while(it != m_pluginCategoryMap.end())
  {
    categories.push_back(it->first);

    ++it;
  }
}

void te::plugin::PluginManager::addCategory(const std::string& name)
{
  std::map<std::string, std::vector<AbstractPlugin*> >::iterator it = m_pluginCategoryMap.find(name);

  if(it == m_pluginCategoryMap.end())
  {
    std::vector<AbstractPlugin*> vec;
    m_pluginCategoryMap.insert(std::map<std::string, std::vector<AbstractPlugin*> >::value_type(name, vec));
  }
}

void te::plugin::PluginManager::sort(boost::ptr_vector<PluginInfo>& plugins) const
{
// let's create an auxiliary index to make a topological sort on the plugin list
  std::map<std::string, std::size_t> newPluginsMap;

  const std::size_t nplugins = plugins.size();

  for(std::size_t i = 0; i < nplugins; ++i)
    newPluginsMap.insert(std::make_pair(plugins[i].m_name, i));

// let's cache the iterator to the end of the map
  std::map<std::string, std::size_t>::const_iterator itend = newPluginsMap.end();

// let's make a graph with the plugins dependency
  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;
  typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

  Graph dgraph;  // The dependency graph must be a DAG - when we make a topological sort this will be assured.

  for(std::size_t i = 0; i < nplugins; ++i) // add all vertex, maybe there aren't dependencies!
    boost::add_vertex(dgraph);

  for(std::size_t i = 0; i < nplugins; ++i)
  {
    const std::vector<std::string>& requiredPlugins = plugins[i].m_requiredPlugins;

    const std::size_t nrequiredplugins = requiredPlugins.size();

    for(std::size_t j = 0; j < nrequiredplugins; ++j)
    {
      std::map<std::string, std::size_t>::const_iterator it = newPluginsMap.find(requiredPlugins[j]);

      if(it != itend)
      {
// if plugin A (it->second) is required for plugin B (i) then there must be a directed edge from A to B (A must come first then B!)
        boost::add_edge(it->second, i, dgraph);
      }
    }
  }

// now let's make a topological sort in order to be possible to traverse an load the plugins in the right order
  std::vector<Vertex> toposortResult;

  boost::topological_sort(dgraph, std::back_inserter(toposortResult));

// now let's load the plugins traversing the output result from topological sort in reverse-order (!!!topo sort output is not cool!!!)
  boost::ptr_vector<PluginInfo> pluginsTemp;

  for(std::vector<Vertex>::reverse_iterator it = toposortResult.rbegin(); it != toposortResult.rend(); ++it)
    pluginsTemp.push_back(new PluginInfo(plugins[*it]));

  plugins = pluginsTemp;
}

void te::plugin::PluginManager::removeFromCategory(AbstractPlugin* plugin, const std::string& category)
{
  std::map<std::string, std::vector<AbstractPlugin*> >::iterator it = m_pluginCategoryMap.find(category);

  if(it == m_pluginCategoryMap.end())
    return;

  std::vector<AbstractPlugin*>& plugins = it->second;

  const std::size_t size = plugins.size();

  for(std::size_t i = 0; i < size; ++i)
  {
    if(plugin == plugins[i])
    {
      plugins.erase(plugins.begin() + i);

      if(plugins.empty())
        m_pluginCategoryMap.erase(it);

      return;
    }
  }
}

void te::plugin::PluginManager::moveToBrokenList(const PluginInfo& pInfo)
{
  removeFromUnloadedList(pInfo);
  removeFromBrokenList(pInfo);

// add it to the end of the broken list
  m_brokenPlugins.push_back(new PluginInfo(pInfo));
}

void te::plugin::PluginManager::removeFromBrokenList(const PluginInfo& pInfo)
{
  for(boost::ptr_vector<PluginInfo>::iterator it = m_brokenPlugins.begin(); it != m_brokenPlugins.end(); ++it)
  {
    if(pInfo.m_name == it->m_name)
    {
      m_brokenPlugins.erase(it);
      break;
    }
  }
}

void te::plugin::PluginManager::removeFromUnloadedList(const PluginInfo& pInfo)
{
  for(boost::ptr_vector<PluginInfo>::iterator it = m_unloadedPlugins.begin(); it != m_unloadedPlugins.end(); ++it)
  {
    if(pInfo.m_name == it->m_name)
    {
      m_unloadedPlugins.erase(it);
      break;
    }
  }
}

void te::plugin::PluginManager::moveDependentsToBrokenList(const std::string& plugin, const bool& unloadPlugin)
{
  if(isBrokenPlugin(plugin))
    return;

  std::vector<std::string> deps = getDependents(plugin);

  if(!deps.empty())
  {
    std::vector<std::string>::iterator it;

    for(it=deps.begin(); it!=deps.end(); ++it)
      moveDependentsToBrokenList(*it, true);
  }

  te::plugin::PluginInfo info(getPlugin(plugin));

  if(unloadPlugin)
  {
    if(isLoaded(info.m_name))
      unload(info.m_name);

    moveToBrokenList(info);
  }
}

void te::plugin::PluginManager::updateDependents(const std::string& plugin)
{
  boost::ptr_vector<PluginInfo> deps = getBrokenPlugins();

  if(!deps.empty())
  {
    boost::ptr_vector<PluginInfo>::iterator it;
    std::vector<PluginInfo*> toUpdate;

    for(it=deps.begin(); it!=deps.end(); ++it)
      if(isLoaded((*it).m_requiredPlugins))
        toUpdate.push_back(new PluginInfo(*it));

    std::vector<PluginInfo*>::iterator it2;
    for(it2=toUpdate.begin(); it2 != toUpdate.end(); ++it2)
    {
      removeFromBrokenList(*(*it2));
      m_unloadedPlugins.push_back(*it2);
    }
  }
}

te::plugin::PluginManager::PluginManager()
{
}

te::plugin::PluginManager::~PluginManager()
{
  te::common::FreeContents(m_finders);
  te::common::FreeContents(m_plugins);
}

