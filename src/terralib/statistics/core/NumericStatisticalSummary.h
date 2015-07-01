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
  \file terralib/statistics/NumericStatisticsSummary.h
 
  \brief A structure to hold the set of statistics from a set of numerical values.
*/

#ifndef __TERRALIB_STATISTICAL_CORE_INTERNAL_NUMERICSTATISTICALSUMMARY_H
#define __TERRALIB_STATISTICAL_CORE_INTERNAL_NUMERICSTATISTICALSUMMARY_H

// TerraLib
#include "Config.h"

// STL
#include <complex>
#include <map>
#include <vector>

namespace te
{
  namespace stat
  {
    /*!
      \struct NumericStatisticalSummary

      \brief A structure to hold the set of statistics from a set of numerical values.
    */
    struct TESTATEXPORT NumericStatisticalSummary
    {
      public:

        /*! \brief Constructor. */
        NumericStatisticalSummary();

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        NumericStatisticalSummary(const NumericStatisticalSummary& rhs);

        /*! \brief Destructor. */
        ~NumericStatisticalSummary();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        NumericStatisticalSummary& operator=(const NumericStatisticalSummary& rhs);

        /*! \brief Clear the structure. */
        void clear();

      public:

        double m_minVal;
        double m_maxVal;
        double m_mean;
        double m_sum;
        int m_count;
        int m_validCount;
        double m_stdDeviation;
        double m_kernel;
        double m_variance;
        double m_skewness;
        double m_kurtosis;
        double m_amplitude;
        double m_median;
        double m_varCoeff;
        std::vector<double> m_mode;

    };

  } // end namespace stat
} // end namespace te

#endif  // __TERRALIB_STATISTICAL_CORE_INTERNAL_NUMERICSTATISTICALSUMMARY_H
