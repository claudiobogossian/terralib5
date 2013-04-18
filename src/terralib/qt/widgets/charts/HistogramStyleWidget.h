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
#include <QtGui/QWidget>

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
    class Histogram;
    class HistogramChart;
    class HistogramStyle;

    /*!
        \class Histogram Style Widget

        \brief A widget used to adjust a histogram's style.
      */
      class TEQTWIDGETSEXPORT HistogramStyleWidget : public QWidget
      {

        Q_OBJECT

        public:

          HistogramStyleWidget(QWidget* parent = 0,  Qt::WindowFlags f = 0);

          ~HistogramStyleWidget();

        protected slots:

          void onFillPushButtonClicked();
          void onStrokePushButtonClicked();

        private:

          std::auto_ptr<Ui::HistogramStyleWidgetForm>  m_ui;      //!< The widget form.
          te::qt::widgets::Histogram*         m_histogram;        //!< The histogram that will be configured by this widget.
          te::qt::widgets::HistogramChart*    m_histogramChart;   //!< The histogram's chart that will be configured by this widget.
          te::qt::widgets::HistogramStyle*    m_histogramStyle;   //!< The histogram's chart style that will be configured by this widget.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMSTYLEWIDGET_H