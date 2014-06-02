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

  \brief Configuration flags for the TerraLib Xerces Module.
 */

#ifndef __TERRALIB_XERCES_INTERNAL_CONFIG_H
#define __TERRALIB_XERCES_INTERNAL_CONFIG_H

// TerraLib
#include "../xml/Config.h"

#define TE_XERCES_MODULE_NAME "TEXERCES"

/** @name XERCES Module Config
 *  Configuration flags for XERCES module.
 */
//@{

/*!
  \def TE_XERCES_READER_MAX_BUFFSIZE
  
  \brief It defines the default internal buffer size for parsing a XERCES file.
 */
#define TE_XERCES_READER_MAX_BUFFSIZE TE_XML_READER_MAX_BUFFSIZE

/*!
  \def XERCES_DRIVER_IDENTIFIER

  \brief The name of this XML reader driver.
 */
#define XERCES_DRIVER_IDENTIFIER "XERCES"

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEXERCESEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEXERCESDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEXERCESSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
  #endif

  #ifdef TEXERCESSTATIC
    #define TEXERCESEXPORT                          // Don't need to export/import... it is a static library
  #elif TEXERCESDLL
    #define TEXERCESEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEXERCESEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEXERCESEXPORT
#endif

//@}        

#endif  // __TERRALIB_XERCES_INTERNAL_CONFIG_H

