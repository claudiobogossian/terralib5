// Examples
#include "CanvasExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadModules()
{
  try
  { 
#if TE_EXAMPLE_USE_PGIS    
    {      
      te::plugin::PluginInfo info;      
      info.m_name = "te.da.pgis";
      info.m_displayName = "POSTGIS DataSource Driver";
      info.m_description = "This data source driver supports data stored in a PostGIS database";
      info.m_engine = "C++";
      info.m_folder = PLUGINS_PATH;
      
      std::pair<std::string, std::string> rsc("SharedLibraryName", "terralib_postgis");
      
      info.m_resources.push_back(rsc);
      
      te::plugin::PluginManager::getInstance().load(info);
    }
#endif    
    
#if TE_EXAMPLE_USE_OGR  
    {
      te::plugin::PluginInfo info; 
      info.m_name = "te.da.ogr";
      info.m_displayName = "OGR DataSource Driver";
      info.m_description = "This data source driver supports spatial data managed by OGR";
      info.m_engine = "C++";
      info.m_folder = PLUGINS_PATH;
      
      std::pair<std::string, std::string> rsc("SharedLibraryName", "terralib_ogr");
      
      info.m_resources.push_back(rsc);
      
      te::plugin::PluginManager::getInstance().load(info);
    }
#endif
    
#if TE_EXAMPLE_USE_GDAL  
    {
      te::plugin::PluginInfo info; 
      info.m_name = "te.da.gdal";
      info.m_displayName = "GDAL DataSource Driver";
      info.m_description = "This data source driver supports spatial data managed by GDAL";
      info.m_engine = "C++";
      info.m_folder = PLUGINS_PATH;
      
      std::pair<std::string, std::string> rsc("SharedLibraryName", "terralib_gdal");
      
      info.m_resources.push_back(rsc);
      
      te::plugin::PluginManager::getInstance().load(info);
    }
#endif    
    
#if TE_EXAMPLE_USE_XERCES  
    {
      te::plugin::PluginInfo info; 
      info.m_name = "te.xerces";
      info.m_displayName = "Xerces XML Reader Driver";
      info.m_description = "";
      info.m_engine = "C++";
      info.m_folder = PLUGINS_PATH;
      
      std::pair<std::string, std::string> rsc("SharedLibraryName", "terralib_xerces");
      
      info.m_resources.push_back(rsc);
      
      te::plugin::PluginManager::getInstance().load(info);
    }
#endif
  }
  catch(const te::common::Exception& e)
  {
    throw e;
  }  
}