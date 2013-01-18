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

  \brief A list of examples for the TerraLib Widgets.
 */

// TerraLib
#include "Config.h"
#include "LoadModules.h"
#include "QueryExamples.h"
#include <terralib/common.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/plugin.h>
#include <terralib/qt/widgets/query/QueryBuilderWizard.h>

// QT
#include <QtGui/QApplication>
#include <QtGui/QIcon>

// STL
#include <string>
#include <exception>
#include <iostream>

int main(int argc, char** argv)
{
  try
  {
    // initialize Terralib support
    TerraLib::getInstance().initialize();
    LoadModules();

    // execute the dialog
    QApplication app(argc, argv);

    // Adjusting icons theme
    QString spaths = std::string(ICON_THEME_PATH).c_str();
    QStringList paths = spaths.split(";");
    QIcon::setThemeName(ICON_THEME);
    QIcon::setThemeSearchPaths(paths);

    // Load datasource
    std::string fileName = TE_DATA_EXAMPLE_LOCALE;
                fileName+= "/data/shp/munic_2001.shp";

    LoadShapeDataSource(fileName, "MunicShp2001");

    LoadPGISDataSource("graphDb");

    te::qt::widgets::QueryBuilderWizard w(0);

    w.exec();

  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried!" << std::endl;

    return EXIT_FAILURE;
  }

  te::plugin::PluginManager::getInstance().unloadAll();

// finalize Terralib support
  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
