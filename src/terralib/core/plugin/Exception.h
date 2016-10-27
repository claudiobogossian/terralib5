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
  \file te/core/plugin/Exception.h

  \brief Specific exception types for handling plugins.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __TERRALIB_CORE_PLUGIN_EXCEPTION_H__
#define __TERRALIB_CORE_PLUGIN_EXCEPTION_H__

// TerraLib
#include "../Exception.h"

namespace te
{
  namespace core
  {
    //! An exception indicating an error when loading a plugin.
    struct PluginLoadException: virtual Exception { };

    //! An exception indicating an error when unloading a plugin.
    struct PluginUnloadException: virtual Exception { };

    //! An exception indicating an error when trying to start a plugin.
    struct PluginStartupException: virtual Exception { };

    //! An exception indicating an error when shutting down a plugin.
    struct PluginShutdownException: virtual Exception { };

    //! An exception indicating an error when loading a plugin.
    struct PluginEngineLoadException: virtual Exception { };

    //! An exception indicating an error when releasing a given plugin.
    struct PluginEngineUnloadException: virtual Exception { };

    //! An exception indicating an error when a dependecy is missing
    struct PluginMissingDependencyException: virtual Exception { };

    //! An exception indicating an error when two or more plugins have cyclic
    //! dependecy
    struct PluginCyclicDependencyException: virtual Exception { };

    //! An exception indicating an error when trying to shutdown a plugin with a
    //! dependent
    struct PluginHasDependentException: virtual Exception { };

    //! An exception indicating that a plugin serializer with the same name was
    //already registered in the system.
    // struct plugin_serializer_already_registered_error: virtual exception { };

  }  // end namespace core
}    // end namespace te

#endif  // __TERRALIB_CORE_PLUGIN_EXCEPTION_H__
