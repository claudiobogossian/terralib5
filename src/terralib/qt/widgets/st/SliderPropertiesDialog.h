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
  \file terralib/qt/widgets/slider/SliderPropertiesWidget.h

  \brief A widget used to alow the user to control the visualization of temporal data
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SLIDERPROPERTIESDIALOG_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SLIDERPROPERTIESDIALOG_H

//TerraLib
#include "../Config.h"
#include "TrajectoryItem.h"

// Qt
#include <QDialog>

//STL
#include <memory>
#include <vector>

namespace Ui { class SliderPropertiesDialogForm; }

namespace te
{
  namespace dt { class TimePeriod; }

  namespace qt
  {
    namespace widgets
    {

      /*!
        \class SliderPropertiesDialog

        \brief A dialog used to configure the behaviour of the SliderSliderWidget.
      */
    class TEQTWIDGETSEXPORT SliderPropertiesDialog : public QDialog
      {
        Q_OBJECT

        friend class TimeSliderWidget;

        public:

          /*!
            \brief Constructor

            It constructs a Slider Slider Dialog
          */
          SliderPropertiesDialog(QWidget* parent,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor

            It destructs a Slider Slider Dialog
          */
          ~SliderPropertiesDialog();

          /*!
            \brief It initialize a property animation dialog
          */
          void populateUi();

          /*!
            \brief It returns a boolean that indicates if animation is going forward or backward

            \return A boolean that indicates if animation is going forward or backward
          */
           bool isForwardChecked();

          /*!
            \brief It returns a boolean that indicates if the time slider animation
                   is supposed to be displayed looping or not.

            \return A boolean that indicates if the animation is supposed to be displayed on a loop.
          */
           bool isLoopChecked();

          /*!
            \brief It returns a boolean that indicates if the time slider animation
                   is to reverse it's direction on ce a loop is finished.

            \return A boolean that indicates if the animation is supposed to have it's direction reversed.
          */
           bool isGoBackChecked();

          /*!
            \brief It gets the initial time as QDateTime.

            \return The initial time.
          */
          QDateTime getInitialQDate();

          /*!
            \brief It gets the final time as QDateTime.

            \return The final time.
          */
          QDateTime getFinalQDate();

          /*!
            \brief It gets the initial time.

            \return The initial time.
          */
           te::dt::TimeInstant getInitialTime();

          /*!
            \brief It gets the final time.

            \return The final time.
          */
           te::dt::TimeInstant getFinalTime();

        protected slots:

          //void onOkPushButtonClicked();
          void onHelpPushButtonClicked();

          /*!
            \brief AutoPan radio button clicked.

            \param b True if the button is checked, or false if the button is unchecked
          */
          //void onAutoPanCheckBoxClicked(bool b);

          /*!
            \brief Draw track radio button clicked.

            \param b True if the button is checked, or false if the button is unchecked
          */
          void onDrawTrackCheckBoxClicked(bool b);

          /*!
            \brief It takes the necessary steps after changing the duration.
            \param v Value of opacity (0 - 255).
          */
          //void onOpacityValueChanged(int v);

          /*!
            \brief Trajectory color combo box activated.

            \param i The index of combo box.
          */
          //void onTrajectoryColorComboBoxActivated(int i);

          /*!
            \brief Opacity combo box activated.

            \param i The index of combo box.
          */
          //void onOpacityComboBoxActivated(int i);

          /*!
            \brief Reset initial time button clicked.
          */
          //void onResetInitialTimePushButtonClicked();


          /*!
            \brief Reset final time button clicked.
          */
          //void onResetFinalTimePushButtonClicked();

        private:

          std::auto_ptr<Ui::SliderPropertiesDialogForm>  m_ui;              //!< The widget form.
          TimeSliderWidget*                              m_tsw;             //!< The Timer Slider Widget pointer
          //te::dt::TimePeriod                             m_temporalExtent;  //!< The temporal extent.
          //QList<QGraphicsItem*>                          m_trajectories;    //!< The trajectory items
    };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SLIDERPROPERTIESDIALOG_H
