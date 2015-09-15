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
  \file src/terraMobilePlugin/Plugin.h

  \brief Plugin implementation for the Terra Mobile Qt Plugin widget.
*/

#ifndef __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_PLUGIN_H
#define __TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_PLUGIN_H

// TerraLib
#include <terralib/plugin/Plugin.h>
#include "Config.h"

// Qt
#include <QAction>
#include <QMenu>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace terramobile
      {
        class GeoPackageBuilderAction;
        class GeoPackagePublisherAction;
        class GatheringLayerConfigurerAction;

        
        class Plugin : public te::plugin::Plugin
        {
          public:

            Plugin(const te::plugin::PluginInfo& pluginInfo);

            ~Plugin();

            void startup();

            void shutdown();

          protected:

            /*!
              \brief Function used to register all terra mobile actions.

            */
            void registerActions();

            /*!
              \brief Function used to unregister all terra mobile actions.

            */
            void unRegisterActions();

          protected:

            QMenu* m_menu;                                           //!< thirdParty Main Menu registered.
            QAction* m_popupAction;                                  //!< thirdParty pop up action registered.

            te::qt::plugins::terramobile::GeoPackageBuilderAction* m_gpBuider;              //!< GeoPackage Builder Operation Process Action
            te::qt::plugins::terramobile::GeoPackagePublisherAction* m_gpPublisher;         //!< GeoPackage Publisher Operation Process Action
            te::qt::plugins::terramobile::GatheringLayerConfigurerAction* m_glConfiguer;    //!< Gathering Layer Configurer Operation Process Action
        };

      } // end namespace thirdParty
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TERRAMOBILEPLUGINSDLLEXPORT);

#endif //__TE_QT_PLUGINS_TERRAMOBILE_INTERNAL_PLUGIN_H
