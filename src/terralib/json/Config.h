/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/json/Config.h

  \brief Configuration flags for the TerraLib JSON module.
*/

#ifndef __TERRALIB_JSON_INTERNAL_CONFIG_H
#define __TERRALIB_JSON_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_JSON_MODULE_NAME "te.da.json"

#define TE_JSON_TEXT_DOMAIN "tejson"

#define TE_JSON_TEXT_DOMAIN_DIR "locale"

#define TR_JSON(message) TR(message, TE_JSON_TEXT_DOMAIN)

#define TE_USE_LIB_JSONCPP

#define TE_USE_LIB_CAJUN

#define TE_USE_LIB_RAPIDJSON

#ifdef WIN32

  #ifdef _MSC_VER
  //  #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
  //  #pragma warning( disable : 4290 )
  #endif

  #ifdef TEJSONSTATIC
    #define TEJSONEXPORT                          // Don't need to export/import... it is a static library
  #elif TEJSONDLL
    #define TEJSONEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEJSONEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEJSONEXPORT
#endif

#endif  // __TERRALIB_JSON_INTERNAL_CONFIG_H


