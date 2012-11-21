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
  \file af_plugins/Config.h

  \brief Configuration flags for exporting / importing plug-in code.
 */

#ifndef __TERRALIB_AFPLUGINS_INTERNAL_CONFIG_H
#define __TERRALIB_AFPLUGINS_INTERNAL_CONFIG_H

/*!
  \def TEAFPLUGINEXPORT

  \brief You can use this macro in order to export/import classes and functions from all plug-ins files.
  \note To compile plug-ins in Windows, remember to insert TEAFPLUGINDLL into the project's list of defines.
 */
#ifdef WIN32
  #ifdef TEAFPLUGINDLL
    #define TEAFPLUGINEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEAFPLUGINEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEAFPLUGINEXPORT
#endif

#endif  // __TERRALIB_AFPLUGINS_INTERNAL_CONFIG_H

