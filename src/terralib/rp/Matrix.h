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
  \file terralib/rp/Matrix.h
  \brief Generic template matrix.
 */

#ifndef __TERRALIB_RP_INTERNAL_MATRIX_H
#define __TERRALIB_RP_INTERNAL_MATRIX_H

  #include "Functions.h"
  #include "Macros.h"
  
  #include "../common/PlatformUtils.h"

  #include <cstring>
  #include <cmath>
  #include <cstdio>
  #include <string>
  
  namespace te 
  {
    namespace rp
    {
      /*!
      \class Matrix
      \brief A generic template matrix
      */
      template< class ElementType >
      class Matrix {
        public :
          
          /*!
          \brief Public matrix element type definition.
          */          
          typedef ElementType ElementTypeT;
          
          /*!
          \brief Memory polycy.
          */
          enum MemoryPolicy 
          {
            /*!
            \details Automatic memory policy ( Try to use RAM or DISK, if there is no 
            avaliable RAM ) DO NOT USE AutoMemPol FOR COMPLEX DATA TYPES.
            */
            AutoMemPol,
            /*!
            \details RAM memory policy.
            */
            RAMMemPol,
            /*!
            \details Disk memory policy ( virtual mapped memory ) -
            DO NOT USE DiskMemPol FOR COMPLEX DATA TYPES.
            */
            DiskMemPol
          };
          
          Matrix();      

          Matrix( const Matrix< ElementType >& external );

          virtual ~Matrix();
          
          /*!
          \brief Reset (clear) the active instance data.
          \note Other parameters will not be changed.
          
          \param memoryPolicy Memory policy.
          */
          void reset();      
          
          /*!
          \brief Reset (clear) the active instance data and update its
          internal parameters folowing the the new supplied parameters.
          \note Other parameters will not be changed.
          
          \param memoryPolicy Memory policy.
          */
          void reset( MemoryPolicy memoryPolicy );       
          
          /*!
          \brief Reset (clear) the active instance data and update its
          internal parameters folowing the the new supplied parameters.
          \note Other parameters will not be changed.
          \param lines Number of lines.
          \param columns Number of columns.
          \return true if OK, false on error.
          */
          bool reset( unsigned int lines, unsigned int columns );      

          /*!
          \brief Reset (clear) the active instance data and update its
          internal parameters folowing the the new supplied parameters.
          \note Other parameters will not be changed.
          
          \param lines Number of lines.
          \param columns Number of columns.
          \param memoryPolicy Memory policy.
          \return true if OK, false on error.
          */
          bool reset( unsigned int lines, unsigned int columns,
            MemoryPolicy memoryPolicy );        
            
          /*!
          \brief Reset (clear) the active instance data and update its
          internal parameters folowing the the new supplied parameters.
          \note Other parameters will not be changed.
          
          \param lines Number of lines.
          \param columns Number of columns.
          \param memoryPolicy Memory policy.
          \param maxTmpFileSize Max temp file size.
          \param maxMemPercentUsage The max amount of free memory to use 
          when necessary (suggested default:40).
          \return true if OK, false on error.
          */
          bool reset( unsigned int lines, unsigned int columns,
            MemoryPolicy memoryPolicy, unsigned long int maxTmpFileSize,
            unsigned char maxMemPercentUsage ); 
            
          /*!
          \brief Clear all allocated resources and go back to the initial
          default parameters.
          */      
          void clear();            
          
          /*!
          \brief The number of current matrix lines.
          
          \return The number of current matrix lines.
          */
          unsigned int getLinesNumber() const;
          
          /*!
          \brief The number of current matrix columns.
          
          \return The number of current matrix columns
          */
          unsigned int getColumnsNumber() const;
          
          /*!
          \brief Empty Matrix verification.
          
          \return true if the matrix is empty.
          */
          bool isEmpty() const;

          /*!
          \brief Operator = overload.
          
          \note The external memory policy will be used as reference.
          
          \param external External instance reference.
          \return A reference to the current matrix.
          */
          const Matrix< ElementType >& operator=( 
            const Matrix< ElementType >& external );

          /*!
          \brief Operator () overload.
          
          \param line Line number.
          \param column Column number.
          \return A reference to the required element.
          */
          inline ElementType& operator()( const unsigned int& line, 
            const unsigned int& column )
          {
            TERP_DEBUG_TRUE_OR_THROW( ( column < totalColumns_ ),
              "Invalid columns" )
                
            return getScanLine( line )[ column ];      
          };
          
          /*!
          \brief Operator () overload.
          
          \param line Line number.
          \param column Column number.
          \return A const reference to the required element.
          */
          inline const ElementType& operator()( const unsigned int& line, 
            const unsigned int& column ) const
          {
            TERP_DEBUG_TRUE_OR_THROW( ( column < totalColumns_ ),
              "Invalid columns" )
              
            return getScanLine( line )[ column ] ;      
          }; 
          
          /*!
          \brief Operator [] overload.
          
          \param line Line number.
          \param column Column number.
          \return A pointer to the required line.
          \note The returned pointer is garanteed to 
          be valid until an acess to another line occurs.
          \note Concurrent thread access to this method is guaranteed
          if RAMMemPol policy method is used.
          */
          inline ElementType* operator[]( const unsigned int& line )
          {
            return getScanLine( line );
          };
          
          /*!
          \brief Operator [] overload.
          
          \param line Line number.
          \param column Column number.
          \return A pointer to the required line.
          \note The returned pointer is garanteed to 
          be valid until an acess to another line occurs.
          \note Concurrent thread access to this method is guaranteed
          if RAMMemPol policy method is used.       
          */
          inline ElementType const* operator[]( const unsigned int& line ) const
          {
            return getScanLine( line );
          };      
          
          /*!
          \brief Returns the current memory policy.
          \return The current memory policy.
          */      
          MemoryPolicy getMemPolicy() const;
          
          /*!
          \brief Returns the current maximum temporary disk file size.
          \return Returns the current maximum temporary disk file size.
          */            
          unsigned long int getMaxTmpFileSize() const; 
                
        protected :
        
          /*!
          \brief Disk lines info node.
          */         
          class DiskLinesInfoNode
          {
            public :
              
              FILE* filePtr_;
              unsigned int fileOff_;
              
              DiskLinesInfoNode() : filePtr_( 0 ), fileOff_( 0 ) {};
              ~DiskLinesInfoNode() {};
          };
          
          /*!
          \brief Openend disk files info node.
          */         
          class OpenDiskFilesInfoNode
          {
            public :
              
              FILE* filePtr_;
              std::string fileName_;
              
              OpenDiskFilesInfoNode() : filePtr_( 0 ) {};          
              ~OpenDiskFilesInfoNode() {};
          };      
          
          /*!
          \brief Max bytes per temp file (for swapped tiles only, default: 2GB).
          */
          unsigned long int maxTmpFileSize_;      
          
          /*!
          \brief the max amount of free memory to use when necessary
          (default:50).
          */        
          unsigned char maxMemPercentUsage_;
          
          /*!
          \brief The total lines number (default:0).
          */
          unsigned int totalLines_;
          
          /*!
          \brief The total columns number (default:0).
          */
          unsigned int totalColumns_;     
          
          /*!
          \brief The used memory policy (default:RAMMemPol).
          */
          MemoryPolicy memoryPolicy_;
          
          /*!
          \brief The line size (bytes, default:0).
          */     
          unsigned int lineSize_;      

          /*!
          \brief A vector with pointers to all lines (size 0 or totalLines_).
          */
          ElementType** allLinesPtrsVec_;     
          
          /*!
          \brief A vector with open disk files info (size 0 or openDiskFilesVecSize_).
          */      
          OpenDiskFilesInfoNode* openDiskFilesVec_;
          
          /*!
          \brief A vector with open disk files info.
          */      
          unsigned int openDiskFilesVecSize_;      
          
          /*!
          \brief Disk lines vector (size 0 or totalLines_).
          */      
          DiskLinesInfoNode* diskLinesVec_;      
          
          /*!
          \brief The indexes inside allTilesPtrsVec_ of all RAM lines (size 0 or ramLinesIndexesVecSize_).
          */        
          unsigned int* ramLinesIndexesVec_;
          
          /*!
          \brief The size of ramLinesIndexesVec_;
          */        
          unsigned int ramLinesIndexesVecSize_;      
          
          /*!
          \brief The index inside ramLinesIndexesVec_ of the next RAM
          line index that will be swapped to disk when a disk line
          is required.
          */        
          mutable unsigned int nextSwapLineRamLinesIndexesVecIdx_;
          
          /*!
          \brief A pointer to the auxiliar line used when swapping
          data to/from disk.
          */              
          mutable ElementType* swapLinePtr_;      
        
          /*!
          \brief Reset the internal variables to the initial state.
          */      
          void init();
          
          /*!
          \brief Allocate disk lines.
          \param startingLineIdx Starting line index.
          \return true if OK, false on errors.
          */    
          bool allocateDiskLines( unsigned int startingLineIdx );      
          
          /*!
          \brief Returns a pointer to the required line.
          
          \param line Line number.
          \param column Column number.
          \return A pointer to the required line.
          \note The returned pointer is garanteed to 
          be valid until an acess to another line occurs.
          \note Concurrent thread access to this method is guaranteed
          if RAMMemPol policy method is used.       
          */
          ElementType* getScanLine( const unsigned int& line ) const;
            
          /*!
          \brief Create a new disk file.
          \param filename The file name.
          \param size The file size.
          \param fileptr The file pointer.
          \return true if OK. false on errors.
          */
          bool createNewDiskFile( unsigned long int size,
            const std::string& filename, FILE** fileptr ) const;      
      };

      template< class ElementType >
      void Matrix< ElementType >::init()
      {
        maxTmpFileSize_ = 2ul * 1024ul * 1024ul * 1024ul;;
        maxMemPercentUsage_ = 40;
        totalLines_ = 0;
        totalColumns_ = 0;  
        memoryPolicy_ = RAMMemPol;
        lineSize_ = 0;
        allLinesPtrsVec_ = 0;
        openDiskFilesVec_ = 0;
        openDiskFilesVecSize_ = 0;
        diskLinesVec_ = 0;
        ramLinesIndexesVec_ = 0;
        ramLinesIndexesVecSize_ = 0;
        nextSwapLineRamLinesIndexesVecIdx_ = 0;
        swapLinePtr_ = 0;
      }

      template< class ElementType >
      Matrix< ElementType >::Matrix()
      {
        init();
      }    
      
      template< class ElementType >
      Matrix< ElementType >::Matrix( 
        const Matrix< ElementType >& external )
      {
        init();
        
        operator=( external );
      }

      template< class ElementType >
        Matrix< ElementType >::~Matrix()
      {
        clear();
      }
      
      template< class ElementType >
      void Matrix< ElementType >::reset()
      {
        reset( 0, 0, memoryPolicy_, maxTmpFileSize_, maxMemPercentUsage_ );
      }  
      
      template< class ElementType >
      void Matrix< ElementType >::reset( MemoryPolicy memoryPolicy )
      {
        reset( 0, 0, memoryPolicy, maxTmpFileSize_, maxMemPercentUsage_ );
      }   
      
      template< class ElementType >
      bool Matrix< ElementType >::reset( unsigned int lines, 
        unsigned int columns )
      {
        return reset( lines, columns, memoryPolicy_, maxTmpFileSize_,
          maxMemPercentUsage_ );
      } 
      
      template< class ElementType >
      bool Matrix< ElementType >::reset( unsigned int lines, 
        unsigned int columns, MemoryPolicy memoryPolicy )
      {
        return reset( lines, columns, memoryPolicy, maxTmpFileSize_, 
          maxMemPercentUsage_ );
      }

      template< class ElementType >
      bool Matrix< ElementType >::reset( unsigned int lines, 
        unsigned int columns, MemoryPolicy memoryPolicy,
        unsigned long int maxTmpFileSize,
        unsigned char maxMemPercentUsage )
      {
        clear();
        
        // Updating the global vars
        
        maxTmpFileSize_ = maxTmpFileSize;
        maxMemPercentUsage_ = maxMemPercentUsage;
        memoryPolicy_ = memoryPolicy;
        
        /* Update the old buffer if necessary */
        
        if( ( lines > 0 ) && ( columns > 0 ) )
        {
          try
          {
            // Updating the global vars
            
            totalLines_ = lines;
            totalColumns_ = columns;    
            lineSize_ = sizeof( ElementType ) * totalColumns_;
                  
            // Allocating the lines pointers vectpr
            
            allLinesPtrsVec_ = new ElementType*[ totalLines_ ];
            if( allLinesPtrsVec_ == 0 )
            {
              clear();
              TERP_LOG_AND_RETURN_FALSE( "Tiles allocation error" )
            }
            else
            {
              for( unsigned int allLinesPtrsVecIdx = 0 ; allLinesPtrsVecIdx <
                totalLines_ ; ++allLinesPtrsVecIdx )
              {
                allLinesPtrsVec_[ allLinesPtrsVecIdx ] = 0;
              }
            }
          
            if( memoryPolicy_ == RAMMemPol )
            {
              for( unsigned int allLinesPtrsVecIdx = 0 ; allLinesPtrsVecIdx <
                totalLines_ ; ++allLinesPtrsVecIdx )
              {
                allLinesPtrsVec_[ allLinesPtrsVecIdx ] = 
                  new ElementType[ totalColumns_ ];
                  
                if( allLinesPtrsVec_[ allLinesPtrsVecIdx ] == 0 )
                {
                  clear();
                  TERP_LOG_AND_RETURN_FALSE( "Tiles allocation error" )
                }
              }
            }
            else
            { // AutoMemPol, DiskMemPol
            
              // Allocating the swap line pointer
              
              swapLinePtr_ = new ElementType[ totalColumns_ ];
              if( swapLinePtr_ == 0 )
              {
                clear();
                TERP_LOG_AND_RETURN_FALSE( "Swap line allocation error" )
              }          
            
              // Defining the number of max RAM lines
              
              unsigned int maxRAMLines = 1;
              
              if( memoryPolicy_ == AutoMemPol )
              {
                // Defining the max number of RAM tiles
                
                const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
                
                const double usedVMem = (double)te::common::GetUsedVirtualMemory();
                
                const double totalVMem = (double)te::common::GetTotalVirtualMemory();
                
                const double freeVMem = MIN( totalPhysMem, 
                  ( totalVMem - usedVMem ) );
                
                const double linesMem = ( ((double)maxMemPercentUsage_) / 100.0 ) *
                  freeVMem;
                  
                maxRAMLines = (unsigned int)MAX( 1, linesMem / 
                  ((double)lineSize_) );
              }        
              
              // Allocating RAM lines
              
              ramLinesIndexesVecSize_ = MIN( maxRAMLines, totalLines_ );
              
              ramLinesIndexesVec_ = new unsigned int[ ramLinesIndexesVecSize_ ];
              if( ramLinesIndexesVec_ == 0 )
              {
                clear();
                TERP_LOG_AND_RETURN_FALSE( "Swap line allocation error" )
              }          
              
              for( unsigned int allLinesPtrsVecIdx = 0 ; allLinesPtrsVecIdx <
                ramLinesIndexesVecSize_ ; ++allLinesPtrsVecIdx )
              {
                allLinesPtrsVec_[ allLinesPtrsVecIdx ] = 
                  new ElementType[ totalColumns_ ];
                  
                if( allLinesPtrsVec_[ allLinesPtrsVecIdx ] == 0 )
                {
                  clear();
                  TERP_LOG_AND_RETURN_FALSE( "Tiles allocation error" )
                }
                else
                {
                  ramLinesIndexesVec_[ allLinesPtrsVecIdx ] = 
                    allLinesPtrsVecIdx;
                }
              }
              
              // Allocating Disk lines
            
              if( ! allocateDiskLines( ramLinesIndexesVecSize_ ) )
              {
                clear();
                TERP_LOG_AND_RETURN_FALSE( "Tiles allocation error" )
              }          
            }
          }
          catch(...)
          {
            clear();
            
            TERP_LOG_AND_RETURN_FALSE( "Tiles allocation error" )
          } 
        }
        
        return true;
      }
      
      template< class ElementType >
      void Matrix< ElementType >::clear()
      {
        if( allLinesPtrsVec_ )
        {
          for( unsigned int allLinesPtrsVecIdx = 0 ; allLinesPtrsVecIdx < 
            totalLines_ ; ++allLinesPtrsVecIdx ) 
          {
            if( allLinesPtrsVec_[ allLinesPtrsVecIdx ] ) 
            {
              delete[] allLinesPtrsVec_[ allLinesPtrsVecIdx ];
            }
          }  
          
          delete[] allLinesPtrsVec_;
        }
        
        if( openDiskFilesVec_ )
        {
          for( unsigned int openDiskFilesVecIdx = 0 ; openDiskFilesVecIdx < 
            openDiskFilesVecSize_ ; ++openDiskFilesVecIdx )
          {
            fclose( openDiskFilesVec_[ openDiskFilesVecIdx ].filePtr_ );
            remove( openDiskFilesVec_[ openDiskFilesVecIdx ].fileName_.c_str() );
          }
          
          delete[] openDiskFilesVec_; 
        }
          
        if( diskLinesVec_ ) delete[] diskLinesVec_;
        
        if( ramLinesIndexesVec_ ) delete[] ramLinesIndexesVec_;
        
        if( swapLinePtr_ ) delete[] swapLinePtr_;
      
        init();
      }    
      
      template< class ElementType >
      unsigned int Matrix< ElementType >::getLinesNumber() const
      {
        return totalLines_;
      }

      
      template< class ElementType >
      unsigned int Matrix< ElementType >::getColumnsNumber() const
      {
        return totalColumns_;
      }
      
      
      template< class ElementType >
      bool Matrix< ElementType >::isEmpty() const
      {
        return ( totalLines_ == 0 ) ? true : false;
      }
      

      template< class ElementType >
      const Matrix< ElementType >& Matrix< ElementType >::operator=(
        const Matrix< ElementType >& external )
      {
        TERP_TRUE_OR_THROW( 
          reset( external.totalLines_, external.totalColumns_,
          memoryPolicy_, maxTmpFileSize_, maxMemPercentUsage_ ),
          "Unable to initiate the matrix object" );
        
        unsigned int column = 0;;
        ElementType const* inLinePtr = 0;
        ElementType* outLinePtr = 0;
        
        for( unsigned int line = 0 ; line < totalLines_ ; ++line ) 
        {
          inLinePtr = external.getScanLine( line );
          outLinePtr = getScanLine( line );
          
          for( column = 0 ; column < totalColumns_ ; ++column ) {
            outLinePtr[ column ] = inLinePtr[ column ];
          }
        }

        return *this;
      }
      
      template< class ElementType >
      typename Matrix< ElementType >::MemoryPolicy Matrix< ElementType >::getMemPolicy() const
      {
        return memoryPolicy_;
      }  
      
      template< class ElementType >
      unsigned long int Matrix< ElementType >::getMaxTmpFileSize() const
      {
        return maxTmpFileSize_;
      }    
      
      template< class ElementType >
      bool Matrix< ElementType >::allocateDiskLines( unsigned int startingLineIdx )
      {
        const unsigned long int diskLinesNmb = totalLines_ - startingLineIdx;
        
        if( diskLinesNmb )
        {    
          const unsigned long int maxLinesPerFile = ( unsigned long int )
            floor( ( (double)maxTmpFileSize_ ) / ( (double) lineSize_ ) );
              
          const unsigned long int maxFileSize = (unsigned long int)
            ( maxLinesPerFile * lineSize_ );
            
          openDiskFilesVecSize_ = (unsigned int)ceil( ((double)diskLinesNmb) 
            / ((double)maxLinesPerFile) );

          // Allocating the open disf files info vector
          
          openDiskFilesVec_ = new OpenDiskFilesInfoNode[ openDiskFilesVecSize_ ];
          if( openDiskFilesVec_ == 0 )
          {
            return false;        
          }      
            
          // Allocating the disk lines info vector
          
          diskLinesVec_ = new DiskLinesInfoNode[ totalLines_ ];
          if( diskLinesVec_ == 0 )
          {
            return false;        
          }
          
          // Allocating each file
          
          unsigned int remainingLinesNmb = diskLinesNmb;
          unsigned int fileSize = 0;
          unsigned int fileLinesNumber = 0;    
          OpenDiskFilesInfoNode auxFileData;  
          unsigned int diskLinesVecIdx = startingLineIdx;
          char* newTempFileNamePtr;
          
          for( unsigned int fileIdx = 0 ; fileIdx < openDiskFilesVecSize_ ; 
            ++fileIdx )
          {
            // Defining the current file size
            
            fileSize = maxFileSize;
            fileLinesNumber = maxLinesPerFile;
            
            if( remainingLinesNmb < maxLinesPerFile )
            {
              fileSize = (unsigned long int)( lineSize_ * remainingLinesNmb );
              fileLinesNumber = remainingLinesNmb;
            }
            
            remainingLinesNmb -= fileLinesNumber;
            
            // generating a temp file name 
            
            newTempFileNamePtr = tempnam( 0, 0 );
            TERP_TRUE_OR_RETURN_FALSE( newTempFileNamePtr,
              "Unable to get temporary file name" );        
              
            // allocating the file
            
            if( ! createNewDiskFile( fileSize, newTempFileNamePtr, 
              &( auxFileData.filePtr_ ) ) )
            {
              TERP_LOGERR( "Unable to create temporary disk file" );
              return false;           
            }
            else
            {
              auxFileData.fileName_ = newTempFileNamePtr;          
              
              openDiskFilesVec_[ fileIdx ] = auxFileData;
              
              for( unsigned int lineIdx = 0; lineIdx < fileLinesNumber ; ++lineIdx )
              {
                diskLinesVec_[ diskLinesVecIdx ].filePtr_ = auxFileData.filePtr_;
                diskLinesVec_[ diskLinesVecIdx ].fileOff_ = lineIdx * lineSize_;
                
                ++diskLinesVecIdx;
              }
            }
          }
        }
        
        return true;
      }
      
      template< class ElementType >
      ElementType* Matrix< ElementType >::getScanLine( const unsigned int& line ) const
      {
        TERP_DEBUG_TRUE_OR_THROW( line < totalLines_, "Invalid tile index" );
          
        if( allLinesPtrsVec_[ line ] ) 
        {
          return allLinesPtrsVec_[ line ];
        } 
        else 
        {
          // Finding the swap line index 

          TERP_DEBUG_TRUE_OR_THROW( nextSwapLineRamLinesIndexesVecIdx_ <
            ramLinesIndexesVecSize_, "Internal error" );
          unsigned int swapLineIdx = ramLinesIndexesVec_[ 
            nextSwapLineRamLinesIndexesVecIdx_ ];
          
          TERP_DEBUG_TRUE_OR_THROW( line < totalLines_, "Internal error" );
          DiskLinesInfoNode& inLineData = diskLinesVec_[ line ];
          
          TERP_DEBUG_TRUE_OR_THROW( swapLineIdx < 
            totalLines_, "Internal error" )
          DiskLinesInfoNode& outLineData = diskLinesVec_[ 
            swapLineIdx ];
            
          /* Reading the required tile into RAM (swapTilePtr_) */
          
          TERP_DEBUG_TRUE_OR_THROW( inLineData.filePtr_, "Internal error" );
          TERP_TRUE_OR_THROW( 0 == fseek( inLineData.filePtr_, 
            (long)( inLineData.fileOff_ ), SEEK_SET ),
            "File seek error" )
            
          TERP_DEBUG_TRUE_OR_THROW( swapLinePtr_, "Internal error" );
          TERP_TRUE_OR_THROW( 1 == fread( (void*)swapLinePtr_, 
            (size_t)( lineSize_ ), 1, inLineData.filePtr_ ),
            "File read error" )
        
          /* Flushing the swap tile to disk */
            
          TERP_TRUE_OR_THROW( 0 == fseek( inLineData.filePtr_, 
            (long)( inLineData.fileOff_ ), SEEK_SET ),
            "File seek error" );
            
          TERP_DEBUG_TRUE_OR_THROW( swapLineIdx < totalLines_, 
            "Internal error" );          
          TERP_TRUE_OR_THROW( 1 == fwrite( (void*)allLinesPtrsVec_[ 
            swapLineIdx ], 
            (size_t)( lineSize_ ), 1, inLineData.filePtr_ ),
            "File write error" )        
          
          // Updating the tile pointers
          
          ElementType* linePtr = allLinesPtrsVec_[ swapLineIdx ];
          
          allLinesPtrsVec_[ swapLineIdx ] = 0;
          
          allLinesPtrsVec_[ line ] = swapLinePtr_;
          
          swapLinePtr_ = linePtr;
          
          /* Updating the info vectors */
              
          outLineData.filePtr_ = inLineData.filePtr_;
          outLineData.fileOff_ = inLineData.fileOff_;
          
          inLineData.filePtr_ = 0;
          inLineData.fileOff_ = 0;    
          
          ramLinesIndexesVec_[ nextSwapLineRamLinesIndexesVecIdx_ ] =
            line;
          nextSwapLineRamLinesIndexesVecIdx_ = 
            ( ( nextSwapLineRamLinesIndexesVecIdx_ + 1 ) % 
            ramLinesIndexesVecSize_ );
          TERP_DEBUG_TRUE_OR_THROW( nextSwapLineRamLinesIndexesVecIdx_ <
            ramLinesIndexesVecSize_, "Internal error" );          
              
          return allLinesPtrsVec_[ line ];
        }
      } 
      
      template< class ElementType >
      bool Matrix< ElementType >::createNewDiskFile( unsigned long int size,
        const std::string& filename, FILE** fileptr ) const
      {
        TERP_TRUE_OR_RETURN_FALSE( ! filename.empty(),
          "Invalid file name" );
          
        (*fileptr) = fopen( filename.c_str(), "wb+" );
        TERP_TRUE_OR_RETURN_FALSE( (*fileptr) != 0, "Invalid file pointer" )
        
        long seekoff = (long)( size - 1 );
        
        if( 0 != fseek( (*fileptr), seekoff, SEEK_SET ) )
        {
          fclose( (*fileptr) );
          TERP_LOGERR( "File seek error" );
          return false;
        }
      
        unsigned char c = '\0';
        if( 1 != fwrite( &c, 1, 1, (*fileptr) ) )
        {
          fclose( (*fileptr) );
          TERP_LOGERR( "File write error" );
          return false;
        }
          
        return true;
      }  
    }  //namespace rp
  } // namespace te
#endif //__TERRALIB_RP_INTERNAL_MATRIX_H

