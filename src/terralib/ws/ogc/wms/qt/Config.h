/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
   \file terralib/ws/ogc/wms/qtplugin/Config.h

   \brief Configuration flags for the TerraLib WMS QT module.

   \author Emerson Moraes
 */

 #ifndef __TERRALIB_WS_OGC_WMS_QT_CONFIG_H
 #define __TERRALIB_WS_OGC_WMS_QT_CONFIG_H

// TerraLib
#include "../../../../Defines.h"

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEOGCWMSQTPLUGINEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEOGCWMSQTDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEOGCWMSQTSTATIC flag into the project list of defines.
*/

#ifdef WIN32

  #ifdef _MSC_VER
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
  #endif

  #ifdef TEOGCWMSQTSTATIC
    #define TEOGCWMSQTEXPORT                          // Don't need to export/import... it is a static library
  #elif TEOGCWMSQTDLL
    #define TEOGCWMSQTEXPORT  TE_DLL_EXPORT   // export DLL information
  #else
    #define TEOGCWMSQTEXPORT  TE_DLL_IMPORT   // import DLL information
  #endif
#else
  #define TEOGCWMSQTEXPORT
#endif

//@}

#endif //__TERRALIB_WS_OGC_WMS_QTPLUGIN_CONFIG_H
