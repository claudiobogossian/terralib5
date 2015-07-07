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
    template< typename FeatureDataTypeT >
    class SegmenterRegionGrowingSegmentsPool : private boost::noncopyable 
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
      */            
      bool initialize( const SegmenterSegmentsBlock::SegmentIdDataType segsNumber,
                       const unsigned int featuresNumber );            

      /*!
        \brief Retrive a stored segment.
        \return A pointer to the next unused segment or null if all segments were used.
      */          
      SegmenterRegionGrowingSegment< FeatureDataTypeT >* getNextSegment();
      
      /*!
        \brief Clear all stored data.
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
      inline Matrix< SegmenterRegionGrowingSegment< FeatureDataTypeT > >& getSegsMatrix()
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

      Matrix< SegmenterRegionGrowingSegment< FeatureDataTypeT > > m_segments; //!< Internal stored segments.

      Matrix< FeatureDataTypeT > m_segmentsFeatures; //!< Internal stored segment features.

      // Variables used by getNextSegment

      unsigned int m_getNextSegment_line;
      unsigned int m_getNextSegment_col;
    };

    template< typename FeatureDataTypeT >
    SegmenterRegionGrowingSegmentsPool< FeatureDataTypeT >::SegmenterRegionGrowingSegmentsPool()
    {
      m_segsNumber = 0;
      m_featuresNumber = 0;
      m_nextUnusedSegmentIdx = 0;
    }

    template< typename FeatureDataTypeT >
    SegmenterRegionGrowingSegmentsPool< FeatureDataTypeT >::~SegmenterRegionGrowingSegmentsPool()
    {
      clear();
    }

    template< typename FeatureDataTypeT >
    bool SegmenterRegionGrowingSegmentsPool< FeatureDataTypeT >::initialize(
      const SegmenterSegmentsBlock::SegmentIdDataType segsNumber,
      const unsigned int featuresNumber )
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
            Matrix< SegmenterRegionGrowingSegment< FeatureDataTypeT > >::RAMMemPol ) == false )
          {
            return false;
          }
          
          m_segsNumber = segsNumber;
          
          if( featuresNumber )
          {
            if( m_segmentsFeatures.reset( nRows, nCols * featuresNumber, 
              Matrix< FeatureDataTypeT >::RAMMemPol ) == false )
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
    
    
    template< typename FeatureDataTypeT >
    SegmenterRegionGrowingSegment< FeatureDataTypeT >*
      SegmenterRegionGrowingSegmentsPool< FeatureDataTypeT >::getNextSegment()
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

    template< typename FeatureDataTypeT >
    void SegmenterRegionGrowingSegmentsPool< FeatureDataTypeT >::clear()
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
            m_segments[ row ][ col ].removeAllNeighborSegmentsPtrs();
          }
        }
      }      
      
      m_segsNumber = 0;
      m_featuresNumber = 0;
      m_nextUnusedSegmentIdx = 0;
      m_segments.reset();
      m_segmentsFeatures.reset();
    }
    
    template< typename FeatureDataTypeT >
    void SegmenterRegionGrowingSegmentsPool< FeatureDataTypeT >::resetUseCounter()
    {
      m_nextUnusedSegmentIdx = 0;
    }  
    
    template< typename FeatureDataTypeT >
    SegmenterSegmentsBlock::SegmentIdDataType
      SegmenterRegionGrowingSegmentsPool< FeatureDataTypeT >::getSegmentsNumber() const
    {
      return m_segsNumber;
    }
  } // namespace rp
} // namespace te

#endif // __TERRALIB_RP_INTERNAL_SEGMENTERREGIONGROWINGSEGMENTSPOOL_H
