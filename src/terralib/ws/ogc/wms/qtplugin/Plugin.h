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
  \file terralib/ws/ogc/wms/qtplugin/Plugin.h

  \brief Plugin implementation for the OGC Web Coverage Service (WMS) data source widget.
*/

#ifndef __TERRALIB_WS_OGC_WMS_QTPLUGIN_PLUGIN_H
#define __TERRALIB_WS_OGC_WMS_QTPLUGIN_PLUGIN_H

#include "../../../../plugin/Plugin.h"
#include "Config.h"

namespace te
{
  namespace ws
  {
    namespace ogc
    {
      namespace wms
      {
        namespace qtplugin
        {
          class Plugin : public te::plugin::Plugin
          {
            public:

              Plugin(const te::plugin::PluginInfo& pluginInfo);

              ~Plugin();

              void startup();

              void shutdown();
          };
        } // end namespace qtplugin
      }   // end namespace wms
    }     // end namespace ogc
  }       // end namespace ws
}         // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEOGCWMSQTPLUGINEXPORT)

#endif // __TERRALIB_WS_OGC_WMS_QTPLUGIN_PLUGIN_H
