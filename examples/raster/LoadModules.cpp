#include "RasterExamples.h"

#include <terralib/common.h>
#include <terralib/plugin.h>

#include <iostream>

void LoadModules()
{
  te::plugin::PluginInfo* info;
  
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
  
  te::plugin::PluginManager::getInstance().loadAll(); 
}

