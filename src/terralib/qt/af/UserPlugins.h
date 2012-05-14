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
  \file terralib/qt/af/UserPlugins.h

  \brief A singleton for managing the plugins enabled by a specific user.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_USERPLUGINS_H
#define __TERRALIB_QT_AF_INTERNAL_USERPLUGINS_H

// TerraLib
#include "../../common/Singleton.h"
#include "../../common/ThreadingPolicies.h"
#include "Config.h"

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class UserPlugins

        \brief A singleton for managing the plugins enabled by a specific user.

        This singleton can be used to keep the list of plugins enabled by a given application user.

        It will look for a plugin configuration file as follows:
        <ul>
        <li>first, searchs for the plugin configuration file name in SystemApplicationSettings</li>
        <li>if the file name is not found in SystemApplicationSettings, uses the default name defined by the macro TERRALIB_APPLICATION_PLUGINS_FILE</li>
        <li>then it check for this file in the current application dir</li>
        <li>if not foud it looks in the syste application dir</li>
        <li>if not found looks in the user folder</li>
        <li>and finally it uses a location defined by an environment variable TERRALIB_DIR_ENVVAR under TERRALIB_CONFIG_DIR</li>
        </ul>
      */
      class TEQTAFEXPORT UserPlugins
        : public te::common::ObjectLevelLockable<UserPlugins,
                                                 ::boost::mutex,
                                                 ::boost::lock_guard<::boost::mutex>,
                                                 ::boost::lock_guard<::boost::mutex> >,
          public te::common::Singleton<UserPlugins>
      {
        friend class te::common::Singleton<UserPlugins>;

        public:

          /*!
            \brief It starts all the plugins enabled by the user.

            \note Thread-safe.

            \note Before calling this method, unload all plugins.
          */
          void load();

        protected:

          /*! \brief It initializes the singleton. */
          UserPlugins();

          /*! \brief Destructor. */
          ~UserPlugins();
      };

    }   // end namespace af
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_AF_INTERNAL_USERPLUGINS_H
