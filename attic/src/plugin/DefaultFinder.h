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
  \file terralib/plugin/DefaultFinder.h

  \brief A plugin finder that search for plugins in some special directories defined by compile time macros.
*/

#ifndef __TERRALIB_PLUGIN_INTERNAL_DEFAULTFINDER_H
#define __TERRALIB_PLUGIN_INTERNAL_DEFAULTFINDER_H

// TerraLib
#include "AbstractFinder.h"

// STL
#include <string>

namespace te
{
  namespace plugin
  { 
    /*!
      \class DefaultFinder

      \brief A plugin finder that search for plugins in some special directories defined by compile time macros.

      This finder will look recursively for plugins in the specified folders.

      \sa AbstractFinder, PluginManager, PluginInfo
    */
    class TEPLUGINEXPORT DefaultFinder : public AbstractFinder
    {
      public:

        /*! \brief Constructor. */
        DefaultFinder();

        /*! \brief Destructor. */
        ~DefaultFinder();

        /*!
          \brief It returns the default plugins directories.

          This method will generate directory paths in the following order:
          <ul>
          <li> The current execution directory.
          <li> A default plugins dir (defined by a macro named TE_DEFAULT_PLUGINS_DIR) under the execution directory</li>
          <li> The default plugins dir(defined by a macro named TE_DEFAULT_PLUGINS_DIR) under the directory given by an environment (TERRALIB_DIR_ENVIRONMENT_VARIABLE)</li>
          <li> Under the directory defined by TE_PLUGINS_INSTALL_PATH
          <li> Iff not found returns an empty string</li>
          </ul>

          \param dirs The default plugin directories (the absolute path) or an empty string if none is found.
        */
        void getDefaultDirs( std::vector< std::string >& dirs ) const;

        /*!
          \brief It adds a new base location where the finder will search for installed plugins.

          \param path A new location to search for plugins. It can be an absolute or relative path.

          \exception Exception If you inform an invalid path it will throw an exception.

          \note If the path is already in the list of plugin directories it will be discarted.
        */
        void addPluginsDir(const std::string& path);

        /*!
          \brief It returns the list of plugins base directories.

          \return The list if plugins base directories.
        */
        const std::vector<std::string>& getPluginsDir() const;

        /*!
          \brief This method searches for installed plugins and output the plugins information in the PluginInfo vector.

          This method performs a recursive search on the base directories extracting all plugins information available on these directories.
          Actually it looks for XML files with a pre-defined name (TE_DEFAULT_PLUGIN_FILE_NAME) and it will
          try to get all information available for each plugin.

          \param plugins A vector to output all information about found plugins. The caller
                         will take the ownership of the pointers in the output vector.

          \exception Exception This method may throw an exception with something goes wrong during the search.

          \note Notice that the plugins vector may contains information about plugins before an exception occurs.
                So, it is the caller responsability to check and release any PluginInfo returned by this method
                in the case of an exception.

          \note If no plugins base dir was set it will implicitly look in the default plugins dir.
        */
        void getPlugins(boost::ptr_vector<PluginInfo>& plugins);

      private:

        std::vector<std::string> m_pluginsDir;  //!< The base list of directories to search for plugins.
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_DEFAULTFINDER_H


   