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
  \file terralib/qt/plugins/sa/Config.h

  \brief Configuration flags for the SA Qt Widget plugin.
*/

#ifndef __TERRALIB_QT_PLUGINS_SA_INTERNAL_CONFIG_H
#define __TERRALIB_QT_PLUGINS_SA_INTERNAL_CONFIG_H

// TerraLib
#include "../../../Config.h"

/*!
  \def TE_QT_PLUGIN_SA_HAVE_BAYESGLOBAL

  \brief It defines if the SA Qt Plugin has the Global Bayes operation
*/
#define TE_QT_PLUGIN_SA_HAVE_BAYESGLOBAL

/*!
  \def TE_QT_PLUGIN_SA_HAVE_BAYESLOCAL

  \brief It defines if the SA Qt Plugin has the Local Bayes operation
*/
#define TE_QT_PLUGIN_SA_HAVE_BAYESLOCAL

/*!
  \def TE_QT_PLUGIN_SA_HAVE_KERNELMAP

  \brief It defines if the SA Qt Plugin has the kernel map operation
*/
#define TE_QT_PLUGIN_SA_HAVE_KERNELMAP

/*!
  \def TE_QT_PLUGIN_SA_HAVE_KERNELRATIO

  \brief It defines if the SA Qt Plugin has the kernel ratio operation
*/
#define TE_QT_PLUGIN_SA_HAVE_KERNELRATIO

/*!
  \def TE_QT_PLUGIN_SA_HAVE_GEOSTATISTICALMETHODS

  \brief It defines if the SA Qt Plugin has the geostatistical methods operation
*/
#define TE_QT_PLUGIN_SA_HAVE_GEOSTATISTICALMETHODS

/*!
  \def TE_QT_PLUGIN_SA_HAVE_PROXIMITYMATRIXCREATOR

  \brief It defines if the SA Qt Plugin has the proximity matrix creator operation
*/
#define TE_QT_PLUGIN_SA_HAVE_PROXIMITYMATRIXCREATOR

/*!
  \def TE_QT_PLUGIN_SA_HAVE_SAMPLEPOINTSGENERATOR

  \brief It defines if the SA Qt Plugin has the sample points generator operation
*/
#define TE_QT_PLUGIN_SA_HAVE_SAMPLEPOINTSGENERATOR

/*!
  \def TE_QT_PLUGIN_SA_HAVE_SKATER

  \brief It defines if the SA Qt Plugin has the skater operation
*/
#define TE_QT_PLUGIN_SA_HAVE_SKATER

/*!
  \def TE_QT_PLUGIN_SA_HAVE_SPATIALSTATISTICS

  \brief It defines if the SA Qt Plugin has the spatial statistics operation
*/
#define TE_QT_PLUGIN_SA_HAVE_SPATIALSTATISTICS

/*!
  \def TE_QT_PLUGIN_SA_PLUGIN_NAME

  \brief It contains the plugin name.
*/
#define TE_QT_PLUGIN_SA_PLUGIN_NAME "te.qt.sa"

/*!
  \def TEQTPLUGINSAEXPORT

  \brief You can use this macro in order to export/import classes and functions from all plug-ins files.

  \note To compile plug-ins in Windows, remember to insert TEQTPLUGINSAEXPORT into the project's list of defines.
 */
#ifdef WIN32
  #ifdef TEQTPLUGINSADLL
    #define TEQTPLUGINSAEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEQTPLUGINSAEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEQTPLUGINSAEXPORT
#endif

#endif  // __TERRALIB_QT_PLUGINS_SA_INTERNAL_CONFIG_H

