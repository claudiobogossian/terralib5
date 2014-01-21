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
  \file terralib/common/SystemApplicationSettings.cpp

  \brief A singleton for managing application settings applied to the whole system (all users).
*/

// TerraLib
#include "Exception.h"
//#include "OSSettingsDir.h"
#include "SystemApplicationSettings.h"
#include "Translator.h"

// STL
#include <cstdlib>

// Boost
#include <boost/filesystem.hpp>

void te::common::SystemApplicationSettings::load()
{
// look in the application current dir for a file TERRALIB_SYSTEM_SETTINGS_FILE inside the folder TERRALIB_CONFIG_DIR
  boost::filesystem::path config_file = boost::filesystem::current_path();

  config_file /= TERRALIB_CONFIG_DIR;

  config_file /= TERRALIB_SYSTEM_SETTINGS_FILE;

  if(boost::filesystem::is_regular_file(config_file))
  {
    load(config_file.string());
    return;
  }

// if not found let's try to search in the system application data dir
  //const std::string& app_data_dir = OSSettingsDir::getInstance().getSystemSettingsPath();

  //if(!app_data_dir.empty())
  //{
  //  config_file = app_data_dir;

  //  config_file /= TERRALIB_DIR;

  //  config_file /= TERRALIB_SYSTEM_SETTINGS_FILE;

  //  if(boost::filesystem::is_regular_file(config_file))
  //  {
  //    load(config_file.string());
  //    return;
  //  }
  //}

// the last chance...
  char* mgis_dir = getenv(TERRALIB_DIR_ENVVAR);

  if(mgis_dir != 0)
  {
    config_file = mgis_dir;

    config_file /= TERRALIB_CONFIG_DIR;

    config_file /= TERRALIB_SYSTEM_SETTINGS_FILE;

    if(boost::filesystem::is_regular_file(config_file))
    {
      load(config_file.string());
      return;
    }
  }

  throw Exception(TR_COMMON("Could not find system application config file!"));
}

void te::common::SystemApplicationSettings::load(const std::string& fileName)
{
  ApplicationSettings::load(fileName);
}

te::common::SystemApplicationSettings::SystemApplicationSettings()
{
}

te::common::SystemApplicationSettings::~SystemApplicationSettings()
{
}


