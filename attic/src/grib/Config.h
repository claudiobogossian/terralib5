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
  \file terralib/grib/Config.h

  \brief Configuration flags for GRIB.
*/

#ifndef __TERRALIB_GRIB_INTERNAL_CONFIG_H
#define __TERRALIB_GRIB_INTERNAL_CONFIG_H

#include "../Config.h"

/** @name General Defines
 *  Configuring MyDriver support.
 */
//@{

/*!
  \def GRIB_DRIVER_IDENTIFIER

  \brief The MyDriver driver identifier string.
 */
#define GRIB_DRIVER_IDENTIFIER "GRIB"

//@}        

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_GRIB_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib MyDriver driver implementation.
 */
#define TE_GRIB_TEXT_DOMAIN "temydriver"

/*!
  \def TE_GRIB_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
 */
#define TE_GRIB_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_GRIB

  \brief It marks a string in order to get translated. This is a special mark used in the DataAccess module of TerraLib.
 */
#define TR_GRIB(message) TR(message, TE_GRIB_TEXT_DOMAIN)

/*!
  \def TR_PLURAL_GRIB

  \brief This mark can be used when you have a plural phrase to be translated. This is a special mark used in the MyDriver module of TerraLib. 
 */
#define TR_PLURAL_GRIB(message1, message2, n) TR_PLURAL(TE_GRIB_TEXT_DOMAIN, message1, message2, n)

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEGRIBEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEGRIBDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEGRIBSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    //#pragma warning( disable : 4273 )
    #pragma warning( disable : 4275 )
  #endif

  #ifdef TEGRIBSTATIC
    #define TEGRIBEXPORT                          // Don't need to export/import... it is a static library
  #elif TEGRIBDLL
    #define TEGRIBEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEGRIBEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEGRIBEXPORT
#endif

//@}        

#endif  // __TERRALIB_GRIB_INTERNAL_CONFIG_H

