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
  \file terralib/qt/plugins/attributefill/Plugin.h

  \brief Plugin implementation for the SA Qt Plugin widget.
*/

#ifndef __TE_QT_PLUGINS_ATTRIBUTEFILL_INTERNAL_PLUGIN_H
#define __TE_QT_PLUGINS_ATTRIBUTEFILL_INTERNAL_PLUGIN_H

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
    namespace plugins
    {
      namespace attributefill
      {
        class RasterToVectorAction;
        class VectorToRasterAction;

        class Plugin : public te::plugin::Plugin
        {
          public:

            Plugin(const te::plugin::PluginInfo& pluginInfo);

            ~Plugin();

            void startup();

            void shutdown();

          protected:

            /*!
              \brief Function used to register all attributefill actions.

            */
            void registerActions();

            /*!
              \brief Function used to unregister all attributefill actions.

            */
            void unRegisterActions();

          protected:

            QMenu* m_attributefillMenu;                               //!< Attribute Fill Sub Menu registered.
            QAction* m_popupAction;                                   //!< Attribute Fill pop up action registered.

            RasterToVectorAction* m_rasterToVector;                   //!< RasterToVector Attribute Fill Action
            VectorToRasterAction* m_vectorToRaster;                   //!< VectorToRaster Attribute Fill Action
            VectorToVectorAction* m_vectorToVector;                   //!< VectorToVector Attribute Fill Action
        };

      } // end namespace attributefill
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

PLUGIN_CALL_BACK_DECLARATION(TEQTPLUGINATTRIBUTEFILLEXPORT);

#endif //__TE_QT_PLUGINS_ATTRIBUTEFILL_INTERNAL_PLUGIN_H
