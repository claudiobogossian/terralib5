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
  \file terralib/qt/widgets/slider/TimeSliderWidget.h

  \brief A widget used to alow the user to control the visualization of temporal data
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TIMESLIDERWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TIMESLIDERWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QWidget>
#include <QTimer>

//STL
#include <memory>

namespace Ui { class TimeSliderWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

      /*!
        \class TimeSliderWidget

        \brief A wdiget used to control the visualization of temporal data
      */
      class TEQTWIDGETSEXPORT TimeSliderWidget : public QWidget
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor

            It constructs a Time Slider Widget
          */
          TimeSliderWidget(QWidget* parent = 0,  Qt::WindowFlags f = 0, int minimum = 0, int maximum = 10);

          /*!
            \brief Destructor

            It destructs a Time Slider Widget
          */
          ~TimeSliderWidget();

          void enterEvent(QEvent*);

          void leaveEvent(QEvent*);

        protected slots:

          void onExitToolButtonnClicked();
          void onSettingsToolButtonnClicked();
          void onPlayToolButtonnClicked();
          void onSkipBackwardToolButtonnClicked();
          void onSkipForwardToolButtonnClicked();
          void onSliderValueChanged(int value);
          void play();

        signals:

          /*!
            \brief Emmit when the slider's value has been changed
          */
          void valueCHanged(int value);

        private:

          bool                                     m_loop;   //!< This property holds whether the slider's animation is on loop.
          int                                      m_speed;  //!< The slider's animation speed in miliseconds
          bool                                     m_state;  //!< This property holds whether the slider's animation is running.
          std::auto_ptr<QTimer>                    m_timer;  //!< The timer that will control the animation proccesses.
          std::auto_ptr<Ui::TimeSliderWidgetForm>  m_ui;     //!< The widget form.
    };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TIMESLIDERWIDGET_H