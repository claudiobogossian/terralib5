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
  \file terralib/qt/af/Config.h

  \brief Configuration flags for the TerraLib Application Framework.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_CONFIG_H
#define __TERRALIB_QT_AF_INTERNAL_CONFIG_H

// TerraLib
#include "../../Config.h"

/*!
  \def TERRALIB_APPLICATION_CONFIG_FILE

  \brief The file that contains the application plugins.
*/
#define TERRALIB_APPLICATION_CONFIG_FILE   "share/terraview/config/config.xml"

/*!
 \def TERRALIB_APPLICATION_DATASOURCE_FILE_NAME
 
 \brief The default name for the application file containing the list of data sources.
 */
#define TERRALIB_APPLICATION_DATASOURCE_FILE_NAME   "datasources.xml"


/*!
  \def TE_QT_AF_MODULE_NAME

  \brief The application framework module name.
*/
#define TE_QT_AF_MODULE_NAME                  "te.qt.af"

/** @name DLL/LIB Module
 *  Flags for building TerraLib Application Framework as a DLL or as a Static Library
 */
//@{

/*!
  \def TEQTAFEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib Application Framework as DLL in Windows, remember to insert TEQTAFDLL into the project's list of defines.

  \note If you want to compile TerraLib Application Framework as an Static Library under Windows, remember to insert the TEQTAFSTATIC flag into the project list of defines.
*/
#ifdef WIN32
  #pragma warning( disable : 4251 )
  #pragma warning( disable : 4275 )
  #pragma warning( disable : 4290 )

  #ifdef TEQTAFSTATIC
    #define TEQTAFEXPORT                          // Don't need to export/import... it is a static library
  #elif TEQTAFDLL
    #define TEQTAFEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEQTAFEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEQTAFEXPORT
#endif

//@}

#endif  // __TERRALIB_QT_AF_INTERNAL_CONFIG_H

