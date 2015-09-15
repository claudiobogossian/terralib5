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
  \file terralib/qt/plugins/st/Plugin.h

  \brief Plugin implementation for the ST Qt Plugin widget.
*/

#ifndef __TE_QT_PLUGINS_ST_INTERNAL_PLUGIN_H
#define __TE_QT_PLUGINS_ST_INTERNAL_PLUGIN_H

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
      { // Forward declarations
        struct Event;
      }
    }

    namespace plugins
    {
      namespace st
      {
        class TimeSliderWidgetAction;
        class ObservationAction;
        class TimeSeriesAction;
        class TrajectoryAction;
        class STItemDelegate;

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

          void updateDelegate(const bool& add);

          QMenu*                                        m_stMenu;              //!< ST Main Menu registered.
          te::qt::plugins::st::TimeSliderWidgetAction*  m_sliderAction;        //!< Slider Process Action
          te::qt::plugins::st::ObservationAction*       m_observactionAction;  //!< Observation Layer Action
          te::qt::plugins::st::TimeSeriesAction*        m_timeSeriesAction;    //!< TimeSeries Layer Action
          te::qt::plugins::st::TrajectoryAction*        m_trajectoryAction;    //!< Trajectory Layer Action
          STItemDelegate*                               m_delegate;            //!< Item delegate.
        };

      } // end namespace st
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEQTPLUGINSTEXPORT)

#endif //__TE_QT_PLUGINS_ST_INTERNAL_PLUGIN_H
