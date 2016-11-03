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
  \file terralib/plugin/Utils.h

  \brief Utility functions for dealing with plugins.
*/

#ifndef __TERRALIB_PLUGIN_INTERNAL_UTILS_H
#define __TERRALIB_PLUGIN_INTERNAL_UTILS_H

// TerraLib
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace plugin
  {
// Forward declaration
    struct PluginInfo;

    /*!
      \brief It unloads all plugins from a given engine.

      \note This method may propagate the unload of dependent plugins.
    */
    TEPLUGINEXPORT void UnloadAllPluginsFromEngine(const std::string& engine);

    /*!
      \brief It recursively unload the plugin and any dependent plugins.
    */
    TEPLUGINEXPORT void Unload(const std::string& plugin);

    /*!
      \brief It returns information about a given plugin provided its plugin configuration file name or dir.

      \param pluginFilePath Plugin configuration file name or dir (may be a relative path).

      \return The information about the plugin contained in the informed configuration file or dir.
              The caller will take the ownership of the returned PluginInfo.

      \exception Exception It throws an exception if it is not possible to retrieve information about the plugin.
    */
    TEPLUGINEXPORT PluginInfo* GetInstalledPlugin(const std::string& pluginFilePath);

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_UTILS_H

