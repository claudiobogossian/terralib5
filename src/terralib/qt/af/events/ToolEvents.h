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
  \file ToolsEvents.h

  \brief Contains the list of the tools related events.
*/

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_TOOLSEVENTS_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_TOOLSEVENTS_H

// TerraLib
#include "Event.h"
#include "Enums.h"

namespace te
{
  namespace qt
  {
// Forward declaration
    namespace widgets
    {
      class AbstractTool;
    }

    namespace af
    {
      namespace evt
      {
        /*!
          \struct ToolChanged

          \brief This event signals that the tool being used, on current map display, changed.
        */
        struct ToolChanged : public Event
        {

          /*!
            \brief Constructor.

            \param newTool Pointer to the new tool to be used.

            \param oldTool Pointer to the previously used tool.
          */
          ToolChanged(te::qt::widgets::AbstractTool* newTool, te::qt::widgets::AbstractTool* oldTool)
            : Event(TOOL_CHANGED),
              m_oldTool(oldTool),
              m_newTool(newTool)
          {
          }

          te::qt::widgets::AbstractTool* m_oldTool; //!< Previously tool being used.
          te::qt::widgets::AbstractTool* m_newTool; //!< The new tool to be used.
        };

        /*!
          \struct CoordinateTracked

          \brief Signals a mouse moved over the current display.
        */
        struct CoordinateTracked : public Event
        {
          /*!
            \brief Constructor.

            \param x The x coordinate value.
            \param y The y coordinate value.
          */
          CoordinateTracked(const double& x, const double& y)
            : Event(COORDINATE_TRACKED),
              m_x(x),
              m_y(y)
          {
          }

          double m_x; //!< The x coordinate value.
          double m_y; //!< The y coordinate value.
        };
      }
    }
  }
}

#endif //__TERRALIB_QT_AF_EVENTS_INTERNAL_TOOLSEVENTS_H