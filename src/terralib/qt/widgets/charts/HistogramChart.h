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

  namespace da    { class ObjectIdSet; }
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
            \note it will take the ownership of the pointer "style". If style is null, the chart will be constructed with a default style.
          */
          HistogramChart(Histogram* data, te::qt::widgets::HistogramStyle* style = 0);

          /*! \brief Destructor. */
          ~HistogramChart();  

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

            \note If the histogram's data contains string labels, this function will define how they are drawn
          */
          void attach(QwtPlot* plot);

          /*!
            \brief It returns the chart's histogram.  

            \return The chart's histogram.
            \note The caller will not take ownership of the returned pointer.
          */
          te::qt::widgets::Histogram* getHistogram();

          /*!            
            \brief It sets the chart's histogram.   

            \param newHistogram The chart's  new histogram.
            \note It will take the ownership of the pointer "newHistogram".
          */
          void setHistogram(te::qt::widgets::Histogram* newHistogram);

          /*!
            \brief Returns a clone of the pointer to the histogram's style

            \return A HistogramStyle type pointer to the histogram's style.
            \note The caller will take ownership of the returned pointer. 
          */
          te::qt::widgets::HistogramStyle* getHistogramStyle();

          /*!            
            \brief It sets the chart's style.

            \param newStyle The chart's  new style.
            \note It will take the ownership of the pointer "newStyle".
          */
          void setHistogramStyle(te::qt::widgets::HistogramStyle* newStyle);

          /*!
            \brief Highlights the objects identified by \a oids

            \param oids The identifiers of plotitems to be highlighted.
          */
          void highlight(const te::da::ObjectIdSet* oids);

          /*!
            \brief Highlights the intervals identified by \a point

            \param point The point to be highlighted.
          */
          te::da::ObjectIdSet* highlight(QPointF point);

          /*!
            \brief Highlights the intervals contained by \a rectangle

            \param rect The rectangle containing the intervals to be highlighted.
          */
          te::da::ObjectIdSet* highlight(QRectF rect);

          /*!
            \brief color used to hgihlight selected objects on this chart.

            \param selColor The color used to hgihlight selected objects on this chart.

          */
          void setSelectionColor(QColor selColor);

          void setData();


        private:

            Histogram*                        m_histogram;           //!< The histogram that will be shown on this chart.
            StringScaleDraw*                  m_histogramScaleDraw;  //!< Scale draw that defines how a label will be displayed on this chart.
            te::qt::widgets::HistogramStyle*  m_histogramStyle;      //!< The histogram's style as displayed by this widget.
            QwtPlotHistogram*                 m_selection;            //!< The PlotItems to be highlighted when a selection occurs;

      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMCHART_H