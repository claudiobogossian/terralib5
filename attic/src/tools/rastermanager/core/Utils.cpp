/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/tools/rastermanager/core/Utils.cpp

  \brief Terralib Raster Manager Tool Utils 
 */

// Terralib Raster Manager tool
#include "Utils.h"

// TerraLib
#include <terralib/plugin/PluginManager.h>
#include <terralib/raster/RasterFactory.h>

// STL
#include <map>

// Boost
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

bool te::tools::rastermanager::Utils::checkInputPath(std::string inputPath, std::string & errorMessage)
{ 
  
  fs::path in(inputPath);
   
  if(fs::exists(in))
  {
    if(fs::is_directory(in))
    {
      errorMessage = "Enter with the full path, including the file name and extension!";
      return false;
    }
    if(!fs::is_regular_file(in))
    {
      errorMessage = "Input not identified!";
      return false;
    }

    std::string extension = fs::extension(in);
  
  }
  else
  {
    errorMessage = "Input Raster not exists!";
    return false;
  }  

  return true;
}

bool te::tools::rastermanager::Utils::checkOutputPath(std::string outputPath, std::string & errorMessage)
{
  if(outputPath.empty())
  {
    errorMessage = "Output path invalid!!";
    return false;
  }

  fs::path out(outputPath);
  std::string extension = fs::extension(out);
      
  out.remove_filename();

  if(!fs::exists(out))
  {
    errorMessage = "The Output path not exists!";
    return false;
  }  

  return true;
}

bool te::tools::rastermanager::Utils::getRaster(std::string path, te::rst::Raster* & raster, std::string & errorMessage)
{
  
  std::map<std::string, std::string> rasterInfo;
  rasterInfo["URI"] = path;  
  
  raster = te::rst::RasterFactory::open(rasterInfo);
 
  return true;
}

bool te::tools::rastermanager::Utils::loadModules(std::string & errorMessage)
{
  
  // Load OGR
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
        info.m_mainFile = "libterralib_ogr.dylib";
      #else
        info.m_mainFile = "libterralib_ogr.dylib";
      #endif
    #endif

    te::plugin::PluginManager::getInstance().loadPlugin(info);
   
  }

  // Load GDAL
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
        info.m_mainFile = "libterralib_gdal.dylib";
      #else
        info.m_mainFile = "libterralib_gdal.dylib";
      #endif
    #endif 
 
    te::plugin::PluginManager::getInstance().loadPlugin(info);
  }

  // Load GRIB
/*  {
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
    info.m_mainFile = "libterralib_grib.dylib";
#endif
#endif

    te::plugin::PluginManager::getInstance().loadPlugin(info);
  }*/
  
  return true;
}