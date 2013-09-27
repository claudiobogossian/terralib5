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
  \file ApplicationEvents.h

  \brief Contains the list of the application events.
*/

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_APPLICATIONREVENTS_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_APPLICATIONREVENTS_H

#include "Event.h"
#include "Enums.h"

// Qt
#include <QtCore/QSize>

// Forward declarations
class QToolBar;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class MapDisplay;
    }

    namespace af
    {
      namespace evt
      {
        /*!
          \struct AppClosed

          \brief This event signals that the application is about to close.
        
          \ingroup afevents
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

          \ingroup afevents
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

          \ingroup afevents
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

          \ingroup afevents
        */

        struct DrawButtonClicked : public Event
        {
          /*!
            \brief Constructor.

            \param toolbar Pointer to the new tool bar.
          */
          DrawButtonClicked() :
          Event(DRAW_BUTTON_CLICKED)
          {
          }
        };
      }
    }
  }
}

#endif //__TERRALIB_QT_AF_EVENTS_INTERNAL_APPLICATIONREVENTS_H