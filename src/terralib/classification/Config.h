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
  \file terralib/classification/core/Config.h

  \brief Configuration flags for the Terrralib Classification module.
 */

#ifndef __TERRALIB_CLASSIFICATION_CORE_INTERNAL_CONFIG_H
#define __TERRALIB_CLASSIFICATION_CORE_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_CL_MODULE_NAME "te.classification"

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_CL_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in Terrralib Classification module.
 */
#define TE_CL_TEXT_DOMAIN "teclassification"

/*!
  \def TE_CL_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
 */
#define TE_CL_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_CL

  \brief It marks a string in order to get translated. This is a special mark used in the Terrralib Classification module.
 */
#define TR_CL(message) TR(message, TE_CL_TEXT_DOMAIN)

/*!
  \def TR_PLURAL_CLASSIFICATION

  \brief This mark can be used when you have a plural phrase to be translated. This is a special mark used in the Classification module of Terrralib.
 */
#define TR_PLURAL_CLASSIFICATION(message1, message2, n) TR_PLURAL(TE_CLASSIFICATION_TEXT_DOMAIN, message1, message2, n)

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TECLEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TECLDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TECLSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER
    #pragma warning( disable : 4251 )
    //#pragma warning( disable : 4275 )
  #endif

  #ifdef TECLSTATIC
    #define TECLEXPORT                           // Don't need to export/import... it is a static library
  #elif TECLDLL
    #define TECLEXPORT  __declspec(dllexport)    // export DLL information
  #else
    #define TECLEXPORT  __declspec(dllimport)    // import DLL information
  #endif 
#else
  #define TECLEXPORT
#endif

//@}

#endif  // __TERRALIB_CLASSIFICATION_CORE_INTERNAL_CONFIG_H

