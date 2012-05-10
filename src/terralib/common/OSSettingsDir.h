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
  \file terralib/common/OSSettingsDir.h

  \brief A singleton class for discovering the Operational System settings directories.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_OSSETTINGSDIR_H
#define __TERRALIB_COMMON_INTERNAL_OSSETTINGSDIR_H

// TerraLib
#include "Config.h"
#include "Singleton.h"

// STL
#include <string>

namespace te
{
  namespace common
  {
    /*!
      \class OSSettingsDir

      \brief A singleton class for discovering Operational System settings dir.

      Each Operational System has a default directory
      where applications may store user settings or the whole
      application settings (or system settings).

      This class is a wrapper around each Operational System.

      \todo This class must be revisited! On Windows it uses some routines that we must pay
            a special attention, they can retrieve wrong places depending on the policies
            installed on the machine.
    */
    class TECOMMONEXPORT OSSettingsDir : public Singleton<OSSettingsDir>
    {
      friend class Singleton<OSSettingsDir>;

      public:

        /*!
          \brief It returns the folder location to store per user data.

          \return The folder location to store per user data.
        */
        const std::string& getUserSettingsPath() const;

        /*!
          \brief It returns the folder location to store application data applied to all users.

          \return The folder location to store application data applied to all users.
        */
        const std::string& getSystemSettingsPath() const;

      protected:

        /*! \brief It initializes the singleton. */
        OSSettingsDir();

        /*! \brief Destructor. */
        ~OSSettingsDir();

      private:

        std::string m_userSettingsPath;     //!< Folder to output data by user.
        std::string m_systemSettingsPath;   //!< Folder to output data for all users.
    };

  } // end namespace common
}   // end namespace te

#endif  // __TERRALIB_COMMON_INTERNAL_OSSETTINGSDIR_H

