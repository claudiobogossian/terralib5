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
  \file terralib/rp/SegmenterRegionGrowingfunctions.h
  \brief Useful functions for Region Growing segmentation.
 */

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGFUNCTIONS_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGFUNCTIONS_H

#include "Matrix.h"
#include "SegmenterIdsManager.h"
#include "SegmenterRegionGrowingMerger.h"
#include "SegmenterRegionGrowingSegment.h"
#include "SegmenterSegmentsBlock.h"

namespace te
{
  namespace rp
  {
    namespace rg
    {
      /*!
        \brief Definition for segments ids matrix type.
      */
      typedef Matrix< SegmenterSegmentsBlock::SegmentIdDataType >
      SegmentsIdsMatrixT;

      /*!
        \brief Iteration counter type.
      */
      typedef unsigned short int IterationCounterType;

      /*!
        \brief Mean Strategy feature type.
      */
      typedef float MeanFeatureType;
      
      /*!
        \brief Baatz Strategy feature type.
      */
      typedef float BaatzFeatureType;
      
      /*!
        \brief Export the segments IDs to a tif file.
        \param segmentsIds The output segment ids container.
        \param normto8bits If true, a 8 bits file will be generated.
        \param fileName The output tif file name.
      */
      void exportSegs2Tif( const SegmentsIdsMatrixT& segmentsIds,
                           bool normto8bits, const std::string& fileName );

      /*!
        \brief Returns the number of active segments.
        \param actSegsListHeadPtr A pointer the the active segments list head.
        \return Returns the number of active segments.
      */
      template< typename FeatureDataTypeT >
      unsigned int getActiveSegmentsNumber( SegmenterRegionGrowingSegment< FeatureDataTypeT >* const actSegsListHeadPtr )
      {
        unsigned int returnValue = 0;
      
        SegmenterRegionGrowingSegment< FeatureDataTypeT >* currSegPtr = actSegsListHeadPtr;
      
        while( currSegPtr ) {
          ++returnValue;
          currSegPtr = currSegPtr->m_nextActiveSegment;
        }

        return returnValue;
      }

      /*!
        \brief Merge closest segments.
        \param segmentsIds Segments ids container.
        \param disimilarityThreshold The maximum similarity value allowed when deciding when to merge two segments.
        \param maxSegSizeThreshold Segments with sizes smaller then this value will allways be merged with the closest segment (disimilarityThreshold will be ignored).
        \param segmenterIdsManager A segments ids manager to acquire unique segments ids.
        \param merger The merger instance to use.
        \param enablelocalMutualBestFitting If enabled, a merge only occurs between two segments if the minimum dissimilarity criteria is best fulfilled mutually.
        \param enableSameIterationMerges If enabled, a merged segment could be merged with another under the same iteration.
        \param auxSeg1Ptr A pointer to a valid auxiliar segment that will be used by this method.
        \param auxSeg2Ptr A pointer to a valid auxiliar segment that will be used by this method.
        \param auxSeg3Ptr A pointer to a valid auxiliar segment that will be used by this method.
        \param minFoundDissimilarity The minimum dissimilarity value found.
        \param maxFoundDissimilarity The maximum dissimilarity value found.
        \param totalMergesNumber The total number of merges.
        \param mergeIterationCounter A reference to a iteration number counter (this variable will be only incremented, never zeroed. It never must be reset. ).
        \param actSegsListHeadPtr A pointer the the active segments list head.
      */
      template< typename FeatureDataTypeT >
      void mergeSegments( SegmentsIdsMatrixT& segmentsIds,
        const FeatureDataTypeT disimilarityThreshold,
        const unsigned int maxSegSizeThreshold,
        SegmenterIdsManager& segmenterIdsManager,
        SegmenterRegionGrowingMerger< FeatureDataTypeT >& merger,
        const bool enablelocalMutualBestFitting,
        const bool enableSameIterationMerges,
        SegmenterRegionGrowingSegment< FeatureDataTypeT >* auxSeg1Ptr,
        SegmenterRegionGrowingSegment< FeatureDataTypeT >* auxSeg2Ptr,
        SegmenterRegionGrowingSegment< FeatureDataTypeT >* auxSeg3Ptr,
        FeatureDataTypeT& minFoundDissimilarity,
        FeatureDataTypeT& maxFoundDissimilarity,
        unsigned int& totalMergesNumber,
        IterationCounterType& globalMergeIterationsCounter,
        SegmenterRegionGrowingSegment< FeatureDataTypeT >** const actSegsListHeadPtrPtr )
      {
        TERP_TRUE_OR_THROW( actSegsListHeadPtrPtr != 0, "Invalid active segments list header pointer" );
        TERP_TRUE_OR_THROW( (*actSegsListHeadPtrPtr) != 0, "Invalid active segments list header pointer" );
      
        minFoundDissimilarity = std::numeric_limits< FeatureDataTypeT >::max();      
        maxFoundDissimilarity = FeatureDataTypeT( -1.0 * minFoundDissimilarity );
        totalMergesNumber = 0;
      
        // Globals
        FeatureDataTypeT internalDisimilarityThreshold =
          disimilarityThreshold;
        unsigned int internalMaxSegSizeThreshold = maxSegSizeThreshold;

        if( maxSegSizeThreshold ) {
          internalDisimilarityThreshold = std::numeric_limits< FeatureDataTypeT >::max();

        } else {
          internalMaxSegSizeThreshold = std::numeric_limits< unsigned int >::max();

        }

        unsigned int iterationMergedSegmentsNumber = 0;
        unsigned int segmentsLine = 0;
        unsigned int segmentsLineBound = 0;
        unsigned int segmentCol = 0;
        unsigned int segmentColStart = 0;
        unsigned int segmentColBound = 0;
        SegmenterRegionGrowingSegment< FeatureDataTypeT >* minForwardDissimilaritySegmentPtr = 0;
        FeatureDataTypeT forwardDissimilarityValue = 0;
        FeatureDataTypeT minForwardDissimilarityValue = 0;
        SegmenterRegionGrowingSegment< FeatureDataTypeT >* minBackwardDissimilaritySegmentPtr = 0;
        FeatureDataTypeT backwardDissimilarityValue = 0;
        FeatureDataTypeT minBackwardDissimilarityValue = 0;
        SegmenterSegmentsBlock::SegmentIdDataType* segmentsIdsLinePtr = 0;
        SegmenterSegmentsBlock::SegmentIdDataType currentSegmentId = 0;
        std::list< SegmenterSegmentsBlock::SegmentIdDataType > freeSegmentIds;
        unsigned int neighborSegIdx = 0;
        SegmenterRegionGrowingSegment< FeatureDataTypeT >* currActSegPtr = (*actSegsListHeadPtrPtr);
      
        // Main iterations loop
        do {
          iterationMergedSegmentsNumber = 0;
          currActSegPtr = (*actSegsListHeadPtrPtr);
        
          // Updating the merger state        
          merger.update( *actSegsListHeadPtrPtr );
        
          // iterating over each segment
          do {
            if( ( enableSameIterationMerges || ( currActSegPtr->m_mergetIteration < globalMergeIterationsCounter ) )
            && 
            ( currActSegPtr->m_size <= internalMaxSegSizeThreshold ) ) {

              // finding the neighbor segment with minimum dissimilary value
              // related to the current sement 
            
              minForwardDissimilaritySegmentPtr = 0;
              minForwardDissimilarityValue = 
                std::numeric_limits< FeatureDataTypeT >::max();
            
              for( neighborSegIdx = 0 ; neighborSegIdx < currActSegPtr->m_neighborSegmentsSize ;
                   ++neighborSegIdx ) {

                if( currActSegPtr->m_neighborSegments[ neighborSegIdx ] ) {
                  forwardDissimilarityValue = merger.getDissimilarity( currActSegPtr,
                                   currActSegPtr->m_neighborSegments[ neighborSegIdx ],
                                   auxSeg1Ptr );
                  
                  if( ( forwardDissimilarityValue <= internalDisimilarityThreshold )
                      &&
                      ( forwardDissimilarityValue < minForwardDissimilarityValue ) ) {
                    minForwardDissimilarityValue = forwardDissimilarityValue;
                    minForwardDissimilaritySegmentPtr = currActSegPtr->m_neighborSegments[ neighborSegIdx ];
                    auxSeg3Ptr->operator=( *auxSeg1Ptr );
                  }
                }
              }
            
              // does the neighbor wants to merge back ?
            
              if( enablelocalMutualBestFitting && ( minForwardDissimilaritySegmentPtr != 0 ) ) {
                // Calculating all neighbor neighbor segments dissimilarity          
              
                minBackwardDissimilaritySegmentPtr = 0;
                backwardDissimilarityValue = 0;
                minBackwardDissimilarityValue = 
                  std::numeric_limits< FeatureDataTypeT >::max();
              
                for( neighborSegIdx = 0 ;
                     neighborSegIdx < minForwardDissimilaritySegmentPtr->m_neighborSegmentsSize ;
                     ++neighborSegIdx ) {

                  if( minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ] ) {

                    backwardDissimilarityValue = 
                      merger.getDissimilarity( minForwardDissimilaritySegmentPtr, 
                             minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ],
                             auxSeg2Ptr );
                    
                    if( backwardDissimilarityValue < minBackwardDissimilarityValue ) {
                      minBackwardDissimilarityValue = backwardDissimilarityValue;
                      minBackwardDissimilaritySegmentPtr = 
                        minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ];
                    }
                  }
                }
              
                if( minBackwardDissimilaritySegmentPtr !=  currActSegPtr ) {
                  minForwardDissimilaritySegmentPtr = 0;
                }
              }
            
              // if the min forward dissimilarity segment was found
              if( ( minForwardDissimilaritySegmentPtr != 0 ) 
              &&
              ( enableSameIterationMerges ||
                ( minForwardDissimilaritySegmentPtr->m_mergetIteration < globalMergeIterationsCounter ) 
              ) ) {

                if( minFoundDissimilarity > minForwardDissimilarityValue ) {
                  minFoundDissimilarity = minForwardDissimilarityValue;
                }
    
                if( maxFoundDissimilarity < minForwardDissimilarityValue ) {
                  maxFoundDissimilarity = minForwardDissimilarityValue;
                }             
              
                // If the maximum similary neighbor was found it will be merged
                // if the dissimilarity value is below the threshold
                // merging segment data
              
                merger.mergeFeatures( currActSegPtr,
                          minForwardDissimilaritySegmentPtr,
                          auxSeg3Ptr );
              
                currActSegPtr->m_mergetIteration = globalMergeIterationsCounter;
                
                currActSegPtr->removeNeighborSegmentPtr( minForwardDissimilaritySegmentPtr );
                
                // updating the max similarity segment neighborhood segments
                // with the current segment
              
                for( neighborSegIdx = 0 ;
                     neighborSegIdx < minForwardDissimilaritySegmentPtr->m_neighborSegmentsSize;
                     ++neighborSegIdx ) {

                  if( 
                     ( minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ] != 0 )
                     &&
                     ( minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ] != currActSegPtr ) ) {
                    // adding the max similarity neighborhood segments to the 
                    // current one, if it is not already there            
                  
                    currActSegPtr->addNeighborSegmentPtr( 

                    minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ] );
                    
                    // adding the current segment into the max similarity 
                    // neighborhood segments list, if it is not already there              
                  
                    minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ]->addNeighborSegmentPtr( 
                      currActSegPtr );
                    
                    // removing the merged segment reference from its neighbor
                    // list      
                    minForwardDissimilaritySegmentPtr->m_neighborSegments[ neighborSegIdx ]->removeNeighborSegmentPtr( 
                      minForwardDissimilaritySegmentPtr );
                  }
                }      
              
                // updating the segments Ids container matrix
              
                segmentsLineBound = minForwardDissimilaritySegmentPtr->m_yBound;
                segmentColStart = minForwardDissimilaritySegmentPtr->m_xStart;
                segmentColBound = minForwardDissimilaritySegmentPtr->m_xBound;          
                currentSegmentId = currActSegPtr->m_id;
                
                for( segmentsLine = minForwardDissimilaritySegmentPtr->m_yStart ; 
                     segmentsLine < segmentsLineBound ; ++segmentsLine ) {
                  segmentsIdsLinePtr = segmentsIds[ segmentsLine ];
                
                  for( segmentCol = segmentColStart ; segmentCol < segmentColBound ; ++segmentCol ) {
                    if( segmentsIdsLinePtr[ segmentCol ] ==
                  minForwardDissimilaritySegmentPtr->m_id ) {
                      segmentsIdsLinePtr[ segmentCol ] = currentSegmentId;
                    }
                  }
                }
              
                // disabling the  merged segment
                // The merged segment id will be given back to ids manager
              
                if( minForwardDissimilaritySegmentPtr == (*actSegsListHeadPtrPtr) ) {
                  (*actSegsListHeadPtrPtr) = (*actSegsListHeadPtrPtr)->m_nextActiveSegment;
                }
              
                if( minForwardDissimilaritySegmentPtr->m_prevActiveSegment ) {
                  minForwardDissimilaritySegmentPtr->m_prevActiveSegment->m_nextActiveSegment =
                    minForwardDissimilaritySegmentPtr->m_nextActiveSegment;
                }
                
                if( minForwardDissimilaritySegmentPtr->m_nextActiveSegment ) {
                  minForwardDissimilaritySegmentPtr->m_nextActiveSegment->m_prevActiveSegment =
                    minForwardDissimilaritySegmentPtr->m_prevActiveSegment;
                }
              
                minForwardDissimilaritySegmentPtr->disable();
              
                minForwardDissimilaritySegmentPtr->removeAllNeighborSegmentsPtrs();
                
                freeSegmentIds.push_back( minForwardDissimilaritySegmentPtr->m_id );
              
                ++iterationMergedSegmentsNumber;
              }
            }
          
            // going to the next segment
            currActSegPtr = currActSegPtr->m_nextActiveSegment;
          
          } while( currActSegPtr );
        
          // give back the free unused sement ids
        
          if( ! freeSegmentIds.empty() ) {
            segmenterIdsManager.addFreeIDs( freeSegmentIds );
            freeSegmentIds.clear();
          }
        
          totalMergesNumber += iterationMergedSegmentsNumber;
        
          //         std::cout << std::endl << "Iteration Number: " << iterationNumber <<
          //           "  Merged segments number:" << iterationMergedSegmentsNumber << std::endl;
        
          ++globalMergeIterationsCounter;

        } while( iterationMergedSegmentsNumber &&
           ( globalMergeIterationsCounter <
             std::numeric_limits< IterationCounterType >::max() ) );
      }
    } // end namespace rg
  } // end namespace rp
} // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGFUNCTIONS_H
