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
  \file terralib/rp/SegmenterRegionGrowingMeanMerger.cpp
  \brief Segmenter mean merger used in region growing process.
*/

#include "SegmenterRegionGrowingMeanMerger.h"

namespace te
{
  namespace rp
  {
    SegmenterRegionGrowingMeanMerger::SegmenterRegionGrowingMeanMerger( const unsigned int featuresNumber )
    {
      m_featuresNumber = featuresNumber;
      m_dissimilarityNormFactor = (rg::MeanFeatureType)( 
        2.0 * std::sqrt( (double)featuresNumber ) );
    }
    
    SegmenterRegionGrowingMeanMerger::~SegmenterRegionGrowingMeanMerger()
    {
    }    
    
    SegmenterRegionGrowingSegment< rg::MeanFeatureType >::FeatureType
    SegmenterRegionGrowingMeanMerger::getDissimilarity( SegmenterRegionGrowingSegment< rg::MeanFeatureType > const * const segment1Ptr, 
      SegmenterRegionGrowingSegment< rg::MeanFeatureType > const * const segment2Ptr, 
      SegmenterRegionGrowingSegment< rg::MeanFeatureType > * const ) const
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
    
    void SegmenterRegionGrowingMeanMerger::mergeFeatures( SegmenterRegionGrowingSegment< rg::MeanFeatureType > * const segment1Ptr, 
      SegmenterRegionGrowingSegment< rg::MeanFeatureType > const * const segment2Ptr, 
      SegmenterRegionGrowingSegment< rg::MeanFeatureType > const * const ) const
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
              ((SegmenterRegionGrowingSegment< rg::MeanFeatureType >::FeatureType)segment1Ptr->m_size ) 
            ) 
            +
            ( 
              segment2Ptr->m_features[ meansIdx ] 
              *
              ( (SegmenterRegionGrowingSegment< rg::MeanFeatureType >::FeatureType)segment2Ptr->m_size) 
            )
          )
          / 
          (
            (SegmenterRegionGrowingSegment< rg::MeanFeatureType >::FeatureType)
            ( 
              segment1Ptr->m_size 
              + 
              segment2Ptr->m_size
            )
          );
      }
    }
  } // end namespace rp
}   // end namespace te    
