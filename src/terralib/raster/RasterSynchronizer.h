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
  \file terralib/raster/RasterSynchronizer.h

  \brief An access synchronizer to be used in SynchronizedRaster raster instances.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_RASTEERSYNCHRONIZER_H
#define __TERRALIB_RASTER_INTERNAL_RASTEERSYNCHRONIZER_H

// TerraLib
#include "Raster.h"
#include "Config.h"
#include "../common/Enums.h"

// Boost
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

// STL
#include <vector>

namespace te
{
  namespace rst
  {
    class SynchronizedBandBlocksManager;
    class SynchronizedBand;
    class SynchronizedRaster;
    
    /*!
      \class RasterSynchronizer

      \brief An access synchronizer to be used in SynchronizedRaster raster instances.
      
      \ingroup rst
    */
    class TERASTEREXPORT RasterSynchronizer: public boost::noncopyable
    {
      friend class SynchronizedBandBlocksManager;
      friend class SynchronizedBand;
      friend class SynchronizedRaster;
      
      public:

        /*!
          \brief Constructor.

          \param raster The raster to synchronize.
          
          \param policy The access policy to use on the given input raster.
        */
        RasterSynchronizer( Raster& raster, const te::common::AccessPolicy policy );
        
        ~RasterSynchronizer();
        
      protected :
        
        /*! 
          \brief Blocks use counter type definition. 
          \note Indexed as [band][blockYIndex][blockXIndex]
        */
        typedef std::vector< std::vector< std::vector< unsigned int > > > BlocksUseCounterT;        
        
        te::common::AccessPolicy m_policy; //!< The access policy used on the given input raster.
        
        Raster& m_raster; //!< The input raster.
        
        boost::mutex m_mutex; //!< General sync mutex;
        
        boost::condition_variable_any m_condVar; //!< Block use request sync variable.
        
        BlocksUseCounterT m_blocksUseCounters; //!< blocks use counter.        
        
        /*!
          \brief Acquire a raster data block.
          \param bandIdx Block band index.
          \param blockXIndex Block X index.
          \param blockYIndex Block Y index.
          \param blkDataPtr A pointer to a pre-allocated area where the block data will be written.
          \return true if OK, false if the increment could not be done.
          \note The block data will be read from the internal raster and the block use counter will be incremented.
        */            
        bool acquireBlock( const unsigned int bandIdx,
          const unsigned int blockXIndex, const unsigned int blockYIndex,
          void* blkDataPtr );
          
        /*!
          \brief Release a raster data block.
          \param bandIdx Block band index.
          \param blockXIndex Block X index.
          \param blockYIndex Block Y index.
          \param blkDataPtr A pointer where the block data will be read.
          \note The block data will be writed to the internal raster and the block use counter will be decremented.
          \return true if OK, false if the increment could not be done.
        */            
        bool releaseBlock( const unsigned int bandIdx,
          const unsigned int blockXIndex, const unsigned int blockYIndex,
          void* blkDataPtr );          

       

    };

  } // end namespace rst
}   // end namespace te

#endif //__TERRALIB_RASTER_INTERNAL_RASTEERSYNCHRONIZER_H
