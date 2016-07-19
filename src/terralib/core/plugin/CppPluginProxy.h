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
  \file terralib/core/plugin/CppPluginProxy.h

  \brief A proxy class for C++ plugins.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __TERRALIB_CORE_PLUGIN_CPPPLUGINPROXY_H__
#define __TERRALIB_CORE_PLUGIN_CPPPLUGINPROXY_H__

// TerraLib
#include "AbstractPlugin.h"

// STL
#include <memory>

namespace te
{
  namespace core
  {

// Forward declaration
    class CppPlugin;
    class Library;

    /*!
      \class CppPluginProxy

      \brief A proxy class for C++ plugins.
     */
    class CppPluginProxy : public AbstractPlugin
    {
      public:

        /*! \brief Constructor. */
        CppPluginProxy(const std::shared_ptr<Library>& slib,
                       std::unique_ptr<CppPlugin> real_plugin);

        /*! \brief Destructor. */
        ~CppPluginProxy();

        /*!
          \brief It returns the PluginInfo of the CppPlugin

          \return The CppPlugin information
         */
        const PluginInfo& info() const;

        /*!
          \brief It returns true or false if the CppPlugin was initialized

          \return The current state of the CppPlugin
         */
        bool initialized() const;

        /*!
          \brief It starts the CppPlugin

          \exception PluginStartupException It may throws an exception if the CppPlugin cannot be started.
         */
        void startup();

        /*!
          \brief It shuts down the CppPlugin

          \exception PluginShutdownException It may throws an exception if the CppPlugin cannot be shutdown.
         */
        void shutdown();

      private:

        struct Impl;

        Impl* m_pimpl;
    };

  } // end namespace core
}   // end namespace te

#endif  // __TERRALIB_CORE_PLUGIN_CPPPLUGINPROXY_H__
