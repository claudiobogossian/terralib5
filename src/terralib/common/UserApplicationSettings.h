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
  \file terralib/common/UserApplicationSettings.h

  \brief A singleton for managing application settings applied to a single user.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_USERAPPLICATIONSETTINGS_H
#define __TERRALIB_COMMON_INTERNAL_USERAPPLICATIONSETTINGS_H

// TerraLib
#include "ApplicationSettings.h"
#include "Singleton.h"

namespace te
{
  namespace common
  {
    /*!
      \class UserApplicationSettings

      \brief A singleton for managing application settings applied to a single user.

      This singleton can be used to keep user preferences that
      customize the application's appearance and behavior for a given user.

      It will look for an user configuration file as follows:
      <ul>
      <li>first, searchs for the user settings file name in the system application settings singleton</li>
      <li>if not found, use the file name defined by the macro TERRALIB_USER_SETTINGS_FILE</li>
      <li>then it check for this file in the current application dir under the folder TERRALIB_CONFIG_DIR</li>
      <li>if not found, it checks for in the user data dir (plataform dependent!)</li>
      <li>and finally it uses a location defined by an environment variable TERRALIB_DIR_ENVVAR and a sub-dir defined by the macro TERRALIB_CONFIG_DIR</li>
      </ul>

      \ingroup common
    */
    class TECOMMONEXPORT UserApplicationSettings
      : public ApplicationSettings,
        public Singleton<UserApplicationSettings>
    {
      friend class te::common::Singleton<UserApplicationSettings>;

      public:

        /*! \brief It tries to find a default config file based on system macros and default condigurations. */
        //void load();

        /*!
          \brief It loads the setting from the given file.
          
          \param fileName It must be the full path to the settings file.
        */
        void load(const std::string& fileName);

      protected:

        /*! \brief It initializes the singleton. */
        UserApplicationSettings();

        /*! \brief Destructor. */
        ~UserApplicationSettings();
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_USERAPPLICATIONSETTINGS_H

