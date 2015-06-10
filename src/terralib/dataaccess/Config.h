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
  \file terralib/dataaccess/Config.h

  \brief Configuration flags for the Data Access module of TerraLib.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_CONFIG_H
#define __TERRALIB_DATAACCESS_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

/** @name General Defines for the DataAccess module
 *  Defines for configuring DataAccess module.
 */
//@{

/*!
  \def TE_DATAACCESS_MODULE_NAME
  
  \brief An identifier for this module.
*/
#define TE_DATAACCESS_MODULE_NAME "te.dataaccess"

/*!
  \def TERRALIB_USE_CONNECTION_POOL_MANAGER
  
  \brief Set this macro to 1 if you want to use the TerraLib connection pool manager.
*/
#define TERRALIB_USE_CONNECTION_POOL_MANAGER 1

/*!
  \def TERRALIB_AUTOMATICALLY_INITIALIZE_CONNECTION_POOL_MANAGER
  
  \brief Set this macro to 1 if you want TerraLib to automatically initialize a thread for managing all the connection pools registered in the system.
*/
#if TERRALIB_USE_CONNECTION_POOL_MANAGER
  #define TERRALIB_AUTOMATICALLY_INITIALIZE_CONNECTION_POOL_MANAGER 1
#else
  #define TERRALIB_AUTOMATICALLY_INITIALIZE_CONNECTION_POOL_MANAGER 0
#endif

/*!
  \def TERRALIB_POOL_DEFAULT_MONITORING_TIME
  
  \brief This sets the default monitoring time in seconds for the connection pool manager.
*/
#define TERRALIB_POOL_DEFAULT_MONITORING_TIME 60

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEDATAACCESSEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEDATAACCESSDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEDATAACCESSSTATIC flag into the project list of defines.
*/
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 4127)
    #pragma warning( disable : 4275 )
  #endif

  #ifdef TEDATAACCESSSTATIC
    #define TEDATAACCESSEXPORT                          // Don't need to export/import... it is a static library
  #elif TEDATAACCESSDLL
    #define TEDATAACCESSEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEDATAACCESSEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEDATAACCESSEXPORT
#endif

//@}        

#endif  // __TERRALIB_DATAACCESS_INTERNAL_CONFIG_H
