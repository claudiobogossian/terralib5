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
  \file Config.h

  \brief Configuration flags for the STLOADER module of TerraLib.
 */

#ifndef __TERRALIB_STLOADER_INTERNAL_CONFIG_H
#define __TERRALIB_STLOADER_INTERNAL_CONFIG_H

#include "../Config.h"

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_STLOADER_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib STLoader module.
 */
#define TE_STLOADER_TEXT_DOMAIN "testloader"

/*!
  \def TE_STLOADER_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
 */
#define TE_STLOADER_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_STLOADER

  \brief It marks a string in order to get translated. This is a special mark used in the STLoader module of TerraLib.
 */
#define TR_STLOADER(message) TR(message, TE_STLOADER_TEXT_DOMAIN)

/*!
  \def TR_PLURAL_STLOADER

  \brief This mark can be used when you have a plural phrase to be translated. This is a special mark used in the STLOADER module of TerraLib. 
 */
#define TR_PLURAL_STLOADER(message1, message2, n) TR_PLURAL(TE_STLOADER_TEXT_DOMAIN, message1, message2, n)

//@}

/** @name Logger Defines
 *  Flags for building TerraLib with Log support
 */
//@{

/*!
  \def TE_STLOADER_LOGGER_NAME
  
  \brief This is the fully qualified TerraLib STLoader API logger name.
 */
#define TE_STLOADER_LOGGER_NAME "te.stloader"

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib STLOADER module as a DLL or as a Static Library
 */
//@{

/*!
  \def TESTLOADEREXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib STLOADER module as DLL in Windows, remember to insert TESTLOADERDLL into the project's list of defines.

  \note If you want to compile TerraLib STLOADER module as a Static Library under Windows, remember to insert the TESTLOADERSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251  4127)
  #endif

  #ifdef TESTLOADERSTATIC
    #define TESTLOADEREXPORT                          // Don't need to export/import... it is a static library
  #elif TESTLOADERDLL
    #define TESTLOADEREXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TESTLOADEREXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TESTLOADEREXPORT
#endif

//@}        

#endif  // __TERRALIB_STLOADER_INTERNAL_CONFIG_H

