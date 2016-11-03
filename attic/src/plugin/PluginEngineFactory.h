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
  \file terralib/plugin/PluginEngineFactory.h

  \brief This is the abstract factory for plugin engines.
*/

#ifndef __TERRALIB_PLUGIN_INTERNAL_PLUGINENGINEFACTORY_H
#define __TERRALIB_PLUGIN_INTERNAL_PLUGINENGINEFACTORY_H

// TerraLib
#include "../common/AbstractFactory.h"
#include "AbstractPluginEngine.h"
#include "Config.h"

namespace te
{
  namespace plugin
  {
    /*!
      \class PluginEngineFactory

      \brief This is the abstract factory for plugin engines.

      If you want a specific engine in order to load a plugin you can use a command like:
      <code>
      te::plugin::AbstractPluginEngine* pEngine = te::plugin::PluginEngineFactory::make("C++");
      </code>

      \note The caller of the method make will take the ownership of the returned engine.

      \sa PluginEngine, AbstractFactory
    */
    class TEPLUGINEXPORT PluginEngineFactory : public te::common::AbstractFactory<AbstractPluginEngine, std::string>
    {
      public:

        /*! \brief Virtual destructor. */
        virtual ~PluginEngineFactory();

      protected:

        /*!
          \brief It creates the factory.

          The key of a PluginEngineFactory is a string.

          \param factoryKey The key that identifies the factory.
        */
        PluginEngineFactory(const std::string& factoryKey);
    };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_PLUGIN_INTERNAL_PLUGINENGINEFACTORY_H
