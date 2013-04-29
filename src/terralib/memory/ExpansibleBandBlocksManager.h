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
#include <boost/shared_array.hpp>
#include <boost/multi_array.hpp>
#include <boost/noncopyable.hpp>

#include <vector>
#include <list>

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
          \brief Returns the number of blocks along the X directon for the required band.
          
          \param band The required band.
          
          \return Returns the number of blocks along the X directon for the required band.
        */         
        inline unsigned int getNumberOfBlocksX( 
          const unsigned int band ) const
        {
          assert( band < m_numbersOfBlocksX.size() );
          return m_numbersOfBlocksX[ band ];
        };
        
        /*!
          \brief Returns the number of blocks along the Y directon for the required band.
          
          \param band The required band.
          
          \return Returns the number of blocks along the Y directon for the required band.
        */         
        inline unsigned int getNumberOfBlocksY( 
          const unsigned int band ) const
        {
          assert( band < m_numbersOfBlocksY.size() );
          return m_numbersOfBlocksY[ band ];
        };
        
        /*!
          \brief Returns the blocks sizes in bytes for the required band.
          
          \param band The required band.
          
          \return Returns the blocks sizes in bytes for the required band.
        */         
        inline unsigned int getBlocksSizesBytes( 
          const unsigned int band ) const
        {
          assert( band < m_blocksSizesBytes.size() );
          return m_blocksSizesBytes[ band ];
        };      
        
        /*!
          \brief Returns the number of bands.
          \return Returns the number of bands.
        */         
        inline unsigned int getNumberOfBands() const
        {
          return (unsigned int)m_blocksSizesBytes.size();
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
        
        /*!
          \brief New blocks will be added at the top of the raster.
          
          \param numberOfBlocks The number of blocks to add.
          
          \param band The band where the operation will be performed.
          
          \return true if OK, false on errors.
        */        
        bool addTopBlocks( const unsigned int& numberOfBlocks, const unsigned int& band );
        
        /*!
          \brief New blocks will be added at the bottom of the raster.
          
          \param numberOfBlocks The number of blocks to add.
          
          \param band The band where the operation will be performed.
          
          \return true if OK, false on errors.
        */         
        bool addBottomBlocks( const unsigned int& numberOfBlocks, const unsigned int& band );
        
        /*!
          \brief New blocks will be added at the left of the raster.
          
          \param number The number of blocks to add.
          
          \param band The band where the operation will be performed.
          
          \return true if OK, false on errors.
        */        
        bool addLeftBlocks( const unsigned int& numberOfBlocks, const unsigned int& band );        
        
        /*!
          \brief New blocks will be added at the right of the raster.
          
          \param number The number of blocks to add.
          
          \param band The band where the operation will be performed.
          
          \return true if OK, false on errors.
        */        
        bool addRightBlocks( const unsigned int& numberOfBlocks, const unsigned int& band );        
        
        /*!
          \brief New bands will be added at the top of the raster (before the first band).
          
          \param numberOfBands The number of bands to add.
          
          \return true if OK, false on errors.
        */        
        bool addTopBands( const unsigned int& numberOfBands );
        
        /*!
          \brief New bands will be added at the bottom of the raster (after de the last band).
          
          \param numberOfBands The number of bands to add.
          
          \return true if OK, false on errors.
        */         
        bool addBottomBands( const unsigned int& numberOfBands );        
        
        /*!
          \brief Blocks will be removed from the top of the raster.
          
          \param numberOfBlocks The number of blocks to remove.
          
          \param band The band where the operation will be performed.
          
          \return true if OK, false on errors.
        */        
        bool removeTopBlocks( const unsigned int& numberOfBlocks, const unsigned int& band );
        
        /*!
          \brief Blocks will be removed from the bottom of the raster.
          
          \param numberOfBlocks The number of blocks to remove.
          
          \param band The band where the operation will be performed.
          
          \return true if OK, false on errors.
        */         
        bool removeBottomBlocks( const unsigned int& numberOfBlocks, const unsigned int& band );
        
        /*!
          \brief Blocks will be removed from the left of the raster.
          
          \param numberOfBlocks The number of columns to remove.
          
          \param band The band where the operation will be performed.
          
          \return true if OK, false on errors.
        */        
        bool removeLeftBlocks( const unsigned int& numberOfBlocks, const unsigned int& band );        
        
        /*!
          \brief Blocks will be removed from the right of the raster.
          
          \param numberOfBlocks The number of columns to add.
          
          \param band The band where the operation will be performed.
          
          \return true if OK, false on errors.
        */        
        bool removeRightBlocks( const unsigned int& numberOfBlocks, const unsigned int& band );     
        
        /*!
          \brief Bands will be removed from the top of the raster.
          
          \param numberOfBands The number of bands to remove.
          
          \return true if OK, false on errors.
        */        
        bool removeTopBands( const unsigned int& numberOfBands );
        
        /*!
          \brief Bands will be removed from the bottom of the raster.
          
          \param numberOfBands The number of bands to remove.
          
          \return true if OK, false on errors.
        */         
        bool removeBottomBands( const unsigned int& numberOfBands );                
        
      protected :
        
        /*!
        \brief 3D Block index.
        */         
        class BlockIndex3D
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
        
        typedef unsigned char BlockElementT; //!< Block element type.
        
        typedef BlockElementT* BlockelementPtrT; //!< Block element pointer type.
        
        typedef boost::shared_array< BlockElementT > RAMBlockHandlerT; //!< RAM Block handler type;  
        
        typedef boost::multi_array< BlockelementPtrT, 3 > RAMBlocksPointersContainerT; //!< RAM blocks pointers container type.
        
        typedef std::list< RAMBlockHandlerT > RAMBlocksHandlerT; //!< Blocks handler type;
        
        typedef boost::multi_array< DiskBlockInfo, 3 > ActiveDiskBlocksInfoT; //!< Active disk blocks info type;
        
        typedef std::list< DiskBlockInfo > InactiveDiskBlocksInfoT; //!< Inactive disk blocks info type.
        
        typedef boost::shared_ptr< OpenDiskFileHandler > OpenDiskFileHandlerPtrT; //!< Open disk file pointer type.
        
        typedef std::list< OpenDiskFileHandlerPtrT > OpenDiskFilesHandlerT; //!< Open dis files handler type.
        
        typedef std::vector< BlockIndex3D > SwapFifoT; //!< Swap fifo type.
        
        bool m_isInitialized; //!< Is this instance initialized ?
        
        unsigned int m_maxNumberRAMBlocks; //!< The maximum number of RAM blocks;
        
        unsigned long int m_maxDiskFilesSize; //!< The maximum temporary disk file size (bytes).        
        
        unsigned long int m_maxBlockSizeBytes; //!< The maximum global used block size in bytes.
        
        unsigned char* m_currSwapBlockPtr; //!< A pointer to the current block where disk data swap will be done.
        
        SwapFifoT::size_type m_nextFIFOPositionOverSwapFifo; //!< The next position where a block swap will occur over m_swapFifo;
        
        std::vector< unsigned int> m_numbersOfBlocksX; //!< The number of blocks along the X directon (for each band).
        
        std::vector< unsigned int> m_numbersOfBlocksY; //!< The number of blocks along the Y directon (for each band).
        
        std::vector< unsigned int> m_blocksSizesBytes; //!<  The blocks sizes in bytes (for each band).
        
        RAMBlocksHandlerT m_activeRAMBlocksHandler; //!< The active RAM blocks handler.
        
        RAMBlocksHandlerT m_inactiveRAMBlocksHandler; //!< The inative RAM blocks handler;
        
        RAMBlocksPointersContainerT m_ramBlocksPointers; //!< 3D Matrix of active RAM blocks pointers indexed in the form [band][blockYIndex][blockXIndex].
        
        SwapFifoT m_swapFifo; //!< Disk swapping FIFO.
        
        ActiveDiskBlocksInfoT m_activeDiskBlocksInfo; //!< 3D Matrix of active disk block info indexed as [band][blockYIndex][blockXIndex].
        
        InactiveDiskBlocksInfoT m_inactiveDiskBlocksInfo; //!< Inactive disk blocks info.
        
        OpenDiskFilesHandlerT m_diskFilesHandler; //!< The disk files handler;
        
        RAMBlockHandlerT m_swapBlockHandler; //!< An extra block for disk swap purposes.
        
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
        bool allocateAndActivateDiskBlocks( 
          const std::vector< BlockIndex3D >& blocksIndxes );          
          
        /*!
          \brief Create a new disk file.
          
          \param size The file size.
          
          \param fileptr The file pointer.
          
          \return true if OK. false on errors.
        */
        bool createNewDiskFile( unsigned long int size, FILE** fileptr ) const;         
        
        /*!
          \brief Performs a shift over a 3D array expanding its size.
          
          \param dim0Shift The shift to perform over the dimension 0.
          
          \param dim1Shift The shift to perform over the dimension 1.
          
          \param dim2Shift The shift to perform over the dimension 2.
          
          \param dummyFillObj A dummy fill object for those positions where no valid elements will be present.
          
          \param array The given array where the operation will be performed.
          
          \param newDummyElementsIndxes The indexes of the new elements filled with the dummy object.
        */        
        template< typename ArrayElementType >
        void shiftExpansion( const int& dim0Shift, const int& dim1Shift,
          const int& dim2Shift, const ArrayElementType& dummyFillObj,
          typename boost::multi_array< ArrayElementType, 3 >& array,
          std::vector< BlockIndex3D >& newDummyElementsIndxes ) const
        {
          assert( array.dimensionality == 3 );
          
          typedef typename boost::multi_array< ArrayElementType, 3 >::size_type
            ArrayIdxT;
            
          newDummyElementsIndxes.clear();
          
          if( ( dim0Shift != 0 ) || ( dim1Shift != 0 ) || ( dim2Shift != 0 ) )
          {
            const int oldDim0Size = (int)array.shape()[ 0 ];
            const int oldDim1Size = (int)array.shape()[ 1 ];
            const int oldDim2Size = (int)array.shape()[ 2 ];
            
            const int newDim0Size = oldDim0Size + ((int)std::abs( dim0Shift ));
            const int newDim1Size = oldDim1Size + ((int)std::abs( dim1Shift ));
            const int newDim2Size = oldDim2Size + ((int)std::abs( dim2Shift ));
            
            boost::multi_array< ArrayElementType, 3 > tempArray;            
            typename boost::multi_array< ArrayElementType, 3 >::extent_gen extent;
            tempArray.resize( extent[ ((ArrayIdxT)newDim0Size) ][ 
              ((ArrayIdxT)newDim1Size) ][ ((ArrayIdxT)newDim2Size) ] );
              
            int newIdx0 = 0;
            int newIdx1 = 0;
            int newIdx2 = 0;
            int oldIdx0 = 0;
            int oldIdx1 = 0;
            int oldIdx2 = 0;
            
            for( newIdx0 = 0; newIdx0 < newDim0Size ; ++newIdx0 )
            {
              oldIdx0 = newIdx0 - dim0Shift;
              
              for( newIdx1 = 0 ; newIdx1 < newDim1Size ; ++newIdx1 )
              {
                oldIdx1 = newIdx1 - dim1Shift;
                
                for( newIdx2 = 0 ; newIdx2 < newDim2Size ; ++newIdx2 )
                {
                  oldIdx2 = newIdx2 - dim2Shift;
                  
                  if( ( oldIdx0 >= 0 ) && ( oldIdx0 < oldDim0Size ) &&
                    ( oldIdx1 >= 0 ) && ( oldIdx1 < oldDim1Size ) &&
                    ( oldIdx2 >= 0 ) && ( oldIdx2 < oldDim2Size ) )
                  {
                    tempArray[ newIdx0 ][ newIdx1 ][ newIdx2 ] =
                      array[ oldIdx0 ][ oldIdx1 ][ oldIdx2 ];
                  }
                  else
                  {
                    tempArray[ newIdx0 ][ newIdx1 ][ newIdx2 ] =
                      dummyFillObj;
                      
                    newDummyElementsIndxes.push_back( BlockIndex3D( 
                      (unsigned int)newIdx0, (unsigned int)newIdx1,
                      (unsigned int)newIdx2 ) );
                  }
                }
              }
            }
            
            array.resize( extent[ ((ArrayIdxT)newDim0Size) ][ 
              ((ArrayIdxT)newDim1Size) ][ ((ArrayIdxT)newDim2Size) ] );              
            array = tempArray;
          }
        };
          
        /*!
          \brief Performs a shift over pre-defined dimention 0 index inside a 3D array expanding its size.
          
          \param dim0Index The fixed dimension 0 index.
          
          \param dim1Shift The shift to perform over the dimension 1.
          
          \param dim2Shift The shift to perform over the dimension 2.
          
          \param dummyFillObj A dummy fill object for those positions where no valid elements will be present.
          
          \param array The given array where the operation will be performed.
          
          \param newDummyElementsIndxes The indexes of the new elements filled with the dummy object from the dimension with index "dim0Index".
        */         
        template< typename ArrayElementType >
        void fixedDim0ShiftExpansion( const int& dim0Index, const int& dim1Shift,
          const int& dim2Shift, const ArrayElementType& dummyFillObj,
          typename boost::multi_array< ArrayElementType, 3 >& array,
          std::vector< BlockIndex3D >& newDummyElementsIndxes ) const
        {
          assert( array.dimensionality == 3 );
          assert( dim0Index < (int)array.shape()[ 0 ] );
          
          typedef typename boost::multi_array< ArrayElementType, 3 >::size_type
            ArrayIdxT;
            
          newDummyElementsIndxes.clear();
          
          if( ( dim1Shift != 0 ) || ( dim2Shift != 0 ) )
          {
            const int oldDim0Size = (int)array.shape()[ 0 ];
            const int oldDim1Size = (int)array.shape()[ 1 ];
            const int oldDim2Size = (int)array.shape()[ 2 ];
            
            const int newDim1Size = oldDim1Size + ((int)std::abs( dim1Shift ));
            const int newDim2Size = oldDim2Size + ((int)std::abs( dim2Shift ));
            
            boost::multi_array< ArrayElementType, 3 > tempArray;            
            typename boost::multi_array< ArrayElementType, 3 >::extent_gen extent;
            tempArray.resize( extent[ ((ArrayIdxT)oldDim0Size) ][ 
              ((ArrayIdxT)newDim1Size) ][ ((ArrayIdxT)newDim2Size) ] );
              
            int newIdx1 = 0;
            int newIdx2 = 0;
            int oldIdx0 = 0;
            int oldIdx1 = 0;
            int oldIdx2 = 0;
            
            for( oldIdx0 = 0; oldIdx0 < oldDim0Size ; ++oldIdx0 )
            {
              for( newIdx1 = 0 ; newIdx1 < newDim1Size ; ++newIdx1 )
              {
                oldIdx1 = newIdx1 - dim1Shift;
                
                for( newIdx2 = 0 ; newIdx2 < newDim2Size ; ++newIdx2 )
                {
                  if( oldIdx0 == dim0Index )
                  {
                    oldIdx2 = newIdx2 - dim2Shift;
                  
                    if( ( oldIdx1 >= 0 ) && ( oldIdx1 < oldDim1Size ) &&
                      ( oldIdx2 >= 0 ) && ( oldIdx2 < oldDim2Size ) )
                    {
                      tempArray[ oldIdx0 ][ newIdx1 ][ newIdx2 ] =
                        array[ oldIdx0 ][ oldIdx1 ][ oldIdx2 ];
                    }
                    else
                    {
                      tempArray[ oldIdx0 ][ newIdx1 ][ newIdx2 ] =
                        dummyFillObj;
                        
                      newDummyElementsIndxes.push_back( BlockIndex3D( 
                        (unsigned int)oldIdx0, (unsigned int)newIdx1,
                        (unsigned int)newIdx2 ) );
                    }
                  }
                  else
                  {
                    if( ( newIdx1 < oldDim1Size ) && ( newIdx2 < oldDim2Size ) )
                    {
                      tempArray[ oldIdx0 ][ newIdx1 ][ newIdx2 ] =
                        array[ oldIdx0 ][ newIdx1 ][ newIdx2 ];
                    }
                    else
                    {
                      tempArray[ oldIdx0 ][ newIdx1 ][ newIdx2 ] =
                        dummyFillObj;
                    }
                  }
                }
              }
            }
            
            array.resize( extent[ ((ArrayIdxT)oldDim0Size) ][ 
              ((ArrayIdxT)newDim1Size) ][ ((ArrayIdxT)newDim2Size) ] );            
            array = tempArray;
          }
        };
        
        /*!
          \brief Retorns only the coords within the given range.
          
          \param inputContainer The input coords container.
          
          \param dim0Start The dimension 0 range start (included).
          
          \param dim0End The dimension 0 range ending (included).
          
          \param dim01tart The dimension 1 range start (included).
          
          \param dim1End The dimension 1 range ending (included).
          
          \param dim2Start The dimension 2 range start (included).
          
          \param dim2End The dimension 2 range ending (included).
          
          \param Retorns only the coords within the given range.
        */         
        template< typename ContainerType >
        ContainerType filterBlockIndexes( 
          const ContainerType& inputContainer,
          const unsigned int dim0Start, const unsigned int dim0End,
          const unsigned int dim1Start, const unsigned int dim1End,
          const unsigned int dim2Start, const unsigned int dim2End ) const
        {
          ContainerType outputContainer;
          
          typename ContainerType::const_iterator it = inputContainer.begin();
          const typename ContainerType::const_iterator itE = inputContainer.end();
          
          while( it != itE )
          {
            if( ( it->m_dim0Index >= dim0Start )
              && ( it->m_dim0Index <= dim0End )
              && ( it->m_dim1Index >= dim1Start )
              && ( it->m_dim1Index <= dim1End )
              && ( it->m_dim2Index >= dim2Start )
              && ( it->m_dim2Index <= dim2End ) )
            {
              outputContainer.push_back( *it );
            }
            
            ++it;
          }
          
          return outputContainer;
        };
        
        /*!
          \brief Retorns only the coords within the given range.
          
          \param inputContainer The input coords container.
          
          \param dim0Start The dimension 0 range start (included).
          
          \param dim0End The dimension 0 range ending (included).
          
          \param dim01tart The dimension 1 range start (included).
          
          \param dim1End The dimension 1 range ending (included).
          
          \param dim2Start The dimension 2 range start (included).
          
          \param dim2End The dimension 2 range ending (included).
          
          \param Retorns only the coords within the given range.
        */         
        template< typename ContainerType >
        ContainerType fixedDim0Shift3DCoords( 
          const ContainerType& inputContainer,
          const unsigned int dim0index, const int dim1Shift, const int dim2Shift ) const
        {
          ContainerType outputContainer;
          
          typename ContainerType::const_iterator it = inputContainer.begin();
          const typename ContainerType::const_iterator itE = inputContainer.end();
          BlockIndex3D newCoords;
          
          while( it != itE )
          {
            if( it->m_dim0Index == dim0index )
            {
              assert( ( dim1Shift < 0 ) ? ( ((int)it->m_dim1Index) > dim1Shift ) : true );
              newCoords.m_dim1Index = (BlockIndex3D::CoordDataType)
                ( ((int)it->m_dim1Index) + dim1Shift );

              assert( ( dim2Shift < 0 ) ? ( ((int)it->m_dim2Index) > dim2Shift ) : true );
              newCoords.m_dim2Index = (BlockIndex3D::CoordDataType)
                ( ((int)it->m_dim2Index) + dim2Shift );
            
              outputContainer.push_back( newCoords );
            }
            else
            {
              outputContainer.push_back( *it );
            }
            
            ++it;
          }
          
          return outputContainer;
        };        
        
      private :
        
        // Variables used by the method getBlockPointer
        unsigned char* m_getBlockPointer_returnValue;

    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_EXPANSIBLEBANDBLOCKSMANAGER_H

