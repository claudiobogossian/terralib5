/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/wfs/Config.h

  \brief Configuration flags for the TerraLib WFS module.
*/

#ifndef __TERRALIB_WFS_INTERNAL_CONFIG_H
#define __TERRALIB_WFS_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_WFS_MODULE_NAME "te.wfs"

/*!
  \def TE_WFS_DRIVER_IDENTIFIER

  \brief The WFS driver identifier string.
*/
#define TE_WFS_DRIVER_IDENTIFIER "WFS"

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_WFS_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib WFS module.
*/
#define TE_WFS_TEXT_DOMAIN "tewfs"

/*!
  \def TE_WFS_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
*/
#define TE_WFS_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_WFS

  \brief It marks a string in order to get translated. This is a special mark used in the TerraLib WFS module.
*/
#define TR_WFS(message) TR(message, TE_WFS_TEXT_DOMAIN)

/*!
  \def TR_WFS_PLURAL

  \brief This mark can be used when you have a plural phrase to be translated. This is a special mark used in the TerraLib WFS module.
*/
#define TR_WFS_PLURAL(message1, message2, n) TR_PLURAL(TE_WFS_TEXT_DOMAIN, message1, message2, n)

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEWFSEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEWFSDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEWFSSTATIC flag into the project list of defines.
*/

#ifdef WIN32

  #ifdef _MSC_VER
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
  #endif

  #ifdef TEWFSSTATIC
    #define TEWFSEXPORT                          // Don't need to export/import... it is a static library
  #elif TEWFSDLL
    #define TEWFSEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEWFSEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEWFSEXPORT
#endif

//@}

#endif  // __TERRALIB_WFS_INTERNAL_CONFIG_H
