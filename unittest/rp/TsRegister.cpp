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
  \file TsRegister.cpp

  \brief A test suit for the Resiter interface.
 */

#include "TsRegister.h"
#include "../Config.h"

#include <terralib/rp/Register.h>
#include <terralib/raster/Grid.h>
#include <terralib/raster/Raster.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/geometry/Envelope.h>

#include <boost/shared_ptr.hpp>

#include <cmath>

CPPUNIT_TEST_SUITE_REGISTRATION( TsRegister );

#define ASSERTMAXDIST( pt1, pt2, maxDist ) \
{ \
  const double diffx = pt1.x - pt2.x; \
  const double diffy = pt1.y - pt2.y; \
  const double error = std::sqrt( ( diffx * diffx ) + ( diffy * diffy ) ); \
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, error, maxDist ); \
}

void TsRegister::Test001()
{
  // openning input raster
  
  std::map<std::string, std::string> auxRasterInfo;
  
  auxRasterInfo["URI"] = TERRALIB_DATA_DIR "/rasters/cbers_rgb342_crop1.tif";
  boost::shared_ptr< te::rst::Raster > inputRasterPtrPointer ( te::rst::RasterFactory::open(
    auxRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPtrPointer.get() );    
    
  // Creating the algorithm parameters
  
  te::rp::Register::InputParameters algoInputParams;
  
  algoInputParams.m_inputRasterPtr = inputRasterPtrPointer.get();
  
  algoInputParams.m_inputRasterBands.push_back( 0 );
  algoInputParams.m_inputRasterBands.push_back( 1 );
  algoInputParams.m_inputRasterBands.push_back( 2 );
  
  te::gm::GTParameters::TiePoint tiePoint1;
  tiePoint1.first.x = 0;
  tiePoint1.first.y = 0;
  inputRasterPtrPointer->getGrid()->gridToGeo( tiePoint1.first.x, tiePoint1.first.y, 
    tiePoint1.second.x, tiePoint1.second.y );
  algoInputParams.m_tiePoints.push_back( tiePoint1 );
  
  te::gm::GTParameters::TiePoint tiePoint2;
  tiePoint2.first.x = (double)( inputRasterPtrPointer->getNumberOfColumns() - 1.0 );
  tiePoint2.first.y = 0;
  inputRasterPtrPointer->getGrid()->gridToGeo( tiePoint2.first.x, tiePoint2.first.y, 
    tiePoint2.second.x, tiePoint2.second.y );
  algoInputParams.m_tiePoints.push_back( tiePoint2 );
  
  te::gm::GTParameters::TiePoint tiePoint3;
  tiePoint3.first.x = (double)( inputRasterPtrPointer->getNumberOfColumns() - 1.0 );
  tiePoint3.first.y = (double)( inputRasterPtrPointer->getNumberOfRows() - 1.0 );
  inputRasterPtrPointer->getGrid()->gridToGeo( tiePoint3.first.x, tiePoint3.first.y, 
    tiePoint3.second.x, tiePoint3.second.y );
  algoInputParams.m_tiePoints.push_back( tiePoint3 );

  te::gm::GTParameters::TiePoint tiePoint4;
  tiePoint4.first.x = 0;
  tiePoint4.first.y = (double)( inputRasterPtrPointer->getNumberOfRows() - 1.0 );
  inputRasterPtrPointer->getGrid()->gridToGeo( tiePoint4.first.x, tiePoint4.first.y, 
    tiePoint4.second.x, tiePoint4.second.y );
  algoInputParams.m_tiePoints.push_back( tiePoint4 );
  
  algoInputParams.m_outputSRID = inputRasterPtrPointer->getSRID();
  
  algoInputParams.m_outputResolutionX = inputRasterPtrPointer->getResolutionX();
  algoInputParams.m_outputResolutionY = inputRasterPtrPointer->getResolutionY();
  
  algoInputParams.m_interpMethod = te::rst::NearestNeighbor;
  
  algoInputParams.m_noDataValue = 0;
  
  algoInputParams.m_geomTransfName = "Affine";

  te::rp::Register::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =  
    "terralib_unittest_rp_Register_Test001.tif";  
  algoOutputParams.m_rType = "GDAL";
  
  // Executing the algorithm
  
  te::rp::Register algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( inputRasterPtrPointer->getResolutionX(), 
    algoOutputParams.m_outputRasterPtr->getResolutionX(), 0.00001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( inputRasterPtrPointer->getResolutionY(), 
    algoOutputParams.m_outputRasterPtr->getResolutionY(), 0.00001 );
    
  ASSERTMAXDIST( inputRasterPtrPointer->getGrid()->getExtent()->getLowerLeft(),
    algoOutputParams.m_outputRasterPtr->getGrid()->getExtent()->getLowerLeft(),
    0.000001 );
  ASSERTMAXDIST( inputRasterPtrPointer->getGrid()->getExtent()->getUpperRight(),
    algoOutputParams.m_outputRasterPtr->getGrid()->getExtent()->getUpperRight(),
    0.000001 );
}
