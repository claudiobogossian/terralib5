// Example
#include "ADOExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadModules()
{
  te::plugin::PluginInfo* info;

  info = te::plugin::GetInstalledPlugin(PLUGINS_PATH + std::string("/plugin_ado_info.xml"));
  te::plugin::PluginManager::getInstance().add(info);
  
  te::plugin::PluginManager::getInstance().loadAll(); 
}
