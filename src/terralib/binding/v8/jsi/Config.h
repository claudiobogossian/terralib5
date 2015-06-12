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
  \file Config.h

  \brief Configuration flags for TerraLib's Google JavaScript V8 engine module.
 */

#ifndef __TERRALIB_BINDING_V8_JSI_INTERNAL_CONFIG_H
#define __TERRALIB_BINDING_V8_JSI_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_V8_JSI_MODULE_NAME "TEBINDINGV8JSI"

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_V8JSI_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib JavaScript API module.
 */
#define TE_V8JSI_TEXT_DOMAIN "tev8jsi"

/*!
  \def GTE_V8JSI_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
 */
#define TE_V8JSI_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_V8JSI

  \brief It marks a string in order to get translated. This is a special mark used in the Vector Geometry module of TerraLib. 
 */
#define TR_V8JSI(message) TR(message, TE_V8JSI_TEXT_DOMAIN)

/*!
  \def TR_V8JSI_PLURAL

  \brief It marks a string in order to get translated according to plural form. This is the mark used in the Common module of TerraLib. 
 */
#define TR_V8JSI_PLURAL(message1, message2, n) TR_PLURAL(TE_V8JSI_TEXT_DOMAIN, message1, message2, n)

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEV8JSIEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEV8JSIDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEV8JSISTATIC flag into the project list of defines.
 */

#ifdef WIN32

  #ifdef _MSC_VER
    #pragma warning( disable : 4251 )
  #endif

  #ifdef TEV8JSISTATIC
    #define TEV8JSIEXPORT                          // Don't need to export/import... it is a static library
  #elif TEV8JSIDLL
    #define TEV8JSIEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEV8JSIEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEV8JSIEXPORT
#endif

//@}        

#endif  // __TERRALIB_BINDING_V8_JSI_INTERNAL_CONFIG_H

