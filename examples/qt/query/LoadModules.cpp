#include "LoadModules.h"

#include "Config.h"

#include <terralib/common/PlatformUtils.h>
#include <terralib/common.h>
#include <terralib/plugin.h>

#include <iostream>

void LoadModules()
{
  te::plugin::PluginInfo* info;
  
  std::string plugins_path = te::common::FindInTerraLibPath("share/terralib/plugins");
  
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.ogr.teplg");
  te::plugin::PluginManager::getInstance().add(info); 
  
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.gdal.teplg");
  te::plugin::PluginManager::getInstance().add(info);
  
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.pgis.teplg");
  te::plugin::PluginManager::getInstance().add(info);
  
  //info = te::plugin::GetInstalledPlugin(plugins_path + "/plugin_grib_info.xml");
  //te::plugin::PluginManager::getInstance().add(info);
  
  te::plugin::PluginManager::getInstance().loadAll();
}
