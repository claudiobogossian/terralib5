// Terralib Raster Manager tool
#include "Utils.h"

// TerraLib
#include <terralib/plugin/PluginManager.h>

bool te::tools::dscopy::Utils::loadModules(std::string & errorMessage)
{
  
  {
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
        info.m_mainFile = "terralib_ogr.dylib"; // should it be different????
      #else
        info.m_mainFile = "terralib_ogr.dylib";
      #endif
    #endif

    te::plugin::PluginManager::getInstance().loadPlugin(info);
  }

  {
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
        info.m_mainFile = "terralib_gdal.dylib";
      #else
        info.m_mainFile = "terralib_gdal.dylib";
      #endif
    #endif
      
    te::plugin::PluginManager::getInstance().loadPlugin(info);
  }

  {
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
        info.m_mainFile = "terralib_postgis.dylib";
      #else
        info.m_mainFile = "terralib_postgis.dylib";
      #endif
    #endif

    te::plugin::PluginManager::getInstance().loadPlugin(info);

  }

  {
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

    info.m_name = "MEM DataSource Driver";
    info.m_description = "This data source driver supports...";

    #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
      #ifdef NDEBUG
        info.m_mainFile = "terralib_memory.dll";
      #else
        info.m_mainFile = "terralib_memory_d.dll";
      #endif
    #endif

    #if TE_PLATFORM == TE_PLATFORMCODE_LINUX
      #ifdef NDEBUG
        info.m_mainFile = "libterralib_memory.so";
      #else
        info.m_mainFile = "libterralib_memory_d.so";
      #endif
    #endif

    #if TE_PLATFORM == TE_PLATFORMCODE_APPLE
      #ifdef NDEBUG
        info.m_mainFile = "terralib_memory.dylib"; // should it be different????
      #else
        info.m_mainFile = "terralib_memory.dylib";
      #endif
    #endif

    te::plugin::PluginManager::getInstance().loadPlugin(info);
  }
  
  return true;
}