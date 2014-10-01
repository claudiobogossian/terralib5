// Examples
#include "MapToolsExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadModules()
{
  std::string plugins_path = te::common::FindInTerraLibPath("share/terralib/plugins");

  te::plugin::PluginInfo* info;
  
#ifdef TERRALIB_MOD_OGR_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.ogr.teplg");
  te::plugin::PluginManager::getInstance().add(info); 
#endif
  
#ifdef TERRALIB_MOD_GDAL_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.gdal.teplg");
  te::plugin::PluginManager::getInstance().add(info);
#endif
  
#ifdef TERRALIB_MOD_POSTGIS_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.pgis.teplg");
  te::plugin::PluginManager::getInstance().add(info);    
#endif
  
  te::plugin::PluginManager::getInstance().loadAll();  
}
