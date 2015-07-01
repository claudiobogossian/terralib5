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
  \file terralib/qt/plugins/rp/Config.h

  \brief Configuration flags for the RP Qt Widget plugin.
*/

#ifndef __TERRALIB_QT_PLUGINS_RP_INTERNAL_CONFIG_H
#define __TERRALIB_QT_PLUGINS_RP_INTERNAL_CONFIG_H

// TerraLib
#include "../../../Config.h"



/*!
  \def TE_QT_PLUGIN_RP_HAVE_ARITHMETICOP

  \brief It defines if the RP Qt Plugin has the arithmetic operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_ARITHMETICOP

/*!
  \def TE_QT_PLUGIN_RP_HAVE_CLASSIFIER

  \brief It defines if the RP Qt Plugin has the classifier operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_CLASSIFIER

/*!
  \def TE_QT_PLUGIN_RP_HAVE_CLIPPING

  \brief It defines if the RP Qt Plugin has the clipping operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_CLIPPING

/*!
  \def TE_QT_PLUGIN_RP_HAVE_COLORTRANSFORM

  \brief It defines if the RP Qt Plugin has the color transform operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_COLORTRANSFORM

/*!
  \def TE_QT_PLUGIN_RP_HAVE_COMPOSEBANDS

  \brief It defines if the RP Qt Plugin has the compose bands operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_COMPOSEBANDS

/*!
  \def TE_QT_PLUGIN_RP_HAVE_CONTRAST

  \brief It defines if the RP Qt Plugin has the contrast operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_CONTRAST

/*!
  \def TE_QT_PLUGIN_RP_HAVE_FILTER

  \brief It defines if the RP Qt Plugin has the filter operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_FILTER

/*!
  \def TE_QT_PLUGIN_RP_HAVE_FUSION

  \brief It defines if the RP Qt Plugin has the fusion operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_FUSION

/*!
  \def TE_QT_PLUGIN_RP_HAVE_MIXTUREMODEL

  \brief It defines if the RP Qt Plugin has the mixture model operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_MIXTUREMODEL

/*!
  \def TE_QT_PLUGIN_RP_HAVE_MOSAIC

  \brief It defines if the RP Qt Plugin has the mosaic operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_MOSAIC

/*!
  \def TE_QT_PLUGIN_RP_HAVE_SEGMENTER

  \brief It defines if the RP Qt Plugin has the segmenter operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_SEGMENTER

/*!
  \def TE_QT_PLUGIN_RP_HAVE_REGISTER

  \brief It defines if the RP Qt Plugin has the register operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_REGISTER


/*!
  \def TE_QT_PLUGIN_RP_HAVE_VECTORIZATION

  \brief It defines if the RP Qt Plugin has the vectorization operation.
*/
#define TE_QT_PLUGIN_RP_HAVE_VECTORIZATION


/*!
  \def TE_QT_PLUGIN_RP_PLUGIN_NAME

  \brief It contains the plugin name.
*/
#define TE_QT_PLUGIN_RP_PLUGIN_NAME "te.qt.rp"

/*!
  \def TEQTPLUGINRPEXPORT

  \brief You can use this macro in order to export/import classes and functions from all plug-ins files.

  \note To compile plug-ins in Windows, remember to insert TEQTPLUGINRPEXPORT into the project's list of defines.
 */
#ifdef WIN32
  #ifdef TEQTPLUGINRPDLL
    #define TEQTPLUGINRPEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEQTPLUGINRPEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEQTPLUGINRPEXPORT
#endif

#endif  // __TERRALIB_QT_PLUGINS_RP_INTERNAL_CONFIG_H

