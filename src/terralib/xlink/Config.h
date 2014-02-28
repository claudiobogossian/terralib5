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
  
  \brief This file contains several defines and configuration parameters used in TerraLib XLink package.
  
  
 */

#ifndef __TERRALIB_XLINK_INTERNAL_CONFIG_H
#define __TERRALIB_XLINK_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_XL_MODULE_NAME "te.xlinks"

# define TE_ENABLED_XML 1

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

/*!
  \def TE_XLINK_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib XLink module.
 */
#define TE_XLINK_TEXT_DOMAIN "texlink"

/*!
  \def TE_XLINK_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
 */
#define TE_XLINK_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_XLINK

  \brief It marks a string in order to get translated. This is a special mark used in the Color module of TerraLib. 
 */
#define TR_XLINK(message) TR(message, TE_XLINK_TEXT_DOMAIN)

/*!
  \def TR_PLURAL_XLINK

  \brief This mark can be used when you have a plural phrase to be translated. This is a special mark used in the XLink module of TerraLib. 
 */
#define TR_PLURAL_XLINK(message1, message2, n) TR_PLURAL(TE_XLINK_TEXT_DOMAIN, message1, message2, n)

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEXLEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TLXLINKDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEXLINKSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
  #endif

  #ifdef TEXLINKSTATIC
    #define TEXLEXPORT                          // Don't need to export/import... it is a static library
  #elif TEXLDLL
    #define TEXLEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEXLEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEXLEXPORT
#endif

//@}   

#endif  // __TERRALIB_XLINK_INTERNAL_CONFIG_H

