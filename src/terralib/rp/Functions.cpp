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
#include <boost/random.hpp>
#include <boost/random/uniform_int_distribution.hpp>


// STL
#include <cstring>
#include <limits>
#include <map>
#include <memory>

namespace te
{
  namespace rp
  {
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

      //std::auto_ptr< te::da::DataSourceTransactor > transactorPtr(
      //  outDataSource.getTransactor() );

      //if( transactorPtr.get() == 0 )
      //{
      //  return false;
      //}

      // acquiring a persistence instance

      //std::auto_ptr< te::da::DataSetTypePersistence > persistencePtr(
      //  transactorPtr->getDataSetTypePersistence() );

      //if( persistencePtr.get() == 0 )
      //{
      //  return false;
      //}

      // Creating a data set instance

      std::auto_ptr< te::da::DataSetType > dataSetTypePtr(
        new te::da::DataSetType( outDataSetName ) );
      dataSetTypePtr->add( rasterPropertyPtr.release() );

      //persistencePtr->create( dataSetTypePtr.release() );

      //std::auto_ptr< te::da::DataSet > dataSetPtr( transactorPtr->getDataSet(
      std::auto_ptr< te::da::DataSet > dataSetPtr( outDataSource.getDataSet(
        outDataSetName, te::common::FORWARDONLY) );

      if( dataSetPtr.get() == 0 )
      {
        return false;
      }

      // Creating a raster instance

      std::size_t rpos = te::da::GetFirstPropertyPos(dataSetPtr.get(), te::dt::RASTER_TYPE);

      std::auto_ptr< te::rst::Raster > rasterPtr( dataSetPtr->getRaster(rpos) );

      if( rasterPtr.get() )
      {
        //outRasterHandler.reset( transactorPtr.release(),
          //persistencePtr.release(), dataSetPtr.release(),
          //rasterPtr.release() );
        outRasterHandler.reset( dataSetPtr.release(), rasterPtr.release() );
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
      // Creating a new memory datasource

      boost::shared_ptr< te::da::DataSource > dataSourcePtr(
        te::da::DataSourceFactory::make( "MEM" ) );
      TERP_TRUE_OR_THROW( dataSourcePtr.get(), "Data source creation error" );

      // Defining the raster properties

      std::auto_ptr< te::rst::RasterProperty > rasterPropertyPtr(
        new te::rst::RasterProperty( new te::rst::Grid( rasterGrid ),
        bandsProperties, std::map< std::string, std::string >(),
        false, 0, 0 ) );

      // acquiring a transactor instance

      // boost::shared_ptr< te::da::DataSourceTransactor > transactorPtr(
      //  dataSourcePtr->getTransactor() );

      //if( transactorPtr.get() == 0 )
      //{
      //  return false;
      //}

      // acquiring a persistence instance

      //boost::shared_ptr< te::da::DataSetTypePersistence > persistencePtr(
      //  transactorPtr->getDataSetTypePersistence() );

      //if( persistencePtr.get() == 0 )
      //{
      //  return false;
      //}

      // Creating a data set instance

      std::auto_ptr< te::da::DataSetType > dataSetTypePtr(
        new te::da::DataSetType( "createNewMemRaster" ) );
      dataSetTypePtr->add( rasterPropertyPtr.release() );

      //persistencePtr->create( dataSetTypePtr.release() );

      //boost::shared_ptr< te::da::DataSet > dataSetPtr( transactorPtr->getDataSet(
      //  "createNewMemRaster", te::common::FORWARDONLY, te::common::RWAccess) );

      boost::shared_ptr< te::da::DataSet > dataSetPtr( dataSourcePtr->getDataSet(
        "createNewMemRaster", te::common::FORWARDONLY) );

      if( dataSetPtr.get() == 0 )
      {
        return false;
      }

      // Creating a raster instance
      std::size_t rpos = te::da::GetFirstPropertyPos(dataSetPtr.get(), te::dt::RASTER_TYPE);

      boost::shared_ptr< te::rst::Raster > rasterPtr( dataSetPtr->getRaster(rpos) );

      if( rasterPtr.get() )
      {
        //outRasterHandler.reset( dataSourcePtr, transactorPtr,
        //  persistencePtr, dataSetPtr, rasterPtr );

        outRasterHandler.reset( dataSourcePtr, dataSetPtr, rasterPtr );

        return true;
      }
      else
      {
        return false;
      }
    }

    bool CreateNewGeotifRaster( const te::rst::Grid& rasterGrid,
      std::vector< te::rst::BandProperty* > bandsProperties,
      const std::string& fileName,
      RasterHandler& outRasterHandler )
    {
      boost::filesystem::path pathInfo( fileName );

      // Creating a new memory datasource

      boost::shared_ptr< te::da::DataSource > dataSourcePtr(
        te::da::DataSourceFactory::make( "GDAL" ) );
      if( dataSourcePtr.get() == 0 ) return false;

      std::map<std::string, std::string> outputRasterInfo;
      outputRasterInfo["URI"] = pathInfo.parent_path().string();

      dataSourcePtr->setConnectionInfo(outputRasterInfo);
      dataSourcePtr->open();
      if( ! dataSourcePtr->isOpened() ) return false;

      // Defining the raster properties

      std::auto_ptr< te::rst::RasterProperty > rasterPropertyPtr(
        new te::rst::RasterProperty( new te::rst::Grid( rasterGrid ),
        bandsProperties, std::map< std::string, std::string >(),
        false, 0, 0 ) );

      // acquiring a transactor instance

      // boost::shared_ptr< te::da::DataSourceTransactor > transactorPtr(
      //  dataSourcePtr->getTransactor() );

      //if( transactorPtr.get() == 0 )
      //{
      //  return false;
      //}

      // acquiring a persistence instance

      //boost::shared_ptr< te::da::DataSetTypePersistence > persistencePtr(
      //  transactorPtr->getDataSetTypePersistence() );

      //if( persistencePtr.get() == 0 )
      //{
      //  return false;
      //}

      // Creating a data set instance

      std::auto_ptr< te::da::DataSetType > dataSetTypePtr(
        new te::da::DataSetType( pathInfo.filename().string() ) );
      dataSetTypePtr->add( rasterPropertyPtr.release() );

      //persistencePtr->create( dataSetTypePtr.release() );

      boost::shared_ptr< te::da::DataSet > dataSetPtr( dataSourcePtr->getDataSet(
        pathInfo.filename().string(), te::common::FORWARDONLY) );

      if( dataSetPtr.get() == 0 )
      {
        return false;
      }

      // Creating a raster instance
      std::size_t rpos = te::da::GetFirstPropertyPos(dataSetPtr.get(), te::dt::RASTER_TYPE);

      boost::shared_ptr< te::rst::Raster > rasterPtr( dataSetPtr->getRaster(rpos) );

      if( rasterPtr.get() )
      {
        //outRasterHandler.reset( dataSourcePtr, transactorPtr,
        //  persistencePtr, dataSetPtr, rasterPtr );

        outRasterHandler.reset( dataSourcePtr, dataSetPtr, rasterPtr );

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
          min = (double)std::numeric_limits<char>::min();
          max = (double)std::numeric_limits<char>::max();
          break;
        case te::dt::UCHAR_TYPE :
          min = (double)std::numeric_limits<unsigned char>::min();
          max = (double)std::numeric_limits<unsigned char>::max();
          break;
        case te::dt::INT16_TYPE :
        case te::dt::CINT16_TYPE :
          min = (double)std::numeric_limits<short int>::min();
          max = (double)std::numeric_limits<short int>::max();
          break;
        case te::dt::UINT16_TYPE :
          min = (double)std::numeric_limits<unsigned short int>::min();
          max = (double)std::numeric_limits<unsigned short int>::max();
          break;
        case te::dt::INT32_TYPE :
        case te::dt::CINT32_TYPE :
          min = (double)std::numeric_limits<int>::min();
          max = (double)std::numeric_limits<int>::max();
          break;
        case te::dt::UINT32_TYPE :
          min = (double)std::numeric_limits<unsigned int>::min();
          max = (double)std::numeric_limits<unsigned int>::max();
          break;
        case te::dt::INT64_TYPE :
          min = (double)std::numeric_limits<long int>::min();
          max = (double)std::numeric_limits<long int>::max();
          break;
        case te::dt::UINT64_TYPE :
          min = (double)std::numeric_limits<unsigned long int>::min();
          max = (double)std::numeric_limits<unsigned long int>::max();
          break;
        case te::dt::FLOAT_TYPE :
        case te::dt::CFLOAT_TYPE :
          min = (double)std::numeric_limits<float>::min();
          max = (double)std::numeric_limits<float>::max();
          break;
        case te::dt::DOUBLE_TYPE :
        case te::dt::CDOUBLE_TYPE :
          min = (double)std::numeric_limits<double>::min();
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

  } // end namespace rp
}   // end namespace te
