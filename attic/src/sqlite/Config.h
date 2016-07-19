/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sqlite/Config.h

  \brief Configuration flags for the SQLite Data Access driver.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_CONFIG_H
#define __TERRALIB_SQLITE_INTERNAL_CONFIG_H

//! The SQLite plugin identifier string.
#define TE_SQLITE_PLUGIN_NAME "te.da.sqlite"

//! The SQLite driver identifier string.
#define TE_SQLITE_DRIVER_IDENTIFIER "SQLITE"

//! This is the default buffer size allocated when reading an SQL script (Warning: this must be a value greater than 2).
#define TE_SQLITE_BUFFER_SCRIPT_SIZE 16777216

//! A flag that indicates a false value (boolean).
#define TE_SQLITE_BOOL_FALSE 0

//! A flag that indicates a false value (boolean).
#define TE_SQLITE_BOOL_TRUE 1

#ifdef _MSC_VER 
    //#pragma warning( disable : 4251 )
    //#pragma warning( disable : 4275 )
#endif

#endif  // __TERRALIB_SQLITE_INTERNAL_CONFIG_H

