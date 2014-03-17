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
  \file TsGeometricRefining.cpp

  \brief A test suit for the GeometricRefining interface.
 */

#include "TsGeometricRefining.h"
#include "../Config.h"
#include <terralib/raster/Grid.h>
#include <terralib/rp/GeometricRefining.h>
#include <terralib/raster/RasterFactory.h>
#include <terralib/common/progress/ConsoleProgressViewer.h>
#include <terralib/common/progress/ProgressManager.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsGeometricRefining );

#define ASSERTMAXDIST( pt1, pt2, maxDist ) \
{ \
  const double diffx = pt1.x - pt2.x; \
  const double diffy = pt1.y - pt2.y; \
  const double error = std::sqrt( ( diffx * diffx ) + ( diffy * diffy ) ); \
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, error, maxDist ); \
}

void TsGeometricRefining::Test001()
{
  // Progress interface
  te::common::ConsoleProgressViewer progressViewerInstance;
  int viewerId = te::common::ProgressManager::getInstance().addViewer( &progressViewerInstance );  
  
  // openning input rasters
  
  std::map<std::string, std::string> inRasterInfo;
  inRasterInfo["URI"] = TE_DATA_DIR "/data/rasters/cbers2b_hrc_crop.tif";
  boost::shared_ptr< te::rst::Raster > inRasterPointer ( te::rst::RasterFactory::open(
    inRasterInfo ) );
  CPPUNIT_ASSERT( inRasterPointer.get() );
  
  std::vector< std::string > referenceRastersTypes( 5, "GDAL" );
  std::vector< std::map< std::string, std::string > >  referenceRastersInfos( 5 );
  referenceRastersInfos[ 0 ]["URI"] = TE_DATA_DIR "/data/rasters/cbers2b_hrc_crop_01.tif";
  referenceRastersInfos[ 1 ]["URI"] = TE_DATA_DIR "/data/rasters/cbers2b_hrc_crop_02.tif";
  referenceRastersInfos[ 2 ]["URI"] = TE_DATA_DIR "/data/rasters/cbers2b_hrc_crop_03.tif";
  referenceRastersInfos[ 3 ]["URI"] = TE_DATA_DIR "/data/rasters/cbers2b_hrc_crop_04.tif";
  referenceRastersInfos[ 4 ]["URI"] = TE_DATA_DIR "/data/rasters/cbers2b_hrc_crop_05.tif";
  te::rp::FeederConstRasterInfo referenceRastersfeeder( referenceRastersTypes,
    referenceRastersInfos );                                                        
    
  // Creating the algorithm parameters
  
  std::vector< unsigned int > referenceTPLocationBands( 1, 0 );
  
  te::rp::GeometricRefining::InputParameters algoInputParams;
  
  algoInputParams.m_inRasterPtr = inRasterPointer.get();
  algoInputParams.m_inRasterBands2Process.push_back( 0 );
  algoInputParams.m_inRasterTPLocationBands.push_back( 0 );
  algoInputParams.m_referenceRastersPtr = &referenceRastersfeeder;
  algoInputParams.m_referenceTPLocationBands.resize( 5, referenceTPLocationBands );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_enableProgress = true;
  algoInputParams.m_interpMethod = te::rst::Interpolator::NearestNeighbor;
  algoInputParams.m_locatorParams.m_interesPointsLocationStrategy = te::rp::TiePointsLocator::InputParameters::MoravecStrategyT;
  algoInputParams.m_minInRasterCoveredAreaPercent = 25;
  algoInputParams.m_minrReferenceRasterCoveredAreaPercent = 25;
  algoInputParams.m_inRasterSubSectorsFactor = 1;
  algoInputParams.m_inRasterExpectedRowError = 10;
  algoInputParams.m_inRasterExpectedColError = 10;
  algoInputParams.m_inRasterExpectedRowDisplacement = 0;
  algoInputParams.m_inRasterExpectedColDisplacement = 0;
  algoInputParams.m_processAllReferenceRasters = true;
  algoInputParams.m_enableRasterCache = true;
  algoInputParams.m_geomTransfName = "Affine";
  algoInputParams.m_geomTransfMaxTiePointError = 1.0;
  algoInputParams.m_outliersRemotionAssurance = 0.1;
  algoInputParams.m_outliersRemotionMaxIterations = 0;
  algoInputParams.m_outputNoDataValue = 0;
  
  te::rp::GeometricRefining::OutputParameters algoOutputParams;
  
  algoOutputParams.m_rInfo["URI"] =  
    "terralib_unittest_rp_GeometricRefining_Test001.tif";  
  algoOutputParams.m_rType = "GDAL";
  
  // Executing the algorithm
  
  te::rp::GeometricRefining algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( inRasterPointer->getResolutionX(), 
    algoOutputParams.m_outputRasterPtr->getResolutionX(), 0.00001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( inRasterPointer->getResolutionY(), 
    algoOutputParams.m_outputRasterPtr->getResolutionY(), 0.00001 );
    
  ASSERTMAXDIST( inRasterPointer->getGrid()->getExtent()->getLowerLeft(),
    algoOutputParams.m_outputRasterPtr->getGrid()->getExtent()->getLowerLeft(),
    0.000001 );
  ASSERTMAXDIST( inRasterPointer->getGrid()->getExtent()->getUpperRight(),
    algoOutputParams.m_outputRasterPtr->getGrid()->getExtent()->getUpperRight(),
    0.000001 );
  
  te::common::ProgressManager::getInstance().removeViewer( viewerId );  
  
}




