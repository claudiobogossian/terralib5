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
  \file terralib/rp/RasterAttributes.h

  \brief Extraction of attributes from Raster, Bands, and Polygons.
 */

#ifndef __TERRALIB_RP_INTERNAL_RASTERATTRIBUTES_H
#define __TERRALIB_RP_INTERNAL_RASTERATTRIBUTES_H

// TerraLib
#include "../statistics/core/NumericStatisticalSummary.h"
#include "Algorithm.h"
#include "Config.h"
#include "Exception.h"

// Boost
#include <boost/numeric/ublas/matrix.hpp>

// STL
#include <complex>

namespace te
{
// Forward declaration
  namespace gm
  {
    class Polygon;
  }
  namespace rst
  {
    class Band;
    class Raster;
  }

  namespace rp
  {

    /*!
      \class RasterAttributes

      \brief Extraction of attributes from Raster, Bands, and Polygons.
    */
    class TERPEXPORT RasterAttributes : public Algorithm
    {
      public:

        /*! \brief Public constructor. */
        RasterAttributes();

        ~RasterAttributes();

        // overload
        bool initialize(const AlgorithmInputParameters& inputParams) throw(te::rp::Exception);

        //overload
        bool isInitialized() const;

        // overload
        bool execute(AlgorithmOutputParameters& outputParams) throw(te::rp::Exception);

        // overload
        void reset() throw(te::rp::Exception);

        /*!
          \brief Returns the pixel values (real and imag) for the band, inside the polygon.

          \param raster      The input raster.
          \param band        The position of the input band.
          \param polygon     The input polygon.

          \return The pixel values for the band, inside the polygon.

          \warning Band and polygon must fit.
        */
        std::vector<std::complex<double> > getComplexValuesFromBand(const te::rst::Raster& raster, unsigned int band, const te::gm::Polygon& polygon);

        /*!
          \brief Returns the pixel values for the band, inside the polygon.

          \param raster      The input raster.
          \param band        The position of the input band.
          \param polygon     The input polygon.

          \return The pixel values for the band, inside the polygon.

          \warning Band and polygon must fit.
        */
        std::vector<double> getValuesFromBand(const te::rst::Raster& raster, unsigned int band, const te::gm::Polygon& polygon);

        /*!
          \brief Returns the pixel values (real and imag) for all the bands in raster, inside the polygon.

          \param raster      The input band.
          \param polygon     The input polygon.
          \param bands       Bands to be processed from the input raster.

          \return A vector with the pixel values for all the bands in raster, inside the polygon, with values[band][pixel].

          \warning Bands and polygon must fit.
        */
        std::vector<std::vector<std::complex<double> > > getComplexValuesFromRaster(const te::rst::Raster& raster, const te::gm::Polygon& polygon, std::vector<unsigned int> bands);

        /*!
          \brief Returns the pixel values for all the bands in raster, inside the polygon.

          \param raster      The input band.
          \param polygon     The input polygon.
          \param bands       Bands to be processed from the input raster.

          \return A vector with the pixel values for all the bands in raster, inside the polygon, with values[band][pixel].

          \warning Bands and polygon must fit.
        */
        std::vector<std::vector<double> > getValuesFromRaster(const te::rst::Raster& raster, const te::gm::Polygon& polygon, std::vector<unsigned int> bands);

        /*!
          \brief Returns several statistics from a set of pixels.

          \param pixels      A vector of pixel values.

          \return A series of statistics (\sa te::stat::NumericStatisticalSummary).
        */
        te::stat::NumericStatisticalSummary getStatistics(std::vector<double>& pixels);

        /*!
          \brief Returns the covariance matrix between vectors of pixel values.

          \param vpixels     The vector of pixel vectors, with vpixels[band][pixel].
          \param vmeans      The vector of pixels means, one mean per vector of pixels.

          \return The covariance matrix between the vectors of pixel values.

          \warning All vectors sizes must fit.
        */
        boost::numeric::ublas::matrix<double> getCovarianceMatrix(const std::vector<std::vector<double> >& vpixels, const std::vector<double>& vmeans);
    };

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_RASTERATTRIBUTES_H
