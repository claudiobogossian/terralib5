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
  \file terralib/plugin/Config.h

  \brief Configuration flags for the TerraLib Plugin module.
*/

#ifndef __TERRALIB_PLUGIN_INTERNAL_CONFIG_H
#define __TERRALIB_PLUGIN_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

/** @name Plugin Specific Macros
 *  Macros used by plugin module.
 */
//@{

/*!
  \def TE_PLUGIN_MODULE_NAME

  \brief The module name (identifier).
*/
#define TE_PLUGIN_MODULE_NAME "te.plugin"

/*!
  \def PLUGIN_CALL_BACK_DECLARATION

  \brief This macro should be used by C++ plugins in order to declare the exportable/callable DLL function.
*/
#define PLUGIN_CALL_BACK_DECLARATION(PLUGIN_EXPORT_MACRO) \
               extern "C" PLUGIN_EXPORT_MACRO te::plugin::Plugin* CppPluginGetInstance(const te::plugin::PluginInfo& info);

/*!
  \def PLUGIN_CALL_BACK_IMPL

  \brief This macro should be used by C++ plugins in order to declare the exportable/callable DLL function.
*/
#define PLUGIN_CALL_BACK_IMPL(PLUGIN_CLASS_NAME) \
               te::plugin::Plugin* CppPluginGetInstance(const te::plugin::PluginInfo& info) \
               { \
                 return new PLUGIN_CLASS_NAME(info); \
               }

/*!
  \def TE_CPPPLUGINENGINE_CODE

  \brief The plugin engine name for C++ plugins.
*/
#define TE_CPPPLUGINENGINE_CODE "C++"

/*!
  \def TE_DEFAULT_PLUGINS_DIR

  \brief The default look up plugin dir.
*/
#define TE_DEFAULT_PLUGINS_DIR "share/terralib/plugins"

/*!
  \def TE_DEFAULT_PLUGIN_EXTENSION

  \brief The default extension for plugins description files.
*/
#define TE_DEFAULT_PLUGIN_EXTENSION ".teplg"

/*!
  \def TE_AUTOMATICALLY_LOAD_DEFAULT_XML_PLUGIN

  \brief If this flag is set to 1 the plugin module will start a default XML driver.
 */
#define TE_AUTOMATICALLY_LOAD_DEFAULT_XML_PLUGIN 1

//@}

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TEPLUGINEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TEPLUGINDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TEPLUGINSTATIC flag into the project list of defines.
*/
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
  #endif

  #ifdef TEPLUGINSTATIC
    #define TEPLUGINEXPORT                          // Don't need to export/import... it is a static library
  #elif TEPLUGINDLL
    #define TEPLUGINEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TEPLUGINEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TEPLUGINEXPORT
#endif

//@}

#endif  // __TERRALIB_PLUGIN_INTERNAL_CONFIG_H

