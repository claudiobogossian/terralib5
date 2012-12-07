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

  \brief Configuration flags for the plug-in OGR for application framework.
 */

#ifndef __TERRALIB_AFPLUGINPGIS_INTERNAL_CONFIG_H
#define __TERRALIB_AFPLUGINPGIS_INTERNAL_CONFIG_H

#include "../Config.h"

#define AF_PGIS_PLUGIN_NAME "te.da.qt.pgis"

/*!
  \def AF_PGIS_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in TerraLib PostGIS driver implementation.
 */
#define AF_PGIS_TEXT_DOMAIN "teaf_pgis"

/*!
  \def AF_PGIS_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
 */
#define AF_PGIS_TEXT_DOMAIN_DIR "locale"

/*!
  \def AF_TR_PGIS

  \brief It marks a string in order to get translated. This is a special mark used in the DataAccess module of TerraLib.
 */
#define AF_TR_PGIS(message) TR(message, AF_PGIS_TEXT_DOMAIN)

/*!
  \def AF_TR_PLURAL_OGR

  \brief This mark can be used when you have a plural phrase to be translated. This is a special mark used in the DataAccess module of TerraLib. 
 */
#define AF_TR_PLURAL_PGIS(message1, message2, n) TR_PLURAL(AF_PGIS_TEXT_DOMAIN, message1, message2, n)

/*!
  \def AF_PGIS_LOGGER_NAME
  
  \brief This is the fully qualified TerraLib DataAccess API logger name.
 */
#define AF_PGIS_LOGGER_NAME "te.af.pgis"

/*!
  \def TEQTPLUGINPGISEXPORT

  \brief You can use this macro in order to export/import classes and functions from all plug-ins files.
  \note To compile plug-ins in Windows, remember to insert TEQTPLUGINPGISDLL into the project's list of defines.
 */
#ifdef WIN32
  #ifdef TEQTPLUGINPGISDLL
    #define TEQTPLUGINPGISEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEQTPLUGINPGISEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEQTPLUGINPGISEXPORT
#endif

#endif  // __TERRALIB_AFPLUGINPGIS_INTERNAL_CONFIG_H

