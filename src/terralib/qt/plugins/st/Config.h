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
  \file terralib/qt/plugins/st/Config.h

  \brief Configuration flags for the ST Qt Widget plugin.
*/

#ifndef __TERRALIB_QT_PLUGINS_ST_INTERNAL_CONFIG_H
#define __TERRALIB_QT_PLUGINS_ST_INTERNAL_CONFIG_H

// TerraLib
#include "../../../Config.h"


/*!
  \def TE_QT_PLUGIN_ST_HAVE_SLIDER

  \brief It defines if the ST Qt Plugin has the slider operation.
*/
#define TE_QT_PLUGIN_ST_HAVE_SLIDER

/*!
  \def TE_QT_PLUGIN_ST_HAVE_OBSERVATION

  \brief It defines if the ST Qt Plugin has the observation operation.
*/
#define TE_QT_PLUGIN_ST_HAVE_OBSERVATION

/*!
  \def TE_QT_PLUGIN_ST_HAVE_TIMESERIES

  \brief It defines if the ST Qt Plugin has the timeseries operation.
*/
#define TE_QT_PLUGIN_ST_HAVE_TIMESERIES

/*!
  \def TE_QT_PLUGIN_ST_HAVE_TRAJECTORY

  \brief It defines if the ST Qt Plugin has the trajectory operation.
*/
#define TE_QT_PLUGIN_ST_HAVE_TRAJECTORY


/*!
  \def TE_QT_PLUGIN_ST_PLUGIN_NAME

  \brief It contains the plugin name.
*/
#define TE_QT_PLUGIN_ST_PLUGIN_NAME "te.qt.st"

/*!
  \def TE_QT_PLUGIN_ST_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib ST Qt Plugin implementation.
*/
#define TE_QT_PLUGIN_ST_TEXT_DOMAIN "teqtpluginst"

/*!
  \def TE_QT_PLUGIN_ST_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
*/
#define TE_QT_PLUGIN_ST_TEXT_DOMAIN_DIR "locale"

/*!
  \def TE_QT_PLUGIN_ST

  \brief It marks a string in order to get translated. This is a special mark used in the ST QtPlugin of TerraLib.
*/
#define TE_QT_PLUGIN_ST(message) TR(message, TE_QT_PLUGIN_ST_TEXT_DOMAIN)

/*!
  \def TEQTPLUGINSTEXPORT

  \brief You can use this macro in order to export/import classes and functions from all plug-ins files.

  \note To compile plug-ins in Windows, remember to insert TEQTPLUGINSTEXPORT into the project's list of defines.
 */
#ifdef WIN32
  #ifdef TEQTPLUGINSTDLL
    #define TEQTPLUGINSTEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEQTPLUGINSTEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEQTPLUGINSTEXPORT
#endif

#endif  // __TERRALIB_QT_PLUGINS_ST_INTERNAL_CONFIG_H

