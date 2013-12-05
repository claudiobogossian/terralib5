/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/raster/SynchronizedBandBlocksManager.h
 
  \brief Synchronized raster raster band blocks manager.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_SYNCHRONIZEDBANDBLOCKSMANAGER_H
#define __TERRALIB_RASTER_INTERNAL_SYNCHRONIZEDBANDBLOCKSMANAGER_H

// TerraLib
#include "RasterSynchronizer.h"
#include "Config.h"
#include "../raster/Raster.h"

// STL
#include <memory>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace rst
  {
    /*!
      \class SynchronizedBandBlocksManager

      \brief Synchronized raster raster band blocks manager.
    */
    class TERASTEREXPORT SynchronizedBandBlocksManager : public boost::noncopyable
    {
      public:

        SynchronizedBandBlocksManager();

        ~SynchronizedBandBlocksManager();

        /*!
          \brief Initialize this instance to an initial state.

          \param sync The synchronized used by this instance.

          \param maxMemPercentUsed The maximum free memory percentual to use valid range: [1:100].
          
          \return true if OK, false on errors.
        */
        bool initialize( RasterSynchronizer& sync,
                         const unsigned char maxMemPercentUsed );

        /*!
          \brief Initialize this instance to an initial state.

          \param sync The synchronized used by this instance.

          \param maxNumberOfCacheBlocks The maximum number of cache blocks.

          \return true if OK, false on errors.
        */
        bool initialize( const unsigned int maxNumberOfCacheBlocks, 
                         RasterSynchronizer& sync );

        /*!
          \brief Returns true if this instance is initialized.

          \return true if this instance is initialized.
        */
        bool isInitialized() const
        {
          return m_syncPtr ? true : false;
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
        te::rst::Raster* getRaster() const;

        /*! \brief The maximum number of cache blocks. */
        unsigned int getMaxNumberOfCacheBlocks() const
        {
          return m_maxNumberOfCacheBlocks;
        };
        
        /*! \brief Return a pointer to the assotiated synchronizer instance or NULL if there is none. */
        inline RasterSynchronizer* getSynchronizer() const
        {
          return m_syncPtr;
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

        RasterSynchronizer* m_syncPtr; //!< A pointer to the synchronizer used by this instance, of null if not initialized.

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

      private :

        /*! \brief Initialize this instance to an initial state. */
        void initState();
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_SYNCHRONIZEDBANDBLOCKSMANAGER_H
