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
  \file terralib/qt/plugins/vp/Plugin.h

  \brief Plugin implementation for the VP Qt Plugin widget.
*/

#ifndef __TE_QT_PLUGINS_VP_INTERNAL_PLUGIN_H
#define __TE_QT_PLUGINS_VP_INTERNAL_PLUGIN_H

// TerraLib
#include "../../../plugin/Plugin.h"
#include "Config.h"

// Qt
#include <QtGui/QMenu>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace vp
      {
        class AggregationAction;
        class BufferAction;
        class GeometricOpAction;
        class IntersectionAction;
        //class PolygonToLineAction;
        //class SummarizationAction;
        //class TransformationAction;

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

            QMenu* m_vpMenu;                                    //!< VP Main Menu registered.

            te::qt::plugins::vp::AggregationAction* m_aggregation;    //!< Aggregation Operation Action
            te::qt::plugins::vp::GeometricOpAction* m_geometricOp;    //!< Geometric Operation
            te::qt::plugins::vp::BufferAction* m_buffer;    //!< Buffer Operation Action
            te::qt::plugins::vp::IntersectionAction* m_intersection;    //!< Intersection Operation Action
            //te::qt::plugins::vp::PolygonToLineAction* m_polygonToLine;    //!< Polygon to Line Operation Action
            //te::qt::plugins::vp::SummarizationAction* m_summarization;    //!< Summarization Operation Action
            //te::qt::plugins::vp::TransformationAction* m_transformation;    //!< Transformation Operation Action

        };

      } // end namespace vp
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEQTPLUGINVPEXPORT);

#endif //__TE_QT_PLUGINS_VP_INTERNAL_PLUGIN_H
