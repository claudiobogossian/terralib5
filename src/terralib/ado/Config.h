/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/ado/Config.h

  \brief Configuration flags for the TerraLib ADO Data Access driver.
*/

#ifndef __TERRALIB_ADO_INTERNAL_CONFIG_H
#define __TERRALIB_ADO_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

/** @name General Defines
 *  Configuring ADO support.
 */
//@{

/*!
  \def ADO_DEFAULT_INITIAL_POOL_SIZE
  
  \brief This sets the default initial number of connections opened by a connection pool.
 */
#define ADO_DEFAULT_INITIAL_POOL_SIZE   4

/*!
  \def ADO_DEFAULT_MIN_POOL_SIZE
  
  \brief This sets the default minimum number of connections to be kept in the pool.
 */
#define ADO_DEFAULT_MIN_POOL_SIZE       2

/*!
  \def ADO_DEFAULT_MAX_POOL_SIZE
  
  \brief This sets the default maximum number of connections in the pool.
 */
#define ADO_DEFAULT_MAX_POOL_SIZE       4

/*!
  \def ADO_DEFAULT_MAX_IDLE_TIME
  
  \brief This sets the default maximum time that a connection can be in the pool without being used.
 */
#define ADO_DEFAULT_MAX_IDLE_TIME       10

/*!
  \def ADO_DEFAULT_PORT

  \brief The default port used to connect to a PostgreSQL database server.
 */
#define ADO_DEFAULT_PORT                "5432"

/*!
  \def ADO_DEFAULT_CONNECT_TIMEOUT
  
  \brief The default maximum wait for connection, in seconds.
 */
#define ADO_DEFAULT_CONNECT_TIMEOUT     "4"

/*!
  \def ADO_DEFAULT_CLIENT_ENCODING
  
  \brief The default client encoding when talking with the database.
 */
#define ADO_DEFAULT_CLIENT_ENCODING     "UTF8"

/*!
  \def ADO_DRIVER_IDENTIFIER

  \brief The ADO driver identifier string.
 */
#define ADO_DRIVER_IDENTIFIER "ADO"

/*!
  \def ADO_PLUGIN_NAME

  \brief The ADO plugin identifier string.
*/
#define ADO_PLUGIN_NAME "te.da.ado"

//@}


/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEADOEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEADODLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEADOSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    //#pragma warning( disable : 4273 )
    #pragma warning( disable : 4275 )
  #endif

  #ifdef TEADOSTATIC
    #define TEADOEXPORT                          // Don't need to export/import... it is a static library
  #elif TEADODLL
    #define TEADOEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEADOEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEADOEXPORT
#endif

//@}        

#endif  // __TERRALIB_ADO_INTERNAL_CONFIG_H

