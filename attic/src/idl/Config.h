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
  \file terralib/idl/Config.h

  \brief Configuration flags for the IDL module of TerraLib.
 */

#ifndef __TERRALIB_IDL_INTERNAL_CONFIG_H
#define __TERRALIB_IDL_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_IDL_MODULE_NAME "te.idl"

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_IDL_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib IDL module.
 */
#define TE_IDL_TEXT_DOMAIN "teidl"

/*!
  \def TE_IDL_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
 */
#define TE_IDL_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_IDL

  \brief It marks a string in order to get translated. This is a special mark used in IDL module of TerraLib.
 */
#define TR_IDL(message) TR(message, TE_IDL_TEXT_DOMAIN)

//@}

/** @name DLL Module
 *  Flags for building this module as a DLL
 */
//@{

/*!
  \def TEIDLEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.
 */
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS

  #if TE_COMPILER == TE_COMPILERCODE_VCPLUSPLUS
    #pragma warning( disable : 4251 )
  #endif

  #ifdef TEIDLDLL
    #define TEIDLEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEIDLEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEIDLEXPORT
#endif

//@}        

#endif  // __TERRALIB_IDL_INTERNAL_CONFIG_H

