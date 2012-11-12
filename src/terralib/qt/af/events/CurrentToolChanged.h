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
  \file CurrentToolChanged.h
  \brief This event signals that the tool being used, on current map display, changed.
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
 */

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_CURRENTTOOLCHANGED_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_CURRENTTOOLCHANGED_H

// TerraLib
#include "Enums.h"
#include "Event.h"

namespace te
{
  namespace qt
  {
    //! Forward declarations
    namespace widgets
    {
      class AbstractTool;
    }

    namespace af
    {
      /*!
        \class CurrentToolChanged
        \copydoc CurrentToolChanged.h
        \ingroup afevents
      */
      class CurrentToolChanged : public Event
      {
        public:

          /*!
            \brief Constructor.
            \param newTool The tool that will be used.
            \param oldTool The tool that are being used.
          */
          CurrentToolChanged(te::qt::widgets::AbstractTool* newTool, te::qt::widgets::AbstractTool* oldTool) : 
          Event(te::qt::af::evt::TOOL_CHANGED),
          m_oldTool(oldTool),
          m_newTool(newTool)
          {
          }

          /*!
            \brief Destructor.
          */
          ~CurrentToolChanged()
          {
          }

          te::qt::widgets::AbstractTool* m_oldTool;     //!< Previously tool being used.
          te::qt::widgets::AbstractTool* m_newTool;     //!< The new tool to be used.
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_EVENTS_INTERNAL_CURRENTTOOLCHANGED_H

