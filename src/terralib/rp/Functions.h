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
  \ingroup RPModule
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

// Boost
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>

// STL
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <limits>

namespace te
{
  namespace rst
  {
    class BandProperty;
    class Grid;
  };

  namespace rp
  {
    class RasterHandler;
    
    /** \addtogroup RPFunctions 
     *  @{
     */    

    /*!
      \brief Create a new raster into the givem data source.
      \param rasterGrid The template grid used to create the output raster.
      \param bandsProperties The template band properties used to create the output raster.
      \param outDataSetName The data set name to give to the created data set.
      \param outDataSource The data source to use when creating the output data set.
      \param outRasterHandler The created raster handler.
      \return true if OK, false on errors.
      \note All bandsProperties pointed objects will be acquired by this function and must not be deleted.
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
     */
    bool TERPEXPORT CreateNewMemRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      RasterHandler& outRasterHandler );

    /*!
      \brief Create a new geotiff raster.
      \param rasterGrid The template grid used to create the output raster.
      \param bandsProperties The template band properties used to create the output raster.
      \param fileName The output tif file name.
      \param outRasterHandler The created raster handler.
      \return true if OK, false on errors.
      \note All bandsProperties pointed objects will be acquired by this function and must not be deleted.
     */
    bool TERPEXPORT CreateNewGeotifRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      const std::string& fileName,
      RasterHandler& outRasterHandler );

    /*!
      \brief Returns the real data type range (all values that can be represented by the given data type).
      \param dataType The data type.
      \param min The minimum value.
      \param max The maximum value.
      \note The types are listed in terralib/datatype/DataTypes.h
    */
    void TERPEXPORT GetDataTypeRange( const int dataType, double& min, double& max );

    /*!
      \brief Convert vector elements.
      \param inputVector Input vector.
      \param inputVectorDataType Input vector data type.
      \param inputVectorSize The numer of input vector elements.
      \param outputVector A pré-allocated output vector.
      \note The types are listed in terralib/datatype/DataTypes.h
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
    }; 
    
    /*! \brief Returns a vector os with band's names. */
    TERPEXPORT std::vector<std::string> GetBandNames();

    /*! \brief Returns the maximun and minimum reflectance values of a given sensor/band. */
    TERPEXPORT std::pair<double, double> GetSpectralBandInfo(std::string bandName);

    /*! \brief Returns the minimum reflectance value of a given sensor/band. */
    TERPEXPORT double GetSpectralBandMin(std::string bandName);

    /*! \brief Returns the maximum reflectance value of a given sensor/band. */
    TERPEXPORT double GetSpectralBandMax(std::string bandName);

    /*! \brief Returns the maximun and minimum digital numbers of a given sensor/band. */
    TERPEXPORT std::pair<double, double> GetDigitalNumberBandInfo(std::string bandName);

    /*! \brief Returns the maximum digital number of a given sensor/band. */
    TERPEXPORT double GetDigitalNumberBandMax(std::string bandName);

    /*!
      \brief Normalizes one raster in a given interval.

      \param inputRaster     The given raster.
      \param nmin            The new minimum value (default = 0.0).
      \param nmax            The new maximum value (default = 255.0).

      \return true if normalization occurs and false otherwise.
    */
    TERPEXPORT bool NormalizeRaster(te::rst::Raster& inputRaster, double nmin = 0.0, double nmax = 255.0);

    /*!
      \brief Creates a vector of random positions (points) inside the raster.

      \param inputRaster     The given raster.
      \param numberOfPoints  The number of random positions to be created (default = 1000).

      \return A vector of random positions (points).
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
      \param outputIHSRaster The output IHS raster.
      \return true if OK, false on errors.
      \note IHS data with the following channels ranges: I:[0,1] H:[0,2pi] (radians) S:[0,1].      
    */
    TERPEXPORT bool ConvertRBG2IHS( const te::rst::Raster& inputRGBRaster, 
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
      \param outputRGBRaster The output RGB raster.
      \return true if OK, false on errors.
      \note IHS data with the following channels ranges: I:[0,1] H:[0,2pi] (radians) S:[0,1].      
    */
    TERPEXPORT bool ConvertIHS2RGB( const te::rst::Raster& inputIHSRaster, 
      const unsigned int intensityBandIdx, const unsigned int hueBandIdx,
      const unsigned int saturationBandIdx, const double rgbRangeMin, 
      const double rgbRangeMax, te::rst::Raster& outputRGBRaster );      
    
    /** @}*/

  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_FUNCTIONS_H

