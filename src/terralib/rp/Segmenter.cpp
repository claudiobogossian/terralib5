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
#include "../raster/Utils.h"
#include "../raster/SynchronizedRaster.h"
#include "../memory/CachedRaster.h"
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
      m_inputRasterNoDataValues.clear();
      m_enableThreadedProcessing = true;
      m_maxSegThreads = 0;
      m_enableBlockProcessing = true;
      m_enableBlockMerging = true;
      m_maxBlockSize = 0;
      m_strategyName.clear();
      m_enableProgress = false;
      m_enableRasterCache = true;
      
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
      m_inputRasterNoDataValues = params.m_inputRasterNoDataValues;
      m_enableThreadedProcessing = params.m_enableThreadedProcessing;
      m_maxSegThreads = params.m_maxSegThreads;
      m_enableBlockProcessing = params.m_enableBlockProcessing;
      m_enableBlockMerging = params.m_enableBlockMerging;
      m_maxBlockSize = params.m_maxBlockSize;
      m_strategyName = params.m_strategyName;
      m_enableProgress = params.m_enableProgress;
      m_enableRasterCache = params.m_enableRasterCache;
      
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
      m_inputParameters.reset();
      m_outputParametersPtr = 0;
      m_segsBlocksMatrixPtr = 0;
      m_generalMutexPtr = 0;
      m_inputRasterSyncPtr = 0;
      m_outputRasterSyncPtr = 0;
      m_blockProcessedSignalMutexPtr = 0;
      m_abortSegmentationFlagPtr = 0;
      m_segmentsIdsManagerPtr = 0;
      m_blockProcessedSignalPtr = 0;
      m_runningThreadsCounterPtr = 0;
      m_inputRasterBandMinValues.clear();
      m_inputRasterBandMaxValues.clear();
      m_inputRasterNoDataValues.clear();
      m_enableStrategyProgress = false;
      m_maxInputRasterCachedBlocks = 0;
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
          
          // Fill with zeroes
          
          te::rst::Raster& outRaster = (*outputParamsPtr->m_outputRasterPtr);
          te::rst::Band& outBand = (*outRaster.getBand( 0 ));
          const unsigned int nRows =outRaster.getNumberOfRows();
          const unsigned int nCols = outRaster.getNumberOfColumns();
          unsigned int row = 0;
          unsigned int col = 0;
          
          for( row = 0 ; row < nRows ; ++row )
          {
            for( col = 0 ; col < nCols ; ++col )
            {
              outBand.setValue( col, row, 0.0 );
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
        
        // Guessing memory limits
        
        const unsigned int totalRasterPixels = 
          m_inputParameters.m_inputRasterPtr->getNumberOfRows() * 
          m_inputParameters.m_inputRasterPtr->getNumberOfColumns();
        const double originalRasterDataMemUsageEstimation = (double)(
          totalRasterPixels *
          ((unsigned int)m_inputParameters.m_inputRasterBands.size()) *
          ( (unsigned int)te::rst::GetPixelSize( m_inputParameters.m_inputRasterPtr->getBandDataType( 0 ) ) ) );          
        const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
        const double usedVMem = (double)te::common::GetUsedVirtualMemory();
        const double totalVMem = ( (double)te::common::GetTotalVirtualMemory() );
        const double freeVMem = MIN( totalPhysMem, ( totalVMem - usedVMem ) );
        const double pixelRequiredRam = 
          ( originalRasterDataMemUsageEstimation + stratMemUsageEstimation )
          / ((double)totalRasterPixels);
        const double maxSimultaneousMemoryPixels = 0.7 * MIN( 
          ((double)totalRasterPixels), 
          freeVMem / pixelRequiredRam );         
        
        // Cache issues
        
        std::auto_ptr< te::rst::Raster > cachedRasterHandler;
        te::rst::Raster const * cachedRasterPtr = m_inputParameters.m_inputRasterPtr;
        
        if( m_inputParameters.m_enableRasterCache )
        {
          cachedRasterHandler.reset( new te::mem::CachedRaster( 
            *m_inputParameters.m_inputRasterPtr, 40, 0 ) );
          cachedRasterPtr = cachedRasterHandler.get();
        }        
        
        // Finding the input raster normalization parameters
        
        std::vector< double > inputRasterBandMinValues( 
          m_inputParameters.m_inputRasterBands.size(), 0.0 );
        std::vector< double > inputRasterBandMaxValues( 
          m_inputParameters.m_inputRasterBands.size(), 0.0 );
          
        {
          const unsigned int nRows = 
            cachedRasterPtr->getNumberOfRows();
          const unsigned int nCols = 
            cachedRasterPtr->getNumberOfColumns();
          unsigned int row = 0;
          unsigned int col = 0;
          double bandMin = DBL_MAX;
          double bandMax = -1.0 * DBL_MAX;
          double value = 0;
          
          std::vector< double > noDataValues;
          if( m_inputParameters.m_inputRasterNoDataValues.empty() )
          {
            for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx < 
              m_inputParameters.m_inputRasterBands.size() ; ++inputRasterBandsIdx )
            {
              noDataValues.push_back(
                m_inputParameters.m_inputRasterPtr->getBand(
                m_inputParameters.m_inputRasterBands[ 
                inputRasterBandsIdx ] )->getProperty()->m_noDataValue );
            }
          }
          else
          {
            noDataValues = m_inputParameters.m_inputRasterNoDataValues;
          }           
          
          for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx <
            m_inputParameters.m_inputRasterBands.size() ; ++inputRasterBandsIdx )
          {
            const te::rst::Band& band = 
              *(cachedRasterPtr->getBand( 
              m_inputParameters.m_inputRasterBands[ inputRasterBandsIdx ] ) );
            bandMin = DBL_MAX;
            bandMax = -1.0 * DBL_MAX;
            
            for( row = 0 ; row < nRows ; ++row )
            {
              for( col = 0 ; col < nCols ; ++col )
              {
                band.getValue( col, row, value );
                
                if( value != noDataValues[ inputRasterBandsIdx ] )
                {
                  if( bandMin > value ) bandMin = value;
                  if( bandMax < value ) bandMax = value;
                }
              }
            }
            
            inputRasterBandMinValues[ inputRasterBandsIdx ] = bandMin;
            inputRasterBandMaxValues[ inputRasterBandsIdx ] = bandMax;
          }
        }
        
        // defining the number of processing threads
        
        unsigned int maxSegThreads = 0;
        
        if( m_inputParameters.m_enableBlockProcessing && 
          m_inputParameters.m_enableThreadedProcessing )
        {
          if( m_inputParameters.m_maxSegThreads )
          {
            maxSegThreads = m_inputParameters.m_maxSegThreads;
          }
          else
          {
            maxSegThreads = te::common::GetPhysProcNumber();
            if( maxSegThreads == 1 )
            {
              maxSegThreads = 0;
            }
          }
        }        
        
        // Calc the maximum block width & height
        
        unsigned int maxNonExpandedBlockWidth = 0;
        unsigned int maxNonExpandedBlockHeight = 0;
        unsigned int maxExpandedBlockWidth = 0;
        unsigned int maxExpandedBlockHeight = 0;
        
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
              cachedRasterPtr->getNumberOfRows(),
              cachedRasterPtr->getNumberOfColumns(), 
              ( 
                ( stratBlocksOverlapSize + stratBlocksOverlapSize + 1 ) 
                *
                ( stratBlocksOverlapSize + stratBlocksOverlapSize + 1 )
              ),
              maxBlockPixels, 
              stratBlocksOverlapSize,
              stratBlocksOverlapSize,
              maxNonExpandedBlockWidth, 
              maxNonExpandedBlockHeight ), 
              "Error calculating best block size" );   
            
            maxExpandedBlockWidth = maxNonExpandedBlockWidth + 
              stratBlocksOverlapSize + stratBlocksOverlapSize;
            maxExpandedBlockHeight = maxNonExpandedBlockHeight +
              stratBlocksOverlapSize + stratBlocksOverlapSize;
          }
          else
          {
            // Adjusting the block sizes
           
            TERP_TRUE_OR_RETURN_FALSE( calcBestBlockSize( 
              cachedRasterPtr->getNumberOfRows(),
              cachedRasterPtr->getNumberOfColumns(), 
              stratBlocksOverlapSize * stratBlocksOverlapSize,
              maxBlockPixels, 
              0,
              0,
              maxNonExpandedBlockWidth, 
              maxNonExpandedBlockHeight ), 
              "Error calculating best block size" );  
              
            maxExpandedBlockWidth = maxNonExpandedBlockWidth;
            maxExpandedBlockHeight = maxNonExpandedBlockHeight;
          }
        }
        else
        {
          maxNonExpandedBlockWidth = maxExpandedBlockWidth =
            cachedRasterPtr->getNumberOfColumns();
          maxNonExpandedBlockHeight = maxExpandedBlockHeight =
            cachedRasterPtr->getNumberOfRows();
        }
        
        const unsigned int blocksHOverlapSize = ( maxExpandedBlockWidth -
           maxNonExpandedBlockWidth );
        const unsigned int blocksVOverlapSize = ( maxExpandedBlockHeight -
           maxNonExpandedBlockHeight );         

        // Defining number of blocks
          
        const unsigned int hBlocksNumber = (unsigned int)ceil( 
          ((double)cachedRasterPtr->getNumberOfColumns()) / 
          ((double)maxNonExpandedBlockWidth) );
        const unsigned int vBlocksNumber = (unsigned int)ceil( 
          ((double)cachedRasterPtr->getNumberOfRows()) / 
          ((double)maxNonExpandedBlockHeight ) );        

        // Generating cut off profiles. When possible, an empty profile
        // vector is generated
        
        std::vector< std::vector< unsigned int> > imageHorizontalProfiles;
        std::vector< std::vector< unsigned int> > imageVerticalProfiles;
        
        if( m_inputParameters.m_enableBlockProcessing &&
          m_inputParameters.m_enableBlockMerging )
        {
//          std::cout << std::endl << "Starting CutOff profiles generation" << std::endl;
          
          const unsigned int verticalProfilesNeighborhoodSize = blocksHOverlapSize / 2;
          const unsigned int horizontalProfilesNeighborhoodSize =  blocksVOverlapSize / 2;
          const unsigned int pixelNeighborhoodSize = 5;
          const unsigned int profileAntiSmoothingFactor = 3;            
          std::vector< unsigned int > imageHorizontalProfilesCenterLines;
          std::vector< unsigned int > imageVerticalProfilesCenterLines;
          std::vector< unsigned int> profile;
          unsigned int profileIdx = 0;
          
          for( profileIdx = 1 ; profileIdx < hBlocksNumber ; 
            ++profileIdx )
          {
            profile.clear();
            
            const unsigned int centerLine = std::min( 
              ( profileIdx * maxNonExpandedBlockHeight ) - 1,
              cachedRasterPtr->getNumberOfRows() - 1 );
            
            TERP_TRUE_OR_RETURN_FALSE( genImageHCutOffProfile( centerLine,
              *(cachedRasterPtr), m_inputParameters.m_inputRasterBands, 
              pixelNeighborhoodSize, horizontalProfilesNeighborhoodSize, 
              profileAntiSmoothingFactor, profile ),
              "Horizontal profile generation error" );
            
            imageHorizontalProfiles.push_back( profile );
            imageHorizontalProfilesCenterLines.push_back( centerLine );
          }
          
          for( profileIdx = 1 ; profileIdx < vBlocksNumber ; 
            ++profileIdx )
          {
            profile.clear();
            
            const unsigned int centerLine = std::min( 
              ( profileIdx * maxNonExpandedBlockWidth ) - 1,
              cachedRasterPtr->getNumberOfColumns() - 1 );
            
            TERP_TRUE_OR_RETURN_FALSE( genImageVCutOffProfile( centerLine,
              *(cachedRasterPtr), m_inputParameters.m_inputRasterBands, 
              pixelNeighborhoodSize, verticalProfilesNeighborhoodSize, 
              profileAntiSmoothingFactor, profile ), 
              "Horizontal profile generation error" );

            imageVerticalProfiles.push_back( profile );
            imageVerticalProfilesCenterLines.push_back( centerLine );
          }          

/*          TERP_TRUE_OR_THROW( createCutOffLinesTiff( imageHorizontalProfiles,
            imageHorizontalProfilesCenterLines,
            imageVerticalProfiles, imageVerticalProfilesCenterLines,
            "cutoflines.tif" ), "internal error" )   */                                      


//          std::cout << std::endl << "CutOff profiles generated" << std::endl;
        }
        
        // Initializing the segments blocks matrix
        
        SegmentsBlocksMatrixT segmentsblocksMatrix;
        segmentsblocksMatrix.reset( SegmentsBlocksMatrixT::RAMMemPol );
        
        {
          TERP_TRUE_OR_RETURN_FALSE( segmentsblocksMatrix.reset( vBlocksNumber, 
            hBlocksNumber ), "Blocks matrix reset error" );
            
          const int linesBound = (int)
            cachedRasterPtr->getNumberOfRows();
          const int colsBound = (int)
            cachedRasterPtr->getNumberOfColumns();
          int expandedBlockXBound = 0;
          int expandedBlockYBound = 0;
          int blockXStart = 0;
          int blockYStart = 0;
          int expandedBlockXStart = 0;
          int expandedBlockYStart = 0;          
            
          for( unsigned int segmentsMatrixLine = 0 ; segmentsMatrixLine < 
            segmentsblocksMatrix.getLinesNumber() ; ++segmentsMatrixLine )
          {
            for( unsigned int segmentsMatrixCol = 0 ; segmentsMatrixCol < 
              segmentsblocksMatrix.getColumnsNumber() ; ++segmentsMatrixCol )
            {
              blockXStart = (int)( segmentsMatrixCol * maxNonExpandedBlockWidth );
              blockYStart = (int)( segmentsMatrixLine * maxNonExpandedBlockHeight );
              
              expandedBlockXStart = std::max( 0, blockXStart - ((int)blocksHOverlapSize) );
              expandedBlockYStart = std::max( 0, blockYStart - ((int)blocksVOverlapSize) );
              
              expandedBlockXBound = std::min( blockXStart + ((int)maxNonExpandedBlockWidth)
                + ((int)blocksHOverlapSize), colsBound );
              expandedBlockYBound = std::min( blockYStart + ((int)maxNonExpandedBlockHeight)
                + ((int)blocksVOverlapSize), linesBound );
              
              SegmenterSegmentsBlock& segmentsBlock = segmentsblocksMatrix( 
                segmentsMatrixLine, segmentsMatrixCol );              
              segmentsBlock.m_status = 
                SegmenterSegmentsBlock::BlockNotProcessed;
              segmentsBlock.m_startX = (unsigned int)expandedBlockXStart;
              segmentsBlock.m_startY = (unsigned int)expandedBlockYStart;  
              segmentsBlock.m_width = (unsigned int)( expandedBlockXBound - 
                expandedBlockXStart );
              segmentsBlock.m_height = (unsigned int)( expandedBlockYBound - 
                expandedBlockYStart );                   
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
              else
              {
                segmentsBlock.m_topCutOffProfile.resize( segmentsBlock.m_width,
                  0 );
                segmentsBlock.m_bottomCutOffProfile.resize( segmentsBlock.m_width,
                  segmentsBlock.m_height - 1 );
                segmentsBlock.m_leftCutOffProfile.resize( segmentsBlock.m_height,
                  0 );
                segmentsBlock.m_rightCutOffProfile.resize( segmentsBlock.m_height,
                  segmentsBlock.m_width - 1 );                
              }
            }
          }
        }
        
        // Disabling de raster cache
        // since it will be not used during segmentation
        
        cachedRasterHandler.reset();
        cachedRasterPtr = 0;
        
         // The progress interface

        const bool enableStrategyProgress =  m_inputParameters.m_enableProgress &&
          ( ( vBlocksNumber * hBlocksNumber ) == 1 ) && ( maxSegThreads == 0 );
        
        std::auto_ptr< te::common::TaskProgress > progressPtr;
        if( m_inputParameters.m_enableProgress && ( ! enableStrategyProgress ) )
        {
          progressPtr.reset( new te::common::TaskProgress );
          progressPtr->setTotalSteps( 1 + ( vBlocksNumber * hBlocksNumber ) );
          progressPtr->setMessage( "Segmentation" );
          progressPtr->pulse();
        }         
        
        // Starting the segmentation 
        
        boost::mutex generalMutex;
        boost::mutex blockProcessedSignalMutex;
        
        te::rst::RasterSynchronizer inputRasterSync( 
          *((te::rst::Raster*)m_inputParameters.m_inputRasterPtr),
          te::common::RAccess );
        te::rst::RasterSynchronizer outputRasterSync( *(outputParamsPtr->m_outputRasterPtr),
          te::common::WAccess );
        
        volatile bool abortSegmentationFlag = false;  
        
        SegmenterIdsManager segmenterIdsManager;
        
        boost::condition_variable blockProcessedSignal;
        
        volatile unsigned int runningThreadsCounter = 0;       
        
        SegmenterThreadEntryParams baseSegThreadParams;
        baseSegThreadParams.m_inputParameters = m_inputParameters;
        baseSegThreadParams.m_outputParametersPtr = outputParamsPtr;
        baseSegThreadParams.m_segsBlocksMatrixPtr = &segmentsblocksMatrix;
        baseSegThreadParams.m_generalMutexPtr = &generalMutex;
        baseSegThreadParams.m_inputRasterSyncPtr = 
          &inputRasterSync;
        baseSegThreadParams.m_outputRasterSyncPtr = 
          &outputRasterSync;
        baseSegThreadParams.m_blockProcessedSignalMutexPtr = 
          &blockProcessedSignalMutex;
        baseSegThreadParams.m_abortSegmentationFlagPtr = &abortSegmentationFlag;
        baseSegThreadParams.m_segmentsIdsManagerPtr = &segmenterIdsManager;
        baseSegThreadParams.m_blockProcessedSignalPtr = &blockProcessedSignal;
        baseSegThreadParams.m_runningThreadsCounterPtr = 
          &runningThreadsCounter;
        baseSegThreadParams.m_inputRasterBandMinValues = inputRasterBandMinValues;
        baseSegThreadParams.m_inputRasterBandMaxValues = inputRasterBandMaxValues;
        baseSegThreadParams.m_enableStrategyProgress = enableStrategyProgress;
          
        if( m_inputParameters.m_inputRasterNoDataValues.empty() )
        {
          for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx < 
            m_inputParameters.m_inputRasterBands.size() ; ++inputRasterBandsIdx )
          {
            baseSegThreadParams.m_inputRasterNoDataValues.push_back(
              m_inputParameters.m_inputRasterPtr->getBand(
              m_inputParameters.m_inputRasterBands[ 
              inputRasterBandsIdx ] )->getProperty()->m_noDataValue );
          }
        }
        else
        {
          baseSegThreadParams.m_inputRasterNoDataValues = m_inputParameters.m_inputRasterNoDataValues;
        }        
        
        if( m_inputParameters.m_enableRasterCache )
        {
          int bandBlockSizeBytes = 
              m_inputParameters.m_inputRasterPtr->getBand( 
              m_inputParameters.m_inputRasterBands[ 0 ] )->getBlockSize();          
          double maxSimultaneousCacheBlocks = ( 0.05 * freeVMem ) / ((double)bandBlockSizeBytes);
          
          if( maxSegThreads )
          {
            baseSegThreadParams.m_maxInputRasterCachedBlocks = std::max( 
              1u, (unsigned int)( maxSimultaneousCacheBlocks / ((double)maxSegThreads) ) );                                                            
          }
          else
          {
            baseSegThreadParams.m_maxInputRasterCachedBlocks = 
              maxSimultaneousCacheBlocks;
          }          
        }
        else
        {
          baseSegThreadParams.m_maxInputRasterCachedBlocks = 1;
        }
        
        if( maxSegThreads && ( ( vBlocksNumber * hBlocksNumber ) > 1 ) )
        { // threaded segmentation mode
          
          // spawning the segmentation threads
          
          runningThreadsCounter = maxSegThreads;
          
          boost::thread_group threads;
          std::vector< SegmenterThreadEntryParams > threadsParams( maxSegThreads );
          
          for( unsigned int threadIdx = 0 ; threadIdx < maxSegThreads ;
            ++threadIdx )
          {
            threadsParams[ threadIdx ] = baseSegThreadParams;
            
            threads.add_thread( new boost::thread( segmenterThreadEntry, 
              &( threadsParams[ threadIdx ] ) ) );
          };
          
          // waiting all threads to finish
          
          int prevSegmentedBlocksNmb = 0;
          
          while( (!abortSegmentationFlag) && (runningThreadsCounter > 0 ) )
          {
            boost::unique_lock<boost::mutex> lock( blockProcessedSignalMutex );
            blockProcessedSignal.timed_wait( lock, 
              boost::posix_time::seconds( 1 ) );
            
//            std::cout << std::endl << "Woke up" << std::endl;
              
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
              
              if( segmentedBlocksNmb != prevSegmentedBlocksNmb )
              {
                progressPtr->pulse();
                prevSegmentedBlocksNmb = segmentedBlocksNmb;
              }
              
              if( ! progressPtr->isActive() ) 
              {
                abortSegmentationFlag = true;
              }
              
//              std::cout << std::endl << "segmentedBlocksNmb:" << segmentedBlocksNmb << std::endl;
            }
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
          segmenterThreadEntry( &baseSegThreadParams );
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
      for( unsigned int inputRasterBandsIdx = 0 ; inputRasterBandsIdx < 
        inputParamsPtr->m_inputRasterBands.size() ; ++inputRasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inputRasterBands[ 
          inputRasterBandsIdx ] < inputParamsPtr->m_inputRasterPtr->getNumberOfBands(),
          "Invalid raster band number" );
      }
      
      TERP_TRUE_OR_RETURN_FALSE( ( inputParamsPtr->m_inputRasterNoDataValues.empty() ?
        true : ( inputParamsPtr->m_inputRasterNoDataValues.size() ==
        inputParamsPtr->m_inputRasterBands.size() ) ),
        "Invalid no-data values" );      
          
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
      const unsigned int minExapandedBlockPixels,
      const unsigned int maxExapandedBlockPixels, 
      const unsigned int blocksHOverlapSize,
      const unsigned int blocksVOverlapSize, 
      unsigned int& nonExpandedBlockWidth,
      unsigned int& nonExpandedBlockHeight ) const
    {
      if( minExapandedBlockPixels > maxExapandedBlockPixels ) return false;
        
      const double maxScaleFactor = 
        ((double)(totalImageLines * totalImageCols)) 
        / 
        ((double)minExapandedBlockPixels);
        
      unsigned int rescaledAndExtendedBlockPixelsNmb = 0;
        
      for( double scaleFactor = 1.0 ; scaleFactor <= maxScaleFactor ;
        scaleFactor += 1.0 )
      {
        nonExpandedBlockHeight = (unsigned int)std::ceil( ((double)totalImageLines) / scaleFactor );
        nonExpandedBlockWidth = (unsigned int)std::ceil( ((double)totalImageCols) / scaleFactor );
        
        rescaledAndExtendedBlockPixelsNmb = ( nonExpandedBlockHeight + blocksVOverlapSize +
          blocksVOverlapSize ) * ( nonExpandedBlockWidth + blocksHOverlapSize + blocksHOverlapSize );
          
        if( rescaledAndExtendedBlockPixelsNmb <= maxExapandedBlockPixels )
        {
          return true;
        }
      }
      
      return false;
    }    
    
    void Segmenter::segmenterThreadEntry(SegmenterThreadEntryParams* paramsPtr)
    {
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr, "Invalid pointer" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_outputParametersPtr,
        "Invalid parameter" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_segsBlocksMatrixPtr,
        "Invalid parameter" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_generalMutexPtr,
        "Invalid parameter" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_inputRasterSyncPtr,
        "Invalid parameter" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_outputRasterSyncPtr,
        "Invalid parameter" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_blockProcessedSignalMutexPtr,
        "Invalid parameter" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_abortSegmentationFlagPtr,
        "Invalid parameter" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_segmentsIdsManagerPtr,
        "Invalid parameter" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_blockProcessedSignalPtr,
        "Invalid parameter" );
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_runningThreadsCounterPtr,
        "Invalid parameter" );
      TERP_DEBUG_TRUE_OR_THROW( !paramsPtr->m_inputRasterBandMinValues.empty(),
        "Invalid parameter" );     
      TERP_DEBUG_TRUE_OR_THROW( !paramsPtr->m_inputRasterBandMaxValues.empty(),
        "Invalid parameter" );  
      TERP_DEBUG_TRUE_OR_THROW( paramsPtr->m_maxInputRasterCachedBlocks,
        "Invalid parameter" );        
      
      // Creating the input raster instance
      
      te::rst::SynchronizedRaster inputRaster( paramsPtr->m_maxInputRasterCachedBlocks, 
        *(paramsPtr->m_inputRasterSyncPtr) );      
        
      // Creating the segmentation strategy instance
      
      paramsPtr->m_generalMutexPtr->lock();
      
      boost::shared_ptr< SegmenterStrategy > strategyPtr(
        SegmenterStrategyFactory::make( 
        paramsPtr->m_inputParameters.m_strategyName ) );
      TERP_TRUE_OR_THROW( strategyPtr.get(), 
        "Unable to create an segmentation strategy" );   
      if( ! strategyPtr->initialize( 
        paramsPtr->m_inputParameters.getSegStrategyParams() ) )
      {
        *(paramsPtr->m_runningThreadsCounterPtr) = 
          *(paramsPtr->m_runningThreadsCounterPtr) - 1;
        *(paramsPtr->m_abortSegmentationFlagPtr) = true;
        
//                std::cout << std::endl<< "Thread exit (error)"
//                  << std::endl;                
        
        paramsPtr->m_generalMutexPtr->unlock();

        return;
      }
      else
      {
        paramsPtr->m_generalMutexPtr->unlock();
      }
      
      // Looking for a non processed segments block
      
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
              
              // Creating the output raster instance
              
              te::rst::SynchronizedRaster outputRaster( 1, *(paramsPtr->m_outputRasterSyncPtr) );
              
              // Executing the strategy
              
              if( ! strategyPtr->execute( 
                *paramsPtr->m_segmentsIdsManagerPtr,
                segsBlk,
                inputRaster, 
                paramsPtr->m_inputParameters.m_inputRasterBands,
                paramsPtr->m_inputRasterNoDataValues,
                paramsPtr->m_inputRasterBandMinValues,
                paramsPtr->m_inputRasterBandMaxValues,
                outputRaster,
                0,
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
      profile.clear();
      
      if( profileAntiSmoothingFactor == 0 )
      {
        return false;
      }
      if( profileCenter >= inRaster.getNumberOfRows() )
      {
        return false;
      }
      if( tileNeighborhoodSize < pixelNeighborhoodSize )
      {
        return false;
      }
      
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
      profile.clear();
      
      if( profileAntiSmoothingFactor == 0 )
      {
        return false;
      }
      if( profileCenter >= inRaster.getNumberOfColumns() )
      {
        return false;
      }
      if( tileNeighborhoodSize < pixelNeighborhoodSize )
      {
        return false;
      }      
      
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
      if( segmentsBlock.m_segmentsMatrixXIndex > imageVerticalProfiles.size() )
      {
        return false;
      }
      if( segmentsBlock.m_segmentsMatrixYIndex > imageHorizontalProfiles.size() )
      {
        return false;
      }      
      
      // generating m_topCutOffProfile
      
      segmentsBlock.m_topCutOffProfile.clear();      
      
      if( ( segmentsBlock.m_segmentsMatrixYIndex > 0 ) &&
        ( segmentsBlock.m_segmentsMatrixYIndex <= 
        imageHorizontalProfiles.size() ) )
      {
        const std::vector< unsigned int >& profile = imageHorizontalProfiles[ 
          segmentsBlock.m_segmentsMatrixYIndex - 1 ];
          
        if( profile.empty() )
        {
          return false;
        }
        else
        {
          segmentsBlock.m_topCutOffProfile.resize( segmentsBlock.m_width );      
          int profElementValue = 0;
          
          for( unsigned int profEIdx = 0 ; profEIdx < segmentsBlock.m_width ;
            ++profEIdx )
          {
            if( profEIdx + segmentsBlock.m_startX >= profile.size() )
            {
              return false;
            }
            
            profElementValue = 
              ((int)profile[ profEIdx + segmentsBlock.m_startX ]) -
              ((int)segmentsBlock.m_startY);
            profElementValue = MAX( 0, profElementValue );
            profElementValue = MIN( (int)segmentsBlock.m_height, profElementValue );
            
            segmentsBlock.m_topCutOffProfile[ profEIdx ] = profElementValue;          
          }
        }
      }
      else
      {
        segmentsBlock.m_topCutOffProfile.resize( segmentsBlock.m_width, 0 );
      }
      
      // generating m_bottomCutOffProfile
      
      segmentsBlock.m_bottomCutOffProfile.clear();            
      
      if( segmentsBlock.m_segmentsMatrixYIndex < 
        imageHorizontalProfiles.size() )
      {
        const std::vector< unsigned int >& profile = imageHorizontalProfiles[ 
          segmentsBlock.m_segmentsMatrixYIndex ];
          
        if( profile.empty() )
        {
          return false;
        }
        else
        {
          segmentsBlock.m_bottomCutOffProfile.resize( segmentsBlock.m_width );            
          int profElementValue = 0;
          
          for( unsigned int profEIdx = 0 ; profEIdx < segmentsBlock.m_width ;
            ++profEIdx )
          {
            if( profEIdx + segmentsBlock.m_startX >= profile.size() )
            {
              return false;
            }
            
            profElementValue = 
              ((int)profile[ profEIdx + segmentsBlock.m_startX ]) -
              ((int)segmentsBlock.m_startY);
            profElementValue = MAX( 0, profElementValue );
            profElementValue = MIN( (int)segmentsBlock.m_height, profElementValue );
            
            segmentsBlock.m_bottomCutOffProfile[ profEIdx ] = profElementValue;          
          }
        }
      }
      else
      {
        segmentsBlock.m_bottomCutOffProfile.resize( segmentsBlock.m_width,
          segmentsBlock.m_height - 1);
      }
      
      // generating m_leftCutOffProfile
      
      segmentsBlock.m_leftCutOffProfile.clear();   
      
      if( ( segmentsBlock.m_segmentsMatrixXIndex > 0 ) &&
        ( segmentsBlock.m_segmentsMatrixXIndex <=
        imageVerticalProfiles.size() ) )
      {
        const std::vector< unsigned int >& profile = imageVerticalProfiles[ 
          segmentsBlock.m_segmentsMatrixXIndex - 1 ];
          
        if( profile.empty() )
        {
          return false;
        }
        else
        {
          segmentsBlock.m_leftCutOffProfile.resize( segmentsBlock.m_height );      
          int profElementValue = 0;
          
          for( unsigned int profEIdx = 0 ; profEIdx < segmentsBlock.m_height ;
            ++profEIdx )
          {
            if( profEIdx + segmentsBlock.m_startY >= profile.size() )
            {
              return false;
            }
            
            profElementValue = 
              ((int)profile[ profEIdx + segmentsBlock.m_startY ]) -
              ((int)segmentsBlock.m_startX);
            profElementValue = MAX( 0, profElementValue );
            profElementValue = MIN( (int)segmentsBlock.m_width, profElementValue );            
            
            segmentsBlock.m_leftCutOffProfile[ profEIdx ] = profElementValue;          
          }
        }
      }
      else
      {
        segmentsBlock.m_leftCutOffProfile.resize( segmentsBlock.m_height, 0 );
      }
      
      // generating m_rightCutOffProfile
      
      segmentsBlock.m_rightCutOffProfile.clear();      
      
      if( segmentsBlock.m_segmentsMatrixXIndex <
        imageVerticalProfiles.size() )
      {
        const std::vector< unsigned int >& profile = imageVerticalProfiles[ 
          segmentsBlock.m_segmentsMatrixXIndex  ];
          
        if( profile.empty() )
        {
          return false;
        }
        else
        {
          segmentsBlock.m_rightCutOffProfile.resize( segmentsBlock.m_height );      
          int profElementValue = 0;
          
          for( unsigned int profEIdx = 0 ; profEIdx < segmentsBlock.m_height ;
            ++profEIdx )
          {
            if( profEIdx + segmentsBlock.m_startY >= profile.size() )
            {
              return false;
            }
            
            profElementValue = 
              ((int)profile[ profEIdx + segmentsBlock.m_startY ]) -
              ((int)segmentsBlock.m_startX);
            profElementValue = MAX( 0, profElementValue );
            profElementValue = MIN( (int)segmentsBlock.m_width, profElementValue );
            
            segmentsBlock.m_rightCutOffProfile[ profEIdx ] = profElementValue;          
          }
        }
      }
      else
      {
        segmentsBlock.m_rightCutOffProfile.resize( segmentsBlock.m_height, 
          segmentsBlock.m_width - 1 ); 
      }
      
      assert( segmentsBlock.m_topCutOffProfile.size() == segmentsBlock.m_width );
      assert( segmentsBlock.m_bottomCutOffProfile.size() == segmentsBlock.m_width );
      assert( segmentsBlock.m_leftCutOffProfile.size() == segmentsBlock.m_height );
      assert( segmentsBlock.m_rightCutOffProfile.size() == segmentsBlock.m_height );
      
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

