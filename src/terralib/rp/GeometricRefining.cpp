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
  \file terralib/rp/GeometricRefining.cpp
  \brief Geometric (and positioning) correction of a already geo-referenced raster using a set of reference rasters.
*/

#include "GeometricRefining.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../geometry/Envelope.h"
#include "../geometry/GTFactory.h"
#include "../geometry/GTFilter.h"
#include "../memory/CachedRaster.h"
#include "../raster/Grid.h"
#include "Register.h"

#include <limits>
#include <memory>
#include <cmath>

namespace te
{
  namespace rp
  {

    GeometricRefining::InputParameters::InputParameters()
    {
      reset();
    }

    GeometricRefining::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    GeometricRefining::InputParameters::~InputParameters()
    {
      reset();
    }

    void GeometricRefining::InputParameters::reset() throw( te::rp::Exception )
    {
      m_inRasterPtr = 0;
      m_inRasterBands2Process.clear();
      m_inRasterTPLocationBands.clear();
      m_referenceRastersPtr = 0;
      m_referenceTPLocationBands.clear();
      m_enableMultiThread = true;
      m_enableProgress = false;
      m_interpMethod = te::rst::Interpolator::NearestNeighbor;
      m_locatorParams.reset();
      m_minInRasterCoveredAreaPercent = 25.0;
      m_minrReferenceRasterCoveredAreaPercent = 25.0;
      m_inRasterSubSectorsFactor = 3;
      m_inRasterExpectedRowError = 10;
      m_inRasterExpectedColError = 10;
      m_inRasterExpectedRowDisplacement = 0;
      m_inRasterExpectedColDisplacement = 0;
      m_processAllReferenceRasters = true;
      m_enableRasterCache = true;
      m_geomTransfName = "affine";
      m_geomTransfMaxTiePointError = 2.0;
      m_outliersRemotionAssurance = 0.1;
      m_outliersRemotionMaxIterations = 0;
      m_outputNoDataValue = 0.0;
    }

    const GeometricRefining::InputParameters& GeometricRefining::InputParameters::operator=(
      const GeometricRefining::InputParameters& params )
    {
      reset();

      m_inRasterPtr = params.m_inRasterPtr;
      m_inRasterBands2Process = params.m_inRasterBands2Process;
      m_inRasterTPLocationBands = params.m_inRasterTPLocationBands;
      m_referenceRastersPtr = params.m_referenceRastersPtr;
      m_referenceTPLocationBands = params.m_referenceTPLocationBands;
      m_enableMultiThread = params.m_enableMultiThread;
      m_enableProgress = params.m_enableProgress;
      m_interpMethod = params.m_interpMethod;
      m_locatorParams = params.m_locatorParams;
      m_minInRasterCoveredAreaPercent = params.m_minInRasterCoveredAreaPercent;
      m_minrReferenceRasterCoveredAreaPercent = params.m_minrReferenceRasterCoveredAreaPercent;
      m_inRasterSubSectorsFactor = params.m_inRasterSubSectorsFactor;
      m_inRasterExpectedRowError = params.m_inRasterExpectedRowError;
      m_inRasterExpectedColError = params.m_inRasterExpectedColError;
      m_inRasterExpectedRowDisplacement = params.m_inRasterExpectedRowDisplacement;
      m_inRasterExpectedColDisplacement = params.m_inRasterExpectedColDisplacement;
      m_processAllReferenceRasters = params.m_processAllReferenceRasters;
      m_enableRasterCache = params.m_enableRasterCache;
      m_geomTransfName = params.m_geomTransfName;
      m_geomTransfMaxTiePointError = params.m_geomTransfMaxTiePointError;
      m_outliersRemotionAssurance = params.m_outliersRemotionAssurance;
      m_outliersRemotionMaxIterations = params.m_outliersRemotionMaxIterations;
      m_outputNoDataValue = params.m_outputNoDataValue;

      return *this;
    }

    te::common::AbstractParameters* GeometricRefining::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }
    
    // ----------------------------------------------------------------------

    GeometricRefining::OutputParameters::OutputParameters()
    {
      reset();
    }

    GeometricRefining::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    GeometricRefining::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void GeometricRefining::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const GeometricRefining::OutputParameters& GeometricRefining::OutputParameters::operator=(
      const GeometricRefining::OutputParameters& params )
    {
      reset();
        
      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }

    te::common::AbstractParameters* GeometricRefining::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }
    
    // ----------------------------------------------------------------------

    GeometricRefining::GeometricRefining()
    {
      reset();
    }

    GeometricRefining::~GeometricRefining()
    {
    }

    bool GeometricRefining::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;
      
      GeometricRefining::OutputParameters* outParamsPtr = dynamic_cast<
        GeometricRefining::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( 1 );
        
        progressPtr->setMessage( "Mosaicking" );
      }          
      
      // A global pointer to the Input raster
      
      te::rst::Raster const* inputRasterPtr = m_inputParameters.m_inRasterPtr;
      
      // Raster cache issues
      
      std::auto_ptr< te::mem::CachedRaster > inputCachedRasterPtr;
      
      if( m_inputParameters.m_enableRasterCache )
      {
        inputCachedRasterPtr.reset( new te::mem::CachedRaster( *inputRasterPtr,
          10, 0 ) );
        inputRasterPtr = inputCachedRasterPtr.get();
      }
      
      // splitting the reference rasters by sector
      
      std::vector< std::vector< unsigned int > > refRastersIndexesBySector( 
        m_inputParameters.m_inRasterSubSectorsFactor *
        m_inputParameters.m_inRasterSubSectorsFactor );
            
      {
        std::vector< te::gm::Envelope > refRastersBBoxes;
        double foundMinX = std::numeric_limits< double >::max();
        double foundMaxX = -1.0 * foundMinX;
        double foundMinY = std::numeric_limits< double >::max();
        double foundMaxY = -1.0 * foundMinY;      
        te::rst::Raster const* refRasterPtr = 0;
        
        while( ( refRasterPtr = m_inputParameters.m_referenceRastersPtr->getCurrentObj() ) )
        {
          te::gm::Envelope refRasterEnvelope( *refRasterPtr->getExtent() );
          // Reprojecting the reference raster bounding box, if needed
          
          if( refRasterPtr->getSRID() != inputRasterPtr->getSRID() )
          {
            refRasterEnvelope.transform( refRasterPtr->getSRID(),
              inputRasterPtr->getSRID() );
          }
          
          refRastersBBoxes.push_back( refRasterEnvelope );
          
          if( foundMinX > refRasterEnvelope.getLowerLeftX() )
            foundMinX = refRasterEnvelope.getLowerLeftX();
          if( foundMinX > refRasterEnvelope.getUpperRightX() )
            foundMinX = refRasterEnvelope.getUpperRightX();          
          
          if( foundMaxX < refRasterEnvelope.getLowerLeftX() )
            foundMaxX = refRasterEnvelope.getLowerLeftX();
          if( foundMaxX < refRasterEnvelope.getUpperRightX() )
            foundMaxX = refRasterEnvelope.getUpperRightX();

          if( foundMinY > refRasterEnvelope.getLowerLeftY() )
            foundMinY = refRasterEnvelope.getLowerLeftY();
          if( foundMinY > refRasterEnvelope.getUpperRightY() )
            foundMinY = refRasterEnvelope.getUpperRightY();          
          
          if( foundMaxY < refRasterEnvelope.getLowerLeftY() )
            foundMaxY = refRasterEnvelope.getLowerLeftY();
          if( foundMaxY < refRasterEnvelope.getUpperRightY() )
            foundMaxY = refRasterEnvelope.getUpperRightY();          
          
          // Moving to the next reference raster
          
          m_inputParameters.m_referenceRastersPtr->moveNext();
        }
        
        te::gm::Coord2D center;
        double sectoXSize = ( foundMaxX - foundMinX ) / ((double)m_inputParameters.m_inRasterSubSectorsFactor);
        double sectoYSize = ( foundMaxY - foundMinY ) / ((double)m_inputParameters.m_inRasterSubSectorsFactor);
        unsigned int sectoXOff = 0;
        unsigned int sectoYOff = 0;
        unsigned int refRastersIndexesBySectorIdx = 0;
        
        for( unsigned int rasterIdx = 0 ; rasterIdx < refRastersBBoxes.size() ;
          ++rasterIdx )
        {
          center = refRastersBBoxes[ rasterIdx ].getCenter();
          sectoXOff = (unsigned int)std::floor( ( center.x - foundMinX ) / sectoXSize );
          sectoYOff = (unsigned int)std::floor( ( center.y - foundMinY ) / sectoYSize );
          
          refRastersIndexesBySectorIdx = ( sectoYOff * m_inputParameters.m_inRasterSubSectorsFactor ) +
            sectoXOff;
          assert( refRastersIndexesBySectorIdx < refRastersIndexesBySector.size() );
          
          refRastersIndexesBySector[ refRastersIndexesBySectorIdx ].push_back( rasterIdx );
        }
      }
      
      // matching the reference rasters
      
      std::auto_ptr< te::gm::GeometricTransformation > baseGeometricTransformPtr(
        te::gm::GTFactory::make( m_inputParameters.m_geomTransfName ) );
      if( baseGeometricTransformPtr.get() == 0 )
      {
        return false;
      }
      const unsigned int minimumRequiredTiePointsNumber = baseGeometricTransformPtr->getMinRequiredTiePoints();
      baseGeometricTransformPtr.reset();
      
      std::vector< te::gm::GTParameters::TiePoint > baseTransAgreementTiePoints;
      
      std::vector< MatchingInfo > refRastersMatchingInfo;    
      
      bool continueOnLoop = true;
      
      while( continueOnLoop )
      {
        bool aRefRasterWasProcessed = false;
        
        for( unsigned int sectorIdx = 0 ; sectorIdx <
          refRastersIndexesBySector.size() ; ++sectorIdx )
        {
          for( unsigned int sectorRastersIdx = 0 ; sectorRastersIdx <
            refRastersIndexesBySector[ sectorIdx ].size() ; ++sectorRastersIdx )
          {          
            const unsigned int& refRasterIdx = 
              refRastersIndexesBySector[ sectorIdx ][ sectorRastersIdx ];
              
            if( refRasterIdx < m_inputParameters.m_referenceRastersPtr->getObjsCount() )
            {
              TERP_TRUE_OR_THROW( m_inputParameters.m_referenceRastersPtr->moveTo( refRasterIdx ),
                "Rasters feeder mover error" ); 
                
              te::rst::Raster const* refRasterPtr = 
                m_inputParameters.m_referenceRastersPtr->getCurrentObj();
                
              // Reprojection issues
                
              std::auto_ptr< te::rst::Raster > reprojectedRefRasterPtr;
              
              if( inputRasterPtr->getSRID() != refRasterPtr->getSRID() )
              {
                std::map< std::string, std::string > rInfo;
                rInfo[ "FORCE_MEM_DRIVER" ] = "TRUE";
                
                reprojectedRefRasterPtr.reset( refRasterPtr->transform( 
                  inputRasterPtr->getSRID(), rInfo, te::rst::Interpolator::NearestNeighbor ) );
                
                refRasterPtr = reprojectedRefRasterPtr.get();
              }              
              
              // The reference image position over the input image
              
              const double& llx = refRasterPtr->getGrid()->getExtent()->getLowerLeftX();
              const double& lly = refRasterPtr->getGrid()->getExtent()->getLowerLeftY();
              const double& urx = refRasterPtr->getGrid()->getExtent()->getUpperRightX();
              const double& ury = refRasterPtr->getGrid()->getExtent()->getUpperRightY();
              
              // The search area over the input image
              
              double llRow = 0;
              double llCol = 0;
              inputRasterPtr->getGrid()->geoToGrid( llx, lly, llCol, llRow );
              llRow += ((double)m_inputParameters.m_inRasterExpectedRowDisplacement);
              llCol += ((double)m_inputParameters.m_inRasterExpectedColDisplacement);
              llRow += ((double)m_inputParameters.m_inRasterExpectedRowError);
              llCol -= ((double)m_inputParameters.m_inRasterExpectedColError);
              llCol = std::max( 0.0, llCol );
              llCol = std::min( llCol, (double)( inputRasterPtr->getNumberOfColumns() - 1 ) );
              llRow = std::max( 0.0, llRow );
              llRow = std::min( llRow, (double)( inputRasterPtr->getNumberOfRows() - 1 ) );
              
              double urRow = 0;
              double urCol = 0;
              inputRasterPtr->getGrid()->geoToGrid( urx, ury, urCol, urRow );
              urRow += ((double)m_inputParameters.m_inRasterExpectedRowDisplacement);
              urCol += ((double)m_inputParameters.m_inRasterExpectedColDisplacement);              
              urCol += ((double)m_inputParameters.m_inRasterExpectedColError);
              urRow -= ((double)m_inputParameters.m_inRasterExpectedRowError);
              urCol = std::max( 0.0, urCol );
              urCol = std::min( urCol, (double)( inputRasterPtr->getNumberOfColumns() - 1 ) );
              urRow = std::max( 0.0, urRow );
              urRow = std::min( urRow, (double)( inputRasterPtr->getNumberOfRows() - 1 ) );              
              
              const unsigned int searchAreaWidth = (unsigned int)( urCol - llCol + 1.0 );
              const unsigned int searchAreaHeight = (unsigned int)( llRow - urRow + 1.0 );
              
              if( ( searchAreaWidth > 0 ) && ( searchAreaHeight > 0 ) )
              {
                // Matching the reference image
                
                te::rp::TiePointsLocator::InputParameters locatorInputParams =
                  m_inputParameters.m_locatorParams;
                locatorInputParams.m_inRaster1Ptr = inputRasterPtr;
                locatorInputParams.m_inMaskRaster1Ptr = 0;
                locatorInputParams.m_inRaster1Bands = m_inputParameters.m_inRasterTPLocationBands;
                locatorInputParams.m_raster1TargetAreaLineStart = (unsigned int)urRow;
                locatorInputParams.m_raster1TargetAreaColStart = (unsigned int)llCol;
                locatorInputParams.m_raster1TargetAreaWidth = searchAreaWidth;
                locatorInputParams.m_raster1TargetAreaHeight = searchAreaHeight;
                locatorInputParams.m_inRaster2Ptr = refRasterPtr;
                locatorInputParams.m_inMaskRaster2Ptr = 0;
                locatorInputParams.m_inRaster2Bands = 
                  m_inputParameters.m_referenceTPLocationBands[ refRasterIdx ];
                locatorInputParams.m_raster2TargetAreaLineStart = 0;
                locatorInputParams.m_raster2TargetAreaColStart = 0;
                locatorInputParams.m_raster2TargetAreaWidth = 0;
                locatorInputParams.m_raster2TargetAreaHeight = 0;
                locatorInputParams.m_enableMultiThread = m_inputParameters.m_enableMultiThread;
                locatorInputParams.m_enableProgress = false;
                locatorInputParams.m_pixelSizeXRelation = inputRasterPtr->getResolutionX() / 
                  refRasterPtr->getResolutionX();
                locatorInputParams.m_pixelSizeYRelation = inputRasterPtr->getResolutionY() / 
                  refRasterPtr->getResolutionY();
                locatorInputParams.m_geomTransfName = m_inputParameters.m_geomTransfName;
                locatorInputParams.m_geomTransfMaxError = m_inputParameters.m_geomTransfMaxTiePointError;
                locatorInputParams.m_maxR1ToR2Offset = 0;
                locatorInputParams.m_enableGeometryFilter = true;
                locatorInputParams.m_geometryFilterAssurance = m_inputParameters.m_outliersRemotionAssurance;
                
                te::rp::TiePointsLocator locator;
                
                if( locator.initialize( locatorInputParams ) )
                {
                  te::rp::TiePointsLocator::OutputParameters locatorOutputParams;
                  
                  if( locator.execute( locatorOutputParams ) )
                  {
                    if( 
                        ( locatorOutputParams.m_transformationPtr.get() != 0 )
                        &&
                        ( 
                          locatorOutputParams.m_tiePoints.size()
                          >
                          minimumRequiredTiePointsNumber
                        )
                      )
                    {
                      const double convexHullAreaPercent =
                        (
                          getTPConvexHullArea( locatorOutputParams.m_tiePoints, true )
                          /
                          (double)
                          (
                            refRasterPtr->getNumberOfColumns()
                            *
                            refRasterPtr->getNumberOfRows()
                          )
                        );
                      
                      if( ( 100.0 * convexHullAreaPercent ) >=
                          m_inputParameters.m_minrReferenceRasterCoveredAreaPercent )
                      {
                        MatchingInfo matchingInfo;
                        matchingInfo.m_referenceRasterIndex = refRasterIdx;
                        matchingInfo.m_convexHullAreaPercent = convexHullAreaPercent;
                        
                        te::gm::GTParameters::TiePoint tiePoint;
                        
                        for( unsigned int tpIdx = 0 ; tpIdx < locatorOutputParams.m_tiePoints.size() ;
                          ++tpIdx )
                        {
                          tiePoint.first.x = locatorOutputParams.m_tiePoints[ tpIdx ].first.x;
                          tiePoint.first.y = locatorOutputParams.m_tiePoints[ tpIdx ].first.y;
                          
                          refRasterPtr->getGrid()->gridToGeo( 
                             locatorOutputParams.m_tiePoints[ tpIdx ].second.x,
                             locatorOutputParams.m_tiePoints[ tpIdx ].second.y,
                             tiePoint.second.x,
                             tiePoint.second.y );
                          
                          matchingInfo.m_tiePoints.push_back( tiePoint );                          
                        }
                        
                        refRastersMatchingInfo.push_back( matchingInfo );
                      }
                    }
                  }
                }
              }
              
              // Finding the tie-points in agreement with the choosen geometric transformation model
              
              if( !m_inputParameters.m_processAllReferenceRasters )
              {
                if( getTransformation( refRastersMatchingInfo, baseGeometricTransformPtr,
                  baseTransAgreementTiePoints ) )
                {
                  sectorRastersIdx = refRastersIndexesBySector[ sectorIdx ].size();
                  sectorIdx = refRastersIndexesBySector.size();
                  continueOnLoop = false;
                }
                else
                {
                  baseGeometricTransformPtr.reset();
                  baseTransAgreementTiePoints.clear();
                }
              }              
                
              // Mark the reference raster as processed
              
              refRastersIndexesBySector[ sectorIdx ][ sectorRastersIdx ] = 
                std::numeric_limits< unsigned int >::max();
                
              aRefRasterWasProcessed = true; 
              
              //skip to the next sector
              
              sectorRastersIdx = refRastersIndexesBySector[ sectorIdx ].size();
            }
          }
        }
        
        if( ! aRefRasterWasProcessed )
        {
          continueOnLoop = false;
        }
      }
      
      // Finding the tie-points in agreement with the choosen geometric transformation model
      
      if( m_inputParameters.m_processAllReferenceRasters )
      {
        if( ! getTransformation( refRastersMatchingInfo, baseGeometricTransformPtr,
          baseTransAgreementTiePoints ) )
        {
          return false;
        }
      }
      
      // Generating the refined output raster
      
      te::rp::Register::InputParameters regInParams;
      regInParams.m_inputRasterPtr = inputRasterPtr;
      regInParams.m_inputRasterBands = m_inputParameters.m_inRasterBands2Process;
      regInParams.m_tiePoints = baseTransAgreementTiePoints;
      regInParams.m_outputSRID = inputRasterPtr->getSRID();
      regInParams.m_outputResolutionX = inputRasterPtr->getResolutionX();
      regInParams.m_outputResolutionY = inputRasterPtr->getResolutionY();
      regInParams.m_interpMethod = m_inputParameters.m_interpMethod;
      regInParams.m_noDataValue = m_inputParameters.m_outputNoDataValue;
      regInParams.m_geomTransfName = m_inputParameters.m_geomTransfName;
      
      te::rp::Register::OutputParameters regOutParams;
      regOutParams.m_rType = outParamsPtr->m_rType;
      regOutParams.m_rInfo = outParamsPtr->m_rInfo;
      
      te::rp::Register registerInstance;
      
      if( ! registerInstance.initialize( regInParams ) )
      {
        return false;
      }
      
      if( ! registerInstance.execute( regOutParams ) )
      {
        return false;
      }
      
      outParamsPtr->m_outputRasterPtr.reset( regOutParams.m_outputRasterPtr.release() );
      
      return true;
    }

    void GeometricRefining::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool GeometricRefining::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      GeometricRefining::InputParameters const* inputParamsPtr = dynamic_cast<
        GeometricRefining::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;
      
      // checking input raster
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRasterPtr,
         "Invalid raster pointer" );
         
      TERP_TRUE_OR_RETURN_FALSE( 
        ( m_inputParameters.m_inRasterPtr->getAccessPolicy() & te::common::RAccess ),
         "Invalid raster" );  
         
      // Checking input raster bands
      
      for( unsigned int inRasterBands2ProcessIdx = 0 ; inRasterBands2ProcessIdx <
        m_inputParameters.m_inRasterBands2Process.size() ; 
        ++inRasterBands2ProcessIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRasterBands2Process[
          inRasterBands2ProcessIdx ] < m_inputParameters.m_inRasterPtr->getNumberOfBands(),
          "Invalid raster bands" ); 
      }
      
      for( unsigned int inRasterTPLocationBandsIdx = 0 ; inRasterTPLocationBandsIdx <
        m_inputParameters.m_inRasterTPLocationBands.size() ; 
        ++inRasterTPLocationBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRasterTPLocationBands[
          inRasterTPLocationBandsIdx ] < m_inputParameters.m_inRasterPtr->getNumberOfBands(),
          "Invalid raster bands" ); 
      }      

      // Checking the feeder

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_referenceRastersPtr,
        "Invalid reference rasters feeder pointer" )

      TERP_TRUE_OR_RETURN_FALSE(
        m_inputParameters.m_referenceRastersPtr->getObjsCount() > 0,
        "Invalid number of reference rasters" )

      // checking other parameters
        
      TERP_TRUE_OR_RETURN_FALSE( 
        ( ( m_inputParameters.m_minInRasterCoveredAreaPercent >= 0.0 ) &&
        ( m_inputParameters.m_minInRasterCoveredAreaPercent <= 100.0 ) ),
        "Invalid parameter m_minRasterCoveredAreaPercent" );
        
      TERP_TRUE_OR_RETURN_FALSE( 
        ( ( m_inputParameters.m_minrReferenceRasterCoveredAreaPercent >= 0.0 ) &&
        ( m_inputParameters.m_minrReferenceRasterCoveredAreaPercent <= 100.0 ) ),
        "Invalid parameter m_minrReferenceRasterCoveredAreaPercent" );        
        
      TERP_TRUE_OR_RETURN_FALSE( 
        ( m_inputParameters.m_inRasterSubSectorsFactor > 0 ),
        "Invalid parameter m_rasterSubSectorsFactor" );           
        
      TERP_TRUE_OR_RETURN_FALSE( 
        ( m_inputParameters.m_inRasterExpectedRowError > 0 ),
        "Invalid parameter m_inRasterRowMaxError" );           
      
      TERP_TRUE_OR_RETURN_FALSE( 
        ( m_inputParameters.m_inRasterExpectedColError > 0 ),
        "Invalid parameter m_inRasterColMaxError" );        

      m_isInitialized = true;

      return true;
    }

    bool GeometricRefining::isInitialized() const
    {
      return m_isInitialized;
    }

    void GeometricRefining::convert( 
          const std::vector< MatchingInfo >& inTiePoints,
          std::vector< te::gm::GTParameters::TiePoint >& outTiePoints ) const
    {
      outTiePoints.clear();
      
      for( unsigned int inTiePointsIdx = 0 ; inTiePointsIdx < inTiePoints.size() ; ++inTiePointsIdx )
      {
        const MatchingInfo& mInfo = inTiePoints[ inTiePointsIdx ];
        
        for( unsigned int mInfoTiePointsIdx = 0 ; mInfoTiePointsIdx < mInfo.m_tiePoints.size() ; 
          ++mInfoTiePointsIdx )
        {
          outTiePoints.push_back( mInfo.m_tiePoints[ mInfoTiePointsIdx ] );
        }
      }
    }
    
    double GeometricRefining::getTPConvexHullArea( 
      const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
      const bool useTPSecondCoordPair ) const
    {
      std::auto_ptr< te::gm::Polygon > convexHullPtr;
      if( !getTPConvexHull( tiePoints, useTPSecondCoordPair, convexHullPtr ) )
      {
        return 0.0;
      }
      else
      {
        return convexHullPtr->getArea();
      }
    }
    
    bool GeometricRefining::getTPConvexHull( 
      const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
      const bool useTPSecondCoordPair,
      std::auto_ptr< te::gm::Polygon >& convexHullPtr ) const
    {
      if( tiePoints.size() < 3 )
      {
        return false;
      }
      else
      {
        te::gm::MultiPoint points( 0, te::gm::MultiPointType );
        
        for( unsigned int tiePointsIdx = 0 ; tiePointsIdx < tiePoints.size() ;
          ++tiePointsIdx )
        {
          if( useTPSecondCoordPair )
            points.add( new te::gm::Point( tiePoints[ tiePointsIdx ].second.x,
              tiePoints[ tiePointsIdx ].second.y ) );
          else
            points.add( new te::gm::Point( tiePoints[ tiePointsIdx ].first.x,
              tiePoints[ tiePointsIdx ].first.y ) );
        }
        
        std::auto_ptr< te::gm::Geometry > geomPtr( points.convexHull() );

        if( dynamic_cast< te::gm::Polygon* >( geomPtr.get() ) )
        {
          convexHullPtr.reset( 
            dynamic_cast< te::gm::Polygon* >( geomPtr.release() ) );
          return true;
        }
        else
        {
          return false;
        }
      }
    }   
    
    bool GeometricRefining::getTransformation( const std::vector< MatchingInfo >& inTiePoints,
      std::auto_ptr< te::gm::GeometricTransformation >& geometricTransformPtr,
      std::vector< te::gm::GTParameters::TiePoint >& baseTransAgreementTiePoints ) const
    {
      geometricTransformPtr.reset();
      baseTransAgreementTiePoints.clear();
      
      te::gm::GTParameters geoTransParams;
      convert( inTiePoints, geoTransParams.m_tiePoints );
      
      te::gm::GTFilter filter;
      
      if( 
          ! filter.applyRansac( 
            m_inputParameters.m_geomTransfName,
            geoTransParams,
            (
              m_inputParameters.m_geomTransfMaxTiePointError
              *
              (
                std::min(
                  m_inputParameters.m_inRasterPtr->getResolutionX(),
                  m_inputParameters.m_inRasterPtr->getResolutionY()
                )
              )
            ),
            m_inputParameters.m_geomTransfMaxTiePointError,
            m_inputParameters.m_outliersRemotionMaxIterations,
            m_inputParameters.m_outliersRemotionAssurance,
            m_inputParameters.m_enableMultiThread,
            std::vector< double >(),
            baseTransAgreementTiePoints,
            geometricTransformPtr
          )
        )
      {
        geometricTransformPtr.reset();
        baseTransAgreementTiePoints.clear();
        return false;
      }
      
      const double convexHullAreaPercent =
        (
          100.0
          *
          getTPConvexHullArea( baseTransAgreementTiePoints, false )
          /
          (double)
          (
            m_inputParameters.m_inRasterPtr->getNumberOfColumns()
            *
            m_inputParameters.m_inRasterPtr->getNumberOfRows()
          )
        );       
        
      if( convexHullAreaPercent >= m_inputParameters.m_minInRasterCoveredAreaPercent )
      {
        return true;
      }
      else
      {      
        geometricTransformPtr.reset();
        baseTransAgreementTiePoints.clear();
        return false;
      }
    }

  } // end namespace rp
}   // end namespace te


