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
 \defgroup SegmenterAlgorithm Raster segmentation.
 \ingroup RPAlgorithms
 */ 

/*!
  \file terralib/rp/Segmenter.h
  \brief Raster segmentation.
 */

#ifndef __TERRALIB_RP_INTERNAL_SEGMENTER_H
#define __TERRALIB_RP_INTERNAL_SEGMENTER_H

#include "SegmenterStrategyParameters.h"
#include "SegmenterStrategy.h"
#include "SegmenterSegmentsBlock.h"
#include "SegmenterIdsManager.h"
#include "Algorithm.h"
#include "AlgorithmParameters.h"
#include "Matrix.h"
#include "Config.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <vector>
#include <string>

namespace te
{
  namespace da
  {
    class DataSource;    
  };
  
  namespace rst
  {
    class Raster;
    class Band;
  };
  
  namespace rp
  {
    // Forward declarations
    class RasterHandler;
    
    /*!
      \class Segmenter
      \brief Raster segmentation.
      \details Raster segmentation following the choosed segmentation
      strategy.
      \note The created label image will be written to the raster instance 
      pointed by m_outRasterPtr (in this case the output band must also be 
      passed by m_outRasterBand ); or written to a new raster instance created 
      inside the given data source pointed by m_outDataSourcePtr (in this case 
      the data set name must be supplied - m_outDataSetName ).      
      \sa te::rp::Segmenter::Parameters For detailed parameters description.
      \sa te::rp::SegmenterStrategy For detailed parameters description.
      \ingroup SegmenterAlgorithm
     */
    class TERPEXPORT Segmenter : public Algorithm
    {
      public:
        
        /*!
          \class Parameters
          \brief Segmenter Parameters
         */        
        class TERPEXPORT Parameters : public AlgorithmParameters
        {
          public:
            
            te::rst::Raster* m_inRasterPtr; //!< Input raster.
            
            std::vector< unsigned int > m_inRasterBands; //!< Bands to be processed from the input raster.
            
            te::rst::Raster* m_outRasterPtr; //!< Output raster (generated label image).
            
            unsigned int m_outRasterBand; //!< Band where the data will be written to the output raster (generated label image).
            
            te::da::DataSource* m_outDataSourcePtr; //!< The data source where the output raster (generated label image) will be stored.
            
            std::string m_outDataSetName; //!< The data set name related to the output raster (generated label image).
            
            boost::shared_ptr< RasterHandler > m_outRasterHandlerPtr; //!< A handler for the generated output raster (generated label image).
            
            bool m_enableThreadedProcessing; //!< If true, threaded processing will be performed (best with  multi-core or multi-processor systems (default:false).
            
            unsigned int m_maxSegThreads; //!< The maximum number of concurrent segmenter threads (default:0 - automatically found).
            
            bool m_enableBlockProcessing; //!< If true, the original raster will be splitted into small blocks, each one will be segmented independently and the result will be merged (if possible) at the end (default:true).
            
            bool m_enableBlockMerging; //!< If true, a block merging procedure will be performed (default:true).
            
            unsigned int m_maxBlockSize; //!< The input image will be split into blocks with this width for processing, this parameter tells the maximum block lateral size (width or height), the default: 0 - the size will be defined following the current system resources and physical processors number).
            
            std::string m_strategyName; //!< The segmenter strategy name see each te::rp::SegmenterStrategyFactory inherited classes documentation for reference.
            
            Parameters();
            
            ~Parameters();
            
            /*!
              \brief Set specific segmenter strategy parameters.
              \param params The specific segmenter strategy parameters.
            */            
            void setSegStrategyParams( 
              const SegmenterStrategyParameters& segStratParams );

            /*!
              \brief Returns a pointer to the internal specific segmenter strategy parameters.
              \return A pointer to the internal specific segmenter strategy parameters, or
              null if no parameters are present.
            */                                
            SegmenterStrategyParameters const* getSegStrategyParams() const;              
            
            //overload
            void reset() throw( te::rp::Exception );
            
            //overload
            const AlgorithmParameters& operator=( const AlgorithmParameters& params );
            
            //overload
            AbstractParameters* clone() const;   
            
          protected :
            
            /*!
              \brief A pointer to the internal specific segmenter strategy parameters or
              NULL if no parameters are present.
            */               
            SegmenterStrategyParameters* m_segStratParamsPtr;
        };

        Segmenter();
        
        ~Segmenter();
       
        //overload
        bool execute() throw( te::rp::Exception );          
        
        //overload
        void reset() throw( te::rp::Exception );        
        
        //overload
        bool initialize( AlgorithmParameters& params ) 
          throw( te::rp::Exception );
          
      protected :
        
        /*! Segments blocks matrix type definition */
        typedef te::rp::Matrix< SegmenterSegmentsBlock > SegmentsBlocksMatrixT;
        
          /*! 
          \brief The parameters passed to the Segmenter::segmenterthreadEntry method.
        */     
        class SegmenterThreadEntryParams
        {
          public :
            //! A pointer to the global segmenter execution parameters (default:0). */
            Segmenter::Parameters const* m_parametersPtr; //!< Segmenter execution parameters.
            
            /*! Pointer to the segments blocks matrix (default:0)*/
            SegmentsBlocksMatrixT* m_segsBlocksMatrixPtr;
            
            /*! Pointer to a general global mutex (default:0)*/
            boost::mutex* m_generalMutexPtr;            
            
            /*! Pointer to the mutex used when accessing the input raster (default:0)*/
            boost::mutex* m_inputRasterIOMutexPtr;
            
            /*! Pointer to the mutex used when accessing the output raster (default:0)*/
            boost::mutex* m_outputRasterIOMutexPtr;
            
            /*! Pointer to the mutex used by the block processed signal (default:0)*/
            boost::mutex* m_blockProcessedSignalMutexPtr;            
            
            /*! Pointer to the abort segmentation flag (default:0)*/
            bool volatile* m_abortSegmentationFlagPtr;
            
            /*! Pointer to the segments Ids manager - (default 0) */
            SegmenterIdsManager* m_segmentsIdsManagerPtr;
            
            /*! Pointer to a signal to be emited when a segments block was processed (default:0)*/
            boost::condition_variable* m_blockProcessedSignalPtr;
            
            /*! Pointer to the running threads counter - default 0) */
            unsigned int volatile* m_runningThreadsCounterPtr;            
            
            SegmenterThreadEntryParams();
            
            ~SegmenterThreadEntryParams();
        };      
        
        bool m_instanceInitialized; //"< Is this instance already initialized ?
        
        Segmenter::Parameters m_parameters; //!< Segmenter execution parameters.
        
        /*! 
          \brief Calc the best sub-image block size for each thread to
          process.
          \param totalImageLines The total original full image lines.
          \param totalImageCols The total original full image columns.
          \param minBlockPixels The minimun allowed pixels number for each block.
          \param maxBlockPixels The maximum allowed pixels number for each block.
          \param blocksHOverlapSize The blocks horizontal overlap size.
          \param blocksVOverlapSize The blocks vertical overlap size.
          \param blockWidth The calculated block width.
          \param blockHeight The calculated block height.
          \return true if OK, false on errors.
        */                
        bool calcBestBlockSize( const unsigned int totalImageLines, 
          const unsigned int totalImageCols, const unsigned int minBlockPixels,
          const unsigned int maxBlockPixels, const unsigned int blocksHOverlapSize,
          const unsigned int blocksVOverlapSize, unsigned int& blockWidth,
          unsigned int& blockHeight ) const;        
          
        /*! 
          \brief Segmenter thread entry.
          \param paramsPtr A pointer to the segmenter thread parameters.
        */      
        static void segmenterThreadEntry(SegmenterThreadEntryParams* paramsPtr);
        
        /*! 
          \brief Generate the horizontal cutOff prifles for the entire image..
          \param profileCenter The profile center line.
          \param inRaster The input raster.
          \param inRasterBands The input raster bands.
          \param pixelNeighborhoodSize The pixel neighborhood size over the 
          line transverse to each tile line.
          \param tileNeighborhoodSize The buffer size around each tile.
          \param profileAntiSmoothingFactor A positive profile anti-smoothing factor.
          \param profile The generated profile (each element is a inRaster
          line index ).
          \return true if OK, false on errors.
        */                
        bool genImageHCutOffProfile( const unsigned int profileCenter,
          const te::rst::Raster& inRaster,
          const std::vector< unsigned int >& inRasterBands,
          const unsigned int pixelNeighborhoodSize,
          const unsigned int tileNeighborhoodSize,
          const unsigned int profileAntiSmoothingFactor,
          std::vector< unsigned int>& profile ) const;  
          
        /*! 
          \brief Generate the vertical cutOff prifles for the entire image..
          \param profileCenter The profile center column.
          \param inRaster The input raster.
          \param inRasterBands The input raster bands.
          \param pixelNeighborhoodSize The pixel neighborhood size over the 
          line transverse to each tile line.
          \param tileNeighborhoodSize The buffer size around each tile.
          \param profileAntiSmoothingFactor A positive profile anti-smoothing factor.
          \param profile The generated profile (each element is a inRaster
          column index )
          \return true if OK, false on errors.
        */                
        bool genImageVCutOffProfile( const unsigned int profileCenter,
          const te::rst::Raster& inRaster,
          const std::vector< unsigned int >& inRasterBands,
          const unsigned int pixelNeighborhoodSize,
          const unsigned int tileNeighborhoodSize,
          const unsigned int profileAntiSmoothingFactor,
          std::vector< unsigned int>& profile ) const;          
          
        /*! 
          \brief Update the block cutOff profiles using the full image profiles.
          \param imageVerticalProfiles The generated horizontal profiles (empty profiles also will be accepted).
          \param horizontalProfiles The generated vertical profiles (empty profiles also will be accepted).
          \param segmentsBlock The block to update.
          \return true if OK, false on errors.
        */           
        bool updateBlockCutOffProfiles( 
          const std::vector< std::vector< unsigned int> >& imageHorizontalProfiles,
          const std::vector< std::vector< unsigned int> >& imageVerticalProfiles,
          SegmenterSegmentsBlock& segmentsBlock ) const;    
          
        bool createCutOffLinesTiff(  
          const std::vector< std::vector< unsigned int> >& imageHorizontalProfiles,
          const std::vector< unsigned int >& imageHorizontalProfilesCenterLines,
          const std::vector< std::vector< unsigned int> >& imageVerticalProfiles,
          const std::vector< unsigned int >& imageVerticalProfilesCenterLines,
          const std::string& filename );
    };

  } // end namespace rp
}   // end namespace te

#endif

