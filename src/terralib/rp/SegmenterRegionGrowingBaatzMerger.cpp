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
  \file terralib/rp/SegmenterRegionGrowingBaatzMerger.cpp
  \brief Segmenter Baatz merger used in region growing process.
*/

#include "SegmenterRegionGrowingBaatzMerger.h"
#include "Macros.h"

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

namespace te
{
  namespace rp
  {
    SegmenterRegionGrowingBaatzMerger::SegmenterRegionGrowingBaatzMerger(
      const double& colorWeight, const double& compactnessWeight,
      const std::vector< double >& bandsWeights,
      const SegmentsIdsMatrixT& segmentsIds )
      : 
        m_segmentsIds( segmentsIds ),
        m_allSegsCompactnessOffset( 0 ),
        m_allSegsCompactnessGain( 1.0 ),
        m_allSegsSmoothnessOffset( 0 ),
        m_allSegsSmoothnessGain( 0 ),
        m_colorWeight( (rg::BaatzFeatureType)colorWeight ),
        m_compactnessWeight( (rg::BaatzFeatureType)compactnessWeight )
    {
      m_bandsNumber = (unsigned int)bandsWeights.size();
      
      m_bandsWeights.resize( m_bandsNumber, 1 );
      
      for( unsigned int band = 0 ; band < m_bandsNumber ; ++band )
      {
        m_bandsWeights[ band ] = (rg::BaatzFeatureType)bandsWeights[ band ];
      }
    }
    
    SegmenterRegionGrowingBaatzMerger::~SegmenterRegionGrowingBaatzMerger()
    {
    }    
    
    DissimilarityTypeT
    SegmenterRegionGrowingBaatzMerger::getDissimilarity( SegmenterRegionGrowingSegment< rg::BaatzFeatureType > const * const segment1Ptr, 
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType > const * const segment2Ptr, 
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType > * const mergePreviewSegPtr ) const
    {
      assert( segment1Ptr );
      assert( segment1Ptr->m_features );
      assert( segment2Ptr );
      assert( segment2Ptr->m_features );      
      assert( mergePreviewSegPtr );
      
      // globals
      
      m_getDissimilarity_sizeSeg1D = (rg::BaatzFeatureType)segment1Ptr->m_size;      
      
      m_getDissimilarity_sizeSeg2D = (rg::BaatzFeatureType)segment2Ptr->m_size;      
      
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
        getTouchingEdgeLength(
          m_segmentsIds, mergePreviewSegPtr->m_xStart, 
          mergePreviewSegPtr->m_yStart,
          mergePreviewSegPtr->m_xBound, 
          mergePreviewSegPtr->m_yBound, 
          segment1Ptr->m_id,
          segment2Ptr->m_id,
          m_getDissimilarity_touchingEdgeLength1,
          m_getDissimilarity_touchingEdgeLength2 );
        
        BAATZ_EL( mergePreviewSegPtr->m_features ) = 
          BAATZ_EL( segment1Ptr->m_features ) - ( (rg::BaatzFeatureType)m_getDissimilarity_touchingEdgeLength1 )
          +
          BAATZ_EL( segment2Ptr->m_features ) - ( (rg::BaatzFeatureType)m_getDissimilarity_touchingEdgeLength2 );
        
        BAATZ_CO( mergePreviewSegPtr->m_features ) = (rg::BaatzFeatureType)(
          BAATZ_EL( mergePreviewSegPtr->m_features ) /
          std::sqrt( m_getDissimilarity_sizeUnionD ) );
          
        BAATZ_SM( mergePreviewSegPtr->m_features ) =
          BAATZ_EL( mergePreviewSegPtr->m_features ) 
          /
          (rg::BaatzFeatureType)(
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
                (rg::BaatzFeatureType)0
                ,
                (
                  (
                    m_getDissimilarity_squaresSumUnion
                    -
                    (
                      ((rg::BaatzFeatureType)2) * m_getDissimilarity_meanUnion * m_getDissimilarity_sumUnion
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
        
      return std::max( (DissimilarityTypeT)m_getDissimilarity_hColor, (DissimilarityTypeT)0 );
    }
    
    void SegmenterRegionGrowingBaatzMerger::mergeFeatures( 
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType > * const segment1Ptr, 
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType > const * const segment2Ptr , 
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType > const * const mergePreviewSegPtr ) const
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
        sizeof( rg::BaatzFeatureType ) * getSegmentFeaturesSize() );
    }    
    
    void SegmenterRegionGrowingBaatzMerger::update(
      SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* const actSegsListHeadPtr )
    {
      m_update_compactnessMin = 
        std::numeric_limits< rg::BaatzFeatureType >::max();
      m_update_compactnessMax = -1.0f * 
        std::numeric_limits< rg::BaatzFeatureType >::max();

      m_update_smoothnessMin =
        std::numeric_limits< rg::BaatzFeatureType >::max();
      m_update_smoothnessMax = -1.0f *
        std::numeric_limits< rg::BaatzFeatureType >::max();
        
      m_update_stdDevMin =
        std::numeric_limits< rg::BaatzFeatureType >::max();
      m_update_stdDevMax = -1.0f *
        std::numeric_limits< rg::BaatzFeatureType >::max();          
        
      m_update_currentActSegPtr = actSegsListHeadPtr;
      
      while( m_update_currentActSegPtr )
      {
        m_update_featuresPtr = m_update_currentActSegPtr->m_features;
        
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
        
        m_update_currentActSegPtr = m_update_currentActSegPtr->m_nextActiveSegment;
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

    void SegmenterRegionGrowingBaatzMerger::getTouchingEdgeLength( 
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
  } // end namespace rp
}   // end namespace te    

