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
  \file terralib/core/plugin/Utils.cpp

  \brief General utilities for plugins.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

// TerraLib
#include "Utils.h"
#include "../translator/Translator.h"
#include "CppPluginEngine.h"
#include "Exception.h"
#include "Finders.h"
#include "PluginEngineManager.h"
#include "PluginManager.h"

// Boost
#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

static bool g_plugin_module_initialized(false);

void te::core::plugin::InitializePluginSystem()
{
  if(g_plugin_module_initialized)
    return;

  std::unique_ptr<te::core::AbstractPluginEngine> cppengine(
      new te::core::CppPluginEngine());

  te::core::PluginEngineManager::instance().insert(std::move(cppengine));

  g_plugin_module_initialized = true;
}

void te::core::plugin::FinalizePluginSystem()
{
  if(!g_plugin_module_initialized)
    return;

  te::core::PluginEngineManager::instance().clear();

  g_plugin_module_initialized = false;
}

std::vector<te::core::PluginInfo> te::core::plugin::TopologicalSort(
    const std::vector<te::core::PluginInfo>& v_pInfo)
{
  std::map<std::string, std::size_t> plugins_map;

  const std::size_t nplugins = v_pInfo.size();

  for(std::size_t i = 0; i != nplugins; ++i)
    plugins_map.insert(std::make_pair(v_pInfo[i].name, i));

  std::map<std::string, std::size_t>::const_iterator it_end = plugins_map.end();

  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS>
      Graph;
  typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

  Graph dgraph;  // The dependency graph must be a DAG

  for(std::size_t i = 0; i < nplugins; ++i)
    boost::add_vertex(dgraph);

  for(std::size_t i = 0; i != nplugins; ++i)
  {
    const std::vector<std::string>& required_plugins = v_pInfo[i].dependencies;

    const std::size_t num_required_plugins = required_plugins.size();

    for(std::size_t j = 0; j != num_required_plugins; ++j)
    {
      std::map<std::string, std::size_t>::const_iterator it =
          plugins_map.find(required_plugins[j]);

      if(it != it_end)
      {
        // if plugin A (it->second) is required for plugin B (i) then there must
        // be a directed edge from A to B (A must come first then B!)
        boost::add_edge(it->second, i, dgraph);
      }
    }
  }

  std::vector<Vertex> toposortResult;

  try
  {
    boost::topological_sort(dgraph, std::back_inserter(toposortResult));
  }
  catch(const boost::not_a_dag& e)
  {
    boost::format err_msg(
        TE_TR("The plugins cannot be sorted due to the following error: %1%"));

    throw PluginCyclicDependencyException()
        << ErrorDescription((err_msg % e.what()).str());
  }

  std::vector<te::core::PluginInfo> sortedPlugins;

  for(auto it = toposortResult.rbegin(); it != toposortResult.rend(); ++it)
    sortedPlugins.push_back(v_pInfo[*it]);

  assert(sortedPlugins.size() == v_pInfo.size());

  return sortedPlugins;
}

void te::core::plugin::LoadAll(bool start)
{
  te::core::PluginManager::instance().clear();

  std::vector<te::core::PluginInfo> v_pInfo = te::core::DefaultPluginFinder();

  v_pInfo = te::core::plugin::TopologicalSort(v_pInfo);

  std::vector<std::string> failToLoad;

  for(const te::core::PluginInfo& pinfo : v_pInfo)
  {
    try
    {
      te::core::PluginManager::instance().insert(pinfo);
      te::core::PluginManager::instance().load(pinfo.name, start);
    }
    catch(...)
    {
      failToLoad.push_back(pinfo.name);
    }
  }
  if(failToLoad.size() > 0)
  {
    boost::format err_msg(
        TE_TR("Could not load the following plugins:\n\n%1%"));

    throw PluginLoadException() << ErrorDescription(
        (err_msg % boost::algorithm::join(failToLoad, "\n")).str());
  }
}

void te::core::plugin::UnloadAll()
{
  std::vector<te::core::PluginInfo> pVec =
      te::core::PluginManager::instance().getLoadedPlugins();

  for(auto plugin = pVec.rbegin(); plugin != pVec.rend(); ++plugin)
  {
    te::core::PluginManager::instance().stop(plugin->name);
    te::core::PluginManager::instance().unload(plugin->name);
  }
}

void te::core::plugin::UnloadPlugin(const std::string& plugin_name)
{
  const te::core::PluginInfo& pInfo =
      te::core::PluginManager::instance().getPluginInfo(plugin_name);

  for(auto dependent : pInfo.dependencies)
    UnloadPlugin(dependent);

  if(te::core::PluginManager::instance().isUnloaded(plugin_name))
    return;

  te::core::PluginManager::instance().stop(plugin_name);
  te::core::PluginManager::instance().unload(plugin_name);
}
