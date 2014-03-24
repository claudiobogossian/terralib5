/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/common/Config.h

  \brief Configuration flags for the TerraLib Common Runtime module.

  \todo We need to move some configuration variables to build system.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_CONFIG_H
#define __TERRALIB_COMMON_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_COMMON_MODULE_NAME "te.common"

/** @name File Defines
 *  Flags for TerraLib special files name and location.
 */
//@{

/*!
  \def TERRALIB_CONFIG_DIR

  \brief Folder location with application config files.
*/
#ifndef TERRALIB_CONFIG_DIR
  #define TERRALIB_CONFIG_DIR    "../../conf"
#endif

/*!
  \def TERRALIB_CONFIG_DIR

  \brief Main application config file.
*/
#ifndef TERRALIB_SYSTEM_SETTINGS_FILE
  #define TERRALIB_SYSTEM_SETTINGS_FILE       "config.xml"
#endif

/*!
  \def TERRALIB_USER_SETTINGS_FILE

  \brief User settings file name
*/
#ifndef TERRALIB_USER_SETTINGS_FILE
  #define TERRALIB_USER_SETTINGS_FILE         "user_settings.xml"
#endif

//@}

#define TE_PROTOCOL_DEFAULT_PORTS_FILE "protocol/protocolPorts.txt"

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TECOMMONEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TECOMMONDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TECOCOMMONSTATIC flag into the project list of defines.
*/
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
  #endif

  #ifdef TECOMMONSTATIC
    #define TECOMMONEXPORT                          // Don't need to export/import... it is a static library
  #elif TECOMMONDLL
    #define TECOMMONEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TECOMMONEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TECOMMONEXPORT
#endif

//@}

#endif  // __TERRALIB_COMMON_INTERNAL_CONFIG_H

