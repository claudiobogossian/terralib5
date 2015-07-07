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
  \file terralib/rp/SegmenterRegionGrowingBaatzMerger.h
  \brief Segmenter Baatz merger used in region growing process.
*/

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGBAATZMERGER_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGBAATZMERGER_H

#include "SegmenterRegionGrowingFunctions.h"
#include "SegmenterRegionGrowingMerger.h"
#include "SegmenterSegmentsBlock.h"

#include "Config.h"

#include <vector>

namespace te
{
  namespace rp
  {
    /*!
      \class SegmenterRegionGrowingBaatzMerger
      \brief Baatz based Segments merger
    */        
    class TERPEXPORT SegmenterRegionGrowingBaatzMerger :
      public SegmenterRegionGrowingMerger< rg::BaatzFeatureType >
    {
      public:
        /*!
          \brief Default constructor.
          \param bandsWeights A reference to an external valid structure where each bands weight are stored.
          \param segmentsIds //!< A reference to an external valid structure where all segments IDs are stored.
          \param segmentsMatrix //!< A reference to an external valid segments matrix.
          \param colorWeight //!< The weight given to the color component, deafult:0.5, valid range: [0,1].
          \param compactnessWeight //!< The weight given to the compactness component, deafult:0.5, valid range: [0,1].
        */
        SegmenterRegionGrowingBaatzMerger( const double& colorWeight,
          const double& compactnessWeight,
          const std::vector< double >& bandsWeights,
          const SegmentsIdsMatrixT& segmentsIds );
      
        ~SegmenterRegionGrowingBaatzMerger();
            
        //overload        
        SegmenterRegionGrowingSegment< rg::BaatzFeatureType >::FeatureType
          getDissimilarity( SegmenterRegionGrowingSegment< rg::BaatzFeatureType > const * const segment1Ptr, 
          SegmenterRegionGrowingSegment< rg::BaatzFeatureType > const * const segment2Ptr, 
          SegmenterRegionGrowingSegment< rg::BaatzFeatureType > * const mergePreviewSegPtr ) const;
              
        //overload                
        void mergeFeatures( SegmenterRegionGrowingSegment< rg::BaatzFeatureType > * const segment1Ptr, 
          SegmenterRegionGrowingSegment< rg::BaatzFeatureType > const * const segment2Ptr, 
          SegmenterRegionGrowingSegment< rg::BaatzFeatureType > const * const mergePreviewSegPtr ) const;
              
        //overload
        void update( SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* const actSegsListHeadPtr );
            
        //overload
        inline unsigned int getSegmentFeaturesSize() const { return 3 + ( 3 * m_bandsNumber ); };            
              
      protected :
        const SegmentsIdsMatrixT& m_segmentsIds; //!< A reference to an external valid structure where each all segments IDs are stored.
            
        unsigned int m_bandsNumber; //!< The number of features (bands).
            
        rg::BaatzFeatureType m_allSegsCompactnessOffset; //!< The offsets applied to normalize the compactness value.
        rg::BaatzFeatureType m_allSegsCompactnessGain; //!< The gains applied to normalize the compactness value.
        rg::BaatzFeatureType m_allSegsSmoothnessOffset; //!< The offsets applied to normalize the smoothness value.
        rg::BaatzFeatureType m_allSegsSmoothnessGain; //!< The gains applied to normalize the smoothness value.
        rg::BaatzFeatureType m_allSegsStdDevOffset; //!< The offsets applied to normalize the standard deviation value.
        rg::BaatzFeatureType m_allSegsStdDevGain; //!< The gains applied to normalize the standard deviation value.
        rg::BaatzFeatureType m_colorWeight; //!< The weight given to the color component, deafult:0.5, valid range: [0,1].
        rg::BaatzFeatureType m_compactnessWeight; //!< The weight given to the compactness component, deafult:0.5, valid range: [0,1].
        std::vector< rg::BaatzFeatureType > m_bandsWeights; //!< A vector where each bands weight are stored.

        // Variables used by the method getDissimilarity            
        mutable rg::BaatzFeatureType m_getDissimilarity_sizeUnionD;            
        mutable rg::BaatzFeatureType m_getDissimilarity_sizeSeg1D;
        mutable rg::BaatzFeatureType m_getDissimilarity_sizeSeg2D;
        mutable unsigned int m_getDissimilarity_touchingEdgeLength1;
        mutable unsigned int m_getDissimilarity_touchingEdgeLength2;               
        mutable rg::BaatzFeatureType m_getDissimilarity_hCompact; 
        mutable rg::BaatzFeatureType m_getDissimilarity_hSmooth;
        mutable rg::BaatzFeatureType m_getDissimilarity_hForm;
        mutable rg::BaatzFeatureType m_getDissimilarity_hColor;
        mutable rg::BaatzFeatureType m_getDissimilarity_sumUnion;
        mutable rg::BaatzFeatureType m_getDissimilarity_squaresSumUnion;
        mutable rg::BaatzFeatureType m_getDissimilarity_meanUnion;
        mutable rg::BaatzFeatureType m_getDissimilarity_stdDevUnion;
        mutable unsigned int m_getDissimilarity_sumsIdx;
            
        // Variables used by the method update
        mutable rg::BaatzFeatureType m_update_compactnessMin;
        mutable rg::BaatzFeatureType m_update_compactnessMax;
        mutable rg::BaatzFeatureType m_update_smoothnessMin;
        mutable rg::BaatzFeatureType m_update_smoothnessMax;
        mutable rg::BaatzFeatureType m_update_stdDevMin;
        mutable rg::BaatzFeatureType m_update_stdDevMax;
        mutable rg::BaatzFeatureType* m_update_featuresPtr;
        mutable SegmenterRegionGrowingSegment< rg::BaatzFeatureType >* m_update_currentActSegPtr;
        mutable unsigned int m_update_band;
        mutable rg::BaatzFeatureType const* m_update_stdDevPtr;

        /*!
          \brief Returns the count of points from region 1 (with ID1) touching the region 2 (with ID2).
          \param segsIds The segment ids container.
          \param xStart The upper left X of the bounding box surrounding both regions.
          \param yStart The upper left Y of the bounding box surrounding both regions.
          \param xBound The lower right X bound of the bounding box surrounding both regions.
          \param yBound The lower right Y bound of the bounding box surrounding both regions.
          \param id1 Region 1 ID.
          \param id2 Region 2 ID.
          \param edgeLength1 The touching edge length for the region 1.
          \param edgeLength2 The touching edge length for the region 2.
          \return Returns the count of points from region 1 (with ID1) touching the region 2 (with ID2).
        */            
        static void getTouchingEdgeLength( const SegmentsIdsMatrixT& segsIds,
          const unsigned int& xStart, const unsigned int& yStart,
          const unsigned int& xBound, const unsigned int& yBound,
          const SegmenterSegmentsBlock::SegmentIdDataType& id1,
          const SegmenterSegmentsBlock::SegmentIdDataType& id2,
          unsigned int& edgeLength1,
          unsigned int& edgeLength2 );
    };
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGBAATZMERGER_H
