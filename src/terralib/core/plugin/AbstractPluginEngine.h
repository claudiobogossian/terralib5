/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/core/plugin/AbstractPluginEngine.h

  \brief The base class for plugin engines.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __TERRALIB_CORE_PLUGIN_ABSTRACTPLUGINENGINE_H__
#define __TERRALIB_CORE_PLUGIN_ABSTRACTPLUGINENGINE_H__

// TerraLib
#include "../Config.h"
#include "PluginInfo.h"

// STL
#include <memory>
#include <string>

namespace te
{
  namespace core
  {

// Forward declaration
    class AbstractPlugin;

    /*!
      \class AbstractPluginEngine

      \brief The base class for plugin engines.
     */
    class AbstractPluginEngine
    {
      public:

        /*! \brief Default construtor. */
        AbstractPluginEngine() { }

        /*! \brief Virtual destructor. */
        virtual ~AbstractPluginEngine() = default;

        /*! \brief Every plugin engine must have a unique identifier. */
        virtual const std::string& id() const = 0;

        /*! \brief The name of plugin engine with a brief title or description. */
        virtual const std::string& name() const = 0;

        /*!
          \brief Load the informed plugin.

          \param pinfo Information about the plugin to be loaded.

          \exception PluginEngineLoadException If the plugin can not be loaded.
         */
        virtual std::unique_ptr<AbstractPlugin> load(const PluginInfo& pinfo) = 0;

        /*!
          \brief Unload the informed plugin.

          On success the informed plugin will be also destroyed 
          and its pointer will be invalidated.

          \exception PluginEngineUnloadException It may throws an exception.
         */
        virtual void unload(std::unique_ptr<AbstractPlugin> plugin) = 0;

      private:

// no copy allowed
        AbstractPluginEngine(const AbstractPluginEngine&);
        AbstractPluginEngine& operator=(const AbstractPluginEngine&);
    };

  }  // end namespace core
}    // end namespace te

#endif  // __TERRALIB_CORE_PLUGIN_ABSTRACTPLUGINENGINE_H__
