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
  \file PluginEngine.h
  
  \brief A plugin engine for plugins written in JavaScript using Google's V8 engine.
 */

#ifndef __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_PLUGINENGINE_H
#define __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_PLUGINENGINE_H

// TerraLib
#include "Config.h"
#include "../../../plugin/AbstractPluginEngine.h"

namespace te
{
  namespace v8
  {  
    namespace plugin
    {
      /*!
        \class PluginEngine
      
        \brief A plugin engine for plugins written in JavaScript using Google's V8 engine.

        \sa te::plugin::Plugin, te::plugin::PluginInfo, te::plugin::PluginEngineFactory, te::plugin::PluginEngine, te::v8::Plugin, te::v8::PluginFactory
       */
      class TEV8PLUGINEXPORT PluginEngine : public te::plugin::AbstractPluginEngine
      {
        public:

          /*! Default construtor. */
          PluginEngine() { }

          /*! \brief Virtual destructor. */
          ~PluginEngine() { }

          te::plugin::AbstractPlugin* load(const te::plugin::PluginInfo& pInfo) throw(...);

          void unload(te::plugin::AbstractPlugin* plugin) throw(...);
      };

    } // end namespace plugin
  }   // end namespace v8
}     // end namespace te

#endif  // __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_PLUGINENGINE_H

