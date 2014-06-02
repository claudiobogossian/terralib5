// TerraLib
#include <terralib/common/PlatformUtils.h>
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadModules()
{
  te::plugin::PluginInfo* info;
  
  std::string plugins_path = te::common::FindInTerraLibPath("share/terralib/plugins");

#ifdef TERRALIB_MOD_POSTGIS_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.pgis.teplg");
  te::plugin::PluginManager::getInstance().add(info);
#endif

#ifdef TERRALIB_MOD_GDAL_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.gdal.teplg");
  te::plugin::PluginManager::getInstance().add(info);
#endif

#ifdef TERRALIB_MOD_OGR_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.ogr.teplg");
  te::plugin::PluginManager::getInstance().add(info); 
#endif

#ifdef TERRALIB_MOD_MYSQL_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.mysql.teplg");
  te::plugin::PluginManager::getInstance().add(info);    
#endif

#ifdef TERRALIB_MOD_SQLITE_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.sqlite.teplg");
  te::plugin::PluginManager::getInstance().add(info);
#endif

  te::plugin::PluginManager::getInstance().loadAll(); 
}
