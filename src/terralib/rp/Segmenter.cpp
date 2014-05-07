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
  \file terralib/rp/Segmenter.cpp
  \brief Raster segmentation.
*/

#include "Segmenter.h"

#include "SegmenterStrategyFactory.h"
#include "Functions.h"
#include "Macros.h"

#include "../raster/RasterFactory.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/Raster.h"
#include "../raster/Enums.h"
#include "../raster/Grid.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../common/PlatformUtils.h"
#include "../common/StringUtils.h"
#include "../geometry/Coord2D.h"
#include "../datatype/Enums.h"

#include <map>
#include <memory>
#include <climits>
#include <cmath>
#include <cfloat>

namespace te
{
  namespace rp
  {
    Segmenter::InputParameters::InputParameters()
    {
      m_segStratParamsPtr = 0;
      reset();
    }

    Segmenter::InputParameters::InputParameters( const InputParameters& other )
    {
      m_segStratParamsPtr = 0;
      reset();
      operator=( other );
    }
    
    Segmenter::InputParameters::~InputParameters()
    {
      reset();
    }
    
    void Segmenter::InputParameters::reset() throw( te::rp::Exception ) 
    {
      m_inputRasterPtr = 0;
      m_inputRasterBands.clear();
      m_enableThreadedProcessing = true;
      m_maxSegThreads = 0;
      m_enableBlockProcessing = true;
      m_enableBlockMerging = true;
      m_maxBlockSize = 0;
      m_strategyName.clear();
      m_enableProgress = false;
      
      if( m_segStratParamsPtr )
      {
        delete m_segStratParamsPtr;
        m_segStratParamsPtr = 0;
      }
    }
    
    const Segmenter::InputParameters& Segmenter::InputParameters::operator=( 
      const Segmenter::InputParameters& params )
    {
      reset();
        
      m_inputRasterPtr = params.m_inputRasterPtr;
      m_inputRasterBands = params.m_inputRasterBands;
      m_enableThreadedProcessing = params.m_enableThreadedProcessing;
      m_maxSegThreads = params.m_maxSegThreads;
      m_enableBlockProcessing = params.m_enableBlockProcessing;
      m_enableBlockMerging = params.m_enableBlockMerging;
      m_maxBlockSize = params.m_maxBlockSize;
      m_strategyName = params.m_strategyName;
      m_enableProgress = params.m_enableProgress;
      
      m_segStratParamsPtr = params.m_segStratParamsPtr ? 
        (SegmenterStrategyParameters*)params.m_segStratParamsPtr->clone()
        : 0; 
      
      return *this;
    }
    
    te::common::AbstractParameters* Segmenter::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }    
    
    void Segmenter::InputParameters::setSegStrategyParams( 
      const SegmenterStrategyParameters& params )
    {
      if( m_segStratParamsPtr )
      {
        delete m_segStratParamsPtr;
        m_segStratParamsPtr = 0;
      }
      
      m_segStratParamsPtr = (SegmenterStrategyParameters*)params.clone();
    }
    
    SegmenterStrategyParameters const* 
      Segmenter::InputParameters::getSegStrategyParams() const
    {
      return m_segStratParamsPtr;
    }    
    
    Segmenter::OutputParameters::OutputParameters()
    {
      reset();
    }
    
    Segmenter::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }
    
    Segmenter::OutputParameters::~OutputParameters()
    {
      reset();
    }
    
    void Segmenter::OutputParameters::reset() throw( te::rp::Exception ) 
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }
    
    const Segmenter::OutputParameters& Segmenter::OutputParameters::operator=( 
      const Segmenter::OutputParameters& params )
    {
      reset();
        
      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }
    
    te::common::AbstractParameters* Segmenter::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }
    
    Segmenter::SegmenterThreadEntryParams::SegmenterThreadEntryParams()
    {
      m_inputParametersPtr = 0;
      m_outputParametersPtr = 0;
      m_segsBlocksMatrixPtr = 0;
      m_generalMutexPtr = 0;
      m_inputRasterIOMutexPtr = 0;
      m_outputRasterIOMutexPtr = 0;
      m_blockProcessedSignalMutexPtr = 0;
      m_abortSegmentationFlagPtr = 0;
      m_segmentsIdsManagerPtr = 0;
      m_blockProcessedSignalPtr = 0;
      m_runningThreadsCounterPtr = 0;
      m_inputRasterGainsPtr = 0;
      m_inputRasterOffsetsPtr = 0;
      m_enableStrategyProgress = false;
      m_progressPtr = 0;
    }
    
    Segmenter::SegmenterThreadEntryParams::~SegmenterThreadEntryParams()
    {
    }
        
    Segmenter::Segmenter()
    {
      m_instanceInitialized = false;
    }
    
    Segmenter::~Segmenter()
    {
    }    
    
    bool Segmenter::execute( AlgorithmOutputParameters& outputParams ) 
      throw( te::rp::Exception )
    {
      if( m_instanceInitialized )
      {
        // creating the output raster
        
        Segmenter::OutputParameters* outputParamsPtr = dynamic_cast< 
          Segmenter::OutputParameters* >( &outputParams );          
        TERP_TRUE_OR_RETURN_FALSE( outputParamsPtr, "Invalid parameters" );
        
        {
          std::vector< te::rst::BandProperty* > bandsProperties;
          bandsProperties.push_back( new te::rst::BandProperty(
            *( m_inputParameters.m_inputRasterPtr->getBand( 
            m_inputParameters.m_inputRasterBands[ 0 ] )->getProperty() ) ) );  
          bandsProperties[ 0 ]->m_colorInterp = te::rst::GrayIdxCInt;
          bandsProperties[ 0 ]->m_noDataValue = 0;
          bandsProperties[ 0 ]->m_type = te::dt::UINT32_TYPE;
          
          outputParamsPtr->m_outputRasterPtr.reset( 
            te::rst::RasterFactory::make(
              outputParamsPtr->m_rType, 
              new te::rst::Grid( *( m_inputParameters.m_inputRasterPtr->getGrid() ) ),
              bandsProperties,
              outputParamsPtr->m_rInfo,
              0,
              0 ) );
          TERP_TRUE_OR_RETURN_FALSE( outputParamsPtr->m_outputRasterPtr.get(),
            "Output raster creation error" );
        }
        
        // Finding the input raster normalization parameters
        
        std::vector< double > inputRasterGains( 
          m_inputParameters.m_inputRasterBands.size(), 0.0 );
        std::vector< double > inputRasterOffsets( 
          m_inputParameters.m_inputRasterBands.size(), 0.0 );
          
        {
          const unsigned int nRows = 
            m_inputParameters.m_inputRasterPtr->getNumberOfRows();
          const unsigned int nCols = 
            m_inputParameters.m_inputRasterPtr->getNumberOfColumns();
          unsigned int row = 0;
          unsigned int col = 0;
          double bandMin = DBL_MAX;
          double bandMax = -1.0 * DBL_MAX;
          double value = 0;
          
          for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx <
            m_inputParameters.m_inputRasterBands.size() ; ++inputRasterBandsIdx )
          {
            const te::rst::Band& band = 
              *(m_inputParameters.m_inputRasterPtr->getBand( 
              m_inputParameters.m_inputRasterBands[ inputRasterBandsIdx ] ) );
            bandMin = DBL_MAX;
            bandMax = -1.0 * DBL_MAX;
            
            for( row = 0 ; row < nRows ; ++row )
              for( col = 0 ; col < nCols ; ++col )
              {
                band.getValue( col, row, value );
                
                if( bandMin > value ) bandMin = value;
                if( bandMax < value ) bandMax = value;
              }
              
            if( bandMax != bandMin )
            {
              inputRasterGains[ inputRasterBandsIdx ] = 1.0 / ( bandMax - bandMin );
              inputRasterOffsets[ inputRasterBandsIdx ] = -1.0 * bandMin;
            }
          }
        }
        
        // instantiating the segmentation strategy
        
        std::auto_ptr< SegmenterStrategy > strategyPtr(
          SegmenterStrategyFactory::make( m_inputParameters.m_strategyName ) );
        TERP_TRUE_OR_RETURN_FALSE( strategyPtr.get(), 
          "Unable to create an segmentation strategy" ); 
        TERP_TRUE_OR_RETURN_FALSE( strategyPtr->initialize( 
          m_inputParameters.getSegStrategyParams() ), 
          "Unable to initialize the segmentation strategy" );   
          
        const double stratMemUsageEstimation = strategyPtr->getMemUsageEstimation(
          m_inputParameters.m_inputRasterBands.size(),
          m_inputParameters.m_inputRasterPtr->getNumberOfRows() *
          m_inputParameters.m_inputRasterPtr->getNumberOfColumns() );  
        TERP_DEBUG_TRUE_OR_THROW( stratMemUsageEstimation > 0.0,
          "Invalid strategy memory usage factorMemUsageFactor" );       
          
        const unsigned stratBlocksOverlapSize = 
          strategyPtr->getOptimalBlocksOverlapSize();
        
        // defining the number of processing threads
        
        unsigned int maxSegThreads = 0;
        if( m_inputParameters.m_enableBlockProcessing && 
          m_inputParameters.m_enableThreadedProcessing )
        {
          maxSegThreads = ( m_inputParameters.m_maxSegThreads ? 
            m_inputParameters.m_maxSegThreads : te::common::GetPhysProcNumber() );
        }
        
        // Guessing memory limits
        
        const unsigned int totalRasterPixels = 
          m_inputParameters.m_inputRasterPtr->getNumberOfRows() * 
          m_inputParameters.m_inputRasterPtr->getNumberOfColumns();
        const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
        const double usedVMem = (double)te::common::GetUsedVirtualMemory();
        const double totalVMem = ( (double)te::common::GetTotalVirtualMemory() );
        const double freeVMem = MIN( totalPhysMem, ( totalVMem - usedVMem ) );
        const double pixelRequiredRam = stratMemUsageEstimation
          / ((double)totalRasterPixels);
        const double maxSimultaneousMemoryPixels = 0.7 * MIN( 
          ((double)totalRasterPixels), 
          freeVMem / pixelRequiredRam );         
        
        // Calc the maximum block width & height
        
        unsigned int maxNonExpandedBlockWidth = 0;
        unsigned int maxNonExpandedBlockHeight = 0;
        unsigned int maxExpandedBlockWidth = 0;
        unsigned int maxExpandedBlockHeight = 0;
        unsigned int blocksHOverlapSize = 0;
        unsigned int blocksVOverlapSize = 0;
        
        if( m_inputParameters.m_enableBlockProcessing 
            &&
            (
              ( maxSegThreads > 0 )
              ||
              ( maxSimultaneousMemoryPixels < ((double)totalRasterPixels ) )
              ||
              ( 
                ( m_inputParameters.m_maxBlockSize > 0 )
                &&
                (
                  ( m_inputParameters.m_maxBlockSize * m_inputParameters.m_maxBlockSize )
                  < 
                  totalRasterPixels 
                )
              )
            )
          )
        {
          // Calculating max bock pixels using the avaliable resources or
          // the user given parameters
          
          unsigned int maxBlockPixels = 0;
          
          if( m_inputParameters.m_maxBlockSize )
          {
            maxBlockPixels = m_inputParameters.m_maxBlockSize * 
              m_inputParameters.m_maxBlockSize;
          }
          else
          {
            maxBlockPixels = static_cast<unsigned int>(
              ( maxSimultaneousMemoryPixels / 
              ( static_cast<double>( maxSegThreads ? maxSegThreads : 1 ) ) ) );
          }

          // updating maxBlockPixels considering the blocks overlap size
          
          if( m_inputParameters.m_enableBlockMerging )
          {
            // Adjusting the block sizes
            
            TERP_TRUE_OR_RETURN_FALSE( calcBestBlockSize( 
              m_inputParameters.m_inputRasterPtr->getNumberOfRows(),
              m_inputParameters.m_inputRasterPtr->getNumberOfColumns(), 
              std::max( blocksHOverlapSize, blocksVOverlapSize ) *
                std::max( blocksHOverlapSize, blocksVOverlapSize ),
              maxBlockPixels, 
              0.4, 
              0.4,
              maxNonExpandedBlockWidth, 
              maxNonExpandedBlockHeight,
              blocksHOverlapSize,
              blocksVOverlapSize ), 
              "Error calculating best block size" );   
            
            maxExpandedBlockWidth = maxNonExpandedBlockWidth + 
              blocksHOverlapSize + blocksHOverlapSize;
            maxExpandedBlockHeight = maxNonExpandedBlockHeight +
              blocksVOverlapSize + blocksVOverlapSize;
          }
          else
          {
            // Adjusting the block sizes
            
            unsigned int blocksHOverlapSize = 0;
            unsigned int blocksVOverlapSize = 0;       
            
            TERP_TRUE_OR_RETURN_FALSE( calcBestBlockSize( 
              m_inputParameters.m_inputRasterPtr->getNumberOfRows(),
              m_inputParameters.m_inputRasterPtr->getNumberOfColumns(), 
              stratBlocksOverlapSize * stratBlocksOverlapSize,
              maxBlockPixels, 
              0.0, 
              0.0,
              maxNonExpandedBlockWidth, 
              maxNonExpandedBlockHeight,
              blocksHOverlapSize,
              blocksVOverlapSize ), 
              "Error calculating best block size" );  
              
            maxExpandedBlockWidth = maxNonExpandedBlockWidth;
            maxExpandedBlockHeight = maxNonExpandedBlockHeight;
          }
        }
        else
        {
          maxNonExpandedBlockWidth = maxExpandedBlockWidth =
            m_inputParameters.m_inputRasterPtr->getNumberOfColumns();
          maxNonExpandedBlockHeight = maxExpandedBlockHeight =
            m_inputParameters.m_inputRasterPtr->getNumberOfRows();
        }

        // Defining number of blocks
          
        const unsigned int hBlocksNumber = (unsigned int)ceil( 
          ((double)m_inputParameters.m_inputRasterPtr->getNumberOfColumns()) / 
          ((double)maxNonExpandedBlockWidth) );
        const unsigned int vBlocksNumber = (unsigned int)ceil( 
          ((double)m_inputParameters.m_inputRasterPtr->getNumberOfRows()) / 
          ((double)maxNonExpandedBlockHeight ) );        

        // Generating cut off profiles. When possible, an empty profile
        // vector is generated
        
        std::vector< std::vector< unsigned int> > imageHorizontalProfiles;
        std::vector< unsigned int > imageHorizontalProfilesCenterLines;
        std::vector< std::vector< unsigned int> > imageVerticalProfiles;
        std::vector< unsigned int > imageVerticalProfilesCenterLines;
        
        if( m_inputParameters.m_enableBlockProcessing &&
          m_inputParameters.m_enableBlockMerging )
        {
//          std::cout << std::endl << "Starting CutOff profiles generation" << std::endl;
          
          const unsigned int tileHNeighborhoodSize =  blocksHOverlapSize / 2;
          const unsigned int tileVNeighborhoodSize =  blocksVOverlapSize / 2;
          const unsigned int pixelNeighborhoodSize = 5;
          const unsigned int profileAntiSmoothingFactor = 3;            
          
          std::vector< unsigned int> profile;
          unsigned int profileIdx = 0;
          
          for( profileIdx = 1 ; profileIdx < hBlocksNumber ; 
            ++profileIdx )
          {
            profile.clear();
            
            const unsigned int centerLine = ( ( profileIdx ) * 
              maxNonExpandedBlockHeight );
            imageHorizontalProfilesCenterLines.push_back( centerLine );
            
            if( genImageHCutOffProfile( centerLine,
              *(m_inputParameters.m_inputRasterPtr), m_inputParameters.m_inputRasterBands, 
              pixelNeighborhoodSize, tileHNeighborhoodSize, 
              profileAntiSmoothingFactor, profile ) )
            {
              imageHorizontalProfiles.push_back( profile );
            }
            else
            {
              imageHorizontalProfiles.push_back( std::vector< unsigned int>() );
            }
          }
          
          for( profileIdx = 1 ; profileIdx < vBlocksNumber ; 
            ++profileIdx )
          {
            profile.clear();
            
            const unsigned int centerLine = ( ( profileIdx ) * 
              maxNonExpandedBlockWidth );
            imageVerticalProfilesCenterLines.push_back( centerLine );            
            
            if( genImageVCutOffProfile( centerLine,
              *(m_inputParameters.m_inputRasterPtr), m_inputParameters.m_inputRasterBands, 
              pixelNeighborhoodSize, tileVNeighborhoodSize, 
              profileAntiSmoothingFactor, profile ) )
            {
              imageVerticalProfiles.push_back( profile );
            }
            else
            {
              imageVerticalProfiles.push_back( std::vector< unsigned int>() );
            }
          }          
/*          
          TERP_TRUE_OR_THROW( createCutOffLinesTiff( imageHorizontalProfiles,
            imageHorizontalProfilesCenterLines,
            imageVerticalProfiles, imageVerticalProfilesCenterLines,
            "cutoflines.tif" ), "internal error" )                                         
*/            

//          std::cout << std::endl << "CutOff profiles generated" << std::endl;
        }
        
        // Initializing the segments blocks matrix
        
        SegmentsBlocksMatrixT segmentsblocksMatrix;
        segmentsblocksMatrix.reset( SegmentsBlocksMatrixT::RAMMemPol );
        
        {
          TERP_TRUE_OR_RETURN_FALSE( segmentsblocksMatrix.reset( vBlocksNumber, 
            hBlocksNumber ), "Blocks matrix reset error" );
            
          const int linesBound = (int)
            m_inputParameters.m_inputRasterPtr->getNumberOfRows();
          const int colsBound = (int)
            m_inputParameters.m_inputRasterPtr->getNumberOfColumns();
          int blockXBound = 0;
          int blockYBound = 0;
          int blockXStart = 0;
          int blockYStart = 0;
            
          for( unsigned int segmentsMatrixLine = 0 ; segmentsMatrixLine < 
            segmentsblocksMatrix.getLinesNumber() ; ++segmentsMatrixLine )
          {
            for( unsigned int segmentsMatrixCol = 0 ; segmentsMatrixCol < 
              segmentsblocksMatrix.getColumnsNumber() ; ++segmentsMatrixCol )
            {
              SegmenterSegmentsBlock& segmentsBlock = segmentsblocksMatrix( 
                segmentsMatrixLine, segmentsMatrixCol );
                
              blockXStart = ((int)( segmentsMatrixCol * 
                maxNonExpandedBlockWidth ) ) - ((int)blocksHOverlapSize );
              blockYStart = ((int)( segmentsMatrixLine * 
                maxNonExpandedBlockHeight ) ) - ((int)blocksVOverlapSize );
              blockXBound = blockXStart + ((int)maxExpandedBlockWidth);
              blockYBound = blockYStart + ((int)maxExpandedBlockHeight);          
                
              segmentsBlock.m_status = 
                SegmenterSegmentsBlock::BlockNotProcessed;
              segmentsBlock.m_startX = (unsigned int)MAX( 0, blockXStart );
              segmentsBlock.m_startY = (unsigned int)MAX( 0, blockYStart );          
                
              segmentsBlock.m_width = ((unsigned int)MIN( blockXBound, 
                colsBound ) ) - segmentsBlock.m_startX;
              segmentsBlock.m_height = ((unsigned int)MIN( blockYBound, 
                linesBound ) ) - segmentsBlock.m_startY;
              
              segmentsBlock.m_segmentsMatrixXIndex = segmentsMatrixCol;
              segmentsBlock.m_segmentsMatrixYIndex = segmentsMatrixLine;
              
              // transfering cutoff profiles
              
              if( m_inputParameters.m_enableBlockProcessing &&
                m_inputParameters.m_enableBlockMerging )
              {
                TERP_TRUE_OR_RETURN_FALSE( updateBlockCutOffProfiles( 
                  imageHorizontalProfiles, imageVerticalProfiles, 
                  segmentsBlock ), "Block cutOff profiles update error" );
              }
            }
          }
        }
        
        // Starting the segmentation 
        
        boost::mutex generalMutex;
        boost::mutex inputRasterIOMutex;
        boost::mutex outputRasterIOMutex;
        boost::mutex blockProcessedSignalMutex;
        
        volatile bool abortSegmentationFlag = false;  
        
        SegmenterIdsManager segmenterIdsManager;
        
        boost::condition_variable blockProcessedSignal;
        
        volatile unsigned int runningThreadsCounter = 0;
        
        std::auto_ptr< te::common::TaskProgress > progressPtr;
        if( m_inputParameters.m_enableProgress &&
          ( ( vBlocksNumber * hBlocksNumber ) > 1 ) )
        {
          progressPtr.reset( new te::common::TaskProgress );
          progressPtr->setTotalSteps( vBlocksNumber * hBlocksNumber );
          progressPtr->setMessage( "Segmentation" );
        }        
        
        SegmenterThreadEntryParams segmenterThreadEntryParams;
        segmenterThreadEntryParams.m_inputParametersPtr = &m_inputParameters;
        segmenterThreadEntryParams.m_outputParametersPtr = outputParamsPtr;
        segmenterThreadEntryParams.m_segsBlocksMatrixPtr = &segmentsblocksMatrix;
        segmenterThreadEntryParams.m_generalMutexPtr = &generalMutex;
        segmenterThreadEntryParams.m_inputRasterIOMutexPtr = 
          &inputRasterIOMutex;
        segmenterThreadEntryParams.m_outputRasterIOMutexPtr = 
          &outputRasterIOMutex;
        segmenterThreadEntryParams.m_blockProcessedSignalMutexPtr = 
          &blockProcessedSignalMutex;
        segmenterThreadEntryParams.m_abortSegmentationFlagPtr = &abortSegmentationFlag;
        segmenterThreadEntryParams.m_segmentsIdsManagerPtr = &segmenterIdsManager;
        segmenterThreadEntryParams.m_blockProcessedSignalPtr = &blockProcessedSignal;
        segmenterThreadEntryParams.m_runningThreadsCounterPtr = 
          &runningThreadsCounter;
        segmenterThreadEntryParams.m_inputRasterGainsPtr = &inputRasterGains;
        segmenterThreadEntryParams.m_inputRasterOffsetsPtr = &inputRasterOffsets;
        segmenterThreadEntryParams.m_enableStrategyProgress =  m_inputParameters.m_enableProgress &&
          ( ( vBlocksNumber * hBlocksNumber ) == 1 );
        segmenterThreadEntryParams.m_progressPtr = maxSegThreads ? 0 : progressPtr.get();
        
        if( maxSegThreads )
        { // threaded segmentation mode
          
          // spawning the segmentation threads
          
          runningThreadsCounter = maxSegThreads;
          
          boost::thread_group threads;
          
          for( unsigned int threadIdx = 0 ; threadIdx < maxSegThreads ;
            ++threadIdx )
          {
            threads.add_thread( new boost::thread( segmenterThreadEntry, 
               &segmenterThreadEntryParams ) );
          };
          
          // waiting all threads to finish
          
          while( (!abortSegmentationFlag) && (runningThreadsCounter > 0 ) )
          {
            boost::unique_lock<boost::mutex> lock( blockProcessedSignalMutex );
            blockProcessedSignal.timed_wait( lock, 
              boost::posix_time::seconds( 1 ) );
              
            if( progressPtr.get() )
            {
              int segmentedBlocksNmb = 0;
              for( unsigned int segmentsMatrixLine = 0 ; segmentsMatrixLine < 
                segmentsblocksMatrix.getLinesNumber() ; ++segmentsMatrixLine )
              {
                for( unsigned int segmentsMatrixCol = 0 ; segmentsMatrixCol < 
                  segmentsblocksMatrix.getColumnsNumber() ; ++segmentsMatrixCol )
                {              
                  if( segmentsblocksMatrix[ segmentsMatrixLine ][ segmentsMatrixCol ].m_status
                    == SegmenterSegmentsBlock::BlockSegmented )
                  {
                    ++segmentedBlocksNmb;
                  }
                }
              }
              
              if( segmentedBlocksNmb != progressPtr->getCurrentStep() )
              {
                progressPtr->pulse();
              }
              
              if( ! progressPtr->isActive() ) 
              {
                abortSegmentationFlag = true;
              }
            }              
              
//            globalMutex.lock();  
//            std::cout << std::endl << "Waiting threads..." << std::endl;
//            globalMutex.unlock();
          }
          
          // joining all threads
          
          threads.join_all();
/*
          globalMutex.lock();
          std::cout << std::endl << "Threads joined." << std::endl;
          globalMutex.unlock();
*/            
        }
        else
        { // non-threaded segmentation mode 
          runningThreadsCounter = 1;
          segmenterThreadEntry( &segmenterThreadEntryParams );
        }
        
        return (!abortSegmentationFlag);
      }
      else
      {
        return false;
      }
    }
    
    void Segmenter::reset() throw( te::rp::Exception )
    {
      m_instanceInitialized = false;
      m_inputParameters.reset();
    }
    
    bool Segmenter::initialize( const AlgorithmInputParameters& inputParams ) 
      throw( te::rp::Exception )
    {
      reset();
      
      Segmenter::InputParameters const* inputParamsPtr = dynamic_cast< 
        Segmenter::InputParameters const* >( &inputParams );
        
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr, "Invalid parameters" );
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inputRasterPtr, 
        "Invalid raster pointer" );
      TERP_TRUE_OR_RETURN_FALSE( 
        inputParamsPtr->m_inputRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );
        
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inputRasterBands.size() > 0,
        "Invalid raster bands number" );
        
      for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx < 
        inputParamsPtr->m_inputRasterBands.size() ; ++inRasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( 
          inputParamsPtr->m_inputRasterBands[ inRasterBandsIdx ] <
          inputParamsPtr->m_inputRasterPtr->getNumberOfBands(), 
          "Invalid raster bands" );
      }
          
      m_inputParameters = *inputParamsPtr;
      m_instanceInitialized = true;
      
      return true;
    }
    
    bool Segmenter::isInitialized() const
    {
      return m_instanceInitialized;
    }    
    
    bool Segmenter::calcBestBlockSize( 
      const unsigned int totalImageLines, 
      const unsigned int totalImageCols, 
      const unsigned int minBlockPixels,
      const unsigned int maxBlockPixels, 
      const double blocksHOverlapSizePercent,
      const double blocksVOverlapSizePectent, 
      unsigned int& blockWidth,
      unsigned int& blockHeight,
      unsigned int& blocksHOverlapSize,
      unsigned int& blocksVOverlapSize ) const
    {
      if( minBlockPixels > maxBlockPixels ) return false;
        
      const double rasterRCFactor = ((double)totalImageLines) /
        ((double)totalImageCols);
        
      const double minRasterLinesNmb = sqrt( ((double)minBlockPixels) * 
        rasterRCFactor );
        
      const double maxScaleFactor = ((double)totalImageLines) / 
        minRasterLinesNmb;
        
      unsigned int rescaledAndExtendedBlockPixelsNmb = 0;
        
      for( double scaleFactor = 1.0 ; scaleFactor <= maxScaleFactor ;
        scaleFactor += 1.0 )
      {
        blockHeight = (unsigned int)std::ceil( ((double)totalImageLines) / scaleFactor );
        blockWidth = (unsigned int)std::ceil( ((double)totalImageCols) / scaleFactor );
        
        blocksHOverlapSize = (unsigned int)( blocksHOverlapSizePercent * ((double)blockWidth) );
        blocksVOverlapSize = (unsigned int)( blocksVOverlapSizePectent * ((double)blockHeight) );
        
        rescaledAndExtendedBlockPixelsNmb = ( blockHeight + blocksVOverlapSize +
          blocksVOverlapSize ) * ( blockWidth + blocksHOverlapSize + blocksHOverlapSize );
          
        if( rescaledAndExtendedBlockPixelsNmb <= maxBlockPixels )
        {
          return true;
        }
      }
      
      return false;
    }    
    
    void Segmenter::segmenterThreadEntry(SegmenterThreadEntryParams* paramsPtr)
    {
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr, "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_inputParametersPtr,
        "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_outputParametersPtr,
        "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_segsBlocksMatrixPtr,
        "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_generalMutexPtr,
        "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_inputRasterIOMutexPtr,
        "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_outputRasterIOMutexPtr,
        "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_outputRasterIOMutexPtr,
        "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_abortSegmentationFlagPtr,
        "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_segmentsIdsManagerPtr,
        "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_blockProcessedSignalPtr,
        "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_runningThreadsCounterPtr,
        "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_inputRasterGainsPtr,
        "Invalid pointer" );     
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_inputRasterOffsetsPtr,
        "Invalid pointer" );         
        
      // Creating the segmentation strategy instance
      
      boost::shared_ptr< SegmenterStrategy > strategyPtr(
        SegmenterStrategyFactory::make( 
        paramsPtr->m_inputParametersPtr->m_strategyName ) );
      TERP_TRUE_OR_THROW( strategyPtr.get(), 
        "Unable to create an segmentation strategy" );   
      if( ! strategyPtr->initialize( 
        paramsPtr->m_inputParametersPtr->getSegStrategyParams() ) )
      {
        paramsPtr->m_generalMutexPtr->lock();
        
        *(paramsPtr->m_runningThreadsCounterPtr) = 
          *(paramsPtr->m_runningThreadsCounterPtr) - 1;
        *(paramsPtr->m_abortSegmentationFlagPtr) = true;
        
//                std::cout << std::endl<< "Thread exit (error)"
//                  << std::endl;                
        
        paramsPtr->m_generalMutexPtr->unlock();

        return;
      }        

      // Looking for a non processed segments block
      
      std::auto_ptr< te::rst::Raster > tempInRasterPtr;
      std::auto_ptr< te::rst::Raster > tempOutRasterPtr;
      
      te::rst::Raster const* currentInRasterPtr = 0;
      te::rst::Raster* currentOutRasterPtr = 0;
      
      std::vector< unsigned int > currentInRasterBands;
      unsigned int currentOutRasterBand = 0;
      
      for( unsigned int sBMLine = 0 ; sBMLine < 
        paramsPtr->m_segsBlocksMatrixPtr->getLinesNumber() ; ++sBMLine )
      {
        for( unsigned int sBMCol = 0 ; sBMCol < 
          paramsPtr->m_segsBlocksMatrixPtr->getColumnsNumber() ;  ++sBMCol )  
        {      
          if( *(paramsPtr->m_abortSegmentationFlagPtr) )
          {
            paramsPtr->m_generalMutexPtr->lock();
            
            *(paramsPtr->m_runningThreadsCounterPtr) = 
              *(paramsPtr->m_runningThreadsCounterPtr) - 1;
              
//            std::cout << std::endl<< "Thread exit (error)"
//              << std::endl;
              
            paramsPtr->m_generalMutexPtr->unlock();
      
            return;
          }
          else
          {
            paramsPtr->m_generalMutexPtr->lock();
            
            SegmenterSegmentsBlock& segsBlk = 
              paramsPtr->m_segsBlocksMatrixPtr->operator()( sBMLine, sBMCol );
            
            if( segsBlk.m_status == SegmenterSegmentsBlock::BlockNotProcessed )
            {
              segsBlk.m_status = SegmenterSegmentsBlock::BlockUnderSegmentation;
/*              
              std::cout << std::endl<< "Start block processing [" + 
                te::common::Convert2String( sBMCol ) + "," +
                te::common::Convert2String( sBMLine ) + "]"
                << std::endl;              
*/              
              paramsPtr->m_generalMutexPtr->unlock();
              
              // Defining the current input and output rasters
              
              if( paramsPtr->m_inputParametersPtr->m_enableBlockProcessing )
              {
                // Defining the current input raster
                
                if( ( currentInRasterPtr == 0 ) ||
                  ( currentInRasterPtr->getNumberOfColumns() != segsBlk.m_width ) ||
                  ( currentInRasterPtr->getNumberOfRows() != segsBlk.m_height ) )
                {
                  paramsPtr->m_inputRasterIOMutexPtr->lock();
                  
                  // Creating a new temporary input raster 
                  
                  std::vector< te::rst::BandProperty* > newInBandProperties;
                  
                  for( unsigned int inRasterBandsIdx = 0; inRasterBandsIdx <
                    paramsPtr->m_inputParametersPtr->m_inputRasterBands.size() ;
                    ++inRasterBandsIdx )
                  {
                    newInBandProperties.push_back( new te::rst::BandProperty(
                      *(paramsPtr->m_inputParametersPtr->m_inputRasterPtr->operator[]( 
                      inRasterBandsIdx ).getProperty() ) ) );
                  }
                  
                  te::rst::Grid const* oldGridPtr =
                    paramsPtr->m_inputParametersPtr->m_inputRasterPtr->getGrid();
                  te::gm::Coord2D newULC( oldGridPtr->gridToGeo( ((double)
                    segsBlk.m_startX) - 0.5, ((double)segsBlk.m_startY )-0.5 ) );
                    
                  std::map< std::string, std::string > rInfo;

                  tempInRasterPtr.reset( 
                    te::rst::RasterFactory::make( "MEM", new te::rst::Grid( 
                    segsBlk.m_width, segsBlk.m_height, 
                    oldGridPtr->getResolutionX(),
                    oldGridPtr->getResolutionY(),
                    &newULC, oldGridPtr->getSRID() ), 
                    newInBandProperties, rInfo ) );
                    
                  if( tempInRasterPtr.get() == 0 )
                  {
                    paramsPtr->m_inputRasterIOMutexPtr->unlock();
                    
                    paramsPtr->m_generalMutexPtr->lock();
                    
                    --( *(paramsPtr->m_runningThreadsCounterPtr) );
                    *(paramsPtr->m_abortSegmentationFlagPtr) = true;
                    
//                    std::cout << std::endl<< "Thread exit (error)"
//                      << std::endl;                    
                    
                    paramsPtr->m_generalMutexPtr->unlock();
                    
                    return;
                  }
                  
                  currentInRasterPtr = tempInRasterPtr.get();
                  
                  currentInRasterBands.clear();
                  
                  for( unsigned int inRasterBandsIdx = 0; inRasterBandsIdx <
                    paramsPtr->m_inputParametersPtr->m_inputRasterBands.size() ;
                    ++inRasterBandsIdx )
                  {         
                    currentInRasterBands.push_back( inRasterBandsIdx );
                  }
                  
                  paramsPtr->m_inputRasterIOMutexPtr->unlock();
                }

                // loading data
                 
                {
                  paramsPtr->m_inputRasterIOMutexPtr->lock();
                  
                  unsigned int blkY = 0;
                  unsigned int blkX = 0;
                  unsigned int rasterY = 0;
                  te::rst::Band const* inBandPtr = 0;
                  te::rst::Band* outBandPtr = 0;
                  double value = 0;
                  
                  for( unsigned int inRasterBandsIdx = 0; inRasterBandsIdx <
                    paramsPtr->m_inputParametersPtr->m_inputRasterBands.size() ;
                    ++inRasterBandsIdx )
                  {         
                    inBandPtr = 
                      paramsPtr->m_inputParametersPtr->m_inputRasterPtr->getBand( 
                      paramsPtr->m_inputParametersPtr->m_inputRasterBands[ 
                      inRasterBandsIdx ] );
                    outBandPtr = 
                      tempInRasterPtr->getBand( inRasterBandsIdx );
                      
                    for( blkY = 0 ; blkY < segsBlk.m_height ; ++blkY )
                    {
                      rasterY = blkY + segsBlk.m_startY;
                      
                      for( blkX = 0 ; blkX < segsBlk.m_width ; ++blkX )
                      {
                        inBandPtr->getValue( blkX + segsBlk.m_startX,
                          rasterY, value );
                        outBandPtr->setValue( blkX, blkY, value );
                      }
                    }
                  }
                  
                  paramsPtr->m_inputRasterIOMutexPtr->unlock();
                }
                
                // Creating a new temporary output raster 
                
                if( ( currentOutRasterPtr == 0 ) ||
                  ( currentOutRasterPtr->getNumberOfColumns() != segsBlk.m_width ) ||
                  ( currentOutRasterPtr->getNumberOfRows() != segsBlk.m_height ) )
                {
                  paramsPtr->m_inputRasterIOMutexPtr->lock();
                  
                  te::rst::BandProperty* newBandPropertyPtr = 
                    new te::rst::BandProperty( 0, te::dt::UINT32_TYPE, "" );
                  newBandPropertyPtr->m_colorInterp = te::rst::GrayIdxCInt;
                  newBandPropertyPtr->m_noDataValue = 0;
                  
                  std::vector< te::rst::BandProperty* > newBandProperties;
                  newBandProperties.push_back( newBandPropertyPtr );
                  
                  te::rst::Grid const* oldGridPtr = 
                    paramsPtr->m_inputParametersPtr->m_inputRasterPtr->getGrid();
                  te::gm::Coord2D newULC( oldGridPtr->gridToGeo( ((double)
                    segsBlk.m_startX) - 0.5, ((double)segsBlk.m_startY )-0.5 ) );
                 
                  std::map< std::string, std::string > rInfo;

                  tempOutRasterPtr.reset( 
                    te::rst::RasterFactory::make( "MEM", new te::rst::Grid( 
                    segsBlk.m_width, segsBlk.m_height,
                    oldGridPtr->getResolutionX(),
                    oldGridPtr->getResolutionY(),
                    &newULC, oldGridPtr->getSRID() ), 
                    newBandProperties, rInfo ) );
                    
                  if( tempOutRasterPtr.get() == 0 )
                  {
                    paramsPtr->m_inputRasterIOMutexPtr->unlock();
                    
                    paramsPtr->m_generalMutexPtr->lock();
                    
                    --( *(paramsPtr->m_runningThreadsCounterPtr) );
                    *(paramsPtr->m_abortSegmentationFlagPtr) = true;
                    
//                    std::cout << std::endl<< "Thread exit (error)"
//                      << std::endl;                    
                    
                    paramsPtr->m_generalMutexPtr->unlock();
                    
                    return;
                  }                    
                  
                  paramsPtr->m_inputRasterIOMutexPtr->unlock();
                  
                  currentOutRasterPtr = tempOutRasterPtr.get();
                  currentOutRasterBand = 0;
                }                
              }
              else
              {
                currentInRasterPtr = paramsPtr->m_inputParametersPtr->m_inputRasterPtr;
                currentOutRasterPtr = paramsPtr->m_outputParametersPtr->m_outputRasterPtr.get();
                currentInRasterBands = paramsPtr->m_inputParametersPtr->m_inputRasterBands;
                currentOutRasterBand = 0;
              }              

              
              // Executing the strategy
              
              if( ! strategyPtr->execute( 
                *paramsPtr->m_segmentsIdsManagerPtr,
                *currentInRasterPtr, currentInRasterBands, 
                *paramsPtr->m_inputRasterGainsPtr,
                *paramsPtr->m_inputRasterOffsetsPtr,
                *currentOutRasterPtr, currentOutRasterBand,
                paramsPtr->m_enableStrategyProgress ) )
              {
                paramsPtr->m_generalMutexPtr->lock();
                
                *(paramsPtr->m_runningThreadsCounterPtr) = 
                  *(paramsPtr->m_runningThreadsCounterPtr) - 1;
                *(paramsPtr->m_abortSegmentationFlagPtr) = true;
                
//                std::cout << std::endl<< "Thread exit (error)"
//                  << std::endl;                
                
                paramsPtr->m_generalMutexPtr->unlock();
                
                return;
              }
              
              // flushing data if needed
              
              if( paramsPtr->m_inputParametersPtr->m_enableBlockProcessing )
              { 
                paramsPtr->m_outputRasterIOMutexPtr->lock();
                
                unsigned int blkY = 0;
                unsigned int blkX = 0;
                unsigned int rasterY = 0;
                te::rst::Band& inBand =  *( tempOutRasterPtr->getBand(0) );
                te::rst::Band& outBand = 
                  *( paramsPtr->m_outputParametersPtr->m_outputRasterPtr->getBand( 
                  0 ) );
                double value = 0;
                
                if( paramsPtr->m_inputParametersPtr->m_enableBlockMerging )
                {
                  const bool hasTopCutOffProfile = 
                    ! segsBlk.m_topCutOffProfile.empty();
                  const bool hasBottomCutOffProfile = 
                    ! segsBlk.m_bottomCutOffProfile.empty();
                  const bool hasLeftCutOffProfile = 
                    ! segsBlk.m_leftCutOffProfile.empty();
                  const bool hasRightCutOffProfile = 
                    ! segsBlk.m_rightCutOffProfile.empty();
                  bool isDummyPixel = false;
                    
                  
                  for( blkY = 0 ; blkY < segsBlk.m_height ; ++blkY )
                  {
                    rasterY = blkY + segsBlk.m_startY;
                    
                    for( blkX = 0 ; blkX < segsBlk.m_width ; ++blkX )
                    {
                      isDummyPixel = false;
                      
                      if( hasTopCutOffProfile )
                      {
                        if( segsBlk.m_topCutOffProfile[ blkX ] > blkY )
                        {
                          isDummyPixel = true;
                        }
                      }                    
                      if( hasBottomCutOffProfile )
                      {
                        if( segsBlk.m_bottomCutOffProfile[ blkX ] < blkY )
                        {
                          isDummyPixel = true;
                        }
                      }      
                      if( hasLeftCutOffProfile )
                      {
                        if( segsBlk.m_leftCutOffProfile[ blkY ] > blkX )
                        {
                          isDummyPixel = true;
                        }
                      }     
                      if( hasRightCutOffProfile )
                      {
                        if( segsBlk.m_rightCutOffProfile[ blkY ] < blkX )
                        {
                          isDummyPixel = true;
                        }
                      }                      
                      
                      if( !isDummyPixel )
                      {
                        inBand.getValue( blkX, blkY, value );
                        outBand.setValue( blkX + segsBlk.m_startX,
                          rasterY, value );
                      }
                    }
                  }
                }
                else
                {
                  for( blkY = 0 ; blkY < segsBlk.m_height ; ++blkY )
                  {
                    rasterY = blkY + segsBlk.m_startY;
                    
                    for( blkX = 0 ; blkX < segsBlk.m_width ; ++blkX )
                    {
                      inBand.getValue( blkX, blkY, value );
                      outBand.setValue( blkX + segsBlk.m_startX,
                        rasterY, value );
                    }
                  }
                }
                
                paramsPtr->m_outputRasterIOMutexPtr->unlock();
              }
              
              // updating block status
              
              paramsPtr->m_generalMutexPtr->lock();
              
              segsBlk.m_status = SegmenterSegmentsBlock::BlockSegmented;
/*              
              std::cout << std::endl<< "Block processed [" + 
                te::common::Convert2String( sBMCol ) + "," +
                te::common::Convert2String( sBMLine ) + "]"
                << std::endl;
*/              
              paramsPtr->m_generalMutexPtr->unlock();
              
              // pulsing the progress
              
              if( paramsPtr->m_progressPtr )
              {
                paramsPtr->m_generalMutexPtr->lock();
                
                if( paramsPtr->m_progressPtr->isActive() )
                {
                  paramsPtr->m_progressPtr->pulse();
                  paramsPtr->m_generalMutexPtr->unlock();
                }
                else
                {
                  --( *(paramsPtr->m_runningThreadsCounterPtr) );
                  *(paramsPtr->m_abortSegmentationFlagPtr) = true;
                  
//                    std::cout << std::endl<< "Thread exit (error)"
//                      << std::endl;                    
                  
                  paramsPtr->m_generalMutexPtr->unlock();
                  
                  return;
                }
              }              
              
              // notifying the main thread with the block processed signal
              
              boost::lock_guard<boost::mutex> blockProcessedSignalLockGuard( 
                *( paramsPtr->m_blockProcessedSignalMutexPtr) );              
              
              paramsPtr->m_blockProcessedSignalPtr->notify_one();             

            }
            else
            {
              paramsPtr->m_generalMutexPtr->unlock();
            }
          }
        }
      }
      
      // ending tasks
      
      paramsPtr->m_generalMutexPtr->lock();
       
      *(paramsPtr->m_runningThreadsCounterPtr) = 
        *(paramsPtr->m_runningThreadsCounterPtr) - 1;
        
//      std::cout << std::endl<< "Thread exit (OK)"
//        << std::endl;        
     
      paramsPtr->m_generalMutexPtr->unlock();
    }
    
    bool Segmenter::genImageHCutOffProfile( const unsigned int profileCenter,
      const te::rst::Raster& inRaster,
      const std::vector< unsigned int >& inRasterBands,
      const unsigned int pixelNeighborhoodSize,
      const unsigned int tileNeighborhoodSize,
      const unsigned int profileAntiSmoothingFactor,
      std::vector< unsigned int>& profile ) const
    {
      TERP_DEBUG_TRUE_OR_THROW( profileAntiSmoothingFactor,
        "Invalid parameter" )
        
      profile.clear();
      
      if( tileNeighborhoodSize < pixelNeighborhoodSize ) return false;
      
      const int inRasterRowsNmb = (int)inRaster.getNumberOfRows();
      const int inRasterColsNmb = (int)inRaster.getNumberOfColumns();
      const int inRasterBandsSize = (int)inRasterBands.size();
      
      // Calculating the tiles buffer dimension
      
      const int tilesBufferStartIdx = MAX( 0, MIN( inRasterRowsNmb - 1, 
        ((int)profileCenter) - ((int)tileNeighborhoodSize) ) );
      const int tilesBufferBoundIdx = MAX( 0, MIN( inRasterRowsNmb, 
        1 + ((int)profileCenter) + ((int)tileNeighborhoodSize) ) );
      const int tilesBufferSize = tilesBufferBoundIdx - tilesBufferStartIdx;
      if( tilesBufferSize < ( 1 + ( 2 * ( (int)( pixelNeighborhoodSize ) ) ) ) ) 
        return false;
      
      const int minCrossProfileStartIdx = tilesBufferStartIdx + 
        (int)pixelNeighborhoodSize;
      const int maxCrossProfileBoundIdx = tilesBufferBoundIdx - 
        (int)pixelNeighborhoodSize;        
      
      int crossProfileIdx = 0;
      int crossProfileStartIdx = 0;
      int crossProfileBoundIdx = 0;
      
      int pixelNBStartIdx = 0;
      int pixelNBEndIdx = 0;
      int pixel1NBIdx = 0;
      int pixel2NBIdx = 0;
      int pixelNBOffset = 0;
      
      double diffSum = 0;
      double currBandDiffSum = 0;
      double higherDiffSum = 0;
      int higherDiffSumIdx = 0;
      
      int inRasterBandsIdx = 0;
      unsigned int bandIdx = 0;
      
      double pixel1Value = 0;
      double pixel2Value = 0;
      
      profile.resize( inRasterColsNmb, 0 );
      
      for( int profileElementIdx = 0 ; profileElementIdx < inRasterColsNmb ;
        ++profileElementIdx )
      {
        if( profileElementIdx  )
        {
          crossProfileStartIdx = profile[ profileElementIdx - 1 ] -
            (int)profileAntiSmoothingFactor;
          crossProfileStartIdx = MAX( crossProfileStartIdx, 
            minCrossProfileStartIdx );
            
          crossProfileBoundIdx = profile[ profileElementIdx - 1 ] + 1 +
            ((int)profileAntiSmoothingFactor);
          crossProfileBoundIdx = MIN( crossProfileBoundIdx, 
            maxCrossProfileBoundIdx );
        }
        else
        {
          crossProfileStartIdx = minCrossProfileStartIdx;
          crossProfileBoundIdx = maxCrossProfileBoundIdx;
        }
        
        higherDiffSum = 0;
        higherDiffSumIdx = crossProfileStartIdx;        
          
        for( crossProfileIdx = crossProfileStartIdx ; crossProfileIdx <
          crossProfileBoundIdx ; ++crossProfileIdx )
        {
          // look for the higher diff using all bands
          // within the defined neighborhood
          
          diffSum = 0;
          
          for( inRasterBandsIdx = 0 ; inRasterBandsIdx < inRasterBandsSize ;
            ++inRasterBandsIdx )
          {
            bandIdx = inRasterBands[ inRasterBandsIdx ];
            TERP_DEBUG_TRUE_OR_THROW( bandIdx < inRaster.getNumberOfBands(),
              "Invalid band" )          
          
            pixelNBStartIdx = crossProfileIdx - ( (int)pixelNeighborhoodSize);
            pixelNBEndIdx = crossProfileIdx + ( (int)pixelNeighborhoodSize);        
            currBandDiffSum = 0;
            
            for( pixelNBOffset = 0 ; pixelNBOffset < ((int)pixelNeighborhoodSize) ; 
              ++pixelNBOffset )
            {
              pixel1NBIdx = pixelNBStartIdx + pixelNBOffset;
              pixel2NBIdx = pixelNBEndIdx - pixelNBOffset;
              
              TERP_DEBUG_TRUE_OR_THROW( ( pixel1NBIdx >= 0 ) &&
                ( pixel1NBIdx < inRasterRowsNmb ), "Invalid pixel2Idx" )            
              TERP_DEBUG_TRUE_OR_THROW( ( pixel2NBIdx >= 0 ) &&
                ( pixel2NBIdx < inRasterRowsNmb ), "Invalid pixel2Idx" )            
              
              inRaster.getValue( profileElementIdx,
                pixel1NBIdx, pixel1Value, bandIdx );
              inRaster.getValue( profileElementIdx,
                pixel2NBIdx, pixel2Value, bandIdx );
                  
              currBandDiffSum += ABS( pixel1Value - pixel2Value );
            }
            
            diffSum += ( currBandDiffSum / ((double)( pixelNBEndIdx -
              pixelNBStartIdx + 1 ) ) );
          }
          
          if( diffSum > higherDiffSum )
          {
            higherDiffSum = diffSum;
            higherDiffSumIdx = crossProfileIdx;
          }            
        }
        
        profile[ profileElementIdx ] = higherDiffSumIdx;                  
      }
    
      return true;
    }
        
    bool Segmenter::genImageVCutOffProfile( const unsigned int profileCenter,
      const te::rst::Raster& inRaster,
      const std::vector< unsigned int >& inRasterBands,
      const unsigned int pixelNeighborhoodSize,
      const unsigned int tileNeighborhoodSize,
      const unsigned int profileAntiSmoothingFactor,
      std::vector< unsigned int>& profile ) const
    {
      TERP_DEBUG_TRUE_OR_THROW( profileAntiSmoothingFactor,
        "Invalid parameter" )
        
      profile.clear();
      
      if( tileNeighborhoodSize < pixelNeighborhoodSize ) return false;
      
      const int inRasterRowsNmb = (int)inRaster.getNumberOfRows();
      const int inRasterColsNmb = (int)inRaster.getNumberOfColumns();
      const int inRasterBandsSize = (int)inRasterBands.size();
      
      // Calculating the tiles buffer dimension
      
      const int tilesBufferStartIdx = MAX( 0, MIN( inRasterColsNmb - 1, 
        ((int)profileCenter) - ((int)tileNeighborhoodSize) ) );
      const int tilesBufferBoundIdx = MAX( 0, MIN( inRasterColsNmb, 
        1 + ((int)profileCenter) + ((int)tileNeighborhoodSize) ) );
      const int tilesBufferSize = tilesBufferBoundIdx - tilesBufferStartIdx;
      if( tilesBufferSize < ( 1 + ( 2 * ( (int)( pixelNeighborhoodSize ) ) ) ) ) 
        return false;
      
      const int minCrossProfileStartIdx = tilesBufferStartIdx + 
        (int)pixelNeighborhoodSize;
      const int maxCrossProfileBoundIdx = tilesBufferBoundIdx - 
        (int)pixelNeighborhoodSize;        
      
      int crossProfileIdx = 0;
      int crossProfileStartIdx = 0;
      int crossProfileBoundIdx = 0;
      
      int pixelNBStartIdx = 0;
      int pixelNBEndIdx = 0;
      int pixel1NBIdx = 0;
      int pixel2NBIdx = 0;
      int pixelNBOffset = 0;
      
      double diffSum = 0;
      double currBandDiffSum = 0;
      double higherDiffSum = 0;
      int higherDiffSumIdx = 0;
      
      int inRasterBandsIdx = 0;
      unsigned int bandIdx = 0;
      
      double pixel1Value = 0;
      double pixel2Value = 0;
      
      profile.resize( inRasterRowsNmb, 0 );
      
      for( int profileElementIdx = 0 ; profileElementIdx < inRasterRowsNmb ;
        ++profileElementIdx )
      {
        if( profileElementIdx  )
        {
          crossProfileStartIdx = profile[ profileElementIdx - 1 ] -
            (int)profileAntiSmoothingFactor;
          crossProfileStartIdx = MAX( crossProfileStartIdx, 
            minCrossProfileStartIdx );
            
          crossProfileBoundIdx = profile[ profileElementIdx - 1 ] + 1 +
            ((int)profileAntiSmoothingFactor);
          crossProfileBoundIdx = MIN( crossProfileBoundIdx, 
            maxCrossProfileBoundIdx );
        }
        else
        {
          crossProfileStartIdx = minCrossProfileStartIdx;
          crossProfileBoundIdx = maxCrossProfileBoundIdx;
        }
        
        higherDiffSum = 0;
        higherDiffSumIdx = crossProfileStartIdx;        
          
        for( crossProfileIdx = crossProfileStartIdx ; crossProfileIdx <
          crossProfileBoundIdx ; ++crossProfileIdx )
        {
          // look for the higher diff using all bands
          // within the defined neighborhood
          
          diffSum = 0;
          
          for( inRasterBandsIdx = 0 ; inRasterBandsIdx < inRasterBandsSize ;
            ++inRasterBandsIdx )
          {
            bandIdx = inRasterBands[ inRasterBandsIdx ];
            TERP_DEBUG_TRUE_OR_THROW( bandIdx < inRaster.getNumberOfBands(),
              "Invalid band" )          
          
            pixelNBStartIdx = crossProfileIdx - ( (int)pixelNeighborhoodSize);
            pixelNBEndIdx = crossProfileIdx + ( (int)pixelNeighborhoodSize);        
            currBandDiffSum = 0;
            
            for( pixelNBOffset = 0 ; pixelNBOffset < ((int)pixelNeighborhoodSize) ; 
              ++pixelNBOffset )
            {
              pixel1NBIdx = pixelNBStartIdx + pixelNBOffset;
              pixel2NBIdx = pixelNBEndIdx - pixelNBOffset;
              
              TERP_DEBUG_TRUE_OR_THROW( ( pixel1NBIdx >= 0 ) &&
                ( pixel1NBIdx < inRasterColsNmb ), "Invalid pixel2Idx" )            
              TERP_DEBUG_TRUE_OR_THROW( ( pixel2NBIdx >= 0 ) &&
                ( pixel2NBIdx < inRasterColsNmb ), "Invalid pixel2Idx" )            
              
              inRaster.getValue( pixel1NBIdx, profileElementIdx,
                pixel1Value, bandIdx );
              inRaster.getValue( pixel2NBIdx, profileElementIdx,
                pixel2Value, bandIdx );
                  
              currBandDiffSum += ABS( pixel1Value - pixel2Value );
            }
            
            diffSum += ( currBandDiffSum / ((double)( pixelNBEndIdx -
              pixelNBStartIdx + 1 ) ) );
          }
          
          if( diffSum > higherDiffSum )
          {
            higherDiffSum = diffSum;
            higherDiffSumIdx = crossProfileIdx;
          }            
        }
        
        profile[ profileElementIdx ] = higherDiffSumIdx;                  
      }
    
      return true;
    }
    
    bool Segmenter::updateBlockCutOffProfiles( 
      const std::vector< std::vector< unsigned int> >& imageHorizontalProfiles,
      const std::vector< std::vector< unsigned int> >& imageVerticalProfiles,
      SegmenterSegmentsBlock& segmentsBlock ) const
    {
      // generating m_topCutOffProfile
      
      segmentsBlock.m_topCutOffProfile.clear();      
      
      if( ( segmentsBlock.m_segmentsMatrixYIndex > 0 ) &&
        ( segmentsBlock.m_segmentsMatrixYIndex <= 
        imageHorizontalProfiles.size() ) )
      {
        const std::vector< unsigned int >& profile = imageHorizontalProfiles[ 
          segmentsBlock.m_segmentsMatrixYIndex - 1 ];
          
        if( ! profile.empty() )
        {
          segmentsBlock.m_topCutOffProfile.resize( segmentsBlock.m_width );      
          int profElementValue = 0;
          
          for( unsigned int profEIdx = 0 ; profEIdx < segmentsBlock.m_width ;
            ++profEIdx )
          {
            TERP_DEBUG_TRUE_OR_THROW( profEIdx + segmentsBlock.m_startX <
              profile.size(), "Internal error" )
            profElementValue = 
              ((int)profile[ profEIdx + segmentsBlock.m_startX ]) -
              ((int)segmentsBlock.m_startY);
            profElementValue = MAX( 0, profElementValue );
            profElementValue = MIN( (int)segmentsBlock.m_height, profElementValue );
            
            segmentsBlock.m_topCutOffProfile[ profEIdx ] = profElementValue;          
          }
        }
      }
      
      // generating m_bottomCutOffProfile
      
      segmentsBlock.m_bottomCutOffProfile.clear();            
      
      if( segmentsBlock.m_segmentsMatrixYIndex < 
        imageHorizontalProfiles.size() )
      {
        const std::vector< unsigned int >& profile = imageHorizontalProfiles[ 
          segmentsBlock.m_segmentsMatrixYIndex ];
          
        if( ! profile.empty() )
        {
          segmentsBlock.m_bottomCutOffProfile.resize( segmentsBlock.m_width );            
          int profElementValue = 0;
          
          for( unsigned int profEIdx = 0 ; profEIdx < segmentsBlock.m_width ;
            ++profEIdx )
          {
            TERP_DEBUG_TRUE_OR_THROW( profEIdx + segmentsBlock.m_startX <
              profile.size(), "Internal error" )            
            profElementValue = 
              ((int)profile[ profEIdx + segmentsBlock.m_startX ]) -
              ((int)segmentsBlock.m_startY);
            profElementValue = MAX( 0, profElementValue );
            profElementValue = MIN( (int)segmentsBlock.m_height, profElementValue );
            
            segmentsBlock.m_bottomCutOffProfile[ profEIdx ] = profElementValue;          
          }
        }
      }      
      
      // generating m_leftCutOffProfile
      
      segmentsBlock.m_leftCutOffProfile.clear();   
      
      if( ( segmentsBlock.m_segmentsMatrixXIndex > 0 ) &&
        ( segmentsBlock.m_segmentsMatrixXIndex <=
        imageVerticalProfiles.size() ) )
      {
        const std::vector< unsigned int >& profile = imageVerticalProfiles[ 
          segmentsBlock.m_segmentsMatrixXIndex - 1 ];
          
        if( ! profile.empty() )
        {
          segmentsBlock.m_leftCutOffProfile.resize( segmentsBlock.m_height );      
          int profElementValue = 0;
          
          for( unsigned int profEIdx = 0 ; profEIdx < segmentsBlock.m_height ;
            ++profEIdx )
          {
            TERP_DEBUG_TRUE_OR_THROW( profEIdx + segmentsBlock.m_startY <
              profile.size(), "Internal error" )            
            profElementValue = 
              ((int)profile[ profEIdx + segmentsBlock.m_startY ]) -
              ((int)segmentsBlock.m_startX);
            profElementValue = MAX( 0, profElementValue );
            profElementValue = MIN( (int)segmentsBlock.m_width, profElementValue );            
            
            segmentsBlock.m_leftCutOffProfile[ profEIdx ] = profElementValue;          
          }
        }
      }      
      
      // generating m_rightCutOffProfile
      
      segmentsBlock.m_rightCutOffProfile.clear();      
      
      if( segmentsBlock.m_segmentsMatrixXIndex <
        imageVerticalProfiles.size() )
      {
        const std::vector< unsigned int >& profile = imageVerticalProfiles[ 
          segmentsBlock.m_segmentsMatrixXIndex  ];
          
        if( ! profile.empty() )
        {
          segmentsBlock.m_rightCutOffProfile.resize( segmentsBlock.m_height );      
          int profElementValue = 0;
          
          for( unsigned int profEIdx = 0 ; profEIdx < segmentsBlock.m_height ;
            ++profEIdx )
          {
            TERP_DEBUG_TRUE_OR_THROW( profEIdx + segmentsBlock.m_startY <
              profile.size(), "Internal error" )            
            profElementValue = 
              ((int)profile[ profEIdx + segmentsBlock.m_startY ]) -
              ((int)segmentsBlock.m_startX);
            profElementValue = MAX( 0, profElementValue );
            profElementValue = MIN( (int)segmentsBlock.m_width, profElementValue );
            
            segmentsBlock.m_rightCutOffProfile[ profEIdx ] = profElementValue;          
          }
        }
      }        
      
      return true;
    }
    
    bool Segmenter::createCutOffLinesTiff(  
      const std::vector< std::vector< unsigned int> >& imageHorizontalProfiles,
      const std::vector< unsigned int >& imageHorizontalProfilesCenterLines,
      const std::vector< std::vector< unsigned int> >& imageVerticalProfiles,
      const std::vector< unsigned int >& imageVerticalProfilesCenterLines,
      const std::string& filename )
    {
      TERP_TRUE_OR_THROW( imageHorizontalProfiles.size() ==
        imageHorizontalProfilesCenterLines.size(), "Internal error" )
      TERP_TRUE_OR_THROW( imageVerticalProfiles.size() ==
        imageVerticalProfilesCenterLines.size(), "Internal error" )
        
      std::vector< te::rst::BandProperty* > bandsProperties;
      bandsProperties.push_back( new te::rst::BandProperty(
        *( m_inputParameters.m_inputRasterPtr->getBand( 
        m_inputParameters.m_inputRasterBands[ 0 ] )->getProperty() ) ) );  
      bandsProperties[ 0 ]->m_colorInterp = te::rst::GrayIdxCInt;
      bandsProperties[ 0 ]->m_noDataValue = 0;
      bandsProperties[ 0 ]->m_type = te::dt::UCHAR_TYPE;
      //bandsProperties[ 0 ]->m_noDataValue = 0;
      
      std::map< std::string, std::string > rInfo;
      rInfo[ "URI" ] = "cutofflines.tif";

      std::auto_ptr< te::rst::Raster > outRasterPtr(
        te::rst::RasterFactory::make( "GDAL",
        new te::rst::Grid( *m_inputParameters.m_inputRasterPtr->getGrid() ),
        bandsProperties, rInfo ) );
      TERP_TRUE_OR_RETURN_FALSE( outRasterPtr.get(),  
        "Tiff creation error" )
        
      {
        for( unsigned int profIdx = 0 ; profIdx < imageHorizontalProfiles.size() ;
          ++profIdx )
        {
          const std::vector< unsigned int>& profile =
            imageHorizontalProfiles[ profIdx ];
          TERP_TRUE_OR_THROW( profile.size() == 
            outRasterPtr->getGrid()->getNumberOfColumns(), "Internal error" )          
            
          const unsigned int centerLineIdx  = 
            imageHorizontalProfilesCenterLines[ profIdx ];
          TERP_TRUE_OR_THROW( centerLineIdx <
            outRasterPtr->getGrid()->getNumberOfRows(), "Internal error" )          
            
          
          for( unsigned int eIdx = 0 ; eIdx < profile.size() ;
            ++eIdx )
          {        
            TERP_TRUE_OR_THROW( profile[ eIdx ] <
              outRasterPtr->getGrid()->getNumberOfRows(), "Internal error" )  
            
            outRasterPtr->setValue( eIdx, centerLineIdx, 255, 0 );
            outRasterPtr->setValue( eIdx, profile[ eIdx ] , 255, 0 );
          }
        }
      }
      
      {
        for( unsigned int profIdx = 0 ; profIdx < imageVerticalProfiles.size() ;
          ++profIdx )
        {
          const std::vector< unsigned int>& profile =
            imageVerticalProfiles[ profIdx ];
          TERP_TRUE_OR_THROW( profile.size() == 
            outRasterPtr->getGrid()->getNumberOfRows(), "Internal error" )              
            
          const unsigned int centerLineIdx  = 
            imageVerticalProfilesCenterLines[ profIdx ];
          TERP_TRUE_OR_THROW( centerLineIdx <
            outRasterPtr->getGrid()->getNumberOfColumns(), "Internal error" )             
          
          for( unsigned int eIdx = 0 ; eIdx < profile.size() ;
            ++eIdx )
          {        
            TERP_TRUE_OR_THROW( profile[ eIdx ] <
              outRasterPtr->getGrid()->getNumberOfRows(), "Internal error" )             

            outRasterPtr->setValue( centerLineIdx, eIdx, 255, 0 );
            outRasterPtr->setValue( profile[ eIdx ], eIdx, 255, 0 );
          }
        }
      }
      
      return true;
    }
  } // end namespace rp
}   // end namespace te    

