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
#include <terralib/core/plugin.h>
#include <terralib/core/utils.h>

// STL
#include <cstdlib>

void InitPluginSystem()
{
// prepare PluginEngineManager with all known plugin engines.
  std::unique_ptr < te::core::AbstractPluginEngine > cppengine(new te::core::CppPluginEngine());
  
  te::core::PluginEngineManager::instance().insert(std::move(cppengine));
}

int main(int argc, char *argv[])
{
  InitPluginSystem();
  
// plugins will be loaded with C++ plugin engine
  te::core::AbstractPluginEngine& plugin_engine = te::core::PluginEngineManager::instance().get("C++");
  
// find fisrt plugin manifest file, read the manifest, load plugin and start it
  std::string p1_manifest_file = te::core::FindInTerraLibPath("share/terralib/plugins/plugin1.teplg.json");
  
  te::core::PluginInfo p1_info = te::core::JSONPluginInfoSerializer(p1_manifest_file);
  
  std::unique_ptr<te::core::AbstractPlugin> p1(plugin_engine.load(p1_info));
  
  p1->startup();
  
  
// find second plugin manifest file, read the manifest, load plugin and start it
  std::string p2_manifest_file = te::core::FindInTerraLibPath("share/terralib/plugins/plugin2.teplg.json");
  
  te::core::PluginInfo p2_info = te::core::JSONPluginInfoSerializer(p2_manifest_file);
  
  std::unique_ptr<te::core::AbstractPlugin> p2(plugin_engine.load(p2_info));
  
  p2->startup();
  
// stop all plugins and unload them
  p2->shutdown();
  plugin_engine.unload(std::move(p2));
  
  p1->shutdown();
  plugin_engine.unload(std::move(p1));

  return EXIT_SUCCESS;
}

