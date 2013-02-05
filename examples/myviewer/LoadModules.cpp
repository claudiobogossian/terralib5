// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <iostream>


void LoadModules()
{
  te::plugin::PluginInfo* info;
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/plugin_ogr_info.xml"));
  te::plugin::PluginManager::getInstance().add(info);
  
  te::plugin::PluginManager::getInstance().loadAll();   
}
