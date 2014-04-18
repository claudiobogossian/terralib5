// TerraLib
#include <terralib/common/PlatformUtils.h>
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadModules()
{
  te::plugin::PluginInfo* info;
  
  std::string plugin_path = te::common::FindInTerraLibPath("share/terralib/plugins");

  info = te::plugin::GetInstalledPlugin(plugin_path + "/te.da.pgis.teplg");
  te::plugin::PluginManager::getInstance().add(info);
 
  info = te::plugin::GetInstalledPlugin(plugin_path + "/te.da.gdal.teplg");
  te::plugin::PluginManager::getInstance().add(info);

  info = te::plugin::GetInstalledPlugin(plugin_path + "/te.da.ogr.teplg");
  te::plugin::PluginManager::getInstance().add(info); 

  te::plugin::PluginManager::getInstance().loadAll(); 

/*
#if TE_EXAMPLE_USE_OGR
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.ogr.teplg"));
  te::plugin::PluginManager::getInstance().add(info); 
#endif
  
#if TE_EXAMPLE_USE_GDAL
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.gdal.teplg"));
  te::plugin::PluginManager::getInstance().add(info);
#endif
  
#if TE_EXAMPLE_USE_PGIS
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.pgis.teplg"));
  te::plugin::PluginManager::getInstance().add(info);    
#endif

#if TE_EXAMPLE_USE_MYSQL
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.mysql.teplg"));
  te::plugin::PluginManager::getInstance().add(info);    
#endif

#if TE_EXAMPLE_USE_SQLITE
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.sqlite.teplg"));
  te::plugin::PluginManager::getInstance().add(info);
#endif 
*/
}
