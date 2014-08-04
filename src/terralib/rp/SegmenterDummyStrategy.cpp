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
#include "../common/progress/TaskProgress.h"

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
      const te::rp::SegmenterSegmentsBlock& block2ProcessInfo,
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,
      const std::vector< double >& inputRasterNoDataValues,
      const std::vector< double >& inputRasterGains,
      const std::vector< double >& inputRasterOffsets,
      te::rst::Raster& outputRaster,
      const unsigned int outputRasterBand,
      const bool enableProgressInterface )
      throw( te::rp::Exception )
    {
      assert( inputRasterBands.size() == inputRasterNoDataValues.size() );
      assert( inputRasterNoDataValues.size() == inputRasterGains.size() );
      assert( inputRasterGains.size() == inputRasterOffsets.size() );      
      assert( block2ProcessInfo.m_topCutOffProfile.size() == block2ProcessInfo.m_width );
      assert( block2ProcessInfo.m_bottomCutOffProfile.size() == block2ProcessInfo.m_width );
      assert( block2ProcessInfo.m_leftCutOffProfile.size() == block2ProcessInfo.m_height );
      assert( block2ProcessInfo.m_rightCutOffProfile.size() == block2ProcessInfo.m_height );      
      
      SegmenterSegmentsBlock::SegmentIdDataType segmentId = 
        segmenterIdsManager.getNewID();
      
      // Progress interface
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( enableProgressInterface )
      {
        progressPtr.reset( new te::common::TaskProgress );
        progressPtr->setTotalSteps( block2ProcessInfo.m_height );
        progressPtr->setMessage( "Segmentation" );
      }        
      
      // processing each block
      
      unsigned int blkCol = 0;
      double value = 0;
      unsigned int inputRasterBandsIdx = 0;
      const unsigned int inputRasterBandsSize = inputRasterBands.size();
      bool rasterValuesAreValid = false;
      
      for( unsigned int blkLine = 0 ; blkLine < block2ProcessInfo.m_height ; ++blkLine )
      {
        for( blkCol = 0 ; blkCol < block2ProcessInfo.m_width ; ++blkCol )
        {
          rasterValuesAreValid = true;
          
          for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < 
            inputRasterBandsSize ; ++inputRasterBandsIdx )
          {
            inputRaster.getValue( blkCol + block2ProcessInfo.m_startX, blkLine +
              block2ProcessInfo.m_startY, value, 
              inputRasterBands[ inputRasterBandsIdx ] );
              
            if( value == inputRasterNoDataValues[ inputRasterBandsIdx ] )
            {
              rasterValuesAreValid = false;
              break;
            }
          }          
          
          if( 
              rasterValuesAreValid
              &&
              ( blkLine >= block2ProcessInfo.m_topCutOffProfile[ blkCol ] )
              &&
              ( blkLine <= block2ProcessInfo.m_bottomCutOffProfile[ blkCol ] )
              &&
              ( blkCol >= block2ProcessInfo.m_leftCutOffProfile[ blkLine ] )
              &&
              ( blkCol <= block2ProcessInfo.m_rightCutOffProfile[ blkLine ] )
            )
          {
            outputRaster.setValue( blkCol + block2ProcessInfo.m_startX, 
               blkLine + block2ProcessInfo.m_startY, segmentId, outputRasterBand );
          }
        }
        
        if( enableProgressInterface )
        {
          if( ! progressPtr->isActive() ) 
          {
            return false;
          }   
          
          progressPtr->pulse();
        }
      }      
      
      return true;
    }
    
    double SegmenterDummyStrategy::getMemUsageEstimation(
      const unsigned int bandsToProcess, const unsigned int pixelsNumber ) const
    {
      return (double)( sizeof(double) * bandsToProcess * pixelsNumber );
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

