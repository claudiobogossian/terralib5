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
  \file  terralib/qt/widgets/charts/TimeSeriesChart.h

  \brief A class to represent a timeSeries' chart. 
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESCHART_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESCHART_H

//TerraLib
#include "../Config.h"

//QWT
#include <qwt_plot_curve.h>

//STL
#include <vector>

namespace te
{

  namespace da  { class ObjectIdSet; }
  namespace st  { class TimeSeries; }
  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class TimeSeriesStyle;
      class StringScaleDraw;

      /*!
        \class TimeSeriesChart

        \brief A class to represent a timeSeries chart.
      */
      class TEQTWIDGETSEXPORT TimeSeriesChart : public QwtPlotCurve
      {
        public:

          /*!
            \brief Constructor

            It constructs a TimeSeres chart with a default style.

            \note It will take the ownership of the pointers.
            \note it will construct a chart with a default style.
          */
          TimeSeriesChart(te::st::TimeSeries* data);
          
          /*!
            \brief Constructor

            \note It will take the ownership of the pointer "style".
            \note It will take the ownership of the pointer "data".
          */
          TimeSeriesChart(te::st::TimeSeries* data, TimeSeriesStyle* style, size_t size);

          /*! \brief Destructor. */
          ~TimeSeriesChart();

          /*! \brief Returns the chart's type. */
          virtual int rtti() const;

          /*!
            \brief It returns the chart's scale draw.  

            \return The chart's scale draw.
            \note The caller will not take ownership of the returned pointer.
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

            \param plot The QwtPlot that will be attached to this chart.
          */
          void attach(QwtPlot* plot);

          /*!
            \brief It returns the chart's timeSeries.  

            \return The chart's timeSeries.  
          */
          te::st::TimeSeries* getTimeSeries();

          /*!            
            \brief It sets the chart's timeSeries.   

            \param newHistogram The chart's  new scatter.
            \note It will take the ownership of the pointer "newScatter".
          */
          void setTimeSeries(te::st::TimeSeries* newTimeSeries);

          /*!
            \brief Returns a clone of the pointer to the timeSeries' style

            \return A ScatterStyle type pointer to the timeSeries' style.
            \note The caller will take ownership of the returned pointer. 
          */
          TimeSeriesStyle* getTimeSeriesStyle();

          /*!            
            \brief It sets the chart's style.   

            \param newStyle The chart's  new style.
            \note It will take the ownership of the pointer "newStyle".
          */
          void setTimeSeriesStyle(TimeSeriesStyle* newStyle);

          void setData();

        private:
          te::st::TimeSeries*  m_timeSeries;          //!< The scatter that will be shown on this chart.
          StringScaleDraw*     m_scaleDraw;        //!< Scale draw that defines how a label will be displayed on this chart.
          TimeSeriesStyle*     m_timeSeriesStyle;  //!< The symbol that defines the look of a scatter's point.
          QColor               m_selColor;         //!< The color used to highlight selected obecjts.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te


#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TIMESERIESCHART_H