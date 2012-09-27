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

  \brief Example for the TerraLib Qt Tools.
 */

// Example
#include "MainWindow.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>

// Qt
#include <QtGui/QApplication>

// STL
#include <exception>
#include <iostream>

void LoadOGRModule()
{
  try
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
    info.m_description = "The OGR data source driver.";

    #if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
    #ifdef NDEBUG
      info.m_mainFile = "terralib_ogr.dll";
    #else
      info.m_mainFile = "terralib_ogr_d.dll";
    #endif
    #endif

    #if TE_PLATFORM == TE_PLATFORMCODE_APPLE
    #ifdef NDEBUG
      info.m_mainFile = "libterralib_ogr.dylib";
    #else
      info.m_mainFile = "libterralib_ogr_d.dylib";
    #endif
    #endif

    #if TE_PLATFORM == TE_PLATFORMCODE_LINUX
    #ifdef NDEBUG
      info.m_mainFile = "libterralib_ogr.so";
    #else
      info.m_mainFile = "libterralib_ogr_d.so";
    #endif
    #endif
      te::plugin::PluginManager::getInstance().loadPlugin(info);
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to load data source drivers: unknow exception!" << std::endl;
  }
}

int main(int argc, char** argv)
{
// initialize Terralib support
  TerraLib::getInstance().initialize();

  try
  {
    QApplication app(argc, argv);

    LoadOGRModule();

    MainWindow window;
    window.show();
    
    app.exec();
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried!" << std::endl;
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }

// finalize Terralib support
  TerraLib::getInstance().finalize();

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}
