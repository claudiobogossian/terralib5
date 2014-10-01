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
  \file terralib/qt/widgets/charts/histogramStyleWidget.h

  \brief A widget used to adjust a histogram's style.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMSTYLEWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMSTYLEWIDGET_H

//TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

//STL
#include <memory>

namespace Ui { class HistogramStyleWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {

    //forward declarations
    class HistogramStyle;

    /*!
        \class HistogramStyleWidget

        \brief A widget used to adjust a histogram's style.
      */
      class TEQTWIDGETSEXPORT HistogramStyleWidget : public QWidget
      {

        Q_OBJECT

        public:

            /*!
              \brief Constructor

              \param initial  An initial HistogramStyle element that will be used, it will take ownership of this pointer if it is given. The widget form will be updated based on
                              it's parameters. It can be NULL.
              \param parent   this widget's parent
              \param f Window flags used to configure this dialog
            */
          HistogramStyleWidget(te::qt::widgets::HistogramStyle* initial = 0, QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*! 
            \brief Destructor.
          */
          ~HistogramStyleWidget();

          /*!
            \brief Returns a pointer to the HistogramStyle being configured

            \return A HistogramStyle type pointer to the style being configured
            \note The caller will take ownership of the returned pointer.
          */
          te::qt::widgets::HistogramStyle* getHistogramStyle();

        protected slots:

          void onFillPushButtonClicked();
          void onStrokePushButtonClicked();

        private:

          std::auto_ptr<Ui::HistogramStyleWidgetForm>  m_ui;              //!< The widget form.
          HistogramStyle*                              m_histogramStyle;  //!< The histogram's chart style that will be configured by this widget.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMSTYLEWIDGET_H