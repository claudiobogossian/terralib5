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
  \file terralib/rp/Functions.cpp

  \brief Raster Processing functions.
*/

// TerraLib

#include "Functions.h"

#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Enums.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterFactory.h"
#include "../raster/RasterProperty.h"
#include "../raster/RasterIterator.h"
#include "../raster/Utils.h"
#include "../geometry/Point.h"
#include "../common/MatrixUtils.h"
#include "Exception.h"
#include "Macros.h"
#include "RasterHandler.h"

// Boost
#include <boost/filesystem.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/random.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>


// STL
#include <cstring>
#include <string>
#include <limits>
#include <map>
#include <memory>

#ifndef M_PI
  #define M_PI       3.14159265358979323846
#endif

namespace te
{
  namespace rp
  {
    struct GetMeanValueThreadParams
    {
      te::rst::Band const * m_inputBandPtr;
      Matrix< bool >* m_rasterBlocksStatusPtr;
      double* m_meanValuePtr;
      double* m_pixelsNumberValuePtr;
      bool m_returnStatus;
      boost::mutex* m_mutexPtr;
    };  
    
    struct GetStdDevValueThreadParams
    {
      te::rst::Band const * m_inputBandPtr;
      Matrix< bool >* m_rasterBlocksStatusPtr;
      double* m_stdDevValuePtr;
      double* m_pixelsNumberValuePtr;
      double m_meanValue;
      bool m_returnStatus;
      boost::mutex* m_mutexPtr;
    };     
    
    struct GetCovarianceValueThreadParams
    {
      te::rst::Band const * m_inputBand1Ptr;
      te::rst::Band const * m_inputBand2Ptr;
      Matrix< bool >* m_rasterBlocksStatusPtr;
      double* m_covarianceValuePtr;
      double* m_pixelsNumberValuePtr;
      bool m_returnStatus;
      double m_mean1Value;
      double m_mean2Value;
      boost::mutex* m_mutexPtr;
    };   
    
    struct RemapValuesThreadParams
    {
      te::rst::Raster const * m_inputRasterPtr;
      std::vector< unsigned int > const * m_inputRasterBandsPtr;
      te::rst::Raster* m_outputRasterPtr;
      std::vector< unsigned int > const * m_outputRasterBandsPtr;
      boost::numeric::ublas::matrix< double > const * m_remapMatrixPtr;
      Matrix< bool > * m_rasterBlocksStatusPtr;
      bool m_returnStatus;
      boost::mutex* m_mutexPtr;
    };     
    
    bool CreateNewRaster( const te::rst::Grid& rasterGrid,
      const std::vector< te::rst::BandProperty* >& bandsProperties,
      const std::string& outDataSetName,
      const std::string& dataSourceType,
      RasterHandler& outRasterHandler )
    {
      // Creating a new datasource

      std::auto_ptr< te::da::DataSource > dataSourcePtr(
        te::da::DataSourceFactory::make( dataSourceType ) );
      if( dataSourcePtr.get() == 0 ) return false;      
      
      RasterHandler internalRasterHandler;
      
      if( CreateNewRaster( rasterGrid, bandsProperties, 
        outDataSetName, *dataSourcePtr, internalRasterHandler ) )
      {
        std::auto_ptr< te::da::DataSource > dummyDataSourcePtr;
        std::auto_ptr< te::da::DataSourceTransactor > transactorPtr;
        std::auto_ptr< te::da::DataSet > dataSetPtr;
        std::auto_ptr< te::rst::Raster > rasterPtr;
        
        internalRasterHandler.release( dummyDataSourcePtr, transactorPtr,
          dataSetPtr, rasterPtr );
          
        outRasterHandler.reset( dataSourcePtr.release(), transactorPtr.release(), 
          dataSetPtr.release(), rasterPtr.release() );
        
        return true;
      }
      else
      {
        return false;
      }      
    }
        
    bool CreateNewRaster( const te::rst::Grid& rasterGrid,
      const std::vector< te::rst::BandProperty* >& bandsProperties,
      const std::string& outDataSetName,
      te::da::DataSource& outDataSource,
      RasterHandler& outRasterHandler )
    {
      // Defining the raster properties

      std::auto_ptr< te::rst::RasterProperty > rasterPropertyPtr(
        new te::rst::RasterProperty( new te::rst::Grid( rasterGrid ),
        bandsProperties, std::map< std::string, std::string >(),
        false, 0, 0 ) );

      // acquiring a transactor instance

      std::auto_ptr< te::da::DataSourceTransactor > transactorPtr(
       outDataSource.getTransactor() );

      if( transactorPtr.get() == 0 )
      {
       return false;
      }

      // Creating a data set instance
      
      if( transactorPtr->dataSetExists( outDataSetName ) )
      {
        transactorPtr->dropDataSet( outDataSetName );
      }

      std::auto_ptr< te::da::DataSetType > dataSetTypePtr(
        new te::da::DataSetType( outDataSetName ) );
      dataSetTypePtr->add( rasterPropertyPtr.release() );

      try
      {
        transactorPtr->createDataSet( dataSetTypePtr.get(),
          std::map< std::string, std::string >() );
      }
      catch( ... )
      {
        return false;
      }
      
       if( ! transactorPtr->dataSetExists( outDataSetName ) ) return false;

      std::auto_ptr< te::da::DataSet > dataSetPtr( transactorPtr->getDataSet(
        outDataSetName, te::common::FORWARDONLY, true, te::common::RWAccess ) );

      if( dataSetPtr.get() == 0 )
      {
        return false;
      }

      // Creating a raster instance

      std::auto_ptr< te::rst::Raster > rasterPtr( dataSetPtr->getRaster( 0 ) );

      if( rasterPtr.get() )
      {
        outRasterHandler.reset( transactorPtr.release(), dataSetPtr.release(), rasterPtr.release() );
        return true;
      }
      else
      {
        return false;
      }
    }

    bool CreateNewMemRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      RasterHandler& outRasterHandler )
    {
      std::string dataSetName = std::string( "createNewMemRaster" ) +
        boost::lexical_cast< std::string >( &outRasterHandler );
        
      return CreateNewRaster( rasterGrid, bandsProperties, dataSetName, 
        "MEM", outRasterHandler );
    }

    bool CreateNewGdalRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      const std::string& fileName,
      RasterHandler& outRasterHandler )
    {
      std::auto_ptr< te::rst::Raster > outRasterPtr;
      if( CreateNewGdalRaster( rasterGrid, bandsProperties, fileName, outRasterPtr ) )
      {
        outRasterHandler.reset( outRasterPtr.release() ); 
        return true;
      }
      else
      {
        outRasterHandler.reset();
        return false;
      }
    }
    
    bool CreateNewGdalRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      const std::string& fileName,
      std::auto_ptr< te::rst::Raster >& outRasterPtr )
    {
      outRasterPtr.reset();
      
      std::map< std::string, std::string > rInfo;
      rInfo[ "URI" ] = fileName;
      
      outRasterPtr.reset( te::rst::RasterFactory::make(
        "GDAL",
        new te::rst::Grid( rasterGrid ),
        bandsProperties,
        rInfo,
        0,
        0 ) );
      
      if( outRasterPtr.get() )
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    
    bool TERPEXPORT Copy2DiskRaster( const te::rst::Raster& inputRaster,
      const std::string& fileName )
    {
      if( !(inputRaster.getAccessPolicy() & te::common::RAccess ) )
      {
        return false;
      };
      
      const unsigned int nBands = inputRaster.getNumberOfBands();
      const unsigned int nCols = inputRaster.getNumberOfColumns();
      const unsigned int nRows = inputRaster.getNumberOfRows();
      unsigned int bandIdx =0;
      unsigned int col = 0;
      unsigned int row = 0;
      
      std::vector< te::rst::BandProperty* > bandsProperties;
      for( bandIdx = 0 ; bandIdx < nBands ; ++bandIdx )
      {
        bandsProperties.push_back( new te::rst::BandProperty(
          *( inputRaster.getBand( bandIdx )->getProperty() ) ) );        
      }
      
      RasterHandler outRasterHandler;
      
      if( ! CreateNewGdalRaster( *( inputRaster.getGrid() ), bandsProperties,
        fileName, outRasterHandler ) )
      {
        return false;
      }
      
      double value = 0;
      
      for( bandIdx = 0 ; bandIdx < nBands ; ++bandIdx )
      {
        const te::rst::Band& inBand = *inputRaster.getBand( bandIdx );
        te::rst::Band& outBand = *outRasterHandler.getRasterPtr()->getBand( bandIdx );
        
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            inBand.getValue( col, row, value );
            outBand.setValue( col, row, value );
          }
        }
      }
      
      return true;
    }

    void Convert2DoublesVector( void* inputVector, const int inputVectorDataType,
      unsigned int inputVectorSize, double* outputVector )
    {
      switch( inputVectorDataType )
      {
        case te::dt::CHAR_TYPE :
        {
          char* vPtr = (char*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::BIT_TYPE :
        case te::dt::UCHAR_TYPE :
        {
          unsigned char* vPtr = (unsigned char*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::INT16_TYPE :
        {
          short int* vPtr = (short int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::CINT16_TYPE :
        {
          std::complex< short int >* vPtr = (std::complex< short int >*)
            inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ].real();
          break;
        }
        case te::dt::UINT16_TYPE :
        {
          unsigned short int* vPtr = (unsigned short int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::INT32_TYPE :
        {
          int* vPtr = (int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::CINT32_TYPE :
        {
          std::complex< int >* vPtr = (std::complex< int >*)
            inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ].real();
          break;
        }
        case te::dt::UINT32_TYPE :
        {
          unsigned int* vPtr = (unsigned int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::INT64_TYPE :
        {
          long int* vPtr = (long int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::UINT64_TYPE :
        {
          unsigned long int* vPtr = (unsigned long int*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::FLOAT_TYPE :
        {
          float* vPtr = (float*)inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ];
          break;
        }
        case te::dt::CFLOAT_TYPE :
        {
          std::complex< float >* vPtr = (std::complex< float >*)
            inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ].real();
          break;
        }
        case te::dt::DOUBLE_TYPE :
        {
          memcpy( outputVector, inputVector, inputVectorSize * sizeof( double ) );
          break;
        }
        case te::dt::CDOUBLE_TYPE :
        {
          std::complex< double >* vPtr = (std::complex< double >*)
            inputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            outputVector[ idx ] = (double)vPtr[ idx ].real();
          break;
        }
        default :
          throw te::rp::Exception( "Invalid data type" );
          break;
      };
    }

    void ConvertDoublesVector( double* inputVector,
      unsigned int inputVectorSize, const int outputVectorDataType,
      void* outputVector )
    {
      switch( outputVectorDataType )
      {
        case te::dt::CHAR_TYPE :
        {
          char* vPtr = (char*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (char)inputVector[ idx ];
          break;
        }
        case te::dt::BIT_TYPE :
        case te::dt::UCHAR_TYPE :
        {
          unsigned char* vPtr = (unsigned char*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (unsigned char)inputVector[ idx ];
          break;
        }
        case te::dt::INT16_TYPE :
        {
          short int* vPtr = (short int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (short int)inputVector[ idx ];
          break;
        }
        case te::dt::CINT16_TYPE :
        {
          std::complex< short int >* vPtr = (std::complex< short int >*)
            outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ]= ( (short int)inputVector[ idx ] );
          break;
        }
        case te::dt::UINT16_TYPE :
        {
          unsigned short int* vPtr = (unsigned short int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (unsigned short int)inputVector[ idx ];
          break;
        }
        case te::dt::INT32_TYPE :
        {
          int* vPtr = (int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (int)inputVector[ idx ];
          break;
        }
        case te::dt::CINT32_TYPE :
        {
          std::complex< int >* vPtr = (std::complex< int >*)
            outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = ( (int)inputVector[ idx ] );
          break;
        }
        case te::dt::UINT32_TYPE :
        {
          unsigned int* vPtr = (unsigned int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (unsigned int)inputVector[ idx ];
          break;
        }
        case te::dt::INT64_TYPE :
        {
          long int* vPtr = (long int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (long int)inputVector[ idx ];
          break;
        }
        case te::dt::UINT64_TYPE :
        {
          unsigned long int* vPtr = (unsigned long int*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (unsigned long int)inputVector[ idx ];
          break;
        }
        case te::dt::FLOAT_TYPE :
        {
          float* vPtr = (float*)outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = (float)inputVector[ idx ];
          break;
        }
        case te::dt::CFLOAT_TYPE :
        {
          std::complex< float >* vPtr = (std::complex< float >*)
            outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = ( (float)inputVector[ idx ] );
          break;
        }
        case te::dt::DOUBLE_TYPE :
        {
          memcpy( outputVector, inputVector, inputVectorSize * sizeof( double ) );
          break;
        }
        case te::dt::CDOUBLE_TYPE :
        {
          std::complex< double >* vPtr = (std::complex< double >*)
            outputVector;
          for( register unsigned int idx = 0 ; idx < inputVectorSize ; ++idx )
            vPtr[ idx ] = ( (double)inputVector[ idx ] );
          break;
        }
        default :
          throw te::rp::Exception( "Invalid data type" );
          break;
      };
    }
    
    std::vector<std::string> GetBandNames()
    {
      static std::vector<std::string> bandNames;

      bandNames.push_back("CBERS2_CCD_1_BLUE");
      bandNames.push_back("CBERS2_CCD_2_GREEN");
      bandNames.push_back("CBERS2_CCD_3_RED");
      bandNames.push_back("CBERS2_CCD_4_NIR");
      bandNames.push_back("CBERS2_CCD_5_PAN");

      bandNames.push_back("LANDSAT5_TM_1_BLUE");
      bandNames.push_back("LANDSAT5_TM_2_GREEN");
      bandNames.push_back("LANDSAT5_TM_3_RED");
      bandNames.push_back("LANDSAT5_TM_4_NIR");
      bandNames.push_back("LANDSAT5_TM_5_SWIR");
      bandNames.push_back("LANDSAT5_TM_6_THERMAL");
      bandNames.push_back("LANDSAT5_TM_7_MIR");

      bandNames.push_back("LANDSAT7_ETM+_1_BLUE");
      bandNames.push_back("LANDSAT7_ETM+_2_GREEN");
      bandNames.push_back("LANDSAT7_ETM+_3_RED");
      bandNames.push_back("LANDSAT7_ETM+_4_NIR");
      bandNames.push_back("LANDSAT7_ETM+_5_SWIR");
      bandNames.push_back("LANDSAT7_ETM+_6_THERMAL");
      bandNames.push_back("LANDSAT7_ETM+_7_MIR");
      bandNames.push_back("LANDSAT7_ETM+_8_PAN");

      bandNames.push_back("WV2_MUL_1_COASTAL");
      bandNames.push_back("WV2_MUL_2_BLUE");
      bandNames.push_back("WV2_MUL_3_GREEN");
      bandNames.push_back("WV2_MUL_4_YELLOW");
      bandNames.push_back("WV2_MUL_5_RED");
      bandNames.push_back("WV2_MUL_6_REDEDGE");
      bandNames.push_back("WV2_MUL_7_NIR1");
      bandNames.push_back("WV2_MUL_8_NIR2");
      bandNames.push_back("WV2_PAN");

      return bandNames;
    }

    std::pair<double, double> GetSpectralBandInfo(std::string bandName)
    {
      static std::map<std::string, std::pair<double, double> > BandInfo;

      BandInfo["CBERS2_CCD_1_BLUE"] = std::pair<double, double> (0.45, 0.52);
      BandInfo["CBERS2_CCD_2_GREEN"] = std::pair<double, double> (0.52, 0.59);
      BandInfo["CBERS2_CCD_3_RED"] = std::pair<double, double> (0.63, 0.69);
      BandInfo["CBERS2_CCD_4_NIR"] = std::pair<double, double> (0.77, 0.89);
      BandInfo["CBERS2_CCD_5_PAN"] = std::pair<double, double> (0.51, 0.73);

      BandInfo["LANDSAT5_TM_1_BLUE"] = std::pair<double, double> (0.45, 0.52);
      BandInfo["LANDSAT5_TM_2_GREEN"] = std::pair<double, double> (0.52, 0.60);
      BandInfo["LANDSAT5_TM_3_RED"] = std::pair<double, double> (0.63, 0.69);
      BandInfo["LANDSAT5_TM_4_NIR"] = std::pair<double, double> (0.76, 0.90);
      BandInfo["LANDSAT5_TM_5_SWIR"] = std::pair<double, double> (1.55, 1.75);
      BandInfo["LANDSAT5_TM_6_THERMAL"] = std::pair<double, double> (10.40, 12.50);
      BandInfo["LANDSAT5_TM_7_MIR"] = std::pair<double, double> (2.08, 2.35);

      BandInfo["LANDSAT7_ETM+_1_BLUE"] = std::pair<double, double> (0.45, 0.515);
      BandInfo["LANDSAT7_ETM+_2_GREEN"] = std::pair<double, double> (0.525, 0.605);
      BandInfo["LANDSAT7_ETM+_3_RED"] = std::pair<double, double> (0.63, 0.69);
      BandInfo["LANDSAT7_ETM+_4_NIR"] = std::pair<double, double> (0.75, 0.90);
      BandInfo["LANDSAT7_ETM+_5_SWIR"] = std::pair<double, double> (1.55, 1.75);
      BandInfo["LANDSAT7_ETM+_6_THERMAL"] = std::pair<double, double> (10.40, 12.5);
      BandInfo["LANDSAT7_ETM+_7_MIR"] = std::pair<double, double> (2.09, 2.35);
      BandInfo["LANDSAT7_ETM+_8_PAN"] = std::pair<double, double> (0.52, 0.90);

      BandInfo["WV2_MUL_1_COASTAL"] = std::pair<double, double> (0.4, 0.45);
      BandInfo["WV2_MUL_2_BLUE"] = std::pair<double, double> (0.45, 0.51);
      BandInfo["WV2_MUL_3_GREEN"] = std::pair<double, double> (0.51, 0.58);
      BandInfo["WV2_MUL_4_YELLOW"] = std::pair<double, double> (0.585, 0.625);
      BandInfo["WV2_MUL_5_RED"] = std::pair<double, double> (0.66, 0.69);
      BandInfo["WV2_MUL_6_REDEDGE"] = std::pair<double, double> (0.705, 0.745);
      BandInfo["WV2_MUL_7_NIR1"] = std::pair<double, double> (0.77, 0.895);
      BandInfo["WV2_MUL_8_NIR2"] = std::pair<double, double> (0.86, 0.104);
      BandInfo["WV2_PAN"] = std::pair<double, double> (0.45, 0.8);

    // needs more Band Info from other sensors/bands

      if (BandInfo.find(bandName) == BandInfo.end())
        return std::pair<double, double> (0.0, 1.0);

      return BandInfo[bandName];
    }

    double GetSpectralBandMin(std::string bandName)
    {
      return GetSpectralBandInfo(bandName).first;
    }

    double GetSpectralBandMax(std::string bandName)
    {
      return GetSpectralBandInfo(bandName).second;
    }

    std::pair<double, double> GetDigitalNumberBandInfo(std::string bandName)
    {
      static std::map<std::string, std::pair<double, double> > DNBandInfo;

      DNBandInfo["CBERS2_CCD_1_BLUE"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["CBERS2_CCD_2_GREEN"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["CBERS2_CCD_3_RED"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["CBERS2_CCD_4_NIR"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["CBERS2_CCD_5_PAN"] = std::pair<double, double> (0.0, 255.0);

      DNBandInfo["LANDSAT5_TM_1_BLUE"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT5_TM_2_GREEN"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT5_TM_3_RED"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT5_TM_4_NIR"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT5_TM_5_SWIR"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT5_TM_6_THERMAL"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT5_TM_7_MIR"] = std::pair<double, double> (0.0, 255.0);

      DNBandInfo["LANDSAT7_ETM+_1_BLUE"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT7_ETM+_2_GREEN"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT7_ETM+_3_RED"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT7_ETM+_4_NIR"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT7_ETM+_5_SWIR"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT7_ETM+_6_THERMAL"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT7_ETM+_7_MIR"] = std::pair<double, double> (0.0, 255.0);
      DNBandInfo["LANDSAT7_ETM+_8_PAN"] = std::pair<double, double> (0.0, 255.0);

      DNBandInfo["WV2_MUL_1_COASTAL"] = std::pair<double, double> (0.0, 2048.0);
      DNBandInfo["WV2_MUL_2_BLUE"] = std::pair<double, double> (0.0, 2048.0);
      DNBandInfo["WV2_MUL_3_GREEN"] = std::pair<double, double> (0.0, 2048.0);
      DNBandInfo["WV2_MUL_4_YELLOW"] = std::pair<double, double> (0.0, 2048.0);
      DNBandInfo["WV2_MUL_5_RED"] = std::pair<double, double> (0.0, 2048.0);
      DNBandInfo["WV2_MUL_6_REDEDGE"] = std::pair<double, double> (0.0, 2048.0);
      DNBandInfo["WV2_MUL_7_NIR1"] = std::pair<double, double> (0.0, 2048.0);
      DNBandInfo["WV2_MUL_7_NIR2"] = std::pair<double, double> (0.0, 2048.0);
      DNBandInfo["WV2_PAN"] = std::pair<double, double> (0.0, 2048.0);

    // needs more Band Info from other sensors/bands

      if (DNBandInfo.find(bandName) == DNBandInfo.end())
        return std::pair<double, double> (0.0, 255.0);

      return DNBandInfo[bandName];
    }

    double GetDigitalNumberBandMax(std::string bandName)
    {
      return GetDigitalNumberBandInfo(bandName).second;
    }

    bool NormalizeRaster(te::rst::Raster& inraster, double nmin, double nmax)
    {
      if (nmin > nmax)
        return false;

    // computing old maximuns and minimums for each band
      std::vector<double> omins;
      std::vector<double> omaxs;
      std::vector<unsigned int> bands;

      for (unsigned int b = 0; b < inraster.getNumberOfBands(); b++)
      {
        omins.push_back(inraster.getBand(b)->getMinValue().real());
        omaxs.push_back(inraster.getBand(b)->getMaxValue().real());

        bands.push_back(b);
      }

    // calculating amplitudes to avoid multiple subtractions
      double value;
      const double namplitude = nmax - nmin;
      std::vector<double> oamplitude;
      for (unsigned int b = 0; b < inraster.getNumberOfBands(); b++)
        oamplitude.push_back(omaxs[b] - omins[b]);

    // iterating over raster to normalize pixel values
      te::rst::RasterIterator<double> it = te::rst::RasterIterator<double>::begin(&inraster, bands);
      te::rst::RasterIterator<double> itend = te::rst::RasterIterator<double>::end(&inraster, bands);

      while (it != itend)
      {
        for (unsigned int b = 0; b < inraster.getNumberOfBands(); b++)
        {
          value = ((*it)[b] - omins[b]) * namplitude / oamplitude[b] + nmin;

          inraster.setValue(it.getColumn(), it.getRow(), value, b);
        }

        ++it;
      }

      return true;
    }

    std::vector<te::gm::Point*> GetRandomPointsInRaster(const te::rst::Raster& inputRaster, unsigned int numberOfPoints)
    {
      std::vector<te::gm::Point*> randomPoints;
      double randX;
      double randY;

      boost::random::mt19937 generator((boost::random::mt19937::result_type) time(0));
      boost::random::uniform_int_distribution<> random_rows(0, inputRaster.getNumberOfRows() - 1);
      boost::random::uniform_int_distribution<> random_columns(0, inputRaster.getNumberOfColumns() - 1);

      for (unsigned int p = 0; p < numberOfPoints; p++)
      {
        inputRaster.getGrid()->gridToGeo(random_columns(generator), random_rows(generator), randX, randY);
        randomPoints.push_back(new te::gm::Point(randX, randY, inputRaster.getSRID()));
      }

      return randomPoints;
    }   
    
    bool ConvertRGB2IHS( const te::rst::Raster& inputRGBRaster, 
      const unsigned int redBandIdx, const unsigned int greenBandIdx,
      const unsigned int blueBandIdx, const double rgbRangeMin, 
      const double rgbRangeMax, te::rst::Raster& outputIHSRaster ) 
    {
      if( ( inputRGBRaster.getAccessPolicy() & te::common::RAccess ) == 0 )
        return false;
      if( redBandIdx > inputRGBRaster.getNumberOfBands() ) return false;
      if( greenBandIdx > inputRGBRaster.getNumberOfBands() ) return false;
      if( blueBandIdx > inputRGBRaster.getNumberOfBands() ) return false;
      if( ( outputIHSRaster.getAccessPolicy() & te::common::WAccess ) == 0 )
        return false;      
      if( outputIHSRaster.getNumberOfBands() < 3 ) return false;
      if( inputRGBRaster.getNumberOfColumns() != outputIHSRaster.getNumberOfColumns() )
        return false;
      if( inputRGBRaster.getNumberOfRows() != outputIHSRaster.getNumberOfRows() )
        return false;      
      
      const te::rst::Band& redBand = *inputRGBRaster.getBand( redBandIdx );
      const te::rst::Band& greenBand = *inputRGBRaster.getBand( greenBandIdx );
      const te::rst::Band& blueBand = *inputRGBRaster.getBand( blueBandIdx );      
      
      const unsigned int outNCols = outputIHSRaster.getNumberOfColumns();
      const unsigned int outNRows = outputIHSRaster.getNumberOfRows();
      const double redNoData = redBand.getProperty()->m_noDataValue;
      const double greenNoData = greenBand.getProperty()->m_noDataValue;
      const double blueNoData = inputRGBRaster.getBand( 
        blueBandIdx )->getProperty()->m_noDataValue;
      
      unsigned int outRow = 0;
      unsigned int outCol = 0;
      double red = 0;
      double green = 0;
      double blue = 0;
      double teta = 0;
      double redNorm = 0, greenNorm = 0, blueNorm = 0;
      double rMinusG = 0, rMinusB = 0;
      double rgbSum = 0;
      double cosValue = 0;
      const double twoPi = 2.0 * ((double)M_PI);
      const double pi2 = ((double)M_PI) / 2.0;   
      const double rgbNormFac = ( rgbRangeMax == rgbRangeMin ) ? 0.0 :
        ( 1.0 / ( rgbRangeMax - rgbRangeMin ) );
      double intensity = 0;
      double hue = 0;
      double saturation = 0;
      
      for( outRow = 0 ; outRow < outNRows ; ++outRow )
      {
        for( outCol = 0 ; outCol < outNCols ; ++outCol )
        {
          redBand.getValue( outCol, outRow, red );
          greenBand.getValue( outCol, outRow, green );
          blueBand.getValue( outCol, outRow, blue );
          
          if( ( red == redNoData ) || ( green == greenNoData ) ||
            ( blue == blueNoData ) )
          {
            intensity = 0.0;
            hue = 0.0;
            saturation = 0.0;
          }            
          else
          {
            if( ( red == green ) && ( green == blue ) ) 
            { // Gray scale case
              // From Wikipedia:
              // h = 0 is used for grays though the hue has no geometric 
              // meaning there, where the saturation s = 0. Similarly, 
              // the choice of 0 as the value for s when l is equal to 0 or 1 
              // is arbitrary.        
              
              hue = 0.0;
              saturation = 0.0;
              intensity = ( red * rgbNormFac ); // or green or blue since they all are the same.
            }
            else
            { // Color case
              redNorm = ( red - rgbRangeMin ) * rgbNormFac;
              greenNorm = ( green - rgbRangeMin ) * rgbNormFac;
              blueNorm = ( blue - rgbRangeMin ) * rgbNormFac;
              
              rMinusG = redNorm - greenNorm;
              rMinusB = redNorm - blueNorm;
              
              cosValue = std::sqrt( ( rMinusG * rMinusG ) + ( rMinusB * 
                ( greenNorm - blueNorm ) ) );
                
              if( cosValue == 0.0 )
              {
                teta = pi2;
              }
              else
              {
                cosValue =  ( 0.5 * ( rMinusG + rMinusB )  ) /
                  cosValue;
                teta = std::acos( cosValue );  
              }
                
              assert( ( cosValue >= (-1.0) ) && ( cosValue <= (1.0) ) );
                
              if( blueNorm > greenNorm )
              {
                hue = ( twoPi - teta );
              }
              else
              {
                hue = teta;
              }
                
              rgbSum = ( redNorm + greenNorm + blueNorm );
              
              saturation = ( 1.0 - ( 3 * std::min( std::min( redNorm, greenNorm ), blueNorm ) /
                rgbSum ) );
                
              intensity = ( rgbSum / 3.0 );            
            }
          }
          
          outputIHSRaster.setValue( outCol, outRow, intensity, 0 );
          outputIHSRaster.setValue( outCol, outRow, hue, 1 );
          outputIHSRaster.setValue( outCol, outRow, saturation, 2 );          
        }
      }      
      
      return true;
    }
    
    bool ConvertIHS2RGB( const te::rst::Raster& inputIHSRaster, 
      const unsigned int intensityBandIdx, const unsigned int hueBandIdx,
      const unsigned int saturationBandIdx, const double rgbRangeMin, 
      const double rgbRangeMax, te::rst::Raster& outputRGBRaster )
    {
      if( ( inputIHSRaster.getAccessPolicy() & te::common::RAccess ) == 0 )
        return false;
      if( intensityBandIdx > inputIHSRaster.getNumberOfBands() ) return false;
      if( hueBandIdx > inputIHSRaster.getNumberOfBands() ) return false;
      if( saturationBandIdx > inputIHSRaster.getNumberOfBands() ) return false;
      if( ( outputRGBRaster.getAccessPolicy() & te::common::WAccess ) == 0 )
        return false;      
      if( outputRGBRaster.getNumberOfBands() < 3 ) return false;
      if( inputIHSRaster.getNumberOfColumns() != outputRGBRaster.getNumberOfColumns() )
        return false;
      if( inputIHSRaster.getNumberOfRows() != outputRGBRaster.getNumberOfRows() )
        return false;      
      
      const unsigned int nCols = outputRGBRaster.getNumberOfColumns();
      const unsigned int nRows = outputRGBRaster.getNumberOfRows();
      const double intensityNoData = inputIHSRaster.getBand( 
        intensityBandIdx )->getProperty()->m_noDataValue;
      const double hueNoData = inputIHSRaster.getBand( 
        hueBandIdx )->getProperty()->m_noDataValue;
      const double saturationNoData = inputIHSRaster.getBand( 
        saturationBandIdx )->getProperty()->m_noDataValue;
        
      const double rgbNormFac = ( rgbRangeMax == rgbRangeMin ) ? 0.0 :
        ( rgbRangeMax - rgbRangeMin ); 
      const double pi3 = M_PI / 3.0; // 60
      const double twoPi3 = 2.0 * M_PI / 3.0; // 120
      const double fourPi3 = 4.0 * M_PI / 3.0; // 240        
      unsigned int row = 0;
      unsigned int col = 0;  
      double hue = 0;
      double lig = 0;
      double sat = 0;
      double red = 0;
      double green = 0;
      double blue = 0;  
      const te::rst::Band& intensityBand = *inputIHSRaster.getBand( intensityBandIdx );
      const te::rst::Band& hueBand = *inputIHSRaster.getBand( hueBandIdx );
      const te::rst::Band& saturationBand = *inputIHSRaster.getBand( saturationBandIdx );      
      te::rst::Band& redBand = *outputRGBRaster.getBand( 0 );
      te::rst::Band& greenBand = *outputRGBRaster.getBand( 1 );
      te::rst::Band& blueBand = *outputRGBRaster.getBand( 2 );
      
      for( row = 0 ; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {     
          intensityBand.getValue( col, row, lig );
          hueBand.getValue( col, row, hue );
          saturationBand.getValue( col, row, sat );
          
          if( ( lig == intensityNoData ) || ( hue == hueNoData ) || 
            ( sat == saturationNoData ) )
          {
            red = green = blue = 0;
          }
          else
          {
            if( ( hue == 0.0 ) && ( sat == 0.0 ) )
            { // Gray scale case
              red = green = blue = ( lig * rgbNormFac );
            }
            else
            { // color case
              /* Hue inside RG sector */
              if( hue < twoPi3 )
              {
                blue = lig * ( 1.0 - sat );
                red = lig * ( 1.0 + ( sat * std::cos( hue ) / 
                  std::cos( pi3 - hue ) ) );
                green = ( 3.0 * lig ) - ( red + blue );
              }
              else if( hue < fourPi3 )
              { /* Hue inside GB sector */
              
                hue -= twoPi3;
                
                red = lig * ( 1.0 - sat );
                green = lig * ( 1.0 + ( sat * std::cos( hue ) / 
                  std::cos( pi3 - hue ) ) );
                blue = ( 3.0 * lig ) - ( red + green );
              }
              else
              { /* Hue inside BR sector */
              
                hue -= fourPi3;
                
                green = lig * ( 1.0 - sat );
                blue = lig * ( 1.0 + ( sat * std::cos( hue ) / 
                  std::cos( pi3 - hue ) ) );
                red = ( 3.0 * lig ) - ( green + blue );
              }
              
              red = ( red * rgbNormFac ) + rgbRangeMin;
              green = ( green * rgbNormFac ) + rgbRangeMin;
              blue = ( blue * rgbNormFac ) + rgbRangeMin;
            }
          }
          
          red = MIN( red, rgbRangeMax );
          green = MIN( green, rgbRangeMax );
          blue = MIN( blue, rgbRangeMax );
          
          red = MAX( red, rgbRangeMin );
          green = MAX( green, rgbRangeMin );
          blue = MAX( blue, rgbRangeMin );           
          
          redBand.setValue( col, row, red );
          greenBand.setValue( col, row, green );
          blueBand.setValue( col, row, blue );
        }
      }        
      
      return true;
    }
    
    void GetMeanValueThread( GetMeanValueThreadParams* paramsPtr )
    {
      paramsPtr->m_mutexPtr->lock();
      const unsigned int blockElementsNumber = paramsPtr->m_inputBandPtr->getProperty()->m_blkh *
        paramsPtr->m_inputBandPtr->getProperty()->m_blkw;
      const int blockDataType = paramsPtr->m_inputBandPtr->getProperty()->getType();
      const int blockSizeBytes = paramsPtr->m_inputBandPtr->getBlockSize();
      const double noDataValue = paramsPtr->m_inputBandPtr->getProperty()->m_noDataValue;
      paramsPtr->m_mutexPtr->unlock();

      boost::scoped_array< unsigned char > blockBuffer( new unsigned char[ blockSizeBytes ] );
      boost::scoped_array< double > doubleBuffer( new double[ blockElementsNumber ] );
      unsigned blkX = 0;
      unsigned int elementIdx = 0;
      double mean = 0;
      double meanElementsNumber = 0;
      
      for( unsigned blkY = 0 ; blkY < paramsPtr->m_rasterBlocksStatusPtr->getLinesNumber() ;
        ++blkY )
      {
        for( blkX = 0 ; blkX < paramsPtr->m_rasterBlocksStatusPtr->getColumnsNumber() ;
          ++blkX )
        {
          paramsPtr->m_mutexPtr->lock();
          
          if( paramsPtr->m_rasterBlocksStatusPtr->operator()( blkY, blkX ) )
          {
            paramsPtr->m_mutexPtr->unlock();
          }
          else
          {
            paramsPtr->m_rasterBlocksStatusPtr->operator()( blkY, blkX ) = true;
            
            paramsPtr->m_inputBandPtr->read( blkX, blkY, blockBuffer.get() );
            
            paramsPtr->m_mutexPtr->unlock();
            
            Convert2DoublesVector( blockBuffer.get(), blockDataType, blockElementsNumber,
              doubleBuffer.get() );
              
            for( elementIdx = 0 ; elementIdx < blockElementsNumber ; ++elementIdx )
            {
              if( noDataValue != doubleBuffer[ elementIdx ] )
              {
                mean = 
                  ( 
                    ( 
                      mean 
                      * 
                      meanElementsNumber 
                    ) 
                    + 
                    doubleBuffer[ elementIdx ] 
                  ) 
                  /
                  ( 
                    meanElementsNumber 
                    + 
                    1.0
                  );
                  
                meanElementsNumber = meanElementsNumber + 1.0;
              }
            }
          }
        }
      }
      
      paramsPtr->m_mutexPtr->lock();
      ( *(paramsPtr->m_meanValuePtr) ) = 
        ( 
          ( 
            ( *(paramsPtr->m_meanValuePtr) ) 
            * 
            ( *( paramsPtr->m_pixelsNumberValuePtr ) ) 
          ) 
          + 
          ( 
            mean 
            * 
            meanElementsNumber 
          ) 
        ) 
        /
        ( 
          ( *( paramsPtr->m_pixelsNumberValuePtr ) ) 
          + 
          meanElementsNumber 
        );
      ( *( paramsPtr->m_pixelsNumberValuePtr ) ) += meanElementsNumber;
      paramsPtr->m_mutexPtr->unlock();      
    }
    
    bool GetMeanValue( const te::rst::Band& band, 
      const unsigned int maxThreads, 
      double& meanValue )
    {
      Matrix< bool > rasterBlocksStatus;
      if( ! rasterBlocksStatus.reset( band.getProperty()->m_nblocksy,
        band.getProperty()->m_nblocksx, Matrix< bool >::RAMMemPol ) )
      {
        return false;
      }
      for( unsigned int row = 0 ; row < rasterBlocksStatus.getLinesNumber() ;
        ++row )
      {
        for( unsigned int col = 0 ; col < rasterBlocksStatus.getColumnsNumber() ;
          ++col )
        {
          rasterBlocksStatus( row, col ) = false;
        }
      }
      
      meanValue = 0.0;
      
      double pixelsNumber = 0.0;
      
      boost::mutex mutex;
      
      GetMeanValueThreadParams params;
      params.m_inputBandPtr = &band;
      params.m_rasterBlocksStatusPtr = &rasterBlocksStatus;
      params.m_meanValuePtr = &meanValue;
      params.m_pixelsNumberValuePtr = &pixelsNumber;
      params.m_returnStatus = true;
      params.m_mutexPtr = &mutex;
      
      if( maxThreads == 1 )
      {
        GetMeanValueThread( &params );
      }
      else
      {
        unsigned int threadsNumber = maxThreads ? maxThreads : te::common::GetPhysProcNumber();
        
        boost::thread_group threads;
          
        for( unsigned int threadIdx = 0 ; threadIdx < threadsNumber ;
          ++threadIdx )
        {
          threads.add_thread( new boost::thread( GetMeanValueThread, 
             &params ) );
        };  
        
        threads.join_all();
      }
      
      return params.m_returnStatus;
    }
    
    void GetStdDevValueThread( GetStdDevValueThreadParams* paramsPtr )
    {
      paramsPtr->m_mutexPtr->lock();
      const unsigned int blockElementsNumber = paramsPtr->m_inputBandPtr->getProperty()->m_blkh *
        paramsPtr->m_inputBandPtr->getProperty()->m_blkw;
      const int blockDataType = paramsPtr->m_inputBandPtr->getProperty()->getType();
      const int blockSizeBytes = paramsPtr->m_inputBandPtr->getBlockSize();
      const double noDataValue = paramsPtr->m_inputBandPtr->getProperty()->m_noDataValue;
      const double& meanValue = paramsPtr->m_meanValue;
      paramsPtr->m_mutexPtr->unlock();

      boost::scoped_array< unsigned char > blockBuffer( new unsigned char[ blockSizeBytes ] );
      boost::scoped_array< double > doubleBuffer( new double[ blockElementsNumber ] );
      unsigned blkX = 0;
      unsigned int elementIdx = 0;
      double diff = 0;
      double squaresDifsSum = 0;
      double elementsNumber = 0;
      
      
      for( unsigned blkY = 0 ; blkY < paramsPtr->m_rasterBlocksStatusPtr->getLinesNumber() ;
        ++blkY )
      {
        for( blkX = 0 ; blkX < paramsPtr->m_rasterBlocksStatusPtr->getColumnsNumber() ;
          ++blkX )
        {
          paramsPtr->m_mutexPtr->lock();
          
          if( paramsPtr->m_rasterBlocksStatusPtr->operator()( blkY, blkX ) )
          {
            paramsPtr->m_mutexPtr->unlock();
          }
          else
          {
            paramsPtr->m_rasterBlocksStatusPtr->operator()( blkY, blkX ) = true;
            
            paramsPtr->m_inputBandPtr->read( blkX, blkY, blockBuffer.get() );
            
            paramsPtr->m_mutexPtr->unlock();
            
            Convert2DoublesVector( blockBuffer.get(), blockDataType, blockElementsNumber,
              doubleBuffer.get() );
              
            for( elementIdx = 0 ; elementIdx < blockElementsNumber ; ++elementIdx )
            {
              if( noDataValue != doubleBuffer[ elementIdx ] )
              {
                diff = doubleBuffer[ elementIdx ] - meanValue;
                diff *= diff;
                squaresDifsSum += diff;
                elementsNumber = elementsNumber + 1.0;
              }
            }
          }
        }
      }
      
      paramsPtr->m_mutexPtr->lock();
      ( *(paramsPtr->m_stdDevValuePtr) ) = 
        ( 
          ( *(paramsPtr->m_stdDevValuePtr) ) 
          + 
          squaresDifsSum 
        );
      ( *( paramsPtr->m_pixelsNumberValuePtr ) ) += elementsNumber;
      paramsPtr->m_mutexPtr->unlock();      
    }
    
    bool GetStdDevValue( const te::rst::Band& band, 
      const unsigned int maxThreads, double const * const meanValuePtr, 
      double& stdDevValue )
    {
      double mean = 0;
      if( meanValuePtr )
      {
        mean = (*meanValuePtr);
      }
      else
      {
        if( ! GetMeanValue( band, maxThreads, mean ) )
        {
          return false;
        }
      }
            
      Matrix< bool > rasterBlocksStatus;
      if( ! rasterBlocksStatus.reset( band.getProperty()->m_nblocksy,
        band.getProperty()->m_nblocksx, Matrix< bool >::RAMMemPol ) )
      {
        return false;
      }
      for( unsigned int row = 0 ; row < rasterBlocksStatus.getLinesNumber() ;
        ++row )
      {
        for( unsigned int col = 0 ; col < rasterBlocksStatus.getColumnsNumber() ;
          ++col )
        {
          rasterBlocksStatus( row, col ) = false;
        }
      }
      
      stdDevValue = 0.0;
      
      double pixelsNumber = 0.0;
      
      boost::mutex mutex;
      
      GetStdDevValueThreadParams params;
      params.m_inputBandPtr = &band;
      params.m_rasterBlocksStatusPtr = &rasterBlocksStatus;
      params.m_stdDevValuePtr = &stdDevValue;
      params.m_pixelsNumberValuePtr = &pixelsNumber;
      params.m_meanValue = mean;
      params.m_returnStatus = true;
      params.m_mutexPtr = &mutex;
      
      if( maxThreads == 1 )
      {
        GetStdDevValueThread( &params );
      }
      else
      {
        unsigned int threadsNumber = maxThreads ? maxThreads : te::common::GetPhysProcNumber();
        
        boost::thread_group threads;
          
        for( unsigned int threadIdx = 0 ; threadIdx < threadsNumber ;
          ++threadIdx )
        {
          threads.add_thread( new boost::thread( GetStdDevValueThread, 
             &params ) );
        };  
        
        threads.join_all();
      }
      
      stdDevValue = std::sqrt( stdDevValue / pixelsNumber );
      
      return params.m_returnStatus;
    }    
    
    void GetCovarianceValueThread( GetCovarianceValueThreadParams* paramsPtr )
    {
      paramsPtr->m_mutexPtr->lock();
      const unsigned int blockElementsNumber = paramsPtr->m_inputBand1Ptr->getProperty()->m_blkh *
        paramsPtr->m_inputBand1Ptr->getProperty()->m_blkw;
      const int blockDataType1 = paramsPtr->m_inputBand1Ptr->getProperty()->getType();
      const int blockDataType2 = paramsPtr->m_inputBand2Ptr->getProperty()->getType();
      const int blockSizeBytes1 = paramsPtr->m_inputBand1Ptr->getBlockSize();
      const int blockSizeBytes2 = paramsPtr->m_inputBand2Ptr->getBlockSize();
      const double noDataValue1 = paramsPtr->m_inputBand1Ptr->getProperty()->m_noDataValue;
      const double noDataValue2 = paramsPtr->m_inputBand2Ptr->getProperty()->m_noDataValue;
      paramsPtr->m_mutexPtr->unlock();

      boost::scoped_array< unsigned char > blockBuffer1( new unsigned char[ blockSizeBytes1 ] );
      boost::scoped_array< unsigned char > blockBuffer2( new unsigned char[ blockSizeBytes2 ] );
      boost::scoped_array< double > doubleBuffer1( new double[ blockElementsNumber ] );
      boost::scoped_array< double > doubleBuffer2( new double[ blockElementsNumber ] );
      unsigned blkX = 0;
      unsigned int elementIdx = 0;
      double covariance = 0;
      double elementsNumber = 0;
      double diff1 = 0;
      double diff2 = 0;
      
      for( unsigned blkY = 0 ; blkY < paramsPtr->m_rasterBlocksStatusPtr->getLinesNumber() ;
        ++blkY )
      {
        for( blkX = 0 ; blkX < paramsPtr->m_rasterBlocksStatusPtr->getColumnsNumber() ;
          ++blkX )
        {
          paramsPtr->m_mutexPtr->lock();
          
          if( paramsPtr->m_rasterBlocksStatusPtr->operator()( blkY, blkX ) )
          {
            paramsPtr->m_mutexPtr->unlock();
          }
          else
          {
            paramsPtr->m_rasterBlocksStatusPtr->operator()( blkY, blkX ) = true;
            
            paramsPtr->m_inputBand1Ptr->read( blkX, blkY, blockBuffer1.get() );
            paramsPtr->m_inputBand2Ptr->read( blkX, blkY, blockBuffer2.get() );
            
            paramsPtr->m_mutexPtr->unlock();
            
            Convert2DoublesVector( blockBuffer1.get(), blockDataType1, blockElementsNumber,
              doubleBuffer1.get() );
            Convert2DoublesVector( blockBuffer2.get(), blockDataType2, blockElementsNumber,
              doubleBuffer2.get() );
              
            for( elementIdx = 0 ; elementIdx < blockElementsNumber ; ++elementIdx )
            {
              if( ( noDataValue1 != doubleBuffer1[ elementIdx ] ) &&
                ( noDataValue2 != doubleBuffer2[ elementIdx ] ) )
              {
                diff1 = doubleBuffer1[ elementIdx ] - paramsPtr->m_mean1Value;
                diff2 = doubleBuffer2[ elementIdx ] - paramsPtr->m_mean2Value;
                
                covariance += ( diff1 * diff2 );
                  
                elementsNumber = elementsNumber + 1.0;
              }
            }
          }
        }
      }
      
      paramsPtr->m_mutexPtr->lock();
      ( *(paramsPtr->m_covarianceValuePtr) ) += covariance;
      ( *( paramsPtr->m_pixelsNumberValuePtr ) ) += elementsNumber;
      paramsPtr->m_mutexPtr->unlock();      
    }    
    
    bool GetCovarianceValue( const te::rst::Band& band1, 
      const te::rst::Band& band2, const unsigned int maxThreads, 
      double const * const mean1ValuePtr, double const * const mean2ValuePtr,  
      double& covarianceValue )
    {
      if( ( band1.getProperty()->m_blkw * band1.getProperty()->m_nblocksx ) !=
        ( band2.getProperty()->m_blkw * band2.getProperty()->m_nblocksx ) )
      {
        return false;
      }
      
      if( ( band1.getProperty()->m_blkh * band1.getProperty()->m_nblocksy ) !=
        ( band2.getProperty()->m_blkh * band2.getProperty()->m_nblocksy ) )
      {
        return false;
      }      
      
      double mean1 = 0;
      if( mean1ValuePtr )
      {
        mean1 = (*mean1ValuePtr);
      }
      else
      {
        if( ! GetMeanValue( band1, maxThreads, mean1 ) )
        {
          return false;
        }
      }
      
      double mean2 = 0;
      if( mean2ValuePtr )
      {
        mean2 = (*mean2ValuePtr);
      }
      else
      {
        if( ! GetMeanValue( band2, maxThreads, mean2 ) )
        {
          return false;
        }
      }      
     
      if( ( band1.getProperty()->m_blkw == band2.getProperty()->m_blkw )
        && ( band1.getProperty()->m_blkh == band2.getProperty()->m_blkh )
        && ( band1.getProperty()->m_nblocksx == band2.getProperty()->m_nblocksx )
        && ( band1.getProperty()->m_nblocksy == band2.getProperty()->m_nblocksy ) )
      {
        Matrix< bool > rasterBlocksStatus;
        if( ! rasterBlocksStatus.reset( band1.getProperty()->m_nblocksy,
          band1.getProperty()->m_nblocksx, Matrix< bool >::RAMMemPol ) )
        {
          return false;
        }
        for( unsigned int row = 0 ; row < rasterBlocksStatus.getLinesNumber() ;
          ++row )
        {
          for( unsigned int col = 0 ; col < rasterBlocksStatus.getColumnsNumber() ;
            ++col )
          {
            rasterBlocksStatus( row, col ) = false;
          }
        }
              
        covarianceValue = 0.0;
        
        double pixelsNumber = 0.0;
        
        boost::mutex mutex;
        
        GetCovarianceValueThreadParams params;
        params.m_inputBand1Ptr = &band1;
        params.m_inputBand2Ptr = &band2;
        params.m_rasterBlocksStatusPtr = &rasterBlocksStatus;
        params.m_covarianceValuePtr = &covarianceValue;
        params.m_pixelsNumberValuePtr = &pixelsNumber;
        params.m_returnStatus = true;
        params.m_mutexPtr = &mutex;
        params.m_mean1Value = mean1;
        params.m_mean2Value = mean2;
        
        if( maxThreads == 1 )
        {
          GetCovarianceValueThread( &params );
        }
        else
        {
          unsigned int threadsNumber = maxThreads ? maxThreads : te::common::GetPhysProcNumber();
          
          boost::thread_group threads;
            
          for( unsigned int threadIdx = 0 ; threadIdx < threadsNumber ;
            ++threadIdx )
          {
            threads.add_thread( new boost::thread( GetCovarianceValueThread, 
               &params ) );
          };  
          
          threads.join_all();
        }
        
        if( pixelsNumber != 0.0 )
        {
          covarianceValue /= pixelsNumber;
        }
        
        return params.m_returnStatus;
      }
      else
      {
        const unsigned int nCols = band1.getProperty()->m_blkw * band1.getProperty()->m_nblocksx;
        const unsigned int nRows = band1.getProperty()->m_blkh * band1.getProperty()->m_nblocksy;
        double value1 = 0;
        double value2 = 0;
        const double noDataValue1 = band1.getProperty()->m_noDataValue;
        const double noDataValue2 = band2.getProperty()->m_noDataValue;    
        unsigned int col = 0;
        double pixelsNumber = 0.0;
        
        covarianceValue = 0;
        
        for( unsigned int row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            band1.getValue( col, row, value1 );
            band2.getValue( col, row, value2 );
            
            if( ( noDataValue1 != value1 ) &&
              ( noDataValue2 != value2 ) )
            {
              covarianceValue += ( ( value1 - mean1 ) * ( value2 - mean2 ) );
                
              pixelsNumber = pixelsNumber + 1.0;
            }            
          }
        }
        
        if( pixelsNumber != 0.0 )
        {
          covarianceValue /= pixelsNumber;
        }
        
        return true;
      }
    }    
    
    bool DirectPrincipalComponents( 
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,
      boost::numeric::ublas::matrix< double >& pcaMatrix,
      te::rst::Raster& pcaRaster,
      const std::vector< unsigned int >& pcaRasterBands,
      const unsigned int maxThreads )
    {
      if( ( inputRaster.getAccessPolicy() & te::common::RAccess ) == 0 )
      {
        return false;
      }
      if( ( pcaRaster.getAccessPolicy() & te::common::WAccess ) == 0 )
      {
        return false;
      }   
      if( inputRaster.getNumberOfBands() != pcaRaster.getNumberOfBands() )
      {
        return false;
      }
      if( pcaRaster.getNumberOfBands() != inputRasterBands.size() )
      {
        return false;
      }
      if( inputRaster.getNumberOfRows() != pcaRaster.getNumberOfRows() )
      {
        return false;
      }
      if( inputRaster.getNumberOfColumns() != pcaRaster.getNumberOfColumns() )
      {
        return false;
      }    
      
      // Covariance matrix
      
      boost::numeric::ublas::matrix< double > covarianceMatrix;
      
      covarianceMatrix.resize( inputRasterBands.size(), inputRasterBands.size() );
      
      for( unsigned int covMatrixIdx1 = 0 ; covMatrixIdx1 < inputRasterBands.size() ;
        ++covMatrixIdx1 )
      {
        if( inputRasterBands[ covMatrixIdx1 ] >= inputRaster.getNumberOfBands() )
        {
          return false;
        }
        
        for( unsigned int covMatrixIdx2 = 0 ; covMatrixIdx2 < inputRasterBands.size() ;
        ++covMatrixIdx2 )
        {
          if( inputRasterBands[ covMatrixIdx2 ] >= inputRaster.getNumberOfBands() )
          {
            return false;
          }    
          
          if( covMatrixIdx1 > covMatrixIdx2 )
          {
            covarianceMatrix( covMatrixIdx1, covMatrixIdx2 ) =
              covarianceMatrix( covMatrixIdx2, covMatrixIdx1 );
          }
          else
          {
            if( ! GetCovarianceValue( *( inputRaster.getBand( inputRasterBands[ covMatrixIdx1 ] ) ),
               *( inputRaster.getBand( inputRasterBands[ covMatrixIdx2 ] ) ),
               maxThreads, 0,  0, covarianceMatrix( covMatrixIdx1, covMatrixIdx2 ) ) )
            {
              return false;
            }
          }
        }
      }
      
//std::cout << std::endl << "Covariance matrix:" << covarianceMatrix << std::endl;      
      
      // Eigen stuff
      
      boost::numeric::ublas::matrix< double > eigenValues;
      boost::numeric::ublas::matrix< double > eigenVectors;
      
      if( ! te::common::EigenVectors( covarianceMatrix, eigenVectors, eigenValues ) )
      {
        return false;
      }
      
      pcaMatrix = boost::numeric::ublas::trans( eigenVectors );
      
      return RemapValues( inputRaster, inputRasterBands, pcaMatrix, pcaRaster, 
        pcaRasterBands, maxThreads );
    }
    
    bool InversePrincipalComponents( 
      const te::rst::Raster& pcaRaster,
      const boost::numeric::ublas::matrix< double >& pcaMatrix,
      te::rst::Raster& outputRaster,
      const std::vector< unsigned int >& outputRasterBands,
      const unsigned int maxThreads )    
    {
      boost::numeric::ublas::matrix< double > inversePcaMatrix;
      if( ! te::common::GetInverseMatrix( pcaMatrix, inversePcaMatrix ) )
      {
        return false;
      }
      
      std::vector< unsigned int > inputRasterBands;
      
      for( unsigned int bandIdx = 0 ; bandIdx < pcaRaster.getNumberOfBands() ;
        ++bandIdx )
      {
        inputRasterBands.push_back( bandIdx );
      }
      
      return RemapValues( pcaRaster, inputRasterBands, inversePcaMatrix, 
        outputRaster, outputRasterBands, maxThreads );
    }
    
    void RemapValuesThread( RemapValuesThreadParams* paramsPtr )
    {
      paramsPtr->m_mutexPtr->lock();
      
      assert( paramsPtr->m_inputRasterBandsPtr->operator[]( 0 ) < 
        paramsPtr->m_inputRasterPtr->getNumberOfBands() );
      const unsigned int blockElementsNumber = paramsPtr->m_inputRasterPtr->getBand( 
        paramsPtr->m_inputRasterBandsPtr->operator[]( 0 ) )->getProperty()->m_blkh *
        paramsPtr->m_inputRasterPtr->getBand( paramsPtr->m_inputRasterBandsPtr->operator[]( 0 ) )->getProperty()->m_blkw;
      
      const std::vector< unsigned int >  inputRasterBands = *( paramsPtr->m_inputRasterBandsPtr );
      const unsigned int  inputRasterBandsSize = (unsigned int)inputRasterBands.size();
      const std::vector< unsigned int >  outputRasterBands = *( paramsPtr->m_inputRasterBandsPtr );
      assert( inputRasterBandsSize == paramsPtr->m_outputRasterPtr->getNumberOfBands() );
      assert( inputRasterBandsSize == outputRasterBands.size() );
      
      unsigned int maxBlocksSizesBytes = 0;
      std::vector< double > inputBandsNoDataValues( inputRasterBandsSize, 0.0 );
      std::vector< double > outputBandsNoDataValues( inputRasterBandsSize, 0.0 );
      std::vector< boost::shared_array< double > > inDoubleBuffersHandlers( inputRasterBandsSize );
      std::vector< boost::shared_array< double > > outDoubleBuffersHandlers( inputRasterBandsSize );
      unsigned int inputRasterBandsIdx = 0;
      boost::numeric::ublas::matrix< double > remapMatrix = *( paramsPtr->m_remapMatrixPtr );
      std::vector< double > outputMinValue( inputRasterBandsSize );
      std::vector< double > outputMaxValue( inputRasterBandsSize );        
      boost::shared_array< double* > inDoubleBuffersPtrsHandler( new double*[ inputRasterBandsSize ] );
      boost::shared_array< double* > outDoubleBuffersPtrsHandler( new double*[ inputRasterBandsSize ] );
      double** inDoubleBuffers = inDoubleBuffersPtrsHandler.get();
      double** outDoubleBuffers = outDoubleBuffersPtrsHandler.get();
      
      for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < inputRasterBandsSize ;
        ++inputRasterBandsIdx )
      {
        const unsigned int& inBandIdx = inputRasterBands[ inputRasterBandsIdx ];
        assert( inBandIdx < paramsPtr->m_inputRasterPtr->getNumberOfBands() );
        
        te::rst::Band const * const inBandPtr = paramsPtr->m_inputRasterPtr->getBand( inBandIdx );
        te::rst::Band * const outBandPtr = paramsPtr->m_outputRasterPtr->getBand( 
          outputRasterBands[ inputRasterBandsIdx ] );
        
        maxBlocksSizesBytes = std::max( maxBlocksSizesBytes, (unsigned int)inBandPtr->getBlockSize() );
        maxBlocksSizesBytes = std::max( maxBlocksSizesBytes, (unsigned int)outBandPtr->getBlockSize() );
        
        inputBandsNoDataValues[ inputRasterBandsIdx ] = inBandPtr->getProperty()->m_noDataValue;
        outputBandsNoDataValues[ inputRasterBandsIdx ] = outBandPtr->getProperty()->m_noDataValue;
          
        inDoubleBuffersHandlers[ inputRasterBandsIdx ].reset( new double[ blockElementsNumber ] );
        inDoubleBuffers[ inputRasterBandsIdx ] = inDoubleBuffersHandlers[ inputRasterBandsIdx ].get();
        
        outDoubleBuffersHandlers[ inputRasterBandsIdx ].reset( new double[ blockElementsNumber ] );
        outDoubleBuffers[ inputRasterBandsIdx ] = outDoubleBuffersHandlers[ inputRasterBandsIdx ].get();
        
        te::rst::GetDataTypeRanges( 
          outBandPtr->getProperty()->getType(),
          outputMinValue[ inputRasterBandsIdx ],
          outputMaxValue[ inputRasterBandsIdx ] );        
      }
      
      paramsPtr->m_mutexPtr->unlock();

      boost::scoped_array< unsigned char > blockBuffer( new unsigned char[ maxBlocksSizesBytes ] );
      unsigned blkX = 0;
      unsigned int elementIdx = 0;
      boost::numeric::ublas::matrix< double > pixelValues( inputRasterBandsSize, 1 );
      boost::numeric::ublas::matrix< double > remappedPixelValues( inputRasterBandsSize, 1 );
      bool elementIsValid = false;
      
      for( unsigned blkY = 0 ; blkY < paramsPtr->m_rasterBlocksStatusPtr->getLinesNumber() ;
        ++blkY )
      {
        for( blkX = 0 ; blkX < paramsPtr->m_rasterBlocksStatusPtr->getColumnsNumber() ;
          ++blkX )
        {
          paramsPtr->m_mutexPtr->lock();
          
          if( paramsPtr->m_rasterBlocksStatusPtr->operator()( blkY, blkX ) )
          {
            paramsPtr->m_mutexPtr->unlock();
          }
          else
          {
            paramsPtr->m_rasterBlocksStatusPtr->operator()( blkY, blkX ) = true;
            
            // reading the input blocks
            
            for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < inputRasterBandsSize ;
              ++inputRasterBandsIdx )
            {    
              const unsigned int& inBandIdx = inputRasterBands[ inputRasterBandsIdx ];
              te::rst::Band const * const inBandPtr = paramsPtr->m_inputRasterPtr->getBand( inBandIdx );
              
              inBandPtr->read( blkX, blkY, blockBuffer.get() );
              Convert2DoublesVector( blockBuffer.get(), 
                inBandPtr->getProperty()->getType(), 
                blockElementsNumber,
                inDoubleBuffers[ inputRasterBandsIdx ] );            
            }
            
            paramsPtr->m_mutexPtr->unlock();
            
            // Remapping the values using the remap matrix
              
            for( elementIdx = 0 ; elementIdx < blockElementsNumber ; ++elementIdx )
            {
              elementIsValid = true;
              
              for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < inputRasterBandsSize ;
                ++inputRasterBandsIdx )
              {              
                if( inDoubleBuffers[ inputRasterBandsIdx ][ elementIdx ] == 
                  inputBandsNoDataValues[ inputRasterBandsIdx ] )
                {
                  elementIsValid = false;
                  break;
                }
                else
                {
                  pixelValues( inputRasterBandsIdx, 0 ) = 
                    inDoubleBuffers[ inputRasterBandsIdx ][ elementIdx ];
                }
              }              
              
              if( elementIsValid )
              {
//std::cout << std::endl << "pixelValues:" << pixelValues << std::endl;
                
                remappedPixelValues = boost::numeric::ublas::prod( remapMatrix, pixelValues );
                
//std::cout << std::endl << "remappedPixelValues:" << remappedPixelValues << std::endl;
                
                for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < inputRasterBandsSize ;
                  ++inputRasterBandsIdx )
                {              
                  outDoubleBuffers[ inputRasterBandsIdx ][ elementIdx ] =
                    std::max(
                      outputMinValue[ inputRasterBandsIdx ],
                      std::min(
                        outputMaxValue[ inputRasterBandsIdx ],
                        remappedPixelValues( inputRasterBandsIdx, 0 )
                      )
                    );
                } 
              }
            }
            
            // Writing the remmaped blocks
            
            paramsPtr->m_mutexPtr->lock();
            
            for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < inputRasterBandsSize ;
              ++inputRasterBandsIdx )
            { 
              te::rst::Band* outBandPtr = paramsPtr->m_outputRasterPtr->getBand( 
                outputRasterBands[ inputRasterBandsIdx ] );
              
              ConvertDoublesVector( outDoubleBuffers[ inputRasterBandsIdx ],
                blockElementsNumber, outBandPtr->getProperty()->getType(),
                blockBuffer.get() );              
              
              outBandPtr->write( blkX, blkY, blockBuffer.get() );          
            }            
            
            paramsPtr->m_mutexPtr->unlock();
          }
        }
      }
     
    }      

    bool RemapValues( 
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands, 
      const boost::numeric::ublas::matrix< double >& remapMatrix,
      te::rst::Raster& outputRaster,
      const std::vector< unsigned int >& outputRasterBands,
      const unsigned int maxThreads )
    {
      if( ( inputRaster.getAccessPolicy() & te::common::RAccess ) == 0 )
      {
        return false;
      }
      if( ( outputRaster.getAccessPolicy() & te::common::WAccess ) == 0 )
      {
        return false;
      }   
      if( remapMatrix.size1() != inputRasterBands.size() )
      {
        return false;
      }
      if( remapMatrix.size2() != inputRasterBands.size() )
      {
        return false;
      }      
      if( outputRaster.getNumberOfBands() != inputRasterBands.size() )
      {
        return false;
      }
      if( inputRaster.getNumberOfRows() != outputRaster.getNumberOfRows() )
      {
        return false;
      }
      if( inputRaster.getNumberOfColumns() != outputRaster.getNumberOfColumns() )
      {
        return false;
      }    
      if( remapMatrix.size1() != outputRasterBands.size() )
      {
        return false;
      }
      if( remapMatrix.size2() != outputRasterBands.size() )
      {
        return false;
      }     
      for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx < inputRasterBands.size() ;
        ++inputRasterBandsIdx )
      {
        if( inputRasterBands[ inputRasterBandsIdx ] >= inputRaster.getNumberOfBands() )
        {
          return false;
        }    
      }
      for( unsigned int outputRasterBandsIdx = 0 ; outputRasterBandsIdx < outputRasterBands.size() ;
        ++outputRasterBandsIdx )
      {
        if( outputRasterBands[ outputRasterBandsIdx ] >= outputRaster.getNumberOfBands() )
        {
          return false;
        }    
      }      
      
      // Checking if optmized PCA can be executed
      
      bool useOptimizedRemap = true;
      
      {
        for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx <
          inputRaster.getNumberOfBands() ; ++inputRasterBandsIdx )
        {
          if(
              ( 
                inputRaster.getBand( inputRasterBands[ inputRasterBandsIdx ] )->getProperty()->m_blkw 
                != 
                inputRaster.getBand( inputRasterBands[ 0 ] )->getProperty()->m_blkw 
              )
              ||
              ( 
                inputRaster.getBand( inputRasterBands[ inputRasterBandsIdx ] )->getProperty()->m_blkh 
                != 
                inputRaster.getBand( inputRasterBands[ 0 ] )->getProperty()->m_blkh
              )              
              ||
              ( 
                inputRaster.getBand( inputRasterBands[ inputRasterBandsIdx ] )->getProperty()->m_nblocksx 
                != 
                inputRaster.getBand( inputRasterBands[ 0 ] )->getProperty()->m_nblocksx 
              )    
              ||
              ( 
                inputRaster.getBand( inputRasterBands[ inputRasterBandsIdx ] )->getProperty()->m_nblocksy 
                != 
                inputRaster.getBand( inputRasterBands[ 0 ] )->getProperty()->m_nblocksy 
              )  
              ||
              ( 
                outputRaster.getBand( inputRasterBandsIdx )->getProperty()->m_blkw 
                != 
                inputRaster.getBand( inputRasterBands[ 0 ] )->getProperty()->m_blkw 
              )
              ||
              ( 
                outputRaster.getBand( inputRasterBandsIdx )->getProperty()->m_blkh 
                != 
                inputRaster.getBand( inputRasterBands[ 0 ] )->getProperty()->m_blkh
              )              
              ||
              ( 
                outputRaster.getBand( inputRasterBandsIdx )->getProperty()->m_nblocksx 
                != 
                inputRaster.getBand( inputRasterBands[ 0 ] )->getProperty()->m_nblocksx 
              )    
              ||
              ( 
                outputRaster.getBand( inputRasterBandsIdx )->getProperty()->m_nblocksy 
                != 
                inputRaster.getBand( inputRasterBands[ 0 ] )->getProperty()->m_nblocksy 
              )                                     
            )
          {
            useOptimizedRemap = false;
            break;
          }
        }
      }
      
      // remapping
      
      if( useOptimizedRemap )
      {
        Matrix< bool > rasterBlocksStatus;
        if( ! rasterBlocksStatus.reset( 
          inputRaster.getBand( inputRasterBands[ 0 ] )->getProperty()->m_nblocksy,
          inputRaster.getBand( inputRasterBands[ 0 ] )->getProperty()->m_nblocksx, 
          Matrix< bool >::RAMMemPol ) )
        {
          return false;
        }
        for( unsigned int row = 0 ; row < rasterBlocksStatus.getLinesNumber() ;
          ++row )
        {
          for( unsigned int col = 0 ; col < rasterBlocksStatus.getColumnsNumber() ;
            ++col )
          {
            rasterBlocksStatus( row, col ) = false;
          }
        }
        
        boost::mutex mutex;
        
        RemapValuesThreadParams params;
        params.m_inputRasterPtr = &inputRaster;
        params.m_inputRasterBandsPtr = &inputRasterBands;
        params.m_outputRasterPtr = &outputRaster;
        params.m_outputRasterBandsPtr = &outputRasterBands;
        params.m_remapMatrixPtr = &remapMatrix;
        params.m_rasterBlocksStatusPtr = &rasterBlocksStatus;
        params.m_returnStatus = true;
        params.m_mutexPtr = &mutex;
        
        if( maxThreads == 1 )
        {
          RemapValuesThread( &params );
        }
        else
        {
          unsigned int threadsNumber = maxThreads ? maxThreads : te::common::GetPhysProcNumber();
          
          boost::thread_group threads;
            
          for( unsigned int threadIdx = 0 ; threadIdx < threadsNumber ;
            ++threadIdx )
          {
            threads.add_thread( new boost::thread( RemapValuesThread, 
               &params ) );
          };  
          
          threads.join_all();
        }
        
        return params.m_returnStatus;        
      }
      else
      {
        const unsigned int inputRasterBandsSize = inputRasterBands.size();
        const unsigned int nRows = inputRaster.getNumberOfRows();
        const unsigned int nCols = inputRaster.getNumberOfColumns();
        boost::numeric::ublas::matrix< double > pixelValues( inputRasterBands.size(), 1 );
        boost::numeric::ublas::matrix< double > remappedPixelValues( inputRasterBands.size(), 1 );
        std::vector< double > inputNoDataValues( inputRasterBandsSize );
        std::vector< double > outputNoDataValues( inputRasterBandsSize );
        std::vector< double > outputMinValue( inputRasterBandsSize );
        std::vector< double > outputMaxValue( inputRasterBandsSize );        
        unsigned int inputRasterBandsIdx = 0;
        unsigned int row = 0;
        unsigned int col = 0;
        bool pixelIsValid = false;
        
        for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < inputRasterBandsSize ;
          ++inputRasterBandsIdx )
        {
          if( inputRasterBands[ inputRasterBandsIdx ] >= inputRaster.getNumberOfBands() )
          {
            return false;
          }
          
          inputNoDataValues[ inputRasterBandsIdx ] = inputRaster.getBand( 
            inputRasterBands[ inputRasterBandsIdx ] )->getProperty()->m_noDataValue;
            
          outputNoDataValues[ inputRasterBandsIdx ] = outputRaster.getBand( 
            outputRasterBands[ inputRasterBandsIdx ] )->getProperty()->m_noDataValue;
            
          te::rst::GetDataTypeRanges( 
            outputRaster.getBand( outputRasterBands[ inputRasterBandsIdx ] )->getProperty()->getType(),
            outputMinValue[ inputRasterBandsIdx ],
            outputMaxValue[ inputRasterBandsIdx ] );
        }
        
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            pixelIsValid = true;
            
            for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < inputRasterBandsSize ;
              ++inputRasterBandsIdx )
            {
              inputRaster.getValue( col, row, pixelValues( inputRasterBandsIdx, 0 ),
                inputRasterBands[ inputRasterBandsIdx ] );
              
              if( pixelValues( inputRasterBandsIdx, 0 ) == inputNoDataValues[ inputRasterBandsIdx ] )
              {
                pixelIsValid = false;
                break;
              }
            }
            
            if( pixelIsValid )
            {
              remappedPixelValues = boost::numeric::ublas::prod( remapMatrix, pixelValues );
              
              for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < inputRasterBandsSize ;
                ++inputRasterBandsIdx )
              {              
                outputRaster.setValue( 
                  col, 
                  row, 
                  std::max(
                    outputMinValue[ inputRasterBandsIdx ]
                    ,
                    std::min( 
                      outputMaxValue[ inputRasterBandsIdx ]
                      ,
                      remappedPixelValues( inputRasterBandsIdx, 0 )
                    )
                  ),
                  outputRasterBands[ inputRasterBandsIdx ] );
              }              
            }
            else
            {
              for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < inputRasterBandsSize ;
                ++inputRasterBandsIdx )
              {              
                outputRaster.setValue( col, row, outputNoDataValues[ inputRasterBandsIdx ], 
                  outputRasterBands[ inputRasterBandsIdx ] );
              }
            }
          }
        }
      }
        
      return true;
    }
    
    bool DecomposeBands( 
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,
      const std::vector< std::string >& outputDataSetNames,
      te::da::DataSource& outputDataSource )
    {
      if( !( inputRaster.getAccessPolicy() & te::common::RAccess ) )
      {
        return false;
      }
      if( ! outputDataSource.isValid() )
      {
        return false;
      }
      if( outputDataSetNames.size() != inputRasterBands.size() )
      {
        return false;
      }
      
      const unsigned int nRows = inputRaster.getNumberOfRows();
      const unsigned int nCols = inputRaster.getNumberOfColumns();
      
      for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx <
        inputRasterBands.size() ; ++inputRasterBandsIdx )
      {
        const unsigned int bandIdx = inputRasterBands[ inputRasterBandsIdx ];
        if( bandIdx >= inputRaster.getNumberOfBands() )
        {
          return false;
        }
        
        std::vector< te::rst::BandProperty* > bandsProperties;

        bandsProperties.push_back( new te::rst::BandProperty( 
          *( inputRaster.getBand( bandIdx )->getProperty()) ) );
        
        if( outputDataSetNames[ inputRasterBandsIdx ].empty() )
        {
          return false;
        }
        
        te::rp::RasterHandler outRasterHandler;
        if( ! te::rp::CreateNewRaster( *( inputRaster.getGrid() ),
          bandsProperties, outputDataSetNames[ inputRasterBandsIdx ], 
          outputDataSource, outRasterHandler) )
        {
          return false;      
        }
        
        unsigned int col = 0;
        unsigned int row = 0;
        double value = 0;
        const te::rst::Band& inBand = *(inputRaster.getBand( bandIdx ));
        te::rst::Band& outBand = *(outRasterHandler.getRasterPtr()->getBand( 0 ));
        
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            inBand.getValue( col, row, value );
            outBand.setValue( col, row, value );
          }
        }
      }
      
      return true;
    }
    
    bool ComposeBands( 
      te::rp::FeederConstRaster& feeder,
      const std::vector< unsigned int >& inputRasterBands,
      const std::string& outputDataSetName,
      const te::rst::Interpolator::Method& interpMethod,
      te::da::DataSource& outputDataSource )
    {
      if( inputRasterBands.size() != feeder.getObjsCount() )
      {
        return false;
      }
      if( outputDataSetName.empty() )
      {
        return false;
      }
      if( ! outputDataSource.isValid() )
      {
        return false;
      }      
      
      // creating the output raster
      
      te::rp::RasterHandler outRasterHandler;
      
      {
        te::rst::Raster const * inputRasterPtr = 0;
        te::rst::Grid outputGrid;
        std::vector< te::rst::BandProperty* > bandsProperties;
        
        feeder.reset();
        while( inputRasterPtr = feeder.getCurrentObj() )
        {
          if( inputRasterBands[ feeder.getCurrentOffset() ] >= 
            inputRasterPtr->getNumberOfBands() )
          {
            return false;
          }
          
          if( feeder.getCurrentOffset() == 0 )
          {
            outputGrid = ( *inputRasterPtr->getGrid() );
          }          

          bandsProperties.push_back( new te::rst::BandProperty( 
            *( inputRasterPtr->getBand( inputRasterBands[ 
            feeder.getCurrentOffset() ] )->getProperty()) ) );
          
          feeder.moveNext();
        }
        
        if( ! te::rp::CreateNewRaster( outputGrid,
          bandsProperties, outputDataSetName, 
          outputDataSource, outRasterHandler) )
        {
          return false;      
        }        
      }
      
      // copying data from each input band
        
      {
        te::rst::Raster const * inputRasterPtr = 0;
        
        feeder.reset();
        while( inputRasterPtr = feeder.getCurrentObj() )
        {
          const unsigned int inBandIdx = inputRasterBands[ 
            feeder.getCurrentOffset() ];
          te::rst::Interpolator interp( inputRasterPtr, interpMethod );
          unsigned int outRow = 0;
          unsigned int outCol = 0;
          const unsigned int nOutRows = outRasterHandler.getRasterPtr()->getNumberOfRows();
          const unsigned int nOutCols = outRasterHandler.getRasterPtr()->getNumberOfColumns();
          te::rst::Band& outBand = *outRasterHandler.getRasterPtr()->getBand( 
            feeder.getCurrentOffset() );
          const te::rst::Grid& inGrid = *inputRasterPtr->getGrid();
          const te::rst::Grid& outGrid = * outRasterHandler.getRasterPtr()->getGrid();
          double xOutCoord = 0;
          double yOutCoord = 0;
          double xInCoord = 0;
          double yInCoord = 0;          
          double inRow = 0;
          double inCol = 0;
          std::complex< double > value = 0;
          te::srs::Converter conv( outRasterHandler.getRasterPtr()->getSRID(),
            inputRasterPtr->getSRID() );
          
          if( inputRasterPtr->getSRID() == outRasterHandler.getRasterPtr()->getSRID() )
          {
            for( outRow = 0 ; outRow < nOutRows ; ++outRow )
            {
              for( outCol = 0 ; outCol < nOutCols ; ++outCol )
              {
                outGrid.gridToGeo( (double)outCol, (double)outRow, xOutCoord, yOutCoord );
                inGrid.geoToGrid( xOutCoord, yOutCoord, inCol, inRow );
                interp.getValue( inCol, inRow, value, inBandIdx );
                outBand.setValue( outCol, outRow, value );
              }
            }
          }
          else
          {
            for( outRow = 0 ; outRow < nOutRows ; ++outRow )
            {
              for( outCol = 0 ; outCol < nOutCols ; ++outCol )
              {
                outGrid.gridToGeo( (double)outCol, (double)outRow, xOutCoord, yOutCoord );
                conv.convert( xOutCoord, yOutCoord, xInCoord, yInCoord );
                inGrid.geoToGrid( xInCoord, yInCoord, inCol, inRow );
                interp.getValue( inCol, inRow, value, inBandIdx );
                outBand.setValue( outCol, outRow, value );                
              }
            }
          }
          
          feeder.moveNext();
        }
      }        
      
      return true;
    }
    
    bool GetDetailedExtent( const te::rst::Grid& grid, te::gm::LinearRing& detailedExtent )
    {
      const int nCols = (int)grid.getNumberOfColumns();
      const int nRows = (int)grid.getNumberOfRows();
      if( ( nCols == 0 ) || ( nRows == 0 ) )
      {
        return false;
      }
      
      const unsigned int ringSize = ( 2 * ( nCols + 1 ) ) +
        ( 2 * ( nRows - 1 ) ) + 1;
      
      te::gm::LinearRing ring( ringSize , te::gm::LineStringType,
        grid.getSRID(), 0 );

      const double lLX = grid.getExtent()->getLowerLeftX();
      const double lLY = grid.getExtent()->getLowerLeftY();
      const double uRX = grid.getExtent()->getUpperRightX();
      const double uRY = grid.getExtent()->getUpperRightY();
      const double& resX = grid.getResolutionX();
      const double& resY = grid.getResolutionY();        
      unsigned int ringIdx = 0;
      int row = 0;
      int col = 0;
      
      ring.setPoint( 0, lLX, uRY ); 
      
      for( col = 0 ; col < nCols ; ++col )
      {
        ring.setPoint( ++ringIdx, lLX + ( ((double)( col + 1 ) ) * resX ), uRY );
      }

      for( row = 0 ; row < nRows ; ++row )
      {
        ring.setPoint( ++ringIdx, uRX, uRY - ( ((double)( row + 1 ) ) * resY ) );
      } 
      
      for( col = nCols - 1 ; col > -1 ; --col )
      {
        ring.setPoint( ++ringIdx, lLX + ( ((double)( col + 1 ) ) * resX ), lLY );
      }    
      
      for( row = nRows - 1 ; row > 0 ; --row )
      {
        ring.setPoint( ++ringIdx, lLX, uRY - ( ((double)( row + 1 ) ) * resY ) );
      }
      
      ring.setPoint( ringSize - 1, lLX, uRY );
      
      detailedExtent = ring;
      
      return true;
    }
    
    bool GetIndexedDetailedExtent( const te::rst::Grid& grid, 
      te::gm::LinearRing& indexedDetailedExtent )
    {
      const int nCols = (int)grid.getNumberOfColumns();
      const int nRows = (int)grid.getNumberOfRows();
      if( ( nCols == 0 ) || ( nRows == 0 ) )
      {
        return false;
      }
      
      const unsigned int ringSize = ( 2 * ( nCols + 1 ) ) +
        ( 2 * ( nRows - 1 ) ) + 1;
      
      te::gm::LinearRing ring( ringSize , te::gm::LineStringType,
        0, 0 );

      const double lLY = ((double)nRows) - 0.5;
      const double uRX = ((double)nCols) - 0.5;
      unsigned int ringIdx = 0;
      int row = 0;
      int col = 0;
      
      ring.setPoint( 0, -0.5, -0.5 );
      
      for( col = 0 ; col < nCols ; ++col )
      {
        ring.setPoint( ++ringIdx, 0.5 + ((double)col), (-0.5) );
      }

      for( row = 0 ; row < nRows ; ++row )
      {
        ring.setPoint( ++ringIdx, uRX, 0.5 + ((double)row) );
      } 
      
      for( col = nCols - 1 ; col > -1 ; --col )
      {
        ring.setPoint( ++ringIdx, ((double)col) - 0.5, lLY );
      }    
      
      for( row = nRows - 1 ; row > -1 ; --row )
      {
        ring.setPoint( ++ringIdx, (-0.5), ((double)row) - 0.5 );
      }       
      
      ring.setPoint( ringSize - 1, -0.5, -0.5 );            
      
      indexedDetailedExtent = ring;
      
      return true;
    }
    
    boost::numeric::ublas::matrix< double > CreateWaveletAtrousFilter( 
      const WaveletAtrousFilterType& filterType )
    {
      boost::numeric::ublas::matrix< double > emptyFilter;
      
      switch( filterType )
      {
        case B3SplineWAFilter :
        {
          boost::numeric::ublas::matrix< double > internalFilter( 5, 5 );
          const double weight = 256;
          
          internalFilter(0,0) = 1/weight; internalFilter(0,1) = 4/weight; internalFilter(0,2) = 6/weight; internalFilter(0,3) = 4/weight; internalFilter(0,4) = 1/weight;
          internalFilter(1,0) = 4/weight; internalFilter(1,1) = 16/weight; internalFilter(1,2) = 24/weight; internalFilter(1,3) = 16/weight; internalFilter(1,4) = 4/weight;
          internalFilter(2,0) = 6/weight; internalFilter(2,1) = 24/weight; internalFilter(2,2) = 36/weight; internalFilter(2,3) = 24/weight; internalFilter(2,4) = 6/weight;
          internalFilter(3,0) = 4/weight; internalFilter(3,1) = 16/weight; internalFilter(3,2) = 24/weight; internalFilter(3,3) = 16/weight; internalFilter(3,4) = 4/weight;
          internalFilter(4,0) = 1/weight; internalFilter(4,1) = 4/weight; internalFilter(4,2) = 6/weight; internalFilter(4,3) = 4/weight; internalFilter(4,4) = 1/weight;
          
          return internalFilter;
          
          break;
        }
        case TriangleWAFilter :
        {
          boost::numeric::ublas::matrix< double > internalFilter( 3, 3 );
          const double weight = 16;
          
          internalFilter(0,0) = 1/weight; internalFilter(0,1) = 2/weight; internalFilter(0,2) = 1/weight;
          internalFilter(1,0) = 2/weight; internalFilter(1,1) = 4/weight; internalFilter(1,2) = 2/weight;
          internalFilter(2,0) = 1/weight; internalFilter(2,1) = 2/weight; internalFilter(2,2) = 1/weight;
          
          return internalFilter;
          
          break;
        }  
        default :
        {
          throw te::rp::Exception( "Invalid filter type" );
          break;
        }
      }
      
      return emptyFilter;
    }
    
    bool DirectWaveletAtrous( 
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,
      te::rst::Raster& waveletRaster,
      const unsigned int levelsNumber,
      const boost::numeric::ublas::matrix< double >& filter )
    {
      if( ! ( inputRaster.getAccessPolicy() & te::common::RAccess ) )
      {
        return false;
      }
      for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx <
        inputRasterBands.size() ; ++inputRasterBandsIdx )
      {
        if( inputRasterBands[ inputRasterBandsIdx ] >= inputRaster.getNumberOfBands() )
        {
          return false;
        }
      }
      if( 
          ( ( waveletRaster.getAccessPolicy() & te::common::WAccess ) == 0 )
          ||
          ( waveletRaster.getNumberOfColumns() != inputRaster.getNumberOfColumns() )
          ||
          ( waveletRaster.getNumberOfRows() != inputRaster.getNumberOfRows() )
          ||
          ( waveletRaster.getNumberOfBands() < ( 2 *  levelsNumber * inputRasterBands.size() ) )
        )
      {
        return false;
      }
      if( levelsNumber == 0 )
      {
        return false;
      }
      if( ( filter.size1() == 0 ) || ( filter.size2() == 0 ) || 
        ( filter.size1() != filter.size2() )
      )
      {
        return false;
      }
      
      // no-data fill
      
      {
        const unsigned int nBands = waveletRaster.getNumberOfBands();
        const unsigned int nCols = waveletRaster.getNumberOfColumns();
        const unsigned int nRows = waveletRaster.getNumberOfRows();
        unsigned int row = 0;
        unsigned int col = 0;
        
        for( unsigned int bandIdx = 0 ; bandIdx < nBands ; ++bandIdx )
        {
          te::rst::Band& band = *waveletRaster.getBand( bandIdx );
          const double noDataVale = band.getProperty()->m_noDataValue;
        
          for( row = 0 ; row < nRows ; ++row )
          {
            for( col = 0 ; col < nCols ; ++col )
            {
              band.setValue( col, row, noDataVale );
            }
          }
        }
      }
      
      // Creating the coeficients and the resitual planes for each required
      // wavelet level

      const int filterWidth = filter.size1();
      const int offset = filterWidth / 2;
      const int nLines = inputRaster.getNumberOfRows();
      const int nCols = inputRaster.getNumberOfColumns();
      
      for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx < inputRasterBands.size() ;
        ++inputRasterBandsIdx )
      {
        for( unsigned int levelIndex = 0; levelIndex < levelsNumber; ++levelIndex)
        {
          const unsigned int currentSmoothBandIdx = ( 2 * levelsNumber * 
            inputRasterBandsIdx ) + ( 2 * levelIndex );
          te::rst::Band& currentSmoothBand = *waveletRaster.getBand( currentSmoothBandIdx );
          
          const unsigned int currentWaveletBandIdx = currentSmoothBandIdx + 1;
          te::rst::Band& currentWaveletBand = *waveletRaster.getBand(
            currentWaveletBandIdx );
          
          const unsigned int prevSmoothBandIdx =  currentSmoothBandIdx - 2;
          te::rst::Band const* prevSmoothBandPtr = 0;
          if( levelIndex == 0 )
          {
            prevSmoothBandPtr = inputRaster.getBand( inputRasterBands[ inputRasterBandsIdx ] );
          }
          else
          {
            prevSmoothBandPtr = waveletRaster.getBand( prevSmoothBandIdx );
          }          
          const te::rst::Band& prevSmoothBand = *prevSmoothBandPtr;          
          
          const int filterScale = (int)std::pow(2.0, (double)levelIndex);          
          
          int col = 0;
          int row = 0;
          int convolutionCenterCol = 0;
          int convolutionCenterRow = 0;
          int filterCol = 0;
          int filterRow = 0; 
          double valueOriginal = 0.0;
          double valuePrev = 0.0;
          double valueNew = 0.0;          

          for (convolutionCenterRow = 0; convolutionCenterRow < nLines; convolutionCenterRow++)
          {
            for (convolutionCenterCol = 0; convolutionCenterCol < nCols; convolutionCenterCol++)
            {
              valueNew = 0.0;
              
              for (filterRow = 0; filterRow < filterWidth; filterRow++)
              {
                for (filterCol = 0; filterCol < filterWidth; filterCol++)
                {
                  col = convolutionCenterCol+(filterCol-offset)*filterScale;
                  row = convolutionCenterRow+(filterRow-offset)*filterScale;
                  
                  if (col < 0)
                    col = nCols + col;
                  else if (col >= nCols)
                    col = col - nCols;
                  if (row < 0)
                    row = nLines + row;
                  else if (row >= nLines)
                    row = row - nLines;
                  
                  prevSmoothBand.getValue( col, row, valueOriginal );
                  
                  valueNew += valueOriginal * filter( filterRow, filterCol );
                }
              }
              
              currentSmoothBand.setValue( convolutionCenterCol, convolutionCenterRow, 
                valueNew );
              prevSmoothBand.getValue( convolutionCenterCol, convolutionCenterRow, 
                valueOriginal );
              currentWaveletBand.setValue( convolutionCenterCol, convolutionCenterRow, 
                valueOriginal - valueNew );
            }
          }
        }
      }
      
      return true;
    }
    
    bool InverseWaveletAtrous( 
      const te::rst::Raster& waveletRaster,
      const unsigned int levelsNumber,
      te::rst::Raster& outputRaster,
      const std::vector< unsigned int >& outputRasterBands )
    {
      if( 
          ( ! ( waveletRaster.getAccessPolicy() & te::common::RAccess ) )
          ||
          ( waveletRaster.getNumberOfBands() < ( 2 *  levelsNumber * outputRasterBands.size() ) )
        )
      {
        return false;
      }
      if( levelsNumber == 0 )
      {
        return false;
      }
      if( 
          ( ( outputRaster.getAccessPolicy() & te::common::WAccess ) == 0 )
          ||
          ( waveletRaster.getNumberOfColumns() != outputRaster.getNumberOfColumns() )
          ||
          ( waveletRaster.getNumberOfRows() != outputRaster.getNumberOfRows() )
        )
      {
        return false;
      }
      for( unsigned int outputRasterBandsIdx = 0 ; outputRasterBandsIdx <
        outputRasterBands.size() ; ++outputRasterBandsIdx )
      {
        if( outputRasterBands[ outputRasterBandsIdx ] >= outputRaster.getNumberOfBands() )
        {
          return false;
        }
      }
      
      for( unsigned int outputRasterBandsIdx = 0 ; outputRasterBandsIdx <
        outputRasterBands.size() ; ++outputRasterBandsIdx )
      {
        const unsigned int outputRasterBandIdx = outputRasterBands[ outputRasterBandsIdx ];
        const unsigned int nRows = waveletRaster.getNumberOfRows();
        const unsigned int nCols = waveletRaster.getNumberOfColumns();        
        const unsigned int firstSmoothBandIdx = outputRasterBandsIdx *
          levelsNumber * 2;
        const unsigned int firstWaveletBandIdx = firstSmoothBandIdx
          + 1;          
        const unsigned int lastSmoothBandIdx = firstSmoothBandIdx
          + ( 2 * levelsNumber ) - 2;
        const unsigned int lastWaveletBandIdx = lastSmoothBandIdx + 1;
        unsigned int col = 0;
        unsigned int row = 0;        
        double value = 0;
        double sum = 0;
        unsigned int waveletRasterBand = 0;
        
        double bandAllowedMinValue = 0;
        double bandAllowedMaxValue = 0;
        te::rst::GetDataTypeRanges( outputRaster.getBand( 
          outputRasterBandIdx )->getProperty()->m_type, bandAllowedMinValue,
          bandAllowedMaxValue );
        
        
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            sum = 0.0;
            
            for( waveletRasterBand = firstWaveletBandIdx ; 
              waveletRasterBand <= lastWaveletBandIdx ; 
              waveletRasterBand += 2)
            {
              waveletRaster.getValue( col, row, value, waveletRasterBand );
              sum += value;
            }
            
            waveletRaster.getValue( col, row, value, lastSmoothBandIdx );
            sum += value;
            
            sum = std::max( sum, bandAllowedMinValue );
            sum = std::min( sum, bandAllowedMaxValue );
            
            outputRaster.setValue( col, row, sum, outputRasterBandIdx );
          }
        }
      }
           
      return true;
    }
    
    bool RasterResample( 
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,
      const te::rst::Interpolator::Method interpMethod, 
      const unsigned int firstRow,
      const unsigned int firstColumn, 
      const unsigned int height, 
      const unsigned int width,
      const unsigned int newheight, 
      const unsigned int newwidth, 
      const std::map<std::string, std::string>& rinfo,
      const std::string& dataSourceType,
      std::auto_ptr< te::rst::Raster >& resampledRasterPtr )
    {
      if( ( firstRow + height ) > inputRaster.getNumberOfRows() )
      {
        return false;
      }
      if( ( firstColumn + width ) > inputRaster.getNumberOfColumns() )
      {
        return false;
      }
      for (std::size_t inputRasterBandsIdx = 0; inputRasterBandsIdx < 
        inputRasterBands.size(); inputRasterBandsIdx++)
      {
        if( inputRasterBands[ inputRasterBandsIdx ] >= inputRaster.getNumberOfBands() )
        {
          return false;
        }
      }
      
      te::gm::Coord2D ulc = inputRaster.getGrid()->gridToGeo( ((double)firstColumn) 
        - 0.5, ((double)firstRow) - 0.5);
      te::gm::Coord2D lrc = inputRaster.getGrid()->gridToGeo( ((double)(firstColumn 
        + width)) - 0.5, ((double)(firstRow + height)) - 0.5);
      
      std::auto_ptr< te::gm::Envelope > newEnvelopePtr( new te::gm::Envelope( 
        ulc.x, lrc.y, lrc.x, ulc.y ) );  

      // create output parameters and raster
      
      std::auto_ptr< te::rst::Grid > gridPtr( new te::rst::Grid(
        newwidth, newheight, newEnvelopePtr.release(),
        inputRaster.getSRID()) );

      std::vector<te::rst::BandProperty*> bands;

      for (std::size_t inputRasterBandsIdx = 0; inputRasterBandsIdx < 
        inputRasterBands.size(); inputRasterBandsIdx++)
      {
        bands.push_back( new te::rst::BandProperty(
          *(inputRaster.getBand( inputRasterBands[ inputRasterBandsIdx ] )->getProperty())));
        bands[ inputRasterBandsIdx ]->m_blkh = 1;
        bands[ inputRasterBandsIdx ]->m_blkw = newwidth;
        bands[ inputRasterBandsIdx ]->m_nblocksx = 1;
        bands[ inputRasterBandsIdx ]->m_nblocksy = newheight;
      }

      resampledRasterPtr.reset( te::rst::RasterFactory::make( dataSourceType, 
        gridPtr.release(), bands, rinfo, 0 ) );
      if( resampledRasterPtr.get() == 0 )
      {
        return false;
      }

      // fill output raster
            
      std::complex<double> interpValue;
      te::rst::Interpolator interp( &inputRaster, interpMethod);      
      const double rowsFactor = ((double)(height-1)) / ((double)(newheight-1));
      const double colsFactor = ((double)(width-1)) / ((double)(newwidth-1));
      double inputRow = 0;
      double inputCol = 0;      
      unsigned int outputCol = 0;
      unsigned int outputRow = 0;
      unsigned int inputBandIdx = 0;
      double allowedMin = 0;
      double allowedMax = 0;
      
      for (std::size_t inputRasterBandsIdx = 0; inputRasterBandsIdx < 
        inputRasterBands.size(); inputRasterBandsIdx++)
      {
        te::rst::Band& outputBand = *resampledRasterPtr->getBand( inputRasterBandsIdx );
        inputBandIdx = inputRasterBands[ inputRasterBandsIdx ];
        te::rp::GetDataTypeRange( outputBand.getProperty()->m_type, allowedMin, 
          allowedMax );
        
        for ( outputRow = 0; outputRow < newheight; ++outputRow)
        {
          inputRow = ( ((double)( outputRow ) ) * rowsFactor ) + ((double)firstRow);
          
          for ( outputCol = 0; outputCol < newwidth; ++outputCol )
          {
            inputCol = ( ((double)( outputCol ) ) * colsFactor ) + ((double)firstColumn);
            
            interp.getValue(inputCol, inputRow, interpValue, inputBandIdx);
            
            interpValue.real( std::max( allowedMin, interpValue.real() ) );
            interpValue.real( std::min( allowedMax, interpValue.real() ) );

            outputBand.setValue(outputCol, outputRow, interpValue);
          }
        }
      }
      
      return true;
    }
    
  } // end namespace rp
}   // end namespace te
