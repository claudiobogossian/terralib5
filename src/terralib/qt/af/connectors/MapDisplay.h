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
  \file connectors/MapDisplay.h

  \brief A connector of the te::qt::widgets::MapDisplay for the application framework.
 */

#ifndef __TERRALIB_QT_AF_INTERNAL_MAPDISPLAY_H
#define __TERRALIB_QT_AF_INTERNAL_MAPDISPLAY_H

//! Terralib include files
#include <terralib/qt/af/Config.h>

//! Qt include files
#include <QObject>
#include <QColor>

//! STL include files
#include <map>
#include <vector>

//! Forward declarations
class QPointF;

namespace te
{
  //! Forward declarations
  namespace map
  {
    class AbstractLayer;
  }

  namespace gm
  {
    class Geometry;
  }

  namespace qt
  {
    //! Forward declarations
    namespace widgets
    {
      class MapDisplay;
      class AbstractTool;
    }

    namespace af
    {
      //! Forward declarations
      class Event;

      /*!
        \enum HLGROUP
        \brief Defines the groups of highlighted objects to be used.
      */
      enum HLGROUP
      {
        QUERIED,              //!< Queried items.
        POINTED,              //!< Pointed items.
        POINTED_AND_QUERIED,  //!< Pointed and queried items.
        USER_DEFINED          //!< User groups must have values greater than this.
      };

      /*!
        \class MapDisplay
        
        \brief A connector of the te::qt::widgets::MapDisplay for the application framework.
  
        This is used to listen events sent by application framework and acts on a te::qt::widgets::MapDisplay object and vice-versa.
        
        \ingroup afconnector
      */
      class TEQTAFEXPORT MapDisplay : public QObject
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.
            \param display The te::qt::widgets::MapDisplay to be presented.
          */
          MapDisplay(te::qt::widgets::MapDisplay* display);

          /*!
            \brief destructor.
          */
          ~MapDisplay();

          /*!
            \brief Reimplementation of the QObject method.
            \param watched The object that raises the event. 
            \param event The event sent.
          */
          bool eventFilter (QObject* watched, QEvent* evt);

          /*!
            \brief Draws the layers.
            \param layers Map containing ordered layers.
          */
          void drawLayers(const std::map<int, te::map::AbstractLayer*>& layers);

          /*!
            \brief Updates the current tool being used on te::qt::widgets::MapDisplay.
            \param tool The new te::qt::widgets::AbstractTool.
            \note This object takes the ownership of the tool.
          */
          void setCurrentTool(te::qt::widgets::AbstractTool* tool);

          /*!
            \brief Returns the componet mapdisplay being assisted.
            \return The pointer to MapDisplay.
          */
          te::qt::widgets::MapDisplay* getDisplayComponent();

        protected slots:

          /*!
            \brief Listener to the application framewrork events.
            \param evt Event sent by framework.
          */
          void onApplicationTriggered(te::qt::af::Event* evt);

          /*!
            \brief Dispatches an Event to framework sinalizing the coord tracking.
            \param pos The mouse coordinate as coordinate in world reference system.
          */
          void onCoordTracked(QPointF& pos);

          /*!
            \brief Redraws the objects highlighted.
            \details Uses the draft pixmap of the MapDisplay to draw the highlighted objects. It will be used to compose the image to be shown on
            MapDisplay.
          */
          void redrawHighlight();

        protected:

          te::qt::widgets::MapDisplay* m_display;                 //!< Pointer to a component MapDisplay.
          te::qt::widgets::AbstractTool* m_current_tool;          //!< Pointer to the current tool being used.
          std::vector< std::vector<te::gm::Geometry*> > m_geoms;  //!< Vector of geometries in each group of a layer.
          std::vector<QColor> m_colors;                           //!< Colors of each group.
          bool m_hl_visible;                                      //!< Layer visibility.
      };
    }
  }
}

#endif //! __TERRALIB_QT_AF_INTERNAL_MAPDISPLAY_H
