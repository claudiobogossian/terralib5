/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/plugins/processing/Config.h

  \brief Configuration flags for the processing Qt Widget plugin.
*/

#ifndef __TERRALIB_QT_PLUGINS_PROCESSING_INTERNAL_CONFIG_H
#define __TERRALIB_QT_PLUGINS_PROCESSING_INTERNAL_CONFIG_H

// TerraLib
#include "../../../Config.h"

/*!
  \def TE_QT_PLUGIN_PROCESSING_PLUGIN_NAME

  \brief It contains the plugin name.
*/
#define TE_QT_PLUGIN_PROCESSING_PLUGIN_NAME "te.qt.processing"

/*!
  \def TEQTPLUGINPROCESSINGEXPORT

  \brief You can use this macro in order to export/import classes and functions from all plug-ins files.

  \note To compile plug-ins in Windows, remember to insert TEQTPLUGINPROCESSINGEXPORT into the project's list of defines.
 */
#ifdef WIN32
  #ifdef TEQTPLUGINPROCESSINGDLL
    #define TEQTPLUGINPROCESSINGEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEQTPLUGINPROCESSINGEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEQTPLUGINPROCESSINGEXPORT
#endif

#endif  // __TERRALIB_QT_PLUGINS_PROCESSING_INTERNAL_CONFIG_H

