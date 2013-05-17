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
  \file terralib/qt/widgets/charts/HistogramChart.h

  \brief A class to represent a histogram chart. 
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMCHART_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMCHART_H

//TerraLib
#include "../Config.h"

//QWT
#include <qwt_plot_histogram.h>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class Histogram;
      class HistogramStyle;
      class StringScaleDraw;

      /*!
        \class HidtogramChart

        \brief A class to represent a histogram chart.
      */
      class TEQTWIDGETSEXPORT HistogramChart : public QwtPlotHistogram
      {
        public:

          /*!
            \brief Constructor

            It constructs a histogram chart .

            \note It will take the ownership of the pointer "data".
          */
          HistogramChart(Histogram* data);
          
          /*! \brief Destructor. */
          ~HistogramChart();  

          /*!
            \brief It returns the chart's scale draw.  

            \return The chart's scale draw.  
          */

           StringScaleDraw* getScaleDraw();

          /*!            
            \brief It sets the chart's scale draw.   

            \param new_labels The new chart's scale draw.   
            \note It will take the ownership of the pointer "newScaleDraw".
          */

          void setScaleDraw( StringScaleDraw* newScaleDraw);

          /*!
            \brief It atttaches a QwtPlot to this Cahrt 

            \param plot The QwtPlot that will be attached to this plot.

             \note If the histogram's data contains string labels, this function will define how they are drawn
          */
          void attach(QwtPlot* plot);

          te::qt::widgets::Histogram* getHistogram();

          void setHistogram(te::qt::widgets::Histogram* newHistogram);

          te::qt::widgets::HistogramStyle* getHistogramStyle();

          void setHistogramStyle(te::qt::widgets::HistogramStyle* newStyle);

        private:

            Histogram*                        m_histogram;           //!< The histogram that will be shown on this chart.
            StringScaleDraw*                  m_histogramScaleDraw;  //!< Scale draw that defines how a label will be displayed on this chart.
            te::qt::widgets::HistogramStyle*  m_histogramStyle;      //!< The histogram's style as displayed by this widget.

      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMCHART_H