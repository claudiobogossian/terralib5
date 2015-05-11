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
  \file Module.h
   
  \brief The TerraLib V8 Plugin module is itself a plugin.
 */

#ifndef __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_MODULE_H
#define __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_MODULE_H

// TerraLib
#include "../../../plugin/Plugin.h"
#include "Config.h"

namespace te
{
  namespace v8
  {
    namespace plugin
    {
      /*!
        \class Module

        \brief An utility class to initialize and terminate TerraLib's Google JavaScript V8 engine module.

        \sa te::plugin::CppPlugin
       */
      class TEV8PLUGINEXPORT Module : public te::plugin::Plugin
      {
        public:

          /*!
            \brief Plugin constructor.
            
            \param pInfo Basic information provided to initialize this module through the plugin API.
           */
          Module(const te::plugin::PluginInfo& pluginInfo);

          /*! \brief destructor. */
          ~Module();

          /*!
            \brief This is the startup function for the TerraLib's Google JavaScript V8 engine module.
           
            The initialization includes:
            <ul>
            <li>Multilanguage support</li>
            <li>PluginEngineFactory initialization</li>
            </ul>
           
            \exception Exception It may throws an exception.
         
            \warning Not thread safe!
           */
          void startup() throw(...) ;

          /*!
            \brief It finalizes the TerraLib's Google JavaScript V8 engine module.

            \exception Exception It may throws an exception.

            \warning Not thread safe!
           */
          void shutdown() throw(...) ;

        private:

          bool m_initialized; //!< A flag that indicates if the V8 Plugin module is initialized or not.
      };

    } // end namespace plugin
  }   // end namespace v8
}     // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEV8PLUGINEXPORT);

#endif  // __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_MODULE_H

