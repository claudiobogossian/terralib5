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
  \file terralib/xml/Config.h

  \brief Configuration flags for the XML API of TerraLib.
*/

#ifndef __TERRALIB_XML_INTERNAL_CONFIG_H
#define __TERRALIB_XML_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_XML_MODULE_NAME "te.xml"

/** @name XML Module Config
 *  Configuration flags for XML module.
 */
//@{

/*!
  \def TE_XML_DEFAULT_READER_TYPE
  
  \brief The default implementation to be used when creating a XML reader.
 */
#define TE_XML_DEFAULT_READER_TYPE "XERCES"

/*!
  \def TE_XML_READER_MAX_BUFFSIZE
  
  \brief It defines the default internal buffer size for parsing a XML file.
 */
#define TE_XML_READER_MAX_BUFFSIZE 65536

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEXMLEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEXMLDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEXMLSTATIC flag into the project list of defines.
 */
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
  #endif

  #ifdef TEXMLSTATIC
    #define TEXMLEXPORT                          // Don't need to export/import... it is a static library
  #elif TEXMLDLL
    #define TEXMLEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEXMLEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEXMLEXPORT
#endif

//@}        

#endif  // __TERRALIB_XML_INTERNAL_CONFIG_H

