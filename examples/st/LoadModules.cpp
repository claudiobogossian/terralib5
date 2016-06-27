// Examples
#include "STExamples.h"

// TerraLib
#include <terralib/core/utils/Platform.h>
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadModules()
{
  try
  {
    std::string plugins_path = te::core::FindInTerraLibPath("share/terralib/plugins");

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
  catch(const te::common::Exception& e)
  {
    std::cout << std::endl << "Failed to load data source drivers: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to load data source drivers: unknow exception!" << std::endl;
  }
}




