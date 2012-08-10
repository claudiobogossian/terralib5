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
#include "../raster/Band.h"

#include <boost/scoped_array.hpp>

#include <algorithm>

#include <climits>
#include <cfloat>
#include <cstdio>

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
      m_correlationWindowWidth = 21;
      m_moravecWindowWidth = 11;
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
      if( ! m_isInitialized ) return false;
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        progressPtr->setTotalSteps( 4 );
        progressPtr->setMessage( "Locating tie points" );
      }
      
      /* Calculating the rescale factors 
        factor = rescaled_orignal_image / original_image 
        and the new moravec window widths */
      
      double raster1XRescFact = 1.0;
      double raster1YRescFact = 1.0;
      double raster2XRescFact = 1.0;
      double raster2YRescFact = 1.0;
      unsigned int raster1MoravecWindowWidth = m_inputParameters.m_moravecWindowWidth;
      unsigned int raster2MoravecWindowWidth = m_inputParameters.m_moravecWindowWidth;      
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
          raster2MoravecWindowWidth = (unsigned int)MAX( 3.0, 
             ((double)m_inputParameters.m_moravecWindowWidth) / 
             meanPixelRelation );
        } 
        else if( meanPixelRelation < 1.0 ) 
        {
          /* The image 2 has poor resolution - bigger pixel resolution values
            and image 1 needs to be rescaled down */
          
          raster1XRescFact = m_inputParameters.m_pixelSizeXRelation;
          raster1YRescFact = m_inputParameters.m_pixelSizeYRelation;
          raster1MoravecWindowWidth = (unsigned int)MAX( 3.0, 
             ((double)m_inputParameters.m_moravecWindowWidth) / 
             meanPixelRelation );          
        }
      }        
      
      // Loading image data
      
      std::vector< Matrix< double > > raster1Data;
      Matrix< unsigned char > maskRaster1Data;
      std::vector< Matrix< double > > raster2Data;
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
        
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }        
        
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
        
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }
      
      /* Calculating the maximum interest points number and the Moravec window
        width for each image trying to keep the same density for both images */
      
      unsigned int raster1MaxInterestPoints = m_inputParameters.m_maxTiePoints;
      unsigned int raster2MaxInterestPoints = m_inputParameters.m_maxTiePoints;
      {
        double rescRaster1Area = (double)( raster1Data[ 0 ].getColumnsNumber() *
          raster1Data[ 0 ].getLinesNumber() );
        double rescRaster2Area = (double)( raster2Data[ 0 ].getColumnsNumber() *
          raster2Data[ 0 ].getLinesNumber() );
          
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
      
      // locating interest points
      
      InterestPointsContainerT raster1InterestPoints;
      InterestPointsContainerT raster2InterestPoints;      
      
      switch( m_inputParameters.m_interesPointsLocationStrategy )
      {
        case InputParameters::MoravecStrategyT :
        {
          TERP_TRUE_OR_RETURN_FALSE( locateMoravecInterestPoints( 
            raster1Data[ 0 ], 
            maskRaster1Data.getLinesNumber() ? (&maskRaster1Data) : 0, 
            raster1MoravecWindowWidth,
            raster1MaxInterestPoints,
            m_inputParameters.m_enableMultiThread,
            raster1InterestPoints ),
            "Error locating raster 1 interest points" );
          TERP_TRUE_OR_RETURN_FALSE( locateMoravecInterestPoints( 
            raster2Data[ 0 ], 
            maskRaster2Data.getLinesNumber() ? (&maskRaster2Data) : 0, 
            raster2MoravecWindowWidth,
            raster2MaxInterestPoints,
            m_inputParameters.m_enableMultiThread,
            raster2InterestPoints ),
            "Error locating raster 2 interest points" );             
          break;
        }
        default :
        {
          return false;
          break;
        }
      };
        
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
      std::vector< Matrix< double > >& loadedRasterData,
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
          TERP_TRUE_OR_RETURN_FALSE( loadedRasterData[ rasterBandsIdx ].reset( 
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
              
            outLinePtr = loadedRasterData[ rasterBandsIdx ][ outLine ];
            
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

      const unsigned int minRasterWidthAndHeight = moravecWindowWidth + 
        ( moravecWindowWidth / 2 );      
      
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
        
        {
          threadParams.m_maxRasterLinesBlockMaxSize = std::max( 
            moravecWindowWidth + ( moravecWindowWidth / 2 ), 
            rasterData.getLinesNumber() / procsNumber );
          threadParams.m_rasterLinesBlocksNumber = (unsigned int)std::ceil(
            ((double)rasterData.getLinesNumber()) / 
            ((double)threadParams.m_maxRasterLinesBlockMaxSize) );
          threadParams.m_maxInterestPointsPerRasterLinesBlock =
            maxInterestPoints / threadParams.m_rasterLinesBlocksNumber;
          
          boost::thread_group threads;
          
          for( unsigned int threadIdx = 0 ; threadIdx < procsNumber ;
            ++threadIdx )
          {
            threads.add_thread( new boost::thread( moravecLocatorThreadEntry, 
              &threadParams ) );
          }
          
          threads.join_all();
        }
      }
      else
      {
        threadParams.m_maxRasterLinesBlockMaxSize = rasterData.getLinesNumber();
        threadParams.m_rasterLinesBlocksNumber = 1;
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
      assert( paramsPtr->m_rasterLinesBlocksNumber );
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
      for( unsigned int maximasBufferDataHandlerLine = 0 ; maximasBufferDataHandlerLine < 
        bufferLines ; ++maximasBufferDataHandlerLine )
      {
        maximasBufferHandler[ maximasBufferDataHandlerLine ] = maximasBufferDataHandler[ 
          maximasBufferDataHandlerLine ];
      }
      
      double** maximasBufferPtr = maximasBufferHandler.get();      
      
      // Pick the next block to process
        
      for( unsigned int rasterLinesBlockIdx = 0; rasterLinesBlockIdx <
        paramsPtr->m_rasterLinesBlocksNumber ; ++rasterLinesBlockIdx )
      {
        InterestPointsContainerT blockMaximas; // the maxima points found inside the current raster block
        
        paramsPtr->m_rastaDataAccessMutexPtr->lock();
         
        if( rasterLinesBlockIdx == ( *(paramsPtr->m_nextRasterLinesBlockToProcessValuePtr ) ) )
        {
          ++( *(paramsPtr->m_nextRasterLinesBlockToProcessValuePtr ) );
           
          paramsPtr->m_rastaDataAccessMutexPtr->unlock();
          
          // fill the maximas buffer with invalid values
          
          unsigned int bufferCol = 0;
          for( unsigned int bufferLine = 0 ; bufferLine < bufferLines ; 
            ++bufferLine )
          {
            for( bufferCol = 0 ; bufferCol < bufferCols ; ++bufferCol )
            {
              maximasBufferPtr[ bufferLine ][ bufferCol ] = 0;
            }
          }

          // Processing each raster line from the current block
          
          const unsigned int rasterLinesStart = (unsigned int)std::max( 0,
            (int)(rasterLinesBlockIdx * paramsPtr->m_maxRasterLinesBlockMaxSize ) - 
            (int)moravecWindowRadius ); 
          const unsigned int rasterLinesEndBound = std::min( rasterLinesStart + 
            paramsPtr->m_maxRasterLinesBlockMaxSize + moravecWindowRadius, 
            rasterLines );
          const unsigned int varianceCalcStartRasterLineBound = rasterLinesStart + 
            moravecWindowWidth - 1;
          const unsigned int maximasLocationStartRasterLineBound = 
            varianceCalcStartRasterLineBound + moravecWindowRadius;
          unsigned int windowStartBufCol = 0;
          const unsigned int windowEndBufColsBound = bufferCols - 
            moravecWindowRadius;
          unsigned int windowStartBufOffset = 0;
          unsigned int windowStartBufXOffset = 0;
          unsigned int windowStartBufYOffset = 0;
          double horVar = 0;
          double verVar = 0;
          double diagVar = 0;
          double adiagVar = 0;
          double diffValue = 0;
          bool isLocalMaxima = false;          
                   
          for( unsigned int rasterLine = rasterLinesStart; rasterLine < rasterLinesEndBound ;
            ++rasterLine )
          {
            // roll up on buffers
            std::rotate( rasterBufferPtr, rasterBufferPtr + bufferLines - 1, 
              rasterBufferPtr + bufferLines - 2); 
            std::rotate( maximasBufferPtr, maximasBufferPtr + bufferLines - 1, 
              maximasBufferPtr + bufferLines - 2); 
            if( paramsPtr->m_maskRasterDataPtr )
              std::rotate( maskRasterBufferPtr, maskRasterBufferPtr + bufferLines - 1, 
                maskRasterBufferPtr + bufferLines - 2); 
              
            // read a new raster line into the last raster buffer line
            paramsPtr->m_rastaDataAccessMutexPtr->lock();
            memcpy( rasterBufferPtr[ bufferLines - 1 ], 
              paramsPtr->m_rasterDataPtr->operator[]( rasterLine ),
              rasterBufferLineSizeBytes );
            if( paramsPtr->m_maskRasterDataPtr )
              memcpy( maskRasterBufferPtr[ bufferLines - 1 ], 
                paramsPtr->m_maskRasterDataPtr->operator[]( rasterLine ),
                maskRasterBufferLineSizeBytes );
            paramsPtr->m_rastaDataAccessMutexPtr->unlock();
            
            // calc the diretional variance for the buffer center line
            if( rasterLine > varianceCalcStartRasterLineBound )
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
                  bufferLines ; ++windowStartBufOffset )
                {
                  diffValue = windowCenterPixelValue - rasterBufferPtr[ 
                    moravecWindowRadius ][ windowStartBufCol + 
                    windowStartBufOffset ];
                  horVar += diffValue * diffValue;
                    
                  diffValue = windowCenterPixelValue - rasterBufferPtr[ 
                    windowStartBufOffset ][ windowStartBufCol +
                    moravecWindowRadius ];
                  verVar += diffValue * diffValue;
                  
                  diffValue = windowCenterPixelValue - rasterBufferPtr[ 
                    windowStartBufOffset ][ windowStartBufCol +
                    windowStartBufOffset ];
                  diagVar += diffValue * diffValue;
                  
                  diffValue = windowCenterPixelValue - rasterBufferPtr[ 
                    bufferLines - 1 - windowStartBufOffset ][ windowStartBufCol +
                    windowStartBufOffset ];
                  adiagVar += diffValue * diffValue;                  
                }
                
                maximasBufferPtr[ bufferLines - 1 ][ windowStartBufCol +
                  moravecWindowRadius ] = std::min( horVar, std::min(
                  verVar, std::min( diagVar, adiagVar ) ) );
              }
            }
            
            // find the local maxima points inside the maximas buffer.
            if( rasterLine > maximasLocationStartRasterLineBound )
            {
              for( windowStartBufCol = 0 ; windowStartBufCol < windowEndBufColsBound ; 
                ++windowStartBufCol )
              {
                isLocalMaxima = true;
                const double& windowCenterPixelValue = maximasBufferPtr[
                  moravecWindowRadius ][ windowStartBufCol + 
                  moravecWindowRadius ];                
                
                for( windowStartBufYOffset = 0 ; windowStartBufYOffset < 
                  bufferLines ; ++windowStartBufYOffset )
                {
                  for( windowStartBufXOffset = 0 ; windowStartBufXOffset < 
                    bufferLines ; ++windowStartBufXOffset )
                  {
                    if( windowCenterPixelValue < maximasBufferPtr[
                      windowStartBufYOffset ][ windowStartBufCol + 
                      windowStartBufXOffset ] )
                    {
                      isLocalMaxima = false;
                      windowStartBufYOffset = bufferLines;
                      break;
                    }
                  }
                }
                
                if( isLocalMaxima )
                {
                  if( maskRasterBufferPtr )
                  {
                    if( maskRasterBufferPtr[ moravecWindowRadius ][ 
                      windowStartBufCol + moravecWindowRadius ] )
                    {
                      blockMaximas.insert( InterestPointT( rasterLine - 
                        moravecWindowRadius, windowStartBufCol + 
                        moravecWindowRadius, windowCenterPixelValue ) );
                    }
                  }
                  else
                  {
                    blockMaximas.insert( InterestPointT( rasterLine - 
                      moravecWindowRadius, windowStartBufCol + 
                      moravecWindowRadius, windowCenterPixelValue ) );
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
          const InterestPointsContainerT::const_reverse_iterator blockMaximasItEnd =
            blockMaximas.rend();
            
          while( pointsToAdd && ( blockMaximasIt != blockMaximasItEnd ) )
          {
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

  } // end namespace rp
}   // end namespace te

