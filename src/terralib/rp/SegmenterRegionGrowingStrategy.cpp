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

#include <boost/lexical_cast.hpp>

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
      m_colorWeight = 0.5;
      m_compactnessWeight = 0.5;
      m_segmentsSimIncreaseSteps = 10;
    }
    
    te::common::AbstractParameters* SegmenterRegionGrowingStrategy::Parameters::clone() const
    {
      return new te::rp::SegmenterRegionGrowingStrategy::Parameters( *this );
    }
    
    //-------------------------------------------------------------------------

    SegmenterRegionGrowingStrategy::SegmentFeatures::SegmentFeatures()
    {
    }

    SegmenterRegionGrowingStrategy::SegmentFeatures::~SegmentFeatures()
    {
    }
    
    const SegmenterRegionGrowingStrategy::SegmentFeatures& 
      SegmenterRegionGrowingStrategy::SegmentFeatures::operator=(
      const SegmenterRegionGrowingStrategy::SegmentFeatures& other )
    {
      m_id = other.m_id;
      m_size = other.m_size;
      m_xStart = other.m_xStart;
      m_xBound = other.m_xBound;
      m_yStart = other.m_yStart;
      m_yBound = other.m_yBound;
      
      return other;
    }
    
    //-------------------------------------------------------------------------

    SegmenterRegionGrowingStrategy::Segment::Segment()
    {
    }

    SegmenterRegionGrowingStrategy::Segment::~Segment()
    {
    }  

     //-------------------------------------------------------------------------
     
    SegmenterRegionGrowingStrategy::SegmentsIndexer::SegmentsIndexer( 
      SegmenterSegmentsPool& segmentsPool )
      : std::map< SegmenterSegmentsBlock::SegmentIdDataType, Segment* >(),
        m_segmentsPool( segmentsPool )
    {
    }      
     
    SegmenterRegionGrowingStrategy::SegmentsIndexer::~SegmentsIndexer()
    {
      clear();
    }   
    
    void SegmenterRegionGrowingStrategy::SegmentsIndexer::clear()
    {
      iterator segmentsIt = begin();
      iterator segmentsItEnd = end();
      
      while( segmentsIt != segmentsItEnd )
      {
        m_segmentsPool.store(segmentsIt->second);
        
        ++segmentsIt;
      }
      
      std::map< SegmenterSegmentsBlock::SegmentIdDataType, Segment* >::clear();
    }
    
    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategy::MeanBasedSegment::SegmentFeatures::SegmentFeatures()
    {
    }

    SegmenterRegionGrowingStrategy::MeanBasedSegment::SegmentFeatures::~SegmentFeatures()
    {
    }

    SegmenterRegionGrowingStrategy::SegmentFeatures*
      SegmenterRegionGrowingStrategy::MeanBasedSegment::SegmentFeatures::clone()
      const
    {
      SegmenterRegionGrowingStrategy::MeanBasedSegment::SegmentFeatures*
        newInstancePtr = new
        SegmenterRegionGrowingStrategy::MeanBasedSegment::SegmentFeatures();
      
      newInstancePtr->copy( this );
        
      return newInstancePtr;
    }
    
    void SegmenterRegionGrowingStrategy::MeanBasedSegment::SegmentFeatures::copy( 
      SegmenterRegionGrowingStrategy::SegmentFeatures const * const otherPtr )
    {
      SegmenterRegionGrowingStrategy::MeanBasedSegment::SegmentFeatures const * const
        otherCastPtr = dynamic_cast< 
        SegmenterRegionGrowingStrategy::MeanBasedSegment::SegmentFeatures const * const >(
        otherPtr );
      TERP_DEBUG_TRUE_OR_THROW( otherCastPtr, "Invalid segment feature type" );  
        
      m_means = otherCastPtr->m_means;

      SegmenterRegionGrowingStrategy::SegmentFeatures::operator=( *otherPtr );
    };
    
    //-------------------------------------------------------------------------

    SegmenterRegionGrowingStrategy::MeanBasedSegment::MeanBasedSegment()
    {
    }

    SegmenterRegionGrowingStrategy::MeanBasedSegment::~MeanBasedSegment()
    {
    }

    //-------------------------------------------------------------------------

    SegmenterRegionGrowingStrategy::BaatzBasedSegment::SegmentFeatures::SegmentFeatures()
    {
    }

    SegmenterRegionGrowingStrategy::BaatzBasedSegment::SegmentFeatures::~SegmentFeatures()
    {
    }
    
    SegmenterRegionGrowingStrategy::SegmentFeatures*
      SegmenterRegionGrowingStrategy::BaatzBasedSegment::SegmentFeatures::clone()
      const
    {
      SegmenterRegionGrowingStrategy::BaatzBasedSegment::SegmentFeatures*
        newInstancePtr = new
        SegmenterRegionGrowingStrategy::BaatzBasedSegment::SegmentFeatures();
      
      newInstancePtr->copy( this );
        
      return newInstancePtr;
    }    
    
    void SegmenterRegionGrowingStrategy::BaatzBasedSegment::SegmentFeatures::copy( 
      SegmenterRegionGrowingStrategy::SegmentFeatures const * const otherPtr )
    {
      SegmenterRegionGrowingStrategy::BaatzBasedSegment::SegmentFeatures const * const
        otherCastPtr = dynamic_cast< 
        SegmenterRegionGrowingStrategy::BaatzBasedSegment::SegmentFeatures const * const >(
        otherPtr );
      TERP_DEBUG_TRUE_OR_THROW( otherCastPtr, "Invalid segment feature type" );  
        
      m_sums = otherCastPtr->m_sums;
	  m_squaresSum = otherCastPtr->m_squaresSum;
      m_stdDev = otherCastPtr->m_stdDev;
      m_edgeLength = otherCastPtr->m_edgeLength;
      m_compactness = otherCastPtr->m_compactness;
      m_smoothness = otherCastPtr->m_smoothness;

      SegmenterRegionGrowingStrategy::SegmentFeatures::operator=( *otherPtr );
    };    
    
    //-------------------------------------------------------------------------

    SegmenterRegionGrowingStrategy::BaatzBasedSegment::BaatzBasedSegment()
    {
    }

    SegmenterRegionGrowingStrategy::BaatzBasedSegment::~BaatzBasedSegment()
    {
    }

    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategy::Merger::Merger()
    {
    }
    
    SegmenterRegionGrowingStrategy::Merger::~Merger()
    {
    } 

    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategy::MeanMerger::MeanMerger()
    {
    }
    
    SegmenterRegionGrowingStrategy::MeanMerger::~MeanMerger()
    {
    }    
    
    double SegmenterRegionGrowingStrategy::MeanMerger::getDissimilarityIndex(
      SegmenterRegionGrowingStrategy::Segment const * const segment1Ptr, 
      SegmenterRegionGrowingStrategy::Segment const * const segment2Ptr, 
      SegmenterRegionGrowingStrategy::SegmentFeatures* ) const
    {
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< MeanBasedSegment const * const >(
        segment1Ptr ), "Invalid segment type" );       
      MeanBasedSegment const * const segment1CastPtr = 
        (MeanBasedSegment*)(segment1Ptr);
        
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< MeanBasedSegment const * const >(
        segment2Ptr ), "Invalid segment type" );       
      MeanBasedSegment const * const segment2CastPtr = 
        (MeanBasedSegment*)(segment2Ptr);
      
      std::vector< double >::size_type meansSize = 
        segment1CastPtr->m_features.m_means.size();
      TERP_DEBUG_TRUE_OR_THROW( meansSize ==
        segment2CastPtr->m_features.m_means.size(),
        "Internal error" );
        
      double dissValue = 0.0;
      double diffValue = 0.0;
        
      for( std::vector< double >::size_type meansIdx = 0 ; meansIdx < 
        meansSize ; ++meansIdx )
      {
        diffValue = segment1CastPtr->m_features.m_means[ meansIdx ] - 
          segment2CastPtr->m_features.m_means[ meansIdx ];
          
        dissValue += ( diffValue * diffValue );
      }
        
      return sqrt( dissValue );
    }
    
    void SegmenterRegionGrowingStrategy::MeanMerger::mergeFeatures( 
      SegmenterRegionGrowingStrategy::Segment * const segment1Ptr, 
      Segment const * const segment2Ptr, 
      SegmenterRegionGrowingStrategy::SegmentFeatures const * const ) const
    {
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< MeanBasedSegment * const >(
        segment1Ptr ), "Invalid segment type" );       
      MeanBasedSegment * const segment1CastPtr = 
        (MeanBasedSegment*)(segment1Ptr);
        
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< MeanBasedSegment const * const >(
        segment2Ptr ), "Invalid segment type" );       
      MeanBasedSegment const * const segment2CastPtr = 
        (MeanBasedSegment*)(segment2Ptr);
        
      // Merging basic features
      
      segment1CastPtr->m_features.m_size += segment2CastPtr->m_features.m_size;
      
      segment1CastPtr->m_features.m_xStart = std::min( 
        segment1CastPtr->m_features.m_xStart, 
        segment2CastPtr->m_features.m_xStart );
      segment1CastPtr->m_features.m_xBound = std::max( 
        segment1CastPtr->m_features.m_xBound, 
        segment2CastPtr->m_features.m_xBound );

      segment1CastPtr->m_features.m_yStart = std::min( 
        segment1CastPtr->m_features.m_yStart, 
        segment2CastPtr->m_features.m_yStart );
      segment1CastPtr->m_features.m_yBound = std::max( 
        segment1CastPtr->m_features.m_yBound, 
        segment2CastPtr->m_features.m_yBound );
        
      // Merging specific features
        
      const std::vector< double >::size_type meansSize = 
        segment1CastPtr->m_features.m_means.size();
      TERP_DEBUG_TRUE_OR_THROW( meansSize ==
        segment2CastPtr->m_features.m_means.size(),
        "Internal error" );
        
      for( std::vector< double >::size_type meansIdx = 0 ; meansIdx < 
        meansSize ; ++meansIdx )
      {
        segment1CastPtr->m_features.m_means[ meansIdx ] = 
          (
            ( 
              segment1CastPtr->m_features.m_means[ meansIdx ] 
              * 
              ((double)segment1CastPtr->m_features.m_size ) 
            ) 
            +
            ( 
              segment2CastPtr->m_features.m_means[ meansIdx ] 
              *
              ( (double)segment2CastPtr->m_features.m_size) 
            )
          )
          / 
          (
            (double)
            ( segment1CastPtr->m_features.m_size 
              + 
              segment2CastPtr->m_features.m_size
            )
          );
      }
    }
    
    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategy::BaatzMerger::BaatzMerger(
      const double& colorWeight, const double& compactnessWeight,
      const std::vector< double >& bandsWeights,
      const SegmentsIdsMatrixT& segmentsIds,
      const SegmenterRegionGrowingStrategy::SegmentsIndexer& segments)
      : 
        m_allSegsCompactnessOffset( 0 ),
        m_allSegsCompactnessGain( 1.0 ),
        m_allSegsSmoothnessOffset( 0 ),
        m_allSegsSmoothnessGain( 0 ),
        m_colorWeight( colorWeight ),
        m_compactnessWeight( compactnessWeight ),
        m_bandsWeights( bandsWeights ),
        m_segmentsIds( segmentsIds ),
        m_segments( segments )
    {
      m_allSegsStdDevOffsets.resize( bandsWeights.size(), 0 );
      m_allSegsStdDevGain.resize( bandsWeights.size(), 1 );
    }
    
    SegmenterRegionGrowingStrategy::BaatzMerger::~BaatzMerger()
    {
    }    
    
    double SegmenterRegionGrowingStrategy::BaatzMerger::getDissimilarityIndex(
      SegmenterRegionGrowingStrategy::Segment const * const segment1Ptr, 
      SegmenterRegionGrowingStrategy::Segment const * const segment2Ptr, 
      SegmenterRegionGrowingStrategy::SegmentFeatures* mergedFeatures ) const
    {
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< BaatzBasedSegment const * const >(
        segment1Ptr ), "Invalid segment type" );       
      BaatzBasedSegment const * const segment1CastPtr = 
        (BaatzBasedSegment*)(segment1Ptr);
        
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< BaatzBasedSegment const * const >(
        segment2Ptr ), "Invalid segment type" );       
      BaatzBasedSegment const * const segment2CastPtr = 
        (BaatzBasedSegment*)(segment2Ptr);
      
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< BaatzBasedSegment::SegmentFeatures* >(
        mergedFeatures ), "Invalid features type" );
      BaatzBasedSegment::SegmentFeatures*  mergedFeaturesCastPtr = 
        (BaatzBasedSegment::SegmentFeatures*)(mergedFeatures);
        
      TERP_DEBUG_TRUE_OR_THROW( m_bandsWeights.size() == 
        segment1CastPtr->m_features.m_sums.size(),
        "Internal error" );
      
      const unsigned int sumsSize = segment1CastPtr->m_features.m_sums.size();
      TERP_DEBUG_TRUE_OR_THROW( sumsSize ==
        segment1CastPtr->m_features.m_stdDev.size(),
        "Internal error" );       
      TERP_DEBUG_TRUE_OR_THROW( sumsSize ==
        segment2CastPtr->m_features.m_sums.size(),
        "Internal error" );       
      TERP_DEBUG_TRUE_OR_THROW( sumsSize ==
        segment2CastPtr->m_features.m_stdDev.size(),
        "Internal error" );         
        
      mergedFeaturesCastPtr->m_sums.resize( sumsSize );
      mergedFeaturesCastPtr->m_squaresSum.resize( sumsSize );
      mergedFeaturesCastPtr->m_stdDev.resize( sumsSize );
      
      // globals
      
      mergedFeaturesCastPtr->m_size = segment1CastPtr->m_features.m_size + 
        segment2CastPtr->m_features.m_size;
      TERP_DEBUG_TRUE_OR_THROW( mergedFeaturesCastPtr->m_size,
        "Internal error" );
      const double sizeUnionD = (double)mergedFeaturesCastPtr->m_size;
      
      const double sizeSeg1D = (double)segment1CastPtr->m_features.m_size;      
      
      const double sizeSeg2D = (double)segment2CastPtr->m_features.m_size;      
      
      // Finding the form heterogeneity
      
      mergedFeaturesCastPtr->m_xStart = std::min( segment1CastPtr->m_features.m_xStart,
        segment2CastPtr->m_features.m_xStart );
      mergedFeaturesCastPtr->m_yStart = std::min( segment1CastPtr->m_features.m_yStart,
        segment2CastPtr->m_features.m_yStart );        
      mergedFeaturesCastPtr->m_xBound = std::max( segment1CastPtr->m_features.m_xBound,
        segment2CastPtr->m_features.m_xBound );      
      mergedFeaturesCastPtr->m_yBound = std::max( segment1CastPtr->m_features.m_yBound,
        segment2CastPtr->m_features.m_yBound ); 
        
      TERP_DEBUG_TRUE_OR_THROW( 
        ( mergedFeaturesCastPtr->m_xBound > mergedFeaturesCastPtr->m_xStart ),
        "Internal error" )
      TERP_DEBUG_TRUE_OR_THROW( 
        ( mergedFeaturesCastPtr->m_yBound > mergedFeaturesCastPtr->m_yStart ),
        "Internal error" )        
        
      unsigned int touchingEdgeLength1 = 0;
      unsigned int touchingEdgeLength2 = 0;
      SegmenterRegionGrowingStrategy::getTouchingEdgeLength(
        m_segmentsIds, mergedFeaturesCastPtr->m_xStart, 
        mergedFeaturesCastPtr->m_yStart,
        mergedFeaturesCastPtr->m_xBound, 
        mergedFeaturesCastPtr->m_yBound, 
        segment1CastPtr->m_features.m_id,
        segment2CastPtr->m_features.m_id,
        touchingEdgeLength1,
        touchingEdgeLength2 );
      
      mergedFeaturesCastPtr->m_edgeLength = 
        segment1CastPtr->m_features.m_edgeLength - touchingEdgeLength1 
        +
        segment2CastPtr->m_features.m_edgeLength - touchingEdgeLength2;
      
      mergedFeaturesCastPtr->m_compactness = 
        ((double)mergedFeaturesCastPtr->m_edgeLength) /
        std::sqrt( sizeUnionD );
        
      mergedFeaturesCastPtr->m_smoothness =
        ((double)mergedFeaturesCastPtr->m_edgeLength) 
        /
        (double)(
          (
            2 * ( mergedFeaturesCastPtr->m_xBound - mergedFeaturesCastPtr->m_xStart )
          )
          +
          (
            2 * ( mergedFeaturesCastPtr->m_yBound - mergedFeaturesCastPtr->m_yStart )
          )
        );
        
      const double hCompact = 
        (
          std::abs(
            mergedFeaturesCastPtr->m_compactness
            -
            (
              (
                (
                  segment1CastPtr->m_features.m_compactness
                  *
                  sizeSeg1D
                )
                +
                (
                  segment2CastPtr->m_features.m_compactness
                  *
                  sizeSeg2D
                )
              )
              /
              sizeUnionD
            )
          )
          +
          m_allSegsCompactnessOffset
        )
        *
        m_allSegsCompactnessGain;
      
      const double hSmooth =
        (
          std::abs(
            mergedFeaturesCastPtr->m_smoothness
            -
            (
              (
                (
                  segment1CastPtr->m_features.m_smoothness
                  *
                  sizeSeg1D
                )
                +
                (
                  segment2CastPtr->m_features.m_smoothness
                  *
                  sizeSeg2D
                )
              )
              /
              sizeUnionD
            )
          )
          +
          m_allSegsSmoothnessOffset
        )
        *
        m_allSegsSmoothnessGain;
        
      const double hForm = 
        (
          (
            m_compactnessWeight 
            *
            hCompact
          )
          +
          (
            ( 1.0 - m_compactnessWeight )
            *
            hSmooth
          )
        );
      
      // Finding the color heterogeneity
      
      double hColor = 0;
      double sumUnion = 0;
      double squaresSumUnion = 0;
      double meanUnion = 0;
      double stdDevUnion = 0.0;      
      
      for( unsigned int sumsIdx = 0 ; sumsIdx < sumsSize ; ++sumsIdx )
      {
        const double& sum1 = segment1CastPtr->m_features.m_sums[ sumsIdx ];
        
        const double& sum2 = segment2CastPtr->m_features.m_sums[ sumsIdx ];
        
        sumUnion = sum1 + sum2;
        mergedFeaturesCastPtr->m_sums[ sumsIdx ] = sumUnion;        
        
        squaresSumUnion = segment1CastPtr->m_features.m_squaresSum[ sumsIdx ] +
          segment2CastPtr->m_features.m_squaresSum[ sumsIdx ];
        mergedFeaturesCastPtr->m_squaresSum[ sumsIdx ] = squaresSumUnion;
        
        meanUnion = ( sum1 + sum2 ) / sizeUnionD;
        
        stdDevUnion =
          (
            (
              squaresSumUnion
              -
              (
                2.0 * meanUnion * sumUnion
              )
              +
              (
                sizeUnionD * meanUnion * meanUnion
              )
            )
            /
            sizeUnionD
          );
        mergedFeaturesCastPtr->m_stdDev[ sumsIdx ] = stdDevUnion;        
         
        hColor += 
          ( 
            m_bandsWeights[ sumsIdx ]
            *
            (
              (
                std::abs(
                  stdDevUnion
                  -
                  (
                    (
                      (
                        segment1CastPtr->m_features.m_stdDev[ sumsIdx ]
                        *
                        sizeSeg1D
                      )
                      +
                      (
                        segment2CastPtr->m_features.m_stdDev[ sumsIdx ]
                        *
                        sizeSeg2D
                      )
                    )
                    /
                    sizeUnionD
                  )
                )
                +
                m_allSegsStdDevOffsets[ sumsIdx ]
              )
              *
              m_allSegsStdDevGain[ sumsIdx ]
            )
          );
      }
      
      return
        (
          ( 
            hColor 
            * 
            m_colorWeight 
          ) 
          + 
          (
            ( 1.0 - m_colorWeight )
            *
            hForm
          )
        );       
    }
    
    void SegmenterRegionGrowingStrategy::BaatzMerger::mergeFeatures( 
      SegmenterRegionGrowingStrategy::Segment * const segment1Ptr, 
      Segment const * const , 
      SegmenterRegionGrowingStrategy::SegmentFeatures const * const mergedFeatures ) const
    {
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< BaatzBasedSegment * const >(
        segment1Ptr ), "Invalid segment type" );       
      BaatzBasedSegment * const segment1CastPtr = 
        (BaatzBasedSegment*)(segment1Ptr);
      
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< BaatzBasedSegment::SegmentFeatures const * const>(
        mergedFeatures ), "Invalid features type" );
      BaatzBasedSegment::SegmentFeatures const * const  mergedFeaturesCastPtr = 
        (BaatzBasedSegment::SegmentFeatures const * const)(mergedFeatures);
      
      // Merging basic features
      
      segment1CastPtr->m_features.m_size = mergedFeaturesCastPtr->m_size;
      segment1CastPtr->m_features.m_xStart = mergedFeaturesCastPtr->m_xStart;
      segment1CastPtr->m_features.m_xBound = mergedFeaturesCastPtr->m_xBound;
      segment1CastPtr->m_features.m_yStart = mergedFeaturesCastPtr->m_yStart;
      segment1CastPtr->m_features.m_yBound = mergedFeaturesCastPtr->m_yBound;
        
      // Merging specific features   
      
      segment1CastPtr->m_features.m_sums = mergedFeaturesCastPtr->m_sums;
      segment1CastPtr->m_features.m_squaresSum = mergedFeaturesCastPtr->m_squaresSum;
      segment1CastPtr->m_features.m_stdDev = mergedFeaturesCastPtr->m_stdDev;
      segment1CastPtr->m_features.m_edgeLength = mergedFeaturesCastPtr->m_edgeLength;
      segment1CastPtr->m_features.m_compactness = mergedFeaturesCastPtr->m_compactness;
      segment1CastPtr->m_features.m_smoothness = mergedFeaturesCastPtr->m_smoothness;
    }    
    
    void SegmenterRegionGrowingStrategy::BaatzMerger::update()
    {
      SegmenterRegionGrowingStrategy::SegmentsIndexer::const_iterator itB =
        m_segments.begin();
      const SegmenterRegionGrowingStrategy::SegmentsIndexer::const_iterator itE =
        m_segments.end();        
      std::vector< double >::size_type dimIdx = 0;
      std::vector< double >::size_type dimsNumber = m_bandsWeights.size();
      
      std::vector< double > stdDevMin( dimsNumber, DBL_MAX );
      std::vector< double > stdDevMax( dimsNumber, -1.0 * DBL_MAX );
      
      double compactnessMin = DBL_MAX;
      double compactnessMax = -1.0 * DBL_MAX;

      double smoothnessMin = DBL_MAX;
      double smoothnessMax = -1.0 * DBL_MAX;
      
      SegmenterRegionGrowingStrategy::BaatzBasedSegment const * segmentCastPtr = 0;
        
      while( itB != itE )
      {
        TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< BaatzBasedSegment const * >(
          itB->second ), "Invalid segment type" );       
        segmentCastPtr = (BaatzBasedSegment const*)(itB->second);    
        TERP_DEBUG_TRUE_OR_THROW( segmentCastPtr->m_features.m_stdDev.size()
          == dimsNumber, "Invalid dims number" )
          
        for( dimIdx = 0; dimIdx < dimsNumber ; ++dimIdx )
        {
          if( stdDevMin[ dimIdx ] > segmentCastPtr->m_features.m_stdDev[ dimIdx ] )
            stdDevMin[ dimIdx ] = segmentCastPtr->m_features.m_stdDev[ dimIdx ];
          if( stdDevMax[ dimIdx ] < segmentCastPtr->m_features.m_stdDev[ dimIdx ] )
            stdDevMax[ dimIdx ] = segmentCastPtr->m_features.m_stdDev[ dimIdx ];          
        }
        
        if( compactnessMin > segmentCastPtr->m_features.m_compactness )
          compactnessMin = segmentCastPtr->m_features.m_compactness;        
        if( compactnessMax < segmentCastPtr->m_features.m_compactness )
          compactnessMax = segmentCastPtr->m_features.m_compactness;        
        
        if( smoothnessMin > segmentCastPtr->m_features.m_smoothness )
          smoothnessMin = segmentCastPtr->m_features.m_smoothness;
        if( smoothnessMax < segmentCastPtr->m_features.m_smoothness )
          smoothnessMax = segmentCastPtr->m_features.m_smoothness;
        
        ++itB;
      }
      
      for( dimIdx = 0; dimIdx < dimsNumber ; ++dimIdx )
      {
        if( stdDevMax[ dimIdx ] == stdDevMin[ dimIdx ] )
        {
          m_allSegsStdDevOffsets[ dimIdx ] = 0.0;
          
          if( stdDevMax[ dimIdx ] == 0.0 )
            m_allSegsStdDevGain[ dimIdx ] = 1.0;
          else
            m_allSegsStdDevGain[ dimIdx ] = 1.0 / stdDevMax[ dimIdx ];
        }
        else
        {
          m_allSegsStdDevOffsets[ dimIdx ] = -1.0 * stdDevMin[ dimIdx ];
          m_allSegsStdDevGain[ dimIdx ] = 1.0 / 
            ( stdDevMax[ dimIdx ] - stdDevMin[ dimIdx ] );
        }
      }      
      
      if( compactnessMax == compactnessMin )
      {
        m_allSegsCompactnessOffset = 0.0;
        
        if( compactnessMax == 0.0 )
          m_allSegsCompactnessGain = 1.0;
        else
          m_allSegsCompactnessGain = 1.0 / compactnessMax;
      }
      else
      {
        m_allSegsCompactnessOffset = -1.0  * compactnessMin;
        m_allSegsCompactnessGain = 1.0 / ( compactnessMax - compactnessMin );
      }
      
      if( smoothnessMax == smoothnessMin )
      {
        m_allSegsSmoothnessOffset = 0.0;
        
        if( smoothnessMax == 0.0 )
          m_allSegsSmoothnessGain = 1.0;
        else
          m_allSegsSmoothnessGain = 1.0 / smoothnessMax;
      }
      else
      {
        m_allSegsSmoothnessOffset = -1.0  * smoothnessMin;
        m_allSegsSmoothnessGain = 1.0 / ( smoothnessMax - smoothnessMin );      
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
          
        if( ! m_parameters.m_bandsWeights.empty() )
        {
          TERP_TRUE_OR_RETURN_FALSE( paramsPtr->m_bandsWeights.size(),
            "Invalid segmenter strategy parameter m_bandsWeights" );
          double bandsWeightsSum = 0;
          for( unsigned int bandsWeightsIdx = 0 ; bandsWeightsIdx < 
            paramsPtr->m_bandsWeights.size() ; ++bandsWeightsIdx )
          {
            bandsWeightsSum += paramsPtr->m_bandsWeights[ bandsWeightsIdx ];
          }
          TERP_TRUE_OR_RETURN_FALSE( bandsWeightsSum == 1.0,
            "Invalid segmenter strategy parameter m_bandsWeights" );        
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
    };
    
    bool SegmenterRegionGrowingStrategy::execute( 
      SegmenterIdsManager& segmenterIdsManager,
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
        
      // Initializing segments
        
      SegmentsIndexer segmentsIndexer( m_segmentsPool );
      TERP_TRUE_OR_RETURN_FALSE( initializeSegments( segmenterIdsManager,
        inputRaster, inputRasterBands, inputRasterGains,
        inputRasterOffsets, segmentsIndexer ), 
        "Segments initalization error" );
        
      // Creating the merger instance
      
      std::auto_ptr< Merger > mergerPtr;
      bool enablelocalMutualBestFitting = false;
      
      switch( m_parameters.m_segmentFeatures )
      {
        case Parameters::MeanFeaturesType :
        {
          mergerPtr.reset( new MeanMerger() );
          enablelocalMutualBestFitting = true;
          break;
        }
        case Parameters::BaatzFeaturesType :
        {
          mergerPtr.reset( new BaatzMerger( m_parameters.m_colorWeight,
            m_parameters.m_compactnessWeight, m_parameters.m_bandsWeights,
            m_segmentsIdsMatrix, segmentsIndexer ) );
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
        
      double similarityThreshold = m_parameters.m_segmentsSimilarityThreshold / 
        (double)( m_parameters.m_segmentsSimIncreaseSteps + 1 );
      unsigned int mergedSegments = 0;
      unsigned int maxMergedSegments = 0;
      unsigned int noMergeIterations = 0;
      int currStep = 0;
      
//      exportSegs2Tif( segmentsIds, true, "merging" + 
//        te::common::Convert2String( mergetIterations ) + ".tif" );
      
      while ( true )
      {
        mergedSegments = mergeSegments( similarityThreshold, segmenterIdsManager, 
          *mergerPtr, enablelocalMutualBestFitting, segmentsIndexer  );
//        exportSegs2Tif( segmentsIds, true, "merging" + 
//          te::common::Convert2String( mergetIterations ) + ".tif" );

        if( enableProgressInterface )
        {
          if( maxMergedSegments )
          {
              currStep = (int)( 100.0 * ( ( (double)( maxMergedSegments - mergedSegments ) ) / 
                ((double)maxMergedSegments ) ) / 2.0 );
            
            if( currStep > progressPtr->getCurrentStep() )
            {
              progressPtr->pulse();
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
          ++noMergeIterations;
          
          if( noMergeIterations > m_parameters.m_segmentsSimIncreaseSteps ) 
          {
            break;
          }
          else
          {
            similarityThreshold += 
              ( 
                ( (double)m_parameters.m_segmentsSimilarityThreshold )
                /
                ( (double)( m_parameters.m_segmentsSimIncreaseSteps + 1 ) )
              );
          }
        }
      }
      
      if( m_parameters.m_minSegmentSize > 1 )
      {
        maxMergedSegments = 0;
        
        while( true )
        {
          mergedSegments = mergeSmallSegments( m_parameters.m_minSegmentSize, 
            segmenterIdsManager, *mergerPtr, segmentsIndexer );
//        exportSegs2Tif( segmentsIds, true, "mergingSmall" + 
//          te::common::Convert2String( mergetIterations ) + ".tif" );

          if( enableProgressInterface )
          {
            if( maxMergedSegments )
            {
              currStep = 50 + (int)( 100.0 * ( ( (double)( maxMergedSegments - mergedSegments ) ) / 
                ((double)maxMergedSegments ) ) / 2.0 );
              
              if( currStep > progressPtr->getCurrentStep() )
              {
                progressPtr->pulse();
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
      
      // Flush result to the output raster
      
      {
        const unsigned int nLines = inputRaster.getNumberOfRows();
        const unsigned int nCols = inputRaster.getNumberOfColumns();
        unsigned int col = 0;
        SegmenterSegmentsBlock::SegmentIdDataType* segmentsIdsLinePtr = 0;
        
        for( unsigned int line = 0 ; line < nLines ; ++line )
        {
          segmentsIdsLinePtr = m_segmentsIdsMatrix[ line ];
          
          for( col = 0 ; col < nCols ; ++col )
          {
            outputRaster.setValue( col, line, segmentsIdsLinePtr[ col ], outputRasterBand );
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
      
      // Test case: 1505000 pixels image
      // case1: one band
      // case2: ten bands      
      
      double minM = 0;
      double maxM = 0;
      
      switch( m_parameters.m_segmentFeatures )
      {
        case Parameters::MeanFeaturesType :
        {
          minM = 556.7;
          maxM = 648.6;
          break;
        }
        case Parameters::BaatzFeaturesType :
        {
          minM = 763.4;
          maxM = 1039.0;
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid segment features type" );
          break;
        }
      } 
      
      if( bandsToProcess == 10 )
      {
        return maxM;
      }
      else
      {
        double dM = ( maxM - minM );
        double slope = dM / ( 10.0 - ((double)bandsToProcess) );
        
        return (
                 ( ((double)bandsToProcess) * slope ) + minM
               )
               *
               (
                 ((double)pixelsNumber) / 1505000.0 
               )
               *
               (
                 1024.0 * 1024.0
               );
      }
    }
    
    unsigned int SegmenterRegionGrowingStrategy::getOptimalBlocksOverlapSize() const
    {
      TERP_TRUE_OR_THROW( m_isInitialized, "Instance not initialized" );
      return (unsigned int)( std::sqrt( (double)m_parameters.m_minSegmentSize) );
    }
    
    bool SegmenterRegionGrowingStrategy::initializeSegments( 
      SegmenterIdsManager& segmenterIdsManager,
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,   
      const std::vector< double >& inputRasterGains,
      const std::vector< double >& inputRasterOffsets,                                                              
      SegmentsIndexer& segsIndexer )
    {
      segsIndexer.clear();
      
      const unsigned int nLines = inputRaster.getNumberOfRows();
      const unsigned int nCols = inputRaster.getNumberOfColumns();
      const unsigned int inputRasterBandsSize = (unsigned int)
        inputRasterBands.size();
      
      // Allocating the ids matrix
      
      if( ( m_segmentsIdsMatrix.getLinesNumber() != nLines ) ||
        ( m_segmentsIdsMatrix.getColumnsNumber() != nCols ) )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_segmentsIdsMatrix.reset( nLines, nCols,
          Matrix< SegmenterSegmentsBlock::SegmentIdDataType >::RAMMemPol ),
          "Error allocating segments Ids matrix" );
      }
        
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
        
      // Indexing each segment
      
      unsigned int line = 0;
      unsigned int col = 0;      
      Segment* segmentPtr = 0;
      Segment* neighborSegmentPtr = 0;
      SegmenterSegmentsBlock::SegmentIdDataType neighborSegmentId = 0;
      bool rasterValuesAreValid = true;
      unsigned int inputRasterBandsIdx = 0;
      double value = 0;
      const std::vector< double > dummyZeroesVector( inputRasterBandsSize, 0 );

      std::list< SegmenterSegmentsBlock::SegmentIdDataType > 
        unusedLineSegmentIds;
      
      std::vector< SegmenterSegmentsBlock::SegmentIdDataType > 
        lineSegmentIds;
      lineSegmentIds.reserve( nCols );
      
      std::vector< double > rasterValues;
      std::vector< double > rasterSquareValues;
      rasterValues.resize( inputRasterBandsSize, 0 );
      rasterSquareValues.resize( inputRasterBandsSize, 0 );
      
      for( line = 0 ; line < nLines ; ++line )
      {
        segmenterIdsManager.getNewIDs( nCols, lineSegmentIds );
        
        for( col = 0 ; col < nCols ; ++col )
        {
          rasterValuesAreValid = true;
          
          for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < 
            inputRasterBandsSize ; ++inputRasterBandsIdx )
          {
            inputRaster.getValue( col, line, value, 
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
              
              rasterValues[ inputRasterBandsIdx ] = value;
              rasterSquareValues[ inputRasterBandsIdx ] = value * value;
            }
          }
          
          // assotiating a segment object
          
          if( rasterValuesAreValid )
          {
            switch( m_parameters.m_segmentFeatures )
            {
              case Parameters::MeanFeaturesType :
              {
                segmentPtr = (MeanBasedSegment*)m_segmentsPool.retrive();
                
                if( segmentPtr == 0 )
                {
                  segmentPtr = new MeanBasedSegment();
                }
                
                ((MeanBasedSegment*)segmentPtr)->m_features.m_means = rasterValues;
                break;
              }
              case Parameters::BaatzFeaturesType :
              {
                segmentPtr = (BaatzBasedSegment*)m_segmentsPool.retrive();
                
                if( segmentPtr == 0 )
                {               
                  segmentPtr = new BaatzBasedSegment();
                }
                
                ((BaatzBasedSegment*)segmentPtr)->m_features.m_sums = rasterValues;
                ((BaatzBasedSegment*)segmentPtr)->m_features.m_squaresSum = rasterSquareValues;
                ((BaatzBasedSegment*)segmentPtr)->m_features.m_stdDev = dummyZeroesVector;
                ((BaatzBasedSegment*)segmentPtr)->m_features.m_edgeLength = 4;
                ((BaatzBasedSegment*)segmentPtr)->m_features.m_compactness = 4;
                ((BaatzBasedSegment*)segmentPtr)->m_features.m_smoothness = 1;
                break;
              }
              default :
              {
                TERP_LOG_AND_THROW( "Invalid segment features type" );
                break;
              }
            }
            
            segmentPtr->getFeatures()->m_id = lineSegmentIds[ col ];
            segmentPtr->getFeatures()->m_size = 1;
            segmentPtr->getFeatures()->m_xStart = col;
            segmentPtr->getFeatures()->m_xBound = col + 1;
            segmentPtr->getFeatures()->m_yStart = line;
            segmentPtr->getFeatures()->m_yBound = line + 1;
            
            m_segmentsIdsMatrix( line, col ) = segmentPtr->getFeatures()->m_id;
            segsIndexer[ segmentPtr->getFeatures()->m_id ] = segmentPtr;
              
            // updating the neighboorhood info
            
            segmentPtr->m_neighborSegments.clear();
              
            if( line ) 
            { 
              neighborSegmentId = m_segmentsIdsMatrix( line - 1, col );
                
              if( neighborSegmentId )
              {
                neighborSegmentPtr = segsIndexer[ neighborSegmentId ];
                TERP_DEBUG_TRUE_OR_THROW( neighborSegmentPtr, 
                  "Invalid neighboorSegmentPtr" );
                  
                segmentPtr->m_neighborSegments.push_back( neighborSegmentPtr );
                                    
                neighborSegmentPtr->m_neighborSegments.push_back( segmentPtr );
              }
            }
            
            if( col ) 
            { 
              neighborSegmentId = m_segmentsIdsMatrix( line, col - 1 );
                
              if( neighborSegmentId )
              {
                neighborSegmentPtr = segsIndexer[ neighborSegmentId ];
                TERP_DEBUG_TRUE_OR_THROW( neighborSegmentPtr, 
                  "Invalid neighboorSegmentPtr" );
                  
                segmentPtr->m_neighborSegments.push_back( neighborSegmentPtr );
                                    
                neighborSegmentPtr->m_neighborSegments.push_back( segmentPtr );
              }
            }            
          }
          else // !rasterValueIsValid
          {
            m_segmentsIdsMatrix( line, col ) = 0;
            unusedLineSegmentIds.push_back( lineSegmentIds[ col ] );
          }
        }
        
        // Free unused IDs
        
        if( ! unusedLineSegmentIds.empty() )
        {
          segmenterIdsManager.addFreeIDs( unusedLineSegmentIds );
          unusedLineSegmentIds.clear();
        }
      }
      
      return true;
    }
    
    unsigned int SegmenterRegionGrowingStrategy::mergeSegments( 
      const double similarityThreshold,
      SegmenterIdsManager& segmenterIdsManager,
      Merger& merger,
      const bool enablelocalMutualBestFitting,
      SegmentsIndexer& segsIndexer )
    {
      unsigned int mergedSegmentsNumber = 0;
      
      std::list< Segment* >::iterator nSegsIt;
      std::list< Segment* >::iterator nSegsItEnd;
      
      std::list< Segment* >::iterator nSegNSegsIt;
      std::list< Segment* >::iterator nSegNSegsItEnd;
      
      std::list< Segment* >::iterator minForwardDissimilaritySegmentIt;
      double minForwardDissimilarityValue = 0;
      SegmenterSegmentsBlock::SegmentIdDataType 
        minForwardDissimilaritySegmentId = 0;
      
      std::list< Segment* >::iterator minBackwardDissimilaritySegmentIt;
//      Segment* minBackwardDissimilaritySegmentPtr;
      double minBackwardDissimilarityValue = 0;
      
      double forwardDissimilarityValue = 0;
      double backwardDissimilarityValue = 0;
      
      unsigned int segmentsLine = 0;
      unsigned int segmentsLineBound = 0;
      unsigned int segmentCol = 0;
      unsigned int segmentColStart = 0;
      unsigned int segmentColBound = 0;
      
      SegmenterSegmentsBlock::SegmentIdDataType* segmentsIdsLinePtr = 0;
      
      SegmenterSegmentsBlock::SegmentIdDataType currentSegmentId = 0;
      
      std::list< SegmenterSegmentsBlock::SegmentIdDataType > freeSegmentIds;
      
      // initializing the auxiliary segment features
      
      std::auto_ptr< SegmentFeatures > auxSegFeatures1;
      std::auto_ptr< SegmentFeatures > auxSegFeatures2;
      std::auto_ptr< SegmentFeatures > minForwardDissimilaritySegmentFeatures;
      if( ! segsIndexer.empty() ) 
      {
        auxSegFeatures1.reset( 
          segsIndexer.begin()->second->getFeatures()->clone() );
        auxSegFeatures2.reset( 
          segsIndexer.begin()->second->getFeatures()->clone() );
        minForwardDissimilaritySegmentFeatures.reset( 
          segsIndexer.begin()->second->getFeatures()->clone() );
      }
      
      // Updating the merger state
      
      merger.update();
      
      // iterating over each segment
      
      SegmentsIndexer::iterator segsIt = segsIndexer.begin();
      
      while( segsIt != segsIndexer.end() )
      {
        // finding the neighbor segment with minimum dissimilary value
        // related to the current sement
        
        nSegsIt = segsIt->second->m_neighborSegments.begin();
        nSegsItEnd = segsIt->second->m_neighborSegments.end();
        
        minForwardDissimilaritySegmentIt = nSegsItEnd;
        minForwardDissimilarityValue = DBL_MAX;        
        
        while( nSegsIt != nSegsItEnd )
        {
          forwardDissimilarityValue = merger.getDissimilarityIndex( segsIt->second,
            *nSegsIt, auxSegFeatures1.get() );
            
          if( ( forwardDissimilarityValue < similarityThreshold ) &&
            ( forwardDissimilarityValue < minForwardDissimilarityValue ) )
          {
            minForwardDissimilarityValue = forwardDissimilarityValue;
            minForwardDissimilaritySegmentIt = nSegsIt;
            minForwardDissimilaritySegmentId = 
              (*minForwardDissimilaritySegmentIt)->getFeatures()->m_id;            
            minForwardDissimilaritySegmentFeatures->copy( auxSegFeatures1.get() );
          }
            
          ++nSegsIt;
        }
        
        // does the neighbor wants to merge back ?
        
        if( enablelocalMutualBestFitting &&
            (minForwardDissimilaritySegmentIt != nSegsItEnd ) )
        {
          // Calculating all neighbor neighbor segments dissimilarity          
          
          minBackwardDissimilaritySegmentIt = 
            (*minForwardDissimilaritySegmentIt)->m_neighborSegments.end();
          minBackwardDissimilarityValue = DBL_MAX;
          
          nSegNSegsIt = 
            (*minForwardDissimilaritySegmentIt)->m_neighborSegments.begin();
          nSegNSegsItEnd = 
            (*minForwardDissimilaritySegmentIt)->m_neighborSegments.end();
            
          while( nSegNSegsIt != nSegNSegsItEnd )
          {
            backwardDissimilarityValue = 
              merger.getDissimilarityIndex( *minForwardDissimilaritySegmentIt, 
              *nSegNSegsIt, auxSegFeatures2.get() );
              
            if( backwardDissimilarityValue < minBackwardDissimilarityValue )
            {
              minBackwardDissimilarityValue = backwardDissimilarityValue;
              minBackwardDissimilaritySegmentIt = nSegNSegsIt;
            }
        
            ++nSegNSegsIt;
          }
          
          if( 
              ( 
                minBackwardDissimilaritySegmentIt 
                == 
                (*minForwardDissimilaritySegmentIt)->m_neighborSegments.end() 
              )
              ||
              (
                ( *minBackwardDissimilaritySegmentIt ) 
                != 
                segsIt->second 
              )
            )
          {
            minForwardDissimilaritySegmentIt = nSegsItEnd;
          }
        }
        
        // If the minimum dissimilary neighbor was found it will be merged
        
        if( minForwardDissimilaritySegmentIt != nSegsItEnd )
        {
          // merging segment data
          
          merger.mergeFeatures( segsIt->second, *minForwardDissimilaritySegmentIt,
            minForwardDissimilaritySegmentFeatures.get() );
            
          // updating the min dissimilarity segment neighborhood segments
          // with the current segment
          
          nSegNSegsIt = 
            (*minForwardDissimilaritySegmentIt)->m_neighborSegments.begin();
          nSegNSegsItEnd = 
            (*minForwardDissimilaritySegmentIt)->m_neighborSegments.end();
          
          while( nSegNSegsIt != nSegNSegsItEnd )
          {
            if( (*nSegNSegsIt) != segsIt->second )
            {
              // adding the min dissimilarity neighborhood segment to the 
              // current one, if it is not already there
              
              if( std::find( segsIt->second->m_neighborSegments.begin(),
                segsIt->second->m_neighborSegments.end(), (*nSegNSegsIt) ) ==
                segsIt->second->m_neighborSegments.end() )
              {
                segsIt->second->m_neighborSegments.push_back( *nSegNSegsIt  );
              }
              
              // adding the current segment into the min dissimilarity 
              // neighborhood segments list, if it is not already there
              
              if( std::find( (*nSegNSegsIt)->m_neighborSegments.begin(),
                (*nSegNSegsIt)->m_neighborSegments.end(), segsIt->second ) ==
                (*nSegNSegsIt)->m_neighborSegments.end() )
              {
                (*nSegNSegsIt)->m_neighborSegments.push_back( segsIt->second );
              }
              
              // removing the merged segment reference from its neighbor
              // list
              
              (*nSegNSegsIt)->m_neighborSegments.remove( 
                *minForwardDissimilaritySegmentIt );            
            }
            
            ++nSegNSegsIt;
          }
          
          // updating the segments Ids indexer
          
          segmentsLineBound = (*minForwardDissimilaritySegmentIt)->getFeatures()->m_yBound;
          segmentColStart = (*minForwardDissimilaritySegmentIt)->getFeatures()->m_xStart;
          segmentColBound = (*minForwardDissimilaritySegmentIt)->getFeatures()->m_xBound;          
          currentSegmentId = segsIt->second->getFeatures()->m_id;
            
          for( segmentsLine = (*minForwardDissimilaritySegmentIt)->getFeatures()->m_yStart ; 
            segmentsLine < segmentsLineBound ; ++segmentsLine )
          {
            segmentsIdsLinePtr = m_segmentsIdsMatrix[ segmentsLine ];
            
            for( segmentCol = segmentColStart ; segmentCol < 
              segmentColBound ; ++segmentCol )
            {
              if( segmentsIdsLinePtr[ segmentCol ] ==
                minForwardDissimilaritySegmentId )
              {
                segmentsIdsLinePtr[ segmentCol ] = currentSegmentId;
              }
            }
          }
          
          // moving to the pool the merged segment from the list
          // of neighborhood segments and from the global 
          // segments container
          // The merged segment id will be given back to ids manager
          
          m_segmentsPool.store( *minForwardDissimilaritySegmentIt );
          
          segsIt->second->m_neighborSegments.erase( 
            minForwardDissimilaritySegmentIt );
            
          segsIndexer.erase( minForwardDissimilaritySegmentId );
          
          freeSegmentIds.push_back( minForwardDissimilaritySegmentId );
          
          ++mergedSegmentsNumber;
        }
        
        ++segsIt;
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
      SegmentsIndexer& segsIndexer )
    {
      unsigned int mergedSegmentsNumber = 0;
      
      std::list< Segment* >::iterator nSegsIt;
      std::list< Segment* >::iterator nSegsItEnd;      
      
      std::list< Segment* >::iterator nSegNSegsIt;
      std::list< Segment* >::iterator nSegNSegsItEnd;      
      
      std::list< Segment* >::iterator minForwardDissimilaritySegmentIt;
      double minForwardDissimilarityValue = 0;
      SegmenterSegmentsBlock::SegmentIdDataType 
        minForwardDissimilaritySegmentId = 0;      
      
      double forwardDissimilarityValue = 0;
      
      unsigned int segmentsLine = 0;
      unsigned int segmentsLineBound = 0;
      unsigned int segmentCol = 0;
      unsigned int segmentColStart = 0;
      unsigned int segmentColBound = 0;  
      
      SegmenterSegmentsBlock::SegmentIdDataType* segmentsIdsLinePtr = 0;
      
      SegmenterSegmentsBlock::SegmentIdDataType currentSegmentId = 0;
      
      // initializing the auxiliary features
      
      std::auto_ptr< SegmentFeatures > candidateAuxSegFeatures;
      std::auto_ptr< SegmentFeatures > minForwardDissimilarityFeatures;
      if( ! segsIndexer.empty() ) 
      {
        candidateAuxSegFeatures.reset( 
          segsIndexer.begin()->second->getFeatures()->clone() );
        minForwardDissimilarityFeatures.reset( 
          candidateAuxSegFeatures->clone() );
      }
      
      // Updating the merger state
      
      merger.update();
      
      // iterating over each segment      
      
      SegmentsIndexer::iterator segsIt = segsIndexer.begin();
      
      while( segsIt != segsIndexer.end() )
      {
        // is this a small segment ?
        
        if( segsIt->second->getFeatures()->m_size < minSegmentSize )
        {
          // Looking for the closest neighboorhood segment
          
          nSegsIt = segsIt->second->m_neighborSegments.begin();
          nSegsItEnd = segsIt->second->m_neighborSegments.end();
          
          minForwardDissimilaritySegmentIt = nSegsItEnd;
          minForwardDissimilarityValue = DBL_MAX;
          
          while( nSegsIt != nSegsItEnd )
          {
            forwardDissimilarityValue = merger.getDissimilarityIndex( segsIt->second,
              *nSegsIt, candidateAuxSegFeatures.get() );
              
            if( forwardDissimilarityValue < minForwardDissimilarityValue )
            { 
              minForwardDissimilarityValue = forwardDissimilarityValue;
              minForwardDissimilaritySegmentIt = nSegsIt;
              minForwardDissimilaritySegmentId = 
                (*minForwardDissimilaritySegmentIt)->getFeatures()->m_id;              
              minForwardDissimilarityFeatures->copy( candidateAuxSegFeatures.get() );
            }
            
            ++nSegsIt;
          }
          
          // If the minimum dissimilary neighbor was found it will be merged
        
          if( minForwardDissimilaritySegmentIt != nSegsItEnd )
          {          
            // merging the small segment data into there
            // closes segment data
            
            merger.mergeFeatures( (*minForwardDissimilaritySegmentIt),
              segsIt->second, minForwardDissimilarityFeatures.get() );
              
            // updating the the small segment neighborhood segments
            // with the current closest segment
            
            nSegNSegsIt = 
              segsIt->second->m_neighborSegments.begin();
            nSegNSegsItEnd = 
              segsIt->second->m_neighborSegments.end();
            
            while( nSegNSegsIt != nSegNSegsItEnd )
            {
              if( (*nSegNSegsIt) != (*minForwardDissimilaritySegmentIt) )
              {
                // adding the small segment neighborhood segment to the 
                // closest segment, if it is not already there
                
                if( std::find( 
                  (*minForwardDissimilaritySegmentIt)->m_neighborSegments.begin(),
                  (*minForwardDissimilaritySegmentIt)->m_neighborSegments.end(), 
                  (*nSegNSegsIt) ) ==
                  (*minForwardDissimilaritySegmentIt)->m_neighborSegments.end() )
                {
                  (*minForwardDissimilaritySegmentIt)->m_neighborSegments.push_back( 
                    *nSegNSegsIt  );
                }
                
                // adding the closest segment into the small segment 
                // neighborhood segments list, if it is not already there
                
                if( std::find( (*nSegNSegsIt)->m_neighborSegments.begin(),
                  (*nSegNSegsIt)->m_neighborSegments.end(), 
                  (*minForwardDissimilaritySegmentIt) ) ==
                  (*nSegNSegsIt)->m_neighborSegments.end() )
                {
                  (*nSegNSegsIt)->m_neighborSegments.push_back( 
                    *minForwardDissimilaritySegmentIt );
                }
                
                // removing the small segment reference from its neighbor
                // list
                
                (*nSegNSegsIt)->m_neighborSegments.remove( segsIt->second );            
              } 
              
              ++nSegNSegsIt;
            }
            
            // updating the segments Ids container
            
            segmentsLineBound = segsIt->second->getFeatures()->m_yBound;
            segmentColStart = segsIt->second->getFeatures()->m_xStart;
            segmentColBound = segsIt->second->getFeatures()->m_xBound;          
            currentSegmentId = segsIt->second->getFeatures()->m_id;
              
            for( segmentsLine = segsIt->second->getFeatures()->m_yStart ; 
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
                    minForwardDissimilaritySegmentId;
                }
              }
            }
            
            // moving to the pool and removing the small segment from the list
            // of the closest segment neighborhood segments list
            // and from the global segments container
            // The merged segment id will be given back to ids manager
            
            (*minForwardDissimilaritySegmentIt)->m_neighborSegments.remove( 
              segsIt->second );

            m_segmentsPool.store( segsIt->second );
            
            ++segsIt;
              
            segsIndexer.erase( currentSegmentId );
            
            segmenterIdsManager.addFreeID( currentSegmentId );
            
            ++mergedSegmentsNumber;
          }
          else
          {
            ++segsIt;
          }
        }
        else
        {
          ++segsIt;
        }
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

