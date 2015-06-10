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
  \file terralib/addressgeocoding/Config.h

  \brief Configuration flags for the Terrralib Address Geocoding module.
 */

#ifndef __TERRALIB_ADDRESSGEOCODING_CORE_INTERNAL_CONFIG_H
#define __TERRALIB_ADDRESSGEOCODING_CORE_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_ADDRESSGEOCODING_MODULE_NAME "te.addressgeocoding"

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEADDRESSGEOCODINGEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEADDRESSGEOCODINGDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEADDRESSGEOCODINGSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
  #endif


  #ifdef TEADDRESSGEOCODINGSTATIC
    #define TEADDRESSGEOCODINGEXPORT                          // Don't need to export/import... it is a static library
  #elif TEADDRESSGEOCODINGDLL
    #define TEADDRESSGEOCODINGEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEADDRESSGEOCODINGEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEADDRESSGEOCODINGEXPORT
#endif

//@}

#endif  // __TERRALIB_ADDRESSGEOCODING_CORE_INTERNAL_CONFIG_H

