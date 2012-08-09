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
  \file TsTiePointsLocator.cpp

  \brief A test suit for the TiePointsLocator interface.
 */

#include "TsTiePointsLocator.h"
#include "../Config.h"

#include <terralib/raster/RasterFactory.h>
#include <terralib/rp/TiePointsLocator.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsTiePointsLocator );

void TsTiePointsLocator::MoravecStrategySameImage()
{
  // open input raster
  
  std::map<std::string, std::string> inputRasterInfo;
  inputRasterInfo["URI"] = TE_DATA_LOCALE 
    "/data/rasters/cbers2b_rgb342_crop.tif";
    
  boost::shared_ptr< te::rst::Raster > inputRasterPointer ( te::rst::RasterFactory::open(
    inputRasterInfo ) );
  CPPUNIT_ASSERT( inputRasterPointer.get() );    
    
  // Creating the algorithm parameters
  
  te::rp::TiePointsLocator::InputParameters algoInputParams;
  algoInputParams.m_interesPointsLocationStrategy = 
    te::rp::TiePointsLocator::InputParameters::MoravecStrategyT;
  algoInputParams.m_inRaster1Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster1Bands.push_back( 1 );
  algoInputParams.m_inRaster2Ptr = inputRasterPointer.get();
  algoInputParams.m_inRaster2Bands.push_back( 1 );
  algoInputParams.m_enableMultiThread = false;
  algoInputParams.m_maxTiePoints = 100;

  te::rp::TiePointsLocator::OutputParameters algoOutputParams;

  
  // Executing the algorithm
  
  te::rp::TiePointsLocator algorithmInstance;
  
  CPPUNIT_ASSERT( algorithmInstance.initialize( algoInputParams ) );
  CPPUNIT_ASSERT( algorithmInstance.execute( algoOutputParams ) );
}

