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

    // Adjusting icons theme
    QString spaths = std::string(ICON_THEME_PATH).c_str();
    QStringList paths = spaths.split(";");
    QIcon::setThemeName(ICON_THEME);
    QIcon::setThemeSearchPaths(paths);

    LoadOGRModule();

    // Instructions
    QString text = QString::fromLatin1("<p>Here you have a Map Display component associated with a set of geographic tools.\
                                        <p>There are somes tools always active. e.g.:\
                                        <ul><li><b>CoordTracking:</b> move the mouse on Map Display and see the tracked geographic coordinate on status bar;</li>\
                                            <li><b>ZoomWheel:</b> use the mouse wheel to performs a zoom on MapDisplay;</li>\
                                            <li><b>ZoomKeyboard:</b> you can use the keys (+) / (Up) or (-) / (Down) to performs a zoom also.</li>\
                                        </ul>\
                                        <p>Some tools can be activated using the tool bar. e.g.\
                                        <ul><li><b>Pan:</b> click on MapDisplay using the left mouse button. Keep the button hold and move. Release the button to perform a pan;</li>\
                                            <li><b>ZoomIn:</b> click on MapDisplay using the left mouse button. You will perform a zoom in on clicked point;</li>\
                                            <li><b>ZoomOut:</b> click on MapDisplay using the left mouse button. You will perform a zoom out on clicked point;</li>\
                                            <li><b>ZoomArea:</b> click on MapDisplay using the left mouse button. Keep the button hold and move to define a boundary rectangle. Release the button to perform a zoom in on this area;</li>\
                                            <li><b>Measure:</b> you can measure distance, area and angles. Try it!\
                                                         Select the specified tool. Click on MapDisplay using the left mouse button to fix a point. Move the mouse to see a geometry and the measured value.\
                                                         Double click left button to stop the measuring.</li>\
                                        </ul>\
                                        ");

    QMessageBox instructions;
    instructions.setWindowTitle("Information");
    instructions.setText(text);
    instructions.setTextFormat(Qt::RichText);
    instructions.setWindowModality(Qt::NonModal);
    instructions.move(0, 0);
    instructions.show();

    // Example MainWindow
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
