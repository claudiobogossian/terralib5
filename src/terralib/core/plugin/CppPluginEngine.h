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
  \file terralib/core/plugin/CppPluginEngine.h

  \brief A plugin engine for plugins written in C++.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __TERRALIB_CORE_PLUGIN_CPPPLUGINENGINE_H__
#define __TERRALIB_CORE_PLUGIN_CPPPLUGINENGINE_H__

// TerraLib
#include "AbstractPluginEngine.h"

namespace te
{
  namespace core
  {

    /*!
      \class CppPluginEgine

      \brief A plugin engine for plugins written in C++.
     */
    class TECOREEXPORT CppPluginEngine : public AbstractPluginEngine
    {
      public:

        /*! \brief Default construtor. */
        CppPluginEngine();

        /*! \brief Destructor. */
        ~CppPluginEngine();

        /*! \brief It returns the id of the plugin engine */
        const std::string& id() const;

        /*! \brief It returns the name of the plugin engine */
        const std::string& name() const;

        /*!
          \brief It loads a cpp plugin from a give PluginInfo

          \exception PluginEngineLoadException If the plugin cannot be loaded.
          \return A unique_ptr of the loaded plugin.
         */
        std::unique_ptr<AbstractPlugin> load(const PluginInfo& pinfo);

        /*!
          \brief It unloads a cpp plugin from a give AbstractPlugin unique_ptr

          \exception PluginEngineUnloadException If the plugin cannot be unloaded.
         */
        void unload(std::unique_ptr<AbstractPlugin> plugin);

      private:

        struct Impl;

        Impl* m_pimpl;
    };

  } // end namespace core
}   // end namespace te

#endif  // __TERRALIB_CORE_PLUGIN_CPPPLUGINENGINE_H__
