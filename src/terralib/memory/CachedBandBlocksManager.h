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
  \file terralib/memory/CachedBandBlocksManager.h
 
  \brief RAM cached and tiled raster band blocks manager.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_CACHEDBANDBLOCKSMANAGER_H
#define __TERRALIB_MEMORY_INTERNAL_CACHEDBANDBLOCKSMANAGER_H

// TerraLib
#include "../raster/Raster.h"
#include "Config.h"

// STL
#include <memory>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_array.hpp>

namespace te
{
  namespace mem
  {
    /*!
      \class CachedBandBlocksManager

      \brief RAM cached and tiled raster band blocks manager.
    */
    class TEMEMORYEXPORT CachedBandBlocksManager : public boost::noncopyable
    {
      public:

        CachedBandBlocksManager();

        ~CachedBandBlocksManager();

        /*!
          \brief Initialize this instance to an initial state.

          \param externalRaster The external raster where the data will be read/written.

          \param maxMemPercentUsed The maximum free memory percentual to use valid range: [1:100].

          \param dataPrefetchThreshold The read-ahead data prefetch threshold (0-will disable prefetch, 1-data always prefetched, higher values will do prefetch when necessary).

          \return true if OK, false on errors.
        */
        bool initialize( const te::rst::Raster& externalRaster,
                          const unsigned char maxMemPercentUsed,
                          const unsigned int dataPrefetchThreshold );

        /*!
          \brief Initialize this instance to an initial state.

          \param externalRaster The external raster where the data will be read/written.

          \param maxNumberOfCacheBlocks The maximum number of cache blocks.

          \param dataPrefetchThreshold The read-ahead data prefetch threshold (0-will disable prefetch, 1-data always prefetched, higher values will do prefetch when necessary).

          \return true if OK, false on errors.
        */
        bool initialize( const unsigned int maxNumberOfCacheBlocks, 
                          const te::rst::Raster& externalRaster, 
                          const unsigned int dataPrefetchThreshold );

        /*!
          \brief Returns true if this instance is initialized.

          \return true if this instance is initialized.
        */
        bool isInitialized() const
        {
          return m_rasterPtr ? true : false;
        };

        /*!
          \note Free all allocated internal resources and go back to the initial state.
        */
        void free(); 

        /*!
          \brief Returns a pointer to the required data block.

          \param band The band index.
          \param x    The block-id in x (or x-offset).
          \param y    The block-id in y (or y-offset).

          \return Pointer to the required data block.
        */
        void* getBlockPointer(unsigned int band, unsigned int x, unsigned int y );


        /*! \brief Returns the associated raster. */
        te::rst::Raster* getRaster() const
        {
          return m_rasterPtr;
        };

        /*! \brief The maximum number of cache blocks. */
        unsigned int getMaxNumberOfCacheBlocks() const
        {
          return m_maxNumberOfCacheBlocks;
        };
        
        /*! \brief The read-ahead data prefetch threshold. */
        unsigned int getDataPrefetchThreshold() const
        {
          return m_dataPrefetchThreshold;
        };

      protected :

        /*!
          \class BlockIndex

          \brief Internal blocks indexes.
        */
        class BlockIndex
        {
          public :

            unsigned int m_b; //!< Block band index
            unsigned int m_y; //!< Block index over the Y axis.
            unsigned int m_x; //!< Block index over the X axis.

            BlockIndex()
              : m_b( 0 ), m_y( 0 ), m_x( 0 )
            {
            }

            ~BlockIndex()
            {
            }
        };

        /*!
          \class ThreadParameters

          \brief Internal read/write thread execution parameters.
        */
        class ThreadParameters
        {
          public :
            
            enum TaskType
            {
              InvalidTaskT = 0,
              ReadTaskT = 1,      //!< m_exchangeBlockPtr must point to a valid exchange block (it will be keept inside the thread for further use), m_blockPtr must be zero and will be updated to point to the read block.
              WriteTaskT = 2,     //!< m_blockPtr must point to the block to be written, m_exchangeBlockPtr must be zero.
              SuicideTastT = 3    //!< The thread will finish the main loop and exit.
            };

            te::rst::Raster* m_rasterPtr; //!< External raster pointer.

            unsigned int m_dataPrefetchThreshold; //!< The user defined read-ahead threshold.

            bool m_taskFinished; //!< true when the thread has finished the required task.

            TaskType m_task; //!< The required task to be performed (read/write/exit).

            unsigned char* m_blockPtr; //!< Input block pointer.

            unsigned char* m_exchangeBlockPtr; //!< Exchange block pointer.

            unsigned int m_blockB; //!< Raster block band index.

            unsigned int m_blockX; //!< Raster block X index

            unsigned int m_blockY; //!< Raster block Y index

            boost::mutex m_doTaskMutex; //!< Used when wakenning the thread to perform some task.

            boost::mutex m_taskFinishedMutex; //!< used by the thread to inform a task finishment.

            boost::condition_variable m_doTaskCondVar; //!< Used by the thread when awakenning to perform some task.

            boost::condition_variable m_taskFinishedCondVar; //!< Used to wait for the required task finishment.

            boost::scoped_array< unsigned char > m_threadDataBlockHandler; //!< A extra block used in exchange when a read-ahead task is performed.

            ThreadParameters()
              : m_rasterPtr( 0 ),
                m_dataPrefetchThreshold( 0 ),
                m_taskFinished( false ),
                m_task( InvalidTaskT ), 
                m_blockPtr( 0 ),
                m_exchangeBlockPtr( 0 ),
                m_blockB( 0 ),
                m_blockX( 0 ),
                m_blockY( 0 )
            {
            };

            ~ThreadParameters() {};
        };

        te::rst::Raster* m_rasterPtr; //!< External raster pointer.

        unsigned int m_dataPrefetchThreshold; //!< The read-ahead data prefetch threshold (0-will disable prefetch, 1-data always prefetched, higher values will do prefetch when necessary).

        unsigned int m_globalBlocksNumberX; //!< The maximum number of blocks (X axis) for all bands.

        unsigned int m_globalBlocksNumberY; //!< The maximum number of blocks (Y axis) for all bands.

        unsigned int m_globalBlockSizeBytes; //!< The maximum block size for all bands.

        unsigned int m_maxNumberOfCacheBlocks; //!< The maximum number of cache blocks.

        unsigned int m_blocksFifoNextSwapBlockIndex; //!< The next block swapp index over m_blocksFifo.

// variables used by internal methods
        unsigned char* m_getBlockPointer_BlkPtr;

        std::vector< std::vector< std::vector< unsigned char* > > > m_blocksPointers; //!< 3D Matrix of block pointers indexed as [band][blockYIndex][blockXIndex].

        std::vector< unsigned char* > m_blocksHandler; //!< Cache blocks handler.

        std::vector< BlockIndex > m_blocksFifo; //!< blocks swap FIFO.

        ThreadParameters m_threadParams; //!< The internal thread execution parameters.

        std::auto_ptr< boost::thread > m_threadHandler; //!< The internal thread handler.

        /*! 
          \brief Thread entry.

          \param paramsPtr A pointer to the thread parameters.
        */
        static void threadEntry(ThreadParameters* paramsPtr);

      private :

        /*! \brief Initialize this instance to an initial state. */
        void initState();
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_CACHEDBANDBLOCKSMANAGER_H
