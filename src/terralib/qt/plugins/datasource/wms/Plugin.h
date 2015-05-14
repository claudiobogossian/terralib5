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
  \file terralib/qt/plugins/datasource/wms/Plugin.h

  \brief Plugin implementation for the OGC Web Map Service (WMS) data source widget.
*/

#ifndef __TE_QT_PLUGINS_DATASOURCE_WMS_INTERNAL_PLUGIN_H
#define __TE_QT_PLUGINS_DATASOURCE_WMS_INTERNAL_PLUGIN_H

// TerraLib
#include "../../../../plugin/Plugin.h"
#include "Config.h"

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace wms
      {
        class Plugin : public te::plugin::Plugin
        {
          public:

            Plugin(const te::plugin::PluginInfo& pluginInfo);

            ~Plugin();

            void startup();

            void shutdown();
        };

      } // end namespace wms
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEQTPLUGINWMSEXPORT);

#endif //__TE_QT_PLUGINS_DATASOURCE_WMS_INTERNAL_PLUGIN_H
