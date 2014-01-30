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
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceFactory.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/Enums.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterFactory.h"
#include "../raster/RasterProperty.h"
#include "../raster/RasterIterator.h"
#include "../geometry/Point.h"
#include "Exception.h"
#include "Functions.h"
#include "Macros.h"
#include "RasterHandler.h"

// Boost
#include <boost/filesystem.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_array.hpp>
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
      
      boost::filesystem::path pathInfo( fileName );

      // Creating a new datasource

      std::auto_ptr< te::da::DataSource > dataSourcePtr(
        te::da::DataSourceFactory::make( "GDAL" ) );
      if( dataSourcePtr.get() == 0 ) return false;

      std::map<std::string, std::string> outputRasterInfo;
      outputRasterInfo["SOURCE"] = pathInfo.parent_path().string();

      dataSourcePtr->setConnectionInfo(outputRasterInfo);
      dataSourcePtr->open();
      if( ! dataSourcePtr->isOpened() ) return false;
      
      RasterHandler internalRasterHandler;
      
      if( CreateNewRaster( rasterGrid, bandsProperties, 
        pathInfo.filename().string(), *dataSourcePtr, internalRasterHandler ) )
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

    void GetDataTypeRange( const int dataType, double& min, double& max )
    {
      switch( dataType )
      {
        case te::dt::BIT_TYPE :
          min = 0;
          max = 1;
          break;
        case te::dt::CHAR_TYPE :
          min = -128;
          max = 128;
          break;
        case te::dt::UCHAR_TYPE :
          min = 0;
          max = 255;
          break;
        case te::dt::INT16_TYPE :
        case te::dt::CINT16_TYPE :
          min = -1.0 * (double)std::numeric_limits<short int>::max();
          max = (double)std::numeric_limits<short int>::max();
          break;
        case te::dt::UINT16_TYPE :
          min = 0;
          max = (double)std::numeric_limits<unsigned short int>::max();
          break;
        case te::dt::INT32_TYPE :
        case te::dt::CINT32_TYPE :
          min = -1.0 * (double)std::numeric_limits<int>::max();
          max = (double)std::numeric_limits<int>::max();
          break;
        case te::dt::UINT32_TYPE :
          min = 0;
          max = (double)std::numeric_limits<unsigned int>::max();
          break;
        case te::dt::INT64_TYPE :
          min = -1.0 * (double)std::numeric_limits<long int>::max();
          max = (double)std::numeric_limits<long int>::max();
          break;
        case te::dt::UINT64_TYPE :
          min = 0;
          max = (double)std::numeric_limits<unsigned long int>::max();
          break;
        case te::dt::FLOAT_TYPE :
        case te::dt::CFLOAT_TYPE :
          min = -1.0 * (double)std::numeric_limits<float>::max();
          max = (double)std::numeric_limits<float>::max();
          break;
        case te::dt::DOUBLE_TYPE :
        case te::dt::CDOUBLE_TYPE :
          min = -1.0 * (double)std::numeric_limits<double>::max();
          max = (double)std::numeric_limits<double>::max();
          break;
        default :
          throw te::rp::Exception( "Invalid data type" );
          break;
      };
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
          memcpy( outputVector, outputVector, inputVectorSize * sizeof( double ) );
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
    
    bool ConvertRBG2IHS( const te::rst::Raster& inputRGBRaster, 
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

  } // end namespace rp
}   // end namespace te
