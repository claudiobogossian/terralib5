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
  \file terralib/rp/SegmenterRegionGrowingSegment.h
  \brief Region Growing segment.
 */

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGSEGMENT_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGSEGMENT_H

  #include "Config.h"
  #include "SegmenterSegmentsBlock.h"
  
  namespace te
  {
    namespace rp
    {  
      /*!
          \brief Region Growing segment.
      */
      struct TERPEXPORT SegmenterRegionGrowingSegment
      {
        /*!
          \brief Feature type definition.
         */          
        typedef float FeatureType;
          
        /*!
          \brief Segment ID.
        */             
        SegmenterSegmentsBlock::SegmentIdDataType m_id;   

        /*!
          \brief Segment status (active=true).
        */                    
        bool m_status;
        
        /*!
          \brief Segment area (pixels number).
        */              
        unsigned int m_size;             
        
        /*!
          \brief Segment left X coordinate box over the label image.
        */              
        unsigned int m_xStart;

        /*!
          \brief Segment lower bound X coordinate box over the label image.
        */              
        unsigned int m_xBound;

        /*!
          \brief Segment upper Y coordinate box over the label image.
        */              
        unsigned int m_yStart;

        /*!
          \brief Segment lower bound Y coordinate box over the label image.
        */              
        unsigned int m_yBound;     
        
        /*!
          \brief Neighborhood segments pointers (some pointers can be null) or a null pointer if there is no neighborhood.
          \note The segments pool will delete the vector pointed by this pointer at this segment deletion time.
        */             
        SegmenterRegionGrowingSegment** m_neighborSegments;           
        
        /*!
          \brief The current size of m_neighborSegments.
        */             
        unsigned int m_neighborSegmentsSize;  

        /*!
          \brief A pionter to a fixed size vector of segment features.
          \note The segments pool have the ownership of this vector.
        */             
        FeatureType* m_features; 

        /*!
          \brief The current size of m_features.
        */                     
        unsigned int m_featuresSize;
        
        //overload
        SegmenterRegionGrowingSegment& operator=( const SegmenterRegionGrowingSegment& other );
        
        /*!
          \brief Add a pointer of a neighbor segment (if it is not already there).
        */             
        void addNeighborSegment( SegmenterRegionGrowingSegment * const nSegPtr );
        
        /*!
          \brief Remove all occurrences of a neighbor segment.
        */             
        void removeNeighborSegment( SegmenterRegionGrowingSegment * const nSegPtr );        
        
        /*!
          \brief Remove all neighbor segments.
        */             
        void clearNeighborSegments();    
      };
    } // namespace rp
  } // namespace te

#endif
