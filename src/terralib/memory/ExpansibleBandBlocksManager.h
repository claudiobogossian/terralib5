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
  \file terralib/memory/ExpansibleBandBlocksManager.h
 
  \brief RAM cached and tiled raster band blocks manager.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_EXPANSIBLEBANDBLOCKSMANAGER_H
#define __TERRALIB_MEMORY_INTERNAL_EXPANSIBLEBANDBLOCKSMANAGER_H

// TerraLib
#include "../raster/Raster.h"
#include "Config.h"

// STL
#include <cstdio>
#include <list>
#include <vector>
#include <string>

// Boost
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

namespace te
{
  namespace mem
  {
    /*!
      \class ExpansibleBandBlocksManager

      \brief RAM cached and tiled raster band blocks manager.
    */
    class TEMEMORYEXPORT ExpansibleBandBlocksManager : private boost::noncopyable
    {
      public:

        /*!
          \brief 3D Block index.
        */
        class TEMEMORYEXPORT BlockIndex3D
        {
          public :

            typedef unsigned int CoordDataType; //!< Coords data type.

            CoordDataType m_dim0Index; //!< Block Z (band) index.

            CoordDataType m_dim1Index; //!< Block Y index.

            CoordDataType m_dim2Index; //!< Block X index.

            BlockIndex3D() : m_dim0Index( 0 ), m_dim1Index( 0 ), m_dim2Index( 0 ) {};

            BlockIndex3D( const CoordDataType& dim0Index, const CoordDataType& dim1Index,
              const CoordDataType& dim2Index ) : m_dim0Index( dim0Index ), 
              m_dim1Index( dim1Index ), m_dim2Index( dim2Index ) {};

            ~BlockIndex3D() {};
        };

        ExpansibleBandBlocksManager();

        ~ExpansibleBandBlocksManager();

        /*!
          \brief Initialize this instance to an initial state.

          \param maxNumberRAMBlocks The maximum number of RAM blocks.

          \param numbersOfBlocksX The number of blocks along the X directon (for each band).

          \param numbersOfBlocksY The number of blocks along the Y directon (for each band).

          \param blocksSizesBytes The blocks sizes in bytes (for each band).

          \param maxDiskFilesSize The maximum temporary disk file size (if required).

          \return true if OK, false on errors.
        */
        bool initialize( const unsigned int maxNumberRAMBlocks,
                          const std::vector< unsigned int>& numbersOfBlocksX,
                          const std::vector< unsigned int>& numbersOfBlocksY,
                          const std::vector< unsigned int>& blocksSizesBytes,
                          const unsigned long int maxDiskFilesSize );

        /*!
          \brief Returns true if this instance is initialized.

          \return true if this instance is initialized.
        */
        bool isInitialized() const
        {
          return m_isInitialized;
        };

        /*!
          \brief Returns the number of blocks along the X directon for the required band.

          \param band The required band.

          \return Returns the number of blocks along the X directon for the required band.
        */
        unsigned int getNumberOfBlocksX( const unsigned int band ) const
        {
          assert( m_ramBlocksPointers.size() > band );
          assert( m_ramBlocksPointers[ band ].size() > 0 );
          return (unsigned int)m_ramBlocksPointers[ band ][ 0 ].size();
        };

        /*!
          \brief Returns the number of blocks along the Y directon for the required band.

          \param band The required band.

          \return Returns the number of blocks along the Y directon for the required band.
        */
        unsigned int getNumberOfBlocksY( 
          const unsigned int band ) const
        {
          assert( m_ramBlocksPointers.size() > band );
          return (unsigned int)m_ramBlocksPointers[ band ].size();
        };

        /*!
          \brief Returns the number of bands.

          \return Returns the number of bands.
        */
        unsigned int getNumberOfBands() const
        {
          return (unsigned int)m_ramBlocksPointers.size();
        };

        /*!
          \brief Returns the internal size( bytes ) used for all internal blocks.

          \return Returns the internal size( bytes ) used for all internal blocks
        */
        unsigned long int getBlockSizeBytes()
        {
          return m_maxBlockSizeBytes;
        };

        /*! \brief Free all allocated internal resources and go back to the initial state. */
        void free(); 

        /*!
          \brief Returns a pointer to the required data block.

          \param band The band index.
          \param x    The block-id in x (or x-offset).
          \param y    The block-id in y (or y-offset).

          \return Pointer to the required data block.
        */
        void* getBlockPointer(unsigned int band, unsigned int x, unsigned int y );

        /*! \brief The maximum number of cache blocks. */
        unsigned int getMaxNumberOfRAMBlocks() const
        {
          return m_maxNumberRAMBlocks;
        };

        /*!
          \brief New blocks will be added at the top of the raster.

          \param expansionSize The expansion size over the Y direction.

          \param band The band where the operation will be performed.

          \param addedBlocksCoords The added blocks coords.

          \return true if OK, false on errors.
        */
        bool addTopBlocks( const unsigned int& expansionSize,
                            const unsigned int& band,
                            std::vector< BlockIndex3D >& addedBlocksCoords );

        /*!
          \brief New blocks will be added at the bottom of the raster.

          \param expansionSize The expansion size over the Y direction.

          \param band The band where the operation will be performed.

          \param addedBlocksCoords The added blocks coords.

          \return true if OK, false on errors.
        */
        bool addBottomBlocks( const unsigned int& expansionSize,
                              const unsigned int& band,
                              std::vector< BlockIndex3D >& addedBlocksCoords );

        /*!
          \brief New blocks will be added at the left of the raster.

          \param expansionSize     The expansion size over the X direction.
          \param band              The band where the operation will be performed.
          \param addedBlocksCoords The added blocks coords.

          \return true if OK, false on errors.
        */
        bool addLeftBlocks( const unsigned int& expansionSize,
                            const unsigned int& band,
                            std::vector< BlockIndex3D >& addedBlocksCoords );

        /*!
          \brief New blocks will be added at the right of the raster.

          \param expansionSize     The expansion size over the X direction.
          \param band              The band where the operation will be performed.
          \param addedBlocksCoords The added blocks coords.

          \return true if OK, false on errors.
        */
        bool addRightBlocks( const unsigned int& expansionSize, const unsigned int& band,
                              std::vector< BlockIndex3D >& addedBlocksCoords );

        /*!
          \brief New bands will be added at the top of the raster (before the first band).

          \param expansionSize The number of bands to add.

          \param addedBlocksCoords The added blocks coords.

          \return true if OK, false on errors.
        */
        bool addTopBands( const unsigned int& expansionSize,
                          std::vector< BlockIndex3D >& addedBlocksCoords );

        /*!
          \brief New bands will be added at the bottom of the raster (after de the last band).

          \param expansionSize     The number of bands to add.
          \param addedBlocksCoords The added blocks coords.

          \return true if OK, false on errors.
        */
        bool addBottomBands( const unsigned int& expansionSize,
                              std::vector< BlockIndex3D >& addedBlocksCoords );

      protected :

        /*!
          \brief Disk block info.
        */
        class DiskBlockInfo
        {
          public :

            FILE* m_filePtr;
            unsigned long int m_fileOff;

            DiskBlockInfo() : m_filePtr( 0 ), m_fileOff( 0 ) {};

            ~DiskBlockInfo() {};
        };

        /*!
          \brief Open disk file handler.
        */
        class OpenDiskFileHandler : private boost::noncopyable
        {
          public :

            FILE* m_filePtr;
            
            std::string m_fullFileName;

            OpenDiskFileHandler();

            ~OpenDiskFileHandler();

        };

        typedef boost::shared_ptr< OpenDiskFileHandler > OpenDiskFileHandlerPtrT; //!< Open disk file pointer type.

        typedef std::list< OpenDiskFileHandlerPtrT > OpenDiskFilesHandlerT; //!< Open dis files handler type.


        /*!
          \brief Initialize this instance to an initial state.
        */  
        void initState();
        
        /*!
          \brief Allocate disk blocks.
          
          \param blocksNumber The number of blocks to allocate.
          
          \param diskBlocksInfos The info for the allocated blocks.
          
          \param diskFilesHandler The file handlers for all the allocated blocks.
          
          \return true if OK. false on errors.
        */
        bool allocateDiskBlocks( const unsigned int blocksNumber,
                                  std::vector< DiskBlockInfo >& diskBlocksInfos,
                                  OpenDiskFilesHandlerT& diskFilesHandler ) const;
          
        /*!
          \brief Allocate and activate disk blocks.
          
          \param blocksIndxes The blocks indexes inside the internal 3D indexing matrices.
          
          \return true if OK. false on errors.
        */
        bool allocateAndActivateDiskBlocks( const std::vector< BlockIndex3D >& blocksIndxes );
          
        /*!
          \brief Create a new disk file.
          
          \param size The file size.
          
          \param fileptr The file pointer.
          
          \param fullFileName The full created file name.
          
          \return true if OK. false on errors.
        */
        bool createNewDiskFile( unsigned long int size, FILE** fileptr,
          std::string& fullFileName ) const;         
        
        /*!
          \brief Shift coords given a fixed dimention 0 index.
          
          \param inputContainer The input coords container.
          
          \param dim0index The dimension 0 fixed index.
          
          \param dim1Shift The shift to be done over the dimension 1.
          
          \param dim2Shift The shift to be done over the dimension 2.
        */         
        template< typename ContainerType >
        void shiftDim03DCoords( 
          ContainerType& inputContainer,
          const unsigned int dim0index, const int dim1Shift, const int dim2Shift ) const
        {
          typename ContainerType::iterator it = inputContainer.begin();
          const typename ContainerType::iterator itE = inputContainer.end();
          BlockIndex3D newCoords;
          
          while( it != itE )
          {
            if( it->m_dim0Index == dim0index )
            {
              assert( ( dim1Shift < 0 ) ? ( ((int)it->m_dim1Index) > dim1Shift ) : true );
              it->m_dim1Index = (BlockIndex3D::CoordDataType)
                ( ((int)it->m_dim1Index) + dim1Shift );

              assert( ( dim2Shift < 0 ) ? ( ((int)it->m_dim2Index) > dim2Shift ) : true );
              it->m_dim2Index = (BlockIndex3D::CoordDataType)
                ( ((int)it->m_dim2Index) + dim2Shift );
            }
            
            ++it;
          }
        }
        
        /*!
          \brief Shift 3D coords.
          
          \param inputContainer The input coords container.
          
          \param dim0Shift The shift to be done over the dimension 0.
          
          \param dim1Shift The shift to be done over the dimension 1.
          
          \param dim2Shift The shift to be done over the dimension 2.
        */         
        template< typename ContainerType >
        void shift3DCoords( 
          ContainerType& inputContainer,
          const int& dim0Shift, const int& dim1Shift, const int& dim2Shift ) const
        {
          typename ContainerType::iterator it = inputContainer.begin();
          const typename ContainerType::iterator itE = inputContainer.end();
          BlockIndex3D newCoords;
          
          while( it != itE )
          {
            assert( ( dim0Shift < 0 ) ? ( ((int)it->m_dim0Index) > dim0Shift ) : true );
            it->m_dim0Index = (BlockIndex3D::CoordDataType)
              ( ((int)it->m_dim0Index) + dim0Shift );
            
            assert( ( dim1Shift < 0 ) ? ( ((int)it->m_dim1Index) > dim1Shift ) : true );
            it->m_dim1Index = (BlockIndex3D::CoordDataType)
              ( ((int)it->m_dim1Index) + dim1Shift );

            assert( ( dim2Shift < 0 ) ? ( ((int)it->m_dim2Index) > dim2Shift ) : true );
            it->m_dim2Index = (BlockIndex3D::CoordDataType)
              ( ((int)it->m_dim2Index) + dim2Shift );

            ++it;
          }
        }

      protected:

        typedef unsigned char BlockElementT; //!< Block element type.

        typedef BlockElementT* BlockelementPtrT; //!< Block element pointer type.

        typedef boost::shared_array< BlockElementT > RAMBlockHandlerT; //!< RAM Block handler type;  

        typedef std::vector< std::vector< std::vector< BlockelementPtrT > > > RAMBlocksPointersContainerT; //!< RAM blocks pointers container type.

        typedef std::list< RAMBlockHandlerT > RAMBlocksHandlerT; //!< Blocks handler type;

        typedef std::vector< std::vector< std::vector< DiskBlockInfo > > > ActiveDiskBlocksInfoT; //!< Active disk blocks info type;

        typedef std::list< DiskBlockInfo > InactiveDiskBlocksInfoT; //!< Inactive disk blocks info type.

        typedef std::vector< BlockIndex3D > SwapFifoT; //!< Swap fifo type.

        bool m_isInitialized; //!< Is this instance initialized ?

        unsigned int m_maxNumberRAMBlocks; //!< The maximum number of RAM blocks;

        unsigned long int m_maxDiskFilesSize; //!< The maximum temporary disk file size (bytes).        

        unsigned long int m_maxBlockSizeBytes; //!< The maximum global used block size in bytes.

        unsigned char* m_currSwapBlockPtr; //!< A pointer to the current block where disk data swap will be done.

        SwapFifoT::size_type m_nextFIFOPositionOverSwapFifo; //!< The next position where a block swap will occur over m_swapFifo;

        RAMBlocksHandlerT m_activeRAMBlocksHandler; //!< The active RAM blocks handler.

        RAMBlocksPointersContainerT m_ramBlocksPointers; //!< 3D Matrix of active RAM blocks pointers indexed in the form [band][blockYIndex][blockXIndex].

        SwapFifoT m_swapFifo; //!< Disk swapping FIFO.

        ActiveDiskBlocksInfoT m_activeDiskBlocksInfo; //!< 3D Matrix of active disk block info indexed as [band][blockYIndex][blockXIndex].

        OpenDiskFilesHandlerT m_diskFilesHandler; //!< The disk files handler;

        RAMBlockHandlerT m_swapBlockHandler; //!< An extra block for disk swap purposes.

      private:

        // Variables used by the method getBlockPointer
        unsigned char* m_getBlockPointer_returnValue;
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_EXPANSIBLEBANDBLOCKSMANAGER_H
