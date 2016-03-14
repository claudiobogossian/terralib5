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
  \file src/terraMobilePlugin/Config.h

  \brief Configuration flags for the terra mobile Qt Widget plugin.
*/

#ifndef __TERRALIB_QT_PLUGINS_TERRAMOBILE_INTERNAL_CONFIG_H
#define __TERRALIB_QT_PLUGINS_TERRAMOBILE_INTERNAL_CONFIG_H

// TerraLib
#include <terralib_buildconfig.h>


/*!
  \def TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGEBUILDER

  \brief It defines if the terra mobile plugin has the geopackage builder operation.
*/
#define TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGEBUILDER

/*!
\def TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGEPUBLISHER

\brief It defines if the terra mobile plugin has the geopackage publisher operation.
*/
#define TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGEPUBLISHER

/*!
\def TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGESYNCHRONIZER

\brief It defines if the terra mobile plugin has the geopackage synchronizer operation.
*/
#define TE_QT_PLUGIN_TERRAMOBILE_HAVE_GEOPACKAGESYNCHRONIZER

/*!
\def TE_QT_PLUGIN_TERRAMOBILE_HAVE_CREATELAYER

\brief It defines if the terra mobile plugin has the create layer operation.
*/
#define TE_QT_PLUGIN_TERRAMOBILE_HAVE_CREATELAYER

/*!
  \def TE_QT_PLUGIN_TERRAMOBILE_PLUGIN_NAME

  \brief It contains the plugin name.
*/
#define TE_QT_PLUGIN_TERRAMOBILE_PLUGIN_NAME "te.qt.terramobile"

/*!
\def GPKG_DRIVER_IDENTIFIER

\brief The OGR driver identifier string.
*/
#define GPKG_DRIVER_IDENTIFIER "GPKG"

#define LAYER_GATHERING_STATUS_COLUMN "TM_STATUS"

#define LAYER_GATHERING_OBJID_COLUMN "OBJ_ID"

/*!
  \def TEQTPLUGINTERRAMOBILEEXPORT

  \brief You can use this macro in order to export/import classes and functions from all plug-ins files.

  \note To compile plug-ins in Windows, remember to insert TEQTPLUGINTERRAMOBILEEXPORT into the project's list of defines.
 */
#ifdef WIN32
#ifdef MOBILEDLL
    #define TERRAMOBILEPLUGINSDLLEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TERRAMOBILEPLUGINSDLLEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TERRAMOBILEPLUGINSDLLEXPORT
#endif

#endif  // __TERRALIB_QT_PLUGINS_TERRAMOBILE_INTERNAL_CONFIG_H

