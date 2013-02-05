// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadModules()
{
  te::plugin::PluginInfo* info;
  
#if TE_EXAMPLE_USE_OGR
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/plugin_ogr_info.xml"));
  te::plugin::PluginManager::getInstance().add(info); 
#endif
  
#if TE_EXAMPLE_USE_GDAL
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/plugin_gdal_info.xml"));
  te::plugin::PluginManager::getInstance().add(info);
#endif
  
#if TE_EXAMPLE_USE_PGIS
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/plugin_pgis_info.xml"));
  te::plugin::PluginManager::getInstance().add(info);    
#endif

#if TE_EXAMPLE_USE_MYSQL
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/plugin_mysql_info.xml"));
  te::plugin::PluginManager::getInstance().add(info);    
#endif

#if TE_EXAMPLE_USE_SQLITE
  info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/plugin_sqlite_info.xml"));
  te::plugin::PluginManager::getInstance().add(info);
#endif   
  
  te::plugin::PluginManager::getInstance().loadAll(); 
}
