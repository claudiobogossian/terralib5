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
  \file terralib/sqlite/Module.h

  \brief The TerraLib SQLite Data Access driver implements the plugin interface.
*/

#ifndef __TERRALIB_SQLITE_INTERNAL_MODULE_H
#define __TERRALIB_SQLITE_INTERNAL_MODULE_H

// TerraLib
#include "../plugin/Plugin.h"

namespace te
{
  namespace sqlite
  {
    class Module : public te::plugin::Plugin
    {
      public:

        Module(const te::plugin::PluginInfo& pluginInfo);

        ~Module();

        void startup();

        void shutdown();
    };

  }
}

#define TESQLITEEXPORT  __declspec(dllexport)

PLUGIN_CALL_BACK_DECLARATION(TESQLITEEXPORT);

#endif  // __TERRALIB_SQLITE_INTERNAL_MODULE_H

