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
  \file terralib/rp/SegmenterRegionGrowingMerger.h
  \brief Segmenter merger used in region growing process.
 */

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGMERGER_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGMERGER_H

#include "SegmenterRegionGrowingSegment.h"
#include "SegmenterSegmentsBlock.h"

#include "Matrix.h"

namespace te
{
  namespace rp
  {
    /*!
      \brief Internal segments ids matrix type definition.
    */          
    typedef Matrix< SegmenterSegmentsBlock::SegmentIdDataType >
      SegmentsIdsMatrixT;
              
    /*!
      \class SegmenterRegionGrowingMerger
      \brief Abstract class for the segments merger.
    */        
    template< typename FeatureDataTypeT >
    class TERPEXPORT SegmenterRegionGrowingMerger {
      public:
        virtual ~SegmenterRegionGrowingMerger()
	      {
	      };
            
	      /*!
	        \brief Returns a dimilarity index between this and the other segment.
	        \param segment1Ptr A pointer to the first segment.
	        \param segment2Ptr A pointer to the second segment.
	        \param mergePreviewSegPtr A pointer to a valid segment where the merged features values will be stored (when aplicable).
	        \return A similarity index between this and the other segment ( normalized between 0 and 1 ).
	      */              
	      virtual FeatureDataTypeT
	        getDissimilarity( SegmenterRegionGrowingSegment< FeatureDataTypeT > const * const segment1Ptr, 
			          SegmenterRegionGrowingSegment< FeatureDataTypeT > const * const segment2Ptr, 
			          SegmenterRegionGrowingSegment< FeatureDataTypeT > * const mergePreviewSegPtr ) const = 0;
              
	      /*!
	        \brief Merge specific segment features from both segments into the first segment.
	        \param segment1Ptr The first segment.
	        \param segment2Ptr A pointer to the second segment.
	        \param mergePreviewSegPtr A pointer to a valid segment where the merged features values were be stored by calling getDissimilarityIndex (when aplicable).
	      */                
	      virtual void mergeFeatures( SegmenterRegionGrowingSegment< FeatureDataTypeT > * const segment1Ptr, 
	        SegmenterRegionGrowingSegment< FeatureDataTypeT > const * const segment2Ptr, 
		      SegmenterRegionGrowingSegment< FeatureDataTypeT > const * const mergePreviewSegPtr ) const = 0;
	
	      /*!
	        \brief Update the internal state.
	        \param actSegsListHeadPtr A pointer the the active segments list head.
	      */    
	      virtual void update( SegmenterRegionGrowingSegment< FeatureDataTypeT >* const actSegsListHeadPtr ) = 0;

	      /*!
	        \brief Return the required segments features vector size (numer of elements).
	        \return Return the required segments features vector size (numer of elements).
	      */                
	      virtual unsigned int getSegmentFeaturesSize() const = 0;
            
      protected :
	      /*!
	        \brief Constructor.
	      */
	      SegmenterRegionGrowingMerger()
	      {
	      };

      private :
	      /*!
	        \brief Copy Constructor.
	      */
	      SegmenterRegionGrowingMerger( const SegmenterRegionGrowingMerger& ) {};
            
	      const SegmenterRegionGrowingMerger&
	        operator=( const SegmenterRegionGrowingMerger& other ) { return other; };
    };    
  } // end namespace rp
} // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGMERGER_H
