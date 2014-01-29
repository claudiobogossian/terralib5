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
  
  auxRasterInfo["URI"] = TE_DATA_DIR "/data/rasters/cbers2b_rgb342_crop.tif";
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
  
  CPPUNIT_ASSERT( te::rp::ConvertRBG2IHS( *diskRasterPtr, 0, 1, 2, 0, 255, *ihsRasterPtr ) );
  
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
  
  auxRasterInfo["URI"] = "terralib_unittest_rp_functions_ihs.tif";
  
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
  
  auxRasterInfo["URI"] = TE_DATA_DIR "/data/rasters/cbers2b_hrc_crop.tif";
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
  
  auxRasterInfo["URI"] = TE_DATA_DIR "/data/rasters/cbers2b_hrc_crop.tif";
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




