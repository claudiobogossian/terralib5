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
#include "../common/progress/TaskProgress.h"
#include "../common/PlatformUtils.h"
#include "../common/StringUtils.h"
#include "../raster/Band.h"
#include "../raster/Grid.h"
#include "../raster/BandProperty.h"
#include "../raster/RasterFactory.h"
#include "../datatype/Enums.h"
#include "../geometry/GTFilter.h"

#include <boost/scoped_array.hpp>

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
      m_geomTransfMaxError = 1;
      m_correlationWindowWidth = 11;
      m_moravecWindowWidth = 5;
      m_maxR1ToR2Offset = 0;
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
    }

    const TiePointsLocator::OutputParameters& TiePointsLocator::OutputParameters::operator=(
      const TiePointsLocator::OutputParameters& params )
    {
      reset();
      
      m_tiePoints = params.m_tiePoints;

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
      TiePointsLocator::OutputParameters* outParamsPtr = dynamic_cast<
        TiePointsLocator::OutputParameters* >( &outputParams );
      assert( outParamsPtr );
        
      if( ! m_isInitialized ) return false;
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        progressPtr->setTotalSteps( 6 );
        progressPtr->setMessage( "Locating tie points" );
      }
      
      /* Calculating the rescale factors 
        factor = rescaled_orignal_image / original_image  */
      
      double raster1XRescFact = 1.0;
      double raster1YRescFact = 1.0;
      double raster2XRescFact = 1.0;
      double raster2YRescFact = 1.0;
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
        } 
        else if( meanPixelRelation < 1.0 ) 
        {
          /* The image 2 has poor resolution - bigger pixel resolution values
            and image 1 needs to be rescaled down */
          
          raster1XRescFact = m_inputParameters.m_pixelSizeXRelation;
          raster1YRescFact = m_inputParameters.m_pixelSizeYRelation;
        }
      }        
      
      // Loading image data
      
      std::vector< boost::shared_ptr< Matrix< double > > > raster1Data;
      Matrix< unsigned char > maskRaster1Data;
      std::vector< boost::shared_ptr< Matrix< double > > > raster2Data;
      Matrix< unsigned char > maskRaster2Data;
      
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
        raster1Data,
        maskRaster1Data ),
        "Error loading raster data" );
        
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
        raster2Data,
        maskRaster2Data ),
        "Error loading raster data" );
        
//      createTifFromMatrix( *(raster1Data[ 0 ]), InterestPointsContainerT(), "loadedRaster1");
//      createTifFromMatrix( *(raster2Data[ 0 ]), InterestPointsContainerT(), "loadedRaster2");          
        
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }
      
      /* Calculating the maximum interest points number and the Moravec window
        width for each image trying to keep the same density for both images 
        - this is usefull for the case where the area covered by raster 1
        is different from the area covered by raster 1*/
      
      unsigned int raster1MaxInterestPoints = m_inputParameters.m_maxTiePoints;
      unsigned int raster2MaxInterestPoints = m_inputParameters.m_maxTiePoints;
      {
        double rescRaster1Area = (double)( raster1Data[ 0 ]->getColumnsNumber() *
          raster1Data[ 0 ]->getLinesNumber() );
        double rescRaster2Area = (double)( raster2Data[ 0 ]->getColumnsNumber() *
          raster2Data[ 0 ]->getLinesNumber() );
          
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
      
      // applying the gaussian filter
      
      switch( m_inputParameters.m_interesPointsLocationStrategy )
      {
        case InputParameters::MoravecStrategyT :
        {
          boost::shared_ptr< Matrix< double > > tempMatrix( 
            new Matrix< double > );
          TERP_TRUE_OR_RETURN_FALSE( tempMatrix->reset( 
            0, 0,
            Matrix< double >::AutoMemPol, 
            raster1Data[ 0 ]->getMaxTmpFileSize(),
            raster1Data[ 0 ]->getMaxMemPercentUsage() ),
            "Cannot allocate image matrix" );
          
          TERP_TRUE_OR_RETURN_FALSE( applyGaussianFilter( 
            *(raster1Data[ 0 ]), 
            *tempMatrix, 1 ),
            "Gaussian filter error" );

          raster1Data[ 0 ] = tempMatrix;
          
          tempMatrix.reset( new Matrix< double > );
          TERP_TRUE_OR_RETURN_FALSE( tempMatrix->reset( 
            0, 0,
            Matrix< double >::AutoMemPol, 
            raster2Data[ 0 ]->getMaxTmpFileSize(),
            raster2Data[ 0 ]->getMaxMemPercentUsage() ),
            "Cannot allocate image matrix" );          
          
          TERP_TRUE_OR_RETURN_FALSE( applyGaussianFilter( 
            *(raster2Data[ 0 ]), 
            *tempMatrix, 1 ),
            "Gaussian filter error" );

          raster2Data[ 0 ] = tempMatrix;          
          
//          createTifFromMatrix( *(raster1Data[ 0 ]), InterestPointsContainerT(), "raster1Gaussian");
//          createTifFromMatrix( *(raster2Data[ 0 ]), InterestPointsContainerT(), "raster2Gaussian");  
          
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid strategy" );
          break;
        }
      };
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }      
      
      // locating interest points
      
      InterestPointsContainerT raster1InterestPoints;
      InterestPointsContainerT raster2InterestPoints;      
      
      switch( m_inputParameters.m_interesPointsLocationStrategy )
      {
        case InputParameters::MoravecStrategyT :
        {
          TERP_TRUE_OR_RETURN_FALSE( locateMoravecInterestPoints( 
            *(raster1Data[ 0 ]), 
            maskRaster1Data.getLinesNumber() ? (&maskRaster1Data) : 0, 
            m_inputParameters.m_moravecWindowWidth,
            raster1MaxInterestPoints,
            m_inputParameters.m_enableMultiThread,
            raster1InterestPoints ),
            "Error locating raster 1 interest points" );
          TERP_TRUE_OR_RETURN_FALSE( locateMoravecInterestPoints( 
            *(raster2Data[ 0 ]), 
            maskRaster2Data.getLinesNumber() ? (&maskRaster2Data) : 0, 
            m_inputParameters.m_moravecWindowWidth,
            raster2MaxInterestPoints,
            m_inputParameters.m_enableMultiThread,
            raster2InterestPoints ),
            "Error locating raster 2 interest points" );             
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid strategy" );
          break;
        }
      };
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }      
      
//      createTifFromMatrix( *(raster1Data[ 0 ]), raster1InterestPoints, "raster1InterestPoints");
//      createTifFromMatrix( *(raster2Data[ 0 ]), raster2InterestPoints, "raster2InterestPoints");
        
      // Generting features (one feature per line)
      
      Matrix< double > raster1Features;
      raster1Features.reset( Matrix< double >::RAMMemPol );
      Matrix< double > raster2Features;
      raster2Features.reset( Matrix< double >::RAMMemPol );
      
      switch( m_inputParameters.m_interesPointsLocationStrategy )
      {
        case InputParameters::MoravecStrategyT :
        {
          InterestPointsContainerT auxInterestPoints;
          
          TERP_TRUE_OR_RETURN_FALSE( generateCorrelationFeatures( 
            raster1InterestPoints,
            m_inputParameters.m_correlationWindowWidth,
            *(raster1Data[ 0 ]),
            true,
            raster1Features,
            auxInterestPoints ),
            "Error generating raster 1 features" );
          raster1InterestPoints = auxInterestPoints;
          
          auxInterestPoints.clear();
          TERP_TRUE_OR_RETURN_FALSE( generateCorrelationFeatures( 
            raster2InterestPoints,
            m_inputParameters.m_correlationWindowWidth,
            *(raster2Data[ 0 ]),
            true,
            raster2Features,
            auxInterestPoints ),
            "Error generating raster 2 features" );
            
          raster2InterestPoints = auxInterestPoints;
          
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid strategy" );
          break;
        }
      };
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }      
      
//      features2Tiff( raster1Features, raster1InterestPoints, "raster1features" );
//      features2Tiff( raster2Features, raster2InterestPoints, "raster2features" );

      // Clean unused data
      
      raster1Data.clear();
      maskRaster1Data.clear();
      raster2Data.clear();
      maskRaster2Data.clear();

      // Matching features
      
      MatchedInterestPointsContainerT matchedPoints;
      
      switch( m_inputParameters.m_interesPointsLocationStrategy )
      {
        case InputParameters::MoravecStrategyT :
        {
          TERP_TRUE_OR_RETURN_FALSE( matchCorrelationEuclidean( 
            raster1Features,
            raster2Features,
            raster1InterestPoints,
            raster2InterestPoints,
            m_inputParameters.m_maxR1ToR2Offset,
            m_inputParameters.m_enableMultiThread,
            matchedPoints ),
            "Error matching features" );
          break;
        }
        default :
        {
          TERP_LOG_AND_THROW( "Invalid strategy" );
          break;
        }
      };
      
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
      
      // Bring matched points to original images reference
      
      te::gm::GTParameters transfParams;
      std::vector< double > tiePointsWeights;
        
      {
        te::gm::GTParameters::TiePoint auxTP;
        MatchedInterestPointsContainerT::const_iterator itB = matchedPoints.begin();
        const MatchedInterestPointsContainerT::const_iterator itE = matchedPoints.end();
        
        while( itB != itE )
        {
          auxTP.first.x = ( itB->m_x1 / raster1XRescFact ) + 
            (double)m_inputParameters.m_raster1TargetAreaColStart;
          auxTP.first.y = ( itB->m_y1 / raster1YRescFact ) + 
            (double)m_inputParameters.m_raster1TargetAreaLineStart;          
          auxTP.second.x = ( itB->m_x2 / raster2XRescFact ) + 
            (double)m_inputParameters.m_raster2TargetAreaColStart;
          auxTP.second.y = ( itB->m_y2 / raster2YRescFact ) + 
            (double)m_inputParameters.m_raster2TargetAreaLineStart;   
            
          tiePointsWeights.push_back( itB->m_featureValue );
            
          transfParams.m_tiePoints.push_back( auxTP );
          
          ++itB;
        }
      }      
      
      // Removing outliers
        
       std::auto_ptr< te::gm::GeometricTransformation > transfPtr;
       
       TERP_TRUE_OR_RETURN_FALSE( te::gm::GTFilter::applyRansac( 
        m_inputParameters.m_geomTransfName, 
        transfParams,
        m_inputParameters.m_geomTransfMaxError,
        m_inputParameters.m_geomTransfMaxError,
        m_inputParameters.m_geomTransfMaxError,
        m_inputParameters.m_geomTransfMaxError,
        transfPtr,
        tiePointsWeights ), "Outliers remotion error" );
       
      outParamsPtr->m_tiePoints = transfPtr->getParameters().m_tiePoints;
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }      
      
      return true;
    }

    void TiePointsLocator::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_outputParametersPtr = 0;
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

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster1Bands.size() > 0,
        "No given raster 1 band" );
      switch( m_inputParameters.m_interesPointsLocationStrategy )
      {
        case InputParameters::MoravecStrategyT :
        {
          TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster1Bands.size() == 1,
            "No given raster 1 band" );
          break;
        }
        default :
        {
          TERP_LOG_AND_RETURN_FALSE( "Invalid innterest points location strategy" );
          break;
        }
      }
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

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRaster2Bands.size() > 0,
        "No given raster 2 band" );
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
      std::vector< boost::shared_ptr< Matrix< double > > >& loadedRasterData,
      Matrix< unsigned char >& loadedMaskRasterData )
    {
      // Allocating the output matrixes
      
      const unsigned int rescaledNLines = (unsigned int)(
        ((double)rasterTargetAreaHeight) * rescaleFactorY );
      const unsigned int rescaledNCols = (unsigned int)(
        ((double)rasterTargetAreaWidth) * rescaleFactorX );
    
      {
        const unsigned int maxMemPercentUsagePerMatrix = MAX( 1, 25 / 
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
        te::rst::Band const* inRasterBand = 0;
        double* outLinePtr = 0;
        unsigned int outLine = 0;
        unsigned int outCol = 0;
        unsigned int inLine = 0;
        unsigned int inCol = 0;      
        
        for( unsigned int rasterBandsIdx = 0 ; rasterBandsIdx < rasterBands.size() ;
          ++rasterBandsIdx )
        {
          inRasterBand = rasterPtr->getBand( rasterBands[ rasterBandsIdx ] );
          assert( inRasterBand );
          
          for( outLine = 0 ; outLine < rescaledNLines ; ++outLine ) 
          {
            inLine = (unsigned int)( ( ( (double)outLine ) / 
              rescaleFactorY ) + ( (double)rasterTargetAreaLineStart ) );      
              
            outLinePtr = loadedRasterData[ rasterBandsIdx ]->operator[]( outLine  );
            
            for( outCol = 0 ; outCol < rescaledNCols ; ++outCol ) 
            {          
              inCol = (unsigned int)( ( ( (double)outCol ) / 
                  rescaleFactorX ) + ( (double)rasterTargetAreaColStart ) );        

              inRasterBand->getValue( inCol, inLine, outLinePtr[ outCol ] );
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
      InterestPointsContainerT& interestPoints )
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
          threads.add_thread( new boost::thread( moravecLocatorThreadEntry, 
            &threadParams ) );
        }
        
        threads.join_all();
      }
      else
      {
        threadParams.m_maxRasterLinesBlockMaxSize = rasterData.getLinesNumber() / 1;
        threadParams.m_maxInterestPointsPerRasterLinesBlock = maxInterestPoints;
        
        moravecLocatorThreadEntry( &threadParams );
      }
     
      return returnValue;
    }
    
    void TiePointsLocator::moravecLocatorThreadEntry(MoravecLocatorThreadParams* paramsPtr)
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
        InterestPointsContainerT blockMaximas; // the maxima points found inside the current raster block
        
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
            // roll up on buffers
            roolUpBuffer( rasterBufferPtr, bufferLines ); 
            if( paramsPtr->m_maskRasterDataPtr )
              roolUpBuffer( maskRasterBufferPtr, bufferLines );
            roolUpBuffer( maximasBufferPtr, bufferLines );
              
            // read a new raster line into the last raster buffer line
            paramsPtr->m_rastaDataAccessMutexPtr->lock();
            memcpy( rasterBufferPtr[ bufferLines - 1 ], 
              paramsPtr->m_rasterDataPtr->operator[]( rasterLine ),
              rasterBufferLineSizeBytes );
              
            // read a new mask raster line into the last mask raster buffer line
            if( paramsPtr->m_maskRasterDataPtr )
              memcpy( maskRasterBufferPtr[ bufferLines - 1 ], 
                paramsPtr->m_maskRasterDataPtr->operator[]( rasterLine ),
                maskRasterBufferLineSizeBytes );
            paramsPtr->m_rastaDataAccessMutexPtr->unlock();
            
            // calc the diretional variance for last line from the
            // diretional variances buffer
            if( rasterLine >= varianceCalcStartRasterLineStart )
            {
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
                
                maximasBufferPtr[ bufferLines - 1 ][ windowStartBufCol + 
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
                  auxInterestPoint.m_y = rasterLine;
                  auxInterestPoint.m_featureValue = windowCenterPixelValue;
                  assert( auxInterestPoint.m_x < 
                    paramsPtr->m_rasterDataPtr->getColumnsNumber() );
                  assert( auxInterestPoint.m_y < 
                    paramsPtr->m_rasterDataPtr->getLinesNumber() );  
                  
                  if( maskRasterBufferPtr )
                  {
                    if( maskRasterBufferPtr[ moravecWindowRadius ][ 
                      auxInterestPoint.m_x ] )
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
          InterestPointsContainerT::const_reverse_iterator blockMaximasIt =
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
    
    void TiePointsLocator::createTifFromMatrix( 
      const Matrix< double >& rasterData,
      const InterestPointsContainerT& interestPoints,
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
      
      for( line = 0 ; line < nLines ; ++line )
        for( col = 0 ; col < nCols ; ++col )
        {
          outputRasterPtr->setValue( col, line, rasterData[ line ][ col ], 0 );
          outputRasterPtr->setValue( col, line, rasterData[ line ][ col ], 1 );
          outputRasterPtr->setValue( col, line, rasterData[ line ][ col ], 2 );
        }
        
      InterestPointsContainerT::const_iterator itB = interestPoints.begin();
      InterestPointsContainerT::const_iterator itE = interestPoints.end();
      
      while( itB != itE )
      {
        outputRasterPtr->setValue( itB->m_x, itB->m_y, 255, 0 );
        
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
    
    bool TiePointsLocator::generateCorrelationFeatures( 
      const InterestPointsContainerT& interestPoints,
      const unsigned int correlationWindowWidth,
      const Matrix< double >& rasterData,
      const bool normalize,
      Matrix< double >& features,
      InterestPointsContainerT& validInteresPoints )
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
        InterestPointsContainerT::const_iterator iPointsIt = interestPoints.begin();
        const InterestPointsContainerT::const_iterator iPointsItEnd = interestPoints.end();        
        
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
      
      double* featuresLinePtr = 0;
      const unsigned int img_features_matrix_cols = 
        features.getColumnsNumber();
      const unsigned int img_features_matrix_lines = 
        features.getLinesNumber();
      unsigned int features_matrix_col = 0;
      
      InterestPointsContainerT::const_iterator viPointsIt = validInteresPoints.begin();
      const InterestPointsContainerT::const_iterator viPointsItEnd = validInteresPoints.end();      
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
            int_y_dir += rasterData( curr_window_y_end - curr_offset, curr_x ) -
              rasterData( curr_window_y_start + curr_offset, curr_x );
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
              ( ( rot_cos * curr_x_minus_radius ) + 
              ( -1.0 * rot_sin * curr_y_minus_radius ) );
            
            rotated_curr_y =
              ( ( rot_sin * curr_x_minus_radius ) + 
              ( rot_cos * curr_y_minus_radius ) );
              
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
    
    void TiePointsLocator::features2Tiff( const Matrix< double >& features,
      const InterestPointsContainerT& interestPoints,
      const std::string& fileNameBeginning )
    {
      const unsigned int tifLinesNumber = (unsigned int)std::sqrt( (double)
        features.getColumnsNumber() );
      const unsigned int featuresColsNumber = features.getColumnsNumber();
      
      double const* featureLinePtr = 0;
      
      InterestPointsContainerT::const_iterator iIt = interestPoints.begin();

      for( unsigned int featuresIdx = 0 ; featuresIdx < features.getLinesNumber() ;
        ++featuresIdx )
      {
        featureLinePtr = features[ featuresIdx ];
        
        std::vector<te::rst::BandProperty*> bandsProperties;
        bandsProperties.push_back(new te::rst::BandProperty( 0, te::dt::UCHAR_TYPE, "" ));
        bandsProperties[0]->m_colorInterp = te::rst::RedCInt;
        bandsProperties[0]->m_noDataValue = 0;
        
        std::map<std::string, std::string> rInfo;
        rInfo["URI"] = fileNameBeginning + "_" + te::common::Convert2String( iIt->m_x ) 
          + "_" + te::common::Convert2String( iIt->m_y ) + ".tif";        
          
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
            value = MIN( 255, value );
            value = MAX( 0, value );
            
            outputRasterPtr->setValue( col, line, value, 0 );
          }
          
        ++iIt;
      }
    }
    
    bool TiePointsLocator::matchCorrelationEuclidean( 
      const Matrix< double >& featuresSet1,
      const Matrix< double >& featuresSet2,
      const InterestPointsContainerT& interestPointsSet1,
      const InterestPointsContainerT& interestPointsSet2,
      const unsigned int maxPt1ToPt2Distance,
      const unsigned int enableMultiThread,
      MatchedInterestPointsContainerT& matchedPoints )
    {
      matchedPoints.clear();
      
      const unsigned int interestPointsSet1Size = interestPointsSet1.size();
      const unsigned int interestPointsSet2Size = interestPointsSet2.size();
      
      assert( featuresSet1.getColumnsNumber() == featuresSet2.getColumnsNumber() );
      assert( featuresSet1.getLinesNumber() == interestPointsSet1Size );
      assert( featuresSet2.getLinesNumber() == interestPointsSet2Size );
      
      // Creating internal objects
      
      double maxTiePointsFeatureValue = DBL_MAX * (-1.0);
      double minTiePointsFeatureValue = DBL_MAX;
      
      InterestPointsContainerT::const_iterator it1 = interestPointsSet1.begin();
      boost::scoped_array< InterestPointT > internalInterestPointsSet1( 
        new InterestPointT[ interestPointsSet1Size ] );
      for( unsigned int idx1 = 0 ; idx1 < interestPointsSet1Size ; ++idx1 )
      {
        internalInterestPointsSet1[ idx1 ] = *it1;
        
        if( it1->m_featureValue > maxTiePointsFeatureValue )
          maxTiePointsFeatureValue = it1->m_featureValue;
        if( it1->m_featureValue < minTiePointsFeatureValue )
          minTiePointsFeatureValue = it1->m_featureValue;        
        
        ++it1;
      }
      
      te::sam::rtree::Index< unsigned int > interestPointsSet2RTree;

      InterestPointsContainerT::const_iterator it2 = interestPointsSet2.begin();
      boost::scoped_array< InterestPointT > internalInterestPointsSet2( 
        new InterestPointT[ interestPointsSet2Size ] );
      for( unsigned int idx2 = 0 ; idx2 < interestPointsSet2Size ; ++idx2 )
      {
        internalInterestPointsSet2[ idx2 ] = *it2;

        if( it2->m_featureValue > maxTiePointsFeatureValue )
          maxTiePointsFeatureValue = it2->m_featureValue;
        if( it2->m_featureValue < minTiePointsFeatureValue )
          minTiePointsFeatureValue = it2->m_featureValue;
        
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
      unsigned int nextFeatureIdx2ToProcess = 0;
      
      CorrelationMatrixCalcThreadParams params;
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
            correlationMatrixCalcThreadEntry, &params ) );
        }
        
        threads.join_all();          
          
      }
      else
      {
        correlationMatrixCalcThreadEntry( &params );
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
      double maxCorrelationValue = DBL_MAX * (-1.0);
      double minCorrelationValue = DBL_MAX;
      double absValue = 0;
        
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        linePtr = corrMatrix[ line ];
        
        for( col = 0 ; col < interestPointsSet2Size ; ++col )
        {
          const double& value = linePtr[ col ];
          
          if( value != 0.0 )
          {
            absValue = std::abs( value );
            
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
          
          if( value > maxCorrelationValue ) maxCorrelationValue = value;
          if( value < minCorrelationValue ) minCorrelationValue = value;
        }
      }
      
      
      
      // Finding tiepoints
      
      const double tiePointsFeatureValueRange = ( maxTiePointsFeatureValue != 
        minTiePointsFeatureValue ) ? ( maxTiePointsFeatureValue -
        minTiePointsFeatureValue ) : 1.0;
      const double correlationValueRange = ( maxCorrelationValue != 
        minCorrelationValue ) ? ( maxCorrelationValue -
        minCorrelationValue ) : 1.0;
      MatchedInterestPointsT auxMatchedPoints;
        
      for( line = 0 ; line < interestPointsSet1Size ; ++line )
      {
        if( eachColMaxABSIndexes[ eachLineMaxABSIndexes[ line ] ] == line )
        {
          const InterestPointT& iPoint1 = internalInterestPointsSet1[ line ];
          const InterestPointT& iPoint2 = internalInterestPointsSet2[ eachLineMaxABSIndexes[ line ] ];
          
          auxMatchedPoints.m_x1 = iPoint1.m_x;
          auxMatchedPoints.m_y1 = iPoint1.m_y,
          auxMatchedPoints.m_x2 = iPoint2.m_x;
          auxMatchedPoints.m_y2 = iPoint2.m_y;
          auxMatchedPoints.m_featureValue = 
            ( 
              ( 
                ( iPoint1.m_featureValue - minTiePointsFeatureValue ) /
                tiePointsFeatureValueRange
              )
              +
              (
                ( corrMatrix( line, eachLineMaxABSIndexes[ line ] ) - 
                minCorrelationValue ) / correlationValueRange
              )
            ) / 2.0;
          
          matchedPoints.insert( auxMatchedPoints );
        }
      }
        
      return true;
    }
    
    void TiePointsLocator::correlationMatrixCalcThreadEntry(
      CorrelationMatrixCalcThreadParams* paramsPtr)
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

  } // end namespace rp
}   // end namespace te

