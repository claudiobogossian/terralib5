#include "SegmenterRegionGrowingSegment.h"

#include <cstdlib>
#include <string.h>

namespace te
{
  namespace rp
  { 
    SegmenterRegionGrowingSegment& SegmenterRegionGrowingSegment::operator=( 
      const SegmenterRegionGrowingSegment& other )
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
    
    void SegmenterRegionGrowingSegment::addNeighborSegment( 
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
          
        ++m_neighborSegmentsSize;
      }
    }
    
    void SegmenterRegionGrowingSegment::removeNeighborSegment( 
      SegmenterRegionGrowingSegment * const nSegPtr )
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
    
    void SegmenterRegionGrowingSegment::clearNeighborSegments()
    {
      free( m_neighborSegments );
      m_neighborSegments = 0;
      m_neighborSegmentsSize = 0;
    }
  } // namespace rp
} // namespace te
