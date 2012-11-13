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
  \file Plugin.h
  
  \brief An abstract interface for TerraLib Plugins.
 */

#ifndef __TERRALIB_PLUGIN_INTERNAL_PLUGIN_H
#define __TERRALIB_PLUGIN_INTERNAL_PLUGIN_H

// TerraLib
#include "Config.h"
#include "PluginInfo.h"

namespace te
{
  namespace plugin
  {     
    /*!
      \class Plugin
      
      \brief An abstract interface for TerraLib Plugins.

      As can be seen in Wikipedia, a plugin (also called plug-in, addin, add-in, addon,
      add-on, snap-in or suplement) is a computer program that interacts with a
      host application (as TerraView) adding new capabilities/functionalities to it.
      <br>
      For more information on plugin definitions,
      see Wikipedia at: http://en.wikipedia.org/wiki/Plug-in_%28computing%29.
      <br>
      In TerraLib, the easy way to create a plugin for TerraView is to use
      the Plugin class interface. This is the same interface for C++, PHP, Lua
      and other languages plugins. In order to easily load
      a plugin, follow these steps:
      <ul>
      <li></li>
      <li></li>
      </ul>
      Note that in TerraLib any plugin will have access to all other system parts. We put no restrictions
      on it, so you can easily use all TerraLib/TerraView API without worry about doing trickies!

      \sa PluginManager
     */
    class TEPLUGINEXPORT Plugin
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It creates a new plugin. */
        Plugin();

        /*! \brief Virtual destructor. */
        virtual ~Plugin();

        //@}

        /** @name Plugin Interface
         *  Plugins must implement this interface in order to be loaded by TerraLib.
         */
        //@{

        /*!
          \brief This method will be called by TerraLib to startup some plugin's functionality.
                  
          \exception Exception It throws and exception if the plugin can not be started.
         */
        virtual void startup() = 0;

        /*!
          \brief This method will be called by TerraLib to shutdown plugin's functionality.
          
          \exception Exception It throws and exception if the plugin can not be shutdown.
         */
        virtual void shutdown() = 0;

        /*!
          \brief It return the information associated to the plugin.

          \return The information associated to the plugin.
         */
        virtual const PluginInfo& getInfo() const = 0;

        //@}

      private:

        /** @name Not Allowed Methods
         *  No copy allowed. 
         */
        //@{

        /*!
          \brief No copy constructor allowed.

          \param rhs The other instance.
         */
        Plugin(const Plugin& rhs);

        /*!
          \brief No assignment operator allowed.

          \param rhs The other instance.

          \return A reference for this instance.
         */
        Plugin& operator=(const Plugin& rhs);

        //@}
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_PLUGIN_H

