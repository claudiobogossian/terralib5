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
  \file terralib/qt/plugins/st/TimeSliderWidgetAction.h

  \brief This file defines the TimeSliderWidgetAction class
*/

#ifndef __TE_QT_PLUGINS_ST_INTERNAL_TIMESLIDERWIDGETACTION_H
#define __TE_QT_PLUGINS_ST_INTERNAL_TIMESLIDERWIDGETACTION_H

// TerraLib
#include "Config.h"
#include "AbstractAction.h"
#include "../../../qt/widgets/st/TimeSliderWidget.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class Slider;
    }

    namespace plugins
    {
      namespace st
      {
        /*!
          \class TimeSliderWidgetAction

          \brief This class register the time slider widget action into st Plugin.

        */
        class TimeSliderWidgetAction : public te::qt::plugins::st::AbstractAction
        {
          Q_OBJECT
            
          public:

            TimeSliderWidgetAction(QMenu* menu);

            virtual ~TimeSliderWidgetAction();

          protected slots:

            virtual void onActionActivated(bool checked);

            virtual void onDeleteTimeSliderWidget();

            void onApplicationTriggered(te::qt::af::evt::Event* e);

          private:
            te::qt::widgets::TimeSliderWidget* m_timeSliderWidget;
        };

      } // end namespace st
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif //__TE_QT_PLUGINS_ST_INTERNAL_TIMESLIDERWIDGETACTION_H