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
  \file terralib/yaml/Config.h

  \brief Configuration flags for the TerraLib YAML module.
*/

#ifndef __TERRALIB_YAML_INTERNAL_CONFIG_H
#define __TERRALIB_YAML_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_YAML_MODULE_NAME "te.da.yaml"

#define TE_YAML_TEXT_DOMAIN "teyaml"

#define TE_YAML_TEXT_DOMAIN_DIR "locale"

#define TR_YAML(message) TR(message, TE_YAML_TEXT_DOMAIN)

#define TE_USE_LIB_YAMLCPP

#ifdef WIN32

  #ifdef _MSC_VER
  //  #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
  //  #pragma warning( disable : 4290 )
  #endif

  #ifdef TEYAMLSTATIC
    #define TEYAMLEXPORT                          // Don't need to export/import... it is a static library
  #elif TEYAMLDLL
    #define TEYAMLEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEYAMLEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEYAMLEXPORT
#endif

#endif  // __TERRALIB_YAML_INTERNAL_CONFIG_H


