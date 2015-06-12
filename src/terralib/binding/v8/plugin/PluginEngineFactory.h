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
  \file PluginEngineFactory.h

  \brief This is the concrete factory for JavaScript plugin engines.
 */

#ifndef __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_PLUGINENGINEFACTORY_H
#define __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_PLUGINENGINEFACTORY_H

// TerraLib
#include "../../../plugin/PluginEngineFactory.h"
#include "Config.h"

// STL
#include <memory>

namespace te
{
  namespace v8
  {
    namespace plugin
    {
      /*!
        \class PluginEngineFactory

        \brief This is the concrete factory for JavaScript plugin engines.

        \sa te::plugin::PluginEngineFactory, te::plugin::PluginEngine, te::plugin::Plugin, te::v8::Plugin, te::v8::PluginEngine
       */
      class TEV8PLUGINEXPORT PluginEngineFactory : public te::plugin::PluginEngineFactory
      {
        protected:

          /*!
            \brief It creates a new plugin engine for Java.

            \return It returns plugin engine for Java.
           */
          te::plugin::AbstractPluginEngine* build();

          /*! It register the factory in the abstract dictionary. */
          PluginEngineFactory();        

          /*! \brief Destructor. */
          ~PluginEngineFactory();

          /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory PluginEngineFactory. */
          static void initialize();

          /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory PluginEngineFactory. */
          static void finalize();

        private:

          static PluginEngineFactory* sm_factory;   //!< The plugin engine factory.

        friend class Module;
      };

    } // end namespace plugin
  }   // end namespace v8
}     // end namespace te


#endif  // __TERRALIB_BINDING_V8_PLUGIN_INTERNAL_PLUGINENGINEFACTORY_H

