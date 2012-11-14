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
#include <terralib/qt/af/CoreApplication.h>
#include <terralib/qt/af/SplashScreenManager.h>

//! Qt include files
#include <QtGui/QApplication>
#include <QSplashScreen>
#include <QDir>

//! STL include files
#include <exception>

void getConfigurations(std::vector< std::pair<std::string, std::string> >& configs)
{
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xmlns", "http://www.terralib.org/schemas/af"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xsd:schemaLocation", "http://www.terralib.org/schemas/af " + std::string(TERRALIB_SCHEMA_LOCATION) + "/terralib/af/af.xsd"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.version", "5.0.0"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.release", "2011-01-01"));
  configs.push_back(std::pair<std::string, std::string>("Application.Organization", "INPE"));
  configs.push_back(std::pair<std::string, std::string>("Application.Name", "TerraView"));
  configs.push_back(std::pair<std::string, std::string>("Application.Title", "TerraView"));
  configs.push_back(std::pair<std::string, std::string>("Application.IconName", "terralib_logo"));

  QString user_set_path = QDir::toNativeSeparators(qApp->applicationDirPath() + "/user_settings.xml");
  QString app_plg_path = QDir::toNativeSeparators(qApp->applicationDirPath() + "/application_plugins.xml");

  configs.push_back(std::pair<std::string, std::string>("Application.UserSettingsFile.<xmlattr>.xlink:href", user_set_path.toStdString()));
  configs.push_back(std::pair<std::string, std::string>("Application.PluginsFile.<xmlattr>.xlink:href", app_plg_path.toStdString()));
  configs.push_back(std::pair<std::string, std::string>("Application.IconThemeInfo.BaseDirectory.<xmlattr>.xlink:href", QDir::toNativeSeparators(ICON_THEME_PATH).toStdString()));
  configs.push_back(std::pair<std::string, std::string>("Application.IconThemeInfo.DefaultTheme", ICON_THEME));
  configs.push_back(std::pair<std::string, std::string>("Application.ToolBarDefaultIconSize", "24"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Cellular Space"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Data Access"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Data Management"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Examples"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Language Bindings"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Location Base Services"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Plugin Management"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Spatial Analysis"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Spatial Operations"));
  configs.push_back(std::pair<std::string, std::string>("Application.DefaultPluginsCategory.Category", "Web Services"));
  configs.push_back(std::pair<std::string, std::string>("Application.AboutDialogLogo.<xmlattr>.xlink:href",  ""));
  configs.push_back(std::pair<std::string, std::string>("Application.TerraLibLogo.<xmlattr>.xlink:href",  QDir::toNativeSeparators(TERRALIB_LOGO).toStdString()));
}

void getPluginsConfigurations(std::vector< std::pair<std::string, std::string> >& configs)
{
  configs.push_back(std::pair<std::string, std::string>("Plugins.<xmlattr>.xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.<xmlattr>.xmlns", "http://www.terralib.org/schemas/af"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.<xmlattr>.xsd:schemaLocation", "http://www.terralib.org/schemas/af " + std::string(TERRALIB_SCHEMA_LOCATION) + "/terralib/af/af.xsd"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.<xmlattr>.version", "5.0.0"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.<xmlattr>.release", "2011-01-01"));
  //! ado
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Name", "te.da.ado"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Path.<xmlattr>.xlink:href", std::string(TERRALIB_BIN_DIR) + "/plugin_ado_info.xml"));
  //! gdal
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Name", "te.da.gdal"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Path.<xmlattr>.xlink:href", std::string(TERRALIB_BIN_DIR) + "/plugin_ogr_info.xml"));
  //! ogr
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Name", "te.da.ogr"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Path.<xmlattr>.xlink:href", std::string(TERRALIB_BIN_DIR) + "/plugin_gdal_info.xml"));
  //! pgis
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Name", "te.da.pgis"));
  configs.push_back(std::pair<std::string, std::string>("Plugins.Plugin.Path.<xmlattr>.xlink:href", std::string(TERRALIB_BIN_DIR) + "/plugin_pgis_info.xml"));
}

void getUserConfigurations(std::vector< std::pair<std::string, std::string> >& configs)
{
  configs.push_back(std::pair<std::string, std::string>("UserSettings.<xmlattr>.xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.<xmlattr>.xmlns", "http://www.terralib.org/schemas/af"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.<xmlattr>.xsd:schemaLocation", "http://www.terralib.org/schemas/af " + std::string(TERRALIB_SCHEMA_LOCATION) + "/terralib/af/af.xsd"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.<xmlattr>.version", "5.0.0"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.<xmlattr>.release", "2011-01-01"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.SelectedIconTheme", "terralib"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.LastSearchedFolder.<xmlattr>.xlink:href", qApp->applicationDirPath().toStdString()));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.ToolBarIconSize", "24"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.SpecificPlugins", ""));

#ifdef _MSC_VER
  configs.push_back(std::pair<std::string, std::string>("UserSettings.EnabledPlugins.Plugin", "te.da.ado"));
#endif
  configs.push_back(std::pair<std::string, std::string>("UserSettings.EnabledPlugins.Plugin", "te.da.gdal"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.EnabledPlugins.Plugin", "te.da.ogr"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.EnabledPlugins.Plugin", "te.da.pgis"));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.DataSourcesFile", ""));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.MostRecentProject.<xmlattr>.xlink:href", ""));
  configs.push_back(std::pair<std::string, std::string>("UserSettings.RecentProjects", ""));
}


int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  std::vector< std::pair<std::string, std::string> > configs;
  getConfigurations(configs);
  te::qt::af::teApp::getInstance().setApplicationInfo(configs);

  configs.clear();
  getPluginsConfigurations(configs);
  te::qt::af::teApp::getInstance().setApplicationPlgInfo(configs);

  configs.clear();
  getUserConfigurations(configs);
  te::qt::af::teApp::getInstance().setUserInfo(configs);

  int waitVal = EXIT_FAILURE;
  try
  {
    MainWindow dlg;

    QSplashScreen* splash = new QSplashScreen(&dlg, QPixmap(), Qt::WindowStaysOnTopHint);
    te::qt::af::SplashScreenManager::getInstance().set(splash, 1, Qt::white);
    splash->show();
    qApp->processEvents();

    te::qt::af::SplashScreenManager::getInstance().showMessage(QObject::tr("Loading TerraLib configuration files."));

// initialize the application
    te::qt::af::teApp::getInstance().initialize();

    dlg.showMaximized();
    splash->finish(&dlg);

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

  te::qt::af::teApp::getInstance().finalize();

  return waitVal;
}

