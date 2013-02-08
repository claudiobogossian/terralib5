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
  \file terralib/sqlite/Globals.h
   
  \brief An static class with global definitions for the TerraLib SQLite Data Access driver.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_GLOBALS_H
#define __TERRALIB_SQLITE_INTERNAL_GLOBALS_H

// TerraLib
#include "../common/Static.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
// Forward declaration
  namespace da
  {
    class SQLDialect;
    class DataSourceCapabilities;
  }

  namespace sqlite
  {
    /*!
      \class Globals

      \brief An static class with global definitions for the TerraLib SQLite Data Access driver.
    */
    class TESQLITEEXPORT Globals : public te::common::Static
    {
      public:

        static const std::string sm_driverIdentifier;     //!< The SQLite driver identifier.
        static const std::string sm_rliteIdentifier;      //!< The RasterLite driver identifier.
        static const std::string sm_teliteIdentifier;      //!< The RasterLite driver identifier.

        static te::da::SQLDialect* sm_spatialiteDialect;  //!< The SQL dialect supported by SQLite driver when using SpatiaLite.
        static te::da::SQLDialect* sm_sqliteDialect;  //!< The SQL dialect supported by SQLite driver when not using SpatiaLite.

        static te::da::DataSourceCapabilities* sm_spatialiteCapabilities;   //!< The SQL capabilities supported by SQLite driver when using SpatiaLite.
        static te::da::DataSourceCapabilities* sm_sqliteCapabilities;       //!< The SQL capabilities supported by SQLite driver when not using SpatiaLite.
    };

  } // end namespace sqlite
}   // end namespace te

#endif  // __TERRALIB_SQLITE_INTERNAL_GLOBALS_H

