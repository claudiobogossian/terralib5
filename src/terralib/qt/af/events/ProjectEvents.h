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
  \file ProjectEvents.h

  \brief Contains the list of the project events.
*/
#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_PROJECTEVENTS_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_PROJECTEVENTS_H

#include "Event.h"
#include "Enums.h"

namespace te
{
  namespace qt
  {
    namespace af
    {
      // Forward declarations
      class Project;

      namespace evt
      {
        /*!
          \struct ProjectAdded

          \brief Signals that a new project was created.

          \ingroup afevents
        */
        struct ProjectAdded : public Event
        {
          /*!
            \brief Constructor.

            \param proj The pointer to the project added.
          */
          ProjectAdded(Project* proj) :
          Event(PROJECT_ADDED),
          m_proj(proj)
          {
          }

          Project* m_proj;    //!< Pointer to the newe project.
        };
      }
    }
  }
}

#endif //__TERRALIB_QT_AF_EVENTS_INTERNAL_PROJECTEVENTS_H