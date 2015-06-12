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
  \file terralib/maptools/ChartRendererManager.h

  \brief This is a singleton for managing chart renderer instance available in the system.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_CHARTRENDERERMANAGER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_CHARTRENDERERMANAGER_H

// TerraLib
#include "../color/RGBAColor.h"
#include "../common/Singleton.h"
#include "../common/ThreadingPolicies.h"
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
// Forward declarations
    class AbstractChartRenderer;
    class Chart;

    /*!
      \class ChartRendererManager

      \brief This is a singleton for managing chart renderer instance available in the system.

      If you want to render a chart, use commands like:
      <code>
      te::color::RBGA** image = te::map::ChartRendererManager::getInstance().render(chart, dataset);
      </code>

      \sa AbstractChartRenderer, Chart
    */
    class TEMAPEXPORT ChartRendererManager : public te::common::ClassLevelLockable<ChartRendererManager,
                                                                                  ::boost::recursive_mutex,
                                                                                  ::boost::lock_guard< ::boost::recursive_mutex>,
                                                                                  ::boost::lock_guard< ::boost::recursive_mutex> >,
                                            public te::common::Singleton<ChartRendererManager>
    {
      friend class te::common::Singleton<ChartRendererManager>;

      public:

        /*!
          \brief It generates the image pattern from the given chart and dataset.

          \param chart The chart informations that will be used.
          \param dataset The dataset that will be used.
          \param width  An output parameter that will be filled with the width of generated image pattern.

          \return The RGBA image that represents the chart.

          \note The caller will take the ownership of the returned pointer.

          \exception Exception It will throws an exception if the image pattern can not be generated.
        */
        te::color::RGBAColor** render(const Chart* chart, const te::da::DataSet* dataset, std::size_t& width);

        te::color::RGBAColor** render(const Chart* chart, const std::map<std::string, double>& chartValue, std::size_t& width);

        /*!
          \brief It sets the chart renderer to the manager.

          \param renderer The renderer that will be set.

          \note The manager will take the owership of the given chart renderer.
        */
        void set(AbstractChartRenderer* renderer);

     protected:

        /*! \brief It initializes the singleton instance of the chart renderer manager. */
        ChartRendererManager();

        /*! \brief Singleton destructor. */
        ~ChartRendererManager();

      private:

        AbstractChartRenderer* m_renderer; //!< The chart renderer in the manager.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_CHARTRENDERERMANAGER_H
