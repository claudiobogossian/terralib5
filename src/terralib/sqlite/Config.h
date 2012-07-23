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
  \file terralib/sqlite/Config.h

  \brief Configuration flags for the SQLite Data Access driver.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_CONFIG_H
#define __TERRALIB_SQLITE_INTERNAL_CONFIG_H

/** @name General Defines
 *  Configuring SQLite support.
 */
//@{

/*!
  \def TE_SQLITE_PLUGIN_NAME

  \brief The SQLite plugin identifier string.
*/
#define TE_SQLITE_PLUGIN_NAME "te.da.sqlite"

/*!
  \def TE_SQLITE_DRIVER_IDENTIFIER

  \brief The SQLite driver identifier string.
*/
#define TE_SQLITE_DRIVER_IDENTIFIER "SQLITE"

/*!
  \def TE_RASTERLITE_DRIVER_IDENTIFIER

  \brief The RasterLite driver identifier string.
*/
#define TE_RASTERLITE_DRIVER_IDENTIFIER "RASTERLITE"

/*!
  \def TE_TERRALIBLITE_DRIVER_IDENTIFIER

  \brief The TerraLib driver for SQL identifier string.
*/
#define TE_TERRALIBLITE_DRIVER_IDENTIFIER "TERRALIBLITE"

/*!
  \def TE_SQLITE_BUFFER_SCRIPT_SIZE

  \brief This is the default buffer size allocated when reading an SQL script.

  \warning This must be a value greater than 2.
*/
#define TE_SQLITE_BUFFER_SCRIPT_SIZE 2048

/*!
  \def TE_SQLITE_IS_NOT_RASTER_TABLE

  \brief A flag that indicates that a table is not a raster table.
*/
#define TE_SQLITE_IS_NOT_RASTER_TABLE 0

/*!
  \def TE_SQLITE_IS_RASTER_METADATA_TABLE

  \brief A flag that indicates that a table is a raster metadata table.
*/
#define TE_SQLITE_IS_RASTER_METADATA_TABLE 1

/*!
  \def TE_SQLITE_IS_RASTER_METADATA_TABLE

  \brief A flag that indicates that a table is a raster table.
*/
#define TE_SQLITE_IS_RASTER_TABLE 2

/*!
  \def TE_SQLITE_BOOL_FALSE

  \brief A flag that indicates a false value (boolean).
*/
#define TE_SQLITE_BOOL_FALSE 0

/*!
  \def TE_SQLITE_BOOL_TRUE

  \brief A flag that indicates a false value (boolean).
*/
#define TE_SQLITE_BOOL_TRUE 1

//@}        

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_SQLITE_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib SQLite driver implementation.
*/
#define TE_SQLITE_TEXT_DOMAIN "tesqlite"

/*!
  \def TE_SQLITE_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
*/
#define TE_SQLITE_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_SQLITE

  \brief It marks a string in order to get translated. This is a special mark used in the DataAccess module of TerraLib.
*/
#define TR_SQLITE(message) TR(message, TE_SQLITE_TEXT_DOMAIN)

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TESQLITEEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TESQLITEDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TESQLITESTATIC flag into the project list of defines.
*/
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
  #endif

  #ifdef TESQLITESTATIC
    #define TESQLITEEXPORT                          // Don't need to export/import... it is a static library
  #elif TESQLITEDLL
    #define TESQLITEEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TESQLITEEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TESQLITEEXPORT
#endif

//@}        

#endif  // __TERRALIB_SQLITE_INTERNAL_CONFIG_H

