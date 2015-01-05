#include "SegmenterRegionGrowingSegmentsPool.h"

#include <cmath>

namespace te
{
  namespace rp
  { 
    
    SegmenterRegionGrowingSegmentsPool::SegmenterRegionGrowingSegmentsPool()
    {
      m_segsNumber = 0;
      m_featuresNumber = 0;
      m_nextUnusedSegmentIdx = 0;
    }

    SegmenterRegionGrowingSegmentsPool::~SegmenterRegionGrowingSegmentsPool()
    {
      clear();
    }
    
    bool SegmenterRegionGrowingSegmentsPool::initialize(
      const SegmenterSegmentsBlock::SegmentIdDataType segsNumber,
      const unsigned int featuresNumber)
    {
      if( ( segsNumber > m_segsNumber ) || ( featuresNumber != m_featuresNumber ) )
      {
        clear();
        
        if( segsNumber )
        {
          unsigned int nCols = (unsigned int)std::floor( std::sqrt( (double)segsNumber ) );
          unsigned int nRows = (unsigned int)std::ceil( ((double)segsNumber) / 
             ((double)nCols) );
          assert( ( nCols * nRows ) >= segsNumber );
          
          if( m_segments.reset( nRows, nCols, 
            Matrix< SegmenterRegionGrowingSegment >::RAMMemPol ) == false )
          {
            return false;
          }
          
          m_segsNumber = segsNumber;
          
          if( featuresNumber )
          {
            if( m_segmentsFeatures.reset( nRows, nCols * featuresNumber, 
              Matrix< SegmenterRegionGrowingSegment::FeatureType >::RAMMemPol ) == false )
            {
              clear();
              return false;
            }
            
            m_featuresNumber = featuresNumber;
            
            unsigned int col = 0;
            for( unsigned int row = 0 ; row < nRows ; ++row )
            {
              for( col = 0 ; col < nCols ; ++col )
              {
                m_segments[ row ][ col ].m_neighborSegments = 0;
                m_segments[ row ][ col ].m_neighborSegmentsSize = 0;
                m_segments[ row ][ col ].m_features = m_segmentsFeatures[ row ] + ( col * featuresNumber );
                m_segments[ row ][ col ].m_featuresSize = featuresNumber;
              }
            }
          }
          else
          {
            unsigned int col = 0;
            for( unsigned int row = 0 ; row < nRows ; ++row )
            {
              for( col = 0 ; col < nCols ; ++col )
              {
                m_segments[ row ][ col ].m_neighborSegments = 0;
                m_segments[ row ][ col ].m_neighborSegmentsSize = 0;
                m_segments[ row ][ col ].m_features = 0;
                m_segments[ row ][ col ].m_featuresSize = 0;
              }
            }
          }
        }
      }
      else
      {
        resetUseCounter();
      }
      
      return true;
    }    
    
    
    SegmenterRegionGrowingSegment* SegmenterRegionGrowingSegmentsPool::getNextSegment()
    {
      if( m_nextUnusedSegmentIdx < m_segsNumber )
      {
        m_getNextSegment_line = m_nextUnusedSegmentIdx / m_segments.getColumnsNumber();
        m_getNextSegment_col = m_nextUnusedSegmentIdx - ( m_getNextSegment_line *
          m_segments.getColumnsNumber() );
        
        ++m_nextUnusedSegmentIdx;
        
        assert( m_getNextSegment_line < m_segments.getLinesNumber() );
        assert( m_getNextSegment_col < m_segments.getColumnsNumber() );
          
        return m_segments[ m_getNextSegment_line ] + m_getNextSegment_col;
      }
      else
      {
        return 0;
      }
    }
    
    void SegmenterRegionGrowingSegmentsPool::clear()
    {
      unsigned int nRows = m_segments.getLinesNumber();
      unsigned int nCols = m_segments.getColumnsNumber();      
      
      unsigned int col = 0;
      for( unsigned int row = 0 ; row < nRows ; ++row )
      {
        for( col = 0 ; col < nCols ; ++col )
        {
          if( m_segments[ row ][ col ].m_neighborSegments )
          {
            free( m_segments[ row ][ col ].m_neighborSegments );
          }
        }
      }      
      
      m_segsNumber = 0;
      m_featuresNumber = 0;
      m_nextUnusedSegmentIdx = 0;
      m_segments.reset();
      m_segmentsFeatures.reset();
    }
    
    void SegmenterRegionGrowingSegmentsPool::resetUseCounter()
    {
      m_nextUnusedSegmentIdx = 0;
    }    
    
    SegmenterSegmentsBlock::SegmentIdDataType SegmenterRegionGrowingSegmentsPool::getSegmentsNumber() const
    {
      return m_segsNumber;
    }

  } // namespace rp
} // namespace te
