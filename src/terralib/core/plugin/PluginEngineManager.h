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
  \file terralib/core/plugin/PluginEngineManager.h

  \brief A singleton that can be used to register plugin engines.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */
 
#ifndef __TERRALIB_CORE_PLUGIN_PLUGINENGINEMANAGER_H__
#define __TERRALIB_CORE_PLUGIN_PLUGINENGINEMANAGER_H__

// TerraLib
#include "../Config.h"

// STL
#include <memory>

namespace te
{
  namespace core
  {

// Forward declaration
    class AbstractPluginEngine;

    /*!
      \class PluginEngineManager

      \brief A singleton that can be used to register plugin engines.
     */
    class TECOREEXPORT PluginEngineManager
    {
      public:

        /*!
          \brief Register a new plugin engine.

          \exception InvalidArgumentException It throws an exception if the engine argument is not valid or another
                                              engine with the same name is already registered.
         */
        void insert(std::unique_ptr<AbstractPluginEngine> engine);

        /*!
          \brief Unregister the plugin engine.

          \exception OutOfRangeException If an engine with the given id is not found.
         */
        void remove(const std::string& engine_id);

        /*!
          \brief Find a plugin engine with the given id.

          \exception OutOfRangeException If an engine with the given id is not found.
         */
        AbstractPluginEngine& get(const std::string& engine_id) const;

        /*! \brief Access the singleton. */
        static PluginEngineManager& instance();

      protected:

        /*! \brief Consructor. */
        PluginEngineManager();

        /*! \brief Destructor. */
        ~PluginEngineManager();

      private:

// No copy allowed
        PluginEngineManager(const PluginEngineManager&);
        PluginEngineManager& operator=(const PluginEngineManager&);

      private:

        struct Impl;

        Impl* m_pimpl;
    };

  } // end namespace plugin
}   // end namespace te

#endif  // __TERRALIB_CORE_PLUGIN_PLUGINENGINEMANAGER_H__
