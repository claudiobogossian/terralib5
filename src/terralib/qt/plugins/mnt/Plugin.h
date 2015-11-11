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
  \file terralib/qt/plugins/mnt/Plugin.h

  \brief Plugin implementation for the MNT Qt Plugin widget.
*/

#ifndef __TE_QT_PLUGINS_MNT_INTERNAL_PLUGIN_H
#define __TE_QT_PLUGINS_MNT_INTERNAL_PLUGIN_H

// TerraLib
#include "../../../plugin/Plugin.h"
#include "Config.h"

// Qt
#include <QMenu>

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
      namespace mnt
      {
        class CreateIsolinesAction;
        class MNTGenerationAction;
        class ShowValuesAction;
        class SlopeAction;
        class SmoothAction;
        class TINGenerationAction;

        class Plugin : public QObject, public te::plugin::Plugin
        {
          Q_OBJECT

        public:

            Plugin(const te::plugin::PluginInfo& pluginInfo);

            ~Plugin();

            void startup();

            void shutdown();

          protected:

            /*!
              \brief Function used to register all raster processing actions.

            */
            void registerActions();

            /*!
              \brief Function used to unregister all raster processing actions.

            */
            void unRegisterActions();

          Q_SIGNALS:

            void triggered(te::qt::af::evt::Event* e);


          protected:

            QMenu* m_mntMenu;                                    //!< MNT Main Menu registered.

            CreateIsolinesAction* m_ISOGeneration;    //!< Isoline Generation Action
            MNTGenerationAction*  m_MNTGeneration;    //!< GRID Generation Action
            ShowValuesAction*     m_ShowValue;
            SlopeAction*          m_Slope;            //!< Slope Generation Action
            SmoothAction*         m_Smooth;           //!< Smooth Isolines Action
            TINGenerationAction*  m_TINGeneration;    //!< TIN Generation Action
        };
      } // end namespace mnt
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEQTPLUGINMNTEXPORT);

#endif //__TE_QT_PLUGINS_MNT_INTERNAL_PLUGIN_H
