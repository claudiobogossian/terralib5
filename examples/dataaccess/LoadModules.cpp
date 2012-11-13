// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadModules()
{
  try
  {
    {
#if TE_EXAMPLE_USE_OGR
      te::plugin::PluginInfo info;

      info.m_name = "OGR DataSource Driver";
      info.m_description = "This data source driver supports...";

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#ifdef NDEBUG
      info.m_mainFile = "terralib_ogr.dll";
#else
      info.m_mainFile = "terralib_ogr_d.dll";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX
#ifdef NDEBUG
      info.m_mainFile = "libterralib_ogr.so";
#else
      info.m_mainFile = "libterralib_ogr_d.so";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_APPLE
#ifdef NDEBUG
      info.m_mainFile = "libterralib_ogr.dylib";
#else
      info.m_mainFile = "libterralib_ogr_d.dylib";
#endif
#endif

      te::plugin::PluginManager::getInstance().load(info);
#endif    
    }

    {
#if TE_EXAMPLE_USE_GDAL
      te::plugin::PluginInfo info;
      
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      info.m_type = "dll";
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
      info.m_type = "s.o.";
#elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
      info.m_type = "dylib";      
#else
  #error "Platform not supported yet"
#endif

      info.m_name = "GDAL DataSource Driver";
      info.m_description = "This data source driver supports...";

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#ifdef NDEBUG
      info.m_mainFile = "terralib_gdal.dll";
#else
      info.m_mainFile = "terralib_gdal_d.dll";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX
#ifdef NDEBUG
      info.m_mainFile = "libterralib_gdal.so";
#else
      info.m_mainFile = "libterralib_gdal_d.so";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_APPLE
#ifdef NDEBUG
      info.m_mainFile = "libterralib_gdal.dylib";
#else
      info.m_mainFile = "libterralib_gdal_d.dylib";
#endif
#endif
      
      te::plugin::PluginManager::getInstance().loadPlugin(info);
#endif    
    }

    {
#if TE_EXAMPLE_USE_PGIS
      te::plugin::PluginInfo info;

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      info.m_type = "dll";
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX
      info.m_type = "s.o.";
#elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
      info.m_type = "dylib";      
#else
  #error "Platform not supported yet"
#endif

      info.m_name = "PostGIS DataSource Driver";
      info.m_description = "This data source driver supports...";

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#ifdef NDEBUG
      info.m_mainFile = "terralib_postgis.dll";
#else
      info.m_mainFile = "terralib_postgis_d.dll";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX
#ifdef NDEBUG
      info.m_mainFile = "libterralib_postgis.so";
#else
      info.m_mainFile = "libterralib_postgis_d.so";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_APPLE
#ifdef NDEBUG
      info.m_mainFile = "libterralib_postgis.dylib";
#else
      info.m_mainFile = "libterralib_postgis_d.dylib";
#endif
#endif

      te::plugin::PluginManager::getInstance().loadPlugin(info);
#endif

    }
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

