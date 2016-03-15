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
  \file terralib/rp/RasterAttributes.h

  \brief Extraction of attributes from Raster, Bands, and Polygons.
 */

#ifndef __TERRALIB_RP_INTERNAL_RASTERATTRIBUTES_H
#define __TERRALIB_RP_INTERNAL_RASTERATTRIBUTES_H

// TerraLib
#include "../statistics/core/NumericStatisticalSummary.h"
#include "../statistics/core/NumericStatisticalComplexSummary.h"
#include "Algorithm.h"
#include "Config.h"
#include "Exception.h"
#include "Texture.h"

// Boost
#include <boost/numeric/ublas/io.hpp> 
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
        std::vector<std::vector<std::complex<double> > > getComplexValuesFromRaster(const te::rst::Raster& raster, const te::gm::Polygon& polygon, std::vector<unsigned int> bands, unsigned int rowstep = 1, unsigned int colstep = 1);

        /*!
          \brief Returns the pixel values for all the bands in raster, inside the polygon.

          \param raster      The input band.
          \param polygon     The input polygon.
          \param bands       Bands to be processed from the input raster.

          \return A vector with the pixel values for all the bands in raster, inside the polygon, with values[band][pixel].

          \warning Bands and polygon must fit.
        */
        std::vector<std::vector<double> > getValuesFromRaster(const te::rst::Raster& raster, const te::gm::Polygon& polygon, std::vector<unsigned int> bands, unsigned int rowstep = 1, unsigned int colstep = 1);

        /*!
          \brief Returns several statistics from a set of pixels.

          \param pixels      A vector of pixel values.

          \return A series of statistics (\sa te::stat::NumericStatisticalSummary).
        */
        te::stat::NumericStatisticalSummary getStatistics(std::vector<double>& pixels);

		/*!
          \brief Returns several statistics from a set of pixels (real and imag).

          \param pixels      A vector of pixel values.

          \return A series of statistics (\sa te::stat::NumericStatisticalSummary).
        */
        te::stat::NumericStatisticalComplexSummary getComplexStatistics(std::vector <std::complex <double> >& pixels);

        /*!
          \brief Returns the covariance matrix between vectors of pixel values.

          \param vpixels     The vector of pixel vectors, with vpixels[band][pixel].
          \param vmeans      The vector of pixels means, one mean per vector of pixels.

          \return The covariance matrix between the vectors of pixel values.

          \warning All vectors sizes must fit.
        */
        boost::numeric::ublas::matrix<double> getCovarianceMatrix(const std::vector<std::vector<double> >& vpixels, const std::vector<double>& vmeans);
        
        /*!
          \brief Returns the covariance matrix between vectors of pixel values.

          \param vpixels     The vector of pixel vectors, with vpixels[band][pixel].
          \param vmeans      The vector of pixels means, one mean per vector of pixels.

          \return The covariance matrix between the vectors of pixel values.

          \warning All vectors sizes must fit.
        */
        boost::numeric::ublas::matrix<std::complex <double> > getComplexCovarianceMatrix(const std::vector<std::vector<std::complex < double> > >& vpixels, const std::vector<std::complex < double > >& vmeans);

		/*!
          \brief Computes the Gray-Level Co-occurrence Matrix (GLCM) from a raster band.

          \param rin               The input raster.
          \param band              The input band position.
          \param dx                The displacement in x direction, to be considered as neighborhood, can be either + or -.
          \param dy                The displacement in y direction, to be considered as neighborhood, can be either + or -.
          \param minPixel          The minimum GL pixel value that occurs in the band.
          \param maxPixel          The maximum GL pixel value that occurs in the band.
          \param gLevels           The number of Gray Levels (GL) to normalize the GLCM. The GLs will be normalized to the interval
                                   [0,(gLevels - 1)]. It is important to emphasize that the matrix will have 'gLevels' lines and 
                                   'gLevels' columns. Default value is 256, normalizing the GLs to the interval [0, 255].
          
          \return The GLCM from the raster band.

          \exception te::common::Exception It will throw an exception if the parameters minPixel and maxPixel were not passed as parameters.
          
          \warning The pixels from the input band will be considered of type unsigned int.
        */
        boost::numeric::ublas::matrix<double> getGLCM(const te::rst::Raster& rin, unsigned int band, int dx, int dy, 
            double minPixel = 0, double maxPixel = 0, double gLevels = 256);
        
        /*!
          \brief Computes the Gray-Level Co-occurrence Matrix (GLCM) from a raster band, inside the polygon.

          \param rin               The input raster.
          \param band              The input band position.
          \param dx                The displacement in x direction, to be considered as neighborhood, can be either + or -.
          \param dy                The displacement in y direction, to be considered as neighborhood, can be either + or -.
          \param polygon           The input polygon.
          \param minPixel          The minimum GL pixel value that occurs in the band.
          \param maxPixel          The maximum GL pixel value that occurs in the band.
          \param readAll           Define if the minimum and maximum values used to normalize the GLs when coomputing the GLCM will be retrieved 
                                   from the entire band or from a 1000 pixels sample (see te::rst::Band::getMinValue() and 
                                   te::rst::Band::getMaxValue() for more details). Default value is false.
          \param gLevels           The number of Gray Levels (GL) to normalize the GLCM. The GLs will be normalized to the interval
                                   [0,(gLevels - 1)]. It is important to emphasize that the matrix will have 'gLevels' lines and 
                                   'gLevels' columns. Default value is 256, normalizing the GLs to the interval [0, 255].
          
          \return The GLCM from the raster band, inside the polygon.

          \exception te::common::Exception It will throw an exception if the parameters minPixel and maxPixel were not passed as parameters.
          
          \warning The pixels from the input band will be considered of type unsigned int.
        */
        boost::numeric::ublas::matrix<double> getGLCM(const te::rst::Raster& rin, unsigned int band, int dx, int dy, 
            const te::gm::Polygon& polygon, double minPixel = 0, double maxPixel = 0, double gLevels = 256);
        
        /*!
          \brief Compute texture metrics from GLCM matrix.
          
          \param glcm The input GLCM matrix.
          
          \return The Texture structure will all available metrics computed
        */
        te::rp::Texture getGLCMMetrics(boost::numeric::ublas::matrix<double> glcm);
        
    };

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_RASTERATTRIBUTES_H
