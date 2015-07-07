/*  Copyright (C) 2015 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/rp/SegmenterRegionGrowingMeanStrategy.cpp
  \briefRaster region growing segmenter Mean strategy.
*/

#include "SegmenterRegionGrowingMeanStrategy.h"
#include "SegmenterRegionGrowingMeanMerger.h"

#include "Macros.h"

#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/RasterFactory.h"
#include "../raster/Grid.h"
#include "../datatype/Enums.h"
#include "../common/progress/TaskProgress.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <limits>

#include <boost/lexical_cast.hpp>

namespace
{
  static te::rp::SegmenterRegionGrowingMeanStrategyFactory 
    segmenterRegionGrowingMeanStrategyFactoryInstance;
}

namespace te
{
  namespace rp
  {
    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingMeanStrategy::Parameters::Parameters()
    {
      reset();
    }
    
    SegmenterRegionGrowingMeanStrategy::Parameters::~Parameters()
    {
    }    
    
    const SegmenterRegionGrowingMeanStrategy::Parameters& 
    SegmenterRegionGrowingMeanStrategy::Parameters::operator=( 
      const SegmenterRegionGrowingMeanStrategy::Parameters& params )
    {
      reset();
      
      m_minSegmentSize = params.m_minSegmentSize;
      m_segmentsSimilarityThreshold = params.m_segmentsSimilarityThreshold;
      m_segmentsSimIncreaseSteps = params.m_segmentsSimIncreaseSteps;
      m_enableLocalMutualBestFitting = params.m_enableLocalMutualBestFitting;
      m_enableSameIterationMerges = params.m_enableSameIterationMerges;
      
      return *this;      
    }
    
    void SegmenterRegionGrowingMeanStrategy::Parameters::reset() 
    throw( te::rp::Exception )
    {
      m_minSegmentSize = 100;
      m_segmentsSimilarityThreshold = 0.03;
      m_segmentsSimIncreaseSteps = 2;
      m_enableLocalMutualBestFitting = false;
      m_enableSameIterationMerges = false;
    }
    
    te::common::AbstractParameters* SegmenterRegionGrowingMeanStrategy::Parameters::clone() const
    {
      return new te::rp::SegmenterRegionGrowingMeanStrategy::Parameters( *this );
    }

    SegmenterRegionGrowingMeanStrategy::SegmenterRegionGrowingMeanStrategy()
    {
      m_isInitialized = false;
    }
    
    SegmenterRegionGrowingMeanStrategy::~SegmenterRegionGrowingMeanStrategy()
    {
      reset();
    }
    
    bool SegmenterRegionGrowingMeanStrategy::initialize( 
      SegmenterStrategyParameters const* const strategyParams ) 
      throw( te::rp::Exception )
    {
      m_isInitialized = false;
      reset();
      
      SegmenterRegionGrowingMeanStrategy::Parameters const* paramsPtr = 
        dynamic_cast< SegmenterRegionGrowingMeanStrategy::Parameters const* >( strategyParams );
        
      if( paramsPtr )
      {
        m_parameters = *( paramsPtr );
        
        TERP_TRUE_OR_RETURN_FALSE( m_parameters.m_minSegmentSize > 0,
          "Invalid segmenter strategy parameter m_minSegmentSize" )
          
        TERP_TRUE_OR_RETURN_FALSE(  
          ( m_parameters.m_segmentsSimilarityThreshold >= 0.0 ),
          "Invalid segmenter strategy parameter m_segmentsSimilarityThreshold" )  
          
        m_isInitialized = true;
        
        return true;        
      } 
      else
      {
        return false;
      }
    }
    
    void SegmenterRegionGrowingMeanStrategy::reset()
    {
      m_isInitialized = false;
      m_segmentsPool.clear();
      m_segmentsIdsMatrix.reset();
      m_parameters.reset();
    }
    
    bool SegmenterRegionGrowingMeanStrategy::execute( 
      SegmenterIdsManager& segmenterIdsManager,
      const te::rp::SegmenterSegmentsBlock& block2ProcessInfo,
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,
      const std::vector< double >& inputRasterNoDataValues,
      const std::vector< double >& inputRasterBandMinValues,
      const std::vector< double >& inputRasterBandMaxValues,
      te::rst::Raster& outputRaster,
      const unsigned int outputRasterBand,
      const bool enableProgressInterface )
      throw( te::rp::Exception )
    {
      TERP_TRUE_OR_RETURN_FALSE( m_isInitialized,
        "Instance not initialized" )
        
      // Creating the merger instance
      
      std::auto_ptr< SegmenterRegionGrowingMeanMerger >
        mergerPtr( new SegmenterRegionGrowingMeanMerger( inputRasterBands.size() ) );
      
      // Initiating the segments pool
      
      const unsigned int segmentFeaturesSize = mergerPtr->getSegmentFeaturesSize();
      
      // The number of segments plus 3 (due 3 auxiliary segments
      TERP_TRUE_OR_RETURN_FALSE( m_segmentsPool.initialize( 3 + ( 
        block2ProcessInfo.m_height * block2ProcessInfo.m_width ), 
        segmentFeaturesSize ), "Segments pool initiation error" );     
        
//       {
//         // checking alignment        
//         SegmenterRegionGrowingSegment< rg::MeanFeatureType >* auxSegPtr = 0;
//         unsigned int counter = 0;
//         while( auxSegPtr = m_segmentsPool.getNextSegment() )
//         {
//           for( unsigned int featureIdx = 0 ; featureIdx < auxSegPtr->m_featuresSize ;
//             ++featureIdx )
//           {
//             auxSegPtr->m_features[ featureIdx ] = (rg::MeanFeatureType)
//             counter;
//           }
//         }
//         m_segmentsPool.resetUseCounter();
//         counter = 0;
//         while( auxSegPtr = m_segmentsPool.getNextSegment() )
//         {
//           for( unsigned int featureIdx = 0 ; featureIdx < auxSegPtr->m_featuresSize ;
//             ++featureIdx )
//           {          
//             if( auxSegPtr->m_features[ featureIdx ] != (rg::MeanFeatureType)
//               counter ) throw;
//           }
//         }        
//       }
        
      SegmenterRegionGrowingSegment< rg::MeanFeatureType >* auxSeg1Ptr = m_segmentsPool.getNextSegment();
      auxSeg1Ptr->disable();
      SegmenterRegionGrowingSegment< rg::MeanFeatureType >* auxSeg2Ptr = m_segmentsPool.getNextSegment();
      auxSeg2Ptr->disable();
      SegmenterRegionGrowingSegment< rg::MeanFeatureType >* auxSeg3Ptr = m_segmentsPool.getNextSegment();
      auxSeg3Ptr->disable();
      
      // Allocating the ids matrix
      
      if( ( m_segmentsIdsMatrix.getLinesNumber() != block2ProcessInfo.m_height ) ||
        ( m_segmentsIdsMatrix.getColumnsNumber() != block2ProcessInfo.m_width ) )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_segmentsIdsMatrix.reset( block2ProcessInfo.m_height, 
          block2ProcessInfo.m_width,
          Matrix< SegmenterSegmentsBlock::SegmentIdDataType >::RAMMemPol ),
          "Error allocating segments Ids matrix" );
      }      
        
      // Initializing segments
        
      SegmenterRegionGrowingSegment< rg::MeanFeatureType >* actSegsListHeadPtr = 0;
      
      TERP_TRUE_OR_RETURN_FALSE( initializeSegments( segmenterIdsManager,
        block2ProcessInfo, inputRaster, inputRasterBands, inputRasterNoDataValues, 
        inputRasterBandMinValues, inputRasterBandMaxValues,
        &actSegsListHeadPtr ), 
        "Segments initalization error" );    
      
      TERP_TRUE_OR_RETURN_FALSE( actSegsListHeadPtr != 0, 
         "Invalid active segments list header" );
      
      // Progress interface
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( enableProgressInterface )
      {
        progressPtr.reset( new te::common::TaskProgress );
        progressPtr->setTotalSteps( 2 + m_parameters.m_segmentsSimIncreaseSteps );
        progressPtr->setMessage( "Segmentation" );
      }          
      
      // Globals
      
      rg::MeanFeatureType minFoundDissimilarity = 0.0;
      rg::MeanFeatureType maxFoundDissimilarity = 0.0;
      unsigned int totalMergesNumber = 0;      
      rg::IterationCounterType globalMergeIterationsCounter = 1;
      
      // Initial merge of equal segments
      
      if( enableProgressInterface )
      {
        if( ! progressPtr->isActive() ) 
        {
          return false;
        }   
        progressPtr->pulse();
      }      
      
      // Merging the segments that are equal
      
//      TERP_LOGMSG( rg::getActiveSegmentsNumber< rg::MeanFeatureType >( actSegsListHeadPtr ) );
      
      rg::mergeSegments< rg::MeanFeatureType >( 
        m_segmentsIdsMatrix,
        0.0, 
        0, 
        segmenterIdsManager, 
        *mergerPtr, 
        false,
        true,
        auxSeg1Ptr, 
        auxSeg2Ptr, 
        auxSeg3Ptr, 
        minFoundDissimilarity, 
        maxFoundDissimilarity,
        totalMergesNumber,
        globalMergeIterationsCounter,
        &actSegsListHeadPtr );
      
//      TERP_LOGMSG( rg::getActiveSegmentsNumber< rg::MeanFeatureType >( actSegsListHeadPtr ) );
      
      // Main Segmentation loop
      
      for( unsigned int segmentsSimIncreaseStep = 1 ; segmentsSimIncreaseStep <=
        m_parameters.m_segmentsSimIncreaseSteps ; ++segmentsSimIncreaseStep )
      {
        const rg::MeanFeatureType disimilarityThreshold = 
          ( ((rg::MeanFeatureType)segmentsSimIncreaseStep) )
          *
          ( ((rg::MeanFeatureType)m_parameters.m_segmentsSimilarityThreshold) )
          /
          ( (rg::MeanFeatureType)( m_parameters.m_segmentsSimIncreaseSteps ) );
          
        rg::mergeSegments< rg::MeanFeatureType >( 
          m_segmentsIdsMatrix,
          disimilarityThreshold, 
          0, 
          segmenterIdsManager, 
          *mergerPtr, 
          m_parameters.m_enableLocalMutualBestFitting,
          m_parameters.m_enableSameIterationMerges,
          auxSeg1Ptr, 
          auxSeg2Ptr, 
          auxSeg3Ptr, 
          minFoundDissimilarity, 
          maxFoundDissimilarity,
          totalMergesNumber,
          globalMergeIterationsCounter,
          &actSegsListHeadPtr );
        
//        TERP_LOGMSG( rg::getActiveSegmentsNumber< rg::MeanFeatureType >( actSegsListHeadPtr ) );
        
        if( enableProgressInterface )
        {
          if( ! progressPtr->isActive() ) 
          {
            return false;
          }   
          progressPtr->pulse();
        }        
      }
      
      // Forcing the merge of too small segments
      
      if( m_parameters.m_minSegmentSize > 1 )
      {
        rg::mergeSegments< rg::MeanFeatureType >( 
          m_segmentsIdsMatrix,
          std::numeric_limits< rg::MeanFeatureType >::max(),
          m_parameters.m_minSegmentSize,
          segmenterIdsManager, 
          *mergerPtr, 
          false,
          true,
          auxSeg1Ptr, 
          auxSeg2Ptr, 
          auxSeg3Ptr, 
          minFoundDissimilarity, 
          maxFoundDissimilarity,
          totalMergesNumber,
          globalMergeIterationsCounter,
          &actSegsListHeadPtr );    
        
//        TERP_LOGMSG( rg::getActiveSegmentsNumber< rg::MeanFeatureType >( actSegsListHeadPtr ) );
      }
      
      if( enableProgressInterface )
      {      
        if( ! progressPtr->isActive() ) 
        {
          return false;
        }     
        progressPtr->pulse();    
      }      
      
      // Flush result to the output raster
      
      {
        unsigned int blkCol = 0;
        SegmenterSegmentsBlock::SegmentIdDataType* segmentsIdsLinePtr = 0;
        
        for( unsigned int blkLine = 0 ; blkLine < block2ProcessInfo.m_height ; ++blkLine )
        {
          segmentsIdsLinePtr = m_segmentsIdsMatrix[ blkLine ];
          
          for( blkCol = 0 ; blkCol < block2ProcessInfo.m_width ; ++blkCol )
          {
            if( segmentsIdsLinePtr[ blkCol ] )
            {
              outputRaster.setValue( blkCol + block2ProcessInfo.m_startX, blkLine
                + block2ProcessInfo.m_startY, segmentsIdsLinePtr[ blkCol ], 
                outputRasterBand );
            }
          }
        }
      }
      
      return true;
    }
    
    double SegmenterRegionGrowingMeanStrategy::getMemUsageEstimation(
      const unsigned int bandsToProcess,
      const unsigned int pixelsNumber ) const
    {
      TERP_TRUE_OR_THROW( m_isInitialized, "Instance not initialized" );
      
      // The features matrix inside the pool
      double featuresSizeBytes = (double)
        (
          pixelsNumber
          *
          bandsToProcess
          *
          sizeof( rg::MeanFeatureType )
        );
      
      return (double)
        (
          featuresSizeBytes
          +
          ( // The segments matrix inside the pool
            pixelsNumber 
            * 
            (
              sizeof( SegmenterRegionGrowingSegment< rg::MeanFeatureType > )
              +
              ( // An initial vector of pointers to 8 neighbors
                6
                *
                sizeof( SegmenterRegionGrowingSegment< rg::MeanFeatureType >* )
              )
            )
          )
          +
          ( // The segments IDs matrix inside the strategy
            pixelsNumber
            *
            sizeof( SegmenterSegmentsBlock::SegmentIdDataType )
          )
        );
    }
    
    unsigned int SegmenterRegionGrowingMeanStrategy::getOptimalBlocksOverlapSize() const
    {
      TERP_TRUE_OR_THROW( m_isInitialized, "Instance not initialized" );
      return (unsigned int)( std::sqrt( (double)m_parameters.m_minSegmentSize) );
    }
    
    bool SegmenterRegionGrowingMeanStrategy::initializeSegments( 
      SegmenterIdsManager& segmenterIdsManager,
      const te::rp::SegmenterSegmentsBlock& block2ProcessInfo,
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,   
      const std::vector< double >& inputRasterNoDataValues,
      const std::vector< double >& inputRasterBandMinValues,
      const std::vector< double >& inputRasterBandMaxValues,
      SegmenterRegionGrowingSegment< rg::MeanFeatureType >** actSegsListHeadPtr )
    {
      const unsigned int inputRasterBandsSize = (unsigned int)
        inputRasterBands.size();
        
      (*actSegsListHeadPtr) = 0;
        
      // calculating offset and gain
        
      std::vector< rg::MeanFeatureType > inputRasterBandGains(
        inputRasterBandMinValues.size() );
      
      {
        for( unsigned int inputRasterBandMinValuesIdx = 0 ; inputRasterBandMinValuesIdx
          < inputRasterBandMinValues.size() ; ++inputRasterBandMinValuesIdx )
        {
          inputRasterBandGains[ inputRasterBandMinValuesIdx ] = (rg::MeanFeatureType)
            ( inputRasterBandMaxValues[ inputRasterBandMinValuesIdx ] -
              inputRasterBandMinValues[ inputRasterBandMinValuesIdx ] );
            
          if( inputRasterBandGains[ inputRasterBandMinValuesIdx ] != 0.0 )
          {
            inputRasterBandGains[ inputRasterBandMinValuesIdx ] = (rg::MeanFeatureType)( 1.0 /
              inputRasterBandGains[ inputRasterBandMinValuesIdx ] );
          }
        }
      }
        
      // Initializing each segment
      
      unsigned int blkLine = 0;
      unsigned int blkCol = 0;      
      SegmenterRegionGrowingSegment< rg::MeanFeatureType >* segmentPtr = 0;
      SegmenterRegionGrowingSegment< rg::MeanFeatureType >* neighborSegmentPtr = 0;
      bool rasterValuesAreValid = true;
      unsigned int inputRasterBandsIdx = 0;
      double value = 0;
      const std::vector< double > dummyZeroesVector( inputRasterBandsSize, 0 );

      std::list< SegmenterSegmentsBlock::SegmentIdDataType > 
        unusedLineSegmentIds;
      
      std::vector< SegmenterSegmentsBlock::SegmentIdDataType > 
        lineSegmentIds;
      lineSegmentIds.reserve( block2ProcessInfo.m_width );
      
      std::vector< rg::MeanFeatureType > rasterValues;
      std::vector< rg::MeanFeatureType > rasterSquareValues;
      rasterValues.resize( inputRasterBandsSize, 0 );
      rasterSquareValues.resize( inputRasterBandsSize, 0 );
      std::vector< SegmenterRegionGrowingSegment< rg::MeanFeatureType >* > usedSegPointers1( block2ProcessInfo.m_width, 0 );
      std::vector< SegmenterRegionGrowingSegment< rg::MeanFeatureType >* > usedSegPointers2( block2ProcessInfo.m_width, 0 );
      std::vector< SegmenterRegionGrowingSegment< rg::MeanFeatureType >* >* lastLineSegsPtrs = &usedSegPointers1;
      std::vector< SegmenterRegionGrowingSegment< rg::MeanFeatureType >* >* currLineSegsPtrs = &usedSegPointers2;
      
      SegmenterRegionGrowingSegment< rg::MeanFeatureType >* prevActSegPtr = 0;
      
      unsigned int rasterValuesIdx = 0;
      
      for( blkLine = 0 ; blkLine < block2ProcessInfo.m_height ; ++blkLine )
      {
        segmenterIdsManager.getNewIDs( block2ProcessInfo.m_width, lineSegmentIds );
        
        for( blkCol = 0 ; blkCol < block2ProcessInfo.m_width ; ++blkCol )
        {
          if( 
              ( blkLine >= block2ProcessInfo.m_topCutOffProfile[ blkCol ] )
              &&
              ( blkLine <= block2ProcessInfo.m_bottomCutOffProfile[ blkCol ] )
              &&
              ( blkCol >= block2ProcessInfo.m_leftCutOffProfile[ blkLine ] )
              &&
              ( blkCol <= block2ProcessInfo.m_rightCutOffProfile[ blkLine ] )
            )
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
              else
              {
                value -= inputRasterBandMinValues[ inputRasterBandsIdx ];
                value *= inputRasterBandGains[ inputRasterBandsIdx ];
                
                rasterValues[ inputRasterBandsIdx ] = 
                  (rg::MeanFeatureType)value;
                rasterSquareValues[ inputRasterBandsIdx ] = 
                  (rg::MeanFeatureType)( value * value );
              }
            }
          }
          else
          {
            rasterValuesAreValid = false;
          }            
          
          // assotiating a segment object
          
          if( rasterValuesAreValid )
          {
            segmentPtr = m_segmentsPool.getNextSegment();
            assert( segmentPtr );                
                
            for( rasterValuesIdx = 0 ; rasterValuesIdx < inputRasterBandsSize ;
              ++rasterValuesIdx )
            {
              segmentPtr->m_features[ rasterValuesIdx ] = rasterValues[ 
                rasterValuesIdx ];
            }
            
            currLineSegsPtrs->operator[]( blkCol ) = segmentPtr;
            
            segmentPtr->m_id = lineSegmentIds[ blkCol ];
            segmentPtr->m_size = 1;
            segmentPtr->m_xStart = blkCol;
            segmentPtr->m_xBound = blkCol + 1;
            segmentPtr->m_yStart = blkLine;
            segmentPtr->m_yBound = blkLine + 1;
            segmentPtr->m_mergetIteration = 0;
            segmentPtr->m_prevActiveSegment = prevActSegPtr;
            segmentPtr->m_nextActiveSegment = 0;
            
            m_segmentsIdsMatrix( blkLine, blkCol ) = segmentPtr->m_id;
              
            // updating the neighboorhood info
            
            segmentPtr->clearNeighborSegments();
              
            if( blkLine ) 
            { 
              neighborSegmentPtr = lastLineSegsPtrs->operator[]( blkCol );
                
              if( neighborSegmentPtr )
              {
                segmentPtr->addNeighborSegment( neighborSegmentPtr );
                                    
                neighborSegmentPtr->addNeighborSegment( segmentPtr );
              }
            }
            
            if( blkCol ) 
            { 
              neighborSegmentPtr = currLineSegsPtrs->operator[]( blkCol - 1 );
                
              if( neighborSegmentPtr )
              {
                segmentPtr->addNeighborSegment( neighborSegmentPtr );
                                    
                neighborSegmentPtr->addNeighborSegment( segmentPtr );
              }
            }   
            
            // Updating the active segments list header
            
            if( (*actSegsListHeadPtr) == 0 )
            {
              (*actSegsListHeadPtr) = segmentPtr;
            }
            
            // Updating the previous active segment
            
            if( prevActSegPtr )
            {
              prevActSegPtr->m_nextActiveSegment = segmentPtr;
            }
            
            prevActSegPtr = segmentPtr;
          }
          else // !rasterValueIsValid
          {
            m_segmentsIdsMatrix( blkLine, blkCol ) = 0;
            unusedLineSegmentIds.push_back( lineSegmentIds[ blkCol ] );
            currLineSegsPtrs->operator[]( blkCol ) = 0;
          }
        }
        
        // Free unused IDs
        
        if( ! unusedLineSegmentIds.empty() )
        {
          segmenterIdsManager.addFreeIDs( unusedLineSegmentIds );
          unusedLineSegmentIds.clear();
        }
        
        // Swapping the pointers to the vectors of used segment pointers
        
        if( lastLineSegsPtrs == ( &usedSegPointers1 ) )
        {
          lastLineSegsPtrs = &usedSegPointers2;
          currLineSegsPtrs = &usedSegPointers1;
        }
        else
        {
          lastLineSegsPtrs = &usedSegPointers1;
          currLineSegsPtrs = &usedSegPointers2;
        }
      }
      
      return true;
    }
    
    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingMeanStrategyFactory::SegmenterRegionGrowingMeanStrategyFactory()
    : te::rp::SegmenterStrategyFactory( "RegionGrowingMean" )
    {
    }
    
    SegmenterRegionGrowingMeanStrategyFactory::~SegmenterRegionGrowingMeanStrategyFactory()
    {
    }    
    
    te::rp::SegmenterStrategy* SegmenterRegionGrowingMeanStrategyFactory::build()
    {
      return new te::rp::SegmenterRegionGrowingMeanStrategy();
    }
    
  } // end namespace rp
}   // end namespace te    

