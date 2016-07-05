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
  \file terralib/core/plugin/AbstractPlugin.h

  \brief The base class for plugins in TerraLib.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

#ifndef __TERRALIB_CORE_PLUGIN_ABSTRACTPLUGIN_H__
#define __TERRALIB_CORE_PLUGIN_ABSTRACTPLUGIN_H__

// TerraLib
#include "../Config.h"
#include "Exception.h"
#include "PluginInfo.h"

namespace te
{
  namespace core
  {

    /*!
      \class AbstractPlugin

      \brief The base class for plugins in TerraLib.
     */
    class AbstractPlugin
    {
      public:

        /*! \brief Default constructor. */
        AbstractPlugin() { }

        /*! \brief Virtual destructor. */
        virtual ~AbstractPlugin() = default;

        /*! \brief Plugin information. */
        virtual const PluginInfo& info() const = 0;

        /*! \brief Tells if the plugin has been started. */
        virtual bool initialized() const = 0;

        /*!
          \brief This method will be called by applications to startup some plugin's functionality.

          \exception PluginStartupException It may throws an exception.
         */
        virtual void startup() = 0;

        /*!
          \brief This method will be called by applicatons to shutdown plugin's functionality.

          \exception PluginShutdownException It may throws an exception.
         */
        virtual void shutdown() = 0;
    };

  }  // end namespace core
}    // end namespace te

#endif  // __TERRALIB_CORE_PLUGIN_ABSTRACTPLUGIN_H__
