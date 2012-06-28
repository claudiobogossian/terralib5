/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/mongodb/Plugin.h

  \brief An utility class to load MongoDB driver as a plugin.
*/

#ifndef __TERRALIB_MONGODB_INTERNAL_MODULE_H
#define __TERRALIB_MONGODB_INTERNAL_MODULE_H

// TerraLib
#include "../plugin/Plugin.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace mongodb
  {
    /*!
      \class Plugin

      \brief An utility class to load MongoDB driver as a plugin.

      \sa te::plugin::Plugin
    */
    class Plugin : public te::plugin::Plugin
    {
      public:

        /*!
          \brief Plugin constructor.

          \param pInfo Basic information provided to initialize this module through the plugin API.
        */
        Plugin(const te::plugin::PluginInfo& pluginInfo);

        /*! \brief Destructor. */
        ~Plugin();

        /*! 
          \brief It initializes the TerraLib MongoDB driver support.

          \exception Exception It may throws an exception.

          \warning Not thread safe!
        */
        void startup();

        /*!
          \brief It finalizes the TerraLib MongoDB driver support.

          \exception Exception It may throws an exception.

          \warning Not thread safe!
        */
        void shutdown();
    };

  } // end namespace mongodb
}   // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEMONGODBEXPORT);

#endif  // __TERRALIB_MONGODB_INTERNAL_MODULE_H

