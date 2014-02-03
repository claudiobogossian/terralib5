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
  \file terralib/qt/plugins/rp/Plugin.h

  \brief Plugin implementation for the RP Qt Plugin widget.
*/

#ifndef __TE_QT_PLUGINS_RP_INTERNAL_PLUGIN_H
#define __TE_QT_PLUGINS_RP_INTERNAL_PLUGIN_H

// TerraLib
#include "../../../plugin/Plugin.h"
#include "Config.h"

// Qt
#include <QtGui/QAction>
#include <QtGui/QMenu>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace rp
      {
        class ClassifierAction;
        class ContrastAction;
        class FilterAction;
        class MixtureModelAction;
        class SegmenterAction;
        class RegisterAction;

        class Plugin : public te::plugin::Plugin
        {
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

          protected:

            QMenu* m_rpMenu;                                        //!< RP Main Menu registered.
            QMenu* m_rpPopupMenu;                                   //!< RP Main Menu registered.
            QAction* m_popupAction;                                 //!< RP pop up action registered.

            te::qt::plugins::rp::ClassifierAction* m_classifier;    //!< Classifier Process Action
            te::qt::plugins::rp::ContrastAction* m_contrast;        //!< Contrast Process Action
            te::qt::plugins::rp::FilterAction* m_filter;            //!< Filter Process Action
            te::qt::plugins::rp::MixtureModelAction* m_mixtureModel;//!< Contrast Process Action
            te::qt::plugins::rp::SegmenterAction* m_segmenter;      //!< Segmenter Process Action
            te::qt::plugins::rp::RegisterAction* m_register;        //!< Register Process Action

        };

      } // end namespace rp
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEQTPLUGINRPEXPORT);

#endif //__TE_QT_PLUGINS_RP_INTERNAL_PLUGIN_H
