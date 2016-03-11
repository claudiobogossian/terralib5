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
  \file terralib/statistics/NumericStatisticsComplexSummary.h
 
  \brief A structure to hold the set of statistics from a set of numerical values.
*/

#ifndef __TERRALIB_STATISTICAL_CORE_INTERNAL_NUMERICSTATISTICALCOMPLEXSUMMARY_H
#define __TERRALIB_STATISTICAL_CORE_INTERNAL_NUMERICSTATISTICALCOMPLEXSUMMARY_H

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
	struct TESTATEXPORT NumericStatisticalComplexSummary
    {
      public :
        
        struct ClassesPercentLess
        {
          bool operator()(const std::complex<double> s1, const std::complex<double> s2) const
          {
            return ( ( s1.real() < s2.real() ) && ( s1.imag() < s2.imag() ) );
          }
        };        
        
        typedef std::map < std::complex< double >, std::complex< double >, ClassesPercentLess > ClassesPercentT;
        
        std::complex< double > m_minVal;
        std::complex< double > m_maxVal;
        std::complex< double > m_mean;
        std::complex< double > m_sum;
        int m_count;
        int m_validCount;
        std::complex< double > m_stdDeviation;
        std::complex< double > m_kernel;
        std::complex< double > m_variance;
        std::complex< double > m_skewness;
        std::complex< double > m_kurtosis;
        std::complex< double > m_amplitude;
        std::complex< double > m_median;
        std::complex< double > m_varCoeff;
        std::vector<std::complex< double > > m_mode;
        ClassesPercentT m_percentEachClass;        
      
        /*! \brief Constructor. */
        NumericStatisticalComplexSummary();

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        NumericStatisticalComplexSummary(const NumericStatisticalComplexSummary& rhs);

        /*! \brief Destructor. */
        ~NumericStatisticalComplexSummary();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        NumericStatisticalComplexSummary& operator=(const NumericStatisticalComplexSummary& rhs);

        /*! \brief Clear the structure. */
        void clear();
    };

  } // end namespace stat
} // end namespace te

#endif  // #define __TERRALIB_STATISTICAL_CORE_INTERNAL_NUMERICSTATISTICALCOMPLEXSUMMARY_H

