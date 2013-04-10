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
  \file  terralib/qt/widgets/charts/ChartDisplay.h

  \brief A class to represent a chart display
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAY_H

//TerraLib
#include "../Config.h"

//QWT
#include <qwt_plot.h>

//STL
#include <string>

namespace te
{
  namespace se { class Stroke; class Fill; class Font; }
  namespace color { class RGBAColor; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ChartDisplay

        \brief A class to represent a chart display
      */
      class TEQTWIDGETSEXPORT ChartDisplay : public QwtPlot
      {
        public:

          /*!
            \brief Constructor

            It constructs a default chart display.
          */
          ChartDisplay(QWidget* parent=NULL);
          
          /*!
            \brief Constructor

            It sets the chart title.
          */
          void setChartTitle(const std::string& title);
          
          /*!
            \brief Constructor

            It sets the chart title.
          */
          void setChartTitle(const std::string& title,  te::color::RGBAColor* color, 
                   te::se::Font*  font, te::se::Fill* backFill, 
                   te::se::Stroke* backStroke);

          /*!
            \brief Destructor
          */
          ~ChartDisplay();
                 
         private:
          
          std::string           m_title;             //!< The chart's title.
          te::color::RGBAColor* m_titleColor;        //!< The chart title's color.
          te::se::Font*         m_titleFont;         //!< The chart title's font.
          te::se::Fill*         m_titleBackFill;     //!< The chart title's background fill..
          te::se::Stroke*       m_titleBackStroke;   //!<  background title stroke
                     
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAY_H

