/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file PluginManager.h
  
  \brief A singleton for managing plugins.
 */

#ifndef __TERRALIB_PLUGIN_INTERNAL_PLUGINMANAGER_H
#define __TERRALIB_PLUGIN_INTERNAL_PLUGINMANAGER_H

// TerraLib
#include "../common/Singleton.h"
#include "Config.h"
#include "PluginInfo.h"

// STL
#include <map>

namespace te
{
  namespace plugin
  {
// Forward declarations
    class Plugin;

    /*!
      \class PluginManager
      
      \brief A singleton for managing plugins.

      \sa Plugin, PluginFactory
     */
    class TEPLUGINEXPORT PluginManager : public te::common::Singleton<PluginManager>
    {
      friend class te::common::Singleton<PluginManager>;

      public:        

        /** @name Singleton Instantiation Control
         *  Singleton destructor.
         */
        //@{

        /*! \brief Singleton destructor. */
        ~PluginManager();

        //@}

        /** @name Plugin Manager Interface
         *  Methods that can be used to loand or unload a given plugin.
         */
        //@{

        /*! \brief It tries to load a plugin.
          
            \param pluginInfo Information about the plugin we want to load. (Input)
         
            \exception Exception It will raise an exception if plugin's code is not loaded, if it fails to startup or if the plugin's is already in use.
       
            \note Before throwing an exception it will unload plugin's code if necessary.
         */
        void loadPlugin(const PluginInfo& pluginInfo);

        /*! \brief It tries to unload a given plugin.
          
            \exception Exception It will raise an exception if plugin's code is not unloaded, if it fails to shutdown or if the plugin's is not loaded.
          
            \note If the plugin's fails to shutdown, you will have
                  to call forceUnloadPlugin to clean up the plugin's code from memory.
                  So, you can try to do something before unloading it definitely.
         */
        void unloadPlugin(const std::string& name);

        /*! \brief It tries to unload all plugins using brute force if needed. */
        void unloadAll();

        /*! \brief It will shutdown a plugin and then unload it, even if plugin's shutdown were not ok.
                    
            \note If you have try to unload a plugin and it doesn't worked,
                  so you should use this method to ensure that all plugin's will
                  be unloaded before TerraLib's code shutdowns.
         */
        void forceUnloadPlugin(const std::string& name);

        //@}

      private:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It creates a new plugin. */
        PluginManager();        

        //@}

      private:
      
        std::map<std::string, Plugin*> m_pluginsMap; //!< A map from (plugin's name) to (plugin instance).
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_PLUGINMANAGER_H
