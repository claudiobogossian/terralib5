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
  \file main.cpp

  \brief Example for the TerraLib Qt Tools.
 */

// Example
#include "MainWindow.h"

// TerraLib
#include <terralib/common/PlatformUtils.h>
#include <terralib/common.h>
#include <terralib/plugin.h>

// Qt
#include <QApplication>
#include <QMessageBox>

// STL
#include <exception>
#include <iostream>
#include <locale.h>


void LoadGDALModule()
{
  try
  {
    std::string plugins_path = te::common::FindInTerraLibPath("share/terralib/plugins");

#ifdef TERRALIB_MOD_GDAL_ENABLED
    te::plugin::PluginInfo* info;
    info = te::plugin::GetInstalledPlugin(plugins_path + "/te.da.gdal.teplg");
#endif

    te::plugin::PluginManager::getInstance().loadAll();
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to load data source drivers: unknown exception!" << std::endl;
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
    QStringList ithemes = QIcon::themeSearchPaths();
    ithemes.push_back(te::common::FindInTerraLibPath("share/terralib/icons").c_str());
    QIcon::setThemeName("terralib");
    QIcon::setThemeSearchPaths(ithemes);

    // Example MainWindow
    MainWindow window;
    window.showMaximized();

    app.exec();
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred!" << std::endl;

    return EXIT_FAILURE;
  }

// finalize Terralib support
  te::plugin::PluginManager::getInstance().unloadAll();
  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
