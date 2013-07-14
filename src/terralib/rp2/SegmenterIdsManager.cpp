#include "SegmenterIdsManager.h"

namespace te
{
  namespace rp
  { 
    
    SegmenterIdsManager::SegmenterIdsManager()
    {
      m_segmentsIdsCounter = 0;
    }


    SegmenterIdsManager::~SegmenterIdsManager()
    {
    }
    
    SegmenterSegmentsBlock::SegmentIdDataType SegmenterIdsManager::getNewID()
    {
      SegmenterSegmentsBlock::SegmentIdDataType newId = 0;
      
      m_mutex.lock();
      
      if( m_freeIDs.empty() )
      {
        if( m_segmentsIdsCounter < std::numeric_limits<
          SegmenterSegmentsBlock::SegmentIdDataType>::max() )
        {
          ++m_segmentsIdsCounter;
          newId = m_segmentsIdsCounter;
        }
      }
      else
      {
        newId = m_freeIDs.front();
        m_freeIDs.pop_front();
      }
      
      m_mutex.unlock();
      
      return newId;
    }
    
    bool SegmenterIdsManager::getNewIDs( 
      const unsigned int& idsNumber,
      std::vector< SegmenterSegmentsBlock::SegmentIdDataType >& ids )
    {
      ids.resize( idsNumber );
      
      m_mutex.lock();
      
      for( unsigned int genIds = 0 ; genIds < idsNumber ; ++genIds )
      {
        if( m_freeIDs.empty() )
        {
          if( m_segmentsIdsCounter == std::numeric_limits<
            SegmenterSegmentsBlock::SegmentIdDataType>::max() )
          {
            m_mutex.unlock();
            return false;
          }
          else
          {
            ++m_segmentsIdsCounter;
            ids[ genIds ] = m_segmentsIdsCounter;
          }          
        }
        else
        {
          ids[ genIds ] = m_freeIDs.front();
          m_freeIDs.pop_front();
        }
      }
      
      m_mutex.unlock();
      
      return true;
    }    
    
    void SegmenterIdsManager::addFreeID( const SegmenterSegmentsBlock::SegmentIdDataType& id )
    {
      m_mutex.lock();
      
      m_freeIDs.push_back( id );
      
      m_mutex.unlock();
    }
    
    void SegmenterIdsManager::addFreeIDs( 
      const std::vector< SegmenterSegmentsBlock::SegmentIdDataType >& ids )
    {
      m_mutex.lock();
      
      const unsigned int idsSize = (unsigned int)ids.size();
      
      for( unsigned int idsIdx = 0 ; idsIdx < idsSize ; ++idsIdx )
      {
        m_freeIDs.push_back( ids[ idsIdx ] );
      }
      
      m_mutex.unlock();
    }    

    void SegmenterIdsManager::addFreeIDs( 
      const std::list< SegmenterSegmentsBlock::SegmentIdDataType >& ids )
    {
      m_mutex.lock();
      
      std::list< SegmenterSegmentsBlock::SegmentIdDataType >::const_iterator
        it = ids.begin();
      std::list< SegmenterSegmentsBlock::SegmentIdDataType >::const_iterator
        itEnd = ids.end();        
        
      while( it != itEnd )
      {
        m_freeIDs.push_back( *it );
        ++it;
      }
      
      m_mutex.unlock();
    }   

  } // namespace rp
} // namespace te
