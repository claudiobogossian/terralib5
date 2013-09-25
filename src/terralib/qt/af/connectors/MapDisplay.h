/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*! 
  \file terralib/qt/af/connectors/MapDisplay.h

  \brief A connector of the te::qt::widgets::MapDisplay class to the Application Framework.
*/

#ifndef __TERRALIB_QT_AF_CONNECTORS_INTERNAL_MAPDISPLAY_H
#define __TERRALIB_QT_AF_CONNECTORS_INTERNAL_MAPDISPLAY_H

// Terralib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// Qt
#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtGui/QMenu>
#include <QtGui/QPixmap>

namespace te
{
// Forward declaration
  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
      class AbstractTool;
    }

    namespace af
    {
      namespace evt
      {
      // Forward declaration
        struct Event;
      }

      /*!
        \class MapDisplay
        
        \brief A connector of the te::qt::widgets::MapDisplay class to the Application Framework.

        It is used to listen events sent by the application framework.
        It is a proxy for the te::qt::widgets::MapDisplay.

        \ingroup afconnector
      */
      class TEQTAFEXPORT MapDisplay : public QObject
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param display te::qt::widgets::MapDisplay to be listened.
          */
          MapDisplay(te::qt::widgets::MapDisplay* display);

          /*! \brief destructor. */
          ~MapDisplay();

          te::qt::widgets::MapDisplay* getDisplay();

          bool eventFilter(QObject* watched, QEvent* e);

          /*!
            \brief It draws the given layer list.

            \param layers The layer list.
          */
          void draw(const std::list<te::map::AbstractLayerPtr>& layers);

          /*! \brief It clears the map display. */
          void clear();

          /*!
            \brief Updates the current tool being used on te::qt::widgets::MapDisplay.

            \param tool The new te::qt::widgets::AbstractTool.

            \note The class will take the ownership of the given pointer.
          */
          void setCurrentTool(te::qt::widgets::AbstractTool* tool);

          void nextExtent();

          void previousExtent();

          void fit(const std::list<te::map::AbstractLayerPtr>& layers);

        protected slots:

          void onCoordTracked(QPointF& coordinate);

          void onDrawLayersFinished(const QMap<QString, QString>& errors);

          void onExtentChanged();

          /*!
            \brief Listener to the application framewrork events.

            \param e An event sent by framework.
          */
          void onApplicationTriggered(te::qt::af::evt::Event* e);

        protected:

          void drawLayersSelection(const std::list<te::map::AbstractLayerPtr>& layers);

          void drawLayerSelection(te::map::AbstractLayer* layer);

          void configSRS(const std::list<te::map::AbstractLayerPtr>& layers);

        protected:

          te::qt::widgets::MapDisplay* m_display;             //!< Pointer to a component te::qt::widgets::MapDisplay.
          te::qt::widgets::AbstractTool* m_tool;              //!< Pointer to the current tool being used.
          QMenu m_menu;                                       //!< The map display popup menu.
          QPixmap m_lastDisplayContent;                       //!< The last map display content. i.e. a "clean" pixmap.
          std::vector<te::gm::Envelope> m_extentStack;        //!< The stack of MapDisplay extents.
          std::size_t m_currentExtent;                        //!< The current extent index.
      };
    }
  }
}

#endif // __TERRALIB_QT_AF_CONNECTORS_INTERNAL_MAPDISPLAY_H
