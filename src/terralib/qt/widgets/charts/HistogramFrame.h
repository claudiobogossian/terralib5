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
  \file terralib/qt/widgets/charts/HistogramFrame.h

  \brief A frame used to adjust a histogram's chart parameters, weather it is about it's data or it's visual style.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMFRAME_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMFRAME_H

// TerraLib
#include "../Config.h"
#include "ChartWidget.h"

#include <memory>

class QwtPlotSeriesItem;

namespace Ui { class HistogramFrameWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
      {
      //Forward declarations
      class HistogramChart;
      class HistogramStyleWidget;
      /*!
        \class HistogramFrame

        \brief A frame for setting display options.
      */
      class TEQTWIDGETSEXPORT HistogramFrame : public ChartWidget
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor

            \param parent this widget's parent
          */
          HistogramFrame(QWidget* parent = 0);

          /*! 
            \brief Destructor.
          */
          ~HistogramFrame();

          /*!
            \brief Returns a pointer to the chart being configured

            \return A QwtPlotSeriesItem type pointer to the chart being configured
            \note The caller will take ownership of the returned pointer. 
          */
          QwtPlotSeriesItem* getChart();

          /*!
            \brief It sets the chart being configured

            \param newChart The new QwtPlotSeriesItem.
            \note It will not take ownership of the given pointer 
          */
          void setChart(QwtPlotSeriesItem* newChart);

        private:

          std::auto_ptr<Ui::HistogramFrameWidgetForm>  m_ui;           //!< The widget Form.
          HistogramChart*                              m_chart;        //!< The chart that will be editted by this widget.
          HistogramStyleWidget*                        m_styleWidget;  //!< The widget used to configure the histogram's style.
      };
    }   // end namespace widgets
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMFRAME_H

