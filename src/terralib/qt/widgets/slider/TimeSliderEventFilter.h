/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/slider/TimeSliderEventFilter.h

  \brief An eventFilter used to define the behavior and appearance of a TimeSliderWidget depending on the events it receives
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TIMESLIDEREVENTFILTER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TIMESLIDEREVENTFILTER_H

//TerraLib
#include "../Config.h"

//QT
#include <qobject.h>

namespace te
{
  namespace qt
  {
    namespace widgets
    {

      /*!
        \class TimeSliderEventFilter

        \brief  An eventFilter used to define the behavior and appearance of a TimeSliderWidget depending on the events it receives
      */
    class TimeSliderEventFilter : public QObject
    {
      Q_OBJECT

      public:

       /*!
         \brief Constructor

         It constructs a Time Slider Widget
       */
        TimeSliderEventFilter(QObject* parent);

        /*!
           brief  An event filter that checks if the mouse has entered or left the are of the given QObject.
        */
        virtual bool eventFilter(QObject* obj, QEvent* e);
    };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TIMESLIDEREVENTFILTER_H