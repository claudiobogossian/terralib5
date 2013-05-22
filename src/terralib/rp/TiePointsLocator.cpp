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
  \file terralib/rp/TiePointsLocator.cpp
  \brief Tie points locator.
*/

#include "TiePointsLocator.h"
#include "Macros.h"
#include "../geometry/GTFactory.h"
#include "../common/PlatformUtils.h"
#include "../common/StringUtils.h"
#include "../raster/Band.h"
#include "../raster/Grid.h"
#include "../raster/BandProperty.h"
#include "../raster/RasterFactory.h"
#include "../datatype/Enums.h"
#include "../geometry/GTFilter.h"

#include <boost/scoped_array.hpp>
#include <boost/shared_array.hpp>
#include <boost/lexical_cast.hpp>

#include <algorithm>

#include <climits>
#include <cfloat>
#include <cstdio>
#include <cmath>

namespace te
{
  namespace rp
  {

    TiePointsLocator::InputParameters::InputParameters()
    {
      reset();
    }
    
    TiePointsLocator::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }    

    TiePointsLocator::InputParameters::~InputParameters()
    {
      reset();
    }

    void TiePointsLocator::InputParameters::reset() throw( te::rp::Exception )
    {
      m_interesPointsLocationStrategy = InputParameters::MoravecStrategyT;
      m_inRaster1Ptr = 0;
      m_inMaskRaster1Ptr = 0;
      m_inRaster1Bands.clear();
      m_raster1TargetAreaLineStart = 0;
      m_raster1TargetAreaColStart = 0;
      m_raster1TargetAreaWidth = 0;
      m_raster1TargetAreaHeight = 0;
      m_inRaster2Ptr = 0;
      m_inMaskRaster2Ptr = 0;
      m_inRaster2Bands.clear();
      m_raster2TargetAreaLineStart = 0;
      m_raster2TargetAreaColStart = 0;
      m_raster2TargetAreaWidth = 0;
      m_raster2TargetAreaHeight = 0;
      m_enableMultiThread = true;
      m_enableProgress = false;
      m_maxTiePoints = 0;
      m_pixelSizeXRelation = 1;
      m_pixelSizeYRelation = 1;
      m_geomTransfName = "Affine";
      m_geomTransfMaxError = 2;
      m_correlationWindowWidth = 11;
      m_moravecWindowWidth = 5;
      m_maxR1ToR2Offset = 0;
      m_enableGeometryFilter = true;
      m_geometryFilterAssurance = 0.5;
      m_gaussianFilterIterations = 1;
      m_scalesNumber = 3;
      m_octavesNumber = 2;
      m_rastersRescaleFactor = 1.0;
      m_maxNormEuclideanDist = 0.5;
      m_minAbsCorrelation = 0.5;
      m_interpMethod = te::rst::Interpolator::NearestNeighbor;
    }

    const TiePointsLocator::InputParameters& TiePointsLocator::InputParameters::operator=(
      const TiePointsLocator::InputParameters& params )
    {
      reset();
      
      m_interesPointsLocationStrategy = params.m_interesPointsLocationStrategy;
      m_inRaster1Ptr = params.m_inRaster1Ptr;
      m_inMaskRaster1Ptr = params.m_inMaskRaster1Ptr;
      m_inRaster1Bands = params.m_inRaster1Bands;
      m_raster1TargetAreaLineStart = params.m_raster1TargetAreaLineStart;
      m_raster1TargetAreaColStart = params.m_raster1TargetAreaColStart;
      m_raster1TargetAreaWidth = params.m_raster1TargetAreaWidth;
      m_raster1TargetAreaHeight = params.m_raster1TargetAreaHeight;
      m_inRaster2Ptr = params.m_inRaster2Ptr;
      m_inMaskRaster2Ptr = params.m_inMaskRaster2Ptr;
      m_inRaster2Bands = params.m_inRaster2Bands;
      m_raster2TargetAreaLineStart = params.m_raster2TargetAreaLineStart;
      m_raster2TargetAreaColStart = params.m_raster2TargetAreaColStart;
      m_raster2TargetAreaWidth = params.m_raster2TargetAreaWidth;
      m_raster2TargetAreaHeight = params.m_raster2TargetAreaHeight;
      m_enableMultiThread = params.m_enableMultiThread;
      m_enableProgress = params.m_enableProgress;
      m_maxTiePoints = params.m_maxTiePoints;
      m_pixelSizeXRelation = params.m_pixelSizeXRelation;
      m_pixelSizeYRelation = params.m_pixelSizeYRelation;
      m_geomTransfName = params.m_geomTransfName;
      m_geomTransfMaxError = params.m_geomTransfMaxError;
      m_correlationWindowWidth = params.m_correlationWindowWidth;
      m_moravecWindowWidth = params.m_moravecWindowWidth;
      m_maxR1ToR2Offset = params.m_maxR1ToR2Offset;
      m_enableGeometryFilter = params.m_enableGeometryFilter;
      m_geometryFilterAssurance = params.m_geometryFilterAssurance;
      m_gaussianFilterIterations = params.m_gaussianFilterIterations;
      m_scalesNumber = params.m_scalesNumber;
      m_octavesNumber = params.m_octavesNumber;
      m_rastersRescaleFactor = params.m_rastersRescaleFactor;
      m_maxNormEuclideanDist = params.m_maxNormEuclideanDist;
      m_minAbsCorrelation = params.m_minAbsCorrelation;
      m_interpMethod = params.m_interpMethod;

      return *this;
    }

    te::common::AbstractParameters* TiePointsLocator::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    TiePointsLocator::OutputParameters::OutputParameters()
    {
      reset();
    }

    TiePointsLocator::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    TiePointsLocator::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void TiePointsLocator::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_tiePoints.clear();
      m_transformationPtr.reset();
    }

    const TiePointsLocator::OutputParameters& TiePointsLocator::OutputParameters::operator=(
      const TiePointsLocator::OutputParameters& params )
    {
      reset();
      
      m_tiePoints = params.m_tiePoints;
      m_transformationPtr.reset( params.m_transformationPtr->clone() );

      return *this;
    }

    te::common::AbstractParameters* TiePointsLocator::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    TiePointsLocator::TiePointsLocator()
    {
      reset();
    }

    TiePointsLocator::~TiePointsLocator()
    {
    }
    
    bool TiePointsLocator::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;
      
      TiePointsLocator::OutputParameters* outParamsPtr = dynamic_cast<
        TiePointsLocator::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      /* Calculating the rescale factors 
        factor = rescaled_orignal_image / original_image  */
      
      double raster1XRescFact = 1.0;
      double raster1YRescFact = 1.0;
      double raster2XRescFact = 1.0;
      double raster2YRescFact = 1.0;
      double ransacExpectedDirectMapRmse = m_inputParameters.m_geomTransfMaxError;
      double ransacExpectedInverseMapRmse = m_inputParameters.m_geomTransfMaxError;
      {
        const double meanPixelRelation = ( m_inputParameters.m_pixelSizeXRelation
          + m_inputParameters.m_pixelSizeYRelation ) /
          2.0;
          
        if( meanPixelRelation > 1.0 ) 
        {
          /* The image 1 has poor resolution - bigger pixel resolution values -
            and image 2 needs to be rescaled down */
          
          raster2XRescFact = 1.0 / m_inputParameters.m_pixelSizeXRelation;
          raster2YRescFact = 1.0 / m_inputParameters.m_pixelSizeYRelation;
          ransacExpectedDirectMapRmse *= meanPixelRelation;
        } 
        else if( meanPixelRelation < 1.0 ) 
        {
          /* The image 2 has poor resolution - bigger pixel resolution values
            and image 1 needs to be rescaled down */
          
          raster1XRescFact = m_inputParameters.m_pixelSizeXRelation;
          raster1YRescFact = m_inputParameters.m_pixelSizeYRelation;
          ransacExpectedInverseMapRmse /= meanPixelRelation;
        }
      }
      
      // Applying the global rescale factor
      
      raster1XRescFact *= m_inputParameters.m_rastersRescaleFactor;
      raster1YRescFact *= m_inputParameters.m_rastersRescaleFactor;
      raster2XRescFact *= m_inputParameters.m_rastersRescaleFactor;
      raster2YRescFact *= m_inputParameters.m_rastersRescaleFactor;
      
      /* Calculating the maximum interest points number and the Moravec window
        width for each image trying to keep the same density for both images 
        - this is usefull for the case where the area covered by raster 1
        is different from the area covered by raster 1*/
      
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
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        progressPtr->setTotalSteps( 1 );
        progressPtr->setMessage( "Locating tie points" );
      }
        
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }      
      
      // executing the choosed strategy
      
      std::vector< double > tiePointsWeights;
      
      switch( m_inputParameters.m_interesPointsLocationStrategy )
      {
        case InputParameters::MoravecStrategyT :
        {
          TERP_TRUE_OR_RETURN_FALSE( executeMoravec( 
            raster1XRescFact, 
            raster1YRescFact,
            raster2XRescFact,
            raster2YRescFact,
            raster1MaxInterestPoints,
            raster2MaxInterestPoints,
            *progressPtr,
            outParamsPtr,
            tiePointsWeights ),
            "Moravec execution error" );   
          break;
        }
        case InputParameters::SurfStrategyT :
        {
          TERP_TRUE_OR_RETURN_FALSE( executeSurf( 
            raster1XRescFact, 
            raster1YRescFact,
            raster2XRescFact,
            raster2YRescFact,
            raster1MaxInterestPoints,
            raster2MaxInterestPoints,
            *progressPtr,
            outParamsPtr,
            tiePointsWeights ),
            "SURF execution error" );   
          break;
        }        
        default :
        {
          TERP_LOG_AND_THROW( "Invalid strategy" );
          break;
        }
      }
      
      // Execute outliers remotion, if required
      
      if( m_inputParameters.m_enableGeometryFilter )
      {
        te::gm::GTParameters transfParams;
        transfParams.m_tiePoints = outParamsPtr->m_tiePoints;
        
        te::gm::GTFilter filter;
        TERP_TRUE_OR_RETURN_FALSE( filter.applyRansac( 
          m_inputParameters.m_geomTransfName, 
          transfParams,
          ransacExpectedDirectMapRmse,
          ransacExpectedInverseMapRmse,
          0,
          m_inputParameters.m_geometryFilterAssurance,
          m_inputParameters.m_enableMultiThread,
          tiePointsWeights,
          outParamsPtr->m_tiePoints,
          outParamsPtr->m_transformationPtr  ), "Outliers remotion error" );
      }
      else
      {
        outParamsPtr->m_transformationPtr.reset( te::gm::GTFactory::make( m_inputParameters.m_geomTransfName ) );
        TERP_DEBUG_TRUE_OR_THROW( outParamsPtr->m_transformationPtr.get(), "Invalid transformation" );
        
        te::gm::GTParameters transfParams;
        
        if( ! outParamsPtr->m_transformationPtr->initialize( transfParams ) )
        {
          outParamsPtr->m_transformationPtr.reset();
        }
      }
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }
      
      return true;
    }

    bool TiePointsLocator::executeMoravec( 
      const double raster1XRescFact, 
      const double raster1YRescFact,
      const double raster2XRescFact,
      const double raster2YRescFact,
      const unsigned int raster1MaxInterestPoints,
      const unsigned int raster2MaxInterestPoints,
      te::common::TaskProgress& progress,
      TiePointsLocator::OutputParameters* outParamsPtr,
      std::vector< double >& tiePointsWeights ) 
      throw( te::rp::Exception )
    {
      outParamsPtr->m_tiePoints.clear();
      tiePointsWeights.clear();
      
      // Updating the progress interface steps number
      
      if( m_inputParameters.m_enableProgress )
        progress.setTotalSteps( progress.getTotalSteps() + 10 );
      
      // Generating raster 1 features
      
      Matrix< double > raster1Features;
      InterestPointsSetT raster1InterestPoints;
      {
        // loading raster data
        std::vector< boost::shared_ptr< Matrix< double > > > raster1Data;
        Matrix< unsigned char > maskRaster1Data;
        
        TERP_TRUE_OR_RETURN_FALSE( loadRasterData( 
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
          raster1Data,
          maskRaster1Data ),
          "Error loading raster data" );        
          
        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }
          
        // applying the noise filter

        if( m_inputParameters.m_gaussianFilterIterations )
        {
          boost::shared_ptr< Matrix< double > > tempMatrix( 
            new Matrix< double > );
          TERP_TRUE_OR_RETURN_FALSE( tempMatrix->reset( 
            0, 0,
            Matrix< double >::AutoMemPol, 
            raster1Data[ 0 ]->getMaxTmpFileSize(),
            raster1Data[ 0 ]->getMaxMemPercentUsage() ),
            "Cannot allocate image matrix" );
          
          TERP_TRUE_OR_RETURN_FALSE( applyMeanFilter( 
            *(raster1Data[ 0 ]), 
            *tempMatrix, m_inputParameters.m_gaussianFilterIterations ),
            "Gaussian filter error" );

          raster1Data[ 0 ] = tempMatrix;
          
//        createTifFromMatrix( *(raster1Data[ 0 ]), InterestPointsSetT(), "raster1Filtered");          
        }
        
        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }        
        
        // locating interest points
        
        TERP_TRUE_OR_RETURN_FALSE( locateMoravecInterestPoints( 
          *(raster1Data[ 0 ]), 
          maskRaster1Data.getLinesNumber() ? (&maskRaster1Data) : 0, 
          m_inputParameters.m_moravecWindowWidth,
          raster1MaxInterestPoints,
          m_inputParameters.m_enableMultiThread,
          raster1InterestPoints ),
          "Error locating raster 1 interest points" );
          
        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }          
          
        // Generting features (one feature per line)
        
        raster1Features.reset( Matrix< double >::RAMMemPol );
        InterestPointsSetT auxInterestPoints;
        
        TERP_TRUE_OR_RETURN_FALSE( generateCorrelationFeatures( 
          raster1InterestPoints,
          m_inputParameters.m_correlationWindowWidth,
          *(raster1Data[ 0 ]),
          true,
          raster1Features,
          auxInterestPoints ),
          "Error generating raster 1 features" );
          
        raster1InterestPoints = auxInterestPoints;        
        
//      features2Tiff( raster1Features, raster1InterestPoints, "raster1features" );        

        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }
      }
      
      // Generating raster 2 features
      
      Matrix< double > raster2Features;
      InterestPointsSetT raster2InterestPoints;
      {
        // Loading image data
      
        std::vector< boost::shared_ptr< Matrix< double > > > raster2Data;
        Matrix< unsigned char > maskRaster2Data;
                     
        TERP_TRUE_OR_RETURN_FALSE( loadRasterData( 
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
          raster2Data,
          maskRaster2Data ),
          "Error loading raster data" );
          
        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }          
          
        // applying the noise filter
        
        if( m_inputParameters.m_gaussianFilterIterations )
        {
          boost::shared_ptr< Matrix< double > > tempMatrix( 
            new Matrix< double > );
          
          tempMatrix.reset( new Matrix< double > );
          TERP_TRUE_OR_RETURN_FALSE( tempMatrix->reset( 
            0, 0,
            Matrix< double >::AutoMemPol, 
            raster2Data[ 0 ]->getMaxTmpFileSize(),
            raster2Data[ 0 ]->getMaxMemPercentUsage() ),
            "Cannot allocate image matrix" );          
          
          TERP_TRUE_OR_RETURN_FALSE( applyMeanFilter( 
            *(raster2Data[ 0 ]), 
            *tempMatrix, m_inputParameters.m_gaussianFilterIterations ),
            "Gaussian filter error" );

          raster2Data[ 0 ] = tempMatrix;          
          
//        createTifFromMatrix( *(raster2Data[ 0 ]), InterestPointsSetT(), "raster2Filtered");  
        }
        
        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }        
        
        // locating interest points        
        
        TERP_TRUE_OR_RETURN_FALSE( locateMoravecInterestPoints( 
          *(raster2Data[ 0 ]), 
          maskRaster2Data.getLinesNumber() ? (&maskRaster2Data) : 0, 
          m_inputParameters.m_moravecWindowWidth,
          raster2MaxInterestPoints,
          m_inputParameters.m_enableMultiThread,
          raster2InterestPoints ),
          "Error locating raster 2 interest points" );
          
        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }          
          
        // Generting features (one feature per line)

        raster2Features.reset( Matrix< double >::RAMMemPol );
        InterestPointsSetT auxInterestPoints;        
        
        TERP_TRUE_OR_RETURN_FALSE( generateCorrelationFeatures( 
          raster2InterestPoints,
          m_inputParameters.m_correlationWindowWidth,
          *(raster2Data[ 0 ]),
          true,
          raster2Features,
          auxInterestPoints ),
          "Error generating raster 2 features" );
          
        raster2InterestPoints = auxInterestPoints;
        
//      features2Tiff( raster2Features, raster2InterestPoints, "raster2features" );        

        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }
      }

      // Matching features
      
      MatchedInterestPointsSetT matchedPoints;
      
      TERP_TRUE_OR_RETURN_FALSE( executeMatchingByCorrelation( 
        raster1Features,
        raster2Features,
        raster1InterestPoints,
        raster2InterestPoints,
        m_inputParameters.m_maxR1ToR2Offset,
        m_inputParameters.m_enableMultiThread,
        m_inputParameters.m_minAbsCorrelation,
        matchedPoints ),
        "Error matching features" );
      
      if( m_inputParameters.m_enableProgress )
      {
        progress.pulse();
        if( ! progress.isActive() ) return false;
      }      
      
      // Clean anused data
      
      raster1Features.clear();
      raster2Features.clear();
      raster1InterestPoints.clear();
      raster2InterestPoints.clear();
      
      // Generating the  output tie-points
      
      {
        te::gm::GTParameters::TiePoint auxTP;
        MatchedInterestPointsSetT::const_iterator itB = matchedPoints.begin();
        const MatchedInterestPointsSetT::const_iterator itE = matchedPoints.end();
        
        double minFeatureValue1 = DBL_MAX;
        double maxFeatureValue1 = (-1.0) * DBL_MAX;
        double minFeatureValue2 = DBL_MAX;
        double maxFeatureValue2 = (-1.0) * DBL_MAX;
        double tiePointWeight = 0;
        
        itB = matchedPoints.begin();
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
        
        if( maxFeatureValue1 == minFeatureValue1 )
        {
          minFeatureValue1 = 0;
          maxFeatureValue1 = 1.0;          
        }
        
        if( maxFeatureValue2 == minFeatureValue2 )
        {
          minFeatureValue2 = 0;
          maxFeatureValue2 = 1.0;          
        }        
        
        itB = matchedPoints.begin();
        while( itB != itE )
        {
          auxTP.first.x = ( itB->m_point1.m_x / raster1XRescFact ) + 
            (double)m_inputParameters.m_raster1TargetAreaColStart;
          auxTP.first.y = ( itB->m_point1.m_y / raster1YRescFact ) + 
            (double)m_inputParameters.m_raster1TargetAreaLineStart;          
          auxTP.second.x = ( itB->m_point2.m_x / raster2XRescFact ) + 
            (double)m_inputParameters.m_raster2TargetAreaColStart;
          auxTP.second.y = ( itB->m_point2.m_y / raster2YRescFact ) + 
            (double)m_inputParameters.m_raster2TargetAreaLineStart; 
            
          tiePointWeight = 
            (
              ( 
                (
                  ( 
                    ( itB->m_point1.m_feature1 - minFeatureValue1 ) 
                    /
                    ( maxFeatureValue1 - minFeatureValue1 )
                  )
                  +
                  ( 
                    ( itB->m_point1.m_feature2 - minFeatureValue2 ) 
                    /
                    ( maxFeatureValue2 - minFeatureValue2 )
                  )                  
                )
                /
                2.0
              )
              +
              itB->m_feature
            ) / 2.0;            
           
          tiePointsWeights.push_back( tiePointWeight );
            
          outParamsPtr->m_tiePoints.push_back( auxTP );
          
          ++itB;
        }        
      }
      
      if( m_inputParameters.m_enableProgress )
      {
        progress.pulse();
        if( ! progress.isActive() ) return false;
      }       
      
      return true;
    }
    
    bool TiePointsLocator::executeSurf( 
      const double raster1XRescFact, 
      const double raster1YRescFact,
      const double raster2XRescFact,
      const double raster2YRescFact,
      const unsigned int raster1MaxInterestPoints,
      const unsigned int raster2MaxInterestPoints,
      te::common::TaskProgress& progress,
      TiePointsLocator::OutputParameters* outParamsPtr,
      std::vector< double >& tiePointsWeights ) 
      throw( te::rp::Exception )
    {
      outParamsPtr->m_tiePoints.clear();
      tiePointsWeights.clear();
      
      if( m_inputParameters.m_enableProgress )
        progress.setTotalSteps( progress.getTotalSteps() + 8 );
      
      // Locating interest points and features from raster 1
      
      InterestPointsSetT raster1InterestPoints;
      Matrix< double > raster1Features;
      {
        // Loading image data
        
        std::vector< boost::shared_ptr< Matrix< double > > > rasterData;
        Matrix< unsigned char > maskRasterData;
        
        TERP_TRUE_OR_RETURN_FALSE( loadRasterData( 
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
          rasterData,
          maskRasterData ),
          "Error loading raster data" );
          
//        printMatrix( *(rasterData[ 0 ]) );
//        createTifFromMatrix( *(rasterData[ 0 ]), InterestPointsSetT(), "loadedRaster1");

        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }           
        
        // Creating the integral image
        
        Matrix< double > integralRaster;
        
        TERP_TRUE_OR_RETURN_FALSE( createIntegralImage( *(rasterData[ 0 ]), 
          integralRaster ), "Integral image creation error" );
          
        rasterData.clear();
        
        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }           
          
//        printMatrix( integralRaster );
//        createTifFromMatrix( integralRaster, InterestPointsSetT(), "integralRaster1" );
        
        // locating interest points        
        
        InterestPointsSetT candidateInterestPoints;
        
        TERP_TRUE_OR_RETURN_FALSE( locateSurfInterestPoints( 
          integralRaster, 
          maskRasterData.getLinesNumber() ? (&maskRasterData) : 0, 
          raster1MaxInterestPoints,
          m_inputParameters.m_enableMultiThread,
          m_inputParameters.m_scalesNumber,
          m_inputParameters.m_octavesNumber,
          candidateInterestPoints ),
          "Error locating raster 1 interest points" );
          
//        createTifFromMatrix( *(rasterData[ 0 ]), candidateInterestPoints, "surfInterestPoints1");

        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }           

        
        TERP_TRUE_OR_RETURN_FALSE( generateSurfFeatures( candidateInterestPoints,
          integralRaster, raster1Features, raster1InterestPoints ),
          "Error generating raster features" );        
          
        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }           
          
      }
      
      // Locating interest points and features from raster 2
      
      InterestPointsSetT raster2InterestPoints;
      Matrix< double > raster2Features;
      {
        // Loading image data
        
        std::vector< boost::shared_ptr< Matrix< double > > > rasterData;
        Matrix< unsigned char > maskRasterData;
        
        TERP_TRUE_OR_RETURN_FALSE( loadRasterData( 
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
          rasterData,
          maskRasterData ),
          "Error loading raster data" );
          
//        printMatrix( *(rasterData[ 0 ]) );
//        createTifFromMatrix( *(rasterData[ 0 ]), InterestPointsSetT(), "loadedRaster2");

        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        } 
        
        // Creating the integral image
        
        Matrix< double > integralRaster;
        
        TERP_TRUE_OR_RETURN_FALSE( createIntegralImage( *(rasterData[ 0 ]), 
          integralRaster ), "Integral image creation error" );
          
        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }           
          
        rasterData.clear();
          
//        printMatrix( integralRaster );
//        createTifFromMatrix( integralRaster, InterestPointsSetT(), "integralRaster2" );
        
        // locating interest points        
        
        InterestPointsSetT candidateInterestPoints;
        
        TERP_TRUE_OR_RETURN_FALSE( locateSurfInterestPoints( 
          integralRaster, 
          maskRasterData.getLinesNumber() ? (&maskRasterData) : 0, 
          raster2MaxInterestPoints,
          m_inputParameters.m_enableMultiThread,
          m_inputParameters.m_scalesNumber,
          m_inputParameters.m_octavesNumber,
          candidateInterestPoints ),
          "Error locating raster interest points" );
          
//        createTifFromMatrix( *(rasterData[ 0 ]), candidateInterestPoints, "surfInterestPoints2");

        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }           

        
        TERP_TRUE_OR_RETURN_FALSE( generateSurfFeatures( candidateInterestPoints,
          integralRaster, raster2Features, raster2InterestPoints ),
          "Error generating raster features" );        
          
        if( m_inputParameters.m_enableProgress )
        {
          progress.pulse();
          if( ! progress.isActive() ) return false;
        }           
      }
      
//      printMatrix( raster1Features );
//      printMatrix( raster2Features );
      
      // Matching features
      
      MatchedInterestPointsSetT matchedPoints;
      
      TERP_TRUE_OR_RETURN_FALSE( executeMatchingByEuclideanDist( 
        raster1Features,
        raster2Features,
        raster1InterestPoints,
        raster2InterestPoints,
        m_inputParameters.m_maxR1ToR2Offset,
        m_inputParameters.m_maxNormEuclideanDist * 2.0, /* since surf feature vectors are unitary verctors */
        m_inputParameters.m_enableMultiThread,
        matchedPoints ),
        "Error matching features" );
      
      if( m_inputParameters.m_enableProgress )
      {
        progress.pulse();
        if( ! progress.isActive() ) return false;
      }      
      
      // Generating the  output tie-points
      
      {
        te::gm::GTParameters::TiePoint auxTP;
        MatchedInterestPointsSetT::const_iterator itB = matchedPoints.begin();
        const MatchedInterestPointsSetT::const_iterator itE = matchedPoints.end();
        double minFeature1 = DBL_MAX;
        double maxFeature1 = (-1.0) * DBL_MAX;
        double minFeature2 = DBL_MAX;
        double maxFeature2 = (-1.0) * DBL_MAX;
        
        while( itB != itE )
        {
          if( minFeature1 > itB->m_point1.m_feature1 )
            minFeature1 = itB->m_point1.m_feature1;
          if( maxFeature1 < itB->m_point1.m_feature1 )
            maxFeature1 = itB->m_point1.m_feature1;
          if( minFeature2 > itB->m_point2.m_feature1 )
            minFeature2 = itB->m_point2.m_feature1;
          if( maxFeature2 < itB->m_point2.m_feature1 )
            maxFeature2 = itB->m_point2.m_feature1;
          
          ++itB;
        }
        
        if( minFeature1 == maxFeature1 )
        {
          minFeature1 = 0;
          maxFeature1 = 1.0;
        }
        if( minFeature2 == maxFeature2 )
        {
          minFeature2 = 0;
          maxFeature2 = 1.0;
        }        
        
        itB = matchedPoints.begin();
        
        while( itB != itE )
        {
          auxTP.first.x = ( itB->m_point1.m_x / raster1XRescFact ) + 
            (double)m_inputParameters.m_raster1TargetAreaColStart;
          auxTP.first.y = ( itB->m_point1.m_y / raster1YRescFact ) + 
            (double)m_inputParameters.m_raster1TargetAreaLineStart;          
          auxTP.second.x = ( itB->m_point2.m_x / raster2XRescFact ) + 
            (double)m_inputParameters.m_raster2TargetAreaColStart;
          auxTP.second.y = ( itB->m_point2.m_y / raster2YRescFact ) + 
            (double)m_inputParameters.m_raster2TargetAreaLineStart;   
           
          tiePointsWeights.push_back( 
              (
                itB->m_feature 
                +
                (
                  (
                    (
                      ( itB->m_point1.m_feature1 - minFeature1 ) /
                      ( maxFeature1 - minFeature1 )
                    )
                    +
                    (
                      ( itB->m_point2.m_feature1 - minFeature2 ) /
                      ( maxFeature2 - minFeature2 )
                    )
                  )
                  /
                  2.0
                )
              )
              /
              2.0
            );
            
          outParamsPtr->m_tiePoints.push_back( auxTP );
          
          ++itB;
        }        
      }                  

      return true;
    }


    void TiePointsLocator::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool TiePointsLocator::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();
      
      {
        TiePointsLocator::InputParameters const* inputParamsPtr = dynamic_cast<
          TiePointsLocator::InputParameters const* >( &inputParams );
        TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr, "Invalid parameters" );
        
        m_inputParameters = *inputParamsPtr;
      }
      
      // Checking m_inRaster1Ptr
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster1Ptr,
        "Invalid m_inRaster1Ptr" );
      TERP_TRUE_OR_RETURN_FALSE(
        m_inputParameters.m_inRaster1Ptr->getAccessPolicy() & te::common::RAccess,
        "Invalid m_inRaster1Ptr" );
        
      // Checking m_inMaskRaster1Ptr
        
      if(m_inputParameters.m_inMaskRaster1Ptr )
      {
        TERP_TRUE_OR_RETURN_FALSE(
          m_inputParameters.m_inMaskRaster1Ptr->getAccessPolicy() & te::common::RAccess,
          "Invalid m_inMaskRaster1Ptr" );
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inMaskRaster1Ptr->getNumberOfBands() > 0,
          "Invalid m_inMaskRaster1Ptr" );
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inMaskRaster1Ptr->getNumberOfRows() ==
          m_inputParameters.m_inRaster1Ptr->getNumberOfRows(),
          "Invalid m_inMaskRaster1Ptr" );
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inMaskRaster1Ptr->getNumberOfColumns() ==
          m_inputParameters.m_inRaster1Ptr->getNumberOfColumns(),
          "Invalid m_inMaskRaster1Ptr" );          
      }
      
      // Checking raster 1 target area
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_raster1TargetAreaLineStart <
        m_inputParameters.m_inRaster1Ptr->getNumberOfRows(),
        "Invalid m_raster1TargetAreaLineStart" );
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_raster1TargetAreaColStart <
        m_inputParameters.m_inRaster1Ptr->getNumberOfColumns(),
        "Invalid m_raster1TargetAreaColStart" );     
        
      if( m_inputParameters.m_raster1TargetAreaWidth ||
        m_inputParameters.m_raster1TargetAreaHeight )
      {
        m_inputParameters.m_raster1TargetAreaWidth = std::min( 
          m_inputParameters.m_raster1TargetAreaWidth,
          ( ( unsigned int ) m_inputParameters.m_inRaster1Ptr->getNumberOfColumns() )
          - m_inputParameters.m_raster1TargetAreaColStart );
          
        m_inputParameters.m_raster1TargetAreaHeight = std::min(
          m_inputParameters.m_raster1TargetAreaHeight,
          ( ( unsigned int ) m_inputParameters.m_inRaster1Ptr->getNumberOfRows() )
          - m_inputParameters.m_raster1TargetAreaLineStart );                                                     
      }
      else
      {
        m_inputParameters.m_raster1TargetAreaWidth =
          m_inputParameters.m_inRaster1Ptr->getNumberOfColumns();
        m_inputParameters.m_raster1TargetAreaHeight = 
          m_inputParameters.m_inRaster1Ptr->getNumberOfRows();
      }

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_raster1TargetAreaWidth,
        "Invalid m_raster1TargetAreaWidth" );
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_raster1TargetAreaHeight ,
        "Invalid m_raster1TargetAreaHeight" );
        
      // Checking raster 1 bands
      {
        for( unsigned int bandIdx = 0 ; bandIdx <
          m_inputParameters.m_inRaster1Bands.size() ; ++bandIdx )
        {
          TERP_TRUE_OR_RETURN_FALSE(
            m_inputParameters.m_inRaster1Bands[ bandIdx ] <
            m_inputParameters.m_inRaster1Ptr->getNumberOfBands(),
            "Invalid m_inRaster1Bands" );
        }
      }
      
      // Checking raster 2
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster2Ptr,
        "Invalid m_inRaster2Ptr" );
      TERP_TRUE_OR_RETURN_FALSE(
        m_inputParameters.m_inRaster2Ptr->getAccessPolicy() & te::common::RAccess,
        "Invalid m_inRaster2Ptr" );      
        
      // Checking m_inMaskRaster2Ptr
        
      if(m_inputParameters.m_inMaskRaster2Ptr )
      {
        TERP_TRUE_OR_RETURN_FALSE(
          m_inputParameters.m_inMaskRaster2Ptr->getAccessPolicy() & te::common::RAccess,
          "Invalid m_inMaskRaster2Ptr" );
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inMaskRaster2Ptr->getNumberOfBands() > 0,
          "Invalid m_inMaskRaster2Ptr" );
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inMaskRaster2Ptr->getNumberOfRows() ==
          m_inputParameters.m_inRaster2Ptr->getNumberOfRows(),
          "Invalid m_inMaskRaster2Ptr" );
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inMaskRaster2Ptr->getNumberOfColumns() ==
          m_inputParameters.m_inRaster2Ptr->getNumberOfColumns(),
          "Invalid m_inMaskRaster2Ptr" );          
      }
      
      // Checking raster target area
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_raster2TargetAreaLineStart <
        m_inputParameters.m_inRaster2Ptr->getNumberOfRows(),
        "Invalid m_raster2TargetAreaLineStart" );
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_raster2TargetAreaColStart <
        m_inputParameters.m_inRaster2Ptr->getNumberOfColumns(),
        "Invalid m_raster2TargetAreaColStart" );
        
      if( m_inputParameters.m_raster2TargetAreaWidth ||
        m_inputParameters.m_raster2TargetAreaHeight )
      {
        m_inputParameters.m_raster2TargetAreaWidth = std::min( 
          m_inputParameters.m_raster2TargetAreaWidth,
          ( ( unsigned int ) m_inputParameters.m_inRaster2Ptr->getNumberOfColumns() )
          - m_inputParameters.m_raster2TargetAreaColStart );
          
        m_inputParameters.m_raster2TargetAreaHeight = std::min(
          m_inputParameters.m_raster2TargetAreaHeight,
          ( ( unsigned int ) m_inputParameters.m_inRaster2Ptr->getNumberOfRows() )
          - m_inputParameters.m_raster2TargetAreaLineStart );
      }
      else
      {
        m_inputParameters.m_raster2TargetAreaWidth =
          m_inputParameters.m_inRaster2Ptr->getNumberOfColumns();
        m_inputParameters.m_raster2TargetAreaHeight =
          m_inputParameters.m_inRaster2Ptr->getNumberOfRows();
      }
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_raster2TargetAreaWidth,
        "Invalid m_raster2TargetAreaWidth" );        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_raster2TargetAreaHeight ,
        "Invalid m_raster2TargetAreaHeight" );
        
      // Checking raster 2 bands

      {
        for( unsigned int bandIdx = 0 ; bandIdx <
          m_inputParameters.m_inRaster2Bands.size() ; ++bandIdx )
        {
          TERP_TRUE_OR_RETURN_FALSE(
            m_inputParameters.m_inRaster2Bands[ bandIdx ] <
            m_inputParameters.m_inRaster2Ptr->getNumberOfBands(),
            "Invalid m_inRaster2Bands" );
        }
      }
      
      // Checking strategy specific parameters
      
      switch( m_inputParameters.m_interesPointsLocationStrategy )
      {
        case InputParameters::MoravecStrategyT :
        {
          TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster1Bands.size()
            == 1, "Invalid number of raster 1 bands" );
          TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster2Bands.size()
            == 1, "Invalid number of raster 2 bands" );
            
          // Defining the number of tie points
          
          if( m_inputParameters.m_maxTiePoints == 0 )
          {
            m_inputParameters.m_maxTiePoints = 
              ( m_inputParameters.m_raster1TargetAreaWidth *
              m_inputParameters.m_raster1TargetAreaWidth ) /
              ( 50 * m_inputParameters.m_correlationWindowWidth *
              m_inputParameters.m_correlationWindowWidth );
          }
          
          break;
        }          
        case InputParameters::SurfStrategyT :
        {
          TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster1Bands.size()
            == 1, "Invalid number of raster 1 bands" );
          TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster2Bands.size()
            == 1, "Invalid number of raster 2 bands" );
            
          // Defining the number of tie points
          
          if( m_inputParameters.m_maxTiePoints == 0 )
          {
            m_inputParameters.m_maxTiePoints = 
              getSurfFilterSize( m_inputParameters.m_octavesNumber - 1, 
                 m_inputParameters.m_scalesNumber - 1 );
            m_inputParameters.m_maxTiePoints = 
              ( m_inputParameters.m_raster1TargetAreaWidth *
              m_inputParameters.m_raster1TargetAreaWidth ) /            
              ( 50 * m_inputParameters.m_maxTiePoints *
              m_inputParameters.m_maxTiePoints );
          }            
            
          break;
        }
        default :
        {
          TERP_LOG_AND_RETURN_FALSE( "Invalid strategy" );
          break;
        }
      };
      
      // Checking other parameters
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_maxTiePoints > 0,
        "Invalid m_maxTiePoints" )      
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_pixelSizeXRelation > 0,
        "Invalid m_pixelSizeXRelation" )
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_pixelSizeYRelation > 0,
        "Invalid m_pixelSizeYRelation" )
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_geomTransfMaxError > 0,
        "Invalid m_geomTransfMaxError" )        
        
      m_inputParameters.m_correlationWindowWidth += 
        m_inputParameters.m_correlationWindowWidth % 2 ? 0 : 1;
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_correlationWindowWidth > 2,
        "Invalid m_correlationWindowWidth" );

      m_inputParameters.m_moravecWindowWidth += 
        m_inputParameters.m_moravecWindowWidth % 2 ? 0 : 1;
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_moravecWindowWidth > 2,
        "Invalid m_moravecWindowWidth" );
        
      TERP_TRUE_OR_RETURN_FALSE( te::gm::GTFactory::find( 
        m_inputParameters.m_geomTransfName ),
        "Invalid m_geomTransfName" );
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_scalesNumber > 2,
        "Invalid m_scalesNumber" );        
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_octavesNumber > 0,
        "Invalid m_octavesNumber" );           
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_rastersRescaleFactor > 0,
        "Invalid m_octavesNumber" );  
        
      TERP_TRUE_OR_RETURN_FALSE( ( m_inputParameters.m_maxNormEuclideanDist >= 0 ) &&
        ( m_inputParameters.m_maxNormEuclideanDist <= 1.0 ),
        "Invalid m_octavesNumber" );         
        
      TERP_TRUE_OR_RETURN_FALSE( ( m_inputParameters.m_minAbsCorrelation >= 0 ) &&
        ( m_inputParameters.m_minAbsCorrelation <= 1.0 ),
        "Invalid m_minAbsCorrelation" );         
        
      TERP_TRUE_OR_RETURN_FALSE( ( m_inputParameters.m_geometryFilterAssurance >= 0 ) &&
        ( m_inputParameters.m_geometryFilterAssurance <= 1.0 ),
        "Invalid m_geometryFilterAssurance" );          
        
      m_isInitialized = true;

      return true;
    }

    bool TiePointsLocator::isInitialized() const
    {
      return m_isInitialized;
    }

    bool TiePointsLocator::loadRasterData( 
      te::rst::Raster const* rasterPtr,
      const std::vector< unsigned int >& rasterBands,
      te::rst::Raster const* maskRasterPtr,
      const unsigned int maskRasterBand,
      const unsigned int rasterTargetAreaLineStart,
      const unsigned int rasterTargetAreaColStart,
      const unsigned int rasterTargetAreaWidth,
      const unsigned int rasterTargetAreaHeight,
      const double rescaleFactorX,
      const double rescaleFactorY,
      const te::rst::Interpolator::Method rasterInterpMethod,
      std::vector< boost::shared_ptr< Matrix< double > > >& loadedRasterData,
      Matrix< unsigned char >& loadedMaskRasterData )
    {
      // Allocating the output matrixes
      
      const unsigned int rescaledNLines = (unsigned int)(
        ((double)rasterTargetAreaHeight) * rescaleFactorY );
      const unsigned int rescaledNCols = (unsigned int)(
        ((double)rasterTargetAreaWidth) * rescaleFactorX );
    
      {
        const unsigned int maxMemPercentUsagePerMatrix = MAX( 1u, 25 / 
          ( 1 + ((unsigned int)rasterBands.size()) ) );
        const unsigned long int  maxTmpFileSize = 2ul * 1024ul * 1024ul * 1024ul;
        
        if( maskRasterPtr ) 
        {
          TERP_TRUE_OR_RETURN_FALSE( loadedMaskRasterData.reset( 
            rescaledNLines, rescaledNCols,
            Matrix< unsigned char >::AutoMemPol, maxTmpFileSize,
            maxMemPercentUsagePerMatrix ),
            "Cannot allocate image 1 mask matrix" );          
        }
        
        loadedRasterData.resize( rasterBands.size() );
        
        for( unsigned int rasterBandsIdx = 0 ; rasterBandsIdx < rasterBands.size() ;
          ++rasterBandsIdx )
        {
          loadedRasterData[ rasterBandsIdx ].reset( new Matrix< double > );
          TERP_TRUE_OR_RETURN_FALSE( loadedRasterData[ rasterBandsIdx ]->reset( 
            rescaledNLines, rescaledNCols,
            Matrix< double >::AutoMemPol, maxTmpFileSize,
            maxMemPercentUsagePerMatrix ),
            "Cannot allocate image 1 matrix" );
        }
      }
      
      // loading mask data
      
      if( maskRasterPtr )
      {
        te::rst::Band const* inMaskRasterBand = maskRasterPtr->getBand( maskRasterBand );
        assert( inMaskRasterBand );
        
        unsigned char* outMaskLinePtr = 0;
        unsigned int outLine = 0;
        unsigned int outCol = 0;
        unsigned int inLine = 0;
        unsigned int inCol = 0;
        double value = 0;
        
        for( outLine = 0 ; outLine < rescaledNLines ; ++outLine ) 
        {
          inLine = (unsigned int)( ( ( (double)outLine ) / 
            rescaleFactorY ) + ( (double)rasterTargetAreaLineStart ) );      
          
          outMaskLinePtr = loadedMaskRasterData[ outLine ];
          
          for( outCol = 0 ; outCol < rescaledNCols ; ++outCol ) 
          {          
            inCol = (unsigned int)( ( ( (double)outCol ) / 
                rescaleFactorX ) + ( (double)rasterTargetAreaColStart ) );        
                
            inMaskRasterBand->getValue( inCol, inLine, value );
            
            if( value == 0 )
              outMaskLinePtr[ outCol ] = 0;
            else
              outMaskLinePtr[ outCol ] = 255;
          }
        }
      }
      
      // loading raster data     
      {
        const double rasterTargetAreaLineStartDouble = (double)
          rasterTargetAreaLineStart;
        const double rasterTargetAreaColStartDouble = (double)
          rasterTargetAreaColStart;
        te::rst::Interpolator interpInstance( rasterPtr, rasterInterpMethod );
        double* outLinePtr = 0;
        unsigned int outLine = 0;
        unsigned int outCol = 0;
        double inLine = 0;
        double inCol = 0;     
        std::complex< double > interpolatedValue;
        unsigned int bandIdx = 0;
        
        for( unsigned int rasterBandsIdx = 0 ; rasterBandsIdx < rasterBands.size() ;
          ++rasterBandsIdx )
        {
          bandIdx= rasterBands[ rasterBandsIdx ];
          
          for( outLine = 0 ; outLine < rescaledNLines ; ++outLine ) 
          {
            inLine = ( ( (double)outLine ) /  rescaleFactorY ) + 
              rasterTargetAreaLineStartDouble;
              
            outLinePtr = loadedRasterData[ rasterBandsIdx ]->operator[]( outLine  );
            
            for( outCol = 0 ; outCol < rescaledNCols ; ++outCol ) 
            {          
              inCol = ( ( (double)outCol ) / rescaleFactorX ) + 
                rasterTargetAreaColStartDouble;

              interpInstance.getValue( inCol, inLine, interpolatedValue,
                bandIdx );
              
              outLinePtr[ outCol ] = interpolatedValue.real();
            }
          }
        }
      }
      
      return true;
    }
    
    bool TiePointsLocator::locateMoravecInterestPoints( 
      const Matrix< double >& rasterData,
      Matrix< unsigned char > const* maskRasterDataPtr,
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
    
    void TiePointsLocator::locateMoravecInterestPointsThreadEntry(MoravecLocatorThreadParams* paramsPtr)
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
        MoravecLocatorThreadParams::RasterDataContainerT::ElementTypeT ) * 
        bufferCols;
      const unsigned int maskRasterBufferLineSizeBytes = sizeof(
        MoravecLocatorThreadParams::MaskRasterDataContainerT::ElementTypeT ) * 
        bufferCols;
      
      paramsPtr->m_rastaDataAccessMutexPtr->unlock();
        
      Matrix< double > rasterBufferDataHandler;
      if( ! rasterBufferDataHandler.reset( bufferLines, bufferCols, 
        Matrix< double >::RAMMemPol ) )
      {
        paramsPtr->m_rastaDataAccessMutexPtr->lock();
        paramsPtr->m_returnValuePtr = false;
        paramsPtr->m_rastaDataAccessMutexPtr->unlock();
        return;
      }
      
      boost::scoped_array< double* > rasterBufferHandler( new double*[ bufferLines ] );
      for( unsigned int rasterBufferDataHandlerLine = 0 ; rasterBufferDataHandlerLine < 
        bufferLines ; ++rasterBufferDataHandlerLine )
      {
        rasterBufferHandler[ rasterBufferDataHandlerLine ] = rasterBufferDataHandler[ 
          rasterBufferDataHandlerLine ];
      }
      
      double** rasterBufferPtr = rasterBufferHandler.get();
      
      // Allocating the mask raster buffer      
      
      Matrix< double > maskRasterBufferDataHandler;
      
      boost::scoped_array< double* > maskRasterBufferHandler( new double*[ bufferLines ] );
      
      double** maskRasterBufferPtr = 0;
      
      if( paramsPtr->m_maskRasterDataPtr )
      {
        if( ! maskRasterBufferDataHandler.reset( bufferLines, bufferCols, 
          Matrix< double >::RAMMemPol ) )
        {
          paramsPtr->m_rastaDataAccessMutexPtr->lock();
          paramsPtr->m_returnValuePtr = false;
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
        
      Matrix< double > maximasBufferDataHandler;
      if( ! maximasBufferDataHandler.reset( bufferLines, bufferCols, 
        Matrix< double >::RAMMemPol ) )
      {
        paramsPtr->m_rastaDataAccessMutexPtr->lock();
        paramsPtr->m_returnValuePtr = false;
        paramsPtr->m_rastaDataAccessMutexPtr->unlock();
        return;
      }
      
      boost::scoped_array< double* > maximasBufferHandler( new double*[ bufferLines ] );
      double** maximasBufferPtr = maximasBufferHandler.get();      
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
          double horVar = 0;
          double verVar = 0;
          double diagVar = 0;
          double adiagVar = 0;
          double diffValue = 0;
          bool isLocalMaxima = false;
          InterestPointT auxInterestPoint;
                   
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
                const double& windowCenterPixelValue = rasterBufferPtr[
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
                const double& windowCenterPixelValue = maximasBufferPtr[
                  moravecWindowRadius ][ windowStartBufCol + 
                  moravecWindowRadius ];                
                
                for( windowStartBufYOffset = 0 ; windowStartBufYOffset < 
                  moravecWindowWidth ; ++windowStartBufYOffset )
                {
                  for( windowStartBufXOffset = 0 ; windowStartBufXOffset < 
                    moravecWindowWidth ; ++windowStartBufXOffset )
                  {
                    if( windowCenterPixelValue < maximasBufferPtr[
                      windowStartBufYOffset ][ windowStartBufCol + 
                      windowStartBufXOffset ] )
                    {
                      isLocalMaxima = false;
                      windowStartBufYOffset = moravecWindowWidth;
                      break;
                    }
                  }
                }
                
                if( isLocalMaxima )
                {
                  auxInterestPoint.m_x = windowStartBufCol + 
                    moravecWindowRadius;
                  auxInterestPoint.m_y = rasterLine - moravecWindowWidth + 1;
                  auxInterestPoint.m_feature1 = windowCenterPixelValue;
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
    
    bool TiePointsLocator::locateSurfInterestPoints( 
      const Matrix< double >& integralRasterData,
      Matrix< unsigned char > const* maskRasterDataPtr,
      const unsigned int maxInterestPoints,
      const unsigned int enableMultiThread,
      const unsigned int scalesNumber,
      const unsigned int octavesNumber,
      InterestPointsSetT& interestPoints )
    {
      interestPoints.clear();
      
      // finding interest points
      
      bool returnValue = true;
      boost::mutex rastaDataAccessMutex;
      boost::mutex interestPointsAccessMutex;
      unsigned int nextRasterLinesBlockToProcess = 0;
      
      SurfLocatorThreadParams threadParams;
      threadParams.m_returnValuePtr = &returnValue;
      threadParams.m_rastaDataAccessMutexPtr = &rastaDataAccessMutex;
      threadParams.m_interestPointsAccessMutexPtr = &interestPointsAccessMutex;
      threadParams.m_nextRasterLinesBlockToProcessValuePtr = 
        &nextRasterLinesBlockToProcess;
      threadParams.m_interestPointsPtr = &interestPoints;
      threadParams.m_integralRasterDataPtr = &integralRasterData;
      threadParams.m_maskRasterDataPtr = maskRasterDataPtr;
      threadParams.m_scalesNumber = scalesNumber;
      threadParams.m_octavesNumber = octavesNumber;
      
      if( enableMultiThread )
      {
        const unsigned int procsNumber = te::common::GetPhysProcNumber();
        const unsigned int maxGausFilterWidth = getSurfFilterSize( 
          octavesNumber - 1, scalesNumber - 1 );          
        
        threadParams.m_maxRasterLinesBlockMaxSize = std::max(
          4 * maxGausFilterWidth, integralRasterData.getLinesNumber() / procsNumber );
        threadParams.m_maxRasterLinesBlockMaxSize = std::min(
          threadParams.m_maxRasterLinesBlockMaxSize, 
          integralRasterData.getLinesNumber() );          
          
        const unsigned int rasterLinesBlocksNumber = 
          ( integralRasterData.getLinesNumber() / threadParams.m_maxRasterLinesBlockMaxSize ) +
          ( ( integralRasterData.getLinesNumber() % threadParams.m_maxRasterLinesBlockMaxSize ) ? 1 : 0 );

        threadParams.m_maxInterestPointsPerRasterLinesBlock =
          maxInterestPoints / rasterLinesBlocksNumber;
        
        boost::thread_group threads;
        
        for( unsigned int threadIdx = 0 ; threadIdx < procsNumber ;
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
        threadParams.m_maxRasterLinesBlockMaxSize = integralRasterData.getLinesNumber();
        threadParams.m_maxInterestPointsPerRasterLinesBlock = maxInterestPoints;
        
        locateSurfInterestPointsThreadEntry( &threadParams );
      }
     
      return returnValue;
    }
    

    
    void TiePointsLocator::locateSurfInterestPointsThreadEntry(SurfLocatorThreadParams* paramsPtr)
    {
      assert( paramsPtr );
      assert( paramsPtr->m_returnValuePtr );
      assert( paramsPtr->m_integralRasterDataPtr );
      assert( paramsPtr->m_interestPointsPtr );
      assert( paramsPtr->m_rastaDataAccessMutexPtr );
      assert( paramsPtr->m_interestPointsAccessMutexPtr );
      assert( paramsPtr->m_maxRasterLinesBlockMaxSize > 2 );
      assert( paramsPtr->m_nextRasterLinesBlockToProcessValuePtr );
      assert( paramsPtr->m_scalesNumber > 2 );
      assert( paramsPtr->m_octavesNumber > 0 );
      
      // Globals
      
      const unsigned int maxGausFilterWidth = getSurfFilterSize( 
        paramsPtr->m_octavesNumber - 1, paramsPtr->m_scalesNumber - 1 );
      const unsigned int maxGausFilterRadius = maxGausFilterWidth / 2;
      const unsigned int prevResponseBufferLineIdx = maxGausFilterRadius - 1;
      const unsigned int nextResponseBufferLineIdx = maxGausFilterRadius + 1;
      const unsigned int buffersLines = maxGausFilterWidth;
      const unsigned int lastBuffersLineIdx = buffersLines - 1;

      paramsPtr->m_rastaDataAccessMutexPtr->lock();
      const unsigned int rasterLines = paramsPtr->m_integralRasterDataPtr->getLinesNumber();
      const unsigned int buffersCols = paramsPtr->m_integralRasterDataPtr->getColumnsNumber();
      const unsigned int rasterBufferLineSizeBytes = sizeof( double ) * 
        buffersCols;
      const unsigned int maskRasterBufferLineSizeBytes = sizeof( double ) * 
        buffersCols;
      paramsPtr->m_rastaDataAccessMutexPtr->unlock();  
      
      const unsigned maxGausFilterCenterBufferColBound = buffersCols - 
        maxGausFilterRadius;
      
      unsigned int scaleIdx = 0 ;
      unsigned int octaveIdx = 0 ;
      
      // Allocating the internal raster data buffer
      // and the mask raster buffer
      
      Matrix< double > rasterBufferDataHandler;
      boost::scoped_array< double* > rasterBufferHandler( new double*[ buffersLines ] );
      {
        if( ! rasterBufferDataHandler.reset( buffersLines, buffersCols, 
          Matrix< double >::RAMMemPol ) )
        {
          paramsPtr->m_rastaDataAccessMutexPtr->lock();
          paramsPtr->m_returnValuePtr = false;
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          return;
        }
        
        for( unsigned int rasterBufferDataHandlerLine = 0 ; rasterBufferDataHandlerLine < 
          buffersLines ; ++rasterBufferDataHandlerLine )
        {
          rasterBufferHandler[ rasterBufferDataHandlerLine ] = rasterBufferDataHandler[ 
            rasterBufferDataHandlerLine ];
        }
      }
      double** rasterBufferPtr = rasterBufferHandler.get();
      
      // Allocating the mask raster buffer      
      
      Matrix< double > maskRasterBufferDataHandler;
      
      boost::scoped_array< double* > maskRasterBufferHandler( new double*[ buffersLines ] );
      
      double** maskRasterBufferPtr = 0;
      
      if( paramsPtr->m_maskRasterDataPtr )
      {
        if( ! maskRasterBufferDataHandler.reset( buffersLines, buffersCols, 
          Matrix< double >::RAMMemPol ) )
        {
          paramsPtr->m_rastaDataAccessMutexPtr->lock();
          paramsPtr->m_returnValuePtr = false;
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          return;
        }        
        
        for( unsigned int maskRasterBufferDataHandlerLine = 0 ; maskRasterBufferDataHandlerLine < 
          buffersLines ; ++maskRasterBufferDataHandlerLine )
        {
          maskRasterBufferHandler[ maskRasterBufferDataHandlerLine ] = maskRasterBufferDataHandler[ 
            maskRasterBufferDataHandlerLine ];
        }
        
        maskRasterBufferPtr = maskRasterBufferHandler.get();      
      }
      
      // Allocating the internal octaves buffers
      
      Matrix< double > octavesBufferDataHandler;
      std::vector< std::vector< boost::shared_array< double* > > >
        octavesBufferHandlers;
      {
        const unsigned int octavesBufferDataHandlerLines = 
          buffersLines * paramsPtr->m_octavesNumber * paramsPtr->m_scalesNumber;
        if( ! octavesBufferDataHandler.reset( octavesBufferDataHandlerLines , 
          buffersCols, 
          Matrix< double >::RAMMemPol ) )
        {
          paramsPtr->m_rastaDataAccessMutexPtr->lock();
          paramsPtr->m_returnValuePtr = false;
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          return;
        }
        unsigned int octavesBufferDataHandlerLine = 0;
        unsigned int octavesBufferDataHandlerCol = 0;
        double* octavesBufferDataHandlerLinePtr = 0;
        const double fillValue = (-1.0) * DBL_MAX;
        for( octavesBufferDataHandlerLine = 0; octavesBufferDataHandlerLine < 
          octavesBufferDataHandlerLines ; ++octavesBufferDataHandlerLine )
        {
          octavesBufferDataHandlerLinePtr = octavesBufferDataHandler[ 
            octavesBufferDataHandlerLine ];
            
          for( octavesBufferDataHandlerCol = 0; octavesBufferDataHandlerCol < 
            buffersCols ; ++octavesBufferDataHandlerCol )
          {
            octavesBufferDataHandlerLinePtr[ octavesBufferDataHandlerCol ] = fillValue;
          }
        }
        
        octavesBufferDataHandlerLine = 0;
        for( octaveIdx = 0 ; octaveIdx < paramsPtr->m_octavesNumber ; ++octaveIdx )
        {
          octavesBufferHandlers.push_back( std::vector< boost::shared_array< double* > >() );
          std::vector< boost::shared_array< double* > >&
            currOctaveBuffersHandler = octavesBufferHandlers.back();
          
          for( scaleIdx = 0 ; scaleIdx < paramsPtr->m_scalesNumber ; ++scaleIdx )
          {
            currOctaveBuffersHandler.push_back( boost::shared_array< double* >(
              new double*[ buffersLines ] ) );
            boost::shared_array< double* >& currOctavesBuffer = 
              currOctaveBuffersHandler.back();
            for( unsigned int bufferLine = 0 ; bufferLine <  buffersLines ; 
              ++bufferLine )
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
      
      Matrix< unsigned char > laplacianSignBufferDataHandler;
      std::vector< std::vector< boost::shared_array< unsigned char* > > >
        laplacianSignBufferHandlers;
      {
        const unsigned int laplacianSignBufferDataHandlerLines = 
          buffersLines * paramsPtr->m_octavesNumber * paramsPtr->m_scalesNumber;
        if( ! laplacianSignBufferDataHandler.reset( laplacianSignBufferDataHandlerLines , 
          buffersCols, 
          Matrix< unsigned char >::RAMMemPol ) )
        {
          paramsPtr->m_rastaDataAccessMutexPtr->lock();
          paramsPtr->m_returnValuePtr = false;
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          return;
        }
        unsigned int laplacianSignBufferDataHandlerLine = 0;
        unsigned char* laplacianSignBufferDataHandlerLinePtr = 0;
        for( laplacianSignBufferDataHandlerLine = 0; laplacianSignBufferDataHandlerLine < 
          laplacianSignBufferDataHandlerLines ; ++laplacianSignBufferDataHandlerLine )
        {
          laplacianSignBufferDataHandlerLinePtr = laplacianSignBufferDataHandler[ 
            laplacianSignBufferDataHandlerLine ];
        }
        
        laplacianSignBufferDataHandlerLine = 0;
        for( octaveIdx = 0 ; octaveIdx < paramsPtr->m_octavesNumber ; ++octaveIdx )
        {
          laplacianSignBufferHandlers.push_back( std::vector< boost::shared_array< unsigned char* > >() );
          std::vector< boost::shared_array< unsigned char* > >&
            currlaplacianSignBuffersHandler = laplacianSignBufferHandlers.back();
          
          for( scaleIdx = 0 ; scaleIdx < paramsPtr->m_scalesNumber ; ++scaleIdx )
          {
            currlaplacianSignBuffersHandler.push_back( boost::shared_array< unsigned char* >(
              new unsigned char*[ buffersLines ] ) );
            boost::shared_array< unsigned char* >& currlaplacianSignBuffer = 
              currlaplacianSignBuffersHandler.back();
            for( unsigned int bufferLine = 0 ; bufferLine <  buffersLines ; 
              ++bufferLine )
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
          
          // globals
          
          const unsigned int rasterLinesStart = (unsigned int)std::max( 0,
            (int)(rasterLinesBlockIdx * paramsPtr->m_maxRasterLinesBlockMaxSize ) - 
            (int)( 2 * maxGausFilterRadius ) ); 
          const unsigned int rasterLinesEndBound = std::min( 1 +
            (rasterLinesBlockIdx * paramsPtr->m_maxRasterLinesBlockMaxSize ) + 
            paramsPtr->m_maxRasterLinesBlockMaxSize + 
            ( 2 * maxGausFilterRadius ), rasterLines );
          const unsigned int firstRasterLineToGenerateResponse = 
            rasterLinesStart + maxGausFilterWidth - 1;
          const unsigned int firstRasterLineToLookForMaximas =
            firstRasterLineToGenerateResponse + maxGausFilterRadius + 1;
//          unsigned int baseFilterSize = 0;
//          unsigned int filterStepSize = 0;
          unsigned int filterWidth = 0;
          unsigned int filterLobeWidth = 0;
          unsigned int filterLobeRadius = 0;
          unsigned int filterRadius = 0;
          unsigned int filterCenterBufCol = 0 ;
//          unsigned int filterCenterBufColBound = 0 ;
          double dXX = 0;
          double dYY = 0;
          double dXY = 0;
          bool isLocalMaxima = false;
          InterestPointT auxInterestPoint;
          double** currScaleBufferPtr = 0;
          unsigned char** currLaplacianSignBufferPtr = 0;
          unsigned int lastScaleIdx = 0;
          unsigned int nextScaleIdx = 0;
          unsigned int prevResponseBufferColIdx = 0;
          unsigned int nextResponseBufferColIdx = 0;
          
          // Processing each raster line from the current block
                   
          for( unsigned int rasterLine = rasterLinesStart; rasterLine < rasterLinesEndBound ;
            ++rasterLine )
          {
            // read a new raster line into the last raster buffer line
            paramsPtr->m_rastaDataAccessMutexPtr->lock();
            //std::cout << std::endl << "rasterLine"; std::cout << rasterLine << std::endl;
            //printBuffer( rasterBufferPtr, buffersLines, buffersCols );
            roolUpBuffer( rasterBufferPtr, buffersLines );
//            printBuffer( rasterBufferPtr, buffersLines, buffersCols );
            memcpy( rasterBufferPtr[ lastBuffersLineIdx ], 
              paramsPtr->m_integralRasterDataPtr->operator[]( rasterLine ),
              rasterBufferLineSizeBytes );
//            printBuffer( rasterBufferPtr, buffersLines, buffersCols );
            // read a new mask raster line into the last mask raster buffer line
            if( paramsPtr->m_maskRasterDataPtr )
            {
              roolUpBuffer( maskRasterBufferPtr, buffersLines );
              memcpy( maskRasterBufferPtr[ lastBuffersLineIdx ], 
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
                  
                  roolUpBuffer( currScaleBufferPtr, buffersLines );
                  roolUpBuffer( currLaplacianSignBufferPtr, buffersLines );
                  
                  // applying the filter kernels for the current scale
                  
                  filterWidth = getSurfFilterSize( octaveIdx, scaleIdx );
                  assert( filterWidth <= maxGausFilterWidth );
                  
                  filterLobeWidth = filterWidth / 3; 
                  filterLobeRadius = filterLobeWidth / 2;
                  filterRadius = filterWidth / 2;
                    
                  for( filterCenterBufCol = maxGausFilterRadius ; filterCenterBufCol < 
                    maxGausFilterCenterBufferColBound ; ++filterCenterBufCol )
                  {
                    dYY = getSurfDyyDerivative( rasterBufferPtr, filterCenterBufCol, 
                      maxGausFilterRadius, filterLobeWidth, filterLobeRadius);
                    dYY /= (double)( filterWidth * filterWidth );
                    
                    dXX = getSurfDxxDerivative( rasterBufferPtr, filterCenterBufCol, 
                      maxGausFilterRadius, filterLobeWidth, filterLobeRadius);
                    dXX /= (double)( filterWidth * filterWidth );
                    
                    dXY = getSurfDxyDerivative( rasterBufferPtr, filterCenterBufCol, 
                      maxGausFilterRadius, filterLobeWidth );
                    dXY /= (double)( filterWidth * filterWidth );
                      
                    currScaleBufferPtr[ lastBuffersLineIdx ][ filterCenterBufCol ] = 
                      ( dXX * dYY ) - ( 0.81 * dXY * dXY );
                    currLaplacianSignBufferPtr[ lastBuffersLineIdx ][ filterCenterBufCol ] = 
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
              
              for( unsigned int windCenterCol = maxGausFilterRadius ; windCenterCol <
                maxGausFilterCenterBufferColBound ; ++windCenterCol )
              {
                prevResponseBufferColIdx = windCenterCol - 1;
                nextResponseBufferColIdx = windCenterCol + 1;
                isLocalMaxima = false;
                auxInterestPoint.m_feature1 = (-1.0) * DBL_MAX;
                    
                for( octaveIdx = 0 ; octaveIdx < paramsPtr->m_octavesNumber ; ++octaveIdx )
                {         
                  std::vector< boost::shared_array< double* > >& 
                    currOctaveBuffersHandler = octavesBufferHandlers[ octaveIdx ];
                    
                  for( scaleIdx = 1 ; scaleIdx < ( paramsPtr->m_scalesNumber - 1 );
                    ++scaleIdx )
                  {   
                    const double& windowCenterPixelValue = currOctaveBuffersHandler[
                      scaleIdx ][ maxGausFilterRadius ][ windCenterCol ];
                    lastScaleIdx = scaleIdx - 1;
                    nextScaleIdx = scaleIdx + 1;
                      
                    if( 
                        ( windowCenterPixelValue > 0.0 )
                        // verifying the current scale (center not included)
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          scaleIdx ][ prevResponseBufferLineIdx ][ prevResponseBufferColIdx ] )
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          scaleIdx ][ prevResponseBufferLineIdx ][ windCenterCol ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          scaleIdx ][ prevResponseBufferLineIdx ][ nextResponseBufferColIdx ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          scaleIdx ][ maxGausFilterRadius ][ prevResponseBufferColIdx ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          scaleIdx ][ maxGausFilterRadius ][ nextResponseBufferColIdx ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          scaleIdx ][ nextResponseBufferLineIdx ][ prevResponseBufferColIdx] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          scaleIdx ][ nextResponseBufferLineIdx ][ windCenterCol ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          scaleIdx ][ nextResponseBufferLineIdx ][ nextResponseBufferColIdx ] ) 
                        // verifying the top scale
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          lastScaleIdx ][ prevResponseBufferLineIdx ][ prevResponseBufferColIdx ] )
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          lastScaleIdx ][ prevResponseBufferLineIdx ][ windCenterCol ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          lastScaleIdx ][ prevResponseBufferLineIdx ][ nextResponseBufferColIdx ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          lastScaleIdx ][ maxGausFilterRadius ][ prevResponseBufferColIdx ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          lastScaleIdx ][ maxGausFilterRadius ][ windCenterCol ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          lastScaleIdx ][ maxGausFilterRadius ][ nextResponseBufferColIdx ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          lastScaleIdx ][ nextResponseBufferLineIdx ][ prevResponseBufferColIdx] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          lastScaleIdx ][ nextResponseBufferLineIdx ][ windCenterCol ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          lastScaleIdx ][ nextResponseBufferLineIdx ][ nextResponseBufferColIdx ] )                          
                        // verifying the next scale
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          nextScaleIdx ][ prevResponseBufferLineIdx ][ prevResponseBufferColIdx ] )
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          nextScaleIdx ][ prevResponseBufferLineIdx ][ windCenterCol ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          nextScaleIdx ][ prevResponseBufferLineIdx ][ nextResponseBufferColIdx ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          nextScaleIdx ][ maxGausFilterRadius ][ prevResponseBufferColIdx ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          nextScaleIdx ][ maxGausFilterRadius ][ windCenterCol ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          nextScaleIdx ][ maxGausFilterRadius ][ nextResponseBufferColIdx ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          nextScaleIdx ][ nextResponseBufferLineIdx ][ prevResponseBufferColIdx] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          nextScaleIdx ][ nextResponseBufferLineIdx ][ windCenterCol ] )                          
                        && ( windowCenterPixelValue > currOctaveBuffersHandler[
                          nextScaleIdx ][ nextResponseBufferLineIdx ][ nextResponseBufferColIdx ] )
                      )
                    {
                      isLocalMaxima = true;
                      
                      if( windowCenterPixelValue > auxInterestPoint.m_feature1 )
                      {
                        auxInterestPoint.m_feature1 = windowCenterPixelValue;
                        auxInterestPoint.m_feature2 = (double)getSurfFilterSize(
                          octaveIdx, scaleIdx );
                        auxInterestPoint.m_feature3 = (double)
                          laplacianSignBufferHandlers[ octaveIdx ][ scaleIdx ][ 
                          maxGausFilterRadius ][ windCenterCol ] ;
                      }
                    }
                  }
                }
                
                if( isLocalMaxima )
                {
                  if( maskRasterBufferPtr )
                  {
                    if( maskRasterBufferPtr[ 0 ][ windCenterCol ] == 0 )
                    {
                      isLocalMaxima = false;
                    }
                  }
                  
                  if( isLocalMaxima )
                  {
                    auxInterestPoint.m_x = windCenterCol;
                    auxInterestPoint.m_y = rasterLine - ( 2 * maxGausFilterRadius) ;
                    assert( auxInterestPoint.m_x < 
                      paramsPtr->m_integralRasterDataPtr->getColumnsNumber() );
                    assert( auxInterestPoint.m_y < 
                      paramsPtr->m_integralRasterDataPtr->getLinesNumber() );                       
                    
                    blockMaximas.insert( auxInterestPoint);
                    
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
        }
        else
        {
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
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
    }
    
    void TiePointsLocator::createTifFromMatrix( 
      const Matrix< double >& rasterData,
      const InterestPointsSetT& interestPoints,
      const std::string& tifFileName )
    {
      std::map<std::string, std::string> rInfo;
      rInfo["URI"] = tifFileName + ".tif";
      
      std::vector<te::rst::BandProperty*> bandsProperties;
      bandsProperties.push_back(new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE, "" ));
      bandsProperties[0]->m_colorInterp = te::rst::RedCInt;
      bandsProperties[0]->m_noDataValue = 0;
      bandsProperties.push_back(new te::rst::BandProperty( *bandsProperties[0] ));
      bandsProperties[1]->m_colorInterp = te::rst::GreenCInt;
      bandsProperties.push_back(new te::rst::BandProperty( *bandsProperties[0] ));
      bandsProperties[2]->m_colorInterp = te::rst::BlueCInt;

      te::rst::Grid* newgrid = new te::rst::Grid( rasterData.getColumnsNumber(),
        rasterData.getLinesNumber(), 0, -1 );

      std::auto_ptr< te::rst::Raster > outputRasterPtr(
        te::rst::RasterFactory::make( "GDAL", newgrid, bandsProperties, rInfo, 0, 0));
      TERP_TRUE_OR_THROW( outputRasterPtr.get(), "Output raster creation error");
          
      unsigned int line = 0;
      unsigned int col = 0;
      const unsigned int nLines = rasterData.getLinesNumber();
      const unsigned int nCols = rasterData.getColumnsNumber();
      double rasterDataMin = DBL_MAX;
      double rasterDataMax = (-1.0) * DBL_MAX;
      double value = 0;
      
      for( line = 0 ; line < nLines ; ++line )
        for( col = 0 ; col < nCols ; ++col )
        {
          value = rasterData[ line ][ col ];
          
          if( rasterDataMin > value )
            rasterDataMin = value;
          if( rasterDataMax < value )
            rasterDataMax = value;
        }     
        
      if( rasterDataMax == rasterDataMin )
      {
        rasterDataMin = 0.0;
        rasterDataMax = 1.0;
      }
      
      for( line = 0 ; line < nLines ; ++line )
        for( col = 0 ; col < nCols ; ++col )
        {
          value = rasterData[ line ][ col ];
          value = ( value - rasterDataMin ) / ( rasterDataMax - rasterDataMin );
          value *= 255.0;
          value = std::max( 0.0, value );
          value = std::min( 255.0, value );
          
          outputRasterPtr->setValue( col, line, value, 0 );
          outputRasterPtr->setValue( col, line, value, 1 );
          outputRasterPtr->setValue( col, line, value, 2 );
        }
        
      InterestPointsSetT::const_iterator itB = interestPoints.begin();
      InterestPointsSetT::const_iterator itE = interestPoints.end();
      
      while( itB != itE )
      {
        outputRasterPtr->setValue( itB->m_x, itB->m_y, 0, 0 );
        outputRasterPtr->setValue( itB->m_x, itB->m_y, 255, 1 );
        outputRasterPtr->setValue( itB->m_x, itB->m_y, 0, 2 );
        
        ++itB;
      }
    }
    
    bool TiePointsLocator::applyGaussianFilter( const Matrix< double >& inputData,
      Matrix< double >& outputData, const unsigned int iterationsNumber )
    {
      if( iterationsNumber == 0 ) return false;
      
      TERP_TRUE_OR_RETURN_FALSE( outputData.reset( inputData.getLinesNumber(),
        inputData.getColumnsNumber() ), "Cannot allocate image matrix" );
    
      const unsigned int nLines = inputData.getLinesNumber();
      const unsigned int nCols = inputData.getColumnsNumber();
      const unsigned int lastLineIndex = nLines - 1;
      const unsigned int lastColIndex = nCols - 1;  
      unsigned int currLine = 0;
      unsigned int currCol = 0;  
      
      // internal temp matrixes
      
      Matrix< double > tempMatrix;
      
      if( iterationsNumber > 1 )
      {
        TERP_TRUE_OR_RETURN_FALSE( tempMatrix.reset( nLines, nCols,
          Matrix< double >::AutoMemPol ),
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
      
      Matrix< double > const* inputPtr = 0;
      Matrix< double >* outputPtr = 0;
      Matrix< double > const* auxPtr = 0;
      
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
          outputPtr = (Matrix< double >*)auxPtr;
        }
        
        for( currLine = 1 ; currLine < lastLineIndex ; ++currLine ) 
        {
          for( currCol = 1 ; currCol < lastColIndex ; ++currCol ) 
          {
            outputPtr->operator()( currLine, currCol ) = 
              ( 
                inputPtr->operator()( currLine - 1, currCol ) +
                ( 4.0 * inputData( currLine, currCol ) ) +
                inputPtr->operator()( currLine + 1, currCol ) +
                inputPtr->operator()( currLine, currCol - 1 ) +
                inputPtr->operator()( currLine, currCol + 1 )        
              ) / 8.0;
          }
        }
      }
      
      return true;
    }
    
    bool TiePointsLocator::applyMeanFilter( const Matrix< double >& inputData,
      Matrix< double >& outputData, const unsigned int iterationsNumber )
    {
      if( iterationsNumber == 0 )
      {
        outputData = inputData;
      }
      else
      {
        
        TERP_TRUE_OR_RETURN_FALSE( outputData.reset( inputData.getLinesNumber(),
          inputData.getColumnsNumber() ), "Cannot allocate image matrix" );
      
        const unsigned int nLines = inputData.getLinesNumber();
        const unsigned int nCols = inputData.getColumnsNumber();
        const unsigned int lastLineIndex = nLines - 1;
        const unsigned int lastColIndex = nCols - 1;  
        unsigned int currLine = 0;
        unsigned int currCol = 0;  
        
        // internal temp matrixes
        
        Matrix< double > tempMatrix;
        
        if( iterationsNumber > 1 )
        {
          TERP_TRUE_OR_RETURN_FALSE( tempMatrix.reset( nLines, nCols,
            Matrix< double >::AutoMemPol ),
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
        
        Matrix< double > const* inputPtr = 0;
        Matrix< double >* outputPtr = 0;
        Matrix< double > const* auxPtr = 0;
        double* outputLinePtr = 0;
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
            outputPtr = (Matrix< double >*)auxPtr;
          }
          
          const Matrix< double >& internalInputMatrix = *inputPtr;
          
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
                ) / 8.0;
            }
          }
        }
      }
      
      return true;
    }    

    bool TiePointsLocator::createIntegralImage( const Matrix< double >& inputData,
      Matrix< double >& outputData )
    {
      TERP_TRUE_OR_RETURN_FALSE( outputData.reset( inputData.getLinesNumber(),
        inputData.getColumnsNumber() ), "Cannot allocate image matrix" );
    
      const unsigned int nLines = inputData.getLinesNumber();
      const unsigned int nCols = inputData.getColumnsNumber();
      
      unsigned int outCol = 0;
      double currSum = 0;
      
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

    bool TiePointsLocator::generateCorrelationFeatures( 
      const InterestPointsSetT& interestPoints,
      const unsigned int correlationWindowWidth,
      const Matrix< double >& rasterData,
      const bool normalize,
      Matrix< double >& features,
      InterestPointsSetT& validInteresPoints )
    {
      // Locating the the valid interest points
      
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
      
      // Allocating the features matrix
      
      TERP_TRUE_OR_RETURN_FALSE( features.reset( 
        validInteresPoints.size(), correlationWindowWidth * correlationWindowWidth ),
        "Cannot allocate features matrix" );      
      
      /* variables related to the current window over the hole image */
      unsigned int curr_window_x_start = 0;
      unsigned int curr_window_y_start = 0;
      unsigned int curr_window_x_center = 0;
      unsigned int curr_window_y_center = 0;
      unsigned int curr_window_x_end = 0; // this coord is also counted in
      unsigned int curr_window_y_end = 0; // this coord is also counted in
      
      /*used on the rotation calcule */

      const unsigned int wind_radius = correlationWindowWidth / 2;
      // output window radius
      const double wind_radius_double = (double)wind_radius;

//      const unsigned int img_features_matrix_cols = 
//        features.getColumnsNumber();
//      const unsigned int img_features_matrix_lines = 
//        features.getLinesNumber();        
      unsigned int curr_x = 0;
      unsigned int curr_y = 0;
      double curr_x_minus_radius = 0;
      double curr_y_minus_radius = 0;
      unsigned int curr_offset = 0;
      double int_x_dir = 0;
      double int_y_dir = 0;
      double int_norm = 0;
      double rotated_curr_x = 0;/* rotaded coord - window ref */
      double rotated_curr_y = 0;/* rotaded coord - window ref */
      
      /* the found rotation parameters */
      double rot_cos = 0;
      double rot_sin = 0;
      
      /* the coords rotated but in the hole image reference */
      unsigned int rotated_curr_x_img = 0;
      unsigned int rotated_curr_y_img = 0;
      
      /* current window mean and standart deviation */
      double curr_wind_mean = 0.0;
      double curr_wind_stddev = 0.0;
      double curr_wind_stddev_aux = 0.0;
      
      // used on intensity vector calcule
//      double imgMatrixValue1 = 0;
//      double imgMatrixValue2 = 0;
      
//      double* featuresLinePtr = 0;
      const unsigned int img_features_matrix_cols = 
        features.getColumnsNumber();
//      const unsigned int img_features_matrix_lines = 
//        features.getLinesNumber();
//      unsigned int features_matrix_col = 0;
      
      InterestPointsSetT::const_iterator viPointsIt = validInteresPoints.begin();
      const InterestPointsSetT::const_iterator viPointsItEnd = validInteresPoints.end();      
      unsigned int validInteresPointsIndex = 0 ;
      
      while( viPointsIt != viPointsItEnd )
      {
        /* Calculating the current window position */
      
        curr_window_x_center = viPointsIt->m_x;
        curr_window_y_center = viPointsIt->m_y;
        assert( curr_window_x_center >= firstValidInterestPointX );
        assert( curr_window_x_center <= lastValidInterestPointX );
        assert( curr_window_y_center >= firstValidInterestPointY );
        assert( curr_window_y_center <= lastValidInterestPointY );
        
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
        int_norm = sqrt( ( int_x_dir * int_x_dir ) + 
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
          
        for( curr_y = 0 ; curr_y < correlationWindowWidth ; ++curr_y ) 
        {
          for( curr_x = 0 ; curr_x < correlationWindowWidth ; ++curr_x ) 
          {
            /* briging the window to the coord system center */
            
            curr_x_minus_radius = ((double)curr_x) - 
              wind_radius_double;
            curr_y_minus_radius = ((double)curr_y) - 
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
            
            features( validInteresPointsIndex, ( curr_y * 
              correlationWindowWidth ) + curr_x ) = rasterData( 
              rotated_curr_y_img, rotated_curr_x_img );
          }
        }
        
        /* Normalizing the generated window by subtracting its mean
          and dividing by its standard deviation */      
        
        if( normalize )
        {
          curr_wind_mean = 0.0;
          
          for( curr_x = 0 ; curr_x < img_features_matrix_cols ; 
            ++curr_x ) 
          {
            
            curr_wind_mean += features( validInteresPointsIndex,
              curr_x );
          }
          
          curr_wind_mean /= ( (double)img_features_matrix_cols  );
          
          curr_wind_stddev = 0.0;  
          
          for( curr_x = 0 ; curr_x < img_features_matrix_cols ; 
            ++curr_x ) 
          {
            curr_wind_stddev_aux = features( validInteresPointsIndex,
              curr_x ) - curr_wind_mean;
              
            curr_wind_stddev += ( curr_wind_stddev_aux *
              curr_wind_stddev_aux );
          }      
          
          curr_wind_stddev /= ( (double)img_features_matrix_cols  );
          curr_wind_stddev = std::sqrt( curr_wind_stddev );
          
          if( curr_wind_stddev == 0.0 ) {
            for( curr_x = 0 ; curr_x < img_features_matrix_cols ; 
              ++curr_x ) {
              
              double& curr_value = features( validInteresPointsIndex,
                curr_x );
              
              curr_value -= curr_wind_mean;
            } 
          } else {
            for( curr_x = 0 ; curr_x < img_features_matrix_cols ; 
              ++curr_x ) {
              
              double& curr_value = features( validInteresPointsIndex, curr_x );
              
              curr_value -= curr_wind_mean;
              curr_value /= curr_wind_stddev;
            }
          }
        }        
        
        ++validInteresPointsIndex;
        ++viPointsIt;
      }
     
      return true;
    }
    
    bool TiePointsLocator::generateSurfFeatures( 
      const InterestPointsSetT& interestPoints,
      const Matrix< double >& integralRasterData,
      Matrix< double >& features,
      InterestPointsSetT& validInterestPoints )
    {
      validInterestPoints.clear();
      
      /* globals  */
      
      unsigned int featureWindowRasterXStart = 0;
      unsigned int featureWindowRasterYStart = 0;
      double currIPointScale = 0;
      double currIPointXIntensity = 0;
      double currIPointYIntensity = 0;
      double currIPointRotationNorm = 0;
      double currIPointRotationSin = 0;
      double currIPointRotationCos = 0;
      unsigned int currIPointFeatureElementIndex = 0;
      unsigned int featureWindowWidth = 0;
      unsigned int featureWindowRadius = 0;
      double featureWindowRadiusDouble = 0;
      unsigned int feature90DegreeRotatedWindowRadius = 0;
      unsigned int currIPointCenterXAllowedMin = 0 ;
      unsigned int currIPointCenterXAllowedMax = 0 ;
      unsigned int currIPointCenterYAllowedMin = 0 ;
      unsigned int currIPointCenterYAllowedMax = 0 ;
      unsigned int featureWindowSampleStep = 0;
      unsigned int featureSubWindowWidth = 0;
      unsigned int featureSubWindowBlockStartX = 0;
      unsigned int featureSubWindowBlockStartY = 0;
      unsigned int featureSubWindowXOffset = 0;
      unsigned int featureSubWindowYOffset = 0;
      double featureSubWindowHaarSumX = 0;
      double featureSubWindowHaarSumY = 0;
      double featureSubWindowHaarAbsSumX = 0;
      double featureSubWindowHaarAbsSumY = 0;            
      double featureElementZeroCenteredOriginalXIdx = 0;
      double featureElementZeroCenteredOriginalYIdx = 0;
      double featureElementZeroCenteredRotatedXIdx = 0;
      double featureElementZeroCenteredRotatedYIdx = 0;
      double featureElementRotatedXIdx = 0;
      double featureElementRotatedYIdx = 0;      
      unsigned int featureElementRasterRotatedXIdx = 0;
      unsigned int featureElementRasterRotatedYIdx = 0;
      double featureElementOriginalHaarXIntensity = 0;
      double featureElementOriginalHaarYIntensity = 0;
      double featureElementZeroCenteredOriginalHaarXIntensity = 0;
      double featureElementZeroCenteredOriginalHaarYIntensity = 0;
      double featureElementRotatedHaarXIntensity = 0;
      double featureElementRotatedHaarYIntensity = 0;
      double featureElementZeroCenteredRotatedHaarXIntensity = 0;
      double featureElementZeroCenteredRotatedHaarYIntensity = 0;      
      unsigned int featureElementHaarWindowRadius = 0;
      double featureElementsNormalizeFactor = 0;
      double* currentFeaturePtr = 0;
      
      std::vector< boost::shared_array< double > > internalFeaturesData;
      
      // iterating over each input innterest point
      
      InterestPointsSetT::const_iterator iPointsIt = interestPoints.begin();
      const InterestPointsSetT::const_iterator iPointsItEnd = interestPoints.end();
      while( iPointsIt != iPointsItEnd )
      {
        // Calculating the current interest point variables
      
        const unsigned int& currIPointCenterX = iPointsIt->m_x;
        const unsigned int& currIPointCenterY = iPointsIt->m_y;
        currIPointScale = 1.2 * iPointsIt->m_feature2 / 9.0;
        featureWindowWidth = (unsigned int)( 20.0 * currIPointScale );
        featureWindowWidth += ( ( featureWindowWidth % 4 ) ?
          ( 4 - ( featureWindowWidth % 4 ) ) : 0 );
        featureWindowWidth += ( ( featureWindowWidth % 2 ) ? 0 : 1 );
        feature90DegreeRotatedWindowRadius = (unsigned int)
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
        featureElementHaarWindowRadius = ((unsigned int)( 2.0 * currIPointScale )) / 2;
        currIPointCenterXAllowedMin = featureElementHaarWindowRadius + 
          feature90DegreeRotatedWindowRadius + 1;
        currIPointCenterXAllowedMax = integralRasterData.getColumnsNumber() - 
          currIPointCenterXAllowedMin - 1;
        currIPointCenterYAllowedMin = currIPointCenterXAllowedMin;
        currIPointCenterYAllowedMax = integralRasterData.getLinesNumber() - 
          currIPointCenterXAllowedMin - 1;
          
//         std::cout << std:: endl << "currIPointCenterX =";
//         std::cout << currIPointCenterX;
//         std::cout << std:: endl << "currIPointCenterY =";
//         std::cout << currIPointCenterY;
//         std::cout << std:: endl << "currIPointCenterXAllowedMin =";
//         std::cout << currIPointCenterXAllowedMin;
//         std::cout << std:: endl << "currIPointCenterXAllowedMax =";
//         std::cout << currIPointCenterXAllowedMax;
//         std::cout << std:: endl << "currIPointCenterYAllowedMin =";
//         std::cout << currIPointCenterYAllowedMin;
//         std::cout << std:: endl << "currIPointCenterYAllowedMax =";
//         std::cout << currIPointCenterYAllowedMax;
        
        
        if( ( currIPointCenterX > currIPointCenterXAllowedMin ) &&
          ( currIPointCenterX < currIPointCenterXAllowedMax ) &&
          ( currIPointCenterY > currIPointCenterYAllowedMin ) &&
          ( currIPointCenterY < currIPointCenterYAllowedMax ) )
        {
          validInterestPoints.insert( *iPointsIt );
          
          // intenal vars
          
          featureWindowRadius = featureWindowWidth / 2;
          featureWindowRadiusDouble = (double)featureWindowRadius;
          featureSubWindowWidth = featureWindowWidth / 4;
          featureWindowSampleStep = MAX( 1u, featureWindowWidth / 9 );
          featureWindowRasterXStart = currIPointCenterX - featureWindowRadius;
          featureWindowRasterYStart = currIPointCenterY - featureWindowRadius;
          
          // Allocating the internal features vector
          
          currentFeaturePtr = new double[ 65 ];
          internalFeaturesData.push_back( boost::shared_array< double >(
            currentFeaturePtr ) );
            
          // Estimating the intensity vectors
          
          assert( ((long int)currIPointCenterX) -
            ((long int)featureWindowRadius) >= 0 );
          assert( ((long int)currIPointCenterY) -
            ((long int)featureWindowRadius) >= 0 );
          assert( currIPointCenterX + 
            featureWindowRadius < integralRasterData.getColumnsNumber() ); 
          assert( currIPointCenterY + 
            featureWindowRadius < integralRasterData.getLinesNumber() );          
          
          currIPointXIntensity = getHaarXVectorIntensity( integralRasterData, currIPointCenterX,
            currIPointCenterY, featureWindowRadius );
          currIPointYIntensity = getHaarYVectorIntensity( integralRasterData, currIPointCenterX,
            currIPointCenterY, featureWindowRadius );
          
          // Calculating the rotation parameters
          
          currIPointRotationNorm = sqrt( ( currIPointXIntensity * currIPointXIntensity ) + 
            ( currIPointYIntensity * currIPointYIntensity ) );
          
          if( currIPointRotationNorm != 0.0 ) 
          {
            currIPointRotationCos = currIPointXIntensity / currIPointRotationNorm;
            currIPointRotationSin = currIPointYIntensity / currIPointRotationNorm;
          } else {
            /* No rotation */
            currIPointRotationCos = 1.0;
            currIPointRotationSin = 0.0;
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
          
          currIPointFeatureElementIndex = 0;
          featureElementsNormalizeFactor = 0;
          
          for( featureSubWindowBlockStartY = 0 ; 
              featureSubWindowBlockStartY < featureWindowWidth ; 
              featureSubWindowBlockStartY += featureSubWindowWidth, 
                featureSubWindowBlockStartY += ( ( featureSubWindowBlockStartY == 
                featureWindowRadius ) ? 1 : 0 ) )
          {
            for( featureSubWindowBlockStartX = 0 ; 
              featureSubWindowBlockStartX < featureWindowWidth ; 
              featureSubWindowBlockStartX += featureSubWindowWidth, 
                featureSubWindowBlockStartX += ( ( featureSubWindowBlockStartX == 
                featureWindowRadius ) ? 1 : 0 ) )
            {
              featureSubWindowHaarSumX = 0;
              featureSubWindowHaarSumY = 0;
              featureSubWindowHaarAbsSumX = 0;
              featureSubWindowHaarAbsSumY = 0;
              
              for( featureSubWindowYOffset = 0 ; featureSubWindowYOffset < 
                featureSubWindowWidth ; featureSubWindowYOffset += featureWindowSampleStep ) 
              {
                featureElementZeroCenteredOriginalYIdx = ((double)featureSubWindowYOffset)
                  - featureWindowRadiusDouble;
                  
                for( featureSubWindowXOffset = 0 ; featureSubWindowXOffset < 
                  featureSubWindowWidth ; featureSubWindowXOffset += featureWindowSampleStep ) 
                {
                  featureElementZeroCenteredOriginalXIdx = ((double)featureSubWindowXOffset)
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
                    (unsigned int)ROUND( featureElementRotatedXIdx );
                  featureElementRasterRotatedYIdx = featureWindowRasterYStart +
                    (unsigned int)ROUND( featureElementRotatedYIdx );
                    
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
                    
                  // Haar intensity sum
                    
                  featureSubWindowHaarSumX += featureElementRotatedHaarXIntensity;
                  featureSubWindowHaarSumY += featureElementRotatedHaarYIntensity;
                  
                  featureSubWindowHaarAbsSumX += std::abs( featureElementRotatedHaarXIntensity );
                  featureSubWindowHaarAbsSumY += std::abs( featureElementRotatedHaarYIntensity );
                }
              }
              
              // Generating the related portion inside the output features vector
              
              assert( currIPointFeatureElementIndex < 61 );
              
              currentFeaturePtr[ currIPointFeatureElementIndex ] = 
                featureSubWindowHaarSumX;
              currentFeaturePtr[ currIPointFeatureElementIndex + 1 ] = 
                featureSubWindowHaarSumY;
              currentFeaturePtr[ currIPointFeatureElementIndex + 2 ] = 
                featureSubWindowHaarAbsSumX;
              currentFeaturePtr[ currIPointFeatureElementIndex + 3 ] = 
                featureSubWindowHaarAbsSumY;
                
              // Calculating the feature normalization factor
                
              featureElementsNormalizeFactor += ( featureSubWindowHaarSumX *
                featureSubWindowHaarSumX );
              featureElementsNormalizeFactor += ( featureSubWindowHaarSumY *
                featureSubWindowHaarSumY );
              featureElementsNormalizeFactor += ( featureSubWindowHaarAbsSumX *
                featureSubWindowHaarAbsSumX );
              featureElementsNormalizeFactor += ( featureSubWindowHaarAbsSumY *
                featureSubWindowHaarAbsSumY );
              
              currIPointFeatureElementIndex += 4;
            }
          }
          
          // turning the descriptor into a unit vector.(Invariance to contrast)
          
          featureElementsNormalizeFactor = std::sqrt( featureElementsNormalizeFactor );
          
          if( featureElementsNormalizeFactor != 0 )
          {
            for( currIPointFeatureElementIndex = 0 ; currIPointFeatureElementIndex < 64 ;
              ++currIPointFeatureElementIndex )
            {
              currentFeaturePtr[ currIPointFeatureElementIndex ] /=
                featureElementsNormalizeFactor;
              TERP_DEBUG_TRUE_OR_THROW( ( currentFeaturePtr[ currIPointFeatureElementIndex ] <= 1.0 ),
                currentFeaturePtr[ currIPointFeatureElementIndex ] );
              TERP_DEBUG_TRUE_OR_THROW( ( currentFeaturePtr[ currIPointFeatureElementIndex ] >= -1.0 ),
                currentFeaturePtr[ currIPointFeatureElementIndex ] );
            }
          }
          
          // Adding an attribute based on the sign of the Laplacian to 
          // distinguishes bright blobs 
          // on dark backgrounds from the reverse situation.
          
          currentFeaturePtr[ 64 ] = (double)( iPointsIt->m_feature3 * 64 );
        }
        
        ++iPointsIt;
      }
      
      // Copying the generated features to the output features matrix
      
      assert( internalFeaturesData.size() == validInterestPoints.size() );
      
      TERP_TRUE_OR_RETURN_FALSE( features.reset( validInterestPoints.size(), 65 ),
        "Cannot allocate features matrix" );            
        
      std::vector< boost::shared_array< double > >::iterator itBeg = 
        internalFeaturesData.begin();
      const std::vector< boost::shared_array< double > >::iterator itEnd = 
        internalFeaturesData.end();
      unsigned int featuresLine = 0;
        
      while( itBeg != itEnd )
      {
        memcpy( features[ featuresLine ], itBeg->get(), 65 * sizeof( double ) );
        ++featuresLine;
        ++itBeg;
      }
                 
      return true;
    }    
    
    void TiePointsLocator::features2Tiff( const Matrix< double >& features,
      const InterestPointsSetT& interestPoints,
      const std::string& fileNameBeginning )
    {
      const unsigned int tifLinesNumber = (unsigned int)std::sqrt( (double)
        features.getColumnsNumber() );
      const unsigned int featuresColsNumber = features.getColumnsNumber();
      
      double const* featureLinePtr = 0;
      
      InterestPointsSetT::const_iterator iIt = interestPoints.begin();

      for( unsigned int featuresIdx = 0 ; featuresIdx < features.getLinesNumber() ;
        ++featuresIdx )
      {
        featureLinePtr = features[ featuresIdx ];
        
        std::vector<te::rst::BandProperty*> bandsProperties;
        bandsProperties.push_back(new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE, "" ));
        bandsProperties[0]->m_colorInterp = te::rst::RedCInt;
        bandsProperties[0]->m_noDataValue = 0;
        
        std::map<std::string, std::string> rInfo;
        rInfo["URI"] = fileNameBeginning + "_" + boost::lexical_cast< std::string >( iIt->m_x ) 
          + "_" + boost::lexical_cast< std::string >( iIt->m_y ) + ".tif";        
          
        te::rst::Grid* newgrid = new te::rst::Grid( tifLinesNumber,
          tifLinesNumber, 0, -1 );          
        
        std::auto_ptr< te::rst::Raster > outputRasterPtr(
          te::rst::RasterFactory::make( "GDAL", newgrid, bandsProperties, rInfo, 0, 0));
        TERP_TRUE_OR_THROW( outputRasterPtr.get(), "Output raster creation error");
          
        unsigned int line = 0;
        unsigned int col = 0;
        double value = 0;
        double min = 0;
        double max = 0;
        double gain = 1.0;
        
        for( col = 0 ; col < featuresColsNumber ; ++col )
        {
          if( min > featureLinePtr[ col ] ) min = featureLinePtr[ col ];
          if( max < featureLinePtr[ col ] ) max = featureLinePtr[ col ];
        }
        
        gain = 255.0 / ( max - min );
        
        for( line = 0 ; line < tifLinesNumber ; ++line )
          for( col = 0 ; col < tifLinesNumber ; ++col )
          {
            value = featureLinePtr[ ( line * tifLinesNumber ) + col ];
            value *= gain;
            value -= min;
            value = MIN( 255.0, value );
            value = MAX( 0.0, value );
            
            outputRasterPtr->setValue( col, line, value, 0 );
          }
          
        ++iIt;
      }
    }
    
    bool TiePointsLocator::executeMatchingByCorrelation( 
      const Matrix< double >& featuresSet1,
      const Matrix< double >& featuresSet2,
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
      
      te::sam::rtree::Index< unsigned int > interestPointsSet2RTree;

      InterestPointsSetT::const_iterator it2 = interestPointsSet2.begin();
      boost::scoped_array< InterestPointT > internalInterestPointsSet2( 
        new InterestPointT[ interestPointsSet2Size ] );
      for( unsigned int idx2 = 0 ; idx2 < interestPointsSet2Size ; ++idx2 )
      {
        internalInterestPointsSet2[ idx2 ] = *it2;
        
        if( maxPt1ToPt2Distance )
          interestPointsSet2RTree.insert( te::gm::Envelope( it2->m_x, it2->m_y,
            it2->m_x, it2->m_y ), idx2 );
        
        ++it2;
      }        
      
      // Creating the correlation matrix
      
      Matrix< double > corrMatrix;
      TERP_TRUE_OR_RETURN_FALSE( corrMatrix.reset( interestPointsSet1Size,
       interestPointsSet2Size, Matrix< double >::RAMMemPol ),
        "Error crearting the correlation matrix" );
        
      unsigned int col = 0;
      unsigned int line = 0;
      double* linePtr = 0;
      
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
      params.m_interestPointsSet2RTreePtr = &interestPointsSet2RTree;
      
      if( enableMultiThread )
      {
        TERP_TRUE_OR_RETURN_FALSE( featuresSet1.getMemPolicy() ==
          Matrix< double >::RAMMemPol, "Invalid memory policy" )
        TERP_TRUE_OR_RETURN_FALSE( featuresSet2.getMemPolicy() ==
          Matrix< double >::RAMMemPol, "Invalid memory policy" )    
          
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
      
      std::vector< double > eachLineMaxABSValues( interestPointsSet1Size,
        DBL_MAX * (-1.0) );
      std::vector< unsigned int > eachLineMaxABSIndexes( interestPointsSet1Size,
        interestPointsSet2Size + 1 );
      std::vector< double > eachColMaxABSValues( interestPointsSet2Size,
        DBL_MAX * (-1.0) );
      std::vector< unsigned int > eachColMaxABSIndexes( interestPointsSet2Size,
        interestPointsSet1Size + 1 );
      double maxCorrelationABSValue = DBL_MAX * (-1.0);
      double minCorrelationABSValue = DBL_MAX;
      double absValue = 0;
        
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        linePtr = corrMatrix[ line ];
        
        for( col = 0 ; col < interestPointsSet2Size ; ++col )
        {
          absValue = std::abs( linePtr[ col ] );
          
          if( absValue != 0.0 )
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
          
          if( absValue > maxCorrelationABSValue ) maxCorrelationABSValue = absValue;
          if( absValue < minCorrelationABSValue ) minCorrelationABSValue = absValue;
        }
      }
      
      // Finding tiepoints
      
      const double correlationABSValueRange = ( maxCorrelationABSValue != 
        minCorrelationABSValue ) ? ( maxCorrelationABSValue -
        minCorrelationABSValue ) : 1.0;
      MatchedInterestPointsT auxMatchedPoints;
      double correlationAbs = 0;
        
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        col = eachLineMaxABSIndexes[ line ];
        
        if( eachColMaxABSIndexes[ col ] == line )
        {
          correlationAbs = std::abs( corrMatrix( line, col ) );
          
          if( correlationAbs >= minAllowedAbsCorrelation )
          {
            auxMatchedPoints.m_point1 = internalInterestPointsSet1[ line ];
            auxMatchedPoints.m_point2 = internalInterestPointsSet2[ col ];
            auxMatchedPoints.m_feature = 
              ( correlationAbs - minCorrelationABSValue ) / correlationABSValueRange;
            
            matchedPoints.insert( auxMatchedPoints );
          }
        }
      }
        
      return true;
    }
    
    void TiePointsLocator::executeMatchingByCorrelationThreadEntry(
      ExecuteMatchingByCorrelationThreadEntryParams* paramsPtr)
    {
      assert( paramsPtr->m_featuresSet1Ptr->getMemPolicy() == 
        Matrix< double >::RAMMemPol );
      assert( paramsPtr->m_featuresSet2Ptr->getMemPolicy() == 
        Matrix< double >::RAMMemPol );
      assert( paramsPtr->m_corrMatrixPtr->getMemPolicy() == 
        Matrix< double >::RAMMemPol );       
        
      unsigned int feat2Idx = 0;
      double const* feat1Ptr = 0;
      double const* feat2Ptr = 0;
      double* corrMatrixLinePtr = 0;
      unsigned int featCol = 0;
      double sumAA = 0;
      double sumBB = 0;
      double cc_norm = 0;
      double ccorrelation = 0;
      te::gm::Envelope auxEnvelope;
      
      // finding the number of features
      
      paramsPtr->m_syncMutexPtr->lock();
      
      const unsigned int featureSize = paramsPtr->m_featuresSet1Ptr->getColumnsNumber();
      const unsigned int featuresSet1Size = 
        paramsPtr->m_featuresSet1Ptr->getLinesNumber();
      const unsigned int featuresSet2Size = 
        paramsPtr->m_featuresSet2Ptr->getLinesNumber();
      
      paramsPtr->m_syncMutexPtr->unlock();
      
      // initializing the features 2 indexes vector
      
      std::vector< unsigned int > selectedFeaturesSet2Indexes;
      unsigned int selectedFeaturesSet2IndexesSize = 0;
      
      if( paramsPtr->m_maxPt1ToPt2Distance == 0 )
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
            paramsPtr->m_interestPointsSet2RTreePtr->search( auxEnvelope,
              selectedFeaturesSet2Indexes );
              
            selectedFeaturesSet2IndexesSize = selectedFeaturesSet2Indexes.size();
          }          
          
          paramsPtr->m_syncMutexPtr->unlock();
          
          corrMatrixLinePtr = paramsPtr->m_corrMatrixPtr->operator[]( feat1Idx );
          
          feat1Ptr = paramsPtr->m_featuresSet1Ptr->operator[]( feat1Idx );
          
          for( unsigned int selectedFSIIdx = 0 ; selectedFSIIdx < 
            selectedFeaturesSet2IndexesSize ; ++selectedFSIIdx )
          {
            feat2Idx = selectedFeaturesSet2Indexes[ selectedFSIIdx ];
            
            feat2Ptr = paramsPtr->m_featuresSet2Ptr->operator[]( feat2Idx );
            
            sumAA = 0;
            sumBB = 0;   
            for( featCol = 0 ; featCol < featureSize ; ++featCol )
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
              ccorrelation = 0;
              for( featCol = 0 ; featCol < featureSize ; ++featCol )
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
    
    bool TiePointsLocator::executeMatchingByEuclideanDist( 
      const Matrix< double >& featuresSet1,
      const Matrix< double >& featuresSet2,
      const InterestPointsSetT& interestPointsSet1,
      const InterestPointsSetT& interestPointsSet2,
      const unsigned int maxPt1ToPt2PixelDistance,
      const double maxEuclideanDist,
      const unsigned int enableMultiThread,
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
      
      te::sam::rtree::Index< unsigned int > interestPointsSet2RTree;

      InterestPointsSetT::const_iterator it2 = interestPointsSet2.begin();
      boost::scoped_array< InterestPointT > internalInterestPointsSet2( 
        new InterestPointT[ interestPointsSet2Size ] );
      for( unsigned int idx2 = 0 ; idx2 < interestPointsSet2Size ; ++idx2 )
      {
        internalInterestPointsSet2[ idx2 ] = *it2;
        
        if( maxPt1ToPt2PixelDistance )
          interestPointsSet2RTree.insert( te::gm::Envelope( it2->m_x, it2->m_y,
            it2->m_x, it2->m_y ), idx2 );
        
        ++it2;
      }        
      
      // Creating the distances matrix
      
      Matrix< double > distMatrix;
      TERP_TRUE_OR_RETURN_FALSE( distMatrix.reset( interestPointsSet1Size,
       interestPointsSet2Size, Matrix< double >::RAMMemPol ),
        "Error crearting the correlation matrix" );
        
      unsigned int col = 0;
      unsigned int line = 0;
      double* linePtr = 0;
      
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        linePtr = distMatrix[ line ];
        
        for( col = 0 ; col < interestPointsSet2Size ; ++col )
        {
          linePtr[ col ] = DBL_MAX;
        }
      }
      
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
      params.m_maxPt1ToPt2PixelDistance = maxPt1ToPt2PixelDistance;
      params.m_maxEuclideanDist = maxEuclideanDist;
      params.m_interestPointsSet2RTreePtr = &interestPointsSet2RTree;
      
      if( enableMultiThread )
      {
        TERP_TRUE_OR_RETURN_FALSE( featuresSet1.getMemPolicy() ==
          Matrix< double >::RAMMemPol, "Invalid memory policy" )
        TERP_TRUE_OR_RETURN_FALSE( featuresSet2.getMemPolicy() ==
          Matrix< double >::RAMMemPol, "Invalid memory policy" )    
          
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
      
      std::vector< double > eachLineMinValues( interestPointsSet1Size,
        DBL_MAX );
      std::vector< unsigned int > eachLineMinIndexes( interestPointsSet1Size,
        interestPointsSet2Size + 1 );
      std::vector< double > eachColMinValues( interestPointsSet2Size,
        DBL_MAX );
      std::vector< unsigned int > eachColMinIndexes( interestPointsSet2Size,
        interestPointsSet1Size + 1 );
      double maxDistValue = DBL_MAX * (-1.0);
      double minDistValue = DBL_MAX;
        
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        linePtr = distMatrix[ line ];
        
        for( col = 0 ; col < interestPointsSet2Size ; ++col )
        {
          const double& value = linePtr[ col ];
          
          if( value != DBL_MAX )
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
          
            if( value > maxDistValue ) maxDistValue = value;
            if( value < minDistValue ) minDistValue = value;
          }
        }
      }
      
      // Finding tiepoints
      
      const double distValueRange = ( ( minDistValue != DBL_MAX ) &&
        ( maxDistValue != minDistValue ) ) ? ( maxDistValue -
        minDistValue ) : 1.0;
      MatchedInterestPointsT auxMatchedPoints;
        
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        col =  eachLineMinIndexes[ line ];
        
        if( eachColMinIndexes[ col ] == line )
        {
          const double& distValue = distMatrix( line, col );
          
          if( ( distValue != DBL_MAX ) && ( distValue <= maxEuclideanDist ) )
          {
            auxMatchedPoints.m_point1 = internalInterestPointsSet1[ line ];
            auxMatchedPoints.m_point2 = internalInterestPointsSet2[ col ],
            auxMatchedPoints.m_feature = ( maxDistValue - distValue )  / 
              distValueRange;
            auxMatchedPoints.m_feature = std::max( 0.0000001, 
              auxMatchedPoints.m_feature );
            
            matchedPoints.insert( auxMatchedPoints );
          }
        }
      }
        
      return true;
    }
    
    void TiePointsLocator::executeMatchingByEuclideanDistThreadEntry(
      ExecuteMatchingByEuclideanDistThreadEntryParams* paramsPtr)
    {
      assert( paramsPtr->m_featuresSet1Ptr->getMemPolicy() == 
        Matrix< double >::RAMMemPol );
      assert( paramsPtr->m_featuresSet2Ptr->getMemPolicy() == 
        Matrix< double >::RAMMemPol );
      assert( paramsPtr->m_distMatrixPtr->getMemPolicy() == 
        Matrix< double >::RAMMemPol );
      assert( paramsPtr->m_featuresSet1Ptr->getColumnsNumber() ==
        paramsPtr->m_featuresSet2Ptr->getColumnsNumber() );
        
      unsigned int feat2Idx = 0;
      double const* feat1Ptr = 0;
      double const* feat2Ptr = 0;
      double* corrMatrixLinePtr = 0;
      unsigned int featCol = 0;
      te::gm::Envelope auxEnvelope;
      double diff = 0;
      double euclideanDist = 0;
      
      // finding the number of features
      
      paramsPtr->m_syncMutexPtr->lock();
      
      const unsigned int featureSize = paramsPtr->m_featuresSet1Ptr->getColumnsNumber();
      const unsigned int featuresSet1Size = 
        paramsPtr->m_featuresSet1Ptr->getLinesNumber();
      const unsigned int featuresSet2Size = 
        paramsPtr->m_featuresSet2Ptr->getLinesNumber();
      
      paramsPtr->m_syncMutexPtr->unlock();
      
      // initializing the features 2 indexes vector
      
      std::vector< unsigned int > selectedFeaturesSet2Indexes;
      unsigned int selectedFeaturesSet2IndexesSize = 0;
      
      if( paramsPtr->m_maxPt1ToPt2PixelDistance == 0 )
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
          
          if( paramsPtr->m_maxPt1ToPt2PixelDistance )
          {
            auxEnvelope.m_llx = auxEnvelope.m_urx = 
              paramsPtr->m_interestPointsSet1Ptr[ feat1Idx ].m_x;
            auxEnvelope.m_llx -= (double)paramsPtr->m_maxPt1ToPt2PixelDistance;
            auxEnvelope.m_urx += (double)paramsPtr->m_maxPt1ToPt2PixelDistance;
            auxEnvelope.m_lly = auxEnvelope.m_ury = 
              paramsPtr->m_interestPointsSet1Ptr[ feat1Idx ].m_y;
            auxEnvelope.m_lly -= (double)paramsPtr->m_maxPt1ToPt2PixelDistance;;
            auxEnvelope.m_ury += (double)paramsPtr->m_maxPt1ToPt2PixelDistance;;
            
            selectedFeaturesSet2Indexes.clear();
            paramsPtr->m_interestPointsSet2RTreePtr->search( auxEnvelope,
              selectedFeaturesSet2Indexes );
              
            selectedFeaturesSet2IndexesSize = selectedFeaturesSet2Indexes.size();
          }          
          
          paramsPtr->m_syncMutexPtr->unlock();
          
          corrMatrixLinePtr = paramsPtr->m_distMatrixPtr->operator[]( feat1Idx );
          
          feat1Ptr = paramsPtr->m_featuresSet1Ptr->operator[]( feat1Idx );
          
          for( unsigned int selectedFSIIdx = 0 ; selectedFSIIdx < 
            selectedFeaturesSet2IndexesSize ; ++selectedFSIIdx )
          {
            feat2Idx = selectedFeaturesSet2Indexes[ selectedFSIIdx ];
            
            feat2Ptr = paramsPtr->m_featuresSet2Ptr->operator[]( feat2Idx );
            
            euclideanDist = 0.0;

            for( featCol = 0 ; featCol < featureSize ; ++featCol )
            {
              diff = feat1Ptr[ featCol ] - feat2Ptr[ featCol ];
              euclideanDist += ( diff * diff );              
            }
            
            euclideanDist = std::sqrt( euclideanDist );
                
            corrMatrixLinePtr[ feat2Idx ] = euclideanDist;            
          }
        }
        else
        {
          paramsPtr->m_syncMutexPtr->unlock();
        }
      }
    }
    
    void TiePointsLocator::printBuffer( double** buffer, const unsigned int nLines,
      const unsigned int nCols )
    {
      std::cout << std::endl;
      
      for( unsigned int line = 0 ; line < nLines ; ++line )
      {
        std::cout << std::endl << "[";
        
        for( unsigned int col = 0 ; col < nCols ; ++col )
        {
          std::cout << " " << buffer[ line ][ col ];
        }
        
        std::cout << "]";
      }
      
      std::cout << std::endl;
    }

  } // end namespace rp
}   // end namespace te

