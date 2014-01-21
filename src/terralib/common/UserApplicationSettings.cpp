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
  \file terralib/common/UserApplicationSettings.cpp

  \brief A singleton for managing application settings applied to a single user.
*/

// TerraLib
#include "Exception.h"
//#include "OSSettingsDir.h"
#include "SystemApplicationSettings.h"
#include "Translator.h"
#include "UserApplicationSettings.h"

// STL
#include <cstdlib>

// Boost
#include <boost/filesystem.hpp>

void te::common::UserApplicationSettings::load()
{
  std::string user_conf_file_name = SystemApplicationSettings::getInstance().getValue("Application.UserSettingsFile.<xmlattr>.xlink:href");

  if(user_conf_file_name.empty())
    user_conf_file_name = TERRALIB_USER_SETTINGS_FILE;

// first: current application dir
  boost::filesystem::path user_settings_file = boost::filesystem::current_path();
  
  user_settings_file /= TERRALIB_CONFIG_DIR;

  user_settings_file /= user_conf_file_name;

  if(boost::filesystem::is_regular_file(user_settings_file))
  {
    load(user_settings_file.string());
    return;
  }

// second: user data dir
  //const std::string& user_data_dir = OSSettingsDir::getInstance().getUserSettingsPath();

  //if(!user_data_dir.empty())
  //{
  //  user_settings_file = user_data_dir;

  //  user_settings_file /= TERRALIB_DIR;

  //  user_settings_file /= user_conf_file_name;

  //  if(boost::filesystem::is_regular_file(user_settings_file))
  //  {
  //    load(user_settings_file.string());
  //    return;
  //  }
  //}

// the last chance...
  char* mgis_dir = getenv(TERRALIB_DIR_ENVVAR);

  if(mgis_dir != 0)
  {
    user_settings_file = mgis_dir;

    user_settings_file /= TERRALIB_CONFIG_DIR;

    user_settings_file /= user_conf_file_name;

    if(boost::filesystem::is_regular_file(user_settings_file))
    {
      load(user_settings_file.string());
      return;
    }
  }
}

void te::common::UserApplicationSettings::load(const std::string& fileName)
{
  ApplicationSettings::load(fileName);
}

te::common::UserApplicationSettings::UserApplicationSettings()
{
}

te::common::UserApplicationSettings::~UserApplicationSettings()
{
}


