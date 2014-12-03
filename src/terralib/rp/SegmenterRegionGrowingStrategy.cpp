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
    {
      m_featuresNumber = featuresNumber;
      m_dissimilarityNormFactor = (SegmenterRegionGrowingSegment::FeatureType)( 
        2.0 * std::sqrt( (double)featuresNumber ) );
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
      m_getDissimilarity_dissValue /= m_dissimilarityNormFactor;

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
      
      m_getDissimilarity_sizeSeg1D = 
        (SegmenterRegionGrowingSegment::FeatureType)segment1Ptr->m_size;      
      
      m_getDissimilarity_sizeSeg2D = 
        (SegmenterRegionGrowingSegment::FeatureType)segment2Ptr->m_size;      
      
      m_getDissimilarity_sizeUnionD = m_getDissimilarity_sizeSeg1D + m_getDissimilarity_sizeSeg2D;
      TERP_DEBUG_TRUE_OR_THROW( m_getDissimilarity_sizeUnionD, "Internal error" );
      mergePreviewSegPtr->m_size = (unsigned int)m_getDissimilarity_sizeUnionD;      
      
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
      
      // Finding the form heterogeneity
      
      if( m_colorWeight != 1.0 )
      {
        SegmenterRegionGrowingStrategy::getTouchingEdgeLength(
          m_segmentsIds, mergePreviewSegPtr->m_xStart, 
          mergePreviewSegPtr->m_yStart,
          mergePreviewSegPtr->m_xBound, 
          mergePreviewSegPtr->m_yBound, 
          segment1Ptr->m_id,
          segment2Ptr->m_id,
          m_getDissimilarity_touchingEdgeLength1,
          m_getDissimilarity_touchingEdgeLength2 );
        
        BAATZ_EL( mergePreviewSegPtr->m_features ) = 
          BAATZ_EL( segment1Ptr->m_features ) - ( (SegmenterRegionGrowingSegment::FeatureType)m_getDissimilarity_touchingEdgeLength1 )
          +
          BAATZ_EL( segment2Ptr->m_features ) - ( (SegmenterRegionGrowingSegment::FeatureType)m_getDissimilarity_touchingEdgeLength2 );
        
        BAATZ_CO( mergePreviewSegPtr->m_features ) = (SegmenterRegionGrowingSegment::FeatureType)(
          BAATZ_EL( mergePreviewSegPtr->m_features ) /
          std::sqrt( m_getDissimilarity_sizeUnionD ) );
          
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
          
        m_getDissimilarity_hCompact = 
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
                  m_getDissimilarity_sizeSeg1D
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
                  m_getDissimilarity_sizeSeg2D
                )
              )
              /
              m_getDissimilarity_sizeUnionD
            )
          );
        
        m_getDissimilarity_hSmooth =
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
                  m_getDissimilarity_sizeSeg1D
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
                  m_getDissimilarity_sizeSeg2D
                )
              )
              /
              m_getDissimilarity_sizeUnionD
            )
          );
          
        m_getDissimilarity_hForm = 
          (
            (
              m_compactnessWeight 
              *
              m_getDissimilarity_hCompact
            )
            +
            (
              ( 1.0f - m_compactnessWeight )
              *
              m_getDissimilarity_hSmooth
            )
          );
      }
      
      // Finding the color heterogeneity
      
      if( m_colorWeight != 0.0 )
      {
        m_getDissimilarity_hColor = 0;
        
        for( m_getDissimilarity_sumsIdx = 0 ; m_getDissimilarity_sumsIdx < m_bandsNumber ; ++m_getDissimilarity_sumsIdx )
        {
          m_getDissimilarity_sumUnion = 
            BAATZ_SU( segment1Ptr->m_features, m_getDissimilarity_sumsIdx ) 
            + 
            BAATZ_SU( segment2Ptr->m_features, m_getDissimilarity_sumsIdx );
          BAATZ_SU( mergePreviewSegPtr->m_features, m_getDissimilarity_sumsIdx ) = 
            m_getDissimilarity_sumUnion;        
          
          m_getDissimilarity_squaresSumUnion = 
            BAATZ_SS( segment1Ptr->m_features, m_bandsNumber, m_getDissimilarity_sumsIdx ) 
            +
            BAATZ_SS( segment2Ptr->m_features, m_bandsNumber, m_getDissimilarity_sumsIdx );
          BAATZ_SS( mergePreviewSegPtr->m_features, m_bandsNumber, m_getDissimilarity_sumsIdx ) = m_getDissimilarity_squaresSumUnion;
          
          m_getDissimilarity_meanUnion = m_getDissimilarity_sumUnion / m_getDissimilarity_sizeUnionD;
          
          m_getDissimilarity_stdDevUnion =
            std::sqrt(
              std::max(
                (SegmenterRegionGrowingSegment::FeatureType)0
                ,
                (
                  (
                    m_getDissimilarity_squaresSumUnion
                    -
                    (
                      ((SegmenterRegionGrowingSegment::FeatureType)2) * m_getDissimilarity_meanUnion * m_getDissimilarity_sumUnion
                    )
                    +
                    (
                      m_getDissimilarity_sizeUnionD * m_getDissimilarity_meanUnion * m_getDissimilarity_meanUnion
                    )
                  )
                  /
                  m_getDissimilarity_sizeUnionD
                )
              )
            );
          BAATZ_ST( mergePreviewSegPtr->m_features, m_bandsNumber, m_getDissimilarity_sumsIdx ) =
            m_getDissimilarity_stdDevUnion;        
           
          m_getDissimilarity_hColor += 
            ( 
              m_bandsWeights[ m_getDissimilarity_sumsIdx ]
              *
              (
                (
                  (
                    m_getDissimilarity_stdDevUnion
                    +
                    m_allSegsStdDevOffset
                  )
                  *
                  m_allSegsStdDevGain
                )
                -
                (
                  (
                    (
                      (
                        BAATZ_ST( segment1Ptr->m_features, m_bandsNumber, m_getDissimilarity_sumsIdx )
                        +
                        m_allSegsStdDevOffset
                      )
                      *
                      m_allSegsStdDevGain
                    )                    
                    +
                    (
                      (
                        BAATZ_ST( segment2Ptr->m_features, m_bandsNumber, m_getDissimilarity_sumsIdx )
                        +
                        m_allSegsStdDevOffset
                      )
                      *
                      m_allSegsStdDevGain
                    )                    
                  )
                  /
                  m_getDissimilarity_sizeUnionD
                )
              )
            );
        }
      }
      
      m_getDissimilarity_hColor = 
        (
          ( 
            m_getDissimilarity_hColor 
            * 
            m_colorWeight 
          ) 
          + 
          (
            ( 1.0f - m_colorWeight )
            *
            m_getDissimilarity_hForm
          )
        );       
        
      return std::max( m_getDissimilarity_hColor, ((SegmenterRegionGrowingSegment::FeatureType)0) );
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
      
      memcpy( segment1Ptr->m_features, mergePreviewSegPtr->m_features,
        sizeof( SegmenterRegionGrowingSegment::FeatureType ) * getSegmentFeaturesSize() );
    }    
    
    void SegmenterRegionGrowingStrategy::BaatzMerger::update()
    {
      m_update_compactnessMin = 
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();
      m_update_compactnessMax = -1.0f * 
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();

      m_update_smoothnessMin =
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();
      m_update_smoothnessMax = -1.0f *
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();
        
      m_update_stdDevMin =
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();
      m_update_stdDevMax = -1.0f *
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();          
        
      m_update_nRows = m_segmentsMatrix.getLinesNumber();
      m_update_nCols = m_segmentsMatrix.getColumnsNumber();      
      
      for( m_update_row = 0 ; m_update_row < m_update_nRows ; ++m_update_row )
      {
        m_update_segsRowPtr = m_segmentsMatrix[ m_update_row ];
        
        for( m_update_col = 0 ; m_update_col < m_update_nCols ; ++m_update_col )
        {
          if( m_update_segsRowPtr[ m_update_col ].m_status )
          {
            m_update_featuresPtr = m_update_segsRowPtr[ m_update_col ].m_features;
            
            if( m_colorWeight != 1.0 )
            {
              if( m_update_compactnessMin > BAATZ_CO( m_update_featuresPtr ) )
              {
                m_update_compactnessMin = BAATZ_CO( m_update_featuresPtr );        
              }
              if( m_update_compactnessMax < BAATZ_CO( m_update_featuresPtr ) )
              {
                m_update_compactnessMax = BAATZ_CO( m_update_featuresPtr );        
              }
              
              if( m_update_smoothnessMin > BAATZ_SM( m_update_featuresPtr ) )
              {
                m_update_smoothnessMin = BAATZ_SM( m_update_featuresPtr );
              }
              if( m_update_smoothnessMax < BAATZ_SM( m_update_featuresPtr ) )
              {
                m_update_smoothnessMax = BAATZ_SM( m_update_featuresPtr );
              }
            }
            
            if( m_colorWeight != 0.0 )
            {
              m_update_stdDevPtr = BAATZ_STSTARTPTR( m_update_featuresPtr, m_bandsNumber ); 
              
              for( m_update_band = 0 ; m_update_band < m_bandsNumber ; ++m_update_band, ++m_update_stdDevPtr )
              {
                if( m_update_stdDevMin > *m_update_stdDevPtr )
                {
                  m_update_stdDevMin = *m_update_stdDevPtr;
                }
                if( m_update_stdDevMax < *m_update_stdDevPtr )
                {
                  m_update_stdDevMax = *m_update_stdDevPtr;
                }
              }
            }
          }
        }
      }       
      
      if( m_update_compactnessMax == m_update_compactnessMin )
      {
        m_allSegsCompactnessOffset = 0.0;
        
        if( m_update_compactnessMax == 0.0 )
          m_allSegsCompactnessGain = 1.0;
        else
          m_allSegsCompactnessGain = 1.0f / m_update_compactnessMax;
      }
      else
      {
        m_allSegsCompactnessOffset = -1.0f  * m_update_compactnessMin;
        m_allSegsCompactnessGain = 1.0f / ( m_update_compactnessMax - m_update_compactnessMin );
      }
      
      if( m_update_smoothnessMax == m_update_smoothnessMin )
      {
        m_allSegsSmoothnessOffset = 0.0;
        
        if( m_update_smoothnessMax == 0.0 )
          m_allSegsSmoothnessGain = 1.0;
        else
          m_allSegsSmoothnessGain = 1.0f / m_update_smoothnessMax;
      }
      else
      {
        m_allSegsSmoothnessOffset = -1.0f  * m_update_smoothnessMin;
        m_allSegsSmoothnessGain = 1.0f / ( m_update_smoothnessMax - m_update_smoothnessMin );      
      }
      
      if( m_update_stdDevMax == m_update_stdDevMin )
      {
        m_allSegsStdDevOffset = 0.0;
        
        if( m_update_stdDevMax == 0.0 )
          m_allSegsStdDevGain = 1.0;
        else
          m_allSegsStdDevGain = 1.0f / m_update_stdDevMax;
      }
      else
      {
        m_allSegsStdDevOffset = -1.0f  * m_update_stdDevMin;
        m_allSegsStdDevGain = 1.0f / ( m_update_stdDevMax - m_update_stdDevMin );      
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
      
      std::auto_ptr< Merger > mergerPtr;
      
      switch( m_parameters.m_segmentFeatures )
      {
        case Parameters::MeanFeaturesType :
        {
          mergerPtr.reset( new MeanMerger( inputRasterBands.size() ) );
          break;
        }
        case Parameters::BaatzFeaturesType :
        {
          mergerPtr.reset( new BaatzMerger( m_parameters.m_colorWeight,
            m_parameters.m_compactnessWeight, m_parameters.m_bandsWeights,
            m_segmentsIdsMatrix, m_segmentsPool.getSegsMatrix() ) );
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
        block2ProcessInfo, inputRaster, inputRasterBands, inputRasterNoDataValues, 
        inputRasterBandMinValues, inputRasterBandMaxValues ), 
        "Segments initalization error" );
    
      
      // Progress interface
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( enableProgressInterface )
      {
        progressPtr.reset( new te::common::TaskProgress );
        progressPtr->setTotalSteps( 2 * ( m_parameters.m_segmentsSimIncreaseSteps + 1 ) + 1 /* plus merge small segments step */ );
        progressPtr->setMessage( "Segmentation" );
      }          
      
      // Globals
      
      SegmenterRegionGrowingSegment::FeatureType minFoundDissimilarity = 0.0;
      SegmenterRegionGrowingSegment::FeatureType maxFoundDissimilarity = 0.0;
      unsigned int totalMergesNumber = 0;      
      SegmenterRegionGrowingSegment::FeatureType disimilarityThreshold = 0;
      
      // Segmentation loop with enablelocalMutualBestFitting
      
      for( unsigned int segmentsSimIncreaseStep = 0 ; segmentsSimIncreaseStep <=
        m_parameters.m_segmentsSimIncreaseSteps ; ++segmentsSimIncreaseStep )
      {
        if( enableProgressInterface )
        {
          if( ! progressPtr->isActive() ) 
          {
            return false;
          }   
          progressPtr->pulse();
        }
                
        disimilarityThreshold = 
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
          true,
          auxSeg1Ptr, 
          auxSeg2Ptr, 
          auxSeg3Ptr, 
          minFoundDissimilarity, 
          maxFoundDissimilarity,
          totalMergesNumber );
      }
      
      // Segmentation loop without enablelocalMutualBestFitting
      
      for( unsigned int segmentsSimIncreaseStep = 0 ; segmentsSimIncreaseStep <=
        m_parameters.m_segmentsSimIncreaseSteps ; ++segmentsSimIncreaseStep )
      {
        if( enableProgressInterface )
        {
          if( ! progressPtr->isActive() ) 
          {
            return false;
          }   
          progressPtr->pulse();
        }
                
        disimilarityThreshold = 
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
          false,
          auxSeg1Ptr, 
          auxSeg2Ptr, 
          auxSeg3Ptr, 
          minFoundDissimilarity, 
          maxFoundDissimilarity,
          totalMergesNumber );
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
          auxSeg1Ptr, 
          auxSeg2Ptr, 
          auxSeg3Ptr, 
          minFoundDissimilarity, 
          maxFoundDissimilarity,
          totalMergesNumber );        
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
      const std::vector< double >& inputRasterBandMaxValues )
    {
      const unsigned int inputRasterBandsSize = (unsigned int)
        inputRasterBands.size();
        
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
    
    void SegmenterRegionGrowingStrategy::mergeSegments( 
      const SegmenterRegionGrowingSegment::FeatureType disimilarityThreshold,
      const unsigned int maxSegSizeThreshold,
      SegmenterIdsManager& segmenterIdsManager,
      Merger& merger,
      const bool enablelocalMutualBestFitting,
      SegmenterRegionGrowingSegment* auxSeg1Ptr,
      SegmenterRegionGrowingSegment* auxSeg2Ptr,
      SegmenterRegionGrowingSegment* auxSeg3Ptr,
      SegmenterRegionGrowingSegment::FeatureType& minFoundDissimilarity,
      SegmenterRegionGrowingSegment::FeatureType& maxFoundDissimilarity,
      unsigned int& totalMergesNumber )
    {
      minFoundDissimilarity =
        std::numeric_limits< SegmenterRegionGrowingSegment::FeatureType >::max();      
      maxFoundDissimilarity = -1.0 * minFoundDissimilarity;
      totalMergesNumber = 0;
      
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
      unsigned int iterationNumber = 0;
      
      do
      {
        iterationMergedSegmentsNumber = 0;
        
        // Globals
        
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
            
            if( ( currSegPtr->m_status ) && ( currSegPtr->m_size <= internalMaxSegSizeThreshold ) )
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
                    
                  if( 
                      ( forwardDissimilarityValue <= internalDisimilarityThreshold )
                      &&
                      ( forwardDissimilarityValue < minForwardDissimilarityValue )
                    )
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
              
              // if the min forward dissimilarity segment was found
              
              if( minForwardDissimilaritySegmentPtr )
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
                
                ++iterationMergedSegmentsNumber;
                ++totalMergesNumber;
              }
            }
          }
        }
        
        // give back the free unused sement ids
        
        if( ! freeSegmentIds.empty() )
        {
          segmenterIdsManager.addFreeIDs( freeSegmentIds );
        }
        
//         std::cout << std::endl << "Iteration Number: " << iterationNumber <<
//           "  Merged segments number:" << iterationMergedSegmentsNumber << std::endl;
        
        ++iterationNumber;
      }
      while( iterationMergedSegmentsNumber );
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

