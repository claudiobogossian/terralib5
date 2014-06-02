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

  std::string plugins_path = te::common::FindInTerraLibPath("share/terralib/plugins");

#ifdef TERRALIB_MOD_ADO_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.ado.teplg");
  te::plugin::PluginManager::getInstance().add(info);
#endif

#ifdef TERRALIB_MOD_OGR_ENABLED
  info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.ogr.teplg");
  te::plugin::PluginManager::getInstance().add(info);
#endif

  te::plugin::PluginManager::getInstance().loadAll(); 
}
