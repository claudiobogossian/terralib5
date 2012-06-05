#include "LoadModules.h"

#include "Config.h"

#include <terralib/common.h>
#include <terralib/plugin.h>

#include <iostream>

void LoadModules()
{
    {
#if TE_EXAMPLE_USE_OGR
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
      info.m_mainFile = "libterralib_ogr.dylib";
#else
      info.m_mainFile = "libterralib_ogr_d.dylib";
#endif
#endif

      te::plugin::PluginManager::getInstance().loadPlugin(info);
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
#if TE_EXAMPLE_USE_GRIB
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

      info.m_name = "GRIB DataSource Driver";
      info.m_description = "This data source driver supports...";

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#ifdef NDEBUG
      info.m_mainFile = "terralib_grib.dll";
#else
      info.m_mainFile = "terralib_grib_d.dll";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX
#ifdef NDEBUG
      info.m_mainFile = "libterralib_grib.so";
#else
      info.m_mainFile = "libterralib_grib.so";
#endif
#endif

#if TE_PLATFORM == TE_PLATFORMCODE_APPLE
#ifdef NDEBUG
      info.m_mainFile = "libterralib_grib.dylib";
#else
      info.m_mainFile = "libterralib_grib_d.dylib";
#endif
#endif

      te::plugin::PluginManager::getInstance().loadPlugin(info);
#endif
    }
}
