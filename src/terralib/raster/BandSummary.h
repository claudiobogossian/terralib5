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
  \file terralib/raster/BandSummary.h
 
  \brief A struct that can be used to keep the summary of a raster band (most statistics).
*/

#ifndef __TERRALIB_RASTER_INTERNAL_BANDSUMMARY_H
#define __TERRALIB_RASTER_INTERNAL_BANDSUMMARY_H

// TerraLib
#include "Config.h"

// STL
#include <complex>
#include <map>

namespace te
{
  namespace rst
  {
    /*!
      \struct BandSummary

      \brief A summary of a raster band (most statistics).

      \ingroup rst
    */
    struct TERASTEREXPORT BandSummary
    {
      public:

        /*! \brief Constructor. */
        BandSummary();

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy used to copy from.
        */
        BandSummary(const BandSummary& rhs);

        /*! \brief Destructor. */
        ~BandSummary();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy used to copy from.

          \return A reference to this object.
        */
        BandSummary& operator=(const BandSummary& rhs);

        /*! \brief Clears all attributes from this class. */
        void clear();

      public:

        std::complex<double>* m_minVal;                    //!< The minimum occurring values (real and imaginary), default is std::numeric_limits<double>::min().
        std::complex<double>* m_maxVal;                    //!< The maximum occurring values (real and imaginary), default is std::numeric_limits<double>::max().
        std::complex<double>* m_stdVal;                    //!< The standard deviation of the occurring values (real and imaginary), default is 1.0.
        std::complex<double>* m_meanVal;                   //!< The mean of the occurring values (real and imaginary), default is 0.0.
        std::map<double, unsigned int>* m_histogramR;      //!< The histogram (a map of occurring values and frequency) of the real part of a band.
        std::map<double, unsigned int>* m_histogramI;      //!< The histogram (a map of occurring values and frequency) of the imaginary part of a band.
    };

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_BANDSUMMARY_H
