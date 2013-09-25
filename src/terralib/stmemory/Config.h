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

  \brief Configuration flags for the ST In-memory driver of TerraLib.
*/

#ifndef __TERRALIB_STMEMORY_INTERNAL_CONFIG_H
#define __TERRALIB_STMEMORY_INTERNAL_CONFIG_H

#include "../Config.h"

/*!
  \def TE_STMEMORY_MODULE_NAME

  \brief The st memory module identifier string.
*/
#define TE_STMEMORY_MODULE_NAME "te.da.stmemory"

/*!
  \def TE_STMEMORY_DRIVER_IDENTIFIER

  \brief The ST In-Memory driver identifier string.
*/
#define TE_STMEMORY_DRIVER_IDENTIFIER "STMEMORY"

/*!
  \brief The maximum number of datasets to be handled by a data source.
*/
#define TE_STMEMORY_DRIVER_MAX_DATASETS 1024


/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_STMEMORY_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib ST memory module.
 */
#define TE_STMEMORY_TEXT_DOMAIN "testmemory"

/*!
  \def TE_STMEMORY_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
 */
#define TE_STMEMORY_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_STMEMORY

  \brief It marks a string in order to get translated. This is a special mark used in the ST memory module of TerraLib.
 */
#define TR_STMEMORY(message) TR(message, TE_STMEMORY_TEXT_DOMAIN)

/*!
  \def TR_PLURAL_ST

  \brief This mark can be used when you have a plural phrase to be translated. This is a special mark used in the ST module of TerraLib. 
 */
#define TR_PLURAL_STMEMORY(message1, message2, n) TR_PLURAL(TE_STMEMORY_TEXT_DOMAIN, message1, message2, n)

//@}

/** @name Logger Defines
 *  Flags for building TerraLib with Log support
 */
//@{

/*!
  \def TE_STMEMORY_LOGGER_NAME
  
  \brief This is the fully qualified TerraLib ST API logger name.
 */
#define TE_STMEMORY_LOGGER_NAME "te.stmemory"

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib ST memory module as a DLL or as a Static Library
 */
//@{

/*!
  \def TESTMEMORYEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib ST memory module as DLL in Windows, remember to insert TESTDLL into the project's list of defines.

  \note If you want to compile TerraLib ST memory module as a Static Library under Windows, remember to insert the TESTSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251  4127  4275)
  #endif

  #ifdef TESTMEMORYSTATIC
    #define TESTMEMORYEXPORT                        // Don't need to export/import... it is a static library
  #elif TESTMEMORYDLL
    #define TESTMEMORYEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TESTMEMORYEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TESTMEMORYEXPORT
#endif

//@}        

#endif  // __TERRALIB_STMEMORY_INTERNAL_CONFIG_H

