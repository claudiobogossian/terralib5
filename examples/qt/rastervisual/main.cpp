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
#include <QtGui/QMessageBox>

// STL
#include <exception>
#include <iostream>
#include <locale.h>


void LoadGDALModule()
{
  try
  {
    /*te::plugin::PluginInfo info; 
    info.m_name = "te.da.gdal";
    info.m_displayName = "GDAL DataSource Driver";
    info.m_description = "This data source driver supports spatial data managed by GDAL";
    info.m_engine = "C++";
    info.m_folder = TE_PLUGINS_PATH;
    
    std::pair<std::string, std::string> rsc("SharedLibraryName", "terralib_gdal");
    
    info.m_resources.push_back(rsc);
    
    te::plugin::PluginManager::getInstance().load(info);*/
    te::plugin::PluginInfo* info = te::plugin::GetInstalledPlugin(TE_PLUGINS_PATH + std::string("/te.da.gdal.teplg"));
    te::plugin::PluginManager::getInstance().loadAll();
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to load data source drivers: unknow exception!" << std::endl;
  }
}

int main(int argc, char** argv)
{
  setlocale(LC_ALL,"C");

// initialize Terralib support
  TerraLib::getInstance().initialize();
  LoadGDALModule();

  try
  {   
    QApplication app(argc, argv);

    // Adjusting icons theme
    QString spaths = std::string(ICON_THEME_PATH).c_str();
    QStringList paths = spaths.split(";");
    QIcon::setThemeName(ICON_THEME);
    QIcon::setThemeSearchPaths(paths);

    // Example MainWindow
    MainWindow window;
    window.showMaximized();

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
  te::plugin::PluginManager::getInstance().unloadAll();
  TerraLib::getInstance().finalize();

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}
