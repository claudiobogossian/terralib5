/*  Copyright (C) 2008-2015 National Institute For Space Research (INPE) - Brazil.

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

#include <limits>

namespace te
{
  namespace rp
  {  
    /*!
      \brief Region Growing segment.
    */
    template< typename FeatureDataTypeT >
    struct SegmenterRegionGrowingSegment
    {
      /*!
        \brief Feature type definition.
      */          
      typedef FeatureDataTypeT FeatureType;

      /*!
        \brief Feature type definition.
      */          
      typedef unsigned short int IterationCounterType;        

      /*!
        \brief Segment ID.
      */             
      SegmenterSegmentsBlock::SegmentIdDataType m_id;   

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
      SegmenterRegionGrowingSegment< FeatureDataTypeT >** m_neighborSegments;           

      /*!
        \brief The current size of m_neighborSegments.
      */             
      unsigned int m_neighborSegmentsSize;  

      /*!
        \brief A pointer to the previous active segment.
      */             
      SegmenterRegionGrowingSegment< FeatureDataTypeT >* m_prevActiveSegment;

      /*!
        \brief A pointer to the next active segment.
      */             
      SegmenterRegionGrowingSegment< FeatureDataTypeT >* m_nextActiveSegment;                 

      /*!
        \brief A pionter to a fixed size vector of segment features.
        \note The segments pool have the ownership of this vector.
      */             
      FeatureType* m_features; 

      /*!
        \brief The current size of m_features.
      */                     
      unsigned int m_featuresSize;

      /*!
        \brief The current merge iteration.
        \note Disabled: std::numeric_limits< SegmenterRegionGrowingSegment::IterationCounterType>::max()
      */                             
      IterationCounterType m_mergetIteration;

      //overload
      SegmenterRegionGrowingSegment< FeatureDataTypeT >& operator=
        ( const SegmenterRegionGrowingSegment< FeatureDataTypeT >& other );

      /*!
        \brief Add a pointer of a neighbor segment (if it is not already there).
      */             
      void addNeighborSegmentPtr( SegmenterRegionGrowingSegment< FeatureDataTypeT > * const nSegPtr );

      /*!
        \brief Remove all occurrences of a neighbor segment.
      */             
      void removeNeighborSegmentPtr( SegmenterRegionGrowingSegment< FeatureDataTypeT > * const nSegPtr );        

      /*!
        \brief Remove all neighbor segments.
      */             
      void removeAllNeighborSegmentsPtrs();    

      /*!
        \brief Disable this segment ( same as m_mergetIteration = std::numeric_limits< SegmenterRegionGrowingSegment::IterationCounterType>::max() ).
      */             
      inline void disable() { m_mergetIteration = std::numeric_limits< IterationCounterType>::max(); };

      /*!
        \brief Returns true if this segment is enabled.
        \returns Returns true if this segment is enabled.          
      */             
      inline bool isEnabled() const { return m_mergetIteration != std::numeric_limits< IterationCounterType>::max(); };        

    };

    template< typename FeatureDataTypeT > 
    SegmenterRegionGrowingSegment< FeatureDataTypeT >&
      SegmenterRegionGrowingSegment< FeatureDataTypeT >::operator=( const SegmenterRegionGrowingSegment< FeatureDataTypeT >& other )
    {
      m_id = other.m_id;
      m_size = other.m_size;
      m_xStart = other.m_xStart;
      m_xBound = other.m_xBound;
      m_yStart = other.m_yStart;
      m_yBound = other.m_yBound;
      m_mergetIteration = other.m_mergetIteration;
      m_prevActiveSegment = other.m_prevActiveSegment;
      m_nextActiveSegment = other.m_nextActiveSegment;

      free( m_neighborSegments );
      if( other.m_neighborSegmentsSize )
      {
        m_neighborSegments = (SegmenterRegionGrowingSegment**)malloc( 
          sizeof( SegmenterRegionGrowingSegment* ) *
          other.m_neighborSegmentsSize );
        memcpy( m_neighborSegments, other.m_neighborSegments, 
          sizeof( SegmenterRegionGrowingSegment* ) *
          other.m_neighborSegmentsSize );
        m_neighborSegmentsSize = other.m_neighborSegmentsSize;
      }
      else
      {
        m_neighborSegments = 0;
        m_neighborSegmentsSize = 0;
      }

      if( other.m_featuresSize )
      {
        memcpy( m_features, other.m_features, 
          sizeof( FeatureType ) * other.m_featuresSize );
      }      
      m_featuresSize = other.m_featuresSize;

      return *this;
    }

    template< typename FeatureDataTypeT >
    void SegmenterRegionGrowingSegment< FeatureDataTypeT >::addNeighborSegmentPtr( 
      SegmenterRegionGrowingSegment * const nSegPtr )
    {
      if( nSegPtr )
      {
        unsigned int idx = 0;

        for( idx = 0 ; idx < m_neighborSegmentsSize ; ++idx )
        {
          if( m_neighborSegments[ idx ] == nSegPtr )
          {
            return;
          }          
        }

        for( idx = 0 ; idx < m_neighborSegmentsSize ; ++idx )
        {
          if( m_neighborSegments[ idx ] == 0 )
          {
            m_neighborSegments[ idx ] = nSegPtr;
            return;
          }
        }

        m_neighborSegments = (SegmenterRegionGrowingSegment**)realloc( 
          m_neighborSegments, ( m_neighborSegmentsSize + 1 ) * sizeof( 
          SegmenterRegionGrowingSegment* ) );

        m_neighborSegments[ m_neighborSegmentsSize ] = nSegPtr;

        m_neighborSegmentsSize = m_neighborSegmentsSize + 1;
      }
    }

    template< typename FeatureDataTypeT >
    void SegmenterRegionGrowingSegment< FeatureDataTypeT >::removeNeighborSegmentPtr( 
      SegmenterRegionGrowingSegment< FeatureDataTypeT > * const nSegPtr )
    {
      unsigned int numberOfZeroes = 0;
      unsigned int idx = 0;
      for( ; idx < m_neighborSegmentsSize ; ++idx )
      {
        if( m_neighborSegments[ idx ] == nSegPtr )
        {
          m_neighborSegments[ idx ] = 0;
          ++numberOfZeroes;
        } 
        else if( m_neighborSegments[ idx ] == 0 )
        {
          ++numberOfZeroes;
        }
      }

      unsigned int numberOfNonZeroes = m_neighborSegmentsSize - numberOfZeroes;

      if( ( m_neighborSegmentsSize > 4 ) && ( numberOfNonZeroes < numberOfZeroes ) )
      {
        SegmenterRegionGrowingSegment** auxVecPtr = (SegmenterRegionGrowingSegment**)
          malloc( numberOfNonZeroes * sizeof( SegmenterRegionGrowingSegment* ) );

        unsigned int idx2 = 0;  

        for( idx = 0 ; idx < m_neighborSegmentsSize ; ++idx )
        {
          if( m_neighborSegments[ idx ] )
          {
            auxVecPtr[ idx2++ ] = m_neighborSegments[ idx ];
          }
        }

        free( m_neighborSegments );
        m_neighborSegments = auxVecPtr;
        m_neighborSegmentsSize = numberOfNonZeroes;
      }
    }  

    template< typename FeatureDataTypeT >
    void SegmenterRegionGrowingSegment< FeatureDataTypeT >::removeAllNeighborSegmentsPtrs()
    {
      free( m_neighborSegments );
      m_neighborSegments = 0;
      m_neighborSegmentsSize = 0;
    }
  } // namespace rp
} // namespace te

#endif // __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGSEGMENT_H
