#include "LoadModules.h"

#include "Config.h"

#include <terralib/common/PlatformUtils.h>
#include <terralib/common.h>
#include <terralib/plugin.h>

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

#ifdef TERRALIB_MOD_GRIB_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/plugin_grib_info.xml");
  te::plugin::PluginManager::getInstance().add(info); 
#endif

  te::plugin::PluginManager::getInstance().loadAll();   
}
