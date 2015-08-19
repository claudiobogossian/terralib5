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
  \file terralib/qt/plugins/edit/Plugin.h

  \brief Plugin implementation for the TerraLib Edit Qt Plugin.
*/

#ifndef __TE_QT_PLUGINS_EDIT_INTERNAL_PLUGIN_H
#define __TE_QT_PLUGINS_EDIT_INTERNAL_PLUGIN_H

// TerraLib
#include "../../../plugin/Plugin.h"
#include "Config.h"

// Qt
#include <QAction>
#include <QMenu>

class EditDelegate;

namespace te
{
  namespace map
  {
    class AbstractLayer;
  }

  namespace qt
  {
    namespace widgets
    {
      class LayerItemView;
    }

    namespace plugins
    {
      namespace edit
      {
        // Forward declaration
        class ToolBar;

        class Plugin : public QObject, public te::plugin::Plugin
        {
          Q_OBJECT

        public:

          Plugin(const te::plugin::PluginInfo& pluginInfo);

          ~Plugin();

          void startup();

          void shutdown();

        protected slots:
          /*!
            \brief Slot function used when a action was selected.

            \param checked Flag used in case a toggle action.
            */
          void onActionActivated(bool checked);

          void onApplicationTriggered(te::qt::af::evt::Event* e);

          void onStashedLayer(te::map::AbstractLayer* layer);

          void onGeometriesChanged();

        Q_SIGNALS:

          void triggered(te::qt::af::evt::Event* e);

        protected:

          void drawStashed();

          void updateDelegate(const bool& add);

          te::qt::widgets::LayerItemView* getLayerExplorer();

          ToolBar* m_toolbar; //!< Main toolbar of TerraLib Edit Qt Plugin.
          QMenu* m_menu;      //!< Main menu of TerraLib Edit Qt Plugin.
          QAction* m_action;    //!< Action used to call the process

          EditDelegate* m_delegate;

          //void createAction(std::string name, std::string pixmap = "");
        };

      } // end namespace edit
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEQTPLUGINEDITEXPORT)

#endif //__TE_QT_PLUGINS_EDIT_INTERNAL_PLUGIN_H
