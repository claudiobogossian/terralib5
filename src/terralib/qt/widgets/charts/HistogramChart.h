/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \fileHistogram.h

  \brief A class to represent a histogram chart. 
*/

//TerraLib
#include "../Config.h"

//QWT
#include <qwt_plot_histogram.h>

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMCHART_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMCHART_H

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class Histogram;

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

            \note It will not take the ownership of the pointers.
          */
          HistogramChart(Histogram* data);
          
          /*! \brief Destructor. */
          ~HistogramChart();  

         private:
           Histogram* m_histogram;

      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_HISTOGRAMCHART_H