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
#include <terralib/core/plugin/PluginEngineManager.h>
#include <terralib/core/plugin/PluginManager.h>

int main(int argc, char *argv[])
{
  // Load all the config files for the plugins that are in the default path.
  std::vector< te::core::PluginInfo > v_pinfo = te::core::DefaultPluginFinder();

  // Create unique_ptr for a AbstractPluginEngine. In this case is a CppPluginEngine
  std::unique_ptr < te::core::AbstractPluginEngine > cppengine(new te::core::CppPluginEngine());

  // Insert the CppPluginEngine to the PluginEngineManager
  te::core::PluginEngineManager::instance().insert(std::move(cppengine));

  // Insert all the plugins stored in the vector from a given PluginInfo.
  for(const te::core::PluginInfo& pinfo: v_pinfo)
  {
    te::core::PluginManager::instance().insert(pinfo);
  }

  // Get all the names for the available plugins
  std::vector< std::string > v_plugins = te::core::PluginManager::instance().getPlugins();

  // Load all the plugins
  for(const std::string& plugin: v_plugins)
  {
    te::core::PluginManager::instance().load(plugin);
  }

  // Unload all the plugins
  for(const std::string& plugin: v_plugins)
  {
    te::core::PluginManager::instance().unload(plugin);
  }

  return EXIT_SUCCESS;
}

