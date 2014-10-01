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
  \file terralib/common/SystemApplicationSettings.h

  \brief A singleton for managing application settings applied to the whole system (all users).
*/

#ifndef __TERRALIB_COMMON_INTERNAL_SYSTEMAPPLICATIONSETTINGS_H
#define __TERRALIB_COMMON_INTERNAL_SYSTEMAPPLICATIONSETTINGS_H

// TerraLib
#include "ApplicationSettings.h"
#include "Singleton.h"

namespace te
{
  namespace common
  {
    /*!
      \class SystemApplicationSettings

      \brief A singleton for managing application settings applied to the whole system (all users).

      This singleton can be used to keep the application configuration.

      It will look for an application configuration file as follows:
      <ul>
      <li>first, searchs for the application configuration file in the current application dir</li>
      <li>then it check for this file in the application data dir (plataform dependent!)</li>
      <li>use a location defined by an environment variable TERRALIB_DIR_ENVVAR and a sub-dir defined by the macro TERRALIB_CONFIG_DIR</li>
      </ul>

      \ingroup common
    */
    class TECOMMONEXPORT SystemApplicationSettings : public ApplicationSettings,
                                                     public Singleton<SystemApplicationSettings>
    {
      friend class Singleton<SystemApplicationSettings>;

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
        SystemApplicationSettings();

        /*! \brief Destructor. */
        ~SystemApplicationSettings();
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_SYSTEMAPPLICATIONSETTINGS_H

