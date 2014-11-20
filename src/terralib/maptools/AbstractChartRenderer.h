/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/AbstractChartRenderer.h

  \brief This is the base class for chart renderers.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTCHARTRENDERER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTCHARTRENDERER_H

// TerraLib
#include "../color/RGBAColor.h"
#include "Config.h"

namespace te
{
// Forward declaration
  namespace da
  {
    class DataSet;
  }

  namespace map
  {
// Forward declaration
    class Chart;

    /*!
      \class AbstractChartRenderer

      \brief This is the base class for chart renderers.
    */
    class TEMAPEXPORT AbstractChartRenderer
    {
      public:

        /*! \brief Constructor. */
        AbstractChartRenderer() { }

        /*! \brief Virtual destructor. */
        virtual ~AbstractChartRenderer() { }

        /*!
          \brief It generates the image pattern from the given chart and the current element of dataset.

          \param chart The chart informations that will be used.
          \param dataset The dataset that will be used.
          \param width  An output parameter that will be filled with the width of generated image pattern.

          \return The RGBA image that represents the chart.

          \note The caller will take the ownership of the returned pointer.
        */
        virtual te::color::RGBAColor** render(const Chart* chart, const te::da::DataSet* dataset, std::size_t& width) = 0;

        virtual te::color::RGBAColor** render(const Chart* chart, const std::map<std::string, double>& chartValue, std::size_t& width) = 0;
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_ABSTRACTCHARTRENDERER_H
