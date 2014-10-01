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
  \file terralib/rp/SegmenterRegionGrowingSegmentsPool.h
  \brief Segments pool.
 */

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGSEGMENTSPOOL_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGSEGMENTSPOOL_H

  #include "Config.h"
  #include "SegmenterRegionGrowingSegment.h"
  #include "SegmenterSegmentsBlock.h"
  #include "Matrix.h"
  
  #include <boost/noncopyable.hpp>
  
  namespace te
  {
    namespace rp
    {  
      /*!
          \class SegmenterRegionGrowingSegmentsPool
          \brief Segments pool.
          \note The vector pointed by each SegmenterRegionGrowingSegment::m_neighborSegments will be deleted by the pool.
      */
      class TERPEXPORT SegmenterRegionGrowingSegmentsPool : private boost::noncopyable 
      {
        public :
          
          SegmenterRegionGrowingSegmentsPool();
          
          ~SegmenterRegionGrowingSegmentsPool();          
            
          /*!
            \brief Pool initialization.
            \param segsNumber The number of segments to allocate.
            \param featuresNumber The number of features for each created segment.
            \return true if ok, false on errors.
            \note The segments pool will initiate just SegmenterRegionGrowingSegment::m_features vector and take the ownership of it.
            \note The segments pool will initiate just SegmenterRegionGrowingSegment::m_status to false.
          */            
          bool initialize( const SegmenterSegmentsBlock::SegmentIdDataType segsNumber,
            const unsigned int featuresNumber );            
          
          /*!
            \brief Retrive a stored segment.
            \return A pointer to the next unused segment or null if all segments were used.
          */          
          SegmenterRegionGrowingSegment* getNextSegment();          
         
          /*!
            \brief Clear all stored segments.
          */          
          void clear();          
          
          /*!
            \brief Reset the segments use counter.
          */          
          void resetUseCounter();    

          /*!
            \brief Return the current allocated segments number.
          */          
          SegmenterSegmentsBlock::SegmentIdDataType getSegmentsNumber() const;
          
          /*!
            \brief Return a reference to the internal segments matrix.
            \return Return a reference to the internal segments matrix.
            \note The number of matrix elemetns may be greater than the requested number of segments.
          */          
          inline Matrix< SegmenterRegionGrowingSegment >& getSegsMatrix()
          {
            return m_segments;
          };
            
        protected :

          /*!
            \brief The number of segments allocated segments.
          */            
          SegmenterSegmentsBlock::SegmentIdDataType m_segsNumber;

          /*!
            \brief The number of features for each created segment.
          */                      
          unsigned int m_featuresNumber;

          /*!
            \brief The next unused segment index.
          */                                
          SegmenterSegmentsBlock::SegmentIdDataType m_nextUnusedSegmentIdx;
          
          Matrix< SegmenterRegionGrowingSegment > m_segments; //!< Internal stored segments.
          
          Matrix< SegmenterRegionGrowingSegment::FeatureType > m_segmentsFeatures; //!< Internal stored segment features.
          
          // Variables used by getNextSegment
          
          unsigned int m_getNextSegment_line;
          unsigned int m_getNextSegment_col;

      };
    } // namespace rp
  } // namespace te

#endif
