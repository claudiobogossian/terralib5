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
  \file terralib/qt/widgets/progress/ProgressResetEvent.h

  \brief The ProgressResetEvent is a custom event used to reset a progress bar.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSRESETEVENT_H
#define __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSRESETEVENT_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtCore/QEvent>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ProgressResetEvent

        \brief The ProgressResetEvent is a custom event used to reset a progress bar.

        This event can be used when working with multithread tasks.
      */
      class TEQTWIDGETSEXPORT ProgressResetEvent : public QEvent
      {
        public:

          /*! \brief It initializes a new ProgressResetEvent. */
          ProgressResetEvent();

          /*! \brief Destructor */
          ~ProgressResetEvent();

          /*! \brief Get the custom event type */
          static QEvent::Type type();

        private:

          static QEvent::Type sm_customEventType;  //!< Custom Event Type
      };

    }  // end namespace widgets
  }    // end namespace qt
}      // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROGRESS_INTERNAL_PROGRESSRESETEVENT_H
