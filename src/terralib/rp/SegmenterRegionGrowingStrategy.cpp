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

#include <algorithm>
#include <cfloat>

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
      
      return *this;      
    }
    
    void SegmenterRegionGrowingStrategy::Parameters::reset() 
    throw( te::rp::Exception )
    {
      m_minSegmentSize = 1;
      m_segmentsSimilarityThreshold = 0.5;
      m_segmentFeatures = MeanFeaturesType;      
      m_bandsWeights.clear();
      m_colorWeight = 0.5;
      m_compactnessWeight = 0.5;
    }
    
    te::common::AbstractParameters* SegmenterRegionGrowingStrategy::Parameters::clone() const
    {
      return new te::rp::SegmenterRegionGrowingStrategy::Parameters( *this );
    }
    
    //-------------------------------------------------------------------------

    SegmenterRegionGrowingStrategy::Segment::Segment()
    {
      m_id = 0;
      m_xStart = m_xBound = m_yStart = m_yBound = 0;
      m_size = 0;
    }
    
    SegmenterRegionGrowingStrategy::Segment::Segment( const Segment& )
    {
    }    
    
    SegmenterRegionGrowingStrategy::Segment::~Segment()
    {
    }
    
    const SegmenterRegionGrowingStrategy::Segment& 
      SegmenterRegionGrowingStrategy::Segment::operator=( 
      const SegmenterRegionGrowingStrategy::Segment& ) 
    {
      return *this;
    };
    
    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategy::SegmentsContainer::SegmentsContainer()
    {
    }
    
    SegmenterRegionGrowingStrategy::SegmentsContainer::~SegmentsContainer()
    {
      deleteSegments();
    }   
    
    void SegmenterRegionGrowingStrategy::SegmentsContainer::deleteSegments()
    {
      iterator segmentsIt = begin();
      iterator segmentsItEnd = end();
      
      while( segmentsIt != segmentsItEnd )
      {
        delete (segmentsIt->second);
        
        ++segmentsIt;
      }
      
      clear();
    }     
    
    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategy::MeanBasedSegment::MeanBasedSegment()
    {
    }
    
    SegmenterRegionGrowingStrategy::MeanBasedSegment::~MeanBasedSegment()
    {
    }    
    
    double 
    SegmenterRegionGrowingStrategy::MeanBasedSegment::getDissimilarityIndex( 
      Segment const * const otherSegment )
    {
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< MeanBasedSegment const * const >(
        otherSegment ), "Invalid segment type" );       
      getDissimilarityIndex_otherCastPtr = 
        (MeanBasedSegment*)(otherSegment);
      
      getDissimilarityIndex_meansSize = m_means.size();
      TERP_DEBUG_TRUE_OR_THROW( getDissimilarityIndex_meansSize ==
        getDissimilarityIndex_otherCastPtr->m_means.size(),
        "Internal error" );
        
      getDissimilarityIndex_dissValue = 0.0;
        
      for( getDissimilarityIndex_meansIdx = 0 ; getDissimilarityIndex_meansIdx
        < getDissimilarityIndex_meansSize ; ++getDissimilarityIndex_meansIdx )
      {
        getDissimilarityIndex_diffValue = m_means[ 
          getDissimilarityIndex_meansIdx ] - 
          getDissimilarityIndex_otherCastPtr->m_means[ 
          getDissimilarityIndex_meansIdx ];
          
        getDissimilarityIndex_dissValue += ( getDissimilarityIndex_diffValue *
          getDissimilarityIndex_diffValue );
      }
        
      return sqrt( getDissimilarityIndex_dissValue );
    };
    
    void SegmenterRegionGrowingStrategy::MeanBasedSegment::mergeFeatures( 
      Segment const * const otherSegment )
    {
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< MeanBasedSegment const * const >(
        otherSegment ), "Invalid segment type" );      
      mergeFeatures_otherCastPtr = (MeanBasedSegment*)(otherSegment);
      
      mergeFeatures_meansSize = m_means.size();
      TERP_DEBUG_TRUE_OR_THROW( mergeFeatures_meansSize ==
        mergeFeatures_otherCastPtr->m_means.size(),
        "Internal error" );
        
      for( mergeFeatures_meansIdx = 0 ; mergeFeatures_meansIdx
        < mergeFeatures_meansSize ; ++mergeFeatures_meansIdx )
      {
        m_means[ mergeFeatures_meansIdx ] = 
          (
            ( m_means[ mergeFeatures_meansIdx ] * ((double)m_size ) ) +
            ( mergeFeatures_otherCastPtr->m_means[ mergeFeatures_meansIdx ] *
            ((double)mergeFeatures_otherCastPtr->m_size) )
          )
          / 
          ((double)( m_size + mergeFeatures_otherCastPtr->m_size));
      }
    }
    
    //-------------------------------------------------------------------------
    
    SegmenterRegionGrowingStrategy::BaatzBasedSegment::BaatzBasedSegment()
    {
    }
    
    SegmenterRegionGrowingStrategy::BaatzBasedSegment::~BaatzBasedSegment()
    {
    }    
    
    double 
    SegmenterRegionGrowingStrategy::BaatzBasedSegment::getDissimilarityIndex( 
      Segment const * const otherSegment )
    {
      TERP_DEBUG_TRUE_OR_THROW( dynamic_cast< BaatzBasedSegment const * const >(
        otherSegment ), "Invalid segment type" );
      BaatzBasedSegment const* const otherCastPtr = (BaatzBasedSegment const*)(otherSegment);
        
      TERP_DEBUG_TRUE_OR_THROW( m_bandsWeightsPtr, 
        "Invalid m_bandsWeightsPtr pointer" );
      
      const unsigned int sumsSize = m_sums.size();
      TERP_DEBUG_TRUE_OR_THROW( sumsSize ==
        m_stdDev.size(),
        "Internal error" );       
      TERP_DEBUG_TRUE_OR_THROW( sumsSize ==
        otherCastPtr->m_sums.size(),
        "Internal error" );       
        
      double dissValue = 0.0;
      
      double sumCurrent = 0.0;
      double sumOther = 0;
      double sumUnion = 0;
      
      const double sizeCurrent = (double)m_size;
      const double sizeOther = (double)otherCastPtr->m_size;
      const double sizeUnion = sizeCurrent + sizeOther;
      
      double meanUnion = 0;
      
      double stdDevUnion = 0.0;
      
      double hColor = 0;
      
      for( unsigned int sumsIdx = 0 ; sumsIdx < sumsSize ; ++sumsIdx )
      {
        sumCurrent = m_sums[ sumsIdx ];
        
        sumOther = otherCastPtr->m_sums[ sumsIdx ];
        
        sumUnion = sumCurrent + sumOther;
        
        meanUnion = sumUnion / sizeUnion;
        
        stdDevUnion =
          ( 
            (
              ( ( sumCurrent * sumCurrent ) / ( sizeCurrent * sizeCurrent ) )
              *
              ( ( sumOther * sumOther ) / ( sizeOther * sizeOther ) )
            )
            -
            (
              2.0 * meanUnion * sumUnion
            )
            +
            (
              sizeUnion * meanUnion * meanUnion
            )
          );        
          
      }
        
      return sqrt( dissValue );
    };
    
    void SegmenterRegionGrowingStrategy::BaatzBasedSegment::mergeFeatures( 
      Segment const * const otherSegment )
    {

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
      
      SegmenterRegionGrowingStrategy::Parameters const* paramsPtr = 
        dynamic_cast< SegmenterRegionGrowingStrategy::Parameters const* >( strategyParams );
        
      if( paramsPtr )
      {
        m_parameters = *( paramsPtr );
        
        TERP_TRUE_OR_RETURN_FALSE( m_parameters.m_minSegmentSize > 0,
          "Invalid segmenter strategy parameter" )
        TERP_TRUE_OR_RETURN_FALSE( ( 
          ( m_parameters.m_segmentsSimilarityThreshold >= 0.0 )
          && ( m_parameters.m_segmentsSimilarityThreshold <= 1.0 ) ),
          "Invalid segmenter strategy parameter" )        
          
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
      m_parameters.reset();
    };
    
    bool SegmenterRegionGrowingStrategy::execute( 
      SegmenterIdsManager& segmenterIdsManager,
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,
      te::rst::Raster& outputRaster,
      const unsigned int outputRasterBand,
      const bool enableProgressInterface )
      throw( te::rp::Exception )
    {
      TERP_TRUE_OR_RETURN_FALSE( m_isInitialized,
        "Instance not initialized" )
        
      // Initializing segments
        
      SegmentsIdsContainerT segmentsIds;
      SegmentsContainer segments;
      TERP_TRUE_OR_RETURN_FALSE( initializeSegments( segmenterIdsManager,
        inputRaster, inputRasterBands, segmentsIds, segments ), 
        "Segments initalization error" );
        
      // Region Growing
      
      const double maxMergesNumber = (double)( 1 + ( 
        inputRaster.getNumberOfRows() *
        inputRaster.getNumberOfColumns() ) / 2 );
      double similarityThreshold = DBL_MIN;
      unsigned int mergedSegments = 0;
      unsigned int mergetIterations = 0;
      
//      exportSegs2Tif( segmentsIds, true, "merging" + 
//        te::common::Convert2String( mergetIterations ) + ".tif" );
      
      while( ( mergedSegments = mergeSegments( similarityThreshold,
        segmenterIdsManager, segmentsIds, segments  ) ) )
      {
        ++mergetIterations;
        
//        exportSegs2Tif( segmentsIds, true, "merging" + 
//          te::common::Convert2String( mergetIterations ) + ".tif" );
        
        similarityThreshold = m_parameters.m_segmentsSimilarityThreshold -
          ( m_parameters.m_segmentsSimilarityThreshold * 
          ((double)mergedSegments) / maxMergesNumber );
        similarityThreshold = MAX( DBL_MIN, similarityThreshold );
        similarityThreshold = MIN( similarityThreshold, 
          m_parameters.m_segmentsSimilarityThreshold );
      }
      
      while( mergeSmallSegments( m_parameters.m_minSegmentSize, 
        segmenterIdsManager, segmentsIds, segments ) )
      {
        ++mergetIterations;
//        exportSegs2Tif( segmentsIds, true, "mergingSmall" + 
//          te::common::Convert2String( mergetIterations ) + ".tif" );
      };
      
      // Free unused resources
      
      segments.deleteSegments();
      
      // Flush result to the output raster
      
      {
        const unsigned int nLines = inputRaster.getNumberOfRows();
        const unsigned int nCols = inputRaster.getNumberOfColumns();
        unsigned int col = 0;
        SegmenterSegmentsBlock::SegmentIdDataType* segmentsIdsLinePtr = 0;
        
        for( unsigned int line = 0 ; line < nLines ; ++line )
        {
          segmentsIdsLinePtr = segmentsIds[ line ];
          
          for( col = 0 ; col < nCols ; ++col )
          {
            outputRaster.setValue( col, line, segmentsIdsLinePtr[ col ], 0 );
          }
        }
      }
      
      return true;
    }
    
    double SegmenterRegionGrowingStrategy::getMemUsageFactor(
      const unsigned int inputRasterBandsNumber ) const
    {
      TERP_TRUE_OR_THROW( m_isInitialized, "Instance not initialized" );
      
      return 
        (
          ( ((double)inputRasterBandsNumber) * ((double)sizeof(double) ) ) +
          ( ((double)inputRasterBandsNumber) * ((double)sizeof(double) ) ) +
          ( (double)sizeof( MeanBasedSegment ) ) +
          ( (double)sizeof( SegmenterSegmentsBlock::SegmentIdDataType ) ) + 
          ( 4.0 * ((double)( sizeof(Segment*) ) ) ) 
        )
        /
        ( 
          ((double)inputRasterBandsNumber) * ((double)sizeof(double) ) 
        );
    }
    
    unsigned int SegmenterRegionGrowingStrategy::getOptimalBlocksOverlapSize() const
    {
      TERP_TRUE_OR_THROW( m_isInitialized, "Instance not initialized" );
      return ( 2 * m_parameters.m_minSegmentSize );
    }
    
    bool SegmenterRegionGrowingStrategy::initializeSegments( 
      SegmenterIdsManager& segmenterIdsManager,
      const te::rst::Raster& inputRaster,
      const std::vector< unsigned int >& inputRasterBands,                                                            
      SegmentsIdsContainerT& segmentsIds,
      SegmentsContainer& segments )
    {
      const unsigned int nLines = inputRaster.getNumberOfRows();
      const unsigned int nCols = inputRaster.getNumberOfColumns();
      const unsigned int inputRasterBandsSize = (unsigned int)
        inputRasterBands.size();
      
      // Allocating the ids matrix
      
      TERP_TRUE_OR_RETURN_FALSE( segmentsIds.reset( nLines, nCols,
        Matrix< SegmenterSegmentsBlock::SegmentIdDataType >::RAMMemPol ),
        "Error allocating segments Ids matrix" );
        
      // fiding the image normalizing scale and offsets
      
      std::vector< double > bandsPixelScales;
      std::vector< double > bandsPixelOffsets;
      std::vector< double > bandDummyValues;
      
      {
        unsigned int line = 0;
        unsigned int col = 0; 
        unsigned int inputRasterBandsIdx = 0;
        double value = 0;
        double bandMinValue = DBL_MAX;
        double bandMaxValue = -1.0 * DBL_MAX;

        for( inputRasterBandsIdx = 0 ; inputRasterBandsIdx < 
          inputRasterBandsSize ; ++inputRasterBandsIdx )
        {
          const te::rst::Band& band = *( inputRaster.getBand( 
            inputRasterBands[ inputRasterBandsIdx ] ) );
          const double& bandNoDataValue = band.getProperty()->m_noDataValue;
            
          for( line = 0 ; line < nLines ; ++line )
          {
            for( col = 0 ; col < nCols ; ++col )
            {
              band.getValue( col, line, value );            
              
              if( value != bandNoDataValue )
              {
                if( bandMinValue > value ) bandMinValue = value;
                if( bandMaxValue < value ) bandMaxValue = value;
              }
            }
          }
          
          bandDummyValues.push_back( bandNoDataValue );
          
          if( bandMaxValue == bandMinValue )
          {
            bandsPixelScales.push_back( 0.0 );
            bandsPixelOffsets.push_back( 0.0 );
          }
          else
          {
            bandsPixelScales.push_back( 1.0 / ( bandMaxValue - bandMinValue ) );
            bandsPixelOffsets.push_back( bandMinValue );
          }
        }
      }
        
      // Creating each new segment
      
      unsigned int line = 0;
      unsigned int col = 0;      
      Segment* segmentPtr = 0;
      Segment* neighborSegmentPtr = 0;
      SegmenterSegmentsBlock::SegmentIdDataType neighborSegmentId = 0;
      bool rasterValuesAreValid = true;
      unsigned int inputRasterBandsIdx = 0;
      double value = 0;

      std::list< SegmenterSegmentsBlock::SegmentIdDataType > 
        unusedLineSegmentIds;
      
      std::vector< SegmenterSegmentsBlock::SegmentIdDataType > 
        lineSegmentIds;
      lineSegmentIds.reserve( nCols );
      
      std::vector< double > rasterValues;
      rasterValues.resize( inputRasterBandsSize, 0 );
      
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
              rasterValues[ inputRasterBandsIdx ] = ( value - bandsPixelOffsets[  
                inputRasterBandsIdx ] ) * bandsPixelScales[ inputRasterBandsIdx ];
            }
          }
          
          // instantiating the segment object
          
          if( rasterValuesAreValid )
          {
            switch( m_parameters.m_segmentFeatures )
            {
              case Parameters::MeanFeaturesType :
              {
                segmentPtr = new MeanBasedSegment();
                ((MeanBasedSegment*)segmentPtr)->m_means = rasterValues;
                break;
              }
              default :
              {
                TERP_LOG_AND_THROW( "Invalid segment features type" );
                break;
              }
            }
            
            segmentPtr->m_id = lineSegmentIds[ col ];
            segmentPtr->m_size = 1;
            segmentPtr->m_xStart = col;
            segmentPtr->m_xBound = col + 1;
            segmentPtr->m_yStart = line;
            segmentPtr->m_yBound = line + 1;
            
            segmentsIds( line, col ) = segmentPtr->m_id;
            segments[ segmentPtr->m_id ] = segmentPtr;
              
            // updating the neighboorhood info
              
            if( line ) 
            { 
              neighborSegmentId = segmentsIds( line - 1, col );
                
              if( neighborSegmentId )
              {
                neighborSegmentPtr = segments[ neighborSegmentId ];
                TERP_DEBUG_TRUE_OR_THROW( neighborSegmentPtr, 
                  "Invalid neighboorSegmentPtr" );
                  
                segmentPtr->m_neighborSegments.push_back( neighborSegmentPtr );
                                    
                neighborSegmentPtr->m_neighborSegments.push_back( segmentPtr );
              }
            }
            
            if( col ) 
            { 
              neighborSegmentId = segmentsIds( line, col - 1 );
                
              if( neighborSegmentId )
              {
                neighborSegmentPtr = segments[ neighborSegmentId ];
                TERP_DEBUG_TRUE_OR_THROW( neighborSegmentPtr, 
                  "Invalid neighboorSegmentPtr" );
                  
                segmentPtr->m_neighborSegments.push_back( neighborSegmentPtr );
                                    
                neighborSegmentPtr->m_neighborSegments.push_back( segmentPtr );
              }
            }            
          }
          else // !rasterValueIsValid
          {
            segmentsIds( line, col ) = 0;
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
      SegmentsIdsContainerT& segmentsIds,
      SegmentsContainer& segments )
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
      
      Segment* minBackwardDissimilaritySegmentPtr;
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
      
      SegmentsContainer::iterator segsIt = segments.begin();
      SegmentsContainer::iterator segsItEnd = segments.end();      
      
      while( segsIt != segsItEnd )
      {
        // finding the neighbor segment with minimum dissimilary value
        // related to the current sement
        
        nSegsIt = segsIt->second->m_neighborSegments.begin();
        nSegsItEnd = segsIt->second->m_neighborSegments.end();
        
        minForwardDissimilaritySegmentIt = nSegsItEnd;
        minForwardDissimilarityValue = DBL_MAX;        
        
        while( nSegsIt != nSegsItEnd )
        {
          forwardDissimilarityValue = segsIt->second->getDissimilarityIndex(
            *nSegsIt );
            
          if( ( forwardDissimilarityValue < similarityThreshold ) &&
            ( forwardDissimilarityValue < minForwardDissimilarityValue ) )
          {
            // does the neighbor wants to merge back ?
            // Calculating all neighbor neighbor segments dissimilarity          
            
            minBackwardDissimilaritySegmentPtr = 0;
            minBackwardDissimilarityValue = DBL_MAX;
            
            nSegNSegsIt = 
              (*nSegsIt)->m_neighborSegments.begin();
            nSegNSegsItEnd = 
              (*nSegsIt)->m_neighborSegments.end();
              
            while( nSegNSegsIt != nSegNSegsItEnd )
            {
              backwardDissimilarityValue = 
                (*nSegsIt)->getDissimilarityIndex( *nSegNSegsIt );
                
              if( backwardDissimilarityValue < minBackwardDissimilarityValue )
              {
                minBackwardDissimilarityValue = backwardDissimilarityValue;
                minBackwardDissimilaritySegmentPtr = (*nSegNSegsIt);
              }
          
              ++nSegNSegsIt;
            }
              
            if( minBackwardDissimilaritySegmentPtr == segsIt->second )
            {
              minForwardDissimilarityValue = forwardDissimilarityValue;
              minForwardDissimilaritySegmentIt = nSegsIt;
            }
          }
            
          ++nSegsIt;
        }
        
        // If the minimum dissimilary neighbor was found it will be merged
        
        if( minForwardDissimilaritySegmentIt != nSegsItEnd )
        {
          // merging segment data
          
          segsIt->second->mergeFeatures( *minForwardDissimilaritySegmentIt );
          
          segsIt->second->m_size += 
            (*minForwardDissimilaritySegmentIt)->m_size;
          segsIt->second->m_xStart = MIN( segsIt->second->m_xStart,
            (*minForwardDissimilaritySegmentIt)->m_xStart );
          segsIt->second->m_xBound = MAX( segsIt->second->m_xBound,
            (*minForwardDissimilaritySegmentIt)->m_xBound );              
          segsIt->second->m_yStart = MIN( segsIt->second->m_yStart,
            (*minForwardDissimilaritySegmentIt)->m_yStart );
          segsIt->second->m_yBound = MAX( segsIt->second->m_yBound,
            (*minForwardDissimilaritySegmentIt)->m_yBound );              
            
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
          
          // updating the segments Ids container
          
          segmentsLineBound = (*minForwardDissimilaritySegmentIt)->m_yBound;
          segmentColStart = (*minForwardDissimilaritySegmentIt)->m_xStart;
          segmentColBound = (*minForwardDissimilaritySegmentIt)->m_xBound;          
        
          minForwardDissimilaritySegmentId = 
            (*minForwardDissimilaritySegmentIt)->m_id;
          currentSegmentId = segsIt->second->m_id;
            
          for( segmentsLine = (*minForwardDissimilaritySegmentIt)->m_yStart ; 
            segmentsLine < segmentsLineBound ; ++segmentsLine )
          {
            segmentsIdsLinePtr = segmentsIds[ segmentsLine ];
            
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
          
          // deleting and removing the merged segment from the list
          // of neighborhood segments and from the global 
          // segments container
          // The merged segment id will be given back to ids manager
          
          delete (*minForwardDissimilaritySegmentIt);
          
          segsIt->second->m_neighborSegments.erase( 
            minForwardDissimilaritySegmentIt );
          
          segments.erase( minForwardDissimilaritySegmentId );
          
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
      SegmentsIdsContainerT& segmentsIds,
      SegmentsContainer& segments )
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
      
      SegmentsContainer::iterator segsIt = segments.begin();
      SegmentsContainer::iterator segsItEnd = segments.end();      
      
      while( segsIt != segsItEnd )
      {
        // is this a small segment ?
        
        if( segsIt->second->m_size < minSegmentSize )
        {
          // Looking for the closest neighboorhood segment
          
          nSegsIt = segsIt->second->m_neighborSegments.begin();
          nSegsItEnd = segsIt->second->m_neighborSegments.end();
          
          minForwardDissimilaritySegmentIt = nSegsItEnd;
          minForwardDissimilarityValue = DBL_MAX;
          
          while( nSegsIt != nSegsItEnd )
          {
            forwardDissimilarityValue = segsIt->second->getDissimilarityIndex(
              *nSegsIt );
              
            if( forwardDissimilarityValue < minForwardDissimilarityValue )
            { 
              minForwardDissimilarityValue = forwardDissimilarityValue;
              minForwardDissimilaritySegmentIt = nSegsIt;
            }
            
            ++nSegsIt;
          }
          
          // If the minimum dissimilary neighbor was found it will be merged
        
          if( minForwardDissimilaritySegmentIt != nSegsItEnd )
          {          
            // merging the small segment data into there
            // closes segment data
            
            (*minForwardDissimilaritySegmentIt)->mergeFeatures( 
              segsIt->second );
            
            (*minForwardDissimilaritySegmentIt)->m_size += 
              segsIt->second->m_size;
            (*minForwardDissimilaritySegmentIt)->m_xStart = 
              MIN( segsIt->second->m_xStart,
              (*minForwardDissimilaritySegmentIt)->m_xStart );
            (*minForwardDissimilaritySegmentIt)->m_xBound = 
              MAX( segsIt->second->m_xBound,
              (*minForwardDissimilaritySegmentIt)->m_xBound );              
            (*minForwardDissimilaritySegmentIt)->m_yStart = 
              MIN( segsIt->second->m_yStart,
              (*minForwardDissimilaritySegmentIt)->m_yStart );
            (*minForwardDissimilaritySegmentIt)->m_yBound = 
              MAX( segsIt->second->m_yBound,
              (*minForwardDissimilaritySegmentIt)->m_yBound );   
              
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
            
            segmentsLineBound = segsIt->second->m_yBound;
            segmentColStart = segsIt->second->m_xStart;
            segmentColBound = segsIt->second->m_xBound;          
          
            minForwardDissimilaritySegmentId = 
              (*minForwardDissimilaritySegmentIt)->m_id;
            currentSegmentId = segsIt->second->m_id;
              
            for( segmentsLine = segsIt->second->m_yStart ; 
              segmentsLine < segmentsLineBound ; ++segmentsLine )
            {
              segmentsIdsLinePtr = segmentsIds[ segmentsLine ];
              
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
            
            // deleting and removing the small segment from the list
            // of the closest segment neighborhood segments list
            // and from the global segments container
            // The merged segment id will be given back to ids manager
            
            (*minForwardDissimilaritySegmentIt)->m_neighborSegments.remove( 
              segsIt->second );
              
            delete (segsIt->second);
              
            ++segsIt;
            
            segments.erase( currentSegmentId );
            
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
      const SegmentsIdsContainerT& segmentsIds, bool normto8bits,
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
            te::common::Convert2String( value ))
          
          rasterPtr->setValue( col, line, value , 0 );
        }
      }
      
      delete rasterPtr;
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

