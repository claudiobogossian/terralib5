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
  \file terralib/plugin/PluginInfo.h

  \brief The basic information about a plugin.
*/

#ifndef __TERRALIB_PLUGIN_INTERNAL_PLUGININFO_H
#define __TERRALIB_PLUGIN_INTERNAL_PLUGININFO_H

// TerraLib
#include "Config.h"
#include "Provider.h"

// STL
#include <map>
#include <string>
#include <vector>

namespace te
{
// Forward declaration
  namespace xml { class Reader; }

  namespace plugin
  {
    /*!
      \struct PluginInfo
      
      \brief The basic information about a plugin.

      \ingroup plugin

      \sa Plugin

      \note There are some special plugins categories:
            <ul>
            <li>Data Access</li>
            <li>Unknown Category</li>
            <li>Language Bindings</li>
            </ul>
    */
    struct PluginInfo
    {
      typedef std::pair<std::string, std::string> Resource;
      typedef std::pair<std::string, std::string> Parameter;

      std::string m_name;                                             //!< The plugin name: an internal value used to identify the plugin in the system. Must be a unique value.
      std::string m_displayName;                                      //!< The plugin name to be displayed in a graphical interface.
      std::string m_description;                                      //!< A brief explanation about the plugin.
      std::string m_version;                                          //!< The plugin version.
      std::string m_release;                                          //!< The release date of the plugin. This may be used to identify new versions of a given plugin.
      std::string m_engine;                                           //!< The type of plugin execution engine: C++, JAVA.
      std::string m_terralibVersion;                                  //!< The TerraLib version this plugin depends on.
      std::string m_licenseDescription;                               //!< A brief description about the plugin license.
      std::string m_licenseURL;                                       //!< An URL where someone can find more information on the license.
      std::string m_category;                                         //!< The plugin category.
      std::string m_site;                                             //!< An URL pointing to the plugin site.
      Provider m_provider;                                            //!< Information about the plugin provider.
      std::vector<std::string> m_requiredPlugins;                     //!< The list of required plugins in order to lunch the plugin.
      std::vector<std::string> m_requiredPluginCategories;            //!< The list of required category of plugins in order to lunch the plugin.
      std::vector<std::string> m_requiredModules;                     //!< The list of required category of plugins in order to lunch the plugin.
      std::vector<Resource> m_resources;                              //!< The list of resources used by plugin.
      std::vector<Parameter> m_parameters;                            //!< Any configuration parameter that can be informed to plugin (map: parameter-name -> parameter-value).

      /*!
        \brief This overloaded operator can be used to index the plugin info in a set.

        \param rhs The right-hand-side plugin info.

        \return True if this plugin name is lexcographical less than the rhs plugin name.
      */
      bool operator<(const PluginInfo& rhs) const
      {
        return m_name < rhs.m_name;
      }

      struct Finder1st : public std::unary_function<std::pair<std::string, std::string>, bool>
      {
        Finder1st(const std::string& val)
          : m_first(val)
        {
        }

        bool operator()(const std::pair<std::string, std::string>& r)
        {
          return r.first == m_first;
        }

        std::string m_first;
      };
    };

  } // end namespace plugin
}   // end namespace te

/*!
  \brief It deserializes the plugin info using the given XML reader.

  \param pInfo     The plugin information struct to be filled.
  \param xmlReader The XML reader to be used to extract the plugin information.

  \return A reference to the input pInfo.
*/
TEPLUGINEXPORT te::plugin::PluginInfo& operator<<(te::plugin::PluginInfo& pInfo, te::xml::Reader& xmlReader);

#endif  // __TERRALIB_PLUGIN_INTERNAL_PLUGININFO_H

