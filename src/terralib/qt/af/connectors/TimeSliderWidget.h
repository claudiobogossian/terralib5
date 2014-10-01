/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraView - A GIS Application.

    TerraView is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraView is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    TerraLib Team at <terralib-team@dpi.inpe.br>.
 */

/*! 
  \file terralib/qt/af/connectors/TimeSliderWidget.h

  \brief A connector of the te::qt::widgets::TimeSliderWidget class to the Application Framework.
*/

#ifndef __TERRALIB_QT_AF_CONNECTORS_INTERNAL_TIME_SLIDER_WIDGET_H
#define __TERRALIB_QT_AF_CONNECTORS_INTERNAL_TIME_SLIDER_WIDGET_H

// Terralib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>

namespace te
{
// Forward declaration
  namespace qt
  {
    namespace widgets
    {
      class AbstractTool;
      class TimeSliderWidget;
    }

    namespace af
    {
      namespace evt
      {
      // Forward declaration
        struct Event;
      }

      /*!
        \class TimeSliderWidget
        
        \brief A connector for the te::qt::widgets::TimeSliderWidget class to the Application Framework.

        \ingroup afconnector
      */
      class TEQTAFEXPORT TimeSliderWidget : public QObject
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param md te::qt::widgets::TimeSliderWidget to be listened.
          */
          TimeSliderWidget(te::qt::widgets::TimeSliderWidget* w);

          /*! \brief destructor. */
          virtual ~TimeSliderWidget();

          te::qt::widgets::TimeSliderWidget* getTimeSliderWidget();

        protected slots:

          /*!
            \brief Listener to the application framewrork events.

            \param e An event sent by framework.
          */
          void onApplicationTriggered(te::qt::af::evt::Event* e);

        protected:

          te::qt::widgets::TimeSliderWidget* m_timeSliderWidget;  //!< Pointer to a component te::qt::widgets::TimeSliderWidget.
      };
    }
  }
}

#endif // __TERRALIB_QT_AF_CONNECTORS_INTERNAL_TIME_SLIDER_WIDGET_H
