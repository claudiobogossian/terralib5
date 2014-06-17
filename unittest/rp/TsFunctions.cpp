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
  \file TsFunctions.cpp

  \brief A test suit for raster processing Functions.
 */

#include "TsFunctions.h"
#include "../Config.h"

#include <terralib/rp/Functions.h>
#include <terralib/raster/Grid.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/raster/BandProperty.h>
#include <terralib/raster/Band.h>

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

#include <memory>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION( TsFunctions );

void TsFunctions::ihs()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( diskRasterPtr.get() );  
  
  auxRasterInfo.clear();
  auxRasterInfo["MEM_RASTER_NROWS"] = boost::lexical_cast< std::string >( 
    diskRasterPtr->getNumberOfRows() );
  auxRasterInfo["MEM_RASTER_NCOLS"] = boost::lexical_cast< std::string >( 
    diskRasterPtr->getNumberOfColumns() );
  auxRasterInfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast< std::string >((int)te::dt::DOUBLE_TYPE);
  auxRasterInfo["MEM_RASTER_NBANDS"] = "3";
   
  std::auto_ptr< te::rst::Raster > ihsRasterPtr( te::rst::RasterFactory::make(
    "MEM", 0, std::vector<te::rst::BandProperty*>(), auxRasterInfo) );
  CPPUNIT_ASSERT( ihsRasterPtr.get() );
  
  CPPUNIT_ASSERT( te::rp::ConvertRGB2IHS( *diskRasterPtr, 0, 1, 2, 0, 255, *ihsRasterPtr ) );
  
  CPPUNIT_ASSERT( te::rp::Copy2DiskRaster( *ihsRasterPtr, "terralib_unittest_rp_functions_rgb2ihs.tif" ) ) ;
  
  auxRasterInfo.clear();
  auxRasterInfo["MEM_RASTER_NROWS"] = boost::lexical_cast< std::string >( 
    diskRasterPtr->getNumberOfRows() );
  auxRasterInfo["MEM_RASTER_NCOLS"] = boost::lexical_cast< std::string >( 
    diskRasterPtr->getNumberOfColumns() );
  auxRasterInfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast< std::string >((int)te::dt::UCHAR_TYPE);
  auxRasterInfo["MEM_RASTER_NBANDS"] = "3";
   
  std::auto_ptr< te::rst::Raster > rgbRasterPtr( te::rst::RasterFactory::make(
    "MEM", 0, std::vector<te::rst::BandProperty*>(), auxRasterInfo) );
  CPPUNIT_ASSERT( rgbRasterPtr.get() );  
  
  CPPUNIT_ASSERT( te::rp::ConvertIHS2RGB( *ihsRasterPtr, 0, 1, 2, 0, 255, *rgbRasterPtr ) );
  
  std::vector< te::rst::BandProperty * > bandProps;
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 0 )->getProperty() ) ) );
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 1 )->getProperty() ) ) );
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 2 )->getProperty() ) ) );
  
  auxRasterInfo["URI"] = "terralib_unittest_rp_functions_ihs2rgb.tif";
  
  std::auto_ptr< te::rst::Raster > outDiskRasterPtr( te::rst::RasterFactory::make(
    "GDAL", new te::rst::Grid( *( diskRasterPtr->getGrid() ) ), bandProps, 
    auxRasterInfo, 0, 0 ) );
  CPPUNIT_ASSERT( outDiskRasterPtr.get() );
  
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

void TsFunctions::getMeanValue()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( diskRasterPtr.get() );  
  
  double meanValue = 0;
  
//  boost::timer timer;
  CPPUNIT_ASSERT( te::rp::GetMeanValue( *diskRasterPtr->getBand( 0 ), 1, meanValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 181.340256531345, meanValue, 0.0001 );  
}

void TsFunctions::getMeanValueOptimized()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( diskRasterPtr.get() );  
  
  double meanValue = 0;
  
//  boost::timer timer;
  CPPUNIT_ASSERT( te::rp::GetMeanValue( *diskRasterPtr->getBand( 0 ), 4, meanValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 181.340256531345, meanValue, 0.0001 );  
}

void TsFunctions::getCovarianceValue()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( diskRasterPtr.get() );  
  
  double covarianceValue = 0;
  
//  boost::timer timer;
  CPPUNIT_ASSERT( te::rp::GetCovarianceValue( *diskRasterPtr->getBand( 0 ), 
    *diskRasterPtr->getBand( 0 ), 1, 0, 0, covarianceValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2143.89743610679, covarianceValue, 0.0001 );  
}

void TsFunctions::getCovarianceValueOptimized()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( diskRasterPtr.get() );  
  
  double covarianceValue = 0;
  
//  boost::timer timer;
  CPPUNIT_ASSERT( te::rp::GetCovarianceValue( *diskRasterPtr->getBand( 0 ), 
    *diskRasterPtr->getBand( 0 ), 4, 0, 0, covarianceValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2143.89743610679, covarianceValue, 0.0001 );  
}

void TsFunctions::PrincipalComponents()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( diskRasterPtr.get() );  
  
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
  CPPUNIT_ASSERT( pcaRasterPtr.get() );  
  
  std::vector< unsigned int > inputRasterBands;
  inputRasterBands.push_back( 0 );
  inputRasterBands.push_back( 1 );
  inputRasterBands.push_back( 2 );
  
  boost::numeric::ublas::matrix< double > pcaMatrix;
  
  CPPUNIT_ASSERT( te::rp::DirectPrincipalComponents( 
    *diskRasterPtr, 
    inputRasterBands,
    pcaMatrix,
    *pcaRasterPtr,
    1 ) );
    
  CPPUNIT_ASSERT( te::rp::Copy2DiskRaster( *pcaRasterPtr, "terralib_unittest_rp_functions_DirectPrincipalComponents.tif" ) ) ;  
  
  auxRasterInfo.clear();
  auxRasterInfo["URI"] = "terralib_unittest_rp_functions_InversePrincipalComponents.tif";
  
  std::vector< te::rst::BandProperty * > bandProps2;
  bandProps2.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 0 )->getProperty() ) ) );
  bandProps2.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 1 )->getProperty() ) ) );
  bandProps2.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 2 )->getProperty() ) ) );
  
  std::auto_ptr< te::rst::Raster > outDiskRasterPtr( te::rst::RasterFactory::make(
    "GDAL", new te::rst::Grid( *( diskRasterPtr->getGrid() ) ), bandProps2, 
    auxRasterInfo, 0, 0 ) );
  CPPUNIT_ASSERT( outDiskRasterPtr.get() );
  
  CPPUNIT_ASSERT( te::rp::InversePrincipalComponents( 
    *pcaRasterPtr,
    pcaMatrix,
    *outDiskRasterPtr,
    1 ) );  
}


void TsFunctions::PrincipalComponentsOptimized()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_rgb342_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( diskRasterPtr.get() );  
  
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
  CPPUNIT_ASSERT( pcaRasterPtr.get() );   
  
  std::vector< unsigned int > inputRasterBands;
  inputRasterBands.push_back( 0 );
  inputRasterBands.push_back( 1 );
  inputRasterBands.push_back( 2 );
  
  boost::numeric::ublas::matrix< double > pcaMatrix;
  
  CPPUNIT_ASSERT( te::rp::DirectPrincipalComponents( 
    *diskRasterPtr, 
    inputRasterBands,
    pcaMatrix,
    *pcaRasterPtr,
    4 ) );
    
  CPPUNIT_ASSERT( te::rp::Copy2DiskRaster( *pcaRasterPtr, "terralib_unittest_rp_functions_DirectPrincipalComponentsOptimized.tif" ) ) ;  
  
  std::vector< te::rst::BandProperty * > bandProps;
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 0 )->getProperty() ) ) );
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 1 )->getProperty() ) ) );
  bandProps.push_back( new te::rst::BandProperty( *( diskRasterPtr->getBand( 2 )->getProperty() ) ) );
  
  auxRasterInfo.clear();
  auxRasterInfo["URI"] = "terralib_unittest_rp_functions_InversePrincipalComponentsOptimized.tif";
  
  std::auto_ptr< te::rst::Raster > outDiskRasterPtr( te::rst::RasterFactory::make(
    "GDAL", new te::rst::Grid( *( diskRasterPtr->getGrid() ) ), bandProps, 
    auxRasterInfo, 0, 0 ) );
  CPPUNIT_ASSERT( outDiskRasterPtr.get() );
  
  CPPUNIT_ASSERT( te::rp::InversePrincipalComponents( 
    *pcaRasterPtr,
    pcaMatrix,
    *outDiskRasterPtr,
    4 ) );  
}

void TsFunctions::GetStdDevValue()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( diskRasterPtr.get() );  
  
  double stdDevValue = 0;
  
//  boost::timer timer;
  CPPUNIT_ASSERT( te::rp::GetStdDevValue( *diskRasterPtr->getBand( 0 ), 1, 0, stdDevValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 46.302240075, stdDevValue, 0.0001 );  
}

void TsFunctions::GetStdDevValueOptimized()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers2b_hrc_crop.tif";
  std::auto_ptr< te::rst::Raster > diskRasterPtr( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( diskRasterPtr.get() );  
  
  double stdDevValue = 0;
  
//  boost::timer timer;
  CPPUNIT_ASSERT( te::rp::GetStdDevValue( *diskRasterPtr->getBand( 0 ), 4, 0, stdDevValue) );
//  std::cout << std::endl << "Elapsed:" << timer.elapsed() << std::endl;
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 46.302240075, stdDevValue, 0.0001 );  
}

