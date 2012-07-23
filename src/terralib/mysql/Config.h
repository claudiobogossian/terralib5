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
  \file terralib/mysql/Config.h

  \brief Configuration flags for MySQL Data Access driver.
*/

#ifndef __TERRALIB_MYSQL_INTERNAL_CONFIG_H
#define __TERRALIB_MYSQL_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

/** @name General Defines
 *  Configuring MySQL support.
 */
//@{


/*!
  \def TE_MYSQL_PLUGIN_NAME

  \brief The MySQL plugin identifier string.
*/
#define TE_MYSQL_PLUGIN_NAME "te.da.mysql"

/*!
  \def TE_MYSQL_BUFFER_SCRIPT_SIZE
  
  \brief This sets the default buffer size when reading a file and executing commands.
*/
#define TE_MYSQL_BUFFER_SCRIPT_SIZE 64*1024

/*!
  \def MYSQL_DEFAULT_INITIAL_POOL_SIZE
  
  \brief This sets the default initial number of connections opened by a connection pool.
*/
#define MYSQL_DEFAULT_INITIAL_POOL_SIZE   1

/*!
  \def MYSQL_DEFAULT_MIN_POOL_SIZE
  
  \brief This sets the default minimum number of connections to be kept in the pool.
*/
#define MYSQL_DEFAULT_MIN_POOL_SIZE       1

/*!
  \def MYSQL_DEFAULT_MAX_POOL_SIZE
  
  \brief This sets the default maximum number of connections in the pool.
*/
#define MYSQL_DEFAULT_MAX_POOL_SIZE       1

/*!
  \def MYSQL_DEFAULT_MAX_IDLE_TIME
  
  \brief This sets the default maximum time that a connection can be in the pool without being used.
*/
#define MYSQL_DEFAULT_MAX_IDLE_TIME       60

/*!
  \def MYSQL_DEFAULT_PORT

  \brief The default port used to connect to a MySQL database server.
*/
#define MYSQL_DEFAULT_PORT                3306

/*!
  \def MYSQL_DEFAULT_HOST

  \brief The default host used to connect to a MySQL database server.
*/
#define MYSQL_DEFAULT_HOST                "localhost"

/*!
  \def MYSQL_DEFAULT_CONNECT_TIMEOUT
  
  \brief The default maximum wait for connection, in seconds.
*/
#define MYSQL_DEFAULT_CONNECT_TIMEOUT     4

/*!
  \def MYSQL_DEFAULT_CLIENT_ENCODING
  
  \brief The default client encoding when talking with the database.
*/
#define MYSQL_DEFAULT_CLIENT_ENCODING     "UTF8"

/*!
  \def TE_MYSQL_DRIVER_IDENTIFIER

  \brief The MySQL data access driver identifier string.
*/
#define TE_MYSQL_DRIVER_IDENTIFIER "MYSQL"

/*!
  \def TE_MYSQL_RASTER_TABLE_IDENTIFIER

  \brief The MySQL data access driver identifier string.
*/
#define TE_MYSQL_RASTER_TABLE_IDENTIFIER "MY_RASTER_TABLE"

/*!
  \def TE_MYSQL_TILED_RASTER_IDENTIFIER

  \brief The MySQL data access driver identifier string.
*/
#define TE_MYSQL_TILED_RASTER_IDENTIFIER "MY_TILED_RASTER"

//@}        

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_MYSQL_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib MySQL data access driver implementation.
*/
#define TE_MYSQL_TEXT_DOMAIN "temysql"

/*!
  \def TE_MYSQL_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
*/
#define TE_MYSQL_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_MYSQL

  \brief It marks a string in order to get translated. This is a special mark used in the DataAccess module of TerraLib.
*/
#define TR_MYSQL(message) TR(message, TE_MYSQL_TEXT_DOMAIN)

/*!
  \def TR_PLURAL_MYSQL

  \brief This mark can be used when you have a plural phrase to be translated. This is a special mark used in the MySQL data access driver module of TerraLib. 
*/
#define TR_PLURAL_MYSQL(message1, message2, n) TR_PLURAL(TE_MYSQL_TEXT_DOMAIN, message1, message2, n)

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEMYSQLEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEMYSQLDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEMYSQLSTATIC flag into the project list of defines.
*/
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    //#pragma warning( disable : 4273 )
    #pragma warning( disable : 4275 )
  #endif

  #ifdef TEMYSQLSTATIC
    #define TEMYSQLEXPORT                          // Don't need to export/import... it is a static library
  #elif TEMYSQLDLL
    #define TEMYSQLEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEMYSQLEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEMYSQLEXPORT
#endif

//@}        

#endif  // __TERRALIB_MYSQL_INTERNAL_CONFIG_H

