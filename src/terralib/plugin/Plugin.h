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
  \file terralib/plugin/Plugin.h

  \brief A base class for plugin types.
*/

#ifndef __TERRALIB_PLUGIN_INTERNAL_PLUGIN_H
#define __TERRALIB_PLUGIN_INTERNAL_PLUGIN_H

// TerraLib
#include "AbstractPlugin.h"
#include "Config.h"
#include "PluginInfo.h"

namespace te
{
  namespace plugin
  { 
    /*!
      \class Plugin

      \brief A base class for plugin types.

      This class already provides a default implementation
      for all plugin methods and some attributes.

      \ingroup plugin

      \sa AbstractPlugin, PluginManager, PluginInfo, PluginEngine
    */
    class TEPLUGINEXPORT Plugin : public AbstractPlugin
    {
      public:

        const PluginInfo& getInfo() const;

        bool isStarted() const;

        /*! \brief Do nothing! Just set plugin as started */
        virtual void startup();

        /*! \brief Do nothing! Just set plugin as stopped */
        virtual void shutdown();

        /*! \brief Virtual destructor. */
        virtual ~Plugin();

      protected:

        /*!
          \brief It initializes a new plugin.

          \param pInfo Basic plugin information.
         */
        Plugin(const PluginInfo& pInfo);

      protected:

        PluginInfo m_pluginInfo;   //!< Information about the plugin.
        bool m_initialized;        //!< A flag that indicates if the plugin was started or not.
    };

    /*!
      \typedef Plugin* (*GetPluginFPtr)(const PluginInfo& info);

      \brief It exports Plugin's access function type.

      When you are building a C++ Plugin, you will have to define a special function
      (that has a special name convention)
      to be called by TerraLib in order to load your plugin.
      Please, see the documentation HowTo Create C++ TerraLib Plugins for more information.
    */
    typedef Plugin* (*GetPluginFPtr)(const PluginInfo& info);

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_PLUGIN_H

