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
  \file terralib/qt/plugins/rp/Plugin.h

  \brief Plugin implementation for the RP Qt Plugin widget.
*/

#ifndef __TE_QT_PLUGINS_RP_INTERNAL_PLUGIN_H
#define __TE_QT_PLUGINS_RP_INTERNAL_PLUGIN_H

// TerraLib
#include "../../../plugin/Plugin.h"
#include "Config.h"

// Qt
#include <QAction>
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
      namespace rp
      {
        class ArithmeticOpAction;
        class ClassifierAction;
        class ClippingAction;
        class ColorTransformAction;
        class ComposeBandsAction;
        class ContrastAction;
        class FilterAction;
        class FusionAction;
        class MixtureModelAction;
        class MosaicAction;
        class RasterizationAction;
        class SegmenterAction;
        class RegisterAction;
        class VectorizationAction;

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

            QMenu* m_rpMenu;                                         //!< RP Main Menu registered.
            QMenu* m_rpPopupMenu;                                    //!< RP Main Menu registered.
            QAction* m_popupAction;                                  //!< RP pop up action registered.

            te::qt::plugins::rp::ArithmeticOpAction* m_arithmeticOp;  //!< Arithmetic Operation Process Action
            te::qt::plugins::rp::ClassifierAction* m_classifier;      //!< Classifier Process Action
            te::qt::plugins::rp::ClippingAction* m_clipping;          //!< Clipping Process Action
            te::qt::plugins::rp::ColorTransformAction* m_colorTrans;  //!< Color Transform Process Action
            te::qt::plugins::rp::ComposeBandsAction* m_composeBands;  //!< Compose Bands Process Action
            te::qt::plugins::rp::ContrastAction* m_contrast;          //!< Contrast Process Action
            te::qt::plugins::rp::FilterAction* m_filter;              //!< Filter Process Action
            te::qt::plugins::rp::FusionAction* m_fusion;              //!< Fusion Process Action
            te::qt::plugins::rp::MixtureModelAction* m_mixtureModel;  //!< Mixture Model Process Action
            te::qt::plugins::rp::MosaicAction* m_mosaic;              //!< Mosaic Process Action
            te::qt::plugins::rp::SegmenterAction* m_segmenter;        //!< Segmenter Process Action
            te::qt::plugins::rp::RasterizationAction* m_raster;       //!< Rasterization Process Action
            te::qt::plugins::rp::RegisterAction* m_register;          //!< Register Process Action
            te::qt::plugins::rp::VectorizationAction* m_vector;       //!< Vectorization Process Action
        };

      } // end namespace rp
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEQTPLUGINRPEXPORT);

#endif //__TE_QT_PLUGINS_RP_INTERNAL_PLUGIN_H
