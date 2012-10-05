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
  \file MultiHistogramChart.h

  \brief A class to represent multiple histogram's charts. 
*/

//TerraLib
#include "../Config.h"

//QWT
#include <qwt_plot_histogram.h>

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_MULTIHISTOGRAMCHART_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_MULTIHISTOGRAMCHART_H

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class HistogramChart;
      class ChartDisplay;

      /*!
        \class HidtogramChart

        \brief A class to represent a histogram chart.
      */
      class TEQTWIDGETSEXPORT MultiHistogramChart : public QwtPlotHistogram
      {
        public:

          /*!
            \brief Constructor

            It constructs a Multi histogram chart .

            \note It will not take the ownership of the pointers.
          */
          MultiHistogramChart();
          
          /*! \brief Destructor. */
          ~MultiHistogramChart();  

          /*!            
            \brief Function that will attach multiple histograms to the Chart Display 
            it overwrites the default behavior of the attach function
            */

          void attach(std::vector<te::qt::widgets::HistogramChart*>& charts, te::qt::widgets::ChartDisplay* display);

          /*!            
            \brief It returns the vector containing the histogram charts. 

            \return The histogram charts. 

            \note The caller will not have the ownership of the returned pointer. 
          */

          std::vector<te::qt::widgets::HistogramChart*>& getCharts();
      
          /*!            
            \brief It sets the histogram's interval. 

            \param new_values The new histogram's interval.
          */

          void setCharts(std::vector<te::qt::widgets::HistogramChart*> new_Charts);

         private:

           std::vector<te::qt::widgets::HistogramChart*> m_histogramCharts;

      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_MULTIHISTOGRAMCHART_H