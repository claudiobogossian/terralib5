/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*!
  \file terraview/main.cpp

  \brief It contains the main routine of TerraView.
*/

#include "MainWindow.h"

//! TerraLib include files
#include <terralib/common/TerraLib.h>
#include <terralib/plugin.h>

//! Qt include files
#include <QtGui/QApplication>

//! STL include files
//#include <cstdlib>
//#include <exception>

void loadModules()
{
  try
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
      info.m_mainFile = "libterralib_ogr.dylib";
#else
      info.m_mainFile = "libterralib_ogr.dylib";
#endif
#endif

      te::plugin::PluginManager::getInstance().loadPlugin(info);
    }
  }
  catch(const te::common::Exception& e)
  {
    throw e;
  }
}


int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  int waitVal = EXIT_FAILURE;
  QString spaths(ICON_THEME_PATH);

  QStringList paths = spaths.split(";");

  QIcon::setThemeName("terralib");
  QIcon::setThemeSearchPaths(paths);

  try
  {

    TerraLib::getInstance().initialize();

    loadModules();

// initialize the application
    MainWindow dlg;
    dlg.showMaximized();

    waitVal = app.exec();
  }
  catch(const std::exception& /*e*/)
  {
    return EXIT_FAILURE;
  }
  catch(...)
  {
    return EXIT_FAILURE;
  }

  TerraLib::getInstance().finalize();

  return waitVal;
}

