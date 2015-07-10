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
  \file terralib/rp/SegmenterRegionGrowingBaatzStrategy.cpp
  \briefRaster region growing segmenter Baatz strategy.
*/

#include "SegmenterRegionGrowingBaatzStrategy.h"
#include "SegmenterRegionGrowingBaatzMerger.h"

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

// Baatz Edge Lengh
#define BAATZ_EL( featPtr ) featPtr[ 0 ]

// Baatz Compactness
#define BAATZ_CO( featPtr ) featPtr[ 1 ]

// Baatz Smoothness
#define BAATZ_SM( featPtr ) featPtr[ 2 ]

// Baatz sums
#define BAATZ_SU( featPtr, band ) featPtr[ 3 + band ]

// Baatz square sums
#define BAATZ_SS( featPtr, bandsNmb, band ) featPtr[ 3 + bandsNmb + band ]

// Baatz stddev
#define BAATZ_ST( featPtr, bandsNmb, band ) featPtr[ 3 + ( 2 * bandsNmb ) + band ]

// Baatz stddev start ponter
#define BAATZ_STSTARTPTR( featPtr, bandsNmb ) ( featPtr + 3 + ( 2 * bandsNmb ) )

namespace
{
  static te::rp::SegmenterRegionGrowingBaatzStrategyFactory 
    segmenterRegionGrowingBaatzStrategyFactoryInstance;
}

namespace te
{
  namespace rp
  {
    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingBaatzStrategy::Parameters::Parameters()
    {
      reset();
    }
    
    SegmenterRegionGrowingBaatzStrategy::Parameters::~Parameters()
    {
    }    
    
    const SegmenterRegionGrowingBaatzStrategy::Parameters& 
    SegmenterRegionGrowingBaatzStrategy::Parameters::operator=( 
      const SegmenterRegionGrowingBaatzStrategy::Parameters& params )
    {
      reset();
      
      m_minSegmentSize = params.m_minSegmentSize;
      m_segmentsSimilarityThreshold = params.m_segmentsSimilarityThreshold;
      m_bandsWeights = params.m_bandsWeights;
      m_colorWeight = params.m_colorWeight;
      m_compactnessWeight = params.m_compactnessWeight;
      m_segmentsSimIncreaseSteps = params.m_segmentsSimIncreaseSteps;
      m_enableLocalMutualBestFitting = params.m_enableLocalMutualBestFitting;
      m_enableSameIterationMerges = params.m_enableSameIterationMerges;
      
      return *this;      
    }
    
    void SegmenterRegionGrowingBaatzStrategy::Parameters::reset() 
    throw( te::rp::Exception )
    {
      m_minSegmentSize = 100;
      m_segmentsSimilarityThreshold = 0.03;
      m_bandsWeights.clear();
      m_colorWeight = 0.9;
      m_compactnessWeight = 0.5;
      m_segmentsSimIncreaseSteps = 2;
      m_enableLocalMutualBestFitting = false;
      m_enableSameIterationMerges = false;
    }
    
    te::common::AbstractParameters* SegmenterRegionGrowingBaatzStrategy::Parameters::clone() const
    {
      return new te::rp::SegmenterRegionGrowingBaatzStrategy::Parameters( *this );
    }

    SegmenterRegionGrowingBaatzStrategy::SegmenterRegionGrowingBaatzStrategy()
    {
      m_isInitialized = false;
    }
    
    SegmenterRegionGrowingBaatzStrategy::~SegmenterRegionGrowingBaatzStrategy()
    {
      reset();
    }
    
    bool SegmenterRegionGrowingBaatzStrategy::initialize( 
      SegmenterStrategyParameters const* const strategyParams ) 
      throw( te::rp::Exception )
    {
      m_isInitialized = false;
      reset();
      
      SegmenterRegionGrowingBaatzStrategy::Parameters const* paramsPtr = 
        dynamic_cast< SegmenterRegionGrowingBaatzStrategy::Parameters const* >( strategyParams );
        
      if( paramsPtr )
      {
        m_parameters = *( paramsPtr );
        
        TERP_TRUE_OR_RETURN_FALSE( m_parameters.m_minSegmentSize > 0,
          "Invalid segmenter strategy parameter m_minSegmentSize" )
          
        TERP_TRUE_OR_RETURN_FALSE(  
          ( m_parameters.m_segmentsSimilarityThreshold >= 0.0 ),
          "Invalid segmenter strategy parameter m_segmentsSimilarityThreshold" )  
          
        TERP_TRUE_OR_RETURN_FALSE( !m_parameters.m_bandsWeights.empty(),
           "Invalid segmenter strategy parameter m_bandsWeights" );
                                     
        double bandsWeightsSum = 0;
        unsigned int bandsWeightsIdx = 0 ;
        for( bandsWeightsIdx = 0 ; bandsWeightsIdx < 
          m_parameters.m_bandsWeights.size() ; ++bandsWeightsIdx )
        {
          TERP_TRUE_OR_RETURN_FALSE( 
            m_parameters.m_bandsWeights[ bandsWeightsIdx ] >= 0.0,
             "Invalid segmenter strategy parameter m_bandsWeights" );            
          bandsWeightsSum += m_parameters.m_bandsWeights[ bandsWeightsIdx ];
        }
        TERP_TRUE_OR_RETURN_FALSE( bandsWeightsSum != 0.0,
          "Invalid segmenter strategy parameter m_bandsWeights" );        
        for( bandsWeightsIdx = 0 ; bandsWeightsIdx < 
          m_parameters.m_bandsWeights.size() ; ++bandsWeightsIdx )
        {
          m_parameters.m_bandsWeights[ bandsWeightsIdx ] /= bandsWeightsSum;
        }
         
        m_isInitialized = true;
        
        return true;        
      } 
      else
      {
        return false;
      }
    }
    
    void SegmenterRegionGrowingBaatzStrategy::reset()
    {
      m_isInitialized = false;
      m_segmentsPool.clear();
      m_segmentsIdsMatrix.reset();
      m_parameters.reset();
    }
    
    bool SegmenterRegionGrowingBaatzStrategy::execute( 
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
        
      // Updating the bands weights info, if needed
      
      if( m_parameters.m_bandsWeights.empty() )
        m_parameters.m_bandsWeights.resize( inputRasterBands.size(), 1.0 /
        ((double)inputRasterBands.size()) );
        
      // Creating the merger instance
      
      std::auto_ptr< SegmenterRegionGrowingBaatzMerger >
        mergerPtr( new SegmenterRegionGrowingBaatzMerger( m_parameters.m_colorWeight,
            m_parameters.m_compactnessWeight, m_parameters.m_bandsWeights,
            m_segmentsIdsMatrix ) );
      
      // Initiating the segments pool
      
      const unsigned int segmentFeaturesSize = mergerPtr->getSegmentFeaturesSize();
      
      // The number of segments plus 3 (due 3 auxiliary segments
      TERP_TRUE_OR_RETURN_FALSE( m_segmentsPool.initialize( 3 + ( 
        block2ProcessInfo.m_height * block2ProcessInfo.m_width ), 
        segmentFeaturesSize ), "Segments pool initiation error" );     
        
//       {
//         // checking alignment        
//         SegmenterRegionGrowingSegment* auxSegPtr = 0;
//         unsigned int counter = 0;
//         while( auxSegPtr = m_segmentsPool.getNextSegment() )
//         {
//           for( unsigned int featureIdx = 0 ; featureIdx < auxSegPtr->m_featuresSize ;
//             ++featureIdx )
//           {
//             auxSegPtr->m_features[ featureIdx ] = (SegmenterRegionGrowingSegment::FeatureType)
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
//             if( auxSegPtr->m_features[ featureIdx ] != (SegmenterRegionGrowingSegment::FeatureType)
//               counter ) throw;
//           }
//         }        
//       }
        
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* auxSeg1Ptr = m_segmentsPool.getNextSegment();
      auxSeg1Ptr->disable();
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* auxSeg2Ptr = m_segmentsPool.getNextSegment();
      auxSeg2Ptr->disable();
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* auxSeg3Ptr = m_segmentsPool.getNextSegment();
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
        
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* actSegsListHeadPtr = 0;
      
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
      
      rg::BaatzFeatureType minFoundDissimilarity = 0.0;
      rg::BaatzFeatureType maxFoundDissimilarity = 0.0;
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
      
//      TERP_LOGMSG( rg::getActiveSegmentsNumber< rg::BaatzFeatureType >( actSegsListHeadPtr ) );
      
      rg::mergeSegments< rg::BaatzFeatureType >(
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
      
//      TERP_LOGMSG( rg::getActiveSegmentsNumber< rg::BaatzFeatureType >( actSegsListHeadPtr ) );
      
      // Main Segmentation loop
      
      for( unsigned int segmentsSimIncreaseStep = 1 ; segmentsSimIncreaseStep <=
        m_parameters.m_segmentsSimIncreaseSteps ; ++segmentsSimIncreaseStep )
      {
        const rg::BaatzFeatureType disimilarityThreshold = 
          ( ((rg::BaatzFeatureType)segmentsSimIncreaseStep) )
          *
          ( ((rg::BaatzFeatureType)m_parameters.m_segmentsSimilarityThreshold) )
          /
          ( (rg::BaatzFeatureType)( m_parameters.m_segmentsSimIncreaseSteps ) );
          
        rg::mergeSegments< rg::BaatzFeatureType >( 
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
        
//        TERP_LOGMSG( rg::getActiveSegmentsNumber< rg::BaatzFeatureType >( actSegsListHeadPtr ) );
        
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
        rg::mergeSegments< rg::BaatzFeatureType >( 
          m_segmentsIdsMatrix,
          std::numeric_limits< rg::BaatzFeatureType >::max(),
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
        
//        TERP_LOGMSG( rg::getActiveSegmentsNumber< rg::BaatzFeatureType >( actSegsListHeadPtr ) );
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
    
    double SegmenterRegionGrowingBaatzStrategy::getMemUsageEstimation(
      const unsigned int bandsToProcess,
      const unsigned int pixelsNumber ) const
    {
      TERP_TRUE_OR_THROW( m_isInitialized, "Instance not initialized" );
      
      // The features matrix inside the pool
      double featuresSizeBytes = (double)
        (
          pixelsNumber
          *
          (
            (
              3
              *
              sizeof( rg::BaatzFeatureType )
            )
            +
            (
              3
              *
              bandsToProcess
              *
              sizeof( rg::BaatzFeatureType )
            )              
          )
        );
      
      return (double)
        (
          featuresSizeBytes
          +
          ( // The segments matrix inside the pool
            pixelsNumber 
            * 
            (
              sizeof( SegmenterRegionGrowingSegment< rg::BaatzFeatureType > )
              +
              ( // An initial vector of pointers to 8 neighbors
                6
                *
                sizeof( SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* )
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
    
    unsigned int SegmenterRegionGrowingBaatzStrategy::getOptimalBlocksOverlapSize() const
    {
      TERP_TRUE_OR_THROW( m_isInitialized, "Instance not initialized" );
      return (unsigned int)( std::sqrt( (double)m_parameters.m_minSegmentSize) );
    }
    
    bool SegmenterRegionGrowingBaatzStrategy::shouldComputeMinMaxValues() const
    {
      return true;
    }

    SegmenterStrategy::BlocksMergingMethod
      SegmenterRegionGrowingBaatzStrategy::getBlocksMergingMethod() const
    {
      return SegmenterStrategy::BlocksMergingMethod::GradientMerging;
    }

    bool SegmenterRegionGrowingBaatzStrategy::initializeSegments( 
      SegmenterIdsManager& segmenterIdsManager,
      const te::rp::SegmenterSegmentsBlock& block2ProcessInfo,
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,   
      const std::vector< double >& inputRasterNoDataValues,
      const std::vector< double >& inputRasterBandMinValues,
      const std::vector< double >& inputRasterBandMaxValues,
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType >** actSegsListHeadPtr )
    {
      const unsigned int inputRasterBandsSize = (unsigned int)
        inputRasterBands.size();
        
      (*actSegsListHeadPtr) = 0;
        
      // calculating offset and gain
        
      std::vector< rg::BaatzFeatureType > inputRasterBandGains(
        inputRasterBandMinValues.size() );
      
      {
        for( unsigned int inputRasterBandMinValuesIdx = 0 ; inputRasterBandMinValuesIdx
          < inputRasterBandMinValues.size() ; ++inputRasterBandMinValuesIdx )
        {
          inputRasterBandGains[ inputRasterBandMinValuesIdx ] = (rg::BaatzFeatureType)
            ( inputRasterBandMaxValues[ inputRasterBandMinValuesIdx ] -
              inputRasterBandMinValues[ inputRasterBandMinValuesIdx ] );
            
          if( inputRasterBandGains[ inputRasterBandMinValuesIdx ] != 0.0 )
          {
            inputRasterBandGains[ inputRasterBandMinValuesIdx ] = (rg::BaatzFeatureType)( 1.0 /
              inputRasterBandGains[ inputRasterBandMinValuesIdx ] );
          }
        }
      }
        
      // Initializing each segment
      
      unsigned int blkLine = 0;
      unsigned int blkCol = 0;      
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* segmentPtr = 0;
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* neighborSegmentPtr = 0;
      bool rasterValuesAreValid = true;
      unsigned int inputRasterBandsIdx = 0;
      double value = 0;
      const std::vector< double > dummyZeroesVector( inputRasterBandsSize, 0 );

      std::list< SegmenterSegmentsBlock::SegmentIdDataType > 
        unusedLineSegmentIds;
      
      std::vector< SegmenterSegmentsBlock::SegmentIdDataType > 
        lineSegmentIds;
      lineSegmentIds.reserve( block2ProcessInfo.m_width );
      
      std::vector< rg::BaatzFeatureType > rasterValues;
      std::vector< rg::BaatzFeatureType > rasterSquareValues;
      rasterValues.resize( inputRasterBandsSize, 0 );
      rasterSquareValues.resize( inputRasterBandsSize, 0 );
      std::vector< SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* > usedSegPointers1( block2ProcessInfo.m_width, 0 );
      std::vector< SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* > usedSegPointers2( block2ProcessInfo.m_width, 0 );
      std::vector< SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* >* lastLineSegsPtrs = &usedSegPointers1;
      std::vector< SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* >* currLineSegsPtrs = &usedSegPointers2;
      
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* prevActSegPtr = 0;
      
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
                  (rg::BaatzFeatureType)value;
                rasterSquareValues[ inputRasterBandsIdx ] = 
                  (rg::BaatzFeatureType)( value * value );
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
              BAATZ_SU( segmentPtr->m_features, rasterValuesIdx ) =
                rasterValues[ rasterValuesIdx ];
              BAATZ_SS( segmentPtr->m_features, inputRasterBandsSize, rasterValuesIdx ) =
                rasterSquareValues[ rasterValuesIdx ];
              BAATZ_ST( segmentPtr->m_features, inputRasterBandsSize, rasterValuesIdx ) =
                0.0;
            }   
                
            BAATZ_EL( segmentPtr->m_features ) = 4;
            BAATZ_CO( segmentPtr->m_features ) = 4;
            BAATZ_SM( segmentPtr->m_features ) = 1;                             

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
            
            segmentPtr->removeAllNeighborSegmentsPtrs();
              
            if( blkLine ) 
            { 
              neighborSegmentPtr = lastLineSegsPtrs->operator[]( blkCol );
                
              if( neighborSegmentPtr )
              {
                segmentPtr->addNeighborSegmentPtr( neighborSegmentPtr );
                                    
                neighborSegmentPtr->addNeighborSegmentPtr( segmentPtr );
              }
            }
            
            if( blkCol ) 
            { 
              neighborSegmentPtr = currLineSegsPtrs->operator[]( blkCol - 1 );
                
              if( neighborSegmentPtr )
              {
                segmentPtr->addNeighborSegmentPtr( neighborSegmentPtr );
                                    
                neighborSegmentPtr->addNeighborSegmentPtr( segmentPtr );
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
    
    SegmenterRegionGrowingBaatzStrategyFactory::SegmenterRegionGrowingBaatzStrategyFactory()
    : te::rp::SegmenterStrategyFactory( "RegionGrowingBaatz" )
    {
    }
    
    SegmenterRegionGrowingBaatzStrategyFactory::~SegmenterRegionGrowingBaatzStrategyFactory()
    {
    }    
    
    te::rp::SegmenterStrategy* SegmenterRegionGrowingBaatzStrategyFactory::build()
    {
      return new te::rp::SegmenterRegionGrowingBaatzStrategy();
    }
    
  } // end namespace rp
}   // end namespace te    

