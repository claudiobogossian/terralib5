/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/core/plugin/PluginInfo.h

  \brief The basic information about a plugin.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __TERRALIB_CORE_PLUGIN_PLUGININFO_H__
#define __TERRALIB_CORE_PLUGIN_PLUGININFO_H__

// STL
#include <string>
#include <utility>
#include <vector>

namespace te
{
  namespace core
  {

    /*! Describe the plugin provider. */
    struct Provider
    {
      std::string name;   //!< Provider name: may be a person or a company.
      std::string site;   //!< The provider home page.
      std::string email;  //!< The provider contact e-mail.
    };

    /*! \brief Describe the host system */
    struct HostApplication
    {
      std::string name;
      std::string version;
    };

    typedef std::pair<std::string, std::string> Resource;

    typedef std::pair<std::string, std::string> Parameter;

    /*! \brief Basic information about a plugin. */
    struct PluginInfo
    {
      std::string name;                          //!< The plugin name: an internal value used to identify the plugin in the system. Must be a unique value.
      std::string display_name;                  //!< The plugin name to be displayed in a graphical interface.
      std::string description;                   //!< A brief explanation about the plugin.
      std::string version;                       //!< The plugin version.
      std::string release;                       //!< The release date of the plugin. This may be used to identify new versions of a given plugin.
      std::string engine;                        //!< The type of plugin execution engine: C++, JAVA, LUA or any other supported engine.
      std::string license_description;           //!< A brief description about the plugin license.
      std::string license_URL;                   //!< An URL where someone can find more information on the license.
      std::string site;                          //!< An URL pointing to the plugin site.
      Provider provider;                         //!< Information about the plugin provider.
      std::vector<std::string> dependencies;     //!< The list of required plugins in order to launch the plugin.
      std::vector<std::string> linked_libraries; //!< The list of linked libraries.
      std::vector<Resource> resources;           //!< The list of resources used by plugin.
      std::vector<Parameter> parameters;         //!< Any configuration parameter that can be informed to plugin (map: parameter-name -> parameter-value).
      HostApplication host_application;          //!< Information about the host system. May be used to validate the plugin version.
    };

  }  // end namespace core
}    // end namespace te

#endif  // __TERRALIB_CORE_PLUGIN_PLUGININFO_H__
