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
  \file terralib/plugin/AbstractPlugin.h
  
  \brief An abstract class for TerraLib Plugins.
*/

#ifndef __TERRALIB_PLUGIN_INTERNAL_ABSTRACTPLUGIN_H
#define __TERRALIB_PLUGIN_INTERNAL_ABSTRACTPLUGIN_H

// TerraLib
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace plugin
  { 
// Forward declaration
    struct PluginInfo;

    /*!
      \class AbstractPlugin
      
      \brief An abstract class for TerraLib Plugins.

      A plugin (also called plug-in, addin, add-in, addon,
      add-on, snap-in, extension or suplement) is a computer program that interacts with a
      host application like TerraView adding new capabilities/functionalities to it.

      For more information on plugin definitions, please,
      see Wikipedia at http://en.wikipedia.org/wiki/Plug-in_%28computing%29.

      In TerraLib, the easy way to create a plugin for TerraView is to use
      the Plugin class interface. This is the same interface for C++, PHP, Lua
      and other languages plugins.

      Note that in TerraLib any plugin may have access to all other system parts. You can put no restrictions
      on it, so you can easily use all TerraLib/TerraView API without worry about doing trickies!

      \ingroup plugin

      \sa Plugin, PluginManager, PluginInfo, PluginEngine
    */
    class TEPLUGINEXPORT AbstractPlugin : public boost::noncopyable
    {
      public:

        /*! \brief Constructor. */
        AbstractPlugin();

        /*! \brief Virtual destructor. */
        virtual ~AbstractPlugin();

        /*!
          \brief It return the information associated to the plugin.

          \return The information associated to the plugin.
        */
        virtual const PluginInfo& getInfo() const = 0;

        /*!
          \brief It tells if the plugin was started or not.

          \return True if the plugin is started and false otherwise (it is shutdown).
        */
        virtual bool isStarted() const = 0;

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
          \brief This overloaded operator can be used to index the plugin in a set.

          \param rhs The right-hand-side plugin.

          \return True if this plugin name is lexcographical less than the rhs plugin name.
        */
        bool operator<(const AbstractPlugin& rhs) const;
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_ABSTRACTPLUGIN_H

