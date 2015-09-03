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
  \file terralib/raster/Interpolator.h

  \brief It interpolates one pixel based on a selected algorithm.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_INTERPOLATOR_H
#define __TERRALIB_RASTER_INTERNAL_INTERPOLATOR_H

// TerraLib
#include "Raster.h"
#include "Enums.h"

// STL
#include <complex>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

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

      \ingroup rst
    */
    class TERASTEREXPORT Interpolator : public boost::noncopyable
    {
      public:

        /*!
          \brief Allowed interpolation methods.
        */
        typedef InterpolationMethod Method;

        /*!
          \brief Constructor.

          \param r The raster where to resample.
          \param m The method of interpolation to apply.
        */
        Interpolator(Raster const* r, int m);
        
        /*!
          \brief Constructor.

          \param r The raster where to resample.
          \param m The method of interpolation to apply.
          \param noDataValues A vector with no-data values (will overwride the input raster no-data values) or an empty vectory if the input raster no-data values must be used.
        */
        Interpolator(Raster const* r, int m, const std::vector< std::complex<double> >& noDataValues);        

        /*! \brief Destructor. */
        virtual ~Interpolator();

        /*!
          \brief Get the interpolated value at specific band.

          \param c The column position (double).
          \param r The row position (double).
          \param v The output value or the current input raster no-data value if the requested coordinates are outside the valid image bounds.
          \param b The band to obtain the value.
          \note The caller of this method must be aware that the returned value may be outside the original input rasters valid values range.
        */
        inline void getValue(const double& c, const double& r, std::complex<double>& v, const std::size_t& b)
        {
          assert(b < m_raster->getNumberOfBands());
          (this->*(m_function))(c, r, v, b);
        };

        /*!
          \brief Get the interpolated value for all bands.

          \param c The column position (double).
          \param r The row position (double).
          \param v A vector of values, for all bands, or the current input raster no-data values if the requested coordinates are outside the valid image bounds..
          \note The caller of this method must be aware that the returned values may be outside the original input rasters valid values range.
        */
        void getValues(const double& c, const double& r, std::vector<std::complex<double> >& values);
        
        /*!
          \brief Create a clone copy of this instance.
          
          \return A clone copy of this instance (the caller of this method must take the ownership of the returned object).
        */        
        te::rst::Interpolator* clone() const;

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
        
        /*!
          \brief Initialize this instance..

          \param r The raster where to resample.
          \param method The method of interpolation to apply.
          \param noDataValues A vector with no-data values (will overwride the input raster no-data values) or an empty vectory if the input raster no-data values must be used.
          \return true if OK, false on errors.
        */
        bool initialize(Raster const * const rasterPointer, int method,
          const std::vector< std::complex<double> >& noDataValues );        

    protected:

        Raster const* m_raster;                                  //!< My input raster.
        int m_method;                                      //!< The interpolation method.
        InterpolationFunction m_function;                  //!< The current interpolation function pointer.
        std::vector< std::complex<double> > m_noDataValues;              //!< Raster no-data values (for each band);
        
        // nearest Neighbor interpolation variables
        
        unsigned int m_nnCR;
        unsigned int m_nnRR;
        double m_nnLastRow;                               //!< Last row available for nearest Neighbor interpolation.
        double m_nnLastCol;                               //!< Last column available for nearest Neighbor interpolation.
        

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
        double m_bicReadRealValue;
        double m_bicReadImagValue;
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
