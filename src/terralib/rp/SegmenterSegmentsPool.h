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
  \file terralib/rp/SegmenterSegmentsPool.h
  \brief Segments pool.
 */

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERSEGMENTSPOOL_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERSEGMENTSPOOL_H

  #include "Config.h"
  #include "SegmenterSegment.h"
  
  #include <boost/noncopyable.hpp>
  
  #include <vector>
  
  namespace te
  {
    namespace rp
    {  
      /*!
          \class SegmenterSegmentsPool
          \brief Segments pool.
      */
      class TERPEXPORT SegmenterSegmentsPool : private boost::noncopyable 
      {
        public :
          
          SegmenterSegmentsPool();

          ~SegmenterSegmentsPool();
          
          /*!
            \brief Store a new segment.
            \param segPtr A pointer to the segment to be stored.
            \note The pool will acquire the segment's ownership.
          */          
          void store( SegmenterSegment* segPtr );
          
          /*!
            \brief Retrive a stored segment.
            \return A pointer to a segment or null if the pool is empty.
            \note The caller of this method must acquire the segment's ownership.
          */          
          SegmenterSegment* retrive();          
         
          /*!
            \brief Clear all stored segments.
          */          
          void clear();          

          /*!
            \brief Returns true if the pool is empty.
            \return Returns true if the pool is empty.
          */          
          inline bool empty() const { return m_segments.empty(); };
            
        protected :
          
          std::vector< SegmenterSegment* > m_segments; //!< Internal stored segments.

      };
    } // namespace rp
  } // namespace te

#endif
