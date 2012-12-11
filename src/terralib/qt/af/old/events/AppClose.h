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
  \file terralib/qt/af/events/AppClose.h

  \brief This event signals that the application is about to close.
*/

#ifndef __TERRALIB_QT_AF_EVENTS_INTERNAL_APPCLOSE_H
#define __TERRALIB_QT_AF_EVENTS_INTERNAL_APPCLOSE_H

// TerraLib
#include "Enums.h"
#include "Event.h"

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class AppClose

        \brief This event signals that the application is about to close.
      */
      class AppClose : public Event
      {
        public:

          AppClose()
            : Event(te::qt::af::evt::APP_CLOSE)
          {
          }

          ~AppClose()
          {
          }
      };

    } // end namespace af
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_AF_EVENTS_INTERNAL_APPCLOSE_H
