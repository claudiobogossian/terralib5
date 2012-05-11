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
  \file QtProgressEvent.h
 
  \brief The QtProgressBarSetValueEvent is a custom event used to set a new value into 
         a progress bar. Used in thread codes.
*/

#ifndef __TERRALIB_QT_PROGRESS_QTPROGRESSSETVALUE_H
#define __TERRALIB_QT_PROGRESS_QTPROGRESSSETVALUE_H

//TerraLib
#include "../../widgets/Config.h"
#include "terralib/common/progress/AbstractProgress.h"

//Qt
#include <QtCore/QEvent>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class TEQTWIDGETSEXPORT QtProgressBarSetValueEvent : public QEvent
      {
        public:
          QtProgressBarSetValueEvent(int value);

          ~QtProgressBarSetValueEvent();

          static QEvent::Type type();
      
        private:
          static QEvent::Type m_customEventType;

        public:
          int m_value;
      };
    
    }  // end namespace progress
  }    // end namespace qt
}      // end namespace te

#endif  // __TERRALIB_QT_PROGRESS_QTPROGRESSSETVALUE_H