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
  \file terralib/terralib4/Config.h

  \brief Configuration flags for the TerraLib 4 driver.
*/

#ifndef __TERRALIB_TERRALIB4_INTERNAL_CONFIG_H
#define __TERRALIB_TERRALIB4_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

/** @name General Defines
 *  Configuring Terralib 4 support.
 */
//@{

/*!
  \def TERRALIB4_DEFAULT_INITIAL_POOL_SIZE
  
  \brief This sets the default initial number of connections opened by a connection pool.
 */
#define TERRALIB4_DEFAULT_INITIAL_POOL_SIZE   4

/*!
  \def TERRALIB4_DEFAULT_MIN_POOL_SIZE
  
  \brief This sets the default minimum number of connections to be kept in the pool.
 */
#define TERRALIB4_DEFAULT_MIN_POOL_SIZE       2

/*!
  \def TERRALIB4_DEFAULT_MAX_POOL_SIZE
  
  \brief This sets the default maximum number of connections in the pool.
 */
#define TERRALIB4_DEFAULT_MAX_POOL_SIZE       4

/*!
  \def TERRALIB4_DEFAULT_MAX_IDLE_TIME
  
  \brief This sets the default maximum time that a connection can be in the pool without being used.
 */
#define TERRALIB4_DEFAULT_MAX_IDLE_TIME       10

/*!
  \def TERRALIB4_DEFAULT_CONNECT_TIMEOUT
  
  \brief The default maximum wait for connection, in seconds.
 */
#define TERRALIB4_DEFAULT_CONNECT_TIMEOUT     "4"

/*!
  \def TERRALIB4_DEFAULT_CLIENT_ENCODING
  
  \brief The default client encoding when talking with the database.
 */
#define TERRALIB4_DEFAULT_CLIENT_ENCODING     "UTF8"

/*!
  \def TERRALIB4_DRIVER_IDENTIFIER

  \brief The Terralib 4 driver identifier string.
 */
#define TERRALIB4_DRIVER_IDENTIFIER "TERRALIB4"

/*!
  \def TERRALIB4_PLUGIN_NAME

  \brief The ADO plugin identifier string.
*/
#define TERRALIB4_PLUGIN_NAME "te.da.terralib4"

//@}


/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TETERRALIB4EXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TETERRALIB4DLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEADOSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    //#pragma warning( disable : 4273 )
    #pragma warning( disable : 4275 )
  #endif

  #ifdef TETERRALIB4STATIC
    #define TETERRALIB4EXPORT                          // Don't need to export/import... it is a static library
  #elif TETERRALIB4DLL
    #define TETERRALIB4EXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TETERRALIB4EXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TETERRALIB4EXPORT
#endif

//@}        

#endif  // __TERRALIB_TERRALIB4_INTERNAL_CONFIG_H

