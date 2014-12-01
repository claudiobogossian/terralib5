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
  \file terralib/maptools/Chart.h

  \brief This class represents the informations needed to build map charts.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_CHART_H
#define __TERRALIB_MAPTOOLS_INTERNAL_CHART_H

// TerraLib
#include "../color/RGBAColor.h"
#include "Config.h"
#include "Enums.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace map
  {
    /*!
      \class Chart

      \brief This class represents the informations needed to build map charts.

      \ingroup map

      \sa AbstractLayer
    */
    class TEMAPEXPORT Chart
    {
      public:

        /*!
          \brief It constructs a new Chart instance.

          \param type       The chart type.
          \param properties The property names that will be used to generate the chart.
        */
        Chart(ChartType type, const std::vector<std::string>& properties);

        /*!
          \brief It constructs a new Chart instance.

          \param type       The chart type.
          \param properties The property names that will be used to generate the chart.
          \param colors     The colors that will be used to represent each property.
        */
        Chart(ChartType type, const std::vector<std::string>& properties, const std::vector<te::color::RGBAColor>& colors);

        /*! \brief Destructor. */
        ~Chart();

        ChartType getType() const;

        const std::vector<std::string>& getProperties() const;

        const te::color::RGBAColor& getColor(std::size_t i) const;

        void setColor(std::size_t i, const te::color::RGBAColor& color);

        const te::color::RGBAColor& getContourColor() const;

        void setContourColor(const te::color::RGBAColor& color);

        std::size_t getContourWidth() const;

        void setContourWidth(std::size_t width);

        std::size_t getHeight() const;

        void setHeight(std::size_t height);

        std::size_t getWidth() const;

        std::size_t getBarWidth() const;

        void setBarWidth(std::size_t width);

        void setMaxValue(double value);

        double getMaxValue() const;

        /*! \brief It gets the chart visibility. */
        bool isVisible() const;

        /*!
          \brief It sets the chart visibility.

          \param visible The visibility value.
        */
        void setVisibility(bool visible);

        void setAvoidConflicts(bool on);

        bool getAvoidConflicts() const;

        /*! \brief It gets the grouping summary. It is used only in case 1 to n.
        */
        std::string getSummary() const;

        /*!
          \brief It gets the grouping summary. It is used only in case 1 to n.

          \param summary The summary mode. It can be: "MIN", "MAX", "SUM", "AVERAGE", "MEDIAN", "STDDEV" or "VARIANCE"
        */
        void setSummary(const std::string& summary);

      private:

        ChartType m_type;                           //!< The chart type.
        std::vector<std::string> m_properties;      //!< The property names that will be used to generate the chart.
        std::vector<te::color::RGBAColor> m_colors; //!< The color used to each property.
        te::color::RGBAColor m_contourColor;        //!< The chart contour color.
        std::size_t m_contourWidth;                 //!< The chart contour width (in pixels).
        std::size_t m_height;                       //!< The chart height (in pixels).
        std::size_t m_barWidth;                     //!< The bar width for char Bar type (in pixels).
        bool m_isVisible;                           //!< A flag that indicates if the chart is visible.
        double m_maxValue;                          //!< The max value of the chart.
        bool m_avoidConflicts;                      //!< A flag that indicates if conflicts must be avoided.
        std::string m_summary;                       //!< The summary used in case 1 to n.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_CHART_H
