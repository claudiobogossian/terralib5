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
  \file DisplayResized.h
  \brief This event signals that a te::qt::widgets::MapDisplay object was resized.
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
 */

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_DISPLAYRESIZED_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_DISPLAYRESIZED_H

//! TerraLib include files
#include "Enums.h"
#include "Event.h"

//! Qt include files
#include <QSize>

//! Forward declarations
namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
    }
  }
}


namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class DisplayResized
        \copydoc DisplayResized.h
        \ingroup afevents
      */
      class DisplayResized : public Event
      {
        public:

          /*!
            \brief Constructor.
            \param display The te::qt::widgets::MapDisplay object associated.
            \param newSize The new size of the display.
            \param oldSize The old size of the display.
          */
          DisplayResized(te::qt::widgets::MapDisplay* display, const QSize& newSize, const QSize& oldSize) : 
          Event(te::qt::af::evt::DISPLAY_RESIZED),
          m_display(display),
          m_newSize(newSize),
          m_oldSize(oldSize)
          {
          }

          /*!
            \brief Destructor.
          */
          ~DisplayResized()
          {
          }

          te::qt::widgets::MapDisplay* m_display;   //!< Pointer to associated te::qt::widgets::MapDisplay.
          QSize m_newSize;                          //!< New size of the display.
          QSize m_oldSize;                          //!< Old size of the display.
      };
    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_EVENTS_INTERNAL_DISPLAYRESIZED_H

