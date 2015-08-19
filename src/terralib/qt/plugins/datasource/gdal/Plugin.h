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
  \file terralib/qt/plugins/datasource/gdal/Plugin.h

  \brief Plugin implementation for the GDAL data source widget.
*/

#ifndef __TE_QT_PLUGINS_DATASOURCE_GDAL_INTERNAL_PLUGIN_H
#define __TE_QT_PLUGINS_DATASOURCE_GDAL_INTERNAL_PLUGIN_H

// TerraLib
#include "../../../../plugin/Plugin.h"
#include "Config.h"

// Qt
#include <QObject>

// Forward declarations
class QAction;

namespace te
{
  namespace qt
  {
    namespace af
    {
      namespace evt
      {
        struct Event;
      }
    }

    namespace plugins
    {
      namespace gdal
      {
        class Plugin : public QObject, public te::plugin::Plugin 
        {
          Q_OBJECT 

          public:

            Plugin(const te::plugin::PluginInfo& pluginInfo);

            ~Plugin();

            void startup();

            void shutdown();

          protected slots:

            void openFileDialog();

        signals:

            void triggered(te::qt::af::evt::Event* e);

          protected:

            QAction* m_openFile;
        };

      } // end namespace gdal
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEQTPLUGINGDALEXPORT)

#endif //__TE_QT_PLUGINS_DATASOURCE_GDAL_INTERNAL_PLUGIN_H
