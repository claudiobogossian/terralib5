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
  \file terralib/rp/SegmenterRegionGrowingMeanMerger.h
  \brief Segmenter mean merger used in region growing process.
*/

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGMEANMERGER_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGMEANMERGER_H

#include "SegmenterRegionGrowingFunctions.h"
#include "SegmenterRegionGrowingMerger.h"

namespace te
{
  namespace rp
  {
    /*!
      \class SegmenterRegionGrowingMeanMerger
      \brief Mean based Segments merger
    */        
    class TERPEXPORT SegmenterRegionGrowingMeanMerger :
      public SegmenterRegionGrowingMerger< rg::MeanFeatureType >
    {
      public:
            
        SegmenterRegionGrowingMeanMerger( const unsigned int featuresNumber );
            
        ~SegmenterRegionGrowingMeanMerger();
            
        //overload        
        SegmenterRegionGrowingSegment< rg::MeanFeatureType >::FeatureType
          getDissimilarity( SegmenterRegionGrowingSegment< rg::MeanFeatureType > const * const segment1Ptr, 
            SegmenterRegionGrowingSegment< rg::MeanFeatureType > const * const segment2Ptr, 
            SegmenterRegionGrowingSegment< rg::MeanFeatureType > * const mergePreviewSegPtr ) const;
              
        //overload                
        void mergeFeatures( SegmenterRegionGrowingSegment< rg::MeanFeatureType > * const segment1Ptr, 
          SegmenterRegionGrowingSegment< rg::MeanFeatureType > const * const segment2Ptr, 
          SegmenterRegionGrowingSegment< rg::MeanFeatureType > const * const mergePreviewSegPtr ) const;
              
        //overload
        void update( SegmenterRegionGrowingSegment< rg::MeanFeatureType >* const ) {};  
            
        //overload
        inline unsigned int getSegmentFeaturesSize() const { return m_featuresNumber; };
            
      protected :
        unsigned int m_featuresNumber; //!< The number of features (bands).
        SegmenterRegionGrowingSegment< rg::MeanFeatureType >::FeatureType m_dissimilarityNormFactor;
            
        // variables used by the method getDissimilarity
        mutable SegmenterRegionGrowingSegment< rg::MeanFeatureType >::FeatureType m_getDissimilarity_dissValue;
        mutable SegmenterRegionGrowingSegment< rg::MeanFeatureType >::FeatureType m_getDissimilarity_diffValue; 
        mutable unsigned int m_getDissimilarity_meansIdx;
    };        
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGMEANMERGER_H
