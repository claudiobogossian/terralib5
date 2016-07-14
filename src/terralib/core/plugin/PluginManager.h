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
  \file terralib/common/plugin/PluginManager.h

  \brief A singleton for managing plugins.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __TERRALIB_CORE_PLUGIN_PLUGINMANAGER_H__
#define __TERRALIB_CORE_PLUGIN_PLUGINMANAGER_H__

// TerraLib
#include "../Config.h"
#include "PluginInfo.h"

namespace te
{
  namespace core
  {

    /*!
      \class PluginManager
      \brief A singleton for managing plugins.

      \note Methods in this class are not thread-safe.
     */
    class TECOREEXPORT PluginManager
    {
      public:

        /*!
          \brief Return the list of plugins managed by PluginManager.

          \param plugins A vector to output the name of plugins managed by this singleton.

          \note The list will contain: loaded, not-loaded and broken plugins.
         */
        std::vector<std::string> getPlugins() const;

        /*!
          \brief Return information about a plugin identified by the given name.

          \param name The plugin name.

          \return A plugin identified by the given name.

          \exception tws::item_not_found_error It throws an exception if there isn't a plugin with the given name in the manager.
         */
        const PluginInfo& getPluginInfo(const std::string& name) const;

        /*! \brief Return the list of plugins that were not loaded. */
        std::vector<PluginInfo> getUnloadedPlugins() const;

        /*! \brief Return the list of plugins that could not be loaded. */
        std::vector<PluginInfo> getBrokenPlugins() const;

        /*! \brief Returns true if the plugin is in the broken list of plugins. */
        bool isBroken(const std::string& plugin_name) const;

        /*! \brief Returns true if the plugin is in the not-loaded list of plugins. */
        bool isUnloaded(const std::string& plugin_name) const;

        /*! \brief Returns true if the plugin is loaded otherwise returns false. */
        bool isLoaded(const std::string& plugin_name) const;

        /*! \brief Tells if a given plugin is registered or not. */
        bool exists(const std::string& plugin_name) const;

        /*!
          \brief Adds plugin with its plugin information to the list of unloaded plugins.

          \exception plugin_already_registered_error Throw an exception if a plugin with the same
                     name already exists and it is registered in the manager.
         */
        void insert(const PluginInfo& pinfo);

        /*!
          \brief Remove plugin from the manager.

          This method removes the plugin from the manager.
          If the plugin was loaded, unload it and remove it from the manager.
          If it was unloaded or broked, just removes it from the correct list.
          Note that all its dependents will be moved to the broken list.

          \param plugin_name Name of the plugin to be removed.

          \exception tws::exception If the plugin can not be removed an exception is raised.

          \note Don't change the type of parameter to a const reference! (guess?)
         */
        void remove(const std::string& plugin_name);

        /*!
          \brief It tries to load the informed plugin.

          The manager will check the plugin's dependencies before trying to load it.

          This method can be used to retry loading a broken plugin.

          \pre plugins must be in the manager before calling this method.

          \param plugin_name  The plugin to be loaded
          \param start        If true it will try to startup the plugin if false it doesn't automatically call plugin startup method.

          \exception te::plugin::exception It throws an exception if any plugin can not be loaded or started.

          \note If the plugin in the list is already loaded this method will throw an exception.

          \note If the plugin load fails this method will add it to the list of broken plugins and continues.
                In this case at the end an exception is thrown.

          \note If the plugin is in the broken list, on success load it will be removed from the broken list.
         */
        void load(const std::string& plugin_name, const bool start = true);

        /*!
          \brief Start a loaded plugin.

          \exception tws::exception It throws an exception if any plugin can not be started.
         */
        void start(const std::string& plugin_name);

        /*!
          \brief Stop a loaded plugin.

          \exception tws::exception It throws an exception if any plugin can not be stoped.
         */
        void stop(const std::string& plugin_name);

        /*!
         \brief Try to unload a given plugin.

         This method will call plugin's shutdown method if needed.

         \param plugin_name The plugin to be unloaded.

         \exception tws::exception It will raise an exception if plugin's code is not unloaded, if it fails to shutdown or if the plugin's is not managed by PluginManager.
         */
        void unload(const std::string& plugin_name);

        /*! \brief  Stop and unload all plugins, then clear the internal list of plugins. */
        void clear();

        /*! \brief  Access the singleton. */
        static PluginManager& instance();

      private:

         /*! \brief Constructor */
        PluginManager();

         /*! \brief Destructor. */
        ~PluginManager();

// no copy allowed
        PluginManager(const PluginManager&);
        PluginManager& operator=(const PluginManager&);

      private:

        struct Impl;

        Impl* m_pimpl;
    };

  } // end namespace core
}   // end namespace te

#endif  // __TERRALIB_CORE_PLUGIN_PLUGINMANAGER_H__
