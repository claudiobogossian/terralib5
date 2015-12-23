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
  \file terralib/rp/TiePointsLocatorSURFStrategy.cpp
  \brief Tie-Pointsr locator SURF strategy.
*/

#include "TiePointsLocatorSURFStrategy.h"
#include "Macros.h"
#include "../common/progress/TaskProgress.h"
#include "../sam/rtree.h"

#include <memory>

#include <boost/shared_array.hpp>

namespace
{
  static te::rp::TiePointsLocatorSURFStrategyFactory 
    TiePointsLocatorSURFStrategyFactoryInstance;
}

namespace te
{
  namespace rp
  {
    TiePointsLocatorSURFStrategy::Parameters::Parameters()
    {
      reset();
    }
    
    TiePointsLocatorSURFStrategy::Parameters::Parameters( 
      const TiePointsLocatorSURFStrategy::Parameters& other )
    {
      reset();
      operator=( other );
    }    

    TiePointsLocatorSURFStrategy::Parameters::~Parameters()
    {
      reset();
    }

    void TiePointsLocatorSURFStrategy::Parameters::reset() 
      throw( te::rp::Exception )
    {
      m_surfScalesNumber = 3;
      m_surfOctavesNumber = 2;
      m_surfMaxNormEuclideanDist = 0.75;
    }

    const TiePointsLocatorSURFStrategy::Parameters& 
      TiePointsLocatorSURFStrategy::Parameters::operator=(
      const TiePointsLocatorSURFStrategy::Parameters& params )
    {
      reset();
      
      m_surfScalesNumber = params.m_surfScalesNumber;
      m_surfOctavesNumber = params.m_surfOctavesNumber;
      m_surfMaxNormEuclideanDist = params.m_surfMaxNormEuclideanDist;

      return *this;
    }

    te::common::AbstractParameters* 
      TiePointsLocatorSURFStrategy::Parameters::clone() const
    {
      return new Parameters( *this );
    }    
    
    /* ------------------------------------------------------------------------*/    
    
    TiePointsLocatorSURFStrategy::TiePointsLocatorSURFStrategy()
    {
      reset();
    }
    
    TiePointsLocatorSURFStrategy::~TiePointsLocatorSURFStrategy()
    {
      reset();
    }
    
    void TiePointsLocatorSURFStrategy::getSubSampledSpecStrategyParams( 
      const double subSampleOptimizationRescaleFactor,
      const TiePointsLocatorStrategyParameters& inputSpecParams,
      std::auto_ptr< TiePointsLocatorStrategyParameters >& subSampledSpecParamsPtr ) const
    {
      Parameters const* inputSpecParamsPtr = dynamic_cast< Parameters const* >( &inputSpecParams );
      TERP_TRUE_OR_THROW( inputSpecParamsPtr, "Invalid specific parameters" );      
      
      subSampledSpecParamsPtr.reset( (te::rp::TiePointsLocatorStrategyParameters*)inputSpecParams.clone() );
      
      Parameters* subSampledSpecParamsNakedPtr = dynamic_cast< Parameters* >( subSampledSpecParamsPtr.get() );
      TERP_TRUE_OR_THROW( subSampledSpecParamsNakedPtr, "Invalid specific parameters" );
      
      subSampledSpecParamsNakedPtr->m_surfScalesNumber =
        3 
        +
        (unsigned int)
        ( 
          ((double)( inputSpecParamsPtr->m_surfScalesNumber - 3 ))
          * 
          subSampleOptimizationRescaleFactor
        );            
      subSampledSpecParamsNakedPtr->m_surfOctavesNumber =
        1 
        +
        (unsigned int)
        ( 
          ((double)( inputSpecParamsPtr->m_surfOctavesNumber - 1 ))
          * 
          subSampleOptimizationRescaleFactor
        );     
    }
    
    void TiePointsLocatorSURFStrategy::getDefaultSpecStrategyParams( 
      std::auto_ptr< TiePointsLocatorStrategyParameters >& defaultSpecParamsPtr ) const
    {
      defaultSpecParamsPtr.reset( new Parameters() );
    }    
    
    bool TiePointsLocatorSURFStrategy::initialize( 
      const te::rp::TiePointsLocatorInputParameters& inputParameters )
    {
      m_inputParameters = inputParameters;
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster1Bands.size()
        == 1, "Invalid number of raster 1 bands" );
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster2Bands.size()
        == 1, "Invalid number of raster 2 bands" );
        
      Parameters const* specParsPtr = dynamic_cast< Parameters const* >( 
        inputParameters.getSpecStrategyParams() );
      TERP_TRUE_OR_RETURN_FALSE( specParsPtr, "Invalid specific parameters" );      
      
      TERP_TRUE_OR_RETURN_FALSE( specParsPtr->m_surfScalesNumber > 2,
        "Invalid m_surfScalesNumber" );        
        
      TERP_TRUE_OR_RETURN_FALSE( specParsPtr->m_surfOctavesNumber > 0,
        "Invalid m_surfOctavesNumber" );           
        
      TERP_TRUE_OR_RETURN_FALSE( ( specParsPtr->m_surfMaxNormEuclideanDist >= 0 ) &&
        ( specParsPtr->m_surfMaxNormEuclideanDist <= 1.0 ),
        "Invalid m_surfMaxNormEuclideanDist" );                
      
      m_isInitialized = true;
      
      // Defining the number of tie points
      
      if( m_inputParameters.m_maxTiePoints == 0 )
      {
        m_inputParameters.m_maxTiePoints = getAutoMaxTiePointsNumber();
      }      
      
      return true;
    }
    
    void TiePointsLocatorSURFStrategy::reset()
    {
      m_isInitialized = false;
      m_inputParameters.reset();
    }
    
    bool TiePointsLocatorSURFStrategy::getMatchedInterestPoints( 
      te::gm::GeometricTransformation const * const raster1ToRaster2TransfPtr,
      const double raster1ToRaster2TransfDMapError,
      MatchedInterestPointsSetT& matchedInterestPoints )
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
      
      // Defining the maximum number of interest points 
      
      unsigned int maxInterestPoints1 = m_inputParameters.m_maxTiePoints;
      unsigned int maxInterestPoints2 = m_inputParameters.m_maxTiePoints;
      
      {
        const double width1 = ((double)m_inputParameters.m_raster1TargetAreaWidth)
          * raster1XRescFact;
        const double height1 = ((double)m_inputParameters.m_raster1TargetAreaHeight)
          * raster1YRescFact;

        const double width2 = ((double)m_inputParameters.m_raster2TargetAreaWidth)
          * raster2XRescFact;
        const double height2 = ((double)m_inputParameters.m_raster2TargetAreaHeight)
          * raster2YRescFact;
          
        const double area1 = width1 * height1;
        const double area2 = width2 * height2;
        
         if( area1 > area2 )
         {
           maxInterestPoints1 = (unsigned int)( ((double)maxInterestPoints1)*( area1 / area2 ) );
         }
         else
         {
           maxInterestPoints2 = (unsigned int)( ((double)maxInterestPoints2)*( area2 / area1 ) );
         }
      }      
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        progressPtr->setTotalSteps( 10 );
        progressPtr->setMessage( "Locating tie points" );
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      } 
      
      // Locating interest points and features from raster 1
      
      InterestPointsSetT raster1InterestPoints;
      FloatsMatrix raster1Features;
      {
        // Loading image data
        
        std::vector< boost::shared_ptr< FloatsMatrix > > rasterData;
        UCharsMatrix maskRasterData;
        double achievedRescaleFactorX = 0;
        double achievedRescaleFactorY = 0;
        
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
          rasterData,
          maskRasterData,
          achievedRescaleFactorX,
          achievedRescaleFactorY ) )
        {
          return false;
        }
        
        raster1XRescFact = achievedRescaleFactorX;
        raster1YRescFact = achievedRescaleFactorY;
          
//        printMatrix( *(rasterData[ 0 ]) );
//        createTifFromMatrix( *(rasterData[ 0 ]), InterestPointsSetT(), "loadedRaster1");

        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }           
        
        // Creating the integral image
        
        FloatsMatrix integralRaster;
        
        TERP_TRUE_OR_RETURN_FALSE( createIntegralImage( *(rasterData[ 0 ]), 
          integralRaster ), "Integral image creation error" );
          
        rasterData.clear();
        
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }            
          
//        printMatrix( integralRaster );
//        createTifFromMatrix( integralRaster, InterestPointsSetT(), "integralRaster1" );
        
        // locating interest points        
        
        TERP_TRUE_OR_RETURN_FALSE( locateSurfInterestPoints( 
          maxInterestPoints1,
          integralRaster, 
          maskRasterData.getLinesNumber() ? (&maskRasterData) : 0, 
          raster1InterestPoints ),
          "Error locating raster 1 interest points" );
          
//        createTifFromMatrix( *(rasterData[ 0 ]), raster1InterestPoints, "surfInterestPoints1");

        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }          

        
        InterestPointsSetT validInterestPoints;
        
        TERP_TRUE_OR_RETURN_FALSE( generateSurfFeatures( raster1InterestPoints,
          integralRaster, validInterestPoints, raster1Features ),
          "Error generating raster features" );        
          
        // Bring interest points to full raster indexed coords reference
      
        raster1InterestPoints.clear();
        
        {
          InterestPointsSetT::iterator itB = validInterestPoints.begin();
          const InterestPointsSetT::iterator itE = validInterestPoints.end();
          InterestPointT auxIP;
          
          while( itB != itE )
          {
            auxIP = *itB;
            auxIP.m_x = ( auxIP.m_x / raster1XRescFact ) + 
              (double)m_inputParameters.m_raster1TargetAreaColStart;
            auxIP.m_y = ( auxIP.m_y / raster1YRescFact ) + 
              (double)m_inputParameters.m_raster1TargetAreaLineStart;  
              
            raster1InterestPoints.insert( auxIP );
              
            ++itB;
          }
        }
          
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }             
          
      }
      
      // Locating interest points and features from raster 2
      
      InterestPointsSetT raster2InterestPoints;
      FloatsMatrix raster2Features;
      {
        // Loading image data
        
        std::vector< boost::shared_ptr< FloatsMatrix > > rasterData;
        UCharsMatrix maskRasterData;
        double achievedRescaleFactorX = 0;
        double achievedRescaleFactorY = 0;        
        
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
          rasterData,
          maskRasterData,
          achievedRescaleFactorX,
          achievedRescaleFactorY ) )
        {
          return false;
        }
        
        raster2XRescFact = achievedRescaleFactorX;
        raster2YRescFact = achievedRescaleFactorY;
          
//        printMatrix( *(rasterData[ 0 ]) );
//        createTifFromMatrix( *(rasterData[ 0 ]), InterestPointsSetT(), "loadedRaster2");

        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }   
        
        // Creating the integral image
        
        FloatsMatrix integralRaster;
        
        TERP_TRUE_OR_RETURN_FALSE( createIntegralImage( *(rasterData[ 0 ]), 
          integralRaster ), "Integral image creation error" );
          
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }             
          
        rasterData.clear();
          
//        printMatrix( integralRaster );
//        createTifFromMatrix( integralRaster, InterestPointsSetT(), "integralRaster2" );
        
        // locating interest points        
        
        TERP_TRUE_OR_RETURN_FALSE( locateSurfInterestPoints( 
          maxInterestPoints2,
          integralRaster, 
          maskRasterData.getLinesNumber() ? (&maskRasterData) : 0, 
          raster2InterestPoints ),
          "Error locating raster interest points" );
          
//        createTifFromMatrix( *(rasterData[ 0 ]), raster2InterestPoints, "surfInterestPoints2");

        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }           

        InterestPointsSetT validInterestPoints;
        
        TERP_TRUE_OR_RETURN_FALSE( generateSurfFeatures( raster2InterestPoints,
          integralRaster, validInterestPoints, raster2Features ),
          "Error generating raster features" );
          
        // Bring interest points to full raster indexed coords reference
      
        raster2InterestPoints.clear();
        
        {
          InterestPointsSetT::iterator itB = validInterestPoints.begin();
          const InterestPointsSetT::iterator itE = validInterestPoints.end();
          InterestPointT auxIP;
          
          while( itB != itE )
          {
            auxIP = *itB;
            auxIP.m_x = ( auxIP.m_x / raster2XRescFact ) + 
              (double)m_inputParameters.m_raster2TargetAreaColStart;
            auxIP.m_y = ( auxIP.m_y / raster2YRescFact ) + 
              (double)m_inputParameters.m_raster2TargetAreaLineStart;          
              
            raster2InterestPoints.insert( auxIP );
              
            ++itB;
          }
        }
          
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }            
      }
      
//      printMatrix( raster1Features );
//      printMatrix( raster2Features );
      
      // Matching features
      
      MatchedInterestPointsSetT internalMatchedInterestPoints;
      
      TERP_TRUE_OR_RETURN_FALSE( executeMatchingByEuclideanDist( 
        raster1Features,
        raster2Features,
        raster1InterestPoints,
        raster2InterestPoints,
        raster1ToRaster2TransfPtr,
        raster1ToRaster2TransfDMapError,
        internalMatchedInterestPoints ),
        "Error matching features" );
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }   
      
      // Removing the repeated matched points (those present in more than one scale)
      // keeping the higher MatchedInterestPointsT::m_feature value
      
      {
        MatchedInterestPointsSetT::iterator it1 = internalMatchedInterestPoints.begin();
        MatchedInterestPointsSetT::iterator it2;
        MatchedInterestPointsSetT::iterator eraseIt;
        
        while( it1 != internalMatchedInterestPoints.end() )
        {
          it2 = it1;
          ++it2;
          eraseIt = internalMatchedInterestPoints.end();
          
          while( it2 != internalMatchedInterestPoints.end() )
          {
            if( 
                ( it1->m_point1.m_x == it2->m_point1.m_x )
                &&
                ( it1->m_point1.m_y == it2->m_point1.m_y )
                &&
                ( it1->m_point2.m_x == it2->m_point2.m_x )
                &&
                ( it1->m_point2.m_y == it2->m_point2.m_y )   
              )
            {
              if( it1->m_feature < it2->m_feature )
              {
                eraseIt = it1;
              }
              else if( it1->m_feature > it2->m_feature )
              {
                eraseIt = it2;
              }
              
              break;
            }
            else
            {
              ++it2;
            }
          }
          
          if( eraseIt == it1 )
          {
            ++it1;
            internalMatchedInterestPoints.erase( eraseIt );
          }
          else if( eraseIt != internalMatchedInterestPoints.end() )
          {
            internalMatchedInterestPoints.erase( eraseIt );
            ++it1;
          }
          else
          {
            ++it1;
          }
        }
      }
      
      // Removing the worse mathed points to fitting the required amount of matched points      
      
      while( internalMatchedInterestPoints.size() > m_inputParameters.m_maxTiePoints )
      {
        internalMatchedInterestPoints.erase( internalMatchedInterestPoints.begin() );
      }
      
      // Generating the  output matched interest points
      
      {
        MatchedInterestPointsT auxMatchedPoints;
        MatchedInterestPointsSetT::const_iterator itB = internalMatchedInterestPoints.begin();
        const MatchedInterestPointsSetT::const_iterator itE = internalMatchedInterestPoints.end();
        
        float minFeature1P1 = FLT_MAX;
        float maxFeature1P1 = (-1.0) * FLT_MAX;
        float minFeature1P2 = FLT_MAX;
        float maxFeature1P2 = (-1.0) * FLT_MAX;
        float minDist = FLT_MAX;
        float maxDist = (-1.0 ) *FLT_MAX;
        
        while( itB != itE )
        {
          if( minFeature1P1 > itB->m_point1.m_feature1 )
            minFeature1P1 = itB->m_point1.m_feature1;
          if( maxFeature1P1 < itB->m_point1.m_feature1 )
            maxFeature1P1 = itB->m_point1.m_feature1;
          
          if( minFeature1P2 > itB->m_point2.m_feature1 )
            minFeature1P2 = itB->m_point2.m_feature1;
          if( maxFeature1P2 < itB->m_point2.m_feature1 )
            maxFeature1P2 = itB->m_point2.m_feature1;        
          
          if( minDist > itB->m_feature )
            minDist = itB->m_feature;
          if( maxDist < itB->m_feature )
            maxDist = itB->m_feature;
          
          ++itB;
        }
        
        float feature1P1Range = maxFeature1P1 - minFeature1P1;
        float feature1P2Range = maxFeature1P2 - minFeature1P2;
        float distRange = maxDist - minDist;

        if( ( feature1P1Range == 0.0 ) || ( feature1P2Range == 0.0 ) ||
          ( distRange == 0.0 ) )
        {
          itB = internalMatchedInterestPoints.begin();
          
          while( itB != itE )
          {
            auxMatchedPoints = *itB;
            auxMatchedPoints.m_feature = 1.0;
            
            matchedInterestPoints.insert( auxMatchedPoints );
           
            ++itB;
          }
        }
        else
        {
          itB = internalMatchedInterestPoints.begin();
          
          while( itB != itE )
          {
            auxMatchedPoints = *itB;
            auxMatchedPoints.m_feature = 
                (
                  (
                    ( maxDist - auxMatchedPoints.m_feature )
                    /
                    distRange
                  )
                  +
                  (
                    ( auxMatchedPoints.m_point1.m_feature1 - minFeature1P1 ) 
                    /
                    feature1P1Range
                  )
                  +
                  (
                    ( auxMatchedPoints.m_point2.m_feature1 - minFeature1P2 ) 
                    /
                    feature1P2Range
                  )
                ) 
                / 
                3.0;
              
            matchedInterestPoints.insert( auxMatchedPoints );
            
            ++itB;
          }
        }
      }                  

      return true;
    }
    
    unsigned int TiePointsLocatorSURFStrategy::getAutoMaxTiePointsNumber() const
    {
      TERP_TRUE_OR_THROW( m_isInitialized, "Not initialized instance" );
      
      unsigned int returnValue = 0;
      
      const unsigned int maxRastersArea = (unsigned int)
          std::max(
            (
              ((double)( m_inputParameters.m_raster1TargetAreaWidth)) 
              *
              m_inputParameters.m_subSampleOptimizationRescaleFactor
              *
              ((double)(m_inputParameters.m_raster1TargetAreaHeight))
              *
              m_inputParameters.m_subSampleOptimizationRescaleFactor
            )
            ,
            (
              ((double)( m_inputParameters.m_raster2TargetAreaWidth)) 
              *
              m_inputParameters.m_subSampleOptimizationRescaleFactor
              *
              ((double)(m_inputParameters.m_raster2TargetAreaHeight))
              *
              m_inputParameters.m_subSampleOptimizationRescaleFactor
            )
          );
            
      const unsigned int filterWindowSize = ( getSurfOctaveBaseFilterSize( 0 ) +
        getSurfFilterSize( ((Parameters*)m_inputParameters.getSpecStrategyParams())->m_surfOctavesNumber - 1,
        ((Parameters*)m_inputParameters.getSpecStrategyParams())->m_surfScalesNumber - 1 ) )  / 2;
        
      returnValue = maxRastersArea /            
        ( filterWindowSize * filterWindowSize );

      // This is because the features and matching matrix bare eing allocated in RAM
        
      const double totalPhysMem = (double)te::common::GetTotalPhysicalMemory();
      const double usedVMem = (double)te::common::GetUsedVirtualMemory();
      const double totalVMem = (double)te::common::GetTotalVirtualMemory();
      const double freeVMem = 0.4 * std::min( totalPhysMem, ( totalVMem - usedVMem ) );
      
      const double featureElementsNumber = 65 * 65;
      
      double maxFeaturesMemory =
        std::max(
          0.0
          ,
          (
            (-2.0) * featureElementsNumber 
            +
            std::sqrt( 
              ( 4.0 * featureElementsNumber * featureElementsNumber )
              +
              ( 4.0 * freeVMem / ((double)sizeof( float ) ) )
            )
          )
        ); 
      maxFeaturesMemory =
        std::max(
          maxFeaturesMemory
          ,
          (
            (-2.0) * featureElementsNumber 
            -
            std::sqrt( 
              ( 4.0 * featureElementsNumber * featureElementsNumber )
              +
              ( 4.0 * freeVMem / ((double)sizeof( float ) ) )
            )
          )
        );      
      
      returnValue = std::min( returnValue, (unsigned int)maxFeaturesMemory ); 
        
      return returnValue;
    }    
    
    bool TiePointsLocatorSURFStrategy::createIntegralImage( const FloatsMatrix& inputData,
      FloatsMatrix& outputData )
    {
      TERP_TRUE_OR_RETURN_FALSE( outputData.reset( inputData.getLinesNumber(),
        inputData.getColumnsNumber() ), "Cannot allocate image matrix" );
    
      const unsigned int nLines = inputData.getLinesNumber();
      const unsigned int nCols = inputData.getColumnsNumber();
      
      unsigned int outCol = 0;
      float currSum = 0;
      
      for( unsigned int outLine = 0 ; outLine < nLines ; ++outLine )
        for( outCol = 0; outCol < nCols ; ++outCol )
        {
          currSum = inputData[ outLine ][ outCol ];
          
          if( outLine )
            currSum += outputData[ outLine - 1 ][ outCol ];
          if( outCol )
            currSum += outputData[ outLine ][ outCol - 1 ];
          if( outLine && outCol )
            currSum -= outputData[ outLine - 1 ][ outCol - 1 ];
          
          outputData[ outLine ][ outCol ] = currSum;
        }
      
      return true;
    }    
    
    bool TiePointsLocatorSURFStrategy::locateSurfInterestPoints( 
      const unsigned int maxInterestPoints,
      const FloatsMatrix& integralRasterData,
      UCharsMatrix const* maskRasterDataPtr,
      InterestPointsSetT& interestPoints ) const
    {
      interestPoints.clear();
      
      const unsigned int minIntegralRasterWidthHeigh = getSurfFilterSize( 
        ((Parameters*)m_inputParameters.getSpecStrategyParams())->m_surfOctavesNumber - 1, 
        ((Parameters*)m_inputParameters.getSpecStrategyParams())->m_surfScalesNumber - 1 ) + 2;
      if( ( integralRasterData.getColumnsNumber() < minIntegralRasterWidthHeigh )
        || ( integralRasterData.getLinesNumber() < minIntegralRasterWidthHeigh ) )
      {
        return true;
      }
      
      // finding interest points
      
      bool returnValue = true;
      boost::mutex rastaDataAccessMutex;
      boost::mutex interestPointsAccessMutex;
      unsigned int nextRasterLinesBlockToProcess = 0;
      std::vector< InterestPointsSetT > interestPointsSubSectors(
        m_inputParameters.m_tiePointsSubSectorsSplitFactor *
        m_inputParameters.m_tiePointsSubSectorsSplitFactor );      
      
      SurfLocatorThreadParams threadParams;
      threadParams.m_returnValuePtr = &returnValue;
      threadParams.m_rastaDataAccessMutexPtr = &rastaDataAccessMutex;
      threadParams.m_interestPointsAccessMutexPtr = &interestPointsAccessMutex;
      threadParams.m_nextRasterLinesBlockToProcessValuePtr = 
        &nextRasterLinesBlockToProcess;
      threadParams.m_integralRasterDataPtr = &integralRasterData;
      threadParams.m_maskRasterDataPtr = maskRasterDataPtr;
      threadParams.m_scalesNumber = ((Parameters*)m_inputParameters.getSpecStrategyParams())->m_surfScalesNumber;
      threadParams.m_octavesNumber = ((Parameters*)m_inputParameters.getSpecStrategyParams())->m_surfOctavesNumber;
      threadParams.m_interestPointsSubSectorsPtr = &interestPointsSubSectors;
      threadParams.m_maxInterestPointsBySubSector = maxInterestPoints /
        ( m_inputParameters.m_tiePointsSubSectorsSplitFactor *
        m_inputParameters.m_tiePointsSubSectorsSplitFactor );
      threadParams.m_tiePointsSubSectorsSplitFactor = m_inputParameters.m_tiePointsSubSectorsSplitFactor;      
      
      if( m_inputParameters.m_enableMultiThread )
      {
        threadParams.m_processingBlocksNumber = te::common::GetPhysProcNumber();
        
        boost::thread_group threads;
        
        for( unsigned int threadIdx = 0 ; threadIdx < threadParams.m_processingBlocksNumber ;
          ++threadIdx )
        {
          threads.add_thread( new boost::thread( 
            locateSurfInterestPointsThreadEntry, 
            &threadParams ) );
        }
        
        threads.join_all();
      }
      else
      {
        threadParams.m_processingBlocksNumber = 1;
        
        locateSurfInterestPointsThreadEntry( &threadParams );
      }
      
      // transfering sector maximas to output
      
      const unsigned int interestPointsSubSectorsSize = interestPointsSubSectors.size();
      
      for( unsigned int interestPointsSubSectorsIdx = 0 ; interestPointsSubSectorsIdx <
        interestPointsSubSectorsSize ; ++interestPointsSubSectorsIdx )
      {
        interestPoints.insert( 
          interestPointsSubSectors[ interestPointsSubSectorsIdx ].begin(),
          interestPointsSubSectors[ interestPointsSubSectorsIdx ].end() );
      }      
     
      return returnValue;
    }
    

    
    void TiePointsLocatorSURFStrategy::locateSurfInterestPointsThreadEntry(SurfLocatorThreadParams* paramsPtr)
    {
      assert( paramsPtr );
      assert( paramsPtr->m_returnValuePtr );
      assert( paramsPtr->m_integralRasterDataPtr );
      assert( paramsPtr->m_interestPointsSubSectorsPtr );
      assert( paramsPtr->m_rastaDataAccessMutexPtr );
      assert( paramsPtr->m_interestPointsAccessMutexPtr );
      assert( paramsPtr->m_processingBlocksNumber > 0 );
      assert( paramsPtr->m_nextRasterLinesBlockToProcessValuePtr );
      assert( paramsPtr->m_scalesNumber > 2 );
      assert( paramsPtr->m_octavesNumber > 0 );
      
      // Globals
      
      paramsPtr->m_rastaDataAccessMutexPtr->lock();
      
      const unsigned int maxGausFilterWidth = getSurfFilterSize( 
        paramsPtr->m_octavesNumber - 1, paramsPtr->m_scalesNumber - 1 );
      const unsigned int maxGausFilterRadius = maxGausFilterWidth / 2;
      const unsigned int rasterBuffersLinesNumber = maxGausFilterWidth;
      const unsigned int lastRasterBuffersLineIdx = rasterBuffersLinesNumber - 1;
      const unsigned int tiePointsSubSectorsSplitFactor = paramsPtr->m_tiePointsSubSectorsSplitFactor;
      const unsigned int rowsBySubSector = (unsigned int)std::ceil(
        ((double)paramsPtr->m_integralRasterDataPtr->getLinesNumber())
        / 
        ((double)tiePointsSubSectorsSplitFactor) );
      const unsigned int colsBySubSector = (unsigned int)std::ceil(
        ((double)paramsPtr->m_integralRasterDataPtr->getColumnsNumber())
        /
        ((double)tiePointsSubSectorsSplitFactor) );      
      const unsigned int maxInterestPointsBySubSector = paramsPtr->m_maxInterestPointsBySubSector;
      const unsigned int maxInterestPointsByScaleSubSector = maxInterestPointsBySubSector
        / ( paramsPtr->m_octavesNumber * ( paramsPtr->m_scalesNumber - 2 ) );
      const unsigned int rasterLines = paramsPtr->m_integralRasterDataPtr->getLinesNumber();
      const unsigned int buffersCols = paramsPtr->m_integralRasterDataPtr->getColumnsNumber();
      const unsigned int rasterBufferLineSizeBytes = sizeof( float ) * 
        buffersCols;
      const unsigned int maskRasterBufferLineSizeBytes = sizeof( UCharsMatrix::ElementTypeT ) * 
        buffersCols;
      const unsigned int processingBlocksNumber = paramsPtr->m_processingBlocksNumber;
      const unsigned int maxLinesPerProcessingBlock = (unsigned int)
        std::ceil(
          ((double)rasterLines)
          /
          ((double)processingBlocksNumber)
        );        
      paramsPtr->m_rastaDataAccessMutexPtr->unlock();  
      
      // Allocating the internal raster data buffer
      // and the mask raster buffer
      
      FloatsMatrix rasterBufferDataHandler;
      boost::scoped_array< float* > rasterBufferHandler( new float*[ rasterBuffersLinesNumber ] );
      {
        if( ! rasterBufferDataHandler.reset( rasterBuffersLinesNumber, buffersCols, 
          FloatsMatrix::RAMMemPol ) )
        {
          paramsPtr->m_rastaDataAccessMutexPtr->lock();
          *(paramsPtr->m_returnValuePtr) = false;
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          return;
        }
        
        for( unsigned int rasterBufferDataHandlerLine = 0 ; rasterBufferDataHandlerLine < 
          rasterBuffersLinesNumber ; ++rasterBufferDataHandlerLine )
        {
          rasterBufferHandler[ rasterBufferDataHandlerLine ] = rasterBufferDataHandler[ 
            rasterBufferDataHandlerLine ];
        }
      }
      float** rasterBufferPtr = rasterBufferHandler.get();
      
      // Allocating the mask raster buffer      
      
      UCharsMatrix maskRasterBufferDataHandler;
      
      boost::scoped_array< unsigned char* > maskRasterBufferHandler( new unsigned char*[ rasterBuffersLinesNumber ] );
      
      unsigned char** maskRasterBufferPtr = 0;
      
      if( paramsPtr->m_maskRasterDataPtr )
      {
        if( ! maskRasterBufferDataHandler.reset( rasterBuffersLinesNumber, buffersCols, 
          UCharsMatrix::RAMMemPol ) )
        {
          paramsPtr->m_rastaDataAccessMutexPtr->lock();
          *(paramsPtr->m_returnValuePtr) = false;
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          return;
        }        
        
        for( unsigned int maskRasterBufferDataHandlerLine = 0 ; maskRasterBufferDataHandlerLine < 
          rasterBuffersLinesNumber ; ++maskRasterBufferDataHandlerLine )
        {
          maskRasterBufferHandler[ maskRasterBufferDataHandlerLine ] = maskRasterBufferDataHandler[ 
            maskRasterBufferDataHandlerLine ];
        }
        
        maskRasterBufferPtr = maskRasterBufferHandler.get();      
      }
      
      // Allocating the internal octaves buffers
      
      FloatsMatrix octavesBufferDataHandler;
      std::vector< std::vector< boost::shared_array< float* > > >
        octavesBufferHandlers;
      {
        const unsigned int octavesBufferDataHandlerLines = 
          3 * paramsPtr->m_octavesNumber * paramsPtr->m_scalesNumber;
        if( ! octavesBufferDataHandler.reset( octavesBufferDataHandlerLines , 
          buffersCols, 
          FloatsMatrix::RAMMemPol ) )
        {
          paramsPtr->m_rastaDataAccessMutexPtr->lock();
          *(paramsPtr->m_returnValuePtr) = false;
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          return;
        }
        unsigned int octavesBufferDataHandlerLine = 0;
        unsigned int octavesBufferDataHandlerCol = 0;
        float* octavesBufferDataHandlerLinePtr = 0;
        for( octavesBufferDataHandlerLine = 0; octavesBufferDataHandlerLine < 
          octavesBufferDataHandlerLines ; ++octavesBufferDataHandlerLine )
        {
          octavesBufferDataHandlerLinePtr = octavesBufferDataHandler[ 
            octavesBufferDataHandlerLine ];
            
          for( octavesBufferDataHandlerCol = 0; octavesBufferDataHandlerCol < 
            buffersCols ; ++octavesBufferDataHandlerCol )
          {
            octavesBufferDataHandlerLinePtr[ octavesBufferDataHandlerCol ] = 0.0;
          }
        }
        
        octavesBufferDataHandlerLine = 0;
        unsigned int scaleIdx = 0 ;
        unsigned int octaveIdx = 0 ;        
        for( octaveIdx = 0 ; octaveIdx < paramsPtr->m_octavesNumber ; ++octaveIdx )
        {
          octavesBufferHandlers.push_back( std::vector< boost::shared_array< float* > >() );
          std::vector< boost::shared_array< float* > >&
            currOctaveBuffersHandler = octavesBufferHandlers[ octaveIdx ];
          
          for( scaleIdx = 0 ; scaleIdx < paramsPtr->m_scalesNumber ; ++scaleIdx )
          {
            currOctaveBuffersHandler.push_back( boost::shared_array< float* >(
              new float*[ 3 ] ) );
            boost::shared_array< float* >& currOctavesBuffer = 
              currOctaveBuffersHandler[ scaleIdx ];
            for( unsigned int bufferLine = 0 ; bufferLine <  3 ; ++bufferLine )
            {
              assert( octavesBufferDataHandlerLine < 
                octavesBufferDataHandler.getLinesNumber() );
                
              currOctavesBuffer[ bufferLine ] = octavesBufferDataHandler[ 
                octavesBufferDataHandlerLine ];
                
              ++octavesBufferDataHandlerLine;
            }          
          }
        }
      }
      
      // Allocating the laplacian sign buffers
      
      UCharsMatrix laplacianSignBufferDataHandler;
      std::vector< std::vector< boost::shared_array< unsigned char* > > >
        laplacianSignBufferHandlers;
      {
        const unsigned int laplacianSignBufferDataHandlerLines = 
          3 * paramsPtr->m_octavesNumber * paramsPtr->m_scalesNumber;
        if( ! laplacianSignBufferDataHandler.reset( laplacianSignBufferDataHandlerLines , 
          buffersCols, 
          UCharsMatrix::RAMMemPol ) )
        {
          paramsPtr->m_rastaDataAccessMutexPtr->lock();
          *(paramsPtr->m_returnValuePtr) = false;
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          return;
        }
        unsigned int laplacianSignBufferDataHandlerLine = 0;
        unsigned int laplacianSignBufferDataHandlerCol = 0;
        unsigned char* laplacianSignBufferDataHandlerLinePtr = 0;
        for( laplacianSignBufferDataHandlerLine = 0; laplacianSignBufferDataHandlerLine < 
          laplacianSignBufferDataHandlerLines ; ++laplacianSignBufferDataHandlerLine )
        {
          laplacianSignBufferDataHandlerLinePtr = laplacianSignBufferDataHandler[ 
            laplacianSignBufferDataHandlerLine ];
            
          for( laplacianSignBufferDataHandlerCol = 0; laplacianSignBufferDataHandlerCol < 
            buffersCols ; ++laplacianSignBufferDataHandlerCol )
          {
            laplacianSignBufferDataHandlerLinePtr[ laplacianSignBufferDataHandlerCol ] =
              255;
          }
        }
        
        laplacianSignBufferDataHandlerLine = 0;
        unsigned int scaleIdx = 0 ;
        unsigned int octaveIdx = 0 ;          
        for( octaveIdx = 0 ; octaveIdx < paramsPtr->m_octavesNumber ; ++octaveIdx )
        {
          laplacianSignBufferHandlers.push_back( std::vector< boost::shared_array< unsigned char* > >() );
          std::vector< boost::shared_array< unsigned char* > >&
            currlaplacianSignBuffersHandler = laplacianSignBufferHandlers[ octaveIdx ];
          
          for( scaleIdx = 0 ; scaleIdx < paramsPtr->m_scalesNumber ; ++scaleIdx )
          {
            currlaplacianSignBuffersHandler.push_back( boost::shared_array< unsigned char* >(
              new unsigned char*[ 3 ] ) );
            boost::shared_array< unsigned char* >& currlaplacianSignBuffer = 
              currlaplacianSignBuffersHandler[ scaleIdx ];
            for( unsigned int bufferLine = 0 ; bufferLine <  3 ; ++bufferLine )
            {
              assert( laplacianSignBufferDataHandlerLine < 
                laplacianSignBufferDataHandler.getLinesNumber() );
                
              currlaplacianSignBuffer[ bufferLine ] = laplacianSignBufferDataHandler[ 
                laplacianSignBufferDataHandlerLine ];
                
              ++laplacianSignBufferDataHandlerLine;
            }          
          }
        }
      }      
      
      // Locating interest points on each scale/octave
      
      const unsigned maxGausResponseFilterCenterBufferColBound = buffersCols - 
        maxGausFilterRadius;      
      const unsigned maxMaximaDetectionCenterBufferColBound = buffersCols - 
        maxGausFilterRadius - 1;              
        
      unsigned int scaleIdx = 0 ;
      unsigned int octaveIdx = 0 ;  
      unsigned int subSectorIndex = 0;
      unsigned int scaleGlobalIndex = 0;
      
      std::vector< std::vector< InterestPointsSetT > > interestPointsSubSectors;
      {
        std::vector< InterestPointsSetT > dummyIPointsContainer( 
          paramsPtr->m_tiePointsSubSectorsSplitFactor *
          paramsPtr->m_tiePointsSubSectorsSplitFactor );
        interestPointsSubSectors.resize( paramsPtr->m_octavesNumber *
          paramsPtr->m_scalesNumber, dummyIPointsContainer );
      } 
        
      for( unsigned int rasterLinesBlockIdx = 0; rasterLinesBlockIdx <
        processingBlocksNumber ; ++rasterLinesBlockIdx )
      {
        // the maxima points found inside the current raster block
        // for each combination of octaves and scales
        std::vector< InterestPointsSetT > blockMaximas( paramsPtr->m_scalesNumber *
          paramsPtr->m_octavesNumber, InterestPointsSetT() ); 
        
        paramsPtr->m_rastaDataAccessMutexPtr->lock();
         
        if( rasterLinesBlockIdx == ( *(paramsPtr->m_nextRasterLinesBlockToProcessValuePtr ) ) )
        {
          ++( *(paramsPtr->m_nextRasterLinesBlockToProcessValuePtr ) );
           
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          
          // globals
          
          const unsigned int rasterLinesStart = 
            (unsigned int)
              std::max( 
                0
                ,
                (
                  ((int)(rasterLinesBlockIdx * maxLinesPerProcessingBlock ))
                  - 
                  ((int)maxGausFilterRadius)
                  -
                  1
                )
              ); 
          const unsigned int rasterLinesEndBound = 
            std::min( 
              rasterLines
              ,
              (
                ( ( rasterLinesBlockIdx + 1 ) * maxLinesPerProcessingBlock ) 
                + 
                maxGausFilterRadius
                +
                1
              )
             );
          const unsigned int firstRasterLineToGenerateResponse = 
            rasterLinesStart + maxGausFilterWidth - 1;
          const unsigned int firstRasterLineToLookForMaximas =
            rasterLinesStart + maxGausFilterWidth + 1;
          unsigned int filterWidth = 0;
          unsigned int filterLobeWidth = 0;
          unsigned int filterLobeRadius = 0;
          unsigned int filterCenterBufCol = 0 ;
          float dXX = 0;
          float dYY = 0;
          float dXY = 0;
          InterestPointT auxInterestPoint;
          float** currScaleBufferPtr = 0;
          unsigned char** currLaplacianSignBufferPtr = 0;
          unsigned int lastScaleIdx = 0;
          unsigned int nextScaleIdx = 0;
          unsigned int prevResponseBufferColIdx = 0;
          unsigned int nextResponseBufferColIdx = 0;
          float windowCenterPixelValue  = 0.0;
          float neighborMaximaDif_0_1 = 0.0;
          float neighborMaximaDif_0_2 = 0.0;
          float neighborMaximaDif_0_3 = 0.0;
          float neighborMaximaDif_0_4 = 0.0;
          float neighborMaximaDif_0_5 = 0.0;
          float neighborMaximaDif_0_6 = 0.0;
          float neighborMaximaDif_0_7 = 0.0;
          float neighborMaximaDif_0_8 = 0.0;
          float neighborMaximaDif_1_1 = 0.0;
          float neighborMaximaDif_1_2 = 0.0;
          float neighborMaximaDif_1_3 = 0.0;
          float neighborMaximaDif_1_4 = 0.0;
          float neighborMaximaDif_1_5 = 0.0;
          float neighborMaximaDif_1_6 = 0.0;
          float neighborMaximaDif_1_7 = 0.0;
          float neighborMaximaDif_1_8 = 0.0;
          float neighborMaximaDif_1_9 = 0.0;
          float neighborMaximaDif_2_1 = 0.0;
          float neighborMaximaDif_2_2 = 0.0;
          float neighborMaximaDif_2_3 = 0.0;
          float neighborMaximaDif_2_4 = 0.0;
          float neighborMaximaDif_2_5 = 0.0;
          float neighborMaximaDif_2_6 = 0.0;
          float neighborMaximaDif_2_7 = 0.0;
          float neighborMaximaDif_2_8 = 0.0;
          float neighborMaximaDif_2_9 = 0.0;          
          
          // Processing each raster line from the current block
                   
          for( unsigned int rasterLine = rasterLinesStart; rasterLine < rasterLinesEndBound ;
            ++rasterLine )
          {
            // read a new raster line into the last raster buffer line
            paramsPtr->m_rastaDataAccessMutexPtr->lock();
            //std::cout << std::endl << "rasterLine"; std::cout << rasterLine << std::endl;
            //printBuffer( rasterBufferPtr, buffersLines, buffersCols );
            roolUpBuffer( rasterBufferPtr, rasterBuffersLinesNumber );
//            printBuffer( rasterBufferPtr, buffersLines, buffersCols );
            memcpy( rasterBufferPtr[ lastRasterBuffersLineIdx ], 
              paramsPtr->m_integralRasterDataPtr->operator[]( rasterLine ),
              rasterBufferLineSizeBytes );
//            printBuffer( rasterBufferPtr, buffersLines, buffersCols );
            // read a new mask raster line into the last mask raster buffer line
            if( paramsPtr->m_maskRasterDataPtr )
            {
              roolUpBuffer( maskRasterBufferPtr, rasterBuffersLinesNumber );
              memcpy( maskRasterBufferPtr[ lastRasterBuffersLineIdx ], 
                paramsPtr->m_maskRasterDataPtr->operator[]( rasterLine ),
                maskRasterBufferLineSizeBytes );
            }    
            paramsPtr->m_rastaDataAccessMutexPtr->unlock();
            
            // generating the response maps for each octave/scale

            if( rasterLine >= firstRasterLineToGenerateResponse )
            {
              for( octaveIdx = 0 ; octaveIdx < paramsPtr->m_octavesNumber ; ++octaveIdx )
              {
                for( scaleIdx = 0 ; scaleIdx < paramsPtr->m_scalesNumber ;
                  ++scaleIdx )
                {
                  assert( octavesBufferHandlers.size() > octaveIdx );
                  assert( octavesBufferHandlers[ octaveIdx].size() > scaleIdx );
                  currScaleBufferPtr = octavesBufferHandlers[ octaveIdx][ scaleIdx ].get();
                  assert( currScaleBufferPtr );
                  
                  assert( laplacianSignBufferHandlers.size() > octaveIdx );
                  assert( laplacianSignBufferHandlers[ octaveIdx].size() > scaleIdx );
                  currLaplacianSignBufferPtr = laplacianSignBufferHandlers[ octaveIdx][ scaleIdx ].get();
                  assert( currLaplacianSignBufferPtr );                  
                  
                  // Roll up buffers
                  
                  roolUpBuffer( currScaleBufferPtr, 3 );
                  roolUpBuffer( currLaplacianSignBufferPtr, 3 );
                  
                  // applying the filter kernels for the current scale
                  
                  filterWidth = getSurfFilterSize( octaveIdx, scaleIdx );
                  assert( filterWidth <= maxGausFilterWidth );
                  
                  filterLobeWidth = filterWidth / 3; 
                  filterLobeRadius = filterLobeWidth / 2;
                    
                  for( filterCenterBufCol = maxGausFilterRadius ; 
                    filterCenterBufCol < maxGausResponseFilterCenterBufferColBound ; 
                    ++filterCenterBufCol )
                  {
                    dYY = getSurfDyyDerivative( rasterBufferPtr, filterCenterBufCol, 
                      maxGausFilterRadius, filterLobeWidth, filterLobeRadius);
                    dYY /= (float)( filterWidth * filterWidth );
                    
                    dXX = getSurfDxxDerivative( rasterBufferPtr, filterCenterBufCol, 
                      maxGausFilterRadius, filterLobeWidth, filterLobeRadius);
                    dXX /= (float)( filterWidth * filterWidth );
                    
                    dXY = getSurfDxyDerivative( rasterBufferPtr, filterCenterBufCol, 
                      maxGausFilterRadius, filterLobeWidth );
                    dXY /= (float)( filterWidth * filterWidth );
                      
                    currScaleBufferPtr[ 2 ][ filterCenterBufCol ] = 
                      ( dXX * dYY ) - ( 0.81f * dXY * dXY );
                    currLaplacianSignBufferPtr[ 2 ][ filterCenterBufCol ] = 
                      ( ( dXX + dYY ) >= 0.0 ) ? 1 : 0;                      
                  }
                }
              }
            }
            
            // Finding local maximas over all scales using 3 x 3 x 3 window
            // at the lowest scale

            if( rasterLine >= firstRasterLineToLookForMaximas )
            {
//              printBuffer( octavesBufferHandlers[ 0 ][ 0 ].get(), buffersLines, buffersCols );            
//              printBuffer( octavesBufferHandlers[ 0 ][ 1 ].get(), buffersLines, buffersCols );
//              printBuffer( octavesBufferHandlers[ 0 ][ 2 ].get(), buffersLines, buffersCols );
//              return;
              
              for( unsigned int windCenterCol = maxGausFilterRadius + 1; 
                windCenterCol < maxMaximaDetectionCenterBufferColBound ; ++windCenterCol )
              {
                prevResponseBufferColIdx = windCenterCol - 1;
                nextResponseBufferColIdx = windCenterCol + 1;
                auxInterestPoint.m_feature1 = (-1.0) * FLT_MAX;
                    
                for( octaveIdx = 0 ; octaveIdx < paramsPtr->m_octavesNumber ; ++octaveIdx )
                {         
                  std::vector< boost::shared_array< float* > >& 
                    currOctaveBuffersHandler = octavesBufferHandlers[ octaveIdx ];
                    
                  for( scaleIdx = 1 ; scaleIdx < ( paramsPtr->m_scalesNumber - 1 );
                    ++scaleIdx )
                  {   
                    windowCenterPixelValue = currOctaveBuffersHandler[
                      scaleIdx ][ 1 ][ windCenterCol ];
                    lastScaleIdx = scaleIdx - 1;
                    nextScaleIdx = scaleIdx + 1;
                    
                    neighborMaximaDif_0_1 = windowCenterPixelValue - currOctaveBuffersHandler[
                      scaleIdx ][ 0 ][ prevResponseBufferColIdx ];
                    neighborMaximaDif_0_2 = windowCenterPixelValue - currOctaveBuffersHandler[
                      scaleIdx ][ 0 ][ windCenterCol ];
                    neighborMaximaDif_0_3 = windowCenterPixelValue - currOctaveBuffersHandler[
                      scaleIdx ][ 0 ][ nextResponseBufferColIdx ];
                    neighborMaximaDif_0_4 = windowCenterPixelValue - currOctaveBuffersHandler[
                      scaleIdx ][ 1 ][ prevResponseBufferColIdx ];                     
                    neighborMaximaDif_0_5 = windowCenterPixelValue - currOctaveBuffersHandler[
                      scaleIdx ][ 1 ][ nextResponseBufferColIdx ];
                    neighborMaximaDif_0_6 = windowCenterPixelValue - currOctaveBuffersHandler[
                      scaleIdx ][ 2 ][ prevResponseBufferColIdx];
                    neighborMaximaDif_0_7 = windowCenterPixelValue - currOctaveBuffersHandler[
                      scaleIdx ][ 2 ][ windCenterCol ];
                    neighborMaximaDif_0_8 = windowCenterPixelValue - currOctaveBuffersHandler[
                      scaleIdx ][ 2 ][ nextResponseBufferColIdx ];
                      
                    if( 
                        ( windowCenterPixelValue > 0.0 )
                        // verifying the current scale (center not included)
                        && ( neighborMaximaDif_0_1 > 0.0 )
                        && ( neighborMaximaDif_0_2 > 0.0 )
                        && ( neighborMaximaDif_0_3 > 0.0 )
                        && ( neighborMaximaDif_0_4 > 0.0 )
                        && ( neighborMaximaDif_0_5 > 0.0 )
                        && ( neighborMaximaDif_0_6 > 0.0 )
                        && ( neighborMaximaDif_0_7 > 0.0 )
                        && ( neighborMaximaDif_0_8 > 0.0 )
                      )
                    {
                      neighborMaximaDif_1_1 = windowCenterPixelValue - currOctaveBuffersHandler[
                            lastScaleIdx ][ 0 ][ prevResponseBufferColIdx ];
                      neighborMaximaDif_1_2 = windowCenterPixelValue - currOctaveBuffersHandler[
                            lastScaleIdx ][ 0 ][ windCenterCol ];
                      neighborMaximaDif_1_3 = windowCenterPixelValue - currOctaveBuffersHandler[
                            lastScaleIdx ][ 0 ][ nextResponseBufferColIdx ];
                      neighborMaximaDif_1_4 = windowCenterPixelValue - currOctaveBuffersHandler[
                            lastScaleIdx ][ 1 ][ prevResponseBufferColIdx ];
                      neighborMaximaDif_1_5 = windowCenterPixelValue - currOctaveBuffersHandler[
                            lastScaleIdx ][ 1 ][ windCenterCol ];
                      neighborMaximaDif_1_6 = windowCenterPixelValue - currOctaveBuffersHandler[
                            lastScaleIdx ][ 1 ][ nextResponseBufferColIdx ];
                      neighborMaximaDif_1_7 = windowCenterPixelValue - currOctaveBuffersHandler[
                            lastScaleIdx ][ 2 ][ prevResponseBufferColIdx];
                      neighborMaximaDif_1_8 = windowCenterPixelValue - currOctaveBuffersHandler[
                            lastScaleIdx ][ 2 ][ windCenterCol ];
                      neighborMaximaDif_1_9 = windowCenterPixelValue - currOctaveBuffersHandler[
                            lastScaleIdx ][ 2 ][ nextResponseBufferColIdx ];
                      
                      if(
                          // verifying the top scale
                          ( neighborMaximaDif_1_1 > 0.0 )
                          && ( neighborMaximaDif_1_2 > 0.0 )
                          && ( neighborMaximaDif_1_3 > 0.0 )
                          && ( neighborMaximaDif_1_4 > 0.0 )
                          && ( neighborMaximaDif_1_5 > 0.0 )
                          && ( neighborMaximaDif_1_6 > 0.0 )
                          && ( neighborMaximaDif_1_7 > 0.0 )
                          && ( neighborMaximaDif_1_8 > 0.0 )
                          && ( neighborMaximaDif_1_9 > 0.0 )
                        )
                      {
                         neighborMaximaDif_2_1 = windowCenterPixelValue - currOctaveBuffersHandler[
                           nextScaleIdx ][ 0 ][ prevResponseBufferColIdx ];
                         neighborMaximaDif_2_2 = windowCenterPixelValue - currOctaveBuffersHandler[
                           nextScaleIdx ][ 0 ][ windCenterCol ];
                         neighborMaximaDif_2_3 = windowCenterPixelValue - currOctaveBuffersHandler[
                           nextScaleIdx ][ 0 ][ nextResponseBufferColIdx ];
                         neighborMaximaDif_2_4 = windowCenterPixelValue - currOctaveBuffersHandler[
                           nextScaleIdx ][ 1 ][ prevResponseBufferColIdx ];
                         neighborMaximaDif_2_5 = windowCenterPixelValue - currOctaveBuffersHandler[
                           nextScaleIdx ][ 1 ][ windCenterCol ];
                         neighborMaximaDif_2_6 = windowCenterPixelValue - currOctaveBuffersHandler[
                           nextScaleIdx ][ 1 ][ nextResponseBufferColIdx ];
                         neighborMaximaDif_2_7 = windowCenterPixelValue - currOctaveBuffersHandler[
                           nextScaleIdx ][ 2 ][ prevResponseBufferColIdx];
                         neighborMaximaDif_2_8 = windowCenterPixelValue - currOctaveBuffersHandler[
                           nextScaleIdx ][ 2 ][ windCenterCol ];
                         neighborMaximaDif_2_9 = windowCenterPixelValue - currOctaveBuffersHandler[
                           nextScaleIdx ][ 2 ][ nextResponseBufferColIdx ];
                           
                        if(
                            // verifying the next scale
                            ( neighborMaximaDif_2_1 > 0.0 )
                            && ( neighborMaximaDif_2_2 > 0.0 )
                            && ( neighborMaximaDif_2_3 > 0.0 )
                            && ( neighborMaximaDif_2_4 > 0.0 )
                            && ( neighborMaximaDif_2_5 > 0.0 )
                            && ( neighborMaximaDif_2_6 > 0.0 )
                            && ( neighborMaximaDif_2_7 > 0.0 )
                            && ( neighborMaximaDif_2_8 > 0.0 )
                            && ( neighborMaximaDif_2_9 > 0.0 )
                            && ( 
                                  maskRasterBufferPtr 
                                  ? 
                                  ( maskRasterBufferPtr[ 0 ][ windCenterCol ] != 0 ) 
                                  :
                                  true
                                )
                          )
                        {
                          auxInterestPoint.m_feature1 =
                            neighborMaximaDif_0_1
                            + neighborMaximaDif_0_2
                            + neighborMaximaDif_0_3
                            + neighborMaximaDif_0_4
                            + neighborMaximaDif_0_5
                            + neighborMaximaDif_0_6
                            + neighborMaximaDif_0_7
                            + neighborMaximaDif_0_8
                            + neighborMaximaDif_1_1
                            + neighborMaximaDif_1_2
                            + neighborMaximaDif_1_3
                            + neighborMaximaDif_1_4
                            + neighborMaximaDif_1_5
                            + neighborMaximaDif_1_6
                            + neighborMaximaDif_1_7
                            + neighborMaximaDif_1_8
                            + neighborMaximaDif_1_9
                            + neighborMaximaDif_2_1
                            + neighborMaximaDif_2_2
                            + neighborMaximaDif_2_3
                            + neighborMaximaDif_2_4
                            + neighborMaximaDif_2_5
                            + neighborMaximaDif_2_6
                            + neighborMaximaDif_2_7
                            + neighborMaximaDif_2_8
                            + neighborMaximaDif_2_9;                            
                          auxInterestPoint.m_feature2 = (float)getSurfFilterSize(
                            octaveIdx, scaleIdx );
                          auxInterestPoint.m_feature3 = (float)
                            laplacianSignBufferHandlers[ octaveIdx ][ scaleIdx ][ 
                            1 ][ windCenterCol ] ;
                            
                          auxInterestPoint.m_x = windCenterCol;
                          auxInterestPoint.m_y = rasterLine - maxGausFilterRadius - 1;
                          assert( auxInterestPoint.m_x < 
                            paramsPtr->m_integralRasterDataPtr->getColumnsNumber() );
                          assert( auxInterestPoint.m_y < 
                            paramsPtr->m_integralRasterDataPtr->getLinesNumber() );   
                          
                          scaleGlobalIndex = ( octaveIdx * 
                            paramsPtr->m_scalesNumber ) + scaleIdx;
                          subSectorIndex = ( ( auxInterestPoint.m_y / 
                            rowsBySubSector ) * tiePointsSubSectorsSplitFactor ) + 
                            ( auxInterestPoint.m_x / colsBySubSector );
                          assert( scaleGlobalIndex < interestPointsSubSectors.size() );                          
                          assert( subSectorIndex < interestPointsSubSectors[ scaleGlobalIndex ].size() );
                            
                          interestPointsSubSectors[ scaleGlobalIndex ][ 
                            subSectorIndex ].insert( auxInterestPoint);
                            
                          if( interestPointsSubSectors[ scaleGlobalIndex ][ 
                            subSectorIndex ].size() > maxInterestPointsByScaleSubSector )
                          {
                            interestPointsSubSectors[ scaleGlobalIndex ][ 
                            subSectorIndex ].erase( interestPointsSubSectors[ 
                            scaleGlobalIndex ][ subSectorIndex ].begin() );
                          }                        
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else
        {
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
        }
        
        // Copying the best found block maximas to the external maximas container
        
        paramsPtr->m_interestPointsAccessMutexPtr->lock();
        
        for( scaleGlobalIndex = 0 ; scaleGlobalIndex < interestPointsSubSectors.size() ;
          ++scaleGlobalIndex )
        {
          for( subSectorIndex = 0 ; subSectorIndex < interestPointsSubSectors[ 
            scaleGlobalIndex ].size() ; ++subSectorIndex )
          {
            double x, y;
            TERP_TRUE_OR_THROW( checkForDuplicatedInterestPoints( 
              interestPointsSubSectors[ scaleGlobalIndex ][ subSectorIndex ], x, y ),
              "Duplicated tie-points" );            
            
            paramsPtr->m_interestPointsSubSectorsPtr->operator[]( 
              subSectorIndex ).insert( interestPointsSubSectors[ 
              scaleGlobalIndex ][ subSectorIndex ].begin(),
              interestPointsSubSectors[ scaleGlobalIndex ][ subSectorIndex ].end() );
            
            while( paramsPtr->m_interestPointsSubSectorsPtr->operator[]( 
              subSectorIndex ).size() > 
              maxInterestPointsBySubSector )
            {
              paramsPtr->m_interestPointsSubSectorsPtr->operator[]( 
                subSectorIndex ).erase( 
                paramsPtr->m_interestPointsSubSectorsPtr->operator[]( 
                subSectorIndex ).begin() );
            }
          }
        }
        
        paramsPtr->m_interestPointsAccessMutexPtr->unlock();         
      }      
    }    
    
    bool TiePointsLocatorSURFStrategy::generateSurfFeatures( 
      const InterestPointsSetT& interestPoints,
      const FloatsMatrix& integralRasterData,
      InterestPointsSetT& validInterestPoints,
      FloatsMatrix& features )
    {
      // Discovering the valid interest points
      
      validInterestPoints.clear();
      
      {
        InterestPointsSetT::const_iterator iPointsIt = interestPoints.begin();
        const InterestPointsSetT::const_iterator iPointsItEnd = interestPoints.end();
        
        while( iPointsIt != iPointsItEnd )
        {
          // Calculating the current interest point variables
        
          const unsigned int& currIPointCenterX = iPointsIt->m_x;
          const unsigned int& currIPointCenterY = iPointsIt->m_y;
          const float currIPointScale = 1.2f * iPointsIt->m_feature2 / 9.0f;
          
          unsigned int featureWindowWidth = (unsigned int)( 20.0 * currIPointScale );
          featureWindowWidth += ( ( featureWindowWidth % 2 ) ? 0 : 1 );
          
          const unsigned int feature90DegreeRotatedWindowRadius = (unsigned int)
            (
              std::ceil( 
                std::sqrt( 
                  2.0
                  * 
                  ( 
                    ( (double)featureWindowWidth ) 
                    *
                    ( (double)featureWindowWidth )
                  )
                ) 
              ) / 2.0
            );
            
          const unsigned int featureElementHaarWindowRadius = 
            ((unsigned int)( 2.0 * currIPointScale )) / 2;
            
          const unsigned int currIPointCenterXAllowedMin = featureElementHaarWindowRadius + 
            feature90DegreeRotatedWindowRadius + 1;
          const unsigned int currIPointCenterXAllowedMax = integralRasterData.getColumnsNumber() - 
            currIPointCenterXAllowedMin - 1;
          const unsigned int currIPointCenterYAllowedMin = currIPointCenterXAllowedMin;
          const unsigned int currIPointCenterYAllowedMax = integralRasterData.getLinesNumber() - 
            currIPointCenterXAllowedMin - 1;
            
          if( ( currIPointCenterX > currIPointCenterXAllowedMin ) &&
            ( currIPointCenterX < currIPointCenterXAllowedMax ) &&
            ( currIPointCenterY > currIPointCenterYAllowedMin ) &&
            ( currIPointCenterY < currIPointCenterYAllowedMax ) )
            
          {
            validInterestPoints.insert( *iPointsIt );
          }   
          
          ++iPointsIt;
        }
      }      
      
      TERP_TRUE_OR_RETURN_FALSE( features.reset( validInterestPoints.size(), 128 ),
        "Cannot allocate features matrix" );       
        
      // globals
      
      float auxFeaturesBuffer[ 128 ];
      
      // iterating over each input innterest point
      
      InterestPointsSetT::const_iterator iPointsIt = validInterestPoints.begin();
      const InterestPointsSetT::const_iterator iPointsItEnd = validInterestPoints.end();
      unsigned int interestPointIdx = 0;
      while( iPointsIt != iPointsItEnd )
      {
        // Calculating the current interest point variables
      
        const unsigned int& currIPointCenterX = iPointsIt->m_x;
        const unsigned int& currIPointCenterY = iPointsIt->m_y;
        const float currIPointScale = 1.2f * iPointsIt->m_feature2 / 9.0f;
        
        unsigned int featureWindowWidth = (unsigned int)( 20.0 * currIPointScale );
        featureWindowWidth += ( ( featureWindowWidth % 2 ) ? 0 : 1 );
        
        const unsigned int featureWindowRadius = featureWindowWidth / 2;
        const float featureWindowRadiusDouble = (float)featureWindowRadius;        
          
        const unsigned int featureSubWindowWidth = featureWindowWidth / 4;
          
        const unsigned int featureElementHaarWindowRadius = 
          ((unsigned int)( 2.0 * currIPointScale )) / 2;

        const unsigned int featureWindowRasterXStart = currIPointCenterX - 
          featureWindowRadius;
        const unsigned int featureWindowRasterYStart = currIPointCenterY - 
          featureWindowRadius;
        
        // Initiating the features vector

        unsigned int currentFeaturePtrStartIdx = 0;
        
        for( currentFeaturePtrStartIdx = 0; currentFeaturePtrStartIdx < 128 ; 
          ++currentFeaturePtrStartIdx )
          auxFeaturesBuffer[ currentFeaturePtrStartIdx ] = 0.0;
          
        // Estimating the intensity vectors
        
        assert( ((long int)currIPointCenterX) -
          ((long int)featureWindowRadius) >= 0 );
        assert( ((long int)currIPointCenterY) -
          ((long int)featureWindowRadius) >= 0 );
        assert( currIPointCenterX + 
          featureWindowRadius < integralRasterData.getColumnsNumber() ); 
        assert( currIPointCenterY + 
          featureWindowRadius < integralRasterData.getLinesNumber() );          
        
        const float currIPointXIntensity = getHaarXVectorIntensity( integralRasterData, currIPointCenterX,
          currIPointCenterY, featureWindowRadius );
        const float currIPointYIntensity = getHaarYVectorIntensity( integralRasterData, currIPointCenterX,
          currIPointCenterY, featureWindowRadius );
        
        // Calculating the rotation parameters
        
        const float currIPointRotationNorm = std::sqrt( ( currIPointXIntensity * currIPointXIntensity ) + 
          ( currIPointYIntensity * currIPointYIntensity ) );
        
        float currIPointRotationSin = 0; // default: no rotation
        float currIPointRotationCos = 1.0; // default: no rotation
          
        if( currIPointRotationNorm != 0.0 ) 
        {
          currIPointRotationCos = currIPointXIntensity / currIPointRotationNorm;
          currIPointRotationSin = currIPointYIntensity / currIPointRotationNorm;
        }
        
        assert( ( currIPointRotationCos >= -1.0 ) && ( currIPointRotationCos <= 1.0 ) );
        assert( ( currIPointRotationSin >= -1.0 ) && ( currIPointRotationSin <= 1.0 ) );
        
//          std::cout << "angle:";
//          std::cout << std::atan2( currIPointRotationSin, currIPointRotationCos ) * 180.0 / M_PI << std::endl;
        
        /* The region surrounding the interest point is split up regularly 
          into smaller 44 square rotated by the haar  intensity vectors 
          calculated above

          counterclockwise rotation 
          | u |    |cos  -sin|   |X|
          | v | == |sin   cos| x |Y|

          clockwise rotation
          | u |    |cos   sin|   |X|
          | v | == |-sin  cos| x |Y|
        */
        
        unsigned int featureWindowYOffset = 0;
        unsigned int featureWindowXOffset = 0;
        float featureElementZeroCenteredOriginalXIdx = 0;
        float featureElementZeroCenteredOriginalYIdx = 0;          
        float featureElementZeroCenteredRotatedXIdx = 0;
        float featureElementZeroCenteredRotatedYIdx = 0;    
        float featureElementRotatedXIdx = 0;
        float featureElementRotatedYIdx = 0; 
        unsigned int featureElementRasterRotatedXIdx = 0;
        unsigned int featureElementRasterRotatedYIdx = 0;    
        float featureElementOriginalHaarXIntensity = 0;
        float featureElementOriginalHaarYIntensity = 0;  
        float featureElementZeroCenteredOriginalHaarXIntensity = 0;
        float featureElementZeroCenteredOriginalHaarYIntensity = 0;   
        float featureElementRotatedHaarXIntensity = 0;
        float featureElementRotatedHaarYIntensity = 0;
        float featureElementZeroCenteredRotatedHaarXIntensity = 0;
        float featureElementZeroCenteredRotatedHaarYIntensity = 0;  
        unsigned int featureSubWindowYIdx = 0;
        unsigned int featureSubWindowXIdx = 0;
        
        for( featureWindowYOffset = 0 ; featureWindowYOffset < featureWindowWidth ; 
          featureWindowYOffset += 5 )
        {
          featureElementZeroCenteredOriginalYIdx = ((float)featureWindowYOffset)
            - featureWindowRadiusDouble;            
            
          featureSubWindowYIdx = featureWindowYOffset / featureSubWindowWidth;
          
          for( featureWindowXOffset = 0 ; featureWindowXOffset < featureWindowWidth ; 
            featureWindowXOffset += 5 )
          {
            featureSubWindowXIdx = featureWindowXOffset / featureSubWindowWidth;
            
            currentFeaturePtrStartIdx = ( featureSubWindowYIdx * 4 ) + 
              featureSubWindowXIdx;
            
            featureElementZeroCenteredOriginalXIdx = ((float)featureWindowXOffset)
              - featureWindowRadiusDouble;              
              
            /* finding the correspondent point over the original raster
                using a clockwize rotation */ 
            
            featureElementZeroCenteredRotatedXIdx = 
              ( currIPointRotationCos * featureElementZeroCenteredOriginalXIdx ) + 
              ( currIPointRotationSin * featureElementZeroCenteredOriginalYIdx );
            featureElementZeroCenteredRotatedYIdx = 
              ( currIPointRotationCos * featureElementZeroCenteredOriginalYIdx )
              - ( currIPointRotationSin * featureElementZeroCenteredOriginalXIdx );
              
            featureElementRotatedXIdx = featureElementZeroCenteredRotatedXIdx +
              featureWindowRadiusDouble;
            featureElementRotatedYIdx = featureElementZeroCenteredRotatedYIdx +
              featureWindowRadiusDouble;
              
            featureElementRasterRotatedXIdx = featureWindowRasterXStart +
              (unsigned int)te::common::Round< float, unsigned int >( featureElementRotatedXIdx );
            featureElementRasterRotatedYIdx = featureWindowRasterYStart +
              (unsigned int)te::common::Round< float, unsigned int >( featureElementRotatedYIdx );
              
            assert( ((long int)featureElementRasterRotatedXIdx) -
              ((long int)featureElementHaarWindowRadius) >= 0 );
            assert( ((long int)featureElementRasterRotatedYIdx) -
              ((long int)featureElementHaarWindowRadius) >= 0 );
            assert( featureElementRasterRotatedXIdx + 
              featureElementHaarWindowRadius < integralRasterData.getColumnsNumber() );
            assert( featureElementRasterRotatedYIdx + 
              featureElementHaarWindowRadius < integralRasterData.getLinesNumber() );                
              
            // Finding the original haar intesity vectors
              
            featureElementOriginalHaarXIntensity = getHaarXVectorIntensity( integralRasterData, 
              featureElementRasterRotatedXIdx, featureElementRasterRotatedYIdx, 
              featureElementHaarWindowRadius );
            featureElementOriginalHaarYIntensity = getHaarYVectorIntensity( integralRasterData, 
              featureElementRasterRotatedXIdx, featureElementRasterRotatedYIdx, 
              featureElementHaarWindowRadius );
              
            // Rotating the intensities by the central point haar intensities vectors
            // usigng a counterclockwise rotation
            
            featureElementZeroCenteredOriginalHaarXIntensity = featureElementOriginalHaarXIntensity +
              featureElementZeroCenteredOriginalXIdx;
            featureElementZeroCenteredOriginalHaarYIntensity = featureElementOriginalHaarYIntensity + 
              featureElementZeroCenteredOriginalYIdx;
            
            featureElementZeroCenteredRotatedHaarXIntensity = 
              ( currIPointRotationCos * featureElementZeroCenteredOriginalHaarXIntensity ) + 
              ( currIPointRotationSin * featureElementZeroCenteredOriginalHaarYIntensity );
            featureElementZeroCenteredRotatedHaarYIntensity = 
              ( currIPointRotationCos * featureElementZeroCenteredOriginalHaarYIntensity )
              - ( currIPointRotationSin * featureElementZeroCenteredOriginalHaarXIntensity );
              
            featureElementRotatedHaarXIntensity = featureElementZeroCenteredRotatedHaarXIntensity
              - featureElementZeroCenteredRotatedXIdx;
            featureElementRotatedHaarYIntensity = featureElementZeroCenteredRotatedHaarYIntensity 
              - featureElementZeroCenteredRotatedYIdx;                
              
            // Generating the related portion inside the output features vector
            
            assert( currentFeaturePtrStartIdx < 121 );
            
            auxFeaturesBuffer[ currentFeaturePtrStartIdx ] += 
              featureElementRotatedHaarXIntensity;
            auxFeaturesBuffer[ currentFeaturePtrStartIdx + 1 ] += 
              featureElementRotatedHaarYIntensity;
            auxFeaturesBuffer[ currentFeaturePtrStartIdx + 2 ] += 
              std::abs( featureElementRotatedHaarXIntensity );
            auxFeaturesBuffer[ currentFeaturePtrStartIdx + 3 ] += 
              std::abs( featureElementRotatedHaarYIntensity );
            if( featureElementRotatedHaarXIntensity < 0.0 )
            {
              auxFeaturesBuffer[ currentFeaturePtrStartIdx + 4 ] += 
                featureElementRotatedHaarXIntensity;
            }
            else
            {
              auxFeaturesBuffer[ currentFeaturePtrStartIdx + 5 ] += 
                featureElementRotatedHaarXIntensity;
            }
            if( featureElementRotatedHaarYIntensity < 0.0 )
            {
              auxFeaturesBuffer[ currentFeaturePtrStartIdx + 6 ] += 
                featureElementRotatedHaarYIntensity;
            }
            else
            {
              auxFeaturesBuffer[ currentFeaturePtrStartIdx + 7 ] += 
                featureElementRotatedHaarYIntensity;
            }            
          }
        }
        
        // turning the descriptor into a unit vector.(Invariance to contrast)
        
        float* currentFeaturePtr = features[ interestPointIdx ];
        
        float featureElementsNormalizeFactor = 0.0;
        
        for( currentFeaturePtrStartIdx = 0 ; currentFeaturePtrStartIdx < 128 ; 
          ++currentFeaturePtrStartIdx )
        {
          featureElementsNormalizeFactor += ( auxFeaturesBuffer[ currentFeaturePtrStartIdx ]
            * auxFeaturesBuffer[ currentFeaturePtrStartIdx ] );
        }
        
        featureElementsNormalizeFactor = std::sqrt( featureElementsNormalizeFactor );
        
        if( featureElementsNormalizeFactor != 0.0 )
        {
          featureElementsNormalizeFactor = 1.0f / featureElementsNormalizeFactor;
        }
        
        for( currentFeaturePtrStartIdx = 0 ; currentFeaturePtrStartIdx < 128 ; 
          ++currentFeaturePtrStartIdx )
        {
          currentFeaturePtr[ currentFeaturePtrStartIdx ] = (
            auxFeaturesBuffer[ currentFeaturePtrStartIdx ] *
            featureElementsNormalizeFactor );
          TERP_DEBUG_TRUE_OR_THROW( ( currentFeaturePtr[ currentFeaturePtrStartIdx ] <= 1.0 ),
            currentFeaturePtr[ currentFeaturePtrStartIdx ] );
          TERP_DEBUG_TRUE_OR_THROW( ( currentFeaturePtr[ currentFeaturePtrStartIdx ] >= -1.0 ),
            currentFeaturePtr[ currentFeaturePtrStartIdx ] );
        }
        
        ++interestPointIdx;
        ++iPointsIt;
      }
                 
      return true;
    }    
    
    bool TiePointsLocatorSURFStrategy::executeMatchingByEuclideanDist( 
      const FloatsMatrix& featuresSet1,
      const FloatsMatrix& featuresSet2,
      const InterestPointsSetT& interestPointsSet1,
      const InterestPointsSetT& interestPointsSet2,
      te::gm::GeometricTransformation const * const raster1ToRaster2TransfPtr,
      const double raster1ToRaster2TransfDMapError,
      MatchedInterestPointsSetT& matchedPoints ) const
    {
      assert( featuresSet1.getColumnsNumber() == featuresSet2.getColumnsNumber() );
      
      matchedPoints.clear();
      
      const double maxEuclideanDist = ((Parameters*)m_inputParameters.getSpecStrategyParams())->m_surfMaxNormEuclideanDist 
        * 2.0; /* since surf feature vectors are unitary verctors */
      
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
      
      // Creating the distances matrix
      
      FloatsMatrix distMatrix;
      TERP_TRUE_OR_RETURN_FALSE( distMatrix.reset( interestPointsSet1Size,
       interestPointsSet2Size, FloatsMatrix::RAMMemPol ),
        "Error crearting the correlation matrix" );
      
      unsigned int col = 0;
      unsigned int line = 0;
      float* linePtr = 0;
      
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        linePtr = distMatrix[ line ];
        
        for( col = 0 ; col < interestPointsSet2Size ; ++col )
        {
          linePtr[ col ] = std::numeric_limits< float >::max();
        }
      }      
        
      // Getting distances
      
      boost::mutex syncMutex;
      unsigned int nextFeatureIdx1ToProcess = 0;
      
      ExecuteMatchingByEuclideanDistThreadEntryParams params;
      params.m_featuresSet1Ptr = &featuresSet1;
      params.m_featuresSet2Ptr = &featuresSet2;
      params.m_interestPointsSet1Ptr = internalInterestPointsSet1.get();
      params.m_interestPointsSet2Ptr = internalInterestPointsSet2.get();
      params.m_nextFeatureIdx1ToProcessPtr = &nextFeatureIdx1ToProcess;
      params.m_distMatrixPtr = &distMatrix;
      params.m_syncMutexPtr = &syncMutex;
      params.m_raster1ToRaster2TransfPtr = raster1ToRaster2TransfPtr;
      params.m_searchOptTreeSearchRadius = raster1ToRaster2TransfDMapError;
      
      if( m_inputParameters.m_enableMultiThread )
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
            executeMatchingByEuclideanDistThreadEntry, &params ) );
        }
        
        threads.join_all();          
          
      }
      else
      {
        executeMatchingByEuclideanDistThreadEntry( &params );
      }
      
      // finding the distances matrix minimum for each line and column
      
      std::vector< float > eachLineMinValues( interestPointsSet1Size,
        FLT_MAX );
      std::vector< unsigned int > eachLineMinIndexes( interestPointsSet1Size,
        interestPointsSet2Size );
      std::vector< float > eachColMinValues( interestPointsSet2Size,
        FLT_MAX );
      std::vector< unsigned int > eachColMinIndexes( interestPointsSet2Size,
        interestPointsSet1Size );
        
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        linePtr = distMatrix[ line ];
        
        for( col = 0 ; col < interestPointsSet2Size ; ++col )
        {
          const float& value = linePtr[ col ];
          
          if( value <= maxEuclideanDist )
          {
            if( value < eachLineMinValues[ line ] )
            {
              eachLineMinValues[ line ] = value;
              eachLineMinIndexes[ line ] = col;
            }
            
            if( value < eachColMinValues[ col ] )
            {
              eachColMinValues[ col ] = value;
              eachColMinIndexes[ col ] = line;
            }
          }
        }
      }
      
      // Finding tiepoints
      
      MatchedInterestPointsT auxMatchedPoints;
        
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        col =  eachLineMinIndexes[ line ];
        
        if( ( col < interestPointsSet2Size ) &&
          ( eachColMinIndexes[ col ] == line ) )
        {
          auxMatchedPoints.m_point1 = internalInterestPointsSet1[ line ];
          auxMatchedPoints.m_point2 = internalInterestPointsSet2[ col ],
          auxMatchedPoints.m_feature = distMatrix( line, col );
          
          matchedPoints.insert( auxMatchedPoints );
        }
      }
        
      return true;
    }
    
    void TiePointsLocatorSURFStrategy::executeMatchingByEuclideanDistThreadEntry(
      ExecuteMatchingByEuclideanDistThreadEntryParams* paramsPtr)
    {
      assert( paramsPtr->m_featuresSet1Ptr->getMemPolicy() == 
        FloatsMatrix::RAMMemPol );
      assert( paramsPtr->m_featuresSet2Ptr->getMemPolicy() == 
        FloatsMatrix::RAMMemPol );
      assert( paramsPtr->m_distMatrixPtr->getMemPolicy() == 
        FloatsMatrix::RAMMemPol );
      assert( paramsPtr->m_featuresSet1Ptr->getColumnsNumber() ==
        paramsPtr->m_featuresSet2Ptr->getColumnsNumber() );
        
      // Globals
      
      const double interestPointsSet2RTreeSearchRadius = paramsPtr->m_searchOptTreeSearchRadius;
      const unsigned int featureElementsNmb = paramsPtr->m_featuresSet1Ptr->getColumnsNumber();
      unsigned int feat2Idx = 0;
      float const* feat1Ptr = 0;
      float const* feat2Ptr = 0;
      float* corrMatrixLinePtr = 0;
      unsigned int featCol = 0;
      te::gm::Envelope auxEnvelope;
      float diff = 0;
      float euclideanDist = 0;
      
      // local transformation copy
      
      std::auto_ptr< te::gm::GeometricTransformation > raster1ToRaster2TransfPtr;
      if( paramsPtr->m_raster1ToRaster2TransfPtr )
      {
        paramsPtr->m_syncMutexPtr->lock();
        raster1ToRaster2TransfPtr.reset( paramsPtr->m_raster1ToRaster2TransfPtr->clone() );        
        paramsPtr->m_syncMutexPtr->unlock();
      }      
      
      // initializing the features 2 indexing
      
      const unsigned int featuresSet1Size = 
        paramsPtr->m_featuresSet1Ptr->getLinesNumber();
      const unsigned int featuresSet2Size = 
        paramsPtr->m_featuresSet2Ptr->getLinesNumber();   
        
      te::sam::rtree::Index< unsigned int > interestPointsSet2RTree;
      
      std::vector< unsigned int > selectedFeaturesSet2Indexes;
      unsigned int selectedFeaturesSet2IndexesSize = 0;      
        
      if( paramsPtr->m_raster1ToRaster2TransfPtr )
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
          
          if( paramsPtr->m_raster1ToRaster2TransfPtr )
          {
            raster1ToRaster2TransfPtr->directMap( 
              paramsPtr->m_interestPointsSet1Ptr[ feat1Idx ].m_x,
              paramsPtr->m_interestPointsSet1Ptr[ feat1Idx ].m_y,
              auxEnvelope.m_llx,
              auxEnvelope.m_lly );
            
            auxEnvelope.m_urx = auxEnvelope.m_llx;
            auxEnvelope.m_ury = auxEnvelope.m_lly;
            
            auxEnvelope.m_llx -= interestPointsSet2RTreeSearchRadius;
            auxEnvelope.m_lly -= interestPointsSet2RTreeSearchRadius;
            auxEnvelope.m_urx += interestPointsSet2RTreeSearchRadius;
            auxEnvelope.m_ury += interestPointsSet2RTreeSearchRadius;
            
            selectedFeaturesSet2Indexes.clear();
            interestPointsSet2RTree.search( auxEnvelope,
              selectedFeaturesSet2Indexes );
              
            selectedFeaturesSet2IndexesSize = selectedFeaturesSet2Indexes.size();
          }          
          
          corrMatrixLinePtr = paramsPtr->m_distMatrixPtr->operator[]( feat1Idx );
          
          feat1Ptr = paramsPtr->m_featuresSet1Ptr->operator[]( feat1Idx );
          
          for( unsigned int selectedFeaturesSet2IndexesIdx = 0 ; 
            selectedFeaturesSet2IndexesIdx < selectedFeaturesSet2IndexesSize ; 
            ++selectedFeaturesSet2IndexesIdx )
          {
            feat2Idx = selectedFeaturesSet2Indexes[ selectedFeaturesSet2IndexesIdx ];
            
            if( 
                ( paramsPtr->m_interestPointsSet1Ptr[ feat1Idx ].m_feature2 ==
                  paramsPtr->m_interestPointsSet2Ptr[ feat2Idx ].m_feature2 )
                &&
                ( paramsPtr->m_interestPointsSet1Ptr[ feat1Idx ].m_feature3 ==
                  paramsPtr->m_interestPointsSet2Ptr[ feat2Idx ].m_feature3 ) 
              )
            {
              feat2Ptr = paramsPtr->m_featuresSet2Ptr->operator[]( feat2Idx );
              
              euclideanDist = 0.0;

              for( featCol = 0 ; featCol < featureElementsNmb ; ++featCol )
              {
                diff = feat1Ptr[ featCol ] - feat2Ptr[ featCol ];
                euclideanDist += ( diff * diff );              
              }
              
              euclideanDist = std::sqrt( euclideanDist );
                  
              corrMatrixLinePtr[ feat2Idx ] = euclideanDist;            
            }
          }
        }
        else
        {
          paramsPtr->m_syncMutexPtr->unlock();
        }
      }
    }        
    
    /* ----------------------------------------------------------------------- */
    
    TiePointsLocatorSURFStrategyFactory::TiePointsLocatorSURFStrategyFactory()
    : te::rp::TiePointsLocatorStrategyFactory( "SURF" )
    {
    }
    
    TiePointsLocatorSURFStrategyFactory::~TiePointsLocatorSURFStrategyFactory()
    {
    }    
    
    te::rp::TiePointsLocatorStrategy* TiePointsLocatorSURFStrategyFactory::build()
    {
      return new te::rp::TiePointsLocatorSURFStrategy();
    }        

  } // end namespace rp
}   // end namespace te    

