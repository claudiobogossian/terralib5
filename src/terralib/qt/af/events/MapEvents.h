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
  \file MapEvents.h

  \brief Contains a list of the map display events.
*/

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_MAPEVENTS_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_MAPEVENTS_H

// TerraLib
#include "../connectors/MapDisplay.h"
#include "Event.h"
#include "Enums.h"

// STL
#include <string>
#include <utility>

// Qt
#include <QtGui/QColor>

namespace te
{
  namespace qt
  {
    namespace af
    {
      namespace evt
      {
        /*!
          \struct MapSRIDChanged

          \brief This event signals that the srid of the map display changed.
         */
        struct MapSRIDChanged : public Event
        {
          /*!
            \brief Constructor.

            \param srid The new SRID of map display.
          */
          MapSRIDChanged(std::pair<int, std::string> srid)
            : Event(MAP_SRID_CHANGED),
              m_srid(srid)
          {
          }

          std::pair<int, std::string> m_srid;
        };

        /*!
          \struct MapColorChanged

          \brief This event signals that the color of the map display changed.
        */
        struct MapColorChanged : public Event
        {
          /*!
            \brief Constructor.

            \param color The new color of map display.
          */
          MapColorChanged(QColor color)
            : Event(MAP_COLOR_CHANGED),
              m_color(color)
          {
          }

          QColor m_color;
        };

        /*!
          \struct GetMapDisplay

          \brief This event can be used to retrieve the MapDisplat component.
        */
        struct GetMapDisplay : public Event
        {
          /*!
            \brief Constructor.
          */
          GetMapDisplay()
            : Event(GET_MAPDISPLAY),
              m_display(0)
          {
          }

          te::qt::af::MapDisplay* m_display;
        };
      }
    }
  }
}

#endif //__TERRALIB_QT_AF_EVENTS_INTERNAL_MAPEVENTS_H
