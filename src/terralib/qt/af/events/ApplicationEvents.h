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
  \file ApplicationEvents.h

  \brief Contains the list of the application events.
*/

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_APPLICATIONREVENTS_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_APPLICATIONREVENTS_H

#include "Event.h"
#include "Enums.h"

// Qt
#include <QSize>
#include <QList>

// Forward declarations
class QToolBar;
class QAction;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class MapDisplay;
      class LayerItemView;
    }

    namespace af
    {
      namespace evt
      {
        /*!
          \struct AppClosed

          \brief This event signals that the application is about to close.
        */
        struct AppClosed : public Event
        {
          /*!
            \brief Constructor.
          */
          AppClosed() : 
          Event(APP_CLOSED)
          {
          }
        };

        /*!
          \struct DisplayResized

          \brief This event signals that a display was resized.
        */
        struct DisplayResized : public Event
        {
          /*!
            \brief Constructor.

            \param display The pointer to the display resized.

            \param newSize The new size of the display.

            \param oldSize The old size if the display.
          */
          DisplayResized(te::qt::widgets::MapDisplay* display, const QSize& newSize, const QSize& oldSize) :
          Event(DISPLAY_RESIZED),
          m_display(display),
          m_newSize(newSize),
          m_oldSize(oldSize)
          {
          }

          te::qt::widgets::MapDisplay* m_display;   //!< Pointer to associated te::qt::widgets::MapDisplay.
          QSize m_newSize;                          //!< New size of the display.
          QSize m_oldSize;                          //!< Old size of the display.
        };

        /*!
          \struct ToolBarAdded

          \brief This event signals that a new toolbar was added.
        */

        struct ToolBarAdded : public Event
        {
          /*!
            \brief Constructor.

            \param toolbar Pointer to the new tool bar.
          */
          ToolBarAdded(QToolBar* toolbar) :
          Event(TOOLBAR_ADDED),
          m_toolbar(toolbar)
          {
          }

          QToolBar* m_toolbar;  //!< Pointer to the new QToolBar.
        };

        /*!
          \struct Draw Button Clicked

          \brief This event signals that the draw button clicked
        */

        struct DrawButtonClicked : public Event
        {
          /*!
            \brief Constructor.
          */
          DrawButtonClicked() :
          Event(DRAW_BUTTON_CLICKED)
          {
          }
        };

        /*!
          \struct Zoom In Button Toggled

          \brief This event signals that the zoom in button toggled
        */

        struct ZoomInButtonToggled : public Event
        {
          /*!
            \brief Constructor.
          */
          ZoomInButtonToggled() :
          Event(ZOOM_IN_BUTTON_TOGGLED)
          {
          }
        };

        /*!
          \struct Zoom Out Button Toggled

          \brief This event signals that the zoom out button toggled
        */

        struct ZoomOutButtonToggled : public Event
        {
          /*!
            \brief Constructor.
          */
          ZoomOutButtonToggled() :
          Event(ZOOM_OUT_BUTTON_TOGGLED)
          {
          }
        };

        /*!
          \struct Pan Button Toggled

          \brief This event signals that the pan button toggled
        */

        struct PanButtonToggled : public Event
        {
          /*!
            \brief Constructor.
          */
          PanButtonToggled() :
          Event(PAN_BUTTON_TOGGLED)
          {
          }
        };

        /*!
          \struct Selection Button Toggled

          \brief This event signals that the selection button toggled
        */

        struct SelectionButtonToggled : public Event
        {
          /*!
            \brief Constructor.
          */
          SelectionButtonToggled() :
          Event(SELECTION_BUTTON_TOGGLED)
          {
          }
        };

        struct NewActionsAvailable : public Event
        {
          NewActionsAvailable() :
          Event(NEW_ACTIONS_AVAILABLE),
          m_toolbar(0)
          {

          }

          std::string m_plgName;
          std::string m_category;
          QToolBar* m_toolbar;
          QList<QAction*> m_actions;
        };

        struct GetLayerExplorer : public Event
        {
          GetLayerExplorer() :
            Event(GET_LAYERS_VIEW),
            m_layerExplorer(0)
          {

          }

          te::qt::widgets::LayerItemView* m_layerExplorer;
        };
      }
    }
  }
}

#endif //__TERRALIB_QT_AF_EVENTS_INTERNAL_APPLICATIONREVENTS_H
