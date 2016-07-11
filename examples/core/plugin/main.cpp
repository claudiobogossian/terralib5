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
  \file terralib/examples/core/plugin/main.cpp

  \brief Examples for the Terralib Plugin Module

  \author Matheus Cavassan Zaglia
*/

// TerraLib
#include <terralib/core/plugin/DefaultFinders.h>
#include <terralib/core/plugin/CppPluginEngine.h>
#include <terralib/core/plugin/CppPlugin.h>

int main(int argc, char *argv[])
{
  // Load all the config files for the plugins that are in the default path.
  std::vector< te::core::PluginInfo > v_pinfo = te::core::DefaultPluginFinder();
  // Create a vector to store the loaded plugins
  std::vector< std::unique_ptr<te::core::AbstractPlugin> >  v_cpp_plugins;

  // CppPluginEngine to manage cpp plugins
  te::core::CppPluginEngine cpp_engine;

  for(const te::core::PluginInfo& pinfo: v_pinfo)
  {
    //Load all the plugins stored in the vector from a given PluginInfo.
    v_cpp_plugins.push_back(cpp_engine.load(pinfo));
  }

  for(const std::unique_ptr<te::core::AbstractPlugin>& plugin: v_cpp_plugins)
  {
    //Start all loaded plugins.
    plugin->startup();
  }

  for(const std::unique_ptr<te::core::AbstractPlugin>& plugin: v_cpp_plugins)
  {
    //Shutdown all loaded plugins.
    plugin->shutdown();
  }

  for(std::unique_ptr<te::core::AbstractPlugin>& plugin: v_cpp_plugins)
  {
    //Unload all plugins.
    cpp_engine.unload(std::move(plugin));
  }

  return EXIT_SUCCESS;
}

