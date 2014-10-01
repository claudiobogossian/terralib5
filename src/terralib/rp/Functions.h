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
  \file terralib/rp/Functions.h
  \brief Raster Processing functions.
 */

#ifndef __TERRALIB_RP_INTERNAL_FUNCTIONS_H
#define __TERRALIB_RP_INTERNAL_FUNCTIONS_H

#include "Config.h"

#include "Matrix.h"
#include "Macros.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../raster/Raster.h"
#include "../raster/RasterFactory.h"
#include "../raster/Grid.h"
#include "../raster/BandProperty.h"
#include "../raster/RasterFactory.h"
#include "../raster/Utils.h"

// STL
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <limits>

// Boost
#include <boost/numeric/ublas/matrix.hpp>

namespace te
{
  namespace rst
  {
    class BandProperty;
    class Grid;
  }

  namespace rp
  {
    class RasterHandler;
    
    /*!
      \brief Create a new raster into the givem data source.
      \param rasterGrid The template grid used to create the output raster.
      \param bandsProperties The template band properties used to create the output raster.
      \param outDataSetName The data set name to give to the created data set.
      \param dataSourceType The data source type string (i.e. GDAL, MEM.).
      \param outRasterHandler The created raster handler.
      \return true if OK, false on errors.
      \note All bandsProperties pointed objects will be acquired by this function and must not be deleted.
      \ingroup rp_func
     */
    bool TERPEXPORT CreateNewRaster( const te::rst::Grid& rasterGrid,
      const std::vector< te::rst::BandProperty* >& bandsProperties,
      const std::string& outDataSetName,
      const std::string& dataSourceType,
      RasterHandler& outRasterHandler );    

    /*!
      \brief Create a new raster into the givem data source.
      \param rasterGrid The template grid used to create the output raster.
      \param bandsProperties The template band properties used to create the output raster.
      \param outDataSetName The data set name to give to the created data set.
      \param outDataSource The data source to use when creating the output data set.
      \param outRasterHandler The created raster handler.
      \return true if OK, false on errors.
      \note All bandsProperties pointed objects will be acquired by this function and must not be deleted.
      \ingroup rp_func
     */
    bool TERPEXPORT CreateNewRaster( const te::rst::Grid& rasterGrid,
      const std::vector< te::rst::BandProperty* >& bandsProperties,
      const std::string& outDataSetName,
      te::da::DataSource& outDataSource,
      RasterHandler& outRasterHandler );

    /*!
      \brief Create a new raster into a new memory datasource.
      \param rasterGrid The template grid used to create the output raster.
      \param bandsProperties The template band properties used to create the output raster.
      \param outRasterHandler The created raster handler.
      \return true if OK, false on errors.
      \note All bandsProperties pointed objects will be acquired by this function and must not be deleted.
      \ingroup rp_func
     */
    bool TERPEXPORT CreateNewMemRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      RasterHandler& outRasterHandler );

    /*!
      \brief Create a new raster into a GDAL datasource.
      \param rasterGrid The template grid used to create the output raster.
      \param bandsProperties The template band properties used to create the output raster.
      \param fileName The output tif file name.
      \param outRasterHandler The created raster handler.
      \return true if OK, false on errors.
      \note All bandsProperties pointed objects will be acquired by this function and must not be deleted.
      \ingroup rp_func
     */
    bool TERPEXPORT CreateNewGdalRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      const std::string& fileName,
      RasterHandler& outRasterHandler );
      
    /*!
      \brief Create a new raster into a GDAL datasource.
      \param rasterGrid The template grid used to create the output raster.
      \param bandsProperties The template band properties used to create the output raster.
      \param fileName The output tif file name.
      \param outRasterHandler The created raster handler.
      \return true if OK, false on errors.
      \note All bandsProperties pointed objects will be acquired by this function and must not be deleted.
      \ingroup rp_func
     */
    bool TERPEXPORT Copy2DiskRaster( 
      const te::rst::Raster& inputRaster,
      const std::string& fileName );      

    /*!
      \brief Returns the real data type range (all values that can be represented by the given data type).
      \param dataType The data type.
      \param min The minimum value.
      \param max The maximum value.
      \note The types are listed in terralib/datatype/DataTypes.h
      \ingroup rp_func
    */
    inline void TERPEXPORT GetDataTypeRange( const int dataType, double& min, double& max )
    {
      te::rst::GetDataTypeRanges( dataType, min, max );
    }

    /*!
      \brief Convert vector elements.
      \param inputVector Input vector.
      \param inputVectorDataType Input vector data type.
      \param inputVectorSize The numer of input vector elements.
      \param outputVector A pré-allocated output vector.
      \note The types are listed in terralib/datatype/DataTypes.h
      \ingroup rp_func
    */
    void TERPEXPORT Convert2DoublesVector( void* inputVector, const int inputVectorDataType,
      unsigned int inputVectorSize, double* outputVector );

    /*!
      \brief Convert a doubles vector.
      \param inputVector Input vector.
      \param inputVectorSize The numer of input vector elements.
      \param inputVectorDataType Input vector data type.
      \param outputVector A pré-allocated output vector.
      \note The types are listed in terralib/datatype/DataTypes.h
      \ingroup rp_func
    */
    void TERPEXPORT ConvertDoublesVector( double* inputVector,
      unsigned int inputVectorSize, const int outputVectorDataType,
      void* outputVector );
      
    /*!
      \brief Create a tiff file from a matrix.
      \param matrix The matrix.
      \param normalize Enable/disable pixel normalization (8bit);
      \param tifFileName Tif file name.
      \param return true if OK, false on errors.
      \ingroup rp_func
    */      
    template< typename MatrixElementT >
    bool CreateRasterFileFromMatrix( const te::rp::Matrix< MatrixElementT >& matrix,
      const bool normalize, const std::string& fileName )
    {
      std::map<std::string, std::string> rInfo;
      rInfo["URI"] = fileName;
      
      std::vector<te::rst::BandProperty*> bandsProperties;
      if( normalize )
        bandsProperties.push_back(new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE, "" ));
      else
        bandsProperties.push_back(new te::rst::BandProperty( 0, te::dt::DOUBLE_TYPE, "" ));
      bandsProperties[0]->m_colorInterp = te::rst::GrayIdxCInt;
      bandsProperties[0]->m_noDataValue = -1.0 * std::numeric_limits<double>::max();

      te::rst::Grid* newgrid = new te::rst::Grid( matrix.getColumnsNumber(),
        matrix.getLinesNumber(), 0, -1 );

      std::auto_ptr< te::rst::Raster > outputRasterPtr(
        te::rst::RasterFactory::make( "GDAL", newgrid, bandsProperties, rInfo, 0, 0));
      TERP_TRUE_OR_RETURN_FALSE( outputRasterPtr.get(), "Output raster creation error");
          
      unsigned int line = 0;
      unsigned int col = 0;
      const unsigned int nLines = matrix.getLinesNumber();
      const unsigned int nCols = matrix.getColumnsNumber();
      MatrixElementT matrixValue = 0;

      MatrixElementT gain = 1.0;
      MatrixElementT offset = 0.0;
      if( normalize )
      {
        MatrixElementT matrixValueMin = std::numeric_limits< MatrixElementT >::max();
        MatrixElementT matrixValueMax = -1.0 * matrixValueMin;
        for( line = 0 ; line < nLines ; ++line )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            matrixValue = matrix[ line ][ col ];
            if( matrixValue < matrixValueMin )
              matrixValueMin = matrixValue;
            if( matrixValue > matrixValueMax )
              matrixValueMax = matrixValue;
          }
        }
          
        if( matrixValueMax == matrixValueMin )
        {
          gain = 0.0;
          offset = 0.0;
        }
        else
        {
          gain = 255.0 / ( matrixValueMax - matrixValueMin );
          offset = -1.0 * ( matrixValueMin );
        }
      }
      
      const MatrixElementT min0 = 0;
      const MatrixElementT max255 = 255;
      
      for( line = 0 ; line < nLines ; ++line )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          matrixValue = matrix[ line ][ col ];
        
          if( normalize )
          {
            matrixValue += offset;
            matrixValue *= gain;
            matrixValue = std::max( min0, matrixValue );
            matrixValue = std::min( max255, matrixValue );
          }
          
          outputRasterPtr->setValue( col, line, (double)matrixValue, 0 );
        }
      }
      
      return true;
    } 
    
    /*! 
      \brief Returns a vector os with band's names. 
      \ingroup rp_func
    */
    TERPEXPORT std::vector<std::string> GetBandNames();

    /*! 
      \brief Returns the maximun and minimum reflectance values of a given sensor/band. 
      \ingroup rp_func
    */
    TERPEXPORT std::pair<double, double> GetSpectralBandInfo(std::string bandName);

    /*! 
      \brief Returns the minimum reflectance value of a given sensor/band. 
      \ingroup rp_func
    */
    TERPEXPORT double GetSpectralBandMin(std::string bandName);

    /*! 
      \brief Returns the maximum reflectance value of a given sensor/band. 
      \ingroup rp_func
    */
    TERPEXPORT double GetSpectralBandMax(std::string bandName);

    /*! 
      \brief Returns the maximun and minimum digital numbers of a given sensor/band. 
      \ingroup rp_func
    */
    TERPEXPORT std::pair<double, double> GetDigitalNumberBandInfo(std::string bandName);

    /*! 
      \brief Returns the maximum digital number of a given sensor/band. 
      \ingroup rp_func
    */
    TERPEXPORT double GetDigitalNumberBandMax(std::string bandName);

    /*!
      \brief Normalizes one raster in a given interval.

      \param inputRaster     The given raster.
      \param nmin            The new minimum value (default = 0.0).
      \param nmax            The new maximum value (default = 255.0).

      \return true if normalization occurs and false otherwise.
      \ingroup rp_func
    */
    TERPEXPORT bool NormalizeRaster(te::rst::Raster& inputRaster, double nmin = 0.0, double nmax = 255.0);

    /*!
      \brief Creates a vector of random positions (points) inside the raster.

      \param inputRaster     The given raster.
      \param numberOfPoints  The number of random positions to be created (default = 1000).

      \return A vector of random positions (points).
      \ingroup rp_func
    */
    TERPEXPORT std::vector<te::gm::Point*> GetRandomPointsInRaster(const te::rst::Raster& inputRaster, unsigned int numberOfPoints = 1000);    
    
    /*!
      \brief RGB to IHS conversion.
      \param inputRGBRaster The input raster.
      \param redBandIdx The red band index.
      \param greenBandIdx The red band index.
      \param blueBandIdx The red band index.
      \param rgbRangeMin The minimum RGB value.
      \param rgbRangeMax The maximum RGB value.
      \param outputIHSRaster An output pré-initiated raster (with the same dimensions of inputRGBRaster) where the IHS data will be written.
      \return true if OK, false on errors.
      \note The outputIHSRaster mas have a float or double data type. 
      \note IHS data with the following channels ranges: I:[0,1] H:[0,2pi] (radians) S:[0,1].
      \ingroup rp_func   
    */
    TERPEXPORT bool ConvertRGB2IHS( const te::rst::Raster& inputRGBRaster, 
      const unsigned int redBandIdx, const unsigned int greenBandIdx,
      const unsigned int blueBandIdx, const double rgbRangeMin, 
      const double rgbRangeMax, te::rst::Raster& outputIHSRaster );  
      
    /*!
      \brief IHS to RGB conversion.
      \param inputIHSRaster The input raster.
      \param intensityBandIdx The intensity band index.
      \param hueBandIdx The hue band index.
      \param saturationBandIdx The saturation band index.
      \param rgbRangeMin The minimum RGB value.
      \param rgbRangeMax The maximum RGB value.      
      \param outputRGBRaster An output pré-initiated raster (with the same dimensions of inputIHSRaster) where the RGB data will be written.
      \return true if OK, false on errors.
      \note IHS data with the following channels ranges: I:[0,1] H:[0,2pi] (radians) S:[0,1].      
      \ingroup rp_func
    */
    TERPEXPORT bool ConvertIHS2RGB( const te::rst::Raster& inputIHSRaster, 
      const unsigned int intensityBandIdx, const unsigned int hueBandIdx,
      const unsigned int saturationBandIdx, const double rgbRangeMin, 
      const double rgbRangeMax, te::rst::Raster& outputRGBRaster );      
      
    /*!
      \brief Get the mean of band pixel values.
      \param band The input raster band.
      \param maxThreads The maximum number of threads to use (0-auto, 1-single thread used).
      \param meanValue The calculated mean value.
      \return true if OK, false on errors.
      \note Optimized for rasters where the used bands have the same blocking scheme.
    */
    TERPEXPORT bool GetMeanValue( 
      const te::rst::Band& band, 
      const unsigned int maxThreads, 
      double& meanValue );    
      
    /*!
      \brief Get the standard deviation of band pixel values.
      \param band The input raster band.
      \param maxThreads The maximum number of threads to use (0-auto, 1-single thread used).
      \param meanValuePtr A pointer to a pre-calculated band mean value or zero if it does not exist.
      \param stdDevValue The calculated standard deviation value.
      \return true if OK, false on errors.
      \note Optimized for rasters where the used bands have the same blocking scheme.
    */
    TERPEXPORT bool GetStdDevValue( 
      const te::rst::Band& band, 
      const unsigned int maxThreads, 
      double const * const meanValuePtr, 
      double& stdDevValue );         
      
    /*!
      \brief Get the covariance of band pixel values.
      \param band1 The input band 1.
      \param band2 The input band 2.
      \param maxThreads The maximum number of threads to use (0-auto, 1-single thread used).
      \param mean1ValuePtr A pointer to a pre-calculated band1 mean value or zero if it does not exist.
      \param mean2ValuePtr A pointer to a pre-calculated band2 mean value or zero if it does not exist.
      \param covarianceValue The calculated covariance value.
      \return true if OK, false on errors.
      \note Optimized for rasters where the used bands have the same blocking scheme.
      \ingroup rp_func
    */
    TERPEXPORT bool GetCovarianceValue( 
      const te::rst::Band& band1, 
      const te::rst::Band& band2, 
      const unsigned int maxThreads, 
      double const * const mean1ValuePtr, 
      double const * const mean2ValuePtr,  
      double& covarianceValue );      
      
    /*!
      \brief Generate all principal components from the given input raster.
      \param inputRaster Input raster.
      \param inputRasterBands Input raster bands.
      \param pcaMatrix The matrix generated over the principal components process.
      \param pcaRaster The pré-initiated output PCA raster (with the same dimensions of inputRaster).
      \param maxThreads The maximum number of threads to use (0-auto, 1-single thread used).
      \return true if OK, false on errors.
      \note Optimized for rasters where the used bands have the same blocking scheme.
      \ingroup rp_func
    */
    TERPEXPORT bool DirectPrincipalComponents( 
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,
      boost::numeric::ublas::matrix< double >& pcaMatrix,
      te::rst::Raster& pcaRaster,
      const unsigned int maxThreads );  
      
    /*!
      \brief Regenerate the original raster from its principal components.
      \param pcaRaster The principal components raster (with the same dimensions of outputRaster).
      \param pcaMatrix The matrix generated by the direct principal components process.
      \param outputRaster The regenerated output raster (with the same dimentions.
      \param maxThreads The maximum number of threads to use (0-auto, 1-single thread used).
      \return true if OK, false on errors.
      \note Optimized for rasters where the used bands have the same blocking scheme.
      \ingroup rp_func
    */
    TERPEXPORT bool InversePrincipalComponents( 
      const te::rst::Raster& pcaRaster,
      const boost::numeric::ublas::matrix< double >& pcaMatrix,
      te::rst::Raster& outputRaster,
      const unsigned int maxThreads );       
      
    /*!
      \brief Remap pixel values using a remap function matrix.
      \param inputRaster Input raster.
      \param inputRasterBands Input raster bands.
      \param remapMatrix The remap function matrix.
      \param outputRaster The pré-initiated output raster (with the same dimentions of inputRaster).
      \param maxThreads The maximum number of threads to use (0-auto, 1-single thread used).
      \return true if OK, false on errors.
      \note Optimized for rasters where the used bands have the same blocking scheme.
      \ingroup rp_func
    */
    TERPEXPORT bool RemapValues( 
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,
      const boost::numeric::ublas::matrix< double >& remapMatrix,
      te::rst::Raster& outputRaster,
      const unsigned int maxThreads );      
    
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_FUNCTIONS_H

