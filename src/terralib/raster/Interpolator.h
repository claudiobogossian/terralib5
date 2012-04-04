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
  \file terralib/raster/Interpolator.h

  \brief It interpolates one pixel based on a selected algorithm.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_INTERPOLATOR_H
#define __TERRALIB_RASTER_INTERNAL_INTERPOLATOR_H

// TerraLib
#include "Raster.h"

// STL
#include <complex>

namespace te
{
  namespace rst
  {
// Forward declaration
    class Raster;

    /*!
      \class Interpolator

      \brief It interpolates one pixel based on a selected algorithm.

      Methods currently available are Nearest Neighbor, Bilinear, and Bicubic.
    */
    class TERASTEREXPORT Interpolator
    {
      public:

        /*!
          \brief Allowed interpolation methods.
        */
        enum Method
        {
          NearestNeighbor = 1,    //!< Near neighborhood interpolation method.
          Bilinear = 2,           //!< Bilinear interpolation method.
          Bicubic = 3             //!< Bicubic interpolation method.
        };

        /*!
          \brief Constructor.

          \param r The raster where to resample.
          \param m The method of interpolation to apply.
        */
        Interpolator(Raster* r, int m);

        /*! \brief Destructor. */
        virtual ~Interpolator();

        /*!
          \brief Get the interpolated value at specific band.

          \param c The column position (double).
          \param r The row position (double).
          \param v The output value.
          \param b The band to obtain the value.
        */
        void getValue(const double& c, const double& r, std::complex<double>& v, const std::size_t& b);

        /*!
          \brief Get the interpolated value for all bands.

          \param c The column position (double).
          \param r The row position (double).
          \param v A vector of values, for all bands.
        */
        void getValues(const double& c, const double& r, std::vector<std::complex<double> >& values);

      protected:

        /*!
          \brief Nearest neighbor interpolation method.

          \param c The column position (double).
          \param r The row position (double).
          \param v The output value.
          \param b The band to obtain the value.
        */
        void nearestNeighborGetValue(const double& c, const double& r, std::complex<double>& v, const std::size_t& b);

        /*!
          \brief Bilinear interpolation method.

          \param c The column position (double).
          \param r The row position (double).
          \param v The output value.
          \param b The band to obtain the value.
        */
        void bilinearGetValue(const double& c, const double& r, std::complex<double>& v, const std::size_t& b);

        /*!
          \brief Bicubic interpolation method.

          \param c The column position (double).
          \param r The row position (double).
          \param v The output value.
          \param b The band to obtain the value.
        */
        void bicubicGetValue(const double& c, const double& r, std::complex<double>& v, const std::size_t& b);

        /*!
          \brief Type definition for the interpolation function.

          \param c The column position (double).
          \param r The row position (double).
          \param v The output value.
          \param b The band to obtain the value.
        */
        typedef void (Interpolator::*InterpolationFunction)(const double& c, const double& r, std::complex<double>& v, const std::size_t& b);

    protected:

        Raster* m_raster;                                  //!< My input raster.
        int m_method;                                      //!< The interpolation method.
        InterpolationFunction m_function;                  //!< The current interpolation function pointer.

        /* Bilinear interapolation variables. */
        double m_bilRowMin;                                //!< Minimum row for bilinear interpolation.
        double m_bilRowMax;                                //!< Maximum row for bilinear interpolation.
        double m_bilColMin;                                //!< Minimum column for bilinear interpolation.
        double m_bilColMax;                                //!< Maximum column for bilinear interpolation.
        double m_bilRowDifMin;                             //!< Minimum difference between rows (min/max).
        double m_bilRowDifMax;                             //!< Maximum difference between rows (min/max).
        double m_bilColDifMin;                             //!< Minimum difference between columns (min/max).
        double m_bilColDifMax;                             //!< Maximum difference between columns (min/max).
        double m_bilDistances[4];                          //!< Bilinear distances.
        double m_bilWeights[4];                            //!< Bilinear weights;
        std::vector<std::complex<double> > m_bilValues;    //!< Bilinear values;
        double m_bilLastRow;                               //!< Last row available for bilinear interpolation.
        double m_bilLastCol;                               //!< Last column available for bilinear interpolation.

        /* Bicubic interapolation variables. */
        unsigned m_bicGridRow;
        unsigned m_bicGridCol;
        unsigned m_bicBufRow;
        unsigned m_bicBufCol;
        double m_bicBbufferReal[4][4];
        double m_bicBbufferImag[4][4];
        double m_bicOffsetX;
        double m_bicOffsetY;
        double m_bicKernel;
        double m_bicHWeights[4];
        double m_bicVWeights[4];
        double m_bicHSum;
        double m_bicVSum;
        double m_bicRowAccumReal;
        double m_bicRowAccumImag;
        double m_bicRowsValuesReal[4];
        double m_bicRowsValuesImag[4];
        double m_bicRowBound;                              //!< Last row available for bicubic interpolation.
        double m_bicColBound;                              //!< Last column available for bicubic interpolation.
    };

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_INTERPOLATOR_H
