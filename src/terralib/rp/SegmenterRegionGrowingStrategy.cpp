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
  \file terralib/rp/SegmenterRegionGrowingStrategy.cpp
  \briefRaster region growing segmenter strategy.
*/

#include "SegmenterRegionGrowingStrategy.h"
#include "SegmenterRegionGrowingMeanMerger.h"
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
  static te::rp::SegmenterRegionGrowingStrategyFactory 
    segmenterRegionGrowingStrategyFactoryInstance;
}

namespace te
{
  namespace rp
  {
    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategy::Parameters::Parameters()
    {
      reset();
    }
    
    SegmenterRegionGrowingStrategy::Parameters::~Parameters()
    {
    }    
    
    const SegmenterRegionGrowingStrategy::Parameters& 
    SegmenterRegionGrowingStrategy::Parameters::operator=( 
      const SegmenterRegionGrowingStrategy::Parameters& params )
    {
      reset();
      
      m_minSegmentSize = params.m_minSegmentSize;
      m_segmentsSimilarityThreshold = params.m_segmentsSimilarityThreshold;
      m_segmentFeatures = params.m_segmentFeatures;
      m_bandsWeights = params.m_bandsWeights;
      m_colorWeight = params.m_colorWeight;
      m_compactnessWeight = params.m_compactnessWeight;
      m_segmentsSimIncreaseSteps = params.m_segmentsSimIncreaseSteps;
      m_enableLocalMutualBestFitting = params.m_enableLocalMutualBestFitting;
      m_enableSameIterationMerges = params.m_enableSameIterationMerges;
      
      return *this;      
    }
    
    void SegmenterRegionGrowingStrategy::Parameters::reset() 
    throw( te::rp::Exception )
    {
      m_minSegmentSize = 100;
      m_segmentsSimilarityThreshold = 0.03;
      m_segmentFeatures = InvalidFeaturesType;      
      m_bandsWeights.clear();
      m_colorWeight = 0.9;
      m_compactnessWeight = 0.5;
      m_segmentsSimIncreaseSteps = 2;
      m_enableLocalMutualBestFitting = false;
      m_enableSameIterationMerges = false;
    }
    
    te::common::AbstractParameters* SegmenterRegionGrowingStrategy::Parameters::clone() const
    {
      return new te::rp::SegmenterRegionGrowingStrategy::Parameters( *this );
    }

    SegmenterRegionGrowingStrategy::SegmenterRegionGrowingStrategy()
    {
      m_isInitialized = false;
    }
    
    SegmenterRegionGrowingStrategy::~SegmenterRegionGrowingStrategy()
    {
      reset();
    }
    
    bool SegmenterRegionGrowingStrategy::initialize( 
      SegmenterStrategyParameters const* const strategyParams ) 
      throw( te::rp::Exception )
    {
      m_isInitialized = false;
      reset();
      
      SegmenterRegionGrowingStrategy::Parameters const* paramsPtr = 
        dynamic_cast< SegmenterRegionGrowingStrategy::Parameters const* >( strategyParams );
        
      if( paramsPtr )
      {
        m_parameters = *( paramsPtr );
        
        TERP_TRUE_OR_RETURN_FALSE( m_parameters.m_minSegmentSize > 0,
          "Invalid segmenter strategy parameter m_minSegmentSize" )
          
        TERP_TRUE_OR_RETURN_FALSE(  
          ( m_parameters.m_segmentsSimilarityThreshold >= 0.0 ),
          "Invalid segmenter strategy parameter m_segmentsSimilarityThreshold" )  
          
        TERP_TRUE_OR_RETURN_FALSE( m_parameters.m_segmentFeatures != 
          SegmenterRegionGrowingStrategy::Parameters::InvalidFeaturesType,
          "Invalid segmenter strategy parameter m_segmentFeatures" )              
          
        if( m_parameters.m_segmentFeatures == Parameters::BaatzFeaturesType )
        {
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
            
        }
        
        m_isInitialized = true;
        
        return true;        
      } 
      else
      {
        return false;
      }
    }
    
    void SegmenterRegionGrowingStrategy::reset()
    {
      m_isInitialized = false;
      m_segmentsPool.clear();
      m_segmentsIdsMatrix.reset();
      m_parameters.reset();
    }
    
    bool SegmenterRegionGrowingStrategy::execute( 
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
      
      std::auto_ptr< SegmenterRegionGrowingMerger > mergerPtr;
      
      switch( m_parameters.m_segmentFeatures )
      {
        case Parameters::MeanFeaturesType :
        {
          mergerPtr.reset( new SegmenterRegionGrowingMeanMerger( inputRasterBands.size() ) );
          break;
        }
        case Parameters::BaatzFeaturesType :
        {
          mergerPtr.reset( new SegmenterRegionGrowingBaatzMerger( m_parameters.m_colorWeight,
            m_parameters.m_compactnessWeight, m_parameters.m_bandsWeights,
            m_segmentsIdsMatrix ) );
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid segment features type" );
          break;
        }
      }        
      
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
        
      SegmenterRegionGrowingSegment* auxSeg1Ptr = m_segmentsPool.getNextSegment();
      auxSeg1Ptr->disable();
      SegmenterRegionGrowingSegment* auxSeg2Ptr = m_segmentsPool.getNextSegment();
      auxSeg2Ptr->disable();
      SegmenterRegionGrowingSegment* auxSeg3Ptr = m_segmentsPool.getNextSegment();
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
        
      SegmenterRegionGrowingSegment* actSegsListHeadPtr = 0;
      
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
      
      SegmenterRegionGrowingSegment::FeatureType minFoundDissimilarity = 0.0;
      SegmenterRegionGrowingSegment::FeatureType maxFoundDissimilarity = 0.0;
      unsigned int totalMergesNumber = 0;      
      SegmenterRegionGrowingSegment::IterationCounterType globalMergeIterationsCounter = 1;
      
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
      
//      TERP_LOGMSG( getActiveSegmentsNumber( actSegsListHeadPtr ) );
      
      mergeSegments( 
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
      
//      TERP_LOGMSG( getActiveSegmentsNumber( actSegsListHeadPtr ) );
      
      // Main Segmentation loop
      
      for( unsigned int segmentsSimIncreaseStep = 1 ; segmentsSimIncreaseStep <=
        m_parameters.m_segmentsSimIncreaseSteps ; ++segmentsSimIncreaseStep )
      {
        const SegmenterRegionGrowingSegment::FeatureType disimilarityThreshold = 
          ( ((SegmenterRegionGrowingSegment::FeatureType)segmentsSimIncreaseStep) )
          *
          ( ((SegmenterRegionGrowingSegment::FeatureType)m_parameters.m_segmentsSimilarityThreshold) )
          /
          ( (SegmenterRegionGrowingSegment::FeatureType)( m_parameters.m_segmentsSimIncreaseSteps ) );
          
        mergeSegments( 
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
        
//        TERP_LOGMSG( getActiveSegmentsNumber( actSegsListHeadPtr ) );
        
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
        mergeSegments( 
          std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max(),
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
        
//        TERP_LOGMSG( getActiveSegmentsNumber( actSegsListHeadPtr ) );
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
    
    double SegmenterRegionGrowingStrategy::getMemUsageEstimation(
      const unsigned int bandsToProcess,
      const unsigned int pixelsNumber ) const
    {
      TERP_TRUE_OR_THROW( m_isInitialized, "Instance not initialized" );
      
      // The features matrix inside the pool
      double featuresSizeBytes = 0.0;
      switch( m_parameters.m_segmentFeatures )
      {
        case Parameters::MeanFeaturesType :
        {
          featuresSizeBytes = (double)
            (
              pixelsNumber
              *
              bandsToProcess
              *
              sizeof( SegmenterRegionGrowingSegment::FeatureType )
            );
          break;
        }
        case Parameters::BaatzFeaturesType :
        {
          featuresSizeBytes = (double)
            (
              pixelsNumber
              *
              (
                (
                  3
                  *
                  sizeof( SegmenterRegionGrowingSegment::FeatureType )
                )
                +
                (
                  3
                  *
                  bandsToProcess
                  *
                  sizeof( SegmenterRegionGrowingSegment::FeatureType )
                )              
              )
            );
          break;          
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid segment features type" );
          break;
        }
      }       
      
      return (double)
        (
          featuresSizeBytes
          +
          ( // The segments matrix inside the pool
            pixelsNumber 
            * 
            (
              sizeof( SegmenterRegionGrowingSegment )
              +
              ( // An initial vector of pointers to 8 neighbors
                6
                *
                sizeof( SegmenterRegionGrowingSegment* )
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
    
    unsigned int SegmenterRegionGrowingStrategy::getOptimalBlocksOverlapSize() const
    {
      TERP_TRUE_OR_THROW( m_isInitialized, "Instance not initialized" );
      return (unsigned int)( std::sqrt( (double)m_parameters.m_minSegmentSize) );
    }
    
    bool SegmenterRegionGrowingStrategy::initializeSegments( 
      SegmenterIdsManager& segmenterIdsManager,
      const te::rp::SegmenterSegmentsBlock& block2ProcessInfo,
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,   
      const std::vector< double >& inputRasterNoDataValues,
      const std::vector< double >& inputRasterBandMinValues,
      const std::vector< double >& inputRasterBandMaxValues,
      SegmenterRegionGrowingSegment** actSegsListHeadPtr )
    {
      const unsigned int inputRasterBandsSize = (unsigned int)
        inputRasterBands.size();
        
      (*actSegsListHeadPtr) = 0;
        
      // calculating offset and gain
        
      std::vector< SegmenterRegionGrowingSegment::FeatureType > inputRasterBandGains(
        inputRasterBandMinValues.size() );
      
      {
        for( unsigned int inputRasterBandMinValuesIdx = 0 ; inputRasterBandMinValuesIdx
          < inputRasterBandMinValues.size() ; ++inputRasterBandMinValuesIdx )
        {
          inputRasterBandGains[ inputRasterBandMinValuesIdx ] = 
            inputRasterBandMaxValues[ inputRasterBandMinValuesIdx ] -
            inputRasterBandMinValues[ inputRasterBandMinValuesIdx ];
            
          if( inputRasterBandGains[ inputRasterBandMinValuesIdx ] != 0.0 )
          {
            inputRasterBandGains[ inputRasterBandMinValuesIdx ] = 1.0 /
              inputRasterBandGains[ inputRasterBandMinValuesIdx ];
          }
        }
      }
        
      // Initializing each segment
      
      unsigned int blkLine = 0;
      unsigned int blkCol = 0;      
      SegmenterRegionGrowingSegment* segmentPtr = 0;
      SegmenterRegionGrowingSegment* neighborSegmentPtr = 0;
      bool rasterValuesAreValid = true;
      unsigned int inputRasterBandsIdx = 0;
      double value = 0;
      const std::vector< double > dummyZeroesVector( inputRasterBandsSize, 0 );

      std::list< SegmenterSegmentsBlock::SegmentIdDataType > 
        unusedLineSegmentIds;
      
      std::vector< SegmenterSegmentsBlock::SegmentIdDataType > 
        lineSegmentIds;
      lineSegmentIds.reserve( block2ProcessInfo.m_width );
      
      std::vector< SegmenterRegionGrowingSegment::FeatureType > rasterValues;
      std::vector< SegmenterRegionGrowingSegment::FeatureType > rasterSquareValues;
      rasterValues.resize( inputRasterBandsSize, 0 );
      rasterSquareValues.resize( inputRasterBandsSize, 0 );
      std::vector< SegmenterRegionGrowingSegment* > usedSegPointers1( block2ProcessInfo.m_width, 0 );
      std::vector< SegmenterRegionGrowingSegment* > usedSegPointers2( block2ProcessInfo.m_width, 0 );
      std::vector< SegmenterRegionGrowingSegment* >* lastLineSegsPtrs = &usedSegPointers1;
      std::vector< SegmenterRegionGrowingSegment* >* currLineSegsPtrs = &usedSegPointers2;
      
      SegmenterRegionGrowingSegment* prevActSegPtr = 0;
      
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
                  (SegmenterRegionGrowingSegment::FeatureType)value;
                rasterSquareValues[ inputRasterBandsIdx ] = 
                  (SegmenterRegionGrowingSegment::FeatureType)( value * value );
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
            switch( m_parameters.m_segmentFeatures )
            {
              case Parameters::MeanFeaturesType :
              {
                segmentPtr = m_segmentsPool.getNextSegment();
                assert( segmentPtr );                
                
                for( rasterValuesIdx = 0 ; rasterValuesIdx < inputRasterBandsSize ;
                  ++rasterValuesIdx )
                {
                  segmentPtr->m_features[ rasterValuesIdx ] = rasterValues[ 
                    rasterValuesIdx ];
                }
                
                break;
              }
              case Parameters::BaatzFeaturesType :
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
                
                break;
              }
              default :
              {
                TERP_LOG_AND_THROW( "Invalid segment features type" );
                break;
              }
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
    
    void SegmenterRegionGrowingStrategy::mergeSegments( 
      const SegmenterRegionGrowingSegment::FeatureType disimilarityThreshold,
      const unsigned int maxSegSizeThreshold,
      SegmenterIdsManager& segmenterIdsManager,
      SegmenterRegionGrowingMerger& merger,
      const bool enablelocalMutualBestFitting,
      const bool enableSameIterationMerges,
      SegmenterRegionGrowingSegment* auxSeg1Ptr,
      SegmenterRegionGrowingSegment* auxSeg2Ptr,
      SegmenterRegionGrowingSegment* auxSeg3Ptr,
      SegmenterRegionGrowingSegment::FeatureType& minFoundDissimilarity,
      SegmenterRegionGrowingSegment::FeatureType& maxFoundDissimilarity,
      unsigned int& totalMergesNumber,
      SegmenterRegionGrowingSegment::IterationCounterType& globalMergeIterationsCounter,
      SegmenterRegionGrowingSegment** const actSegsListHeadPtrPtr )
    {
      TERP_TRUE_OR_THROW( actSegsListHeadPtrPtr != 0, "Invalid active segments list header pointer" );
      TERP_TRUE_OR_THROW( (*actSegsListHeadPtrPtr) != 0, "Invalid active segments list header pointer" );
      
      minFoundDissimilarity =
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();      
      maxFoundDissimilarity = -1.0 * minFoundDissimilarity;
      totalMergesNumber = 0;
      
      // Globals
      
      SegmenterRegionGrowingSegment::FeatureType internalDisimilarityThreshold =
        disimilarityThreshold;
      unsigned int internalMaxSegSizeThreshold = maxSegSizeThreshold;
      if( maxSegSizeThreshold )
      {
        internalDisimilarityThreshold = std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();
      }
      else
      {
        internalMaxSegSizeThreshold = std::numeric_limits< unsigned int >::max();
      }
      unsigned int iterationMergedSegmentsNumber = 0;
      unsigned int segmentsLine = 0;
      unsigned int segmentsLineBound = 0;
      unsigned int segmentCol = 0;
      unsigned int segmentColStart = 0;
      unsigned int segmentColBound = 0;
      SegmenterRegionGrowingSegment* minForwardDissimilaritySegmentPtr = 0;
      SegmenterRegionGrowingSegment::FeatureType forwardDissimilarityValue = 0;
      SegmenterRegionGrowingSegment::FeatureType minForwardDissimilarityValue = 0;  
      SegmenterRegionGrowingSegment* minBackwardDissimilaritySegmentPtr = 0;
      SegmenterRegionGrowingSegment::FeatureType backwardDissimilarityValue = 0;
      SegmenterRegionGrowingSegment::FeatureType minBackwardDissimilarityValue = 0;             
      SegmenterSegmentsBlock::SegmentIdDataType* segmentsIdsLinePtr = 0;      
      SegmenterSegmentsBlock::SegmentIdDataType currentSegmentId = 0;      
      std::list< SegmenterSegmentsBlock::SegmentIdDataType > freeSegmentIds;
      unsigned int neighborSegIdx = 0;
      SegmenterRegionGrowingSegment* currActSegPtr = (*actSegsListHeadPtrPtr);      
      
      // Main iterations loop
      
      do
      {
        iterationMergedSegmentsNumber = 0;
        currActSegPtr = (*actSegsListHeadPtrPtr);
        
        // Updating the merger state
        
        merger.update( *actSegsListHeadPtrPtr );
        
        // iterating over each segment

        do
        {
          if( 
              (
                enableSameIterationMerges
                ||
                ( currActSegPtr->m_mergetIteration < globalMergeIterationsCounter ) 
              )
              && 
              ( currActSegPtr->m_size <= internalMaxSegSizeThreshold ) 
            )
          {
            // finding the neighbor segment with minimum dissimilary value
            // related to the current sement 
            
            minForwardDissimilaritySegmentPtr = 0;
            minForwardDissimilarityValue = 
              std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();
            
            for( neighborSegIdx = 0 ; neighborSegIdx < currActSegPtr->m_neighborSegmentsSize ;
              ++neighborSegIdx )
            {
              if( currActSegPtr->m_neighborSegments[ neighborSegIdx ] )
              {
                forwardDissimilarityValue = merger.getDissimilarity( currActSegPtr,
                  currActSegPtr->m_neighborSegments[ neighborSegIdx ], auxSeg1Ptr );
                  
                if( 
                    ( forwardDissimilarityValue <= internalDisimilarityThreshold )
                    &&
                    ( forwardDissimilarityValue < minForwardDissimilarityValue )
                  )
                {
                  minForwardDissimilarityValue = forwardDissimilarityValue;
                  minForwardDissimilaritySegmentPtr = currActSegPtr->m_neighborSegments[ neighborSegIdx ];
                  auxSeg3Ptr->operator=( *auxSeg1Ptr );
                }
              }
            }
            
            // does the neighbor wants to merge back ?
            
            if( enablelocalMutualBestFitting && ( minForwardDissimilaritySegmentPtr != 0 ) )
            {
              // Calculating all neighbor neighbor segments dissimilarity          
              
              minBackwardDissimilaritySegmentPtr = 0;
              backwardDissimilarityValue = 0;
              minBackwardDissimilarityValue = 
                std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();          
              
              for( neighborSegIdx = 0 ; neighborSegIdx < minForwardDissimilaritySegmentPtr->m_neighborSegmentsSize ;
                ++neighborSegIdx )
              {
                if( minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ] )
                {
                  backwardDissimilarityValue = 
                    merger.getDissimilarity( minForwardDissimilaritySegmentPtr, 
                    minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ], auxSeg2Ptr );
                    
                  if( backwardDissimilarityValue < minBackwardDissimilarityValue )
                  {
                    minBackwardDissimilarityValue = backwardDissimilarityValue;
                    minBackwardDissimilaritySegmentPtr = 
                      minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ];
                  }
                }
              }
              
              if( minBackwardDissimilaritySegmentPtr !=  currActSegPtr )
              {
                minForwardDissimilaritySegmentPtr = 0;
              }
            }
            
            // if the min forward dissimilarity segment was found
            
            if( 
                ( minForwardDissimilaritySegmentPtr != 0 ) 
                &&
                ( 
                  enableSameIterationMerges
                  ||
                  (
                    minForwardDissimilaritySegmentPtr->m_mergetIteration 
                    < 
                    globalMergeIterationsCounter 
                  ) 
                )
              )
            {
              if( minFoundDissimilarity > minForwardDissimilarityValue )
              {
                minFoundDissimilarity = minForwardDissimilarityValue;
              }
              if( maxFoundDissimilarity < minForwardDissimilarityValue )
              {
                maxFoundDissimilarity = minForwardDissimilarityValue;
              }                
              
              // If the maximum similary neighbor was found it will be merged
              // if the dissimilarity value is below the threshold
              // merging segment data
              
              merger.mergeFeatures( currActSegPtr, minForwardDissimilaritySegmentPtr,
                auxSeg3Ptr );
              
              currActSegPtr->m_mergetIteration = globalMergeIterationsCounter;
                
              currActSegPtr->removeNeighborSegment( minForwardDissimilaritySegmentPtr );
                
              // updating the max similarity segment neighborhood segments
              // with the current segment
              
              for( neighborSegIdx = 0 ; neighborSegIdx < minForwardDissimilaritySegmentPtr->m_neighborSegmentsSize ;
                ++neighborSegIdx )
              {
                if( 
                    ( minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ] != 0 )
                    &&
                    ( minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ] != currActSegPtr )
                  )
                {
                  // adding the max similarity neighborhood segments to the 
                  // current one, if it is not already there            
                  
                  currActSegPtr->addNeighborSegment( 
                    minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ] );
                    
                  // adding the current segment into the max similarity 
                  // neighborhood segments list, if it is not already there              
                  
                  minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ]->addNeighborSegment( 
                    currActSegPtr );
                    
                  // removing the merged segment reference from its neighbor
                  // list      
                  
                  minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ]->removeNeighborSegment( 
                    minForwardDissimilaritySegmentPtr );
                }
              }          
              
              // updating the segments Ids container matrix
              
              segmentsLineBound = minForwardDissimilaritySegmentPtr->m_yBound;
              segmentColStart = minForwardDissimilaritySegmentPtr->m_xStart;
              segmentColBound = minForwardDissimilaritySegmentPtr->m_xBound;          
              currentSegmentId = currActSegPtr->m_id;
                
              for( segmentsLine = minForwardDissimilaritySegmentPtr->m_yStart ; 
                segmentsLine < segmentsLineBound ; ++segmentsLine )
              {
                segmentsIdsLinePtr = m_segmentsIdsMatrix[ segmentsLine ];
                
                for( segmentCol = segmentColStart ; segmentCol < 
                  segmentColBound ; ++segmentCol )
                {
                  if( segmentsIdsLinePtr[ segmentCol ] ==
                    minForwardDissimilaritySegmentPtr->m_id )
                  {
                    segmentsIdsLinePtr[ segmentCol ] = currentSegmentId;
                  }
                }
              }
              
              // disabling the  merged segment
              // The merged segment id will be given back to ids manager
              
              if( minForwardDissimilaritySegmentPtr == (*actSegsListHeadPtrPtr) )
              {
                (*actSegsListHeadPtrPtr) = (*actSegsListHeadPtrPtr)->m_nextActiveSegment;
              }
              
              if( minForwardDissimilaritySegmentPtr->m_prevActiveSegment )
              {
                minForwardDissimilaritySegmentPtr->m_prevActiveSegment->m_nextActiveSegment =
                  minForwardDissimilaritySegmentPtr->m_nextActiveSegment;
              }
                
              if( minForwardDissimilaritySegmentPtr->m_nextActiveSegment )
              {
                minForwardDissimilaritySegmentPtr->m_nextActiveSegment->m_prevActiveSegment =
                  minForwardDissimilaritySegmentPtr->m_prevActiveSegment;
              }
              
              minForwardDissimilaritySegmentPtr->disable();
              
              minForwardDissimilaritySegmentPtr->clearNeighborSegments();
                
              freeSegmentIds.push_back( minForwardDissimilaritySegmentPtr->m_id );
              
              ++iterationMergedSegmentsNumber;
            }
          }
          
          // going to the next segment
          
          currActSegPtr = currActSegPtr->m_nextActiveSegment;
          
        } while( currActSegPtr );
        
        // give back the free unused sement ids
        
        if( ! freeSegmentIds.empty() )
        {
          segmenterIdsManager.addFreeIDs( freeSegmentIds );
          freeSegmentIds.clear();
        }
        
        totalMergesNumber += iterationMergedSegmentsNumber;
        
//         std::cout << std::endl << "Iteration Number: " << iterationNumber <<
//           "  Merged segments number:" << iterationMergedSegmentsNumber << std::endl;
        
        ++globalMergeIterationsCounter;
      }
      while( iterationMergedSegmentsNumber && ( globalMergeIterationsCounter <
        std::numeric_limits< SegmenterRegionGrowingSegment::IterationCounterType>::max() ) );
    }
    
    void  SegmenterRegionGrowingStrategy::exportSegs2Tif( 
      const SegmentsIdsMatrixT& segmentsIds, bool normto8bits,
      const std::string& fileName )
    {
      std::map<std::string, std::string> rinfo; 
      rinfo["SOURCE"] = fileName;
      
      const unsigned int linesNmb = segmentsIds.getLinesNumber();
      const unsigned int colsNmb = segmentsIds.getColumnsNumber();
 
      te::rst::Grid* gridPtr = new te::rst::Grid( colsNmb, linesNmb );

      std::vector< te::rst::BandProperty* > bandsProps;
      bandsProps.push_back( new te::rst::BandProperty( 0, 
        (normto8bits ? te::dt::UCHAR_TYPE : te::dt::UINT32_TYPE) ) );
      
      te::rst::Raster* rasterPtr = te::rst::RasterFactory::make( "GDAL", 
        gridPtr, bandsProps, rinfo );
      TERP_TRUE_OR_THROW( rasterPtr, "Invalid pointer" )
      
      unsigned int col = 0;
      unsigned int line = 0 ;
      
      double offset = 0.0;
      double scale = 1.0;
      
      if( normto8bits )
      {
        double minValue = DBL_MAX;
        double maxValue = -1.0 * DBL_MAX;
        double value = 0;
      
        for( line = 0 ; line < linesNmb ; ++line )
        {
          for( col = 0 ; col < colsNmb ; ++col )
          {
            value = (double)segmentsIds( line, col );
            
            if( value > maxValue ) maxValue = value;
            if( value < minValue ) minValue = value;
          }
        }
        
        offset = minValue;
        scale = 254.0 / ( maxValue - minValue );
      }
      
      double value = 0;
      
      for( line = 0 ; line < linesNmb ; ++line )
      {
        for( col = 0 ; col < colsNmb ; ++col )
        {
          value = ( ((double)segmentsIds( line, col )) - offset ) * scale;
          TERP_TRUE_OR_THROW( value <= 255.0, "Invalid value:" +
            boost::lexical_cast< std::string >( value ) )
          
          rasterPtr->setValue( col, line, value , 0 );
        }
      }
      
      delete rasterPtr;
    }
    
    unsigned int SegmenterRegionGrowingStrategy::getActiveSegmentsNumber( 
      SegmenterRegionGrowingSegment* const actSegsListHeadPtr ) const
    {
      unsigned int returnValue = 0;
      
      SegmenterRegionGrowingSegment* currSegPtr = actSegsListHeadPtr;
      
      while( currSegPtr )
      {
        ++returnValue;
        currSegPtr = currSegPtr->m_nextActiveSegment;
      }
      
      return returnValue;
    }
    
    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategyFactory::SegmenterRegionGrowingStrategyFactory()
    : te::rp::SegmenterStrategyFactory( "RegionGrowing" )
    {
    }
    
    SegmenterRegionGrowingStrategyFactory::~SegmenterRegionGrowingStrategyFactory()
    {
    }    
    
    te::rp::SegmenterStrategy* SegmenterRegionGrowingStrategyFactory::build()
    {
      return new te::rp::SegmenterRegionGrowingStrategy();
    }
    
  } // end namespace rp
}   // end namespace te    

