// Examples
#include "CanvasExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadModules()
{
  te::plugin::PluginInfo* info;

  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.pgis.teplg"));
  te::plugin::PluginManager::getInstance().add(info); 
 
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.gdal.teplg"));
  te::plugin::PluginManager::getInstance().add(info);

  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.ogr.teplg"));
  te::plugin::PluginManager::getInstance().add(info); 

  te::plugin::PluginManager::getInstance().loadAll(); 
}