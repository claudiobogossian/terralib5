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
  \file terralib/rp/SegmenterDummyStrategy.cpp
  \briefRaster dummy strategy.
*/

#include "SegmenterDummyStrategy.h"

namespace
{
  static te::rp::SegmenterDummyStrategyFactory 
    segmenterDummyStrategyFactoryInstance;
}

namespace te
{
  namespace rp
  {
    
    SegmenterDummyStrategy::SegmenterDummyStrategy()
    {
    }
    
    SegmenterDummyStrategy::~SegmenterDummyStrategy()
    {
    }    
    
    bool SegmenterDummyStrategy::initialize( 
      SegmenterStrategyParameters const* const strategyParams ) 
      throw( te::rp::Exception )
    {
      return true;        
    }
    
    void SegmenterDummyStrategy::reset()
    {
    }
    
    bool SegmenterDummyStrategy::execute( 
      SegmenterIdsManager& segmenterIdsManager,
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,
      te::rst::Raster& outputRaster,
      const unsigned int outputRasterBand,
      const bool enableProgressInterface )
      throw( te::rp::Exception )
    {
      SegmenterSegmentsBlock::SegmentIdDataType segmentId = 
        segmenterIdsManager.getNewID();
      
      const unsigned int nLines = outputRaster.getNumberOfRows();
      const unsigned int nCols = outputRaster.getNumberOfColumns();
      unsigned int col = 0;
      
      for( unsigned int line = 0 ; line < nLines ; ++line )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          outputRaster.setValue( col, line, segmentId, 0 );
        }
      }      
      
      return true;
    }
    
    double SegmenterDummyStrategy::getMemUsageFactor(
      const unsigned int inputRasterBandsNumber ) const
    {
      return 1.0;
    }
    
    unsigned int SegmenterDummyStrategy::getOptimalBlocksOverlapSize() const
    {
      return 10;
    }
    
    SegmenterDummyStrategyFactory::SegmenterDummyStrategyFactory()
    : te::rp::SegmenterStrategyFactory( "Dummy" )
    {
    }
    
    SegmenterDummyStrategyFactory::~SegmenterDummyStrategyFactory()
    {
    }    
    
    te::rp::SegmenterStrategy* SegmenterDummyStrategyFactory::build()
    {
      return new te::rp::SegmenterDummyStrategy();
    }
    
  } // end namespace rp
}   // end namespace te    

