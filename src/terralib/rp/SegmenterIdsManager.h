/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/rp/SegmenterIdsManager.h
  \brief Segments IDs manager.
 */

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTERIDSMANAGER_H
#define __TERRALIB_RP_INTERNAL_SEGMENTERIDSMANAGER_H

  #include "SegmenterSegmentsBlock.h"
  #include "Config.h"
  
  #include <boost/thread.hpp>
  
  #include <list>
  #include <vector>
  
  namespace te
  {
    namespace rp
    {  
      /*!
          \class SegmenterIdsManager
          \brief Segmenter segments IDs manager.
      */
      class TERPEXPORT SegmenterIdsManager
      {
        public :
          
          SegmenterIdsManager();

          ~SegmenterIdsManager();
          
          /*!
            \brief Returns a new segment unique ID.
            \return id A positive new segment ID or zero if an error occurred.
            \note This method is thread safe.
          */          
          SegmenterSegmentsBlock::SegmentIdDataType getNewID();
          
          /*!
            \brief Returns new segment unique IDs.
            \param idsNumber The number of unique IDs to return.
            \param id The segment IDs.
            \return true if OK, false on errors.
            \note This method is thread safe.
          */          
          bool getNewIDs( 
            const unsigned int& idsNumber,
            std::vector< SegmenterSegmentsBlock::SegmentIdDataType >& ids );

          /*!
            \brief Stores a free unique ID for later use.
            \param id The segment ID.
            \note This method is thread safe.
          */          
          void addFreeID( const SegmenterSegmentsBlock::SegmentIdDataType& id );
          
          /*!
            \brief Stores free unique IDs for later use.
            \param ids The segment IDs.
            \note This method is thread safe.
          */          
          void addFreeIDs( 
            const std::vector< SegmenterSegmentsBlock::SegmentIdDataType >& ids );          
            
          /*!
            \brief Stores free unique IDs for later use.
            \param ids The segment IDs.
            \note This method is thread safe.
          */          
          void addFreeIDs( 
            const std::list< SegmenterSegmentsBlock::SegmentIdDataType >& ids );          
            
        protected :
          
          /*! Internal free segments IDs counter - default 0) */
          volatile SegmenterSegmentsBlock::SegmentIdDataType m_segmentsIdsCounter;
          
          /*! Internal free segments IDs counter mutex */
          boost::mutex m_mutex;
          
          /*! Internal free segments IDs list */
          std::list< SegmenterSegmentsBlock::SegmentIdDataType > m_freeIDs;
      };
    } // namespace rp
  } // namespace te

#endif
