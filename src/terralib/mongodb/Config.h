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
  \file terralib/mongodb/Config.h

  \brief Configuration flags for the TerraLib MongoDB Data Access driver.
*/

#ifndef __TERRALIB_MONGODB_INTERNAL_CONFIG_H
#define __TERRALIB_MONGODB_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

/** @name General Defines
 *  Configuring MongoDB driver support.
 */
//@{

#define MONGODB_DEFAULT_URI "localhost"

/*!
  \def MONGODB_DEFAULT_INITIAL_POOL_SIZE
  
  \brief This sets the default initial number of connections opened by a connection pool.
*/
#define MONGODB_DEFAULT_INITIAL_POOL_SIZE   4

/*!
  \def MONGODB_DEFAULT_MIN_POOL_SIZE
  
  \brief This sets the default minimum number of connections to be kept in the pool.
*/
#define MONGODB_DEFAULT_MIN_POOL_SIZE       2

/*!
  \def MONGODB_DEFAULT_MAX_POOL_SIZE
  
  \brief This sets the default maximum number of connections in the pool.
*/
#define MONGODB_DEFAULT_MAX_POOL_SIZE       4

/*!
  \def MONGODB_DEFAULT_MAX_IDLE_TIME
  
  \brief This sets the default maximum time that a connection can be in the pool without being used.
*/
#define MONGODB_DEFAULT_MAX_IDLE_TIME       10

/*!
  \def MONGODB_DEFAULT_PORT

  \brief The default port used to connect to a PostgreSQL database server.
*/
#define MONGODB_DEFAULT_PORT                "27107"

/*!
  \def MONGODB_DEFAULT_CONNECT_TIMEOUT
  
  \brief The default maximum wait for connection, in seconds.
*/
#define MONGODB_DEFAULT_CONNECT_TIMEOUT     "4"

/*!
  \def MONGODB_DEFAULT_CLIENT_ENCODING
  
  \brief The default client encoding when talking with the database.
*/
#define MONGODB_DEFAULT_CLIENT_ENCODING     "UTF8"

/*!
  \def MONGODB_DRIVER_IDENTIFIER

  \brief The MongoDB driver identifier string.
*/
#define MONGODB_DRIVER_IDENTIFIER "MONGODB"

/*!
  \def MONGODB_PLUGIN_NAME

  \brief The MongoDB plugin identifier string.
*/
#define MONGODB_PLUGIN_NAME "te.da.mongodb"

//@}

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_MONGODB_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib MongoDB driver implementation.
*/
#define TE_MONGODB_TEXT_DOMAIN "temongodb"

/*!
  \def TE_MONGODB_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
*/
#define TE_MONGODB_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_MONGODB

  \brief It marks a string in order to get translated. This is a special mark used in the DataAccess module of TerraLib.
*/
#define TR_MONGODB(message) TR(message, TE_MONGODB_TEXT_DOMAIN)

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEMONGODBEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEMONGODBDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEMONGODBSTATIC flag into the project list of defines.
*/
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4273 )
    #pragma warning( disable : 4275 )
  #endif

  #ifdef TEMONGODBSTATIC
    #define TEMONGODBEXPORT                          // Don't need to export/import... it is a static library
  #elif TEMONGODBDLL
    #define TEMONGODBEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEMONGODBEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEMONGODBEXPORT
#endif

//@}        

#endif  // __TERRALIB_MONGODB_INTERNAL_CONFIG_H

