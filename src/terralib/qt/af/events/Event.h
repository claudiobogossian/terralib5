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
  \defgroup afevents Application Framework Events

  \ingroup appframework

  \brief Default events used with application framework.

  \details These are default event classes that can be sent by the application framework.
  Event classes can contain anything as members, such as pointers, instances or any information that can be usefull
  for the application. These may be classes as simple as possible with public members to facilitate the access. 

  \note Events MAY NOT gets the ownership of its internal pointers. 
*/

/*!
  \file Event.h

  \brief Contains a base class for application events.
*/

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_EVENT_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_EVENT_H

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class Event
        \ingroup afevents

        \brief A base class for application events.

        This is the type of event that components and plugins may
        use to notify changes in the application state.

        One must specialize this class in order to notify specific events.
      */
      class Event
      {
        public:

          /*!
            \brief Returns the event identifier.
            \return Identifier of the event.
          */
          int getId() const;

        protected:

          /*!
            \brief Contructor.
            \param id The identifier.
          */
          Event(int id);

          /*!
            \brief Virtual destructor.
          */
          virtual ~Event();

        private:

          int m_id;         //!< Identifier.
      };

      inline int Event::getId() const
      {
        return m_id;
      }

      inline Event::Event(int id)
        : m_id(id)
      {
      }

      inline Event::~Event()
      {
      }

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_EVENTS_INTERNAL_EVENT_H

