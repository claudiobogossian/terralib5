/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/plugin.h

  \brief This file contains include headers for the Plugin module of TerraLib.
*/

#ifndef __TERRALIB_INTERNAL_PLUGIN_H
#define __TERRALIB_INTERNAL_PLUGIN_H

// TerraLib
#include "plugin/Config.h"
#include "plugin/CppPlugin.h"
#include "plugin/Plugin.h"
#include "plugin/PluginInfo.h"
#include "plugin/PluginManager.h"
#include "plugin/Utils.h"

/*!
  \defgroup plugin Plugin

  \brief The plugin module provides support for the startup and shutdown of modules loaded at run time.

    A plugin (also called plug-in, addin, add-in, addon, add-on, snap-in, extension or suplement) is a computer program
    that interacts with a host application adding new capabilities/functionalities to it.
    Plugins can also be used to plug new functionalities in an extensible library/platform.

    The TerraLib Plugin module comes with a basic framework for those who want to create plugins in the TerraLib Platform.
    Instead of having a framework target to a specific application like TerraView or SISMADEN, TerraLib Plugin framework just handle
    the extensions (plugins) and is capable of loading then dynamically.
    
    It provides the basic foundation to simplify the burden of dealing with plugins: dependencies, configuration and startup/shutdown actions.
    Besides that in TerraLib 5 a plugin can be built using any language that has a support in the language binding modules.
    For instance you can create plugins in Haskell, Lua, PHP, Java, Python, C and C++.

    Note that in TerraLib any plugin may have access to all other system parts. You can put no restrictions on it,
    so you can easily use all TerraLib API without worry about doing trickies.
    Also you have to define the API of your application. Each application plugin will be dependent on its application API.
    If you have parts of your plugin that can be shared between applications, you should isolate that part and make a plugin with a dependency for it.

    <ul>
      <li>a plugin must provide a root object derived from an abstract plugin class that implements a couple of methods (startup and shutdown);</li>
      <li>a plugin can have initialization parameters;</li>
      <li>the plugin framework provides a basic model for the information needed by a plugin;</li>
      <li>Although a given plugin doesn't have a link dependency of another plugin it can expouse this dependency in its configuration;</li>
      <li>It provides a nice way to control the existence of debug/release versions of the plugins.</li>
    </ul>

    The main classes/concepts in this module are listed here. The namespace associated to the Plugin module is te::plugin.
    To know more about it, see the te::plugin namespace documentation.
*/

namespace te
{
  /*! \brief Namespace for the Plugin module of TerraLib. */
  namespace plugin
  {
  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_PLUGIN_H

