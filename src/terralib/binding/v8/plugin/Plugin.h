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
  \file CppPlugin.h
  
  \brief A class that handles JavaScript Plugins using Google's V8 engine.
 */

#ifndef __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_PLUGIN_H
#define __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_PLUGIN_H

// TerraLib
#include "../../../plugin/Plugin.h"
#include "Config.h"

// Google V8
#include <v8.h>

namespace te
{
  namespace v8
  {  
// Forward declaration
    namespace common { class JsContext; }

    namespace plugin
    {
      /*!
        \class Plugin
      
        \brief A class that handles JavaScript Plugins using Google's V8 engine.

        \sa te::plugin::Plugin, te::plugin::PluginEngine, te::java::PluginEngine, te::plugin::PluginEngineFactory, te::v8::PluginEngineFactory
       */
      class TEV8PLUGINEXPORT Plugin : public te::plugin::Plugin
      {
        public:

          /*!
            \brief It creates a new Java plugin.

            \param pInfo Basic plugin information.
           */
          Plugin(const te::plugin::PluginInfo& pInfo);

          /*! \brief Destructor. */
          ~Plugin();

          void startup() throw(...) ;

          void shutdown() throw(...) ;

        private:

          te::v8::common::JsContext* m_ctx;                 //!< The context used to run this plugin. (note: the manager is the owner of this context!)
          ::v8::Persistent<::v8::Function> m_jspluginFtor;  //!< A reference to plugin's class constructor function.
          ::v8::Persistent<::v8::Object> m_jsplugin;        //!< A reference to an instance of the plugin.
          ::v8::Persistent<::v8::Function> m_startup;       //!< A reference to plugin startup method.
          ::v8::Persistent<::v8::Function> m_shutdown;      //!< A reference to plugin shutdown method.

        friend class PluginEngine;
      };

    } // end namespace plugin
  }   // end namespace v8
}     // end namespace te

#endif  // __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_PLUGIN_H

