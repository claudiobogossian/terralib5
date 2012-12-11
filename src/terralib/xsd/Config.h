/*  Copyright (C) 2010-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/xsd/Config.h

  \brief Configuration flags for the XSD Module of TerraLib.  
*/

#ifndef __TERRALIB_XSD_INTERNAL_CONFIG_H
#define __TERRALIB_XSD_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_XSD_BUILD_WITH_XML_PARSER 1

#define TE_XSD_MODULE_NAME "te.xsd"

/*!
  \def TE_XSD_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib PostGIS driver implementation.
*/
#define TE_XSD_TEXT_DOMAIN "texsd"

/*!
  \def TE_XSD_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
*/
#define TE_XSD_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_XSD

  \brief It marks a string in order to get translated. This is a special mark used in the DataAccess module of TerraLib.
*/
#define TR_XSD(message) TR(message, TE_XSD_TEXT_DOMAIN)

/*!
  \def TR_PLURAL_XSD

  \brief This mark can be used when you have a plural phrase to be translated. This is a special mark used in the DataAccess module of TerraLib. 
*/
#define TR_PLURAL_XSD(message1, message2, n) TR_PLURAL(TE_XSD_TEXT_DOMAIN, message1, message2, n)

/*!
  \def TEXSDEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEXSDDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEXSDSTATIC flag into the project list of defines.
*/
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
  #endif

  #ifdef TEXSDSTATIC
    #define TEXSDEXPORT                          // Don't need to export/import... it is a static library
  #elif TEXSDDLL
    #define TEXSDEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEXSDEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEXSDEXPORT
#endif

#endif  // __TERRALIB_XSD_INTERNAL_CONFIG_H
