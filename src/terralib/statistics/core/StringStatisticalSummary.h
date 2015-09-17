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
  \file terralib/statistics/StringStatisticsSummary.h
 
  \brief A structure to hold the set of statistics from a set of categorical (sample) values.
*/

#ifndef __TERRALIB_STATISTICAL_CORE_INTERNAL_STRINGSTATISTICALSUMMARY_H
#define __TERRALIB_STATISTICAL_CORE_INTERNAL_STRINGSTATISTICALSUMMARY_H

// TerraLib
#include "Config.h"

// STL
#include <complex>
#include <map>

namespace te
{
  namespace stat
  {
    /*!
      \struct StringStatisticalSummary

      \brief A structure to hold the set of statistics from a set of categorical (sample) values.
    */
    struct TESTATEXPORT StringStatisticalSummary
    {
      public:

        /*! \brief Constructor. */
        StringStatisticalSummary();

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        StringStatisticalSummary(const StringStatisticalSummary& rhs);

        /*! \brief Destructor. */
        ~StringStatisticalSummary();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        StringStatisticalSummary& operator=(const StringStatisticalSummary& rhs);

        /*! \brief Clears all attributes from this class. */
        void clear();

      public:

        std::string m_minVal;
        std::string m_maxVal;
        std::string m_mode;
        int m_count;
        int m_validCount;
    };

  } // end namespace stat
} // end namespace te

#endif  // __TERRALIB_STATISTICAL_CORE_INTERNAL_STRINGSTATISTICALSUMMARY_H