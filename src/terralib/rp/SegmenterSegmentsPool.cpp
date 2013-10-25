#include "SegmenterSegmentsPool.h"

namespace te
{
  namespace rp
  { 
    
    SegmenterSegmentsPool::SegmenterSegmentsPool()
    {
    }

    SegmenterSegmentsPool::~SegmenterSegmentsPool()
    {
      clear();
    }
    
    void SegmenterSegmentsPool::store( SegmenterSegment* segPtr )
    {
      if( segPtr )
      {
        m_segments.push_back( segPtr );
      }
    }
    
    SegmenterSegment* SegmenterSegmentsPool::retrive()
    {
      if( m_segments.empty() )
      {
        return 0;
      }
      else
      {
        SegmenterSegment* auxSegPtr = m_segments.back();
        m_segments.pop_back();
        return auxSegPtr;
      }
    }
    
    void SegmenterSegmentsPool::clear()
    {
      const std::vector< SegmenterSegment* >::size_type size = 
        m_segments.size();
        
      for( std::vector< SegmenterSegment* >::size_type idx = 0 ;
        idx < size ; ++idx )
      {
        delete( m_segments[ idx ] );
      }
      
      m_segments.clear();
    }

  } // namespace rp
} // namespace te
