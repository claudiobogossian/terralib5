/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/ApplicationPlugins.cpp

  \brief A singleton for managing the application plugins.
*/

// TerraLib
#include "../../common/OSSettingsDir.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/Translator.h"
#include "ApplicationPlugins.h"
#include "Exception.h"

// Boost
#include <boost/filesystem.hpp>

void te::qt::af::ApplicationPlugins::load()
{
// look for the plugins file in order to load it
  std::string plugins_conf_file_name = te::common::SystemApplicationSettings::getInstance().getValue("Application.PluginsFile.<xmlattr>.xlink:href");

  if(plugins_conf_file_name.empty())
    plugins_conf_file_name = TERRALIB_APPLICATION_PLUGINS_FILE;

// first: current application dir
  boost::filesystem::path plugins_conf_file = boost::filesystem::current_path();

  plugins_conf_file /= TERRALIB_CONFIG_DIR;

  plugins_conf_file /= plugins_conf_file_name;

  if(boost::filesystem::is_regular_file(plugins_conf_file))
  {
    load(plugins_conf_file.string());
    return;
  }

// second: system application data dir
  const std::string& system_settings_path = te::common::OSSettingsDir::getInstance().getSystemSettingsPath();

  if(!system_settings_path.empty())
  {
    plugins_conf_file = system_settings_path;

    plugins_conf_file /= TERRALIB_DIR;

    plugins_conf_file /= plugins_conf_file_name;

    if(boost::filesystem::is_regular_file(plugins_conf_file))
    {
      load(plugins_conf_file.string());
      return;
    }
  }

// third: user dir
  const std::string& user_settings_path = te::common::OSSettingsDir::getInstance().getUserSettingsPath();

  if(!user_settings_path.empty())
  {
    plugins_conf_file = user_settings_path;

    plugins_conf_file /= TERRALIB_DIR;

    plugins_conf_file /= plugins_conf_file_name;

    if(boost::filesystem::is_regular_file(plugins_conf_file))
    {
      load(plugins_conf_file.string());
      return;
    }
  }

// the last chance...
  char* mgis_dir = getenv(TERRALIB_DIR_ENVVAR);

  if(mgis_dir != 0)
  {
    plugins_conf_file = mgis_dir;

    plugins_conf_file /= TERRALIB_CONFIG_DIR;

    plugins_conf_file /= plugins_conf_file_name;

    if(boost::filesystem::is_regular_file(plugins_conf_file))
    {
      load(plugins_conf_file.string());
      return;
    }
  }

  throw Exception(TR_QT_AF("Can not find application plugins config file!"));
}

void te::qt::af::ApplicationPlugins::load(const std::string& fileName)
{
  ApplicationSettings::load(fileName);
}

te::qt::af::ApplicationPlugins::ApplicationPlugins()
{
}

te::qt::af::ApplicationPlugins::~ApplicationPlugins()
{
}


