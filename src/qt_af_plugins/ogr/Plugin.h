/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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

#ifndef __TE_QTAF_PLUGIN_OGR_INTERNAL_PLUGIN_H
#define __TE_QTAF_PLUGIN_OGR_INTERNAL_PLUGIN_H

#include "Config.h"

//! Terralib include files
#include <terralib/plugin/Plugin.h>

namespace qt_af
{
  namespace plugin
  {
    namespace ogr
    {
      class Plugin : public te::plugin::Plugin
      {
        public:
          Plugin(const te::plugin::PluginInfo& pluginInfo);

          ~Plugin();

          void startup();

          void shutdown();
      };
    }
  }
}

PLUGIN_CALL_BACK_DECLARATION(TEAFPLUGINEXPORT);

#endif //__TE_QTAF_PLUGIN_GDAL_INTERNAL_PLUGIN_H
