/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/UserRecentProjects.h

  \brief A singleton for managing the recent projects of a specific user.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_USERRECENTPROJECTS_H
#define __TERRALIB_QT_AF_INTERNAL_USERRECENTPROJECTS_H

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
        \class UserRecentProjects

        \brief A singleton for managing the recent projects of a specific user.
      */
      class TEQTAFEXPORT UserRecentProjects
        : public te::common::ObjectLevelLockable<UserRecentProjects,
                                                 ::boost::recursive_mutex,
                                                 ::boost::lock_guard<::boost::recursive_mutex>,
                                                 ::boost::lock_guard<::boost::recursive_mutex> >,
          public te::common::Singleton<UserRecentProjects>
      {
        friend class te::common::Singleton<UserRecentProjects>;

        public:

          /*! \brief This method will add in the application the list of recent opened projects. */
          void load();

        protected:

          /*! \brief It initializes the singleton. */
          UserRecentProjects();

          /*! \brief Destructor. */
          ~UserRecentProjects();
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_INTERNAL_USERRECENTPROJECTS_H
