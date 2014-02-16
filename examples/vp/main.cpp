/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file main.cpp

  \brief A list of examples for the TerraLib Spatial Reference Systems Module.
 */

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>

// Examples
#include "VectorProcessing.h"

// STL
#include <cassert>
#include <cstdlib>
#include <exception>
#include <iostream>

int main(int argc, char** argv)
{
// initialize Terralib support
// It initializes all the data source drivers (see LoadModule.cpp)
  try
  {
    TerraLib::getInstance().initialize();
    
    te::plugin::PluginInfo* info;
    
    info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.gdal.teplg"));
    te::plugin::PluginManager::getInstance().add(info);
    
    info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.ogr.teplg"));
    te::plugin::PluginManager::getInstance().add(info);
    
    info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.pgis.teplg"));
    te::plugin::PluginManager::getInstance().add(info);
    
    te::plugin::PluginManager::getInstance().loadAll();
    
    std::cout << std::endl << "PostGIS to OGR: " << std::endl;
    if (AggregPGISToOGR())
      std::cout << "\tOK!" << std::endl;

    std::cout << std::endl << "OGR to PostGIS: " << std::endl;
    if (AggregOGRToPGIS())
      std::cout << "\tOK!" << std::endl;

    std::cout << std::endl << "OGR to OGR: " << std::endl;
    if (AggregOGRToOGR())
      std::cout << "\tOK!" << std::endl;

    std::cout << std::endl << "PostGIS to PostGIS: " << std::endl;
    if (AggregPGISToPGIS())
      std::cout << "\tOK!" << std::endl;

    std::cout << std::endl << "Intersection PostGIS to OGR: " << std::endl;
    if (IntersectionPGISToOGR())
      std::cout << "\tOK!" << std::endl;

    std::cout << std::endl << "Intersection OGR to PostGIS: " << std::endl;
    if (IntersectionOGRToPGIS())
      std::cout << "\tOK!" << std::endl;

    std::cout << std::endl << "Intersection PostGIS to PostGIS: " << std::endl;
    if (IntersectionPGISToPGIS())
      std::cout << "\tOK!" << std::endl;

    te::plugin::PluginManager::getInstance().unloadAll();

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried: " << e.what() << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried!" << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}

