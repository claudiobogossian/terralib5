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
  \file terralib/qt/widgets/slider/TimePropertiesWidget.h

  \brief A widget used to alow the user to control the visualization of temporal data
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TIMEPROPERTIESWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TIMEPROPERTIESWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>

//STL
#include <memory>

namespace Ui { class TimePropertiesDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

      /*!
        \class TimePropertiesDialog

        \brief A dialog used to configure the behaviour of the TimeSliderWidget.
      */
    class TEQTWIDGETSEXPORT TimePropertiesDialog : public QDialog
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor

            It constructs a Time Slider Dialog
          */
          TimePropertiesDialog(QWidget* parent = 0,  Qt::WindowFlags f = 0, int minimum = 10, int maximum = 500);

          /*!
            \brief Destructor

            It destructs a Time Slider Dialog
          */
          ~TimePropertiesDialog();

          /*!
            \brief It returns the speed's value.

            \return The speed's value.
          */
           int getValue();

          /*!
            \brief It returns a boolean that indicates if the time slider animation
                   is supposed to be displayed looping or not.

            \return A boolean that indicates if the animation is supposed to be displayed on a loop.
          */
           bool isLoopChecked();

        protected slots:

          void onOkPushButtonClicked();
          void onHelpPushButtonClicked();

        private:

          std::auto_ptr<Ui::TimePropertiesDialogForm>  m_ui;  //!< The widget form.

    };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TIMEPROPERTIESWIDGET_H
