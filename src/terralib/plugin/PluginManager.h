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
  \file terralib/plugin/PluginManager.h

  \brief A singleton for managing plugins.
*/

#ifndef __TERRALIB_PLUGIN_INTERNAL_PLUGINMANAGER_H
#define __TERRALIB_PLUGIN_INTERNAL_PLUGINMANAGER_H

// TerraLib
#include "../common/Singleton.h"
#include "Config.h"
#include "Exception.h"

// STL
#include <map>
#include <string>
#include <vector>

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace plugin
  {
// Forward declarations
    class AbstractFinder;
    class AbstractPlugin;

    struct PluginInfo;

    /*!
      \class PluginManager

      \brief A singleton for managing plugins.

      \ingroup plugin

      \sa AbstractPlugin, PluginInfo, AbstractFinder, PluginEngine, PluginEngineFactory
    */
    class TEPLUGINEXPORT PluginManager : public te::common::Singleton<PluginManager>
    {
      friend class te::common::Singleton<PluginManager>;

      public:

        /*!
          \brief It returns the list of plugins managed by PluginManager.

          \param plugins A vector to output the name of plugins managed by this singleton.

          \note The list will contain: loaded, not-loaded and broken plugins.
        */
        std::vector<std::string> getPlugins() const;

        /*!
          \brief It returns the plugin identified by the given name.

          \param name The plugin name.

          \return A plugin identified by the given name.

          \exception Exception It throws an exception if there isn't a plugin with the given name in the manager.
        */
        const PluginInfo& getPlugin(const std::string& name) const;

        /*! \brief It returns the list of plugins that are not loaded. */
        const boost::ptr_vector<PluginInfo>& getUnloadedPlugins() const;

        /*! \*brief It sets a list of unloaded plugins. */
        void setUnloadedPlugins(boost::ptr_vector<te::plugin::PluginInfo> unloadedPlugins);
        
        /*! \brief It returns the list of plugins that could not be loaded. */
        const boost::ptr_vector<PluginInfo>& getBrokenPlugins() const;

        /*! \*brief It sets a list of broken plugins. */
        void setBrokenPlugins(boost::ptr_vector<te::plugin::PluginInfo> brokenPlugins);

        /*!
          \brief It returns true if the plugin is in the broken list of plugins.
        */
        bool isBrokenPlugin(const std::string& pluginName) const;

        /*!
          \brief It returns true if the plugin is in the not-loaded list of plugins.
        */
        bool isUnloadedPlugin(const std::string& pluginName) const;

        /*!
          \brief It returns true if the plugin is loaded otherwise returns false.

          \param pname The plugin name to be checked.

          \return It returns true if the plugin in the list are loaded otherwise returns false.
        */
        bool isLoaded(const std::string& pname) const;

        /*!
          \brief Adds plug-in to unload list.

          \param plugin Information of the plug-in.
        */
        void add(const PluginInfo& plugin);

        /*!
          \brief Adds plugin to unload list and take its ownership.

          \param plugin Information of the plug-in.
        */
        void add(PluginInfo* plugin);

        /*
          \brief Removes plug-in from the manager.

          \details This method removes the plug-in from the manager. If the plug-in was loaded, unload it and remove it from the manager.
          If it was unloaded or broked, just removes it from the correct list. Note that all its dependents will be moved to the broken list.

          \param plugin Name of the plug-in to be removed.

          \exception This method can raise te::plugin::Exception, because internally it calls te::plugin::PluginManager::detach and 
          te::plugin::PluginManager::getPlugin methods and don't handle exceptions raised by it;

          \sa te::plugin::PluginManager::detach, te::plugin::PluginManager::getPlugin
        */
        void remove(const std::string& plugin);

        /*!
          \brief It loads all the plugins in the not-loaded list or searchs for installed plugin with installed finders.

          PluginManager will check the dependency between plugins before trying to load them.

          The associated plugin finders will be used to locate the information about plugins.

          If no plugin finder is available this method will use the default finder to look up for plugins.

          After the fitrst call to this method the plugins list is cached until clear is explicitly called.

          \param start If true it will try to startup the plugins if false it doesn't automatically call plugins startup method and applications must control this.

          \exception Exception It throws an exception if any plugin can not be loaded.

          \note This method will unload all previously loaded plugins.

          \note If a plugin load fails this method will add it to the list of broken plugins and continues. In this case at the end an exception is thrown.
        */
        void loadAll(const bool start = true);

        /*!
          \brief It try to unload all plugins.

          This method will call shutdown for all managed plugins.

          \exception It will raise an exception if it is not possible to unload a plugin.
        */
        void unloadAll();

        /*!
          \brief Unload all plugins and them clear the internal list.

          After calling this method if you call loadAll the manager will look for plugins
          using the registered finders.
        */
        void clear();

        /*!
          \brief It tries to load all informed plugins.

          PluginManager will check the dependency between plugins before trying to load them.

          \param plugins  The list of plugins to be loaded
          \param start    If true it will try to startup the plugin if false it doesn't automatically call plugin startup method.

          \exception Exception It throws an exception if any plugin can not be loaded or started.

          \note If a plugin in the list is already loaded this methods will throw an exception.

          \note If a plugin load fails this method will add it to the list of broken plugins and continues. In this case at the end an exception is thrown.

          \note The manager will sort the plugins according to their load priority.
        */
        void load(boost::ptr_vector<PluginInfo>& plugins, const bool start = true);

        /*!
          \brief It loads the informed plugin and adds it to the list of managed plugins.

          PluginManager will check the dependency between plugins before trying to load the informed plugin.

          \param pInfo  Information about the plugin to be loaded.
          \param start  If true it will try to startup the plugin if false it doesn't automatically call plugin startup method.

          \exception Exception It throws an exception if something goes wrong during plugin load.

          \note This method can checks for category dependency and plugin dependency. It throws an error if the dependency is not satisfied.

          \note If a plugin in the list is already loaded this methods will raise an exception.

          \note If an exception occurs the plugin will be added to the list of broken plugins (except if it is already loaded!).
          
          \note If no plugin folder or an invalid folder was supplied the interal finders will be used to locate the plugin file.
        */
        void load(const PluginInfo& pInfo, const bool start = true);

        void load(const std::string& pluginName);

        /*!
          \brief It tries to unload a given plugin.

          This method will call plugin's shutdown method if needed.

          \param name The plugin to be unloaded.

          \exception It will raise an exception if plugin's code is not unloaded, if it fails to shutdown or if the plugin's is not managed by PluginManager.
        */
        void unload(const std::string& name);

        /*!
          \brief It tries to unload a given plugin.

          This method will call plugin's shutdown method if needed.

          \param plugin The plugin to be unloaded. The pointer will be invalidated if it acomplishes the task.

          \exception It will raise an exception if plugin's code is not unloaded or if it fails to shutdown.

          \note It doesn't throw exceptions if the plugin is not managed by the manager.
        */
        void unload(AbstractPlugin* plugin);

        /*!
          \brief It try to shutdown all plugins.

          This method calls shutdown for all managed plugins.

          \exception It will raise an exception if it is not possible to shutdown a plugin.
        */
        void shutdownAll();

        /*!
          \brief It returns the number of plugins kept in the manager.

          \return The number of plugins in the manager.

          \note This will account for loaded, unloaded and broken plugins.
        */
        std::size_t getNumPlugins() const;

        /*!
          \brief It returns true if each plugin in the list are loaded otherwise returns false.

          \param plugins A list with plugin names to be checked.

          \return It returns true if all plugins in the list are loaded otherwise returns false.

          \note This method can be used to check if a given plugin has all pre-requisites to be loaded.
        */
        bool isLoaded(const std::vector<std::string>& plugins) const;

        /*!
          \brief It searches for all plugins that depends on the given plugin.

          \param pluginName The plugin to be checked.

          \return It returns the list of plugins that depends on the given plugin.
        */
        std::vector<std::string> getDependents(const std::string& pluginName) const;

        /*!
          \brief If there is a plugin that depends on the informed plugin it returns true, otherwise, if no plugin depends on it, return false.

          \param pluginName The plugin to be checked.

          \return It returns true if there is a plugin that depends on the given plugin.
        */
        bool hasDependents(const std::string& pluginName) const;

        /*!
          \brief It detaches the given plugin from the list of loaded plugins.

          \param pluginName The plugin name.

          \exception Exception If there are plugins that depends on the plugin being detached or if the plugin is not found this method raises an exception.

          \note The caller of this method will take the ownership of the given plugin.
        */
        AbstractPlugin* detach(const std::string& pluginName);

        /*!
          \brief Get plugins category types.

          \param A vector to output the name of plugins categories managed by this singleton.
        */
        void getCategories(std::vector<std::string>& categories) const;

        /*!
          \brief Add a new category type.

          \param name  The category name.
        */
        void addCategory(const std::string& name);

      protected:

        /*!
          \brief It sorts the plugins according to their dependency.
        */
        void sort(boost::ptr_vector<PluginInfo>& plugins) const;

        /*!
          \brief It removes the given plugin from the category and then updates the internal category index.

          \param plugin   The plugin to be removed from the given category.
          \param category The categpry to be updated.
        */
        void removeFromCategory(AbstractPlugin* plugin, const std::string& category);

        void moveToBrokenList(const PluginInfo& pInfo);

        void removeFromBrokenList(const PluginInfo& pInfo);

        void removeFromUnloadedList(const PluginInfo& pInfo);

        void moveDependentsToBrokenList(const std::string& plugin, const bool& unloadPlugin=false);

        void updateDependents(const std::string& plugin);
      
      private:

        /*! \brief It creates a new plugin. */
        PluginManager();

        /*! \brief Singleton destructor. */
        ~PluginManager();

      private:

        std::vector<AbstractFinder*> m_finders;                                     //!< The list of plugin finders.
        std::vector<AbstractPlugin*> m_plugins;                                     //!< The list of managed plugins: this will be need to unload accordinly the plugins!
        std::map<std::string, AbstractPlugin*> m_pluginsMap;                        //!< A map from (plugin's name) to (plugin instance).
        std::map<std::string, std::vector<AbstractPlugin*> > m_pluginCategoryMap;   //!< A map from (plugin category) to (plugins in category)
        boost::ptr_vector<PluginInfo> m_unloadedPlugins;                            //!< The list of plugins that are not loaded.
        boost::ptr_vector<PluginInfo> m_brokenPlugins;                              //!< The list of plugins that could not be loaded.
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_PLUGINMANAGER_H
