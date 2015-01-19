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
#include "TiePointsLocatorMoravecStrategy.h"
#include "TiePointsLocatorSURFStrategy.h"
#include "Functions.h"
#include "../geometry/GTFactory.h"
#include "../geometry/GTFilter.h"
#include "../common/progress/TaskProgress.h"

namespace te
{
  namespace rp
  {

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
    
    // -----------------------------------------------------------------------

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
      
      outParamsPtr->m_tiePoints.clear();
      outParamsPtr->m_tiePointsWeights.clear();
      outParamsPtr->m_transformationPtr.reset();
      
      // creating the choosed strategy
      
      std::auto_ptr< TiePointsLocatorStrategy > stratPtr;
      switch( m_inputParameters.m_interesPointsLocationStrategy )
      {
        case InputParameters::MoravecStrategyT :
        {
          stratPtr.reset( new te::rp::TiePointsLocatorMoravecStrategy() );
          break;
        }
        case InputParameters::SurfStrategyT :
        {
          stratPtr.reset( new te::rp::TiePointsLocatorSURFStrategy() );
          break;
        }        
        default :
        {
          TERP_LOG_AND_THROW( "Invalid strategy" );
          break;
        }
      }
      
      // Matching
      
      te::rp::TiePointsLocatorStrategy::MatchedInterestPointsSetT matchedInterestPoints;
      
      if( m_inputParameters.m_subSampleOptimizationRescaleFactor == 1.0 )
      {
        TERP_TRUE_OR_RETURN_FALSE( stratPtr->initialize( m_inputParameters ),
          "Tie points locator strategy initialization error" );
        
        TERP_TRUE_OR_RETURN_FALSE( stratPtr->getMatchedInterestPoints( 
          (te::gm::GeometricTransformation*)0, 0,
          matchedInterestPoints ),
          "Tie points interest points location error" );        
      }
      else
      {
        // First pass : trying to find mathed points over
        // the sub-sampled raster
        
        TiePointsLocator::InputParameters subSampledinputParameters = 
          m_inputParameters;
        subSampledinputParameters.m_geomTransfMaxError = 0;        
        subSampledinputParameters.m_maxTiePoints = (unsigned int)
          ( 
            ((double)subSampledinputParameters.m_maxTiePoints) 
            * 
            std::sqrt( subSampledinputParameters.m_subSampleOptimizationRescaleFactor ) 
          );
        switch( m_inputParameters.m_interesPointsLocationStrategy )
        {
          case InputParameters::MoravecStrategyT :
          {
            subSampledinputParameters.m_moravecCorrelationWindowWidth = 
              3 
              +
              (unsigned int)
              ( 
                ((double)( subSampledinputParameters.m_moravecCorrelationWindowWidth - 3 ))
                * 
                subSampledinputParameters.m_subSampleOptimizationRescaleFactor
              );
            
            subSampledinputParameters.m_moravecWindowWidth =
              3 
              +
              (unsigned int)
              ( 
                ((double)( subSampledinputParameters.m_moravecWindowWidth - 3 ))
                * 
                subSampledinputParameters.m_subSampleOptimizationRescaleFactor
              );            
            break;
          }
          case InputParameters::SurfStrategyT :
          {
            subSampledinputParameters.m_surfScalesNumber =
              3 
              +
              (unsigned int)
              ( 
                ((double)( subSampledinputParameters.m_surfScalesNumber - 3 ))
                * 
                subSampledinputParameters.m_subSampleOptimizationRescaleFactor
              );            
            subSampledinputParameters.m_surfOctavesNumber =
              1 
              +
              (unsigned int)
              ( 
                ((double)( subSampledinputParameters.m_surfOctavesNumber - 1 ))
                * 
                subSampledinputParameters.m_subSampleOptimizationRescaleFactor
              );              
            break;
          }        
          default :
          {
            TERP_LOG_AND_THROW( "Invalid strategy" );
            break;
          }
        }          
          
        te::rp::TiePointsLocatorStrategy::MatchedInterestPointsSetT subSampledMatchedInterestPoints;
        TERP_TRUE_OR_RETURN_FALSE( stratPtr->initialize( subSampledinputParameters ),
          "Tie points locator strategy initialization error" );
        if( ! stratPtr->getMatchedInterestPoints( (te::gm::GeometricTransformation*)0,
          0, subSampledMatchedInterestPoints ) )
        {
          subSampledMatchedInterestPoints.clear();
        }
        
        // Generating tie-points
        
        te::gm::GTParameters subSampledTransParams;
        std::vector< double > subSampledTiePointsWeights;
        convertMatchedInterestPoints2TiePoints( subSampledMatchedInterestPoints, 
          subSampledTransParams.m_tiePoints, subSampledTiePointsWeights );       
        
        subSampledMatchedInterestPoints.clear();
        
        // trying to create the global geometric transformation
        
        std::auto_ptr< te::gm::GeometricTransformation > transformationPtr(
          te::gm::GTFactory::make( m_inputParameters.m_geomTransfName ) );  
        TERP_TRUE_OR_RETURN_FALSE( transformationPtr.get(), "Invalid geometric transformation" );
        
        double transformationCoveredAreaPercentR1 = 0;
        double transformationCoveredAreaPercentR2 = 0;
        
        if( m_inputParameters.m_enableGeometryFilter )
        {
          std::vector< te::gm::GTParameters::TiePoint > filteredTiePoints;
          te::gm::GTFilter filter;
          if( filter.applyRansac( 
            m_inputParameters.m_geomTransfName, 
            subSampledTransParams,
            m_inputParameters.m_geomTransfMaxError / m_inputParameters.m_subSampleOptimizationRescaleFactor,
            m_inputParameters.m_geomTransfMaxError / m_inputParameters.m_subSampleOptimizationRescaleFactor,
            0,
            m_inputParameters.m_geometryFilterAssurance,
            m_inputParameters.m_enableMultiThread,
            subSampledTiePointsWeights,
            filteredTiePoints,
            transformationPtr  ) )
          {
            transformationCoveredAreaPercentR1 =
              (
                te::rp::GetTPConvexHullArea( filteredTiePoints, false ) 
                /
                (double)(
                  m_inputParameters.m_raster1TargetAreaWidth 
                  *
                  m_inputParameters.m_raster1TargetAreaHeight
                )
              );
              
            transformationCoveredAreaPercentR2 = 
              (
                te::rp::GetTPConvexHullArea( filteredTiePoints, true ) 
                /
                (double)(
                  m_inputParameters.m_raster2TargetAreaWidth 
                  *
                  m_inputParameters.m_raster2TargetAreaHeight
                )
              );
            
            if(
                ( 
                  ((double)filteredTiePoints.size())
                  < 
                  ( 
                    m_inputParameters.m_subSampleOptimizationMinTPNumberFactor
                    *
                    ((double)transformationPtr->getMinRequiredTiePoints() )
                  ) 
                ) 
                ||
                (
                  std::max( 
                    transformationCoveredAreaPercentR1
                    ,
                    transformationCoveredAreaPercentR2
                  )
                  < 
                  (
                    m_inputParameters.m_subSampleOptimizationMinTPAreaCoverage 
                    /
                    100.0
                  )
                )
              )
            {
              transformationPtr.reset();
            }
          }
          else
          {
            transformationPtr.reset();
          };
        }
        else
        {
          if( !transformationPtr->initialize( subSampledTransParams ) )
          {
            transformationPtr.reset();
          }
        }        
        
        // Second pass - Trying to find tie-points over the original images using the found
        // global geometric transformation
        
        TiePointsLocator::InputParameters noSubSampledInputParameters = 
          m_inputParameters;        
        noSubSampledInputParameters.m_subSampleOptimizationRescaleFactor = 1.0;
        
        TERP_TRUE_OR_RETURN_FALSE( stratPtr->initialize( noSubSampledInputParameters ),
          "Tie points locator strategy initialization error" );
        
        TERP_TRUE_OR_RETURN_FALSE( stratPtr->getMatchedInterestPoints( 
          transformationPtr.get(),
          m_inputParameters.m_geomTransfMaxError / ( transformationCoveredAreaPercentR2 *
            m_inputParameters.m_subSampleOptimizationRescaleFactor ),
          matchedInterestPoints ),
          "Tie points interest points location error" );
      }
      
      // Converting to tie-points
      
      std::vector< te::gm::GTParameters::TiePoint > tiePoints;
      std::vector< double > tiePointsWeights;
      convertMatchedInterestPoints2TiePoints( matchedInterestPoints, tiePoints,
        tiePointsWeights );
      
      matchedInterestPoints.clear();
      
      // Execute outliers remotion, if required
      
      if( m_inputParameters.m_enableGeometryFilter )
      {
        te::gm::GTParameters transfParams;
        transfParams.m_tiePoints = tiePoints;
        
        te::gm::GTFilter filter;
        if( !filter.applyRansac( 
          m_inputParameters.m_geomTransfName, 
          transfParams,
          m_inputParameters.m_geomTransfMaxError,
          m_inputParameters.m_geomTransfMaxError,
          0,
          m_inputParameters.m_geometryFilterAssurance,
          m_inputParameters.m_enableMultiThread,
          tiePointsWeights,
          outParamsPtr->m_tiePoints,
          outParamsPtr->m_transformationPtr  ) )
        {
          return false;
        };
      }
      else
      {
        outParamsPtr->m_tiePoints = tiePoints;
        
        outParamsPtr->m_transformationPtr.reset( te::gm::GTFactory::make( 
          m_inputParameters.m_geomTransfName ) );
        TERP_DEBUG_TRUE_OR_THROW( outParamsPtr->m_transformationPtr.get(), 
          "Invalid transformation" );
        
        te::gm::GTParameters transfParams;
        transfParams.m_tiePoints = outParamsPtr->m_tiePoints;
        
        if( ! outParamsPtr->m_transformationPtr->initialize( transfParams ) )
        {
          outParamsPtr->m_transformationPtr.reset();
        }
      }
      
      // tie-points weights
      
      unsigned int tiePointsIdx = 0;
      
      for( unsigned int outTiePointdx = 0 ; outTiePointdx < outParamsPtr->m_tiePoints.size() ;
        ++outTiePointdx )
      {
        for( tiePointsIdx = 0 ; tiePointsIdx < tiePoints.size() ; ++tiePointsIdx )
        {
          if( tiePoints[ tiePointsIdx ] == outParamsPtr->m_tiePoints[ outTiePointdx ] )
          {
            outParamsPtr->m_tiePointsWeights.push_back( tiePointsWeights[ tiePointsIdx ] );
            break;
          }
        }
      }
      
      TERP_TRUE_OR_THROW( outParamsPtr->m_tiePoints.size() ==
        outParamsPtr->m_tiePointsWeights.size(), "Internal error" );
      
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
        
      if( ( m_inputParameters.m_raster1TargetAreaWidth != 0 ) ||
        ( m_inputParameters.m_raster1TargetAreaHeight != 0 ) )
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
        
      if( ( m_inputParameters.m_raster2TargetAreaWidth != 0 )||
        ( m_inputParameters.m_raster2TargetAreaHeight != 0 ) )
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
      
      // Checking strategy
      
      switch( m_inputParameters.m_interesPointsLocationStrategy )
      {
        case InputParameters::MoravecStrategyT :
        {
          break;
        }          
        case InputParameters::SurfStrategyT :
        {
          break;
        }
        default :
        {
          TERP_LOG_AND_RETURN_FALSE( "Invalid strategy" );
          break;
        }
      };
      
      // Checking other parameters
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_pixelSizeXRelation > 0,
        "Invalid m_pixelSizeXRelation" )
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_pixelSizeYRelation > 0,
        "Invalid m_pixelSizeYRelation" )
        
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_geomTransfMaxError > 0,
        "Invalid m_geomTransfMaxError" )        
        
      TERP_TRUE_OR_RETURN_FALSE( te::gm::GTFactory::find( 
        m_inputParameters.m_geomTransfName ),
        "Invalid m_geomTransfName" );
        
      TERP_TRUE_OR_RETURN_FALSE( ( m_inputParameters.m_geometryFilterAssurance >= 0 ) &&
        ( m_inputParameters.m_geometryFilterAssurance <= 1.0 ),
        "Invalid m_geometryFilterAssurance" );          
        
      TERP_TRUE_OR_RETURN_FALSE( ( m_inputParameters.m_subSampleOptimizationRescaleFactor > 0.0 ) &&
        ( m_inputParameters.m_subSampleOptimizationRescaleFactor <= 1.0 ),
        "Invalid m_subSampleOptimizationRescaleFactor" );        
      
      TERP_TRUE_OR_RETURN_FALSE( ( m_inputParameters.m_subSampleOptimizationMinTPAreaCoverage >= 0.0 ) &&
        ( m_inputParameters.m_subSampleOptimizationMinTPAreaCoverage <= 100.0 ),
        "Invalid m_subSampleOptimizationMinTPAreaCoverage" );            
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_subSampleOptimizationMinTPNumberFactor >= 1.0,
        "Invalid m_subSampleOptimizationMinTPNumber" );            
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_tiePointsSubSectorsSplitFactor >= 1,
        "Invalid m_tiePointsSubSectorsSplitFactor" );  
      
      m_isInitialized = true;

      return true;
    }

    bool TiePointsLocator::isInitialized() const
    {
      return m_isInitialized;
    }
    
    void TiePointsLocator::convertMatchedInterestPoints2TiePoints(
      const te::rp::TiePointsLocatorStrategy::MatchedInterestPointsSetT& matchedInterestPoints,
      std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
      std::vector< double >& tiePointsWeights )
    {
      tiePoints.clear();
      tiePointsWeights.clear();
      
      te::gm::GTParameters::TiePoint auxTP;
      te::rp::TiePointsLocatorStrategy::MatchedInterestPointsSetT::const_iterator 
        itB = matchedInterestPoints.begin();
      const te::rp::TiePointsLocatorStrategy::MatchedInterestPointsSetT::const_iterator 
        itE = matchedInterestPoints.end();
      
      while( itB != itE )
      {
        auxTP.first.x = itB->m_point1.m_x;
        auxTP.first.y = itB->m_point1.m_y;
        auxTP.second.x = itB->m_point2.m_x;
        auxTP.second.y = itB->m_point2.m_y;
        tiePoints.push_back( auxTP );
        
        tiePointsWeights.push_back( itB->m_feature );

        ++itB;
      }        
    }     

  } // end namespace rp
}   // end namespace te

