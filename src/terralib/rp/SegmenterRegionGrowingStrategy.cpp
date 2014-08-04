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
  \file terralib/rp/SegmenterRegionGrowingStrategy.cpp
  \briefRaster region growing segmenter strategy.
*/

#include "SegmenterRegionGrowingStrategy.h"

#include "Macros.h"

#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/RasterFactory.h"
#include "../raster/Grid.h"
#include "../datatype/Enums.h"
#include "../common/StringUtils.h"
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
#define BAATZ_ST( featPtr, bandsNmb, band ) featPtr[ 3 + bandsNmb + bandsNmb + band ]

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
      
      return *this;      
    }
    
    void SegmenterRegionGrowingStrategy::Parameters::reset() 
    throw( te::rp::Exception )
    {
      m_minSegmentSize = 100;
      m_segmentsSimilarityThreshold = 0.1;
      m_segmentFeatures = InvalidFeaturesType;      
      m_bandsWeights.clear();
      m_colorWeight = 0.75;
      m_compactnessWeight = 0.5;
      m_segmentsSimIncreaseSteps = 10;
    }
    
    te::common::AbstractParameters* SegmenterRegionGrowingStrategy::Parameters::clone() const
    {
      return new te::rp::SegmenterRegionGrowingStrategy::Parameters( *this );
    }

    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategy::Merger::Merger()
    {
    }
    
    SegmenterRegionGrowingStrategy::Merger::~Merger()
    {
    } 

    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategy::MeanMerger::MeanMerger( const unsigned int featuresNumber )
    : m_featuresNumber( featuresNumber )
    {
    }
    
    SegmenterRegionGrowingStrategy::MeanMerger::~MeanMerger()
    {
    }    
    
    SegmenterRegionGrowingSegment::FeatureType SegmenterRegionGrowingStrategy::MeanMerger::getDissimilarity(
      SegmenterRegionGrowingSegment const * const segment1Ptr, 
      SegmenterRegionGrowingSegment const * const segment2Ptr, 
      SegmenterRegionGrowingSegment * const ) const
    {
      assert( segment1Ptr );
      assert( segment1Ptr->m_features );
      assert( segment2Ptr );
      assert( segment2Ptr->m_features );
      
      m_getDissimilarity_dissValue = 0.0;
        
      for( m_getDissimilarity_meansIdx = 0 ; m_getDissimilarity_meansIdx < m_featuresNumber ; 
        ++m_getDissimilarity_meansIdx )
      {
        m_getDissimilarity_diffValue = segment1Ptr->m_features[ m_getDissimilarity_meansIdx ] - 
          segment2Ptr->m_features[ m_getDissimilarity_meansIdx ];
          
        m_getDissimilarity_dissValue += ( m_getDissimilarity_diffValue * m_getDissimilarity_diffValue );
      }
      
      m_getDissimilarity_dissValue = std::sqrt( m_getDissimilarity_dissValue );
      
      return m_getDissimilarity_dissValue;
    }
    
    void SegmenterRegionGrowingStrategy::MeanMerger::mergeFeatures( 
      SegmenterRegionGrowingSegment * const segment1Ptr, 
      SegmenterRegionGrowingSegment const * const segment2Ptr, 
      SegmenterRegionGrowingSegment const * const ) const
    {
      assert( segment1Ptr );
      assert( segment1Ptr->m_features );
      assert( segment2Ptr );
      assert( segment2Ptr->m_features );
        
      // Merging basic features
      
      segment1Ptr->m_size += segment2Ptr->m_size;
      
      segment1Ptr->m_xStart = std::min( 
        segment1Ptr->m_xStart, 
        segment2Ptr->m_xStart );
      segment1Ptr->m_xBound = std::max( 
        segment1Ptr->m_xBound, 
        segment2Ptr->m_xBound );

      segment1Ptr->m_yStart = std::min( 
        segment1Ptr->m_yStart, 
        segment2Ptr->m_yStart );
      segment1Ptr->m_yBound = std::max( 
        segment1Ptr->m_yBound, 
        segment2Ptr->m_yBound );
        
      // Merging specific features
        
      for( unsigned int meansIdx = 0 ; meansIdx < m_featuresNumber ; ++meansIdx )
      {
        segment1Ptr->m_features[ meansIdx ] = 
          (
            ( 
              segment1Ptr->m_features[ meansIdx ] 
              * 
              ((SegmenterRegionGrowingSegment::FeatureType)segment1Ptr->m_size ) 
            ) 
            +
            ( 
              segment2Ptr->m_features[ meansIdx ] 
              *
              ( (SegmenterRegionGrowingSegment::FeatureType)segment2Ptr->m_size) 
            )
          )
          / 
          (
            (SegmenterRegionGrowingSegment::FeatureType)
            ( 
              segment1Ptr->m_size 
              + 
              segment2Ptr->m_size
            )
          );
      }
    }
    
    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategy::BaatzMerger::BaatzMerger(
      const double& colorWeight, const double& compactnessWeight,
      const std::vector< double >& bandsWeights,
      const SegmentsIdsMatrixT& segmentsIds,
      Matrix< SegmenterRegionGrowingSegment >& segmentsMatrix )
      : 
        m_segmentsIds( segmentsIds ),
        m_segmentsMatrix( segmentsMatrix ),      
        m_allSegsCompactnessOffset( 0 ),
        m_allSegsCompactnessGain( 1.0 ),
        m_allSegsSmoothnessOffset( 0 ),
        m_allSegsSmoothnessGain( 0 ),
        m_colorWeight( (SegmenterRegionGrowingSegment::FeatureType)colorWeight ),
        m_compactnessWeight( (SegmenterRegionGrowingSegment::FeatureType)compactnessWeight )
    {
      m_bandsNumber = (unsigned int)bandsWeights.size();
      
      m_bandsWeights.resize( m_bandsNumber, 1 );
      
      for( unsigned int band = 0 ; band < m_bandsNumber ; ++band )
      {
        m_bandsWeights[ band ] = (SegmenterRegionGrowingSegment::FeatureType)
          bandsWeights[ band ];
      }
    }
    
    SegmenterRegionGrowingStrategy::BaatzMerger::~BaatzMerger()
    {
    }    
    
    SegmenterRegionGrowingSegment::FeatureType SegmenterRegionGrowingStrategy::BaatzMerger::getDissimilarity(
      SegmenterRegionGrowingSegment const * const segment1Ptr, 
      SegmenterRegionGrowingSegment const * const segment2Ptr, 
      SegmenterRegionGrowingSegment * const mergePreviewSegPtr ) const
    {
      assert( segment1Ptr );
      assert( segment1Ptr->m_features );
      assert( segment2Ptr );
      assert( segment2Ptr->m_features );      
      assert( mergePreviewSegPtr );
      
      // globals
      
      mergePreviewSegPtr->m_size = segment1Ptr->m_size + 
        segment2Ptr->m_size;
      TERP_DEBUG_TRUE_OR_THROW( mergePreviewSegPtr->m_size,
        "Internal error" );
      const SegmenterRegionGrowingSegment::FeatureType sizeUnionD = 
        (SegmenterRegionGrowingSegment::FeatureType)mergePreviewSegPtr->m_size;
      
      const SegmenterRegionGrowingSegment::FeatureType sizeSeg1D = 
        (SegmenterRegionGrowingSegment::FeatureType)segment1Ptr->m_size;      
      
      const SegmenterRegionGrowingSegment::FeatureType sizeSeg2D = 
        (SegmenterRegionGrowingSegment::FeatureType)segment2Ptr->m_size;      
      
      // Finding the form heterogeneity
      
      mergePreviewSegPtr->m_xStart = std::min( segment1Ptr->m_xStart,
        segment2Ptr->m_xStart );
      mergePreviewSegPtr->m_yStart = std::min( segment1Ptr->m_yStart,
        segment2Ptr->m_yStart );        
      mergePreviewSegPtr->m_xBound = std::max( segment1Ptr->m_xBound,
        segment2Ptr->m_xBound );      
      mergePreviewSegPtr->m_yBound = std::max( segment1Ptr->m_yBound,
        segment2Ptr->m_yBound ); 
        
      assert( mergePreviewSegPtr->m_xBound > mergePreviewSegPtr->m_xStart );
      assert( mergePreviewSegPtr->m_yBound > mergePreviewSegPtr->m_yStart );
        
      unsigned int touchingEdgeLength1 = 0;
      unsigned int touchingEdgeLength2 = 0;
      SegmenterRegionGrowingStrategy::getTouchingEdgeLength(
        m_segmentsIds, mergePreviewSegPtr->m_xStart, 
        mergePreviewSegPtr->m_yStart,
        mergePreviewSegPtr->m_xBound, 
        mergePreviewSegPtr->m_yBound, 
        segment1Ptr->m_id,
        segment2Ptr->m_id,
        touchingEdgeLength1,
        touchingEdgeLength2 );
      
      BAATZ_EL( mergePreviewSegPtr->m_features ) = 
        BAATZ_EL( segment1Ptr->m_features ) - ( (SegmenterRegionGrowingSegment::FeatureType)touchingEdgeLength1 )
        +
        BAATZ_EL( segment2Ptr->m_features ) - ( (SegmenterRegionGrowingSegment::FeatureType)touchingEdgeLength2 );
      
      BAATZ_CO( mergePreviewSegPtr->m_features ) = (SegmenterRegionGrowingSegment::FeatureType)(
        BAATZ_EL( mergePreviewSegPtr->m_features ) /
        std::sqrt( sizeUnionD ) );
        
      BAATZ_SM( mergePreviewSegPtr->m_features ) =
        BAATZ_EL( mergePreviewSegPtr->m_features ) 
        /
        (SegmenterRegionGrowingSegment::FeatureType)(
          (
            2 * ( mergePreviewSegPtr->m_xBound - mergePreviewSegPtr->m_xStart )
          )
          +
          (
            2 * ( mergePreviewSegPtr->m_yBound - mergePreviewSegPtr->m_yStart )
          )
        );
        
      const SegmenterRegionGrowingSegment::FeatureType hCompact = 
        (
          (
            (
              BAATZ_CO( mergePreviewSegPtr->m_features )
              +
              m_allSegsCompactnessOffset
            )
            *
            m_allSegsCompactnessGain
          )
          -
          (
            (
              (
                (
                  (
                    BAATZ_CO( segment1Ptr->m_features )
                    +
                    m_allSegsCompactnessOffset
                  )
                  *
                  m_allSegsCompactnessGain
                )
                *
                sizeSeg1D
              )
              +
              (
                (
                  (
                    BAATZ_CO( segment2Ptr->m_features )
                    +
                    m_allSegsCompactnessOffset
                  )
                  *
                  m_allSegsCompactnessGain
                )
                *
                sizeSeg2D
              )
            )
            /
            sizeUnionD
          )
        );
      
      const SegmenterRegionGrowingSegment::FeatureType hSmooth =
        (
          (
            (
              BAATZ_SM( mergePreviewSegPtr->m_features )
              +
              m_allSegsSmoothnessOffset
            )
            *
            m_allSegsSmoothnessGain
          )
          -
          (
            (
              (
                (
                  (
                    BAATZ_SM( segment1Ptr->m_features )
                    +
                    m_allSegsSmoothnessOffset
                  )
                  *
                  m_allSegsSmoothnessGain
                )
                *
                sizeSeg1D
              )
              +
              (
                (
                  (
                    BAATZ_SM( segment2Ptr->m_features )
                    +
                    m_allSegsSmoothnessOffset
                  )
                  *
                  m_allSegsSmoothnessGain
                )
                *
                sizeSeg2D
              )
            )
            /
            sizeUnionD
          )
        );
        
      const SegmenterRegionGrowingSegment::FeatureType hForm = 
        (
          (
            m_compactnessWeight 
            *
            hCompact
          )
          +
          (
            ( 1.0f - m_compactnessWeight )
            *
            hSmooth
          )
        );
      
      // Finding the color heterogeneity
      
      SegmenterRegionGrowingSegment::FeatureType hColor = 0;
      SegmenterRegionGrowingSegment::FeatureType sumUnion = 0;
      SegmenterRegionGrowingSegment::FeatureType squaresSumUnion = 0;
      SegmenterRegionGrowingSegment::FeatureType meanUnion = 0;
      SegmenterRegionGrowingSegment::FeatureType stdDevUnion = 0.0;      
      
      for( unsigned int sumsIdx = 0 ; sumsIdx < m_bandsNumber ; ++sumsIdx )
      {
        const SegmenterRegionGrowingSegment::FeatureType& sum1 = 
          BAATZ_SU( segment1Ptr->m_features, sumsIdx );
        
        const SegmenterRegionGrowingSegment::FeatureType& sum2 = 
          BAATZ_SU( segment2Ptr->m_features, sumsIdx );
        
        sumUnion = sum1 + sum2;
        BAATZ_SU( mergePreviewSegPtr->m_features, sumsIdx ) = sumUnion;        
        
        squaresSumUnion = BAATZ_SS( segment1Ptr->m_features, m_bandsNumber, sumsIdx ) +
          BAATZ_SS( segment2Ptr->m_features, m_bandsNumber, sumsIdx );
        BAATZ_SS( mergePreviewSegPtr->m_features, m_bandsNumber, sumsIdx ) = squaresSumUnion;
        
        meanUnion = sumUnion / sizeUnionD;
        
        stdDevUnion =
          std::sqrt(
            std::max(
              (SegmenterRegionGrowingSegment::FeatureType)0
              ,
              (
                (
                  squaresSumUnion
                  -
                  (
                    ((SegmenterRegionGrowingSegment::FeatureType)2) * meanUnion * sumUnion
                  )
                  +
                  (
                    sizeUnionD * meanUnion * meanUnion
                  )
                )
                /
                sizeUnionD
              )
            )
          );
        BAATZ_ST( mergePreviewSegPtr->m_features, m_bandsNumber, sumsIdx ) =
          stdDevUnion;        
         
        hColor += 
          ( 
            m_bandsWeights[ sumsIdx ]
            *
            (
              stdDevUnion
              -
              (
                (
                  (
                    BAATZ_ST( segment1Ptr->m_features, m_bandsNumber, sumsIdx )
                    *
                    sizeSeg1D
                  )
                  +
                  (
                    BAATZ_ST( segment2Ptr->m_features, m_bandsNumber, sumsIdx )
                    *
                    sizeSeg2D
                  )
                )
                /
                sizeUnionD
              )
            )
          );
      }
      
      hColor = 
        (
          ( 
            hColor 
            * 
            m_colorWeight 
          ) 
          + 
          (
            ( 1.0f - m_colorWeight )
            *
            hForm
          )
        );       
        
      return std::max( hColor, ((SegmenterRegionGrowingSegment::FeatureType)0) );
    }
    
    void SegmenterRegionGrowingStrategy::BaatzMerger::mergeFeatures( 
      SegmenterRegionGrowingSegment * const segment1Ptr, 
      SegmenterRegionGrowingSegment const * const segment2Ptr , 
      SegmenterRegionGrowingSegment const * const mergePreviewSegPtr ) const
    {
      assert( segment1Ptr );
      assert( segment1Ptr->m_features );
      assert( segment2Ptr );
      assert( segment2Ptr->m_features );      
      assert( mergePreviewSegPtr );
      
      // Merging basic features
      
      segment1Ptr->m_size = mergePreviewSegPtr->m_size;
      segment1Ptr->m_xStart = mergePreviewSegPtr->m_xStart;
      segment1Ptr->m_xBound = mergePreviewSegPtr->m_xBound;
      segment1Ptr->m_yStart = mergePreviewSegPtr->m_yStart;
      segment1Ptr->m_yBound = mergePreviewSegPtr->m_yBound;
        
      // Merging specific features   
      
      memcpy( segment1Ptr->m_features, mergePreviewSegPtr->m_features, 3 + ( 3 *
        sizeof( SegmenterRegionGrowingSegment::FeatureType ) * m_bandsNumber ) );
    }    
    
    void SegmenterRegionGrowingStrategy::BaatzMerger::update()
    {
      SegmenterRegionGrowingSegment::FeatureType compactnessMin = 
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();
      SegmenterRegionGrowingSegment::FeatureType compactnessMax = -1.0f * 
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();

      SegmenterRegionGrowingSegment::FeatureType smoothnessMin =
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();
      SegmenterRegionGrowingSegment::FeatureType smoothnessMax = -1.0f *
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();
        
      SegmenterRegionGrowingSegment::FeatureType* featuresPtr = 0;
      
      const unsigned int nRows = m_segmentsMatrix.getLinesNumber();
      const unsigned int nCols = m_segmentsMatrix.getColumnsNumber();      
      SegmenterRegionGrowingSegment* segsRowPtr = 0;
      
      unsigned int col = 0;
      for( unsigned int row = 0 ; row < nRows ; ++row )
      {
        segsRowPtr = m_segmentsMatrix[ row ];
        
        for( col = 0 ; col < nCols ; ++col )
        {
          if( segsRowPtr[ col ].m_status )
          {
            featuresPtr = segsRowPtr[ col ].m_features;
            
            if( compactnessMin > BAATZ_CO( featuresPtr ) )
            {
              compactnessMin = BAATZ_CO( featuresPtr );        
            }
            if( compactnessMax < BAATZ_CO( featuresPtr ) )
            {
              compactnessMax = BAATZ_CO( featuresPtr );        
            }
            
            if( smoothnessMin > BAATZ_SM( featuresPtr ) )
            {
              smoothnessMin = BAATZ_SM( featuresPtr );
            }
            if( smoothnessMax < BAATZ_SM( featuresPtr ) )
            {
              smoothnessMax = BAATZ_SM( featuresPtr );
            }
          }
        }
      }       
      
      if( compactnessMax == compactnessMin )
      {
        m_allSegsCompactnessOffset = 0.0;
        
        if( compactnessMax == 0.0 )
          m_allSegsCompactnessGain = 1.0;
        else
          m_allSegsCompactnessGain = 1.0f / compactnessMax;
      }
      else
      {
        m_allSegsCompactnessOffset = -1.0f  * compactnessMin;
        m_allSegsCompactnessGain = 1.0f / ( compactnessMax - compactnessMin );
      }
      
      if( smoothnessMax == smoothnessMin )
      {
        m_allSegsSmoothnessOffset = 0.0;
        
        if( smoothnessMax == 0.0 )
          m_allSegsSmoothnessGain = 1.0;
        else
          m_allSegsSmoothnessGain = 1.0f / smoothnessMax;
      }
      else
      {
        m_allSegsSmoothnessOffset = -1.0f  * smoothnessMin;
        m_allSegsSmoothnessGain = 1.0f / ( smoothnessMax - smoothnessMin );      
      }
    }
    
    //-------------------------------------------------------------------------
    
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
      const std::vector< double >& inputRasterGains,
      const std::vector< double >& inputRasterOffsets,
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
      
      // Initiating the segments pool
      
      unsigned int segmentFeaturesSize = 0;
      switch( m_parameters.m_segmentFeatures )
      {
        case Parameters::MeanFeaturesType :
        {
          segmentFeaturesSize = inputRasterBands.size();
          break;
        }
        case Parameters::BaatzFeaturesType :
        {
          segmentFeaturesSize = 3 + ( 3 * inputRasterBands.size() );
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid segment features type" );
          break;
        }
      }     
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
      auxSeg1Ptr->m_status = false;
      SegmenterRegionGrowingSegment* auxSeg2Ptr = m_segmentsPool.getNextSegment();
      auxSeg2Ptr->m_status = false;
      SegmenterRegionGrowingSegment* auxSeg3Ptr = m_segmentsPool.getNextSegment();
      auxSeg3Ptr->m_status = false;
      
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
        
      TERP_TRUE_OR_RETURN_FALSE( initializeSegments( segmenterIdsManager,
        block2ProcessInfo, inputRaster, inputRasterBands, inputRasterGains,
        inputRasterOffsets ), 
        "Segments initalization error" );
        
      // Creating the merger instance
      
      std::auto_ptr< Merger > mergerPtr;
      bool enablelocalMutualBestFitting = false;
      
      switch( m_parameters.m_segmentFeatures )
      {
        case Parameters::MeanFeaturesType :
        {
          mergerPtr.reset( new MeanMerger( inputRasterBands.size() ) );
          enablelocalMutualBestFitting = true;
          break;
        }
        case Parameters::BaatzFeaturesType :
        {
          mergerPtr.reset( new BaatzMerger( m_parameters.m_colorWeight,
            m_parameters.m_compactnessWeight, m_parameters.m_bandsWeights,
            m_segmentsIdsMatrix, m_segmentsPool.getSegsMatrix() ) );
          enablelocalMutualBestFitting = true;
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid segment features type" );
          break;
        }
      }      
      
      // Progress interface
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( enableProgressInterface )
      {
        progressPtr.reset( new te::common::TaskProgress );
        progressPtr->setTotalSteps( 100 );
        progressPtr->setMessage( "Segmentation" );
      }          
      
      // Segmentation loop
        
      SegmenterRegionGrowingSegment::FeatureType disimilarityThreshold = 0;
      const SegmenterRegionGrowingSegment::FeatureType disimilarityThresholdStep = 
        ( 
          ( ((SegmenterRegionGrowingSegment::FeatureType)m_parameters.m_segmentsSimilarityThreshold) )
          /
          ( (SegmenterRegionGrowingSegment::FeatureType)( m_parameters.m_segmentsSimIncreaseSteps ) )
        );
      unsigned int mergedSegments = 0;
      unsigned int maxMergedSegments = 0;
      int currStep = 0;
      
//       unsigned int mergetIterations = 0;
//       exportSegs2Tif( m_segmentsIdsMatrix, true, "merging" + 
//         te::common::Convert2String( mergetIterations ) + ".tif" );
      
      while ( true )
      {
        mergedSegments = mergeSegments( disimilarityThreshold, segmenterIdsManager, 
          *mergerPtr, enablelocalMutualBestFitting,
          auxSeg1Ptr, auxSeg2Ptr, auxSeg3Ptr);
//         exportSegs2Tif( m_segmentsIdsMatrix, true, "merging" + 
//           te::common::Convert2String( ++mergetIterations ) + ".tif" );

        if( enableProgressInterface )
        {
          if( maxMergedSegments )
          {
            currStep = (int)
              ( 
                (
                  ( 
                    ( (double)( maxMergedSegments - mergedSegments ) ) 
                    / 
                    ((double)maxMergedSegments ) 
                  )
                ) 
                * 
                50.0
              );
            
            if( currStep > progressPtr->getCurrentStep() )
            {
              progressPtr->setCurrentStep( currStep );
            }
          }
          
          if( ! progressPtr->isActive() ) 
          {
            return false;
          }   
          
          if( maxMergedSegments < mergedSegments )
          {
            maxMergedSegments = mergedSegments;
          }                  
        }
        
        if( mergedSegments == 0 )
        {
          if( disimilarityThreshold == (SegmenterRegionGrowingSegment::FeatureType)m_parameters.m_segmentsSimilarityThreshold ) 
          {
            break;
          }
          else
          {
            disimilarityThreshold += disimilarityThresholdStep;
            disimilarityThreshold = std::min( disimilarityThreshold,
              (SegmenterRegionGrowingSegment::FeatureType)m_parameters.m_segmentsSimilarityThreshold);
          }
        }
      }
      
      if( enableProgressInterface )
      {      
        progressPtr->setCurrentStep( 50 );
        if( ! progressPtr->isActive() ) 
        {
          return false;
        }         
      }
      
      if( m_parameters.m_minSegmentSize > 1 )
      {
        maxMergedSegments = 0;
        
        while( true )
        {
          mergedSegments = mergeSmallSegments( m_parameters.m_minSegmentSize, 
            segmenterIdsManager, *mergerPtr, auxSeg1Ptr, auxSeg2Ptr );
//        exportSegs2Tif( segmentsIds, true, "mergingSmall" + 
//          te::common::Convert2String( mergetIterations ) + ".tif" );

          if( enableProgressInterface )
          {
            if( maxMergedSegments )
            {
              currStep = 50 + (int)
                ( 
                  (
                    ( 
                      ( (double)( maxMergedSegments - mergedSegments ) ) 
                      / 
                      ((double)maxMergedSegments ) 
                    )
                  ) 
                  * 
                  50.0
                );                
              
              if( currStep > progressPtr->getCurrentStep() )
              {
                progressPtr->setCurrentStep( currStep );
              }
            }
            
            if( ! progressPtr->isActive() ) 
            {
              return false;
            }          
            
            if( maxMergedSegments < mergedSegments )
            {
              maxMergedSegments = mergedSegments;
            }             
          }
          
          if( mergedSegments == 0 )
          {
            break;
          }
        }
      }
      
      if( enableProgressInterface )
      {      
        progressPtr->setCurrentStep( 100 );
        if( ! progressPtr->isActive() ) 
        {
          return false;
        }         
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
      const std::vector< double >& inputRasterGains,
      const std::vector< double >& inputRasterOffsets )
    {
      const unsigned int inputRasterBandsSize = (unsigned int)
        inputRasterBands.size();
        
      // fiding band dummy values
      
      std::vector< double > bandDummyValues;
      
      {
        for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx < 
          inputRasterBandsSize ; ++inputRasterBandsIdx )
        {
          bandDummyValues.push_back( inputRaster.getBand( 
            inputRasterBands[ inputRasterBandsIdx ] )->getProperty()->m_noDataValue );
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
                
              if( value == bandDummyValues[ inputRasterBandsIdx ] )
              {
                rasterValuesAreValid = false;
                break;
              }
              else
              {
                value += inputRasterOffsets[ inputRasterBandsIdx ];
                value *= inputRasterGains[ inputRasterBandsIdx ];
                
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
                  BAATZ_EL( segmentPtr->m_features ) = 4;
                  BAATZ_CO( segmentPtr->m_features ) = 4;
                  BAATZ_SM( segmentPtr->m_features ) = 1;
                }                
                
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
            segmentPtr->m_status = true;
            segmentPtr->m_size = 1;
            segmentPtr->m_xStart = blkCol;
            segmentPtr->m_xBound = blkCol + 1;
            segmentPtr->m_yStart = blkLine;
            segmentPtr->m_yBound = blkLine + 1;
            
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
    
    unsigned int SegmenterRegionGrowingStrategy::mergeSegments( 
      const SegmenterRegionGrowingSegment::FeatureType disimilarityThreshold,
      SegmenterIdsManager& segmenterIdsManager,
      Merger& merger,
      const bool enablelocalMutualBestFitting,
      SegmenterRegionGrowingSegment* auxSeg1Ptr,
      SegmenterRegionGrowingSegment* auxSeg2Ptr,
      SegmenterRegionGrowingSegment* auxSeg3Ptr)
    {
      unsigned int mergedSegmentsNumber = 0;
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
      Matrix< SegmenterRegionGrowingSegment >& segmentsMatrix = m_segmentsPool.getSegsMatrix();
      const unsigned int segmentsMatrixNRows = segmentsMatrix.getLinesNumber();
      const unsigned int segmentsMatrixNCols = segmentsMatrix.getColumnsNumber();      
      SegmenterRegionGrowingSegment* segmentsMatrixLinePtr = 0;
      unsigned int col = 0;      
      SegmenterRegionGrowingSegment* currSegPtr = 0;
      
      // Updating the merger state
      
      merger.update();
      
      // iterating over each segment

      for( unsigned int row = 0 ; row < segmentsMatrixNRows ; ++row )
      {
        segmentsMatrixLinePtr = segmentsMatrix[ row ];
        
        for( col = 0 ; col < segmentsMatrixNCols ; ++col )
        {
          currSegPtr = segmentsMatrixLinePtr + col;
          
          if( currSegPtr->m_status )
          {
            // finding the neighbor segment with minimum dissimilary value
            // related to the current sement
            
            minForwardDissimilaritySegmentPtr = 0;
            minForwardDissimilarityValue = 
              std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();
            
            for( neighborSegIdx = 0 ; neighborSegIdx < currSegPtr->m_neighborSegmentsSize ;
              ++neighborSegIdx )
            {
              if( currSegPtr->m_neighborSegments[ neighborSegIdx ] )
              {
                forwardDissimilarityValue = merger.getDissimilarity( currSegPtr,
                  currSegPtr->m_neighborSegments[ neighborSegIdx ], auxSeg1Ptr );
                  
                if( ( forwardDissimilarityValue <= disimilarityThreshold ) &&
                  ( forwardDissimilarityValue < minForwardDissimilarityValue ) )
                {
                  minForwardDissimilarityValue = forwardDissimilarityValue;
                  minForwardDissimilaritySegmentPtr = currSegPtr->m_neighborSegments[ neighborSegIdx ];
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
              
              if( minBackwardDissimilaritySegmentPtr !=  currSegPtr )
              {
                minForwardDissimilaritySegmentPtr = 0;
              }
            }
            
            // If the maximum similary neighbor was found it will be merged
            
            if( minForwardDissimilaritySegmentPtr )
            {
              // merging segment data
              
              merger.mergeFeatures( currSegPtr, minForwardDissimilaritySegmentPtr,
                auxSeg3Ptr );
                
              currSegPtr->removeNeighborSegment( minForwardDissimilaritySegmentPtr );
                
              // updating the max similarity segment neighborhood segments
              // with the current segment
              
              for( neighborSegIdx = 0 ; neighborSegIdx < minForwardDissimilaritySegmentPtr->m_neighborSegmentsSize ;
                ++neighborSegIdx )
              {
                if( 
                    ( minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ] != 0 )
                    &&
                    ( minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ] != currSegPtr )
                  )
                {
                  // adding the max similarity neighborhood segments to the 
                  // current one, if it is not already there            
                  
                  currSegPtr->addNeighborSegment( 
                    minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ] );
                    
                  // adding the current segment into the max similarity 
                  // neighborhood segments list, if it is not already there              
                  
                  minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ]->addNeighborSegment( 
                    currSegPtr );
                    
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
              currentSegmentId = currSegPtr->m_id;
                
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
              
              minForwardDissimilaritySegmentPtr->m_status = false;
              
              minForwardDissimilaritySegmentPtr->clearNeighborSegments();
                
              freeSegmentIds.push_back( minForwardDissimilaritySegmentPtr->m_id );
              
              ++mergedSegmentsNumber;
            }
          }
        }
      }
      
      // give back the free unused sement ids
      
      if( ! freeSegmentIds.empty() )
      {
        segmenterIdsManager.addFreeIDs( freeSegmentIds );
      }
      
      return mergedSegmentsNumber;
    }
    
    unsigned int SegmenterRegionGrowingStrategy::mergeSmallSegments(
      const unsigned int minSegmentSize,
      SegmenterIdsManager& segmenterIdsManager,
      Merger& merger,
      SegmenterRegionGrowingSegment* auxSeg1Ptr,
      SegmenterRegionGrowingSegment* auxSeg2Ptr )
    {
      unsigned int mergedSegmentsNumber = 0;
      SegmenterRegionGrowingSegment* currSmallSegPtr = 0;
      SegmenterRegionGrowingSegment* minForwardDissimilaritySegmentPtr = 0;
      SegmenterRegionGrowingSegment::FeatureType forwardDissimilarityValue = 0;
      SegmenterRegionGrowingSegment::FeatureType minForwardDissimilarityValue = 0;        
      unsigned int segmentsLine = 0;
      unsigned int segmentsLineBound = 0;
      unsigned int segmentCol = 0;
      unsigned int segmentColStart = 0;
      unsigned int segmentColBound = 0;      
      SegmenterSegmentsBlock::SegmentIdDataType* segmentsIdsLinePtr = 0;
      SegmenterSegmentsBlock::SegmentIdDataType currentSegmentId = 0;
      std::list< SegmenterSegmentsBlock::SegmentIdDataType > freeSegmentIds;
      unsigned int neighborSegIdx = 0;
      Matrix< SegmenterRegionGrowingSegment >& segmentsMatrix = m_segmentsPool.getSegsMatrix();
      const unsigned int segmentsMatrixNRows = segmentsMatrix.getLinesNumber();
      const unsigned int segmentsMatrixNCols = segmentsMatrix.getColumnsNumber();       
      SegmenterRegionGrowingSegment* segmentsMatrixLinePtr = 0;
      unsigned int col = 0;
      
      // Updating the merger state
      
      merger.update();
      
      // iterating over each segment      
      
      for( unsigned int row = 0 ; row < segmentsMatrixNRows ; ++row )
      {
        segmentsMatrixLinePtr = segmentsMatrix[ row ];
        
        for( col = 0 ; col < segmentsMatrixNCols ; ++col )
        {
          currSmallSegPtr = segmentsMatrixLinePtr + col;
          
          if( currSmallSegPtr->m_status )
          {
            // is this a small segment ?
            
            if( currSmallSegPtr->m_size < minSegmentSize )
            {
              // Looking for the closest neighboorhood segment
              
              minForwardDissimilaritySegmentPtr = 0;
              minForwardDissimilarityValue = 
                std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();
                
              for( neighborSegIdx = 0 ; neighborSegIdx < currSmallSegPtr->m_neighborSegmentsSize ;
                ++neighborSegIdx )
              {
                if( currSmallSegPtr->m_neighborSegments[ neighborSegIdx ] )
                {
                  forwardDissimilarityValue = merger.getDissimilarity( currSmallSegPtr,
                    currSmallSegPtr->m_neighborSegments[ neighborSegIdx ], auxSeg1Ptr );
                    
                  if( forwardDissimilarityValue < minForwardDissimilarityValue )
                  {
                    minForwardDissimilarityValue = forwardDissimilarityValue;
                    minForwardDissimilaritySegmentPtr = currSmallSegPtr->m_neighborSegments[ neighborSegIdx ];
                    auxSeg2Ptr->operator=( *auxSeg1Ptr );
                  }
                }
              }            
              
              // If the minimum dissimilary neighbor was found it will be merged
            
              if( minForwardDissimilaritySegmentPtr )
              {          
                // merging the small segment data into there
                // closes segment data
                
                merger.mergeFeatures( minForwardDissimilaritySegmentPtr,
                  currSmallSegPtr, auxSeg2Ptr );
                  
                minForwardDissimilaritySegmentPtr->removeNeighborSegment( currSmallSegPtr );
                  
                // updating the the small segment neighborhood segments
                // with the current segment
                
                for( neighborSegIdx = 0 ; neighborSegIdx < currSmallSegPtr->m_neighborSegmentsSize ;
                  ++neighborSegIdx )
                {
                  if( 
                      ( currSmallSegPtr->m_neighborSegments[ neighborSegIdx ] != 0 )
                      &&
                      ( currSmallSegPtr->m_neighborSegments[ neighborSegIdx ] != minForwardDissimilaritySegmentPtr )
                    )
                  {
                    // adding the small segment neighborhood segments to the 
                    // closest segment, if it is not already there         
                    
                    minForwardDissimilaritySegmentPtr->addNeighborSegment( 
                      currSmallSegPtr->m_neighborSegments[ neighborSegIdx ] );
                      
                    // adding the closest segment into the small segment 
                    // neighborhood segments list, if it is not already there           
                    
                    currSmallSegPtr->m_neighborSegments[ neighborSegIdx ]->addNeighborSegment( 
                      minForwardDissimilaritySegmentPtr );
                      
                    // removing the small segment reference from its neighbor
                    // list   
                    
                    currSmallSegPtr->m_neighborSegments[ neighborSegIdx ]->removeNeighborSegment( 
                      currSmallSegPtr );
                  }
                }             
                
                // updating the segments Ids container matrix
                
                segmentsLineBound = currSmallSegPtr->m_yBound;
                segmentColStart = currSmallSegPtr->m_xStart;
                segmentColBound = currSmallSegPtr->m_xBound;          
                currentSegmentId = currSmallSegPtr->m_id;
                  
                for( segmentsLine = currSmallSegPtr->m_yStart ; 
                  segmentsLine < segmentsLineBound ; ++segmentsLine )
                {
                  segmentsIdsLinePtr = m_segmentsIdsMatrix[ segmentsLine ];
                  
                  for( segmentCol = segmentColStart ; segmentCol < 
                    segmentColBound ; ++segmentCol )
                  {
                    if( segmentsIdsLinePtr[ segmentCol ] ==
                      currentSegmentId )
                    {
                      segmentsIdsLinePtr[ segmentCol ] = 
                        minForwardDissimilaritySegmentPtr->m_id;
                    }
                  }
                }
                
                // disabling the small segment
                // The merged segment id will be given back to ids manager
                
                currSmallSegPtr->clearNeighborSegments();

                currSmallSegPtr->m_status = false;
                
                freeSegmentIds.push_back( currentSegmentId );
                
                ++mergedSegmentsNumber;
              }
            }
          }
        }
      }
      
      // give back the free unused sement ids
      
      if( ! freeSegmentIds.empty() )
      {
        segmenterIdsManager.addFreeIDs( freeSegmentIds );
      }      
      
      return mergedSegmentsNumber;
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
    
    void SegmenterRegionGrowingStrategy::getTouchingEdgeLength( 
      const SegmentsIdsMatrixT& segsIds,
      const unsigned int& xStart, const unsigned int& yStart,
      const unsigned int& xBound, const unsigned int& yBound,
      const SegmenterSegmentsBlock::SegmentIdDataType& id1,
      const SegmenterSegmentsBlock::SegmentIdDataType& id2,
      unsigned int& edgeLength1,
      unsigned int& edgeLength2 )
    {
      const unsigned int colsNumber = segsIds.getColumnsNumber();
      const unsigned int linesNumber = segsIds.getLinesNumber();
      
      TERP_DEBUG_TRUE_OR_THROW( xStart < colsNumber,
        "Internal Error" )
      TERP_DEBUG_TRUE_OR_THROW( xBound <= colsNumber,
        "Internal Error" )
      TERP_DEBUG_TRUE_OR_THROW( yStart < linesNumber,
        "Internal Error" )
      TERP_DEBUG_TRUE_OR_THROW( yBound <= linesNumber,
        "Internal Error" )        
      TERP_DEBUG_TRUE_OR_THROW( xStart < xBound, "Internal Error" )
      TERP_DEBUG_TRUE_OR_THROW( yStart < yBound, "Internal Error" )        
      
      // finding the touching pixels
      
      edgeLength1 = 0;
      edgeLength2 = 0;
      
      unsigned int xIdx = 0;
      const unsigned int lastColIdx = colsNumber - 1;
      const unsigned int lastLineIdx = linesNumber - 1;
      
      for( unsigned int yIdx = yStart ; yIdx < yBound ; ++yIdx )
      {
        for( xIdx = xStart; xIdx < xBound ; ++xIdx )
        {
          if( segsIds[ yIdx ][ xIdx ] == id1 )
          {
            if( yIdx ) 
              if( segsIds[ yIdx - 1 ][ xIdx ] == id2 )
              {
                ++edgeLength1;
                continue;
              }
            if( xIdx ) 
              if( segsIds[ yIdx ][ xIdx - 1 ] == id2 )
              {
                ++edgeLength1;
                continue;
              }  
            if( yIdx < lastLineIdx) 
              if( segsIds[ yIdx + 1 ][ xIdx ] == id2 )
              {
                ++edgeLength1;
                continue;
              }              
            if( xIdx < lastColIdx ) 
              if( segsIds[ yIdx ][ xIdx + 1 ] == id2 )
              {
                ++edgeLength1;
                continue;
              }              
          }
          else if( segsIds[ yIdx ][ xIdx ] == id2 )
          {
            if( yIdx ) 
              if( segsIds[ yIdx - 1 ][ xIdx ] == id1 )
              {
                ++edgeLength2;
                continue;
              }
            if( xIdx ) 
              if( segsIds[ yIdx ][ xIdx - 1 ] == id1 )
              {
                ++edgeLength2;
                continue;
              }  
            if( yIdx < lastLineIdx) 
              if( segsIds[ yIdx + 1 ][ xIdx ] == id1 )
              {
                ++edgeLength2;
                continue;
              }              
            if( xIdx < lastColIdx ) 
              if( segsIds[ yIdx ][ xIdx + 1 ] == id1 )
              {
                ++edgeLength2;
                continue;
              }              
          }            
        }
      }
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

