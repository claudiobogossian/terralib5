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
  \file terralib/unittest/rp/functions/TsFunctions.cpp

  \brief A test suit for the Functions interface.
*/

// TerraLib
#include "../Config.h"
#include <terralib/rp.h>
#include <terralib/raster.h>
#include <terralib/dataaccess.h>

// STL
#include <memory>
#include <string>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

BOOST_AUTO_TEST_SUITE (functions_tests)

BOOST_AUTO_TEST_CASE( createFixedStepPalette_test )
{
  std::vector< te::rst::BandProperty * > bandsProps;
  bandsProps.push_back( new te::rst::BandProperty( 0, 
    te::dt::UCHAR_TYPE ) );    
  bandsProps.push_back( new te::rst::BandProperty( 1, 
    te::dt::UCHAR_TYPE ) );    
  bandsProps.push_back( new te::rst::BandProperty( 2, 
    te::dt::UCHAR_TYPE ) );   
  bandsProps[ 0 ]->m_colorInterp = te::rst::RedCInt;
  bandsProps[ 1 ]->m_colorInterp = te::rst::GreenCInt;
  bandsProps[ 2 ]->m_colorInterp = te::rst::BlueCInt;  
  bandsProps[ 0 ]->m_noDataValue = std::numeric_limits< double >::max();
  bandsProps[ 1 ]->m_noDataValue = std::numeric_limits< double >::max();
  bandsProps[ 2 ]->m_noDataValue = std::numeric_limits< double >::max();  
  
  std::map< std::string, std::string > rInfo;
  rInfo["URI"] = "terralib_unittest_rp_functions_CreateFixedStepPalette.tif";
  
  std::auto_ptr< te::rst::Raster > rasterPointer( te::rst::RasterFactory::make( "GDAL", 
    new te::rst::Grid( 256, 256 ), bandsProps, 
    rInfo, 0, 0 ) );
  
  std::vector< te::rst::BandProperty::ColorEntry > palette;
  te::rp::CreateFixedStepPalette( 256, true, palette );
    
  unsigned int line = 0;
  unsigned int col = 0;
  
  for( line = 0 ; line < 256 ; ++line )
    for( col = 0 ; col < 256 ; ++col )
    {
      rasterPointer->setValue( col, line, (double)palette[ line ].c1, 0 );
      rasterPointer->setValue( col, line, (double)palette[ line ].c2, 1 );
      rasterPointer->setValue( col, line, (double)palette[ line ].c3, 2 );
    }
}

BOOST_AUTO_TEST_CASE(rasterSlicing_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );

  {
    std::auto_ptr< te::rst::Raster > outRasterPtr;
    std::map<std::string, std::string> auxRasterInfo2;
    auxRasterInfo2["URI"] = "terralib_unittest_rp_functions_RasterSlicing_RGB.tif";
    BOOST_CHECK( te::rp::RasterSlicing(
      *diskRasterPtr.get(), 
      0, 
      false,
      10,
      true,
      auxRasterInfo2,
      "GDAL",
      true,
      0,
      outRasterPtr ) );  
  }
  
  {
    std::auto_ptr< te::rst::Raster > outRasterPtr;
    std::map<std::string, std::string> auxRasterInfo2;
    auxRasterInfo2["URI"] = "terralib_unittest_rp_functions_RasterSlicing_palette.tif";
    BOOST_CHECK( te::rp::RasterSlicing(
      *diskRasterPtr.get(), 
      0, 
      true,
      10,
      true,
      auxRasterInfo2,
      "GDAL",
      true,
      0,
      outRasterPtr ) );
  }
}

BOOST_AUTO_TEST_CASE(ihs_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );
  
  auxRasterInfo.clear();
  auxRasterInfo["MEM_RASTER_NROWS"] = boost::lexical_cast< std::string >( 
    diskRasterPtr->getNumberOfRows() );
  auxRasterInfo["MEM_RASTER_NCOLS"] = boost::lexical_cast< std::string >( 
    diskRasterPtr->getNumberOfColumns() );
  auxRasterInfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast< std::string >((int)te::dt::DOUBLE_TYPE);
  auxRasterInfo["MEM_RASTER_NBANDS"] = "3";
   
  std::auto_ptr< te::rst::Raster > ihsRasterPtr( te::rst::RasterFactory::make(
    "MEM", 0, std::vector<te::rst::BandProperty*>(), auxRasterInfo) );
  BOOST_CHECK( ihsRasterPtr.get() );

  BOOST_CHECK( te::rp::ConvertRGB2IHS( *diskRasterPtr, 0, 1, 2, 0, 255, *ihsRasterPtr ) );
  
  BOOST_CHECK( te::rp::Copy2DiskRaster( *ihsRasterPtr, "terralib_unittest_rp_functions_rgb2ihs.tif" ) ) ;
  
  auxRasterInfo.clear();
  auxRasterInfo["MEM_RASTER_NROWS"] = boost::lexical_cast< std::string >( 
    diskRasterPtr->getNumberOfRows() );
  auxRasterInfo["MEM_RASTER_NCOLS"] = boost::lexical_cast< std::string >( 
    diskRasterPtr->getNumberOfColumns() );
  auxRasterInfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast< std::string >((int)te::dt::UCHAR_TYPE);
  auxRasterInfo["MEM_RASTER_NBANDS"] = "3";
   
  std::auto_ptr< te::rst::Raster > rgbRasterPtr( te::rst::RasterFactory::make(
    "MEM", 0, std::vector<te::rst::BandProperty*>(), auxRasterInfo) );
  BOOST_CHECK( rgbRasterPtr.get() );
  
  BOOST_CHECK( te::rp::ConvertIHS2RGB( *ihsRasterPtr, 0, 1, 2, 0, 255, *rgbRasterPtr ) );
  
  std::vector< te::rst::BandProperty * > bandProps;
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 0 )->getProperty() ) ) );
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 1 )->getProperty() ) ) );
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 2 )->getProperty() ) ) );
  
  auxRasterInfo["URI"] = "terralib_unittest_rp_functions_ihs2rgb.tif";
  
  std::auto_ptr< te::rst::Raster > outDiskRasterPtr( te::rst::RasterFactory::make(
    "GDAL", new te::rst::Grid( *( diskRasterPtr->getGrid() ) ), bandProps, 
    auxRasterInfo, 0, 0 ) );
  BOOST_CHECK( outDiskRasterPtr.get() );
  
  const unsigned int nBands = outDiskRasterPtr->getNumberOfBands();
  const unsigned int nCols = outDiskRasterPtr->getNumberOfColumns();
  const unsigned int nRows = outDiskRasterPtr->getNumberOfRows();
  unsigned int col = 0;
  unsigned int row = 0;
  double value = 0;
  
  for( unsigned int band = 0 ; band < nBands ; ++band )
  {
    for( row = 0 ; row < nRows ; ++row )
    {
      for( col = 0 ; col < nCols ; ++col )
      {
        rgbRasterPtr->getValue( col, row, value, band );
        outDiskRasterPtr->setValue( col, row, value, band );
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(getMeanValue_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );
  
  double meanValue = 0;
  
//  boost::timer timer;
  BOOST_CHECK( te::rp::GetMeanValue( *diskRasterPtr->getBand( 0 ), 1, meanValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  BOOST_CHECK_CLOSE( 181.340256531345, meanValue, 0.0001 );
}

BOOST_AUTO_TEST_CASE(getMeanValueOptimized_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );
  
  double meanValue = 0;
  
//  boost::timer timer;
  BOOST_CHECK( te::rp::GetMeanValue( *diskRasterPtr->getBand( 0 ), 4, meanValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  BOOST_CHECK_CLOSE( 181.340256531345, meanValue, 0.0001 );
}

BOOST_AUTO_TEST_CASE(getCovarianceValue_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );
  
  double covarianceValue = 0;
  
//  boost::timer timer;
  BOOST_CHECK( te::rp::GetCovarianceValue( *diskRasterPtr->getBand( 0 ),
    *diskRasterPtr->getBand( 0 ), 1, 0, 0, covarianceValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  BOOST_CHECK_CLOSE( 2143.89743610679, covarianceValue, 0.0001 );
}

BOOST_AUTO_TEST_CASE(getCovarianceValueOptimized_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );
  
  double covarianceValue = 0;
  
//  boost::timer timer;
  BOOST_CHECK( te::rp::GetCovarianceValue( *diskRasterPtr->getBand( 0 ),
    *diskRasterPtr->getBand( 0 ), 4, 0, 0, covarianceValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  BOOST_CHECK_CLOSE( 2143.89743610679, covarianceValue, 0.0001 );
}

BOOST_AUTO_TEST_CASE(principalComponents_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );
  
  std::vector< te::rst::BandProperty * > bandProps1;
  bandProps1.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 0 )->getProperty() ) ) );
  bandProps1[ 0 ]->m_blkh = diskRasterPtr->getNumberOfRows();
  bandProps1[ 0 ]->m_blkw = diskRasterPtr->getNumberOfColumns();
  bandProps1[ 0 ]->m_nblocksx = 1;
  bandProps1[ 0 ]->m_nblocksy = 1;
  bandProps1[ 0 ]->m_type = te::dt::DOUBLE_TYPE;
  bandProps1.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 1 )->getProperty() ) ) );
  bandProps1[ 1 ]->m_blkh = diskRasterPtr->getNumberOfRows();
  bandProps1[ 1 ]->m_blkw = diskRasterPtr->getNumberOfColumns();
  bandProps1[ 1 ]->m_nblocksx = 1;
  bandProps1[ 1 ]->m_nblocksy = 1;  
  bandProps1[ 1 ]->m_type = te::dt::DOUBLE_TYPE;
  bandProps1.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 2 )->getProperty() ) ) );
  bandProps1[ 2 ]->m_blkh = diskRasterPtr->getNumberOfRows();
  bandProps1[ 2 ]->m_blkw = diskRasterPtr->getNumberOfColumns();
  bandProps1[ 2 ]->m_nblocksx = 1;
  bandProps1[ 2 ]->m_nblocksy = 1; 
  bandProps1[ 2 ]->m_type = te::dt::DOUBLE_TYPE;
  
  std::auto_ptr< te::rst::Raster > pcaRasterPtr( te::rst::RasterFactory::make(
    "MEM", new te::rst::Grid( *( diskRasterPtr->getGrid() ) ), bandProps1, 
    std::map<std::string, std::string>(), 0, 0 ) );
  BOOST_CHECK( pcaRasterPtr.get() );
  
  std::vector< unsigned int > inputRasterBands;
  inputRasterBands.push_back( 0 );
  inputRasterBands.push_back( 1 );
  inputRasterBands.push_back( 2 );
  
  boost::numeric::ublas::matrix< double > pcaMatrix;
  
  BOOST_CHECK( te::rp::DirectPrincipalComponents(
    *diskRasterPtr, 
    inputRasterBands,
    pcaMatrix,
    *pcaRasterPtr,
    inputRasterBands,
    1 ) );
    
  BOOST_CHECK( te::rp::Copy2DiskRaster( *pcaRasterPtr, "terralib_unittest_rp_functions_DirectPrincipalComponents.tif" ) ) ;
  
  auxRasterInfo.clear();
  auxRasterInfo["URI"] = "terralib_unittest_rp_functions_InversePrincipalComponents.tif";
  
  std::vector< te::rst::BandProperty * > bandProps2;
  bandProps2.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 0 )->getProperty() ) ) );
  bandProps2.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 1 )->getProperty() ) ) );
  bandProps2.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 2 )->getProperty() ) ) );
  
  std::auto_ptr< te::rst::Raster > outDiskRasterPtr( te::rst::RasterFactory::make(
    "GDAL", new te::rst::Grid( *( diskRasterPtr->getGrid() ) ), bandProps2, 
    auxRasterInfo, 0, 0 ) );
  BOOST_CHECK( outDiskRasterPtr.get() );
  
  BOOST_CHECK( te::rp::InversePrincipalComponents(
    *pcaRasterPtr,
    pcaMatrix,
    *outDiskRasterPtr,
    inputRasterBands,
    1 ) );  
}

BOOST_AUTO_TEST_CASE(principalComponentsOptimized_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );
  
  std::vector< te::rst::BandProperty * > bandProps1;
  bandProps1.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 0 )->getProperty() ) ) );
  bandProps1[ 0 ]->m_type = te::dt::DOUBLE_TYPE;
  bandProps1.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 1 )->getProperty() ) ) );
  bandProps1[ 1 ]->m_type = te::dt::DOUBLE_TYPE;
  bandProps1.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 2 )->getProperty() ) ) );
  bandProps1[ 2 ]->m_type = te::dt::DOUBLE_TYPE;
  
  std::auto_ptr< te::rst::Raster > pcaRasterPtr( te::rst::RasterFactory::make(
    "MEM", new te::rst::Grid( *( diskRasterPtr->getGrid() ) ), bandProps1, 
    std::map<std::string, std::string>(), 0, 0 ) );
  BOOST_CHECK( pcaRasterPtr.get() );
  
  std::vector< unsigned int > inputRasterBands;
  inputRasterBands.push_back( 0 );
  inputRasterBands.push_back( 1 );
  inputRasterBands.push_back( 2 );
  
  boost::numeric::ublas::matrix< double > pcaMatrix;
  
  BOOST_CHECK( te::rp::DirectPrincipalComponents(
    *diskRasterPtr, 
    inputRasterBands,
    pcaMatrix,
    *pcaRasterPtr,
    inputRasterBands,
    4 ) );
    
  BOOST_CHECK( te::rp::Copy2DiskRaster( *pcaRasterPtr, "terralib_unittest_rp_functions_DirectPrincipalComponentsOptimized.tif" ) ) ;
  
  std::vector< te::rst::BandProperty * > bandProps;
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 0 )->getProperty() ) ) );
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 1 )->getProperty() ) ) );
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 2 )->getProperty() ) ) );
  
  auxRasterInfo.clear();
  auxRasterInfo["URI"] = "terralib_unittest_rp_functions_InversePrincipalComponentsOptimized.tif";
  
  std::auto_ptr< te::rst::Raster > outDiskRasterPtr( te::rst::RasterFactory::make(
    "GDAL", new te::rst::Grid( *( diskRasterPtr->getGrid() ) ), bandProps, 
    auxRasterInfo, 0, 0 ) );
  BOOST_CHECK( outDiskRasterPtr.get() );
  
  BOOST_CHECK( te::rp::InversePrincipalComponents(
    *pcaRasterPtr,
    pcaMatrix,
    *outDiskRasterPtr,
    inputRasterBands,
    4 ) );  
}

BOOST_AUTO_TEST_CASE(getStdDevValue_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );
  
  double stdDevValue = 0;
  
//  boost::timer timer;
  BOOST_CHECK( te::rp::GetStdDevValue( *diskRasterPtr->getBand( 0 ), 1, 0, stdDevValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  BOOST_CHECK_CLOSE( 46.302240075, stdDevValue, 0.0001 );
}

BOOST_AUTO_TEST_CASE(getStdDevValueOptimized_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );
  
  double stdDevValue = 0;
  
//  boost::timer timer;
  BOOST_CHECK( te::rp::GetStdDevValue( *diskRasterPtr->getBand( 0 ), 4, 0, stdDevValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  BOOST_CHECK_CLOSE( 46.302240075, stdDevValue, 0.0001 );
}

BOOST_AUTO_TEST_CASE(decomposeBands_test)
{
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );
  
  std::vector< unsigned int > inputRasterBands;
  inputRasterBands.push_back( 2 );
  inputRasterBands.push_back( 0 );
  
  std::vector< std::map<std::string, std::string> > outputRastersInfos( 2 );
  outputRastersInfos[ 0 ][ "URI" ] = "terralib_unittest_rp_functions_DecomposeBands_2.tif";
  outputRastersInfos[ 1 ][ "URI" ] = "terralib_unittest_rp_functions_DecomposeBands_0.tif";

  std::vector< boost::shared_ptr< te::rst::Raster > > outputRastersPtrs;
  
  BOOST_CHECK( te::rp::DecomposeBands( *diskRasterPtr, inputRasterBands,
    outputRastersInfos, "GDAL", outputRastersPtrs ) );  
}

BOOST_AUTO_TEST_CASE(composeBandsSameSRID_test)
{
  /* Openning input rasters */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop2.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr1( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr1.get() );

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop3.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr2( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr2.get() );
  
  std::vector< const te::rst::Raster * > rasters;
  rasters.push_back( diskRasterPtr1.get() );
  rasters.push_back( diskRasterPtr2.get() );

  te::rp::FeederConstRasterVector feeder( rasters );
  
  std::vector< unsigned int > inputRasterBands;
  inputRasterBands.push_back( 2 );
  inputRasterBands.push_back( 0 );
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_functions_ComposeBandsSameSRID.tif";
  
  std::auto_ptr< te::rst::Raster > outputRasterPtr;
  
  BOOST_CHECK( te::rp::ComposeBands( feeder, inputRasterBands,
    te::rst::NearestNeighbor, outputRasterInfo,
    "GDAL", outputRasterPtr ) );
}

BOOST_AUTO_TEST_CASE(composeBandsDifSRID_test)
{
  /* Openning input rasters */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop2.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr1( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr1.get() );

  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers_rgb342_crop3_EPSG_22522.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr2( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr2.get() );
  
  std::vector< const te::rst::Raster * > rasters;
  rasters.push_back( diskRasterPtr1.get() );
  rasters.push_back( diskRasterPtr2.get() );

  te::rp::FeederConstRasterVector feeder( rasters );
  
  std::vector< unsigned int > inputRasterBands;
  inputRasterBands.push_back( 2 );
  inputRasterBands.push_back( 0 );
  
  std::map<std::string, std::string> outputRasterInfo;
  outputRasterInfo["URI"] = "terralib_unittest_rp_functions_ComposeBandsDifSRID.tif";
  
  std::auto_ptr< te::rst::Raster > outputRasterPtr;
  
  BOOST_CHECK( te::rp::ComposeBands( feeder, inputRasterBands,
    te::rst::NearestNeighbor, outputRasterInfo,
    "GDAL", outputRasterPtr ) );  
}

BOOST_AUTO_TEST_CASE(getDetailedExtent_test)
{
  te::rst::Grid grid( 2u, 2u, new te::gm::Envelope( 0.0, 0.0, 2.0, 2.0 ), 12345 );
  
  te::gm::LinearRing lr( te::gm::LineStringType, 0, 0 );
  
  BOOST_CHECK( te::rp::GetDetailedExtent( grid, lr ) );
  BOOST_CHECK( lr.size() == 9 );
  
  BOOST_CHECK_CLOSE( lr.getX( 0 ), 0.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 0 ), 2.0 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 1 ), 1.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 1 ), 2.0 , 0.0000000001 );

  BOOST_CHECK_CLOSE( lr.getX( 2 ), 2.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 2 ), 2.0 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 3 ), 2.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 3 ), 1.0 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 4 ), 2.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 4 ), 0.0 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 5 ), 1.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 5 ), 0.0 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 6 ), 0.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 6 ), 0.0 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 7 ), 0.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 7 ), 1.0 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 8 ), 0.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 8 ), 2.0 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( grid.getExtent()->m_llx, 0.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( grid.getExtent()->m_lly, 0.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( grid.getExtent()->m_urx, 2.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( grid.getExtent()->m_ury, 2.0 , 0.0000000001 );
}

BOOST_AUTO_TEST_CASE(getIndexedDetailedExtent_test)
{
  te::rst::Grid grid( 2u, 2u, new te::gm::Envelope( 0.0, 0.0, 2.0, 2.0 ), 12345 );
  
  te::gm::LinearRing lr( te::gm::LineStringType, 0, 0 );
  
  BOOST_CHECK( te::rp::GetIndexedDetailedExtent( grid, lr ) );
  BOOST_CHECK( lr.size() == 9 );
  
  BOOST_CHECK_CLOSE( lr.getX( 0 ), -0.5 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 0 ), -0.5 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 1 ), 0.5 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 1 ), -0.5 , 0.0000000001 );

  BOOST_CHECK_CLOSE( lr.getX( 2 ), 1.5 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 2 ), -0.5 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 3 ), 1.5 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 3 ), 0.5 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 4 ), 1.5 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 4 ), 1.5 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 5 ), 0.5 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 5 ), 1.5 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 6 ), -0.5 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 6 ), 1.5 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 7 ), -0.5 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 7 ), 0.5 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( lr.getX( 8 ), -0.5 , 0.0000000001 );
  BOOST_CHECK_CLOSE( lr.getY( 8 ), -0.5 , 0.0000000001 );
  
  BOOST_CHECK_CLOSE( grid.getExtent()->m_llx, 0.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( grid.getExtent()->m_lly, 0.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( grid.getExtent()->m_urx, 2.0 , 0.0000000001 );
  BOOST_CHECK_CLOSE( grid.getExtent()->m_ury, 2.0 , 0.0000000001 );  
}

BOOST_AUTO_TEST_CASE(waveletAtrous_test)
{
  const unsigned int waveletDecompLevelsNumb = 3;
  
  /* Openning input raster */
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/geotiff/cbers2b_rgb342_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  BOOST_CHECK( diskRasterPtr.get() );
  
  std::vector< te::rst::BandProperty * > waveletRasterBandProps;
  for( unsigned int bandIdx = 0 ; bandIdx < diskRasterPtr->getNumberOfBands() ;
    ++bandIdx )
  {
    for( unsigned int levelIdx = 0 ; levelIdx < waveletDecompLevelsNumb ;
      ++levelIdx )
    {
      waveletRasterBandProps.push_back( new te::rst::BandProperty( 
        *( diskRasterPtr->getBand( 0 )->getProperty() ) ) );
      waveletRasterBandProps.back()->m_blkh = diskRasterPtr->getNumberOfRows();
      waveletRasterBandProps.back()->m_blkw = diskRasterPtr->getNumberOfColumns();
      waveletRasterBandProps.back()->m_nblocksx = 1;
      waveletRasterBandProps.back()->m_nblocksy = 1;
      waveletRasterBandProps.back()->m_type = te::dt::DOUBLE_TYPE;
      
      waveletRasterBandProps.push_back( new te::rst::BandProperty( 
        *( diskRasterPtr->getBand( 0 )->getProperty() ) ) );
      waveletRasterBandProps.back()->m_blkh = diskRasterPtr->getNumberOfRows();
      waveletRasterBandProps.back()->m_blkw = diskRasterPtr->getNumberOfColumns();
      waveletRasterBandProps.back()->m_nblocksx = 1;
      waveletRasterBandProps.back()->m_nblocksy = 1;
      waveletRasterBandProps.back()->m_type = te::dt::DOUBLE_TYPE;      
    }
  }
  
  std::auto_ptr< te::rst::Raster > waveletRasterPtr( te::rst::RasterFactory::make(
    "MEM", new te::rst::Grid( *( diskRasterPtr->getGrid() ) ), waveletRasterBandProps, 
    std::map<std::string, std::string>(), 0, 0 ) );
  BOOST_CHECK( waveletRasterPtr.get() );
  
  std::vector< unsigned int > inputRasterBands;
  inputRasterBands.push_back( 0 );
  inputRasterBands.push_back( 1 );
  inputRasterBands.push_back( 2 );
  
  boost::numeric::ublas::matrix< double > filter = te::rp::CreateWaveletAtrousFilter( 
    te::rp::B3SplineWAFilter );  
  
  BOOST_CHECK( te::rp::DirectWaveletAtrous(
    *diskRasterPtr, 
    inputRasterBands,
    *waveletRasterPtr,
    waveletDecompLevelsNumb,
    filter ) );
    
  BOOST_CHECK( te::rp::Copy2DiskRaster( *waveletRasterPtr, "terralib_unittest_rp_functions_DirectWaveletAtrous.tif" ) ) ;
  
  auxRasterInfo.clear();
  auxRasterInfo["URI"] = "terralib_unittest_rp_functions_InverseWaveletAtrous.tif";
  
  std::vector< te::rst::BandProperty * > bandProps2;
  bandProps2.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 0 )->getProperty() ) ) );
  bandProps2.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 1 )->getProperty() ) ) );
  bandProps2.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 2 )->getProperty() ) ) );
  
  std::auto_ptr< te::rst::Raster > outDiskRasterPtr( te::rst::RasterFactory::make(
    "GDAL", new te::rst::Grid( *( diskRasterPtr->getGrid() ) ), bandProps2, 
    auxRasterInfo, 0, 0 ) );
  BOOST_CHECK( outDiskRasterPtr.get() );
  
  BOOST_CHECK( te::rp::InverseWaveletAtrous(
    *waveletRasterPtr,
    waveletDecompLevelsNumb,
    *outDiskRasterPtr,
    inputRasterBands ) );  
}

BOOST_AUTO_TEST_SUITE_END()
