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
  \file terralib/rp/TiePointsLocatorMoravecStrategy.cpp
  \brief Tie-Pointsr locator Moravec strategy.
*/

#include "TiePointsLocatorMoravecStrategy.h"
#include "Macros.h"
#include "../common/progress/TaskProgress.h"

#include <memory>

namespace te
{
  namespace rp
  {
    TiePointsLocatorMoravecStrategy::TiePointsLocatorMoravecStrategy()
    {
      reset();
    }
    
    TiePointsLocatorMoravecStrategy::~TiePointsLocatorMoravecStrategy()
    {
      reset();
    }
    
    bool TiePointsLocatorMoravecStrategy::initialize( 
      const te::rp::TiePointsLocatorInputParameters& inputParameters )
    {
      m_inputParameters = inputParameters;
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster1Bands.size()
        == 1, "Invalid number of raster 1 bands" );
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster2Bands.size()
        == 1, "Invalid number of raster 2 bands" );
        
      // Defining the number of tie points
      
      if( m_inputParameters.m_maxTiePoints == 0 )
      {
        const unsigned int maxRastersArea = 
          std::max(
            ( m_inputParameters.m_raster1TargetAreaWidth *
              m_inputParameters.m_raster1TargetAreaHeight )
            ,
            ( m_inputParameters.m_raster2TargetAreaWidth *
              m_inputParameters.m_raster2TargetAreaHeight )                       
          );
        const unsigned maxWindowSize = std::max( 
           m_inputParameters.m_moravecCorrelationWindowWidth,
           m_inputParameters.m_moravecWindowWidth );
        m_inputParameters.m_maxTiePoints = maxRastersArea / 
          ( 4 * maxWindowSize * maxWindowSize );
          
        // This is because the features and matching matrix are being allocated in RAM
        const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
        const double usedVMem = (double)te::common::GetUsedVirtualMemory();
        const double totalVMem = (double)te::common::GetTotalVirtualMemory();
        const double freeVMem = 0.4 * std::min( totalPhysMem, ( totalVMem - usedVMem ) );                
        const double featSize = (double)( m_inputParameters.m_moravecCorrelationWindowWidth *
          m_inputParameters.m_moravecCorrelationWindowWidth );
        m_inputParameters.m_maxTiePoints = 
          std::min(
            m_inputParameters.m_maxTiePoints,
            (unsigned int)(
              std::sqrt(
                ( featSize * featSize )
                +
                ( freeVMem / (double)( sizeof( float ) ) )
              )
              -
              featSize
            )
          );
      }
        
      m_inputParameters.m_moravecCorrelationWindowWidth += 
        m_inputParameters.m_moravecCorrelationWindowWidth % 2 ? 0 : 1;
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_moravecCorrelationWindowWidth > 2,
        "Invalid m_moravecCorrelationWindowWidth" );

      m_inputParameters.m_moravecWindowWidth += 
        m_inputParameters.m_moravecWindowWidth % 2 ? 0 : 1;
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_moravecWindowWidth > 2,
        "Invalid m_moravecWindowWidth" );      
      
      TERP_TRUE_OR_RETURN_FALSE( ( m_inputParameters.m_moravecMinAbsCorrelation >= 0 ) &&
        ( m_inputParameters.m_moravecMinAbsCorrelation <= 1.0 ),
        "Invalid m_moravecMinAbsCorrelation" );        
      
      m_isInitialized = true;
      return true;
    }
    
    void TiePointsLocatorMoravecStrategy::reset()
    {
      m_isInitialized = false;
      m_inputParameters.reset();
    }
    
    bool TiePointsLocatorMoravecStrategy::getMatchedInterestPoints( MatchedInterestPointsSetT& matchedInterestPoints )
    {
      matchedInterestPoints.clear(); 
      
      if( !m_isInitialized )
      {
        return false;
      }      
      
      /* Calculating the rescale factors 
        factor = rescaled_orignal_image / original_image  */
      
      double raster1XRescFact = 1.0;
      double raster1YRescFact = 1.0;
      double raster2XRescFact = 1.0;
      double raster2YRescFact = 1.0;
      
      if( m_inputParameters.m_pixelSizeXRelation > 1.0 ) 
      {
        /* The image 1 has poor resolution - bigger pixel resolution values -
          and image 2 needs to be rescaled down */
        
        raster2XRescFact = 1.0 / m_inputParameters.m_pixelSizeXRelation;
      } 
      else if( m_inputParameters.m_pixelSizeXRelation < 1.0 ) 
      {
        /* The image 2 has poor resolution - bigger pixel resolution values
          and image 1 needs to be rescaled down */
        
        raster1XRescFact = m_inputParameters.m_pixelSizeXRelation;
      }     
      
      if( m_inputParameters.m_pixelSizeYRelation > 1.0 ) 
      {
        /* The image 1 has poor resolution - bigger pixel resolution values -
          and image 2 needs to be rescaled down */
        
        raster2YRescFact = 1.0 / m_inputParameters.m_pixelSizeYRelation;
      } 
      else if( m_inputParameters.m_pixelSizeYRelation < 1.0 ) 
      {
        /* The image 2 has poor resolution - bigger pixel resolution values
          and image 1 needs to be rescaled down */
        
        raster1YRescFact = m_inputParameters.m_pixelSizeYRelation;
      }        
      
      raster1XRescFact *= m_inputParameters.m_subSampleOptimizationRescaleFactor;
      raster1YRescFact *= m_inputParameters.m_subSampleOptimizationRescaleFactor;
      raster2XRescFact *= m_inputParameters.m_subSampleOptimizationRescaleFactor;
      raster2YRescFact *= m_inputParameters.m_subSampleOptimizationRescaleFactor;      
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        progressPtr->setTotalSteps( 1 );
        progressPtr->setMessage( "Locating tie points" );
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }           
      
      // Calculating the number of tie points to be found
      
      unsigned int raster1MaxInterestPoints = m_inputParameters.m_maxTiePoints;
      unsigned int raster2MaxInterestPoints = m_inputParameters.m_maxTiePoints;
      {
        double rescRaster1Area = 
          ( (double)( m_inputParameters.m_raster1TargetAreaWidth ) * raster1XRescFact ) *
          ( (double)( m_inputParameters.m_raster1TargetAreaHeight ) * raster1YRescFact );
        double rescRaster2Area =
          ( (double)( m_inputParameters.m_raster2TargetAreaWidth ) * raster2XRescFact ) *
          ( (double)( m_inputParameters.m_raster2TargetAreaHeight ) * raster2YRescFact );;
          
        if( rescRaster1Area > rescRaster2Area )
        {
          raster1MaxInterestPoints = (unsigned int)( 
            rescRaster1Area / 
            ( rescRaster2Area / ( (double)m_inputParameters.m_maxTiePoints ) ) );
        }
        else if( rescRaster1Area < rescRaster2Area )
        {
          raster2MaxInterestPoints = (unsigned int)( 
            rescRaster2Area / 
            ( rescRaster1Area / ( (double)m_inputParameters.m_maxTiePoints ) ) ); 
        }
      }
      
      // Updating the progress interface steps number
      
      if( m_inputParameters.m_enableProgress )
        progressPtr->setTotalSteps( progressPtr->getTotalSteps() + 10 );
      
      // Generating raster 1 features
      
      FloatsMatrix raster1Features;
      InterestPointsSetT raster1InterestPoints;
      {
        // loading raster data
        std::vector< boost::shared_ptr< FloatsMatrix > > raster1Data;
        UCharsMatrix maskRaster1Data;
        
        if( !loadRasterData( 
          m_inputParameters.m_inRaster1Ptr,
          m_inputParameters.m_inRaster1Bands,
          m_inputParameters.m_inMaskRaster1Ptr,
          0,
          m_inputParameters.m_raster1TargetAreaLineStart,
          m_inputParameters.m_raster1TargetAreaColStart,
          m_inputParameters.m_raster1TargetAreaWidth,
          m_inputParameters.m_raster1TargetAreaHeight,
          raster1XRescFact,
          raster1YRescFact,
          m_inputParameters.m_interpMethod,
          20,
          raster1Data,
          maskRaster1Data ) )
        {
          return false;
        }
          
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }
          
        // applying the noise filter

        if( m_inputParameters.m_moravecNoiseFilterIterations )
        {
          boost::shared_ptr< FloatsMatrix > tempMatrix( 
            new FloatsMatrix );
          TERP_TRUE_OR_RETURN_FALSE( tempMatrix->reset( 
            0, 0,
            FloatsMatrix::AutoMemPol, 
            raster1Data[ 0 ]->getMaxTmpFileSize(),
            raster1Data[ 0 ]->getMaxMemPercentUsage() ),
            "Cannot allocate image matrix" );
          
          if( !applyMeanFilter( *(raster1Data[ 0 ]), 
            *tempMatrix, m_inputParameters.m_moravecNoiseFilterIterations ) )
          {
            return false;
          }  
            
          raster1Data[ 0 ]->reset();
          raster1Data[ 0 ] = tempMatrix;
          
//        createTifFromMatrix( *(raster1Data[ 0 ]), InterestPointsSetT(), "raster1Filtered");          
        }
        
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }        
        
        // locating interest points
        
        if( !locateMoravecInterestPoints( 
          *(raster1Data[ 0 ]), 
          maskRaster1Data.getLinesNumber() ? (&maskRaster1Data) : 0, 
          m_inputParameters.m_moravecWindowWidth,
          raster1MaxInterestPoints,
          m_inputParameters.m_enableMultiThread,
          raster1InterestPoints ) )
        {
          return false;
        }
          
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }          
          
        // Generting features (one feature per line)
        
        raster1Features.reset( FloatsMatrix::RAMMemPol );
        InterestPointsSetT auxInterestPoints;
        
        TERP_TRUE_OR_RETURN_FALSE( generateCorrelationFeatures( 
          raster1InterestPoints,
          m_inputParameters.m_moravecCorrelationWindowWidth,
          *(raster1Data[ 0 ]),
          raster1Features,
          auxInterestPoints ),
          "Error generating raster 1 features" );
          
        raster1InterestPoints = auxInterestPoints;        
        
//      features2Tiff( raster1Features, raster1InterestPoints, "raster1features" );        

        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }
      }
      
      // Generating raster 2 features
      
      FloatsMatrix raster2Features;
      InterestPointsSetT raster2InterestPoints;
      {
        // Loading image data
      
        std::vector< boost::shared_ptr< FloatsMatrix > > raster2Data;
        UCharsMatrix maskRaster2Data;
                     
        if( !loadRasterData( 
          m_inputParameters.m_inRaster2Ptr,
          m_inputParameters.m_inRaster2Bands,
          m_inputParameters.m_inMaskRaster2Ptr,
          0,
          m_inputParameters.m_raster2TargetAreaLineStart,
          m_inputParameters.m_raster2TargetAreaColStart,
          m_inputParameters.m_raster2TargetAreaWidth,
          m_inputParameters.m_raster2TargetAreaHeight,                                                
          raster2XRescFact,
          raster2YRescFact,
          m_inputParameters.m_interpMethod,
          20,
          raster2Data,
          maskRaster2Data ) )
        {
          return false;
        }
          
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }          
          
        // applying the noise filter
        
        if( m_inputParameters.m_moravecNoiseFilterIterations )
        {
          boost::shared_ptr< FloatsMatrix > tempMatrix( 
            new FloatsMatrix );
          
          tempMatrix.reset( new FloatsMatrix );
          TERP_TRUE_OR_RETURN_FALSE( tempMatrix->reset( 
            0, 0,
            FloatsMatrix::AutoMemPol, 
            raster2Data[ 0 ]->getMaxTmpFileSize(),
            raster2Data[ 0 ]->getMaxMemPercentUsage() ),
            "Cannot allocate image matrix" );          
          
          if( !applyMeanFilter( *(raster2Data[ 0 ]), 
            *tempMatrix, m_inputParameters.m_moravecNoiseFilterIterations ) )
          {
            return false;
          }

          raster2Data[ 0 ] = tempMatrix;          
          
//        createTifFromMatrix( *(raster2Data[ 0 ]), InterestPointsSetT(), "raster2Filtered");  
        }
        
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }       
        
        // locating interest points        
        
        if( !locateMoravecInterestPoints( 
          *(raster2Data[ 0 ]), 
          maskRaster2Data.getLinesNumber() ? (&maskRaster2Data) : 0, 
          m_inputParameters.m_moravecWindowWidth,
          raster2MaxInterestPoints,
          m_inputParameters.m_enableMultiThread,
          raster2InterestPoints ) )
        {
          return false;
        }
          
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }       
          
        // Generting features (one feature per line)

        raster2Features.reset( FloatsMatrix::RAMMemPol );
        InterestPointsSetT auxInterestPoints;        
        
        TERP_TRUE_OR_RETURN_FALSE( generateCorrelationFeatures( 
          raster2InterestPoints,
          m_inputParameters.m_moravecCorrelationWindowWidth,
          *(raster2Data[ 0 ]),
          raster2Features,
          auxInterestPoints ),
          "Error generating raster 2 features" );
          
        raster2InterestPoints = auxInterestPoints;
        
//      features2Tiff( raster2Features, raster2InterestPoints, "raster2features" );        

        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }
      }

      // Matching features
      
      MatchedInterestPointsSetT internalMatchedInterestPoints;
      
      TERP_TRUE_OR_RETURN_FALSE( executeMatchingByCorrelation( 
        raster1Features,
        raster2Features,
        raster1InterestPoints,
        raster2InterestPoints,
        m_inputParameters.m_maxR1ToR2Offset,
        m_inputParameters.m_enableMultiThread,
        m_inputParameters.m_moravecMinAbsCorrelation,
        internalMatchedInterestPoints ),
        "Error matching features" );
      
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }    
      
      // Clean anused data
      
      raster1Features.clear();
      raster2Features.clear();
      raster1InterestPoints.clear();
      raster2InterestPoints.clear();
      
      // Generating the  output matched interest points
      
      {
        MatchedInterestPointsT auxMatchedPoints;
        MatchedInterestPointsSetT::const_iterator itB = internalMatchedInterestPoints.begin();
        const MatchedInterestPointsSetT::const_iterator itE = internalMatchedInterestPoints.end();
        
        float minFeatureValue1 = FLT_MAX;
        float maxFeatureValue1 = (-1.0) * FLT_MAX;
        float minFeatureValue2 = FLT_MAX;
        float maxFeatureValue2 = (-1.0) * FLT_MAX;
        
        itB = internalMatchedInterestPoints.begin();
        while( itB != itE )
        {
          if( minFeatureValue1 > itB->m_point1.m_feature1 )
            minFeatureValue1 = itB->m_point1.m_feature1;
          if( maxFeatureValue1 < itB->m_point1.m_feature1 )
            maxFeatureValue1 = itB->m_point1.m_feature1;

          if( minFeatureValue2 > itB->m_point2.m_feature1 )
            minFeatureValue2 = itB->m_point2.m_feature1;
          if( maxFeatureValue2 < itB->m_point2.m_feature1 )
            maxFeatureValue2 = itB->m_point2.m_feature1;

          ++itB;
        }        
       
        float featureValue1Range = maxFeatureValue1 - minFeatureValue1;
        float featureValue2Range = maxFeatureValue2 - minFeatureValue2;
        
        if( ( featureValue1Range == 0.0 ) || ( featureValue2Range == 0.0 ) )
        {
          itB = internalMatchedInterestPoints.begin();
          
          while( itB != itE )
          {
            matchedInterestPoints.insert( *itB );
           
            ++itB;
          }
        }
        else
        {
          itB = internalMatchedInterestPoints.begin();
          
          while( itB != itE )
          {
            auxMatchedPoints = *itB;
            
            auxMatchedPoints.m_point1.m_x = ( auxMatchedPoints.m_point1.m_x / raster1XRescFact ) + 
              (double)m_inputParameters.m_raster1TargetAreaColStart;
            auxMatchedPoints.m_point1.m_y = ( auxMatchedPoints.m_point1.m_y / raster1YRescFact ) + 
              (double)m_inputParameters.m_raster1TargetAreaLineStart;          
            auxMatchedPoints.m_point2.m_x = ( auxMatchedPoints.m_point2.m_x / raster2XRescFact ) + 
              (double)m_inputParameters.m_raster2TargetAreaColStart;
            auxMatchedPoints.m_point2.m_y = ( auxMatchedPoints.m_point2.m_y / raster2YRescFact ) + 
              (double)m_inputParameters.m_raster2TargetAreaLineStart;  
              
            auxMatchedPoints.m_feature = 
              (
                ( 2.0f * auxMatchedPoints.m_feature )
                +
                std::min(
                  ( 
                    ( auxMatchedPoints.m_point1.m_feature1 - minFeatureValue1 ) 
                    /
                    featureValue1Range
                  )
                  ,
                  ( 
                    ( auxMatchedPoints.m_point2.m_feature1 - minFeatureValue2 ) 
                    /
                    featureValue2Range
                  )                  
                )
              );            
            
            matchedInterestPoints.insert( auxMatchedPoints );
            
            ++itB;
          }
        }
      }
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }          
      
      return true;
    }
    
    bool TiePointsLocatorMoravecStrategy::applyMeanFilter( const FloatsMatrix& inputData,
      FloatsMatrix& outputData, const unsigned int iterationsNumber )
    {
      if( iterationsNumber == 0 )
      {
        outputData = inputData;
      }
      else
      {
        if( inputData.getColumnsNumber() < 3 )
        {
          return false;
        }
        if( inputData.getLinesNumber() < 3 )
        {
          return false;
        }
                           
        TERP_TRUE_OR_RETURN_FALSE( outputData.reset( inputData.getLinesNumber(),
          inputData.getColumnsNumber() ), "Cannot allocate image matrix" );
      
        const unsigned int nLines = inputData.getLinesNumber();
        const unsigned int nCols = inputData.getColumnsNumber();
        const unsigned int lastLineIndex = nLines - 1;
        const unsigned int lastColIndex = nCols - 1;  
        unsigned int currLine = 0;
        unsigned int currCol = 0;  
        
        // internal temp matrixes
        
        FloatsMatrix tempMatrix;
        
        if( iterationsNumber > 1 )
        {
          TERP_TRUE_OR_RETURN_FALSE( tempMatrix.reset( nLines, nCols,
            FloatsMatrix::AutoMemPol ),
            "Cannot allocate image matrix" );
        }
        
        /* Fill borders with zero */
        
        for( currLine = 0 ; currLine < nLines ; ++currLine ) {
          outputData( currLine, 0 ) = 0.0;
          outputData( currLine, lastColIndex ) = 0.0;
        }
        
        for( currCol = 0 ; currCol < nCols ; ++currCol ) {
          outputData( 0, currCol ) = 0.0;
          outputData( lastLineIndex, currCol ) = 0.0;
        }  
        
        /* Smoothing */
        
        FloatsMatrix const* inputPtr = 0;
        FloatsMatrix* outputPtr = 0;
        FloatsMatrix const* auxPtr = 0;
        float* outputLinePtr = 0;
        unsigned int prevLine = 0;
        unsigned int prevCol = 0;
        unsigned int nextLine = 0;
        unsigned int nextCol = 0;
        
        for( unsigned int iteration = 0 ; iteration < iterationsNumber ;
          ++iteration )
        {
          if( iteration == 0 )
          {
            inputPtr = &inputData;
            
            if( iterationsNumber > 1 )
              outputPtr = &tempMatrix;
            else
              outputPtr = &outputData;
          }
          else if( iteration == iterationsNumber - 1 )
          {
            inputPtr = outputPtr;
            outputPtr = &outputData;
          }
          else
          {
            auxPtr = inputPtr;
            inputPtr = outputPtr;
            outputPtr = (FloatsMatrix*)auxPtr;
          }
          
          const FloatsMatrix& internalInputMatrix = *inputPtr;
          
          for( currLine = 1 ; currLine < lastLineIndex ; ++currLine ) 
          {
            prevLine = currLine - 1;
            nextLine = currLine + 1;
            
            outputLinePtr = outputPtr->operator[]( currLine );
            
            for( currCol = 1 ; currCol < lastColIndex ; ++currCol ) 
            {
              prevCol = currCol - 1;
              nextCol = currCol + 1;
              
              outputLinePtr[ currCol ] = 
                ( 
                  internalInputMatrix( prevLine, prevCol )
                  + internalInputMatrix( prevLine, currCol )
                  + internalInputMatrix( prevLine, nextCol )
                  + internalInputMatrix( currLine, prevCol )
                  + internalInputMatrix( currLine, nextCol )
                  + internalInputMatrix( nextLine, prevCol )
                  + internalInputMatrix( nextLine, currCol )
                  + internalInputMatrix( nextLine, nextCol )
                ) / 8.0f;
            }
          }
        }
      }
      
      return true;
    }        
    
    bool TiePointsLocatorMoravecStrategy::locateMoravecInterestPoints( 
      const FloatsMatrix& rasterData,
      UCharsMatrix const* maskRasterDataPtr,
      const unsigned int moravecWindowWidth,
      const unsigned int maxInterestPoints,
      const unsigned int enableMultiThread,
      InterestPointsSetT& interestPoints )
    {
      interestPoints.clear();

      const unsigned int minRasterWidthAndHeight = 2 * ( ( 2 * 
        moravecWindowWidth ) - 1 );
      // There is not enough data to look for interest points!
      if( rasterData.getColumnsNumber() < minRasterWidthAndHeight ) return true;
      if( rasterData.getLinesNumber() < minRasterWidthAndHeight ) return true;
      
      bool returnValue = true;
      boost::mutex rastaDataAccessMutex;
      boost::mutex interestPointsAccessMutex;
      unsigned int nextRasterLinesBlockToProcess = 0;
      
      MoravecLocatorThreadParams threadParams;
      threadParams.m_returnValuePtr = &returnValue;
      threadParams.m_rastaDataAccessMutexPtr = &rastaDataAccessMutex;
      threadParams.m_interestPointsAccessMutexPtr = &interestPointsAccessMutex;
      threadParams.m_nextRasterLinesBlockToProcessValuePtr = 
        &nextRasterLinesBlockToProcess;
      threadParams.m_interestPointsPtr = &interestPoints;
      threadParams.m_rasterDataPtr = &rasterData;
      threadParams.m_maskRasterDataPtr = maskRasterDataPtr;
      threadParams.m_moravecWindowWidth = moravecWindowWidth;        
      
      if( enableMultiThread )
      {
        const unsigned int procsNumber = te::common::GetPhysProcNumber();
        
        threadParams.m_maxRasterLinesBlockMaxSize = std::max(
          minRasterWidthAndHeight, rasterData.getLinesNumber() / procsNumber );
          
        const unsigned int rasterLinesBlocksNumber = 
          ( rasterData.getLinesNumber() / threadParams.m_maxRasterLinesBlockMaxSize ) +
          ( ( rasterData.getLinesNumber() % threadParams.m_maxRasterLinesBlockMaxSize ) ? 1 : 0 );

        threadParams.m_maxInterestPointsPerRasterLinesBlock =
          maxInterestPoints / rasterLinesBlocksNumber;
        
        boost::thread_group threads;
        
        for( unsigned int threadIdx = 0 ; threadIdx < procsNumber ;
          ++threadIdx )
        {
          threads.add_thread( new boost::thread( 
            locateMoravecInterestPointsThreadEntry, 
            &threadParams ) );
        }
        
        threads.join_all();
      }
      else
      {
        threadParams.m_maxRasterLinesBlockMaxSize = rasterData.getLinesNumber();
        threadParams.m_maxInterestPointsPerRasterLinesBlock = maxInterestPoints;
        
        locateMoravecInterestPointsThreadEntry( &threadParams );
      }
     
      return returnValue;
    }
    
    void TiePointsLocatorMoravecStrategy::locateMoravecInterestPointsThreadEntry(MoravecLocatorThreadParams* paramsPtr)
    {
      assert( paramsPtr );
      assert( paramsPtr->m_returnValuePtr );
      assert( paramsPtr->m_moravecWindowWidth > 2 );
      assert( paramsPtr->m_rasterDataPtr );
      assert( paramsPtr->m_interestPointsPtr );
      assert( paramsPtr->m_rastaDataAccessMutexPtr );
      assert( paramsPtr->m_interestPointsAccessMutexPtr );
      assert( paramsPtr->m_maxRasterLinesBlockMaxSize > 2 );
      assert( paramsPtr->m_nextRasterLinesBlockToProcessValuePtr );
      
      const unsigned int moravecWindowWidth = paramsPtr->m_moravecWindowWidth;
      const unsigned int moravecWindowRadius = moravecWindowWidth / 2;   
      
      // Allocating the internal raster data buffer
      // and the mask raster buffer
      
      paramsPtr->m_rastaDataAccessMutexPtr->lock();
      
      const unsigned int rasterLines = paramsPtr->m_rasterDataPtr->getLinesNumber();
      const unsigned int bufferLines = moravecWindowWidth;
      const unsigned int lastBufferLineIdx = bufferLines - 1;
      const unsigned int bufferCols = paramsPtr->m_rasterDataPtr->getColumnsNumber();
      const unsigned int rasterBufferLineSizeBytes = sizeof( 
        FloatsMatrix::ElementTypeT ) * bufferCols;
      const unsigned int maskRasterBufferLineSizeBytes = sizeof(
        UCharsMatrix::ElementTypeT ) * 
        bufferCols;
      
      paramsPtr->m_rastaDataAccessMutexPtr->unlock();
        
      FloatsMatrix rasterBufferDataHandler;
      if( ! rasterBufferDataHandler.reset( bufferLines, bufferCols, 
        FloatsMatrix::RAMMemPol ) )
      {
        paramsPtr->m_rastaDataAccessMutexPtr->lock();
        *(paramsPtr->m_returnValuePtr) = false;
        paramsPtr->m_rastaDataAccessMutexPtr->unlock();
        return;
      }
      
      boost::scoped_array< float* > rasterBufferHandler( new float*[ bufferLines ] );
      for( unsigned int rasterBufferDataHandlerLine = 0 ; rasterBufferDataHandlerLine < 
        bufferLines ; ++rasterBufferDataHandlerLine )
      {
        rasterBufferHandler[ rasterBufferDataHandlerLine ] = rasterBufferDataHandler[ 
          rasterBufferDataHandlerLine ];
      }
      
      float** rasterBufferPtr = rasterBufferHandler.get();
      
      // Allocating the mask raster buffer      
      
      UCharsMatrix maskRasterBufferDataHandler;
      
      boost::scoped_array< unsigned char* > maskRasterBufferHandler( new unsigned char*[ bufferLines ] );
      
      unsigned char** maskRasterBufferPtr = 0;
      
      if( paramsPtr->m_maskRasterDataPtr )
      {
        if( ! maskRasterBufferDataHandler.reset( bufferLines, bufferCols, 
          UCharsMatrix::RAMMemPol ) )
        {
          paramsPtr->m_rastaDataAccessMutexPtr->lock();
          *(paramsPtr->m_returnValuePtr) = false;
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          return;
        }        
        
        for( unsigned int maskRasterBufferDataHandlerLine = 0 ; maskRasterBufferDataHandlerLine < 
          bufferLines ; ++maskRasterBufferDataHandlerLine )
        {
          maskRasterBufferHandler[ maskRasterBufferDataHandlerLine ] = maskRasterBufferDataHandler[ 
            maskRasterBufferDataHandlerLine ];
        }
        
        maskRasterBufferPtr = maskRasterBufferHandler.get();      
      }      
      
      // Allocating the internal maximas values data buffer
        
      FloatsMatrix maximasBufferDataHandler;
      if( ! maximasBufferDataHandler.reset( bufferLines, bufferCols, 
        FloatsMatrix::RAMMemPol ) )
      {
        paramsPtr->m_rastaDataAccessMutexPtr->lock();
        *(paramsPtr->m_returnValuePtr) = false;
        paramsPtr->m_rastaDataAccessMutexPtr->unlock();
        return;
      }
      
      boost::scoped_array< float* > maximasBufferHandler( new float*[ bufferLines ] );
      float** maximasBufferPtr = maximasBufferHandler.get();      
      unsigned int bufferCol = 0;
      for( unsigned int maximasBufferDataHandlerLine = 0 ; maximasBufferDataHandlerLine < 
        bufferLines ; ++maximasBufferDataHandlerLine )
      {
        maximasBufferHandler[ maximasBufferDataHandlerLine ] = maximasBufferDataHandler[ 
          maximasBufferDataHandlerLine ];
        for( bufferCol = 0 ; bufferCol < bufferCols ; ++bufferCol )
        {
          maximasBufferPtr[ maximasBufferDataHandlerLine ][ bufferCol ] = 0;
        }          
      }
      
      // Pick the next block to process
      
      const unsigned int rasterLinesBlocksNumber = 
        ( rasterLines / paramsPtr->m_maxRasterLinesBlockMaxSize ) +
        ( ( rasterLines % paramsPtr->m_maxRasterLinesBlockMaxSize ) ? 1 : 0 );
        
      for( unsigned int rasterLinesBlockIdx = 0; rasterLinesBlockIdx <
        rasterLinesBlocksNumber ; ++rasterLinesBlockIdx )
      {
        InterestPointsSetT blockMaximas; // the maxima points found inside the current raster block
        
        paramsPtr->m_rastaDataAccessMutexPtr->lock();
         
        if( rasterLinesBlockIdx == ( *(paramsPtr->m_nextRasterLinesBlockToProcessValuePtr ) ) )
        {
          ++( *(paramsPtr->m_nextRasterLinesBlockToProcessValuePtr ) );
           
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          
          // Processing each raster line from the current block
          
          const unsigned int rasterLinesStart = (unsigned int)std::max( 0,
            (int)(rasterLinesBlockIdx * paramsPtr->m_maxRasterLinesBlockMaxSize ) - 
            (int)( 2 * moravecWindowRadius ) ); 
          const unsigned int rasterLinesEndBound = std::min( 1 +
            (rasterLinesBlockIdx * paramsPtr->m_maxRasterLinesBlockMaxSize ) + 
            paramsPtr->m_maxRasterLinesBlockMaxSize + 
            ( 2 * moravecWindowRadius ), rasterLines );
          const unsigned int varianceCalcStartRasterLineStart = rasterLinesStart + 
            moravecWindowWidth - 1;
          const unsigned int maximasLocationStartRasterLineStart = 
            varianceCalcStartRasterLineStart + moravecWindowWidth - 1;            
          unsigned int windowStartBufCol = 0;
          const unsigned int windowEndBufColsBound = bufferCols - 
            moravecWindowWidth;
          unsigned int windowStartBufOffset = 0;
          unsigned int windowStartBufXOffset = 0;
          unsigned int windowStartBufYOffset = 0;
          float horVar = 0;
          float verVar = 0;
          float diagVar = 0;
          float adiagVar = 0;
          float diffValue = 0;
          bool isLocalMaxima = false;
          InterestPointT auxInterestPoint;
          float neighborMaximaDif = 0;
                   
          for( unsigned int rasterLine = rasterLinesStart; rasterLine < rasterLinesEndBound ;
            ++rasterLine )
          {
            // read a new raster line into the last raster buffer line
            paramsPtr->m_rastaDataAccessMutexPtr->lock();
            
            roolUpBuffer( rasterBufferPtr, bufferLines );             
            memcpy( rasterBufferPtr[ lastBufferLineIdx ], 
              paramsPtr->m_rasterDataPtr->operator[]( rasterLine ),
              rasterBufferLineSizeBytes );
              
            // read a new mask raster line into the last mask raster buffer line
            if( paramsPtr->m_maskRasterDataPtr )
            {
              roolUpBuffer( maskRasterBufferPtr, bufferLines );
              memcpy( maskRasterBufferPtr[ lastBufferLineIdx ], 
                paramsPtr->m_maskRasterDataPtr->operator[]( rasterLine ),
                maskRasterBufferLineSizeBytes );
            }    
            
            paramsPtr->m_rastaDataAccessMutexPtr->unlock();
            
            // calc the diretional variance for last line from the
            // diretional variances buffer
            if( rasterLine >= varianceCalcStartRasterLineStart )
            {
              roolUpBuffer( maximasBufferPtr, bufferLines );
              
              for( windowStartBufCol = 0 ; windowStartBufCol < windowEndBufColsBound ; 
                ++windowStartBufCol )
              {
                const float& windowCenterPixelValue = rasterBufferPtr[
                  moravecWindowRadius ][ windowStartBufCol + 
                  moravecWindowRadius ];
                horVar = 0;
                verVar = 0;
                diagVar = 0;
                adiagVar = 0;                  
                
                for( windowStartBufOffset = 0 ; windowStartBufOffset < 
                  moravecWindowWidth ; ++windowStartBufOffset )
                {
                  diffValue = windowCenterPixelValue - rasterBufferPtr[ 
                    moravecWindowRadius ][ windowStartBufCol + 
                    windowStartBufOffset ];
                  horVar += ( diffValue * diffValue );
                    
                  diffValue = windowCenterPixelValue - rasterBufferPtr[ 
                    windowStartBufOffset ][ windowStartBufCol +
                    moravecWindowRadius ];
                  verVar += ( diffValue * diffValue );
                  
                  diffValue = windowCenterPixelValue - rasterBufferPtr[ 
                    windowStartBufOffset ][ windowStartBufCol +
                    windowStartBufOffset ];
                  diagVar += ( diffValue * diffValue );
                  
                  diffValue = windowCenterPixelValue - rasterBufferPtr[ 
                    moravecWindowWidth - 1 - windowStartBufOffset ][ windowStartBufCol +
                    windowStartBufOffset ];
                  adiagVar += ( diffValue * diffValue );
                }
                
                maximasBufferPtr[ lastBufferLineIdx ][ windowStartBufCol + 
                  moravecWindowRadius ] = std::min( horVar, std::min(
                  verVar, std::min( diagVar, adiagVar ) ) );
              }
            }
            
            // find the local maxima points for the diretional variances buffer
            // center line.
            if( rasterLine >= maximasLocationStartRasterLineStart )
            {
              for( windowStartBufCol = 0 ; windowStartBufCol < windowEndBufColsBound ; 
                ++windowStartBufCol )
              {
                isLocalMaxima = true;
                const float& windowCenterPixelValue = maximasBufferPtr[
                  moravecWindowRadius ][ windowStartBufCol + 
                  moravecWindowRadius ];       
                auxInterestPoint.m_feature1  = 0.0;
                
                for( windowStartBufYOffset = 0 ; windowStartBufYOffset < 
                  moravecWindowWidth ; ++windowStartBufYOffset )
                {
                  for( windowStartBufXOffset = 0 ; windowStartBufXOffset < 
                    moravecWindowWidth ; ++windowStartBufXOffset )
                  {
                    neighborMaximaDif = windowCenterPixelValue - maximasBufferPtr[
                      windowStartBufYOffset ][ windowStartBufCol + 
                      windowStartBufXOffset ];
                      
                    if( neighborMaximaDif < 0.0 )
                    {
                      isLocalMaxima = false;
                      windowStartBufYOffset = moravecWindowWidth;
                      break;
                    }
                    
                    auxInterestPoint.m_feature1 += std::abs( neighborMaximaDif );
                  }
                }
                
                if( isLocalMaxima )
                {
                  auxInterestPoint.m_x = windowStartBufCol + 
                    moravecWindowRadius;
                  auxInterestPoint.m_y = rasterLine - moravecWindowWidth + 1;
                  assert( auxInterestPoint.m_x < 
                    paramsPtr->m_rasterDataPtr->getColumnsNumber() );
                  assert( auxInterestPoint.m_y < 
                    paramsPtr->m_rasterDataPtr->getLinesNumber() );  
                  
                  if( maskRasterBufferPtr )
                  {
                    if( maskRasterBufferPtr[ 0 ][ auxInterestPoint.m_x ] )
                    {
                      blockMaximas.insert( auxInterestPoint);
                      
                      if( blockMaximas.size() > 
                        paramsPtr->m_maxInterestPointsPerRasterLinesBlock )
                      {
                        blockMaximas.erase( blockMaximas.begin() );
                      }
                    }
                  }
                  else
                  {
                    blockMaximas.insert( auxInterestPoint );
                    
                    if( blockMaximas.size() > 
                      paramsPtr->m_maxInterestPointsPerRasterLinesBlock )
                    {
                      blockMaximas.erase( blockMaximas.begin() );
                    }
                  }
                }
              }
            }
          }
          
          // Copying the best found block maximas to the external maximas container
          
          paramsPtr->m_interestPointsAccessMutexPtr->lock();
          
          unsigned int pointsToAdd = std::min( 
            paramsPtr->m_maxInterestPointsPerRasterLinesBlock, 
            (unsigned int)blockMaximas.size() );
          InterestPointsSetT::const_reverse_iterator blockMaximasIt =
            blockMaximas.rbegin();
            
          while( pointsToAdd )
          {
//            std::cout << std::endl << blockMaximasIt->m_featureValue
//              << std::endl;
              
            paramsPtr->m_interestPointsPtr->insert( *blockMaximasIt );
           
            ++blockMaximasIt;
            --pointsToAdd;
          }
          
          paramsPtr->m_interestPointsAccessMutexPtr->unlock();          
        }
        else
        {
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
        }
      }
    }  
    
    bool TiePointsLocatorMoravecStrategy::generateCorrelationFeatures( 
      const InterestPointsSetT& interestPoints,
      const unsigned int correlationWindowWidth,
      const FloatsMatrix& rasterData,
      FloatsMatrix& features,
      InterestPointsSetT& validInteresPoints )
    {
      validInteresPoints.clear();
      
      /* The radius of a feature window rotated by 90 degrees. 
        * over the input image */
      
      const unsigned int rotated90CorralationWindowRadius = (unsigned int)
        (
          std::ceil( 
            sqrt( 
              2 
              * 
              ( 
                ( (double)correlationWindowWidth ) 
                *
                ( (double)correlationWindowWidth )
              )
            ) / 2.0
          )
        );      
      
      // Locating the the valid interest points
        
      {
        /* The radius of a feature window rotated by 90 degrees. 
        * over the input image */
          
        const unsigned int rasterDataCols = rasterData.getColumnsNumber();
        const unsigned int rasterDataLines = rasterData.getLinesNumber();
        const unsigned int firstValidInterestPointX = 
          rotated90CorralationWindowRadius + 1;
        const unsigned int lastValidInterestPointX = rasterDataCols
          - rotated90CorralationWindowRadius - 2; 
        const unsigned int firstValidInterestPointY = 
          rotated90CorralationWindowRadius + 1;
        const unsigned int lastValidInterestPointY = rasterDataLines
          - rotated90CorralationWindowRadius - 2;        
        
        {
          InterestPointsSetT::const_iterator iPointsIt = interestPoints.begin();
          const InterestPointsSetT::const_iterator iPointsItEnd = interestPoints.end();        
          
          while( iPointsIt != iPointsItEnd )
          {
            if( ( iPointsIt->m_x >= firstValidInterestPointX ) &&
              ( iPointsIt->m_x <= lastValidInterestPointX ) &&
              ( iPointsIt->m_y >= firstValidInterestPointY ) &&
              ( iPointsIt->m_y <= lastValidInterestPointY ) )
            {
              validInteresPoints.insert( *iPointsIt );
            }
            
            ++iPointsIt;
          }
        }
      }
      
      // Allocating the features matrix
      
      const unsigned int featureElemsNmb = correlationWindowWidth * 
        correlationWindowWidth;
      const unsigned int featureSizeBytes = sizeof( float ) *
        featureElemsNmb;
      
      TERP_TRUE_OR_RETURN_FALSE( features.reset( 
        validInteresPoints.size(), featureElemsNmb ),
        "Cannot allocate features matrix" );      
        
      // Allocating the auxiliary features buffer
      
      boost::scoped_array< float > auxFeatureBufferHandler( 
        new float[ featureElemsNmb ] );
      float* auxFeatureBufferPtr = auxFeatureBufferHandler.get();

      // Creating features

      unsigned int curr_window_x_start = 0; //related to the current window over the hole image
      unsigned int curr_window_y_start = 0; //related to the current window over the hole image
      unsigned int curr_window_x_center = 0; //related to the current window over the hole image
      unsigned int curr_window_y_center = 0; //related to the current window over the hole image
      unsigned int curr_window_x_end = 0; // this coord is also counted in
      unsigned int curr_window_y_end = 0; // this coord is also counted in
      const unsigned int wind_radius = correlationWindowWidth / 2; //output window radius
      const float wind_radius_double = (float)wind_radius;
      unsigned int curr_x = 0;
      unsigned int curr_y = 0;
      float curr_x_minus_radius = 0;
      float curr_y_minus_radius = 0;
      unsigned int curr_offset = 0;
      float int_x_dir = 0;
      float int_y_dir = 0;
      float int_norm = 0;
      float rotated_curr_x = 0;/* rotaded coord - window ref */
      float rotated_curr_y = 0;/* rotaded coord - window ref */
      float rot_cos = 0;
      float rot_sin = 0;
      unsigned int rotated_curr_x_img = 0; //coords rotated but in the hole image reference
      unsigned int rotated_curr_y_img = 0; //coords rotated but in the hole image reference
      float featureElementsNormalizeFactor = 0.0;
      unsigned int featureElementIdx = 0;
      float* featurePtr = 0;
      float featureElementValue = 0.0;
      float featureElementMaxValue = 0.0;
      float featureElementMinValue = 0.0;
      
      InterestPointsSetT::const_iterator viPointsIt = validInteresPoints.begin();
      const InterestPointsSetT::const_iterator viPointsItEnd = validInteresPoints.end();      
      unsigned int validInteresPointsIndex = 0 ;
      
      while( viPointsIt != viPointsItEnd )
      {
        /* Calculating the current window position */
      
        curr_window_x_center = viPointsIt->m_x;
        assert( curr_window_x_center >= rotated90CorralationWindowRadius );
        assert( curr_window_x_center < ( rasterData.getColumnsNumber() - 1 -
          rotated90CorralationWindowRadius ) );
        curr_window_y_center = viPointsIt->m_y;
        assert( curr_window_y_center >= rotated90CorralationWindowRadius );
        assert( curr_window_y_center < ( rasterData.getLinesNumber() - 1 -
          rotated90CorralationWindowRadius ) );
        
        curr_window_x_start = curr_window_x_center - wind_radius;
        curr_window_y_start = curr_window_y_center - wind_radius;
        curr_window_x_end = curr_window_x_start + correlationWindowWidth - 1;
        curr_window_y_end = curr_window_y_start + correlationWindowWidth - 1;
          
        /* Estimating the intensity vector X direction */
        
        int_x_dir = 0;
        
        for( curr_y = curr_window_y_start ; curr_y <= curr_window_y_end ; 
          ++curr_y ) 
        {        
          for( curr_offset = 0 ; curr_offset < wind_radius ;
            ++curr_offset ) 
          {
            int_x_dir += rasterData( curr_y, curr_window_x_end - curr_offset ) -
              rasterData( curr_y, curr_window_x_start + curr_offset );
          }
        }
        
        /* Estimating the intensity vector y direction */
        
        int_y_dir = 0;
        
        for( curr_x = curr_window_x_start ; curr_x <= curr_window_x_end ;
          ++curr_x )        
        {
          for( curr_offset = 0 ; curr_offset < wind_radius ; ++curr_offset )
          {
            int_y_dir += rasterData( curr_window_y_start + curr_offset, curr_x ) -
              rasterData( curr_window_y_end - curr_offset, curr_x );
          }
        }      
        
        /* Calculating the rotation parameters - 
        */
        int_norm = std::sqrt( ( int_x_dir * int_x_dir ) + 
          ( int_y_dir * int_y_dir ) );
        
        if( int_norm != 0.0 ) {
          rot_cos = int_x_dir / int_norm;
          rot_sin = int_y_dir / int_norm;
        } else {
          /* No rotation */
          rot_cos = 1.0;
          rot_sin = 0.0;
        }
        
        assert( ( rot_cos >= -1.0 ) && ( rot_cos <= 1.0 ) );
        assert( ( rot_sin >= -1.0 ) && ( rot_sin <= 1.0 ) );
        
        /* Generating the rotated window data and inserting it into 
          the img_features_matrix by setting the intensity vector
          to the direction (1,0) 

          counterclockwise rotation 
          | u |    |cos  -sin|   |X|
          | v | == |sin   cos| x |Y|

          clockwise rotation
          | u |    |cos   sin|   |X|
          | v | == |-sin  cos| x |Y|
        */
          
        memset( auxFeatureBufferPtr, 0, featureSizeBytes );
        featureElementIdx = 0;
        featureElementMaxValue = -1.0 * FLT_MAX;
        featureElementMinValue =  FLT_MAX;
        
        for( curr_y = 0 ; curr_y < correlationWindowWidth ; ++curr_y ) 
        {
          for( curr_x = 0 ; curr_x < correlationWindowWidth ; ++curr_x ) 
          {
            /* briging the window to the coord system center */
            
            curr_x_minus_radius = ((float)curr_x) - 
              wind_radius_double;
            curr_y_minus_radius = ((float)curr_y) - 
              wind_radius_double;
            
            /* rotating the centered window */
            
            rotated_curr_x = 
              ( rot_cos * curr_x_minus_radius ) + 
              ( rot_sin * curr_y_minus_radius );
            
            rotated_curr_y = 
              ( rot_cos * curr_y_minus_radius )
              - ( rot_sin * curr_x_minus_radius );
              
            /* bringing the window back to its original
              location with the correct new scale */ 
              
            rotated_curr_x += wind_radius_double;
            rotated_curr_y += wind_radius_double;
            
            /* copy the new rotated window to the output vector */
              
            rotated_curr_x_img = curr_window_x_start +
              (unsigned int)ROUND( rotated_curr_x );
            rotated_curr_y_img = curr_window_y_start +
              (unsigned int)ROUND( rotated_curr_y );  
              
            featureElementValue = rasterData( rotated_curr_y_img, 
              rotated_curr_x_img );
            
            auxFeatureBufferPtr[ featureElementIdx++ ] = featureElementValue;
            
            if( featureElementMaxValue < featureElementValue )
              featureElementMaxValue = featureElementValue;
            
            if( featureElementMinValue > featureElementValue )
              featureElementMinValue = featureElementValue;
            
          }
        }
        
        // feature normaliztion
        
        if( featureElementMaxValue == featureElementMinValue )
          featureElementsNormalizeFactor = 0.0;
        else
          featureElementsNormalizeFactor = 1.0f / ( featureElementMaxValue -
            featureElementMinValue );
        
        featurePtr = features[ validInteresPointsIndex ];
        
        for( featureElementIdx = 0 ; featureElementIdx < featureElemsNmb ;
          ++featureElementIdx )
        {
          featurePtr[ featureElementIdx ] = (
            ( auxFeatureBufferPtr[ featureElementIdx ] - featureElementMinValue )
            * featureElementsNormalizeFactor );
          assert( featurePtr[ featureElementIdx ] >= 0.0 );
          assert( featurePtr[ featureElementIdx ] <= 1.0 );
        }
        
        ++validInteresPointsIndex;
        ++viPointsIt;
      }
     
      return true;
    }          
    
    bool TiePointsLocatorMoravecStrategy::executeMatchingByCorrelation( 
      const FloatsMatrix& featuresSet1,
      const FloatsMatrix& featuresSet2,
      const InterestPointsSetT& interestPointsSet1,
      const InterestPointsSetT& interestPointsSet2,
      const unsigned int maxPt1ToPt2Distance,
      const unsigned int enableMultiThread,
      const double minAllowedAbsCorrelation,
      MatchedInterestPointsSetT& matchedPoints )
    {
      matchedPoints.clear();
      
      const unsigned int interestPointsSet1Size = interestPointsSet1.size();
      if( interestPointsSet1Size == 0 ) return true;
      
      const unsigned int interestPointsSet2Size = interestPointsSet2.size();
      if( interestPointsSet2Size == 0 ) return true;
      
      assert( featuresSet1.getColumnsNumber() == featuresSet2.getColumnsNumber() );
      assert( featuresSet1.getLinesNumber() == interestPointsSet1Size );
      assert( featuresSet2.getLinesNumber() == interestPointsSet2Size );
      
      // Creating internal objects
      
      InterestPointsSetT::const_iterator it1 = interestPointsSet1.begin();
      boost::scoped_array< InterestPointT > internalInterestPointsSet1( 
        new InterestPointT[ interestPointsSet1Size ] );
      for( unsigned int idx1 = 0 ; idx1 < interestPointsSet1Size ; ++idx1 )
      {
        internalInterestPointsSet1[ idx1 ] = *it1;
        
        ++it1;
      }
      
      InterestPointsSetT::const_iterator it2 = interestPointsSet2.begin();
      boost::scoped_array< InterestPointT > internalInterestPointsSet2( 
        new InterestPointT[ interestPointsSet2Size ] );
      for( unsigned int idx2 = 0 ; idx2 < interestPointsSet2Size ; ++idx2 )
      {
        internalInterestPointsSet2[ idx2 ] = *it2;
        
        ++it2;
      }        
      
      // Creating the correlation matrix
      
      FloatsMatrix corrMatrix;
      TERP_TRUE_OR_RETURN_FALSE( corrMatrix.reset( interestPointsSet1Size,
       interestPointsSet2Size, FloatsMatrix::RAMMemPol ),
        "Error crearting the correlation matrix" );
        
      unsigned int col = 0;
      unsigned int line = 0;
      float* linePtr = 0;
      
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        linePtr = corrMatrix[ line ];
        
        for( col = 0 ; col < interestPointsSet2Size ; ++col )
        {
          linePtr[ col ] = 0;
        }
      }
      
      boost::mutex syncMutex;
      unsigned int nextFeatureIdx1ToProcess = 0;
//      unsigned int nextFeatureIdx2ToProcess = 0;
      
      ExecuteMatchingByCorrelationThreadEntryParams params;
      params.m_featuresSet1Ptr = &featuresSet1;
      params.m_featuresSet2Ptr = &featuresSet2;
      params.m_interestPointsSet1Ptr = internalInterestPointsSet1.get();
      params.m_interestPointsSet2Ptr = internalInterestPointsSet2.get();
      params.m_nextFeatureIdx1ToProcessPtr = &nextFeatureIdx1ToProcess;
      params.m_corrMatrixPtr = &corrMatrix;
      params.m_syncMutexPtr = &syncMutex;
      params.m_maxPt1ToPt2Distance = maxPt1ToPt2Distance;
      
      if( enableMultiThread )
      {
        TERP_TRUE_OR_RETURN_FALSE( featuresSet1.getMemPolicy() ==
          FloatsMatrix::RAMMemPol, "Invalid memory policy" )
        TERP_TRUE_OR_RETURN_FALSE( featuresSet2.getMemPolicy() ==
          FloatsMatrix::RAMMemPol, "Invalid memory policy" )    
          
        const unsigned int procsNumber = te::common::GetPhysProcNumber();
        
        boost::thread_group threads;
        
        for( unsigned int threadIdx = 0 ; threadIdx < procsNumber ;
          ++threadIdx )
        {
          threads.add_thread( new boost::thread( 
            executeMatchingByCorrelationThreadEntry, &params ) );
        }
        
        threads.join_all();          
          
      }
      else
      {
        executeMatchingByCorrelationThreadEntry( &params );
      }
      
      // finding the correlation matrix maximas for each line and column
      
      std::vector< float > eachLineMaxABSValues( interestPointsSet1Size,
        0.0 );
      std::vector< unsigned int > eachLineMaxABSIndexes( interestPointsSet1Size,
        interestPointsSet2Size );
      std::vector< float > eachColMaxABSValues( interestPointsSet2Size,
        0.0 );
      std::vector< unsigned int > eachColMaxABSIndexes( interestPointsSet2Size,
        interestPointsSet1Size );
      float absValue = 0;
        
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        linePtr = corrMatrix[ line ];
        
        for( col = 0 ; col < interestPointsSet2Size ; ++col )
        {
          absValue = std::abs( linePtr[ col ] );
          
          if( absValue >= minAllowedAbsCorrelation )
          {
            if( absValue > eachLineMaxABSValues[ line ] )
            {
              eachLineMaxABSValues[ line ] = absValue;
              eachLineMaxABSIndexes[ line ] = col;
            }
            
            if( absValue > eachColMaxABSValues[ col ] )
            {
              eachColMaxABSValues[ col ] = absValue;
              eachColMaxABSIndexes[ col ] = line;
            }
          }
        }
      }
      
      // Finding tiepoints
      
      MatchedInterestPointsT auxMatchedPoints;
        
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        col = eachLineMaxABSIndexes[ line ];
        
        if( ( col < interestPointsSet2Size ) &&
          ( eachColMaxABSIndexes[ col ] == line ) )
        {
          auxMatchedPoints.m_point1 = internalInterestPointsSet1[ line ];
          auxMatchedPoints.m_point2 = internalInterestPointsSet2[ col ];
          auxMatchedPoints.m_feature = std::abs( corrMatrix( line, col ) );
          
          matchedPoints.insert( auxMatchedPoints );
        }
      }
        
      return true;
    }
    
    void TiePointsLocatorMoravecStrategy::executeMatchingByCorrelationThreadEntry(
      ExecuteMatchingByCorrelationThreadEntryParams* paramsPtr)
    {
      assert( paramsPtr->m_featuresSet1Ptr->getMemPolicy() == 
        FloatsMatrix::RAMMemPol );
      assert( paramsPtr->m_featuresSet2Ptr->getMemPolicy() == 
        FloatsMatrix::RAMMemPol );
      assert( paramsPtr->m_corrMatrixPtr->getMemPolicy() == 
        FloatsMatrix::RAMMemPol );
        
      // globals
        
      const unsigned int featureElementsNmb = paramsPtr->m_featuresSet1Ptr->getColumnsNumber();
      unsigned int feat2Idx = 0;
      float const* feat1Ptr = 0;
      float const* feat2Ptr = 0;
      float* corrMatrixLinePtr = 0;
      unsigned int featCol = 0;
      float sumAA = 0;
      float sumBB = 0;
      float cc_norm = 0;
      float ccorrelation = 0;
      te::gm::Envelope auxEnvelope;        
        
      // Indexing tree building
      
      const unsigned int featuresSet1Size = 
        paramsPtr->m_featuresSet1Ptr->getLinesNumber();
      const unsigned int featuresSet2Size = 
        paramsPtr->m_featuresSet2Ptr->getLinesNumber();   
        
      te::sam::rtree::Index< unsigned int > interestPointsSet2RTree;
      
      std::vector< unsigned int > selectedFeaturesSet2Indexes;
      unsigned int selectedFeaturesSet2IndexesSize = 0;      
        
      if( paramsPtr->m_maxPt1ToPt2Distance )
      {
        for( unsigned int feat2Idx = 0 ; feat2Idx < featuresSet2Size ; ++feat2Idx )
        {
          interestPointsSet2RTree.insert( 
            te::gm::Envelope( 
              paramsPtr->m_interestPointsSet2Ptr[ feat2Idx ].m_x, 
              paramsPtr->m_interestPointsSet2Ptr[ feat2Idx ].m_y,
              paramsPtr->m_interestPointsSet2Ptr[ feat2Idx ].m_x, 
              paramsPtr->m_interestPointsSet2Ptr[ feat2Idx ].m_y ), 
            feat2Idx );
        }         
      }
      else
      {
        selectedFeaturesSet2Indexes.resize( featuresSet2Size );
        selectedFeaturesSet2IndexesSize = featuresSet2Size;
        for( unsigned int feat2Idx = 0 ; feat2Idx < featuresSet2Size ; ++feat2Idx )
        {
          selectedFeaturesSet2Indexes[ feat2Idx ] = feat2Idx;
        }
      }
      
      // Analysing each feature
      
      for( unsigned int feat1Idx = 0 ; feat1Idx < featuresSet1Size ; ++feat1Idx )
      {
        paramsPtr->m_syncMutexPtr->lock();
        
        if( feat1Idx == (*paramsPtr->m_nextFeatureIdx1ToProcessPtr) )
        {
          ++(*paramsPtr->m_nextFeatureIdx1ToProcessPtr);
          
          paramsPtr->m_syncMutexPtr->unlock();
          
          if( paramsPtr->m_maxPt1ToPt2Distance )
          {
            auxEnvelope.m_llx = auxEnvelope.m_urx = 
              paramsPtr->m_interestPointsSet1Ptr[ feat1Idx ].m_x;
            auxEnvelope.m_llx -= (double)paramsPtr->m_maxPt1ToPt2Distance;
            auxEnvelope.m_urx += (double)paramsPtr->m_maxPt1ToPt2Distance;
            auxEnvelope.m_lly = auxEnvelope.m_ury = 
              paramsPtr->m_interestPointsSet1Ptr[ feat1Idx ].m_y;
            auxEnvelope.m_lly -= (double)paramsPtr->m_maxPt1ToPt2Distance;;
            auxEnvelope.m_ury += (double)paramsPtr->m_maxPt1ToPt2Distance;;
            
            selectedFeaturesSet2Indexes.clear();
            interestPointsSet2RTree.search( auxEnvelope,
              selectedFeaturesSet2Indexes );
              
            selectedFeaturesSet2IndexesSize = selectedFeaturesSet2Indexes.size();
          }          
          
          corrMatrixLinePtr = paramsPtr->m_corrMatrixPtr->operator[]( feat1Idx );
          
          feat1Ptr = paramsPtr->m_featuresSet1Ptr->operator[]( feat1Idx );          
          
          for( unsigned int selectedFSIIdx = 0 ; selectedFSIIdx < 
            selectedFeaturesSet2IndexesSize ; ++selectedFSIIdx )
          {
            feat2Idx = selectedFeaturesSet2Indexes[ selectedFSIIdx ];
            
            feat2Ptr = paramsPtr->m_featuresSet2Ptr->operator[]( feat2Idx );
            
            sumAA = 0.0;
            sumBB = 0.0;   
            for( featCol = 0 ; featCol < featureElementsNmb ; ++featCol )
            {
              sumAA += feat1Ptr[ featCol ] * feat1Ptr[ featCol ];
              sumBB += feat2Ptr[ featCol ] * feat2Ptr[ featCol ];
            }
            
            cc_norm = std::sqrt( sumAA * sumBB );
            
            if( cc_norm == 0.0 )
            {
              corrMatrixLinePtr[ feat2Idx ] = 0;
            }
            else
            {
              ccorrelation = 0.0;
              for( featCol = 0 ; featCol < featureElementsNmb ; ++featCol )
              {
                ccorrelation += ( feat1Ptr[ featCol ] * feat2Ptr[ featCol ] ) / 
                  cc_norm;
              }
                
              corrMatrixLinePtr[ feat2Idx ] = ccorrelation;            
            }
          }
        }
        else
        {
          paramsPtr->m_syncMutexPtr->unlock();
        }
      }
    }    

  } // end namespace rp
}   // end namespace te    

