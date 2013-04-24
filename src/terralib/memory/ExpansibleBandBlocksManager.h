/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
#include "Config.h"
#include "../raster/Raster.h"

#include <boost/shared_ptr.hpp>
#include <boost/multi_array.hpp>
#include <boost/noncopyable.hpp>

#include <vector>

#include <cstdio>

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
        inline bool isInitialized() const
        {
          return m_isInitialized;
        };

        /*!
          \note Free all allocated internal resources and go back to the initial state.
        */
        void free(); 
        
        /*!
          \brief Returns a pointer to the required data block.

          \param band The band index.
          
          \param x The block-id in x (or x-offset).
          
          \param y The block-id in y (or y-offset).

          \return Pointer to the required data block.
        */
        void* getBlockPointer(unsigned int band, unsigned int x, unsigned int y );
        
        /*! \brief The maximum number of cache blocks. */
        inline unsigned int getMaxNumberOfRAMBlocks() const
        {
          return m_maxNumberRAMBlocks;
        };
        
      protected :
        
        
        /*!
        \brief Block index.
        */         
        class BlockIndex
        {
          public :
            
            unsigned int m_bIndex; //!< Block band index.
            
            unsigned int m_xIndex; //!< Block X index.
            
            unsigned int m_yIndex; //!< Block Y index.
            
            BlockIndex() : m_bIndex( 0 ), m_xIndex( 0 ), m_yIndex( 0 ) {};
            
            BlockIndex( const unsigned int& bIndex, const unsigned int& xIndex,
              const unsigned int& yIndex ) : m_bIndex( bIndex ), m_xIndex( xIndex ), 
              m_yIndex( yIndex ) {};
            
            ~BlockIndex() {};
        };        
        
        /*!
        \brief RAM block handler.
        */         
        class RAMBlockHandler : private boost::noncopyable
        {
          public :
            
            unsigned char* m_dataPtr; //!< Block data pointer.
            
            RAMBlockHandler() : m_dataPtr( 0 ) {};
            
            ~RAMBlockHandler()
            {
              if( m_dataPtr ) delete[] ( m_dataPtr );
            };
        };        
      
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
            
            OpenDiskFileHandler() : m_filePtr( 0 ) {};          
            ~OpenDiskFileHandler() 
            {
              if( m_filePtr ) 
              {
                fclose( m_filePtr );
              }
            };
        };   
        
        bool m_isInitialized; //!< Is this instance initialized ?
        
        unsigned int m_maxNumberRAMBlocks; //!< The maximum number of RAM blocks;
        
        unsigned long int m_maxDiskFilesSize; //!< The maximum temporary disk file size (bytes).        
        
        unsigned long int m_maxBlockSizeBytes; //!< The maximum global used block size in bytes.
        
        std::vector< boost::shared_ptr< RAMBlockHandler > >::size_type m_nextFIFOPositionOverSwapFifo; //!< The next position where a block swap will occur over m_swapFifo;
        
        unsigned char* m_currSwapBlockPtr; //!< A pointer to the current block where disk data swap will be done.
        
        std::vector< unsigned int> m_numbersOfBlocksX; //!< The number of blocks along the X directon (for each band).
        
        std::vector< unsigned int> m_numbersOfBlocksY; //!< The number of blocks along the Y directon (for each band).
        
        std::vector< unsigned int> m_blocksSizesBytes; //!<  The blocks sizes in bytes (for each band).
        
        boost::multi_array< unsigned char*, 3 > m_blocksPointers; //!< 3D Matrix of block pointers indexed as [band][blockYIndex][blockXIndex].
        
        std::vector< BlockIndex > m_swapFifo; //!< Disk swapping FIFO.
        
        std::vector< boost::shared_ptr< RAMBlockHandler > > m_activeRamBlocksHandler; //!< The active RAM blocks handler;
        
        std::vector< boost::shared_ptr< RAMBlockHandler > > m_inactiveRamBlocksHandler; //!< The inative RAM blocks handler;
        
        boost::multi_array< DiskBlockInfo, 3 > m_diskBlocksInfo; //!< 3D Matrix of disk block info indexed as [band][blockYIndex][blockXIndex].
        
        std::vector< boost::shared_ptr< OpenDiskFileHandler > > m_diskFilesHandler; //!< The disk files handler;
        
        RAMBlockHandler m_swapBlockHandler; //!< An extra block for disk swap purposes.
        
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
          std::vector< boost::shared_ptr< OpenDiskFileHandler > >& diskFilesHandler );
          
        /*!
          \brief Create a new disk file.
          
          \param size The file size.
          
          \param fileptr The file pointer.
          
          \return true if OK. false on errors.
        */
        bool createNewDiskFile( unsigned long int size, FILE** fileptr ) const;         
        
      private :
        
        // Variables used by the method getBlockPointer
        
        unsigned char* getBlockPointer_returnValue;
        unsigned int getBlockPointer_outBlockB;
        unsigned int getBlockPointer_outBlockX;
        unsigned int getBlockPointer_outBlockY;
        

    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_EXPANSIBLEBANDBLOCKSMANAGER_H

