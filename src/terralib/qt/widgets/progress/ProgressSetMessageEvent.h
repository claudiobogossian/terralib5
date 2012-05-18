/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/progress/ProgressSetMessageEvent.h
 
  \brief The ProgressSetMessageEvent is a custom event used to set a new message into 
         a progress bar. Used in thread codes.
*/

#ifndef __TERRALIB_QT_WIDGETS_PROGRESS_PROGRESSSETMESSAGEEVENT_H
#define __TERRALIB_QT_WIDGETS_PROGRESS_PROGRESSSETMESSAGEEVENT_H

//TerraLib
#include "../../widgets/Config.h"

// C++ Library
#include <string>

//Qt
#include <QtCore/QEvent>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ProgressSetMessageEvent

        \brief The ProgressSetMessageEvent is a custom event used to set a new message into 
         a progress bar. Used in thread codes.

        \sa DialogProgressViewer, WidgetProgressItem
      */
      class TEQTWIDGETSEXPORT ProgressSetMessageEvent : public QEvent
      {
        public:
          /*! \brief It initializes a new QtProgress. */
          ProgressSetMessageEvent(std::string value);
          
          /*! \brief Destructor */
          ~ProgressSetMessageEvent();
          
          /*! \brief Get the custom event type */
          static QEvent::Type type();
      
        private:
          static QEvent::Type m_customEventType;  //!< Custom Event Type

        public:
          std::string m_value;                    //!< Progress bar message attribute
      };
    }  // end namespace progress

  }    // end namespace qt
}      // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_PROGRESS_PROGRESSSETMESSAGEEVENT_H

