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
#include "../common/progress/TaskProgress.h"
#include "Register.h"
#include "Functions.h"

#include <limits>
#include <memory>
#include <cmath>

#include <boost/lexical_cast.hpp>

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
      m_referenceRastersWeights.clear();
      m_referenceTPLocationBands.clear();
      m_enableMultiThread = true;
      m_enableProgress = false;
      m_interpMethod = te::rst::Interpolator::NearestNeighbor;
      m_locatorParams.reset();
      m_minInRasterCoveredAreaPercent = 25.0;
      m_minrReferenceRasterCoveredAreaPercent = 10.0;
      m_inRasterSubSectorsFactor = 3;
      m_inRasterExpectedRowError = 10;
      m_inRasterExpectedColError = 10;
      m_inRasterExpectedRowDisplacement = 0;
      m_inRasterExpectedColDisplacement = 0;
      m_processAllReferenceRasters = true;
      m_enableRasterCache = true;
      m_geomTransfName = "Affine";
      m_geomTransfMaxTiePointError = 2.0;
      m_outliersRemotionAssurance = 0.75;
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
      m_referenceRastersWeights = params.m_referenceRastersWeights;
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
      m_matchingResult.clear();
      m_geomTransPtr.reset();
      m_tiePoints.clear();
    }

    const GeometricRefining::OutputParameters& GeometricRefining::OutputParameters::operator=(
      const GeometricRefining::OutputParameters& params )
    {
      reset();
        
      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;
      m_matchingResult = params.m_matchingResult;
      m_geomTransPtr.reset( params.m_geomTransPtr->clone() );
      m_tiePoints = params.m_tiePoints;

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
      
      outParamsPtr->m_outputRasterPtr.reset();
      
      {
        outParamsPtr->m_matchingResult.resize( 
          m_inputParameters.m_referenceRastersPtr->getObjsCount() );
        for( unsigned int idx = 0 ; idx < outParamsPtr->m_matchingResult.size() ;
          ++idx )
        {
          outParamsPtr->m_matchingResult[ idx ].m_status = 
            OutputParameters::MatchingResult::NotMatched;
          outParamsPtr->m_matchingResult[ idx ].m_referenceRasterIndex = idx;
        }
      }
      
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( 
          m_inputParameters.m_referenceRastersPtr->getObjsCount() );
        
        progressPtr->setMessage( "Analysing reference images" );
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
      unsigned int validReferenceRastersNumber = 0;
            
      {
        // reference rasters weights
        
        std::vector< double > referenceRastersWeights;
        if( m_inputParameters.m_referenceRastersWeights.size() )
        {
          referenceRastersWeights = m_inputParameters.m_referenceRastersWeights;
        }
        else
        {
          referenceRastersWeights.resize( m_inputParameters.m_referenceRastersPtr->getObjsCount(),
            1.0 );
        }
        
        // The search area
        
        const double inRasterExpectedXError = ((double)m_inputParameters.m_inRasterExpectedColError)
          * inputRasterPtr->getResolutionX();
        const double inRasterExpectedYError = ((double)m_inputParameters.m_inRasterExpectedRowError)
          * inputRasterPtr->getResolutionY();
        const double inRasterExpectedXDisplacement = ((double)m_inputParameters.m_inRasterExpectedColDisplacement)
          * inputRasterPtr->getResolutionX();
        const double inRasterExpectedYDisplacement = ((double)m_inputParameters.m_inRasterExpectedRowDisplacement)
          * inputRasterPtr->getResolutionY();
          
        const double searchAreaMinX = inputRasterPtr->getGrid()->getExtent()->getLowerLeftX() -
          inRasterExpectedXError + inRasterExpectedXDisplacement;
        const double searchAreaMaxX = inputRasterPtr->getGrid()->getExtent()->getUpperRightX() +
          inRasterExpectedXError + inRasterExpectedXDisplacement;

        const double searchAreaMinY = inputRasterPtr->getGrid()->getExtent()->getLowerLeftY() -
          inRasterExpectedYError + inRasterExpectedYDisplacement;
        const double searchAreaMaxY = inputRasterPtr->getGrid()->getExtent()->getUpperRightY() +
          inRasterExpectedYError + inRasterExpectedYDisplacement;          
          
        // Retriving each reference raster bounding boxes
        
        std::vector< te::gm::Envelope > selectedRefRastersBBoxes;
        std::vector< unsigned int > selectedRefRastersIndexes;
        te::rst::Raster const* refRasterPtr = 0;
        
        m_inputParameters.m_referenceRastersPtr->reset();
        
        while( ( refRasterPtr = m_inputParameters.m_referenceRastersPtr->getCurrentObj() ) )
        {
          te::gm::Envelope refRasterEnvelope( *refRasterPtr->getExtent() );
          
          // Reprojecting the reference raster bounding box, if needed
          
          if( refRasterPtr->getSRID() != inputRasterPtr->getSRID() )
          {
            refRasterEnvelope.transform( refRasterPtr->getSRID(),
              inputRasterPtr->getSRID() );
          }
          
          if( 
                 ( refRasterEnvelope.getLowerLeftX() > searchAreaMinX )
              && ( refRasterEnvelope.getLowerLeftX() < searchAreaMaxX )   
              && ( refRasterEnvelope.getUpperRightX() > searchAreaMinX )
              && ( refRasterEnvelope.getUpperRightX() < searchAreaMaxX )   
              && ( refRasterEnvelope.getLowerLeftY() > searchAreaMinY )
              && ( refRasterEnvelope.getLowerLeftY() < searchAreaMaxY )
              && ( refRasterEnvelope.getUpperRightY() > searchAreaMinY )
              && ( refRasterEnvelope.getUpperRightY() < searchAreaMaxY )              
            )
          {
            selectedRefRastersBBoxes.push_back( refRasterEnvelope );
            selectedRefRastersIndexes.push_back( 
              m_inputParameters.m_referenceRastersPtr->getCurrentOffset() );
            ++validReferenceRastersNumber;
          }
          
          // Moving to the next reference raster
          
          m_inputParameters.m_referenceRastersPtr->moveNext();
          
          // Progress 
          
          if( m_inputParameters.m_enableProgress )
          {
            progressPtr->pulse();
            if( ! progressPtr->isActive() ) return false;
          }             
        }
        
        // defining sectors
        
        double sectoXSize = ( searchAreaMaxX - searchAreaMinX ) / 
          ((double)m_inputParameters.m_inRasterSubSectorsFactor);
        double sectoYSize = ( searchAreaMaxY - searchAreaMinY ) / 
          ((double)m_inputParameters.m_inRasterSubSectorsFactor);        
        
        te::gm::Coord2D center;
        unsigned int sectoXOff = 0;
        unsigned int sectoYOff = 0;
        unsigned int refRastersIndexesBySectorIdx = 0;
        
        for( unsigned int selectedRefRastersBBoxesIdx = 0 ; selectedRefRastersBBoxesIdx < 
          selectedRefRastersBBoxes.size() ; ++selectedRefRastersBBoxesIdx )
        {
          center = selectedRefRastersBBoxes[ selectedRefRastersBBoxesIdx ].getCenter();
          sectoXOff = (unsigned int)std::floor( ( center.x - searchAreaMinX ) / sectoXSize );
          sectoYOff = (unsigned int)std::floor( ( center.y - searchAreaMinY ) / sectoYSize );
          
          refRastersIndexesBySectorIdx = ( sectoYOff * m_inputParameters.m_inRasterSubSectorsFactor ) +
            sectoXOff;
          assert( refRastersIndexesBySectorIdx < refRastersIndexesBySector.size() );
          
          refRastersIndexesBySector[ refRastersIndexesBySectorIdx ].push_back(
            selectedRefRastersIndexes[ selectedRefRastersBBoxesIdx ] );
        }
        
        // sorting the rasters inside each sector by the given weights
        // the first reference raster with higher weight
        
        unsigned int sectorRastersIndexesIdx = 0;
        unsigned int sectorRastersIndexesIdxBound = 0;
        unsigned int sectorRastersIndexesNextIdx = 0;
        unsigned int rasterIdx = 0;
        unsigned int nextRasterIdx = 0;
        
        for( refRastersIndexesBySectorIdx = 0 ; refRastersIndexesBySectorIdx <
          refRastersIndexesBySector.size() ; ++refRastersIndexesBySectorIdx )
        {
          std::vector< unsigned int >& sectorRastersIndexes = 
            refRastersIndexesBySector[ refRastersIndexesBySectorIdx ];
            
          if( sectorRastersIndexes.size() > 1 )  
          {
            sectorRastersIndexesIdxBound = sectorRastersIndexes.size() - 1;
            
            while( sectorRastersIndexesIdxBound != 0 )
            {
              for( sectorRastersIndexesIdx = 0 ; sectorRastersIndexesIdx <
                sectorRastersIndexesIdxBound  ; ++sectorRastersIndexesIdx )
              {
                sectorRastersIndexesNextIdx = sectorRastersIndexesIdx + 1;
                
                rasterIdx = sectorRastersIndexes[ sectorRastersIndexesIdx ];
                nextRasterIdx = sectorRastersIndexes[ sectorRastersIndexesNextIdx ];
                
                if( 
                    referenceRastersWeights[ rasterIdx ]  
                    <
                    referenceRastersWeights[ nextRasterIdx ]  
                  )
                {
                  sectorRastersIndexes[ sectorRastersIndexesIdx ] = nextRasterIdx;
                  sectorRastersIndexes[ sectorRastersIndexesNextIdx ] = rasterIdx;
                }
              }
              
              --sectorRastersIndexesIdxBound;
            }
          }
        }
      }
      
      // matching the reference rasters    
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset();
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( validReferenceRastersNumber );
        
        progressPtr->setMessage( "Matching reference images" );
      }        
      
      std::auto_ptr< te::gm::GeometricTransformation > baseGeometricTransformPtr(
        te::gm::GTFactory::make( m_inputParameters.m_geomTransfName ) );
      if( baseGeometricTransformPtr.get() == 0 )
      {
        return false;
      }
      const unsigned int minimumRequiredTiePointsNumber = baseGeometricTransformPtr->getMinRequiredTiePoints();
      baseGeometricTransformPtr.reset();
      
      std::vector< te::gm::GTParameters::TiePoint > baseTransAgreementTiePoints;
      
      std::vector< InternalMatchingInfo > refRastersMatchingInfo;    
      
      bool continueOnLoop = true;
      
      while( continueOnLoop )
      {
        bool aRefRasterWasProcessed = false;
        
        for( unsigned int refRastersIndexesBySectorIdx = 0 ; refRastersIndexesBySectorIdx <
          refRastersIndexesBySector.size() ; ++refRastersIndexesBySectorIdx )
        {
          std::vector< unsigned int >& sector = 
            refRastersIndexesBySector[ refRastersIndexesBySectorIdx ];
            
          for( unsigned int sectorIdx = 0 ; sectorIdx < sector.size() ; ++sectorIdx )
          {          
            const unsigned int refRasterIdx = sector[ sectorIdx ];
              
            if( refRasterIdx < m_inputParameters.m_referenceRastersPtr->getObjsCount() )
            {
              // Mark the reference raster as processed
              
              sector[ sectorIdx ] = std::numeric_limits< unsigned int >::max();       
                
              aRefRasterWasProcessed = true; 
                
              // open the reference raster
              
              TERP_TRUE_OR_THROW( m_inputParameters.m_referenceRastersPtr->moveTo( refRasterIdx ),
                "Rasters feeder mover error" ); 
                
              te::rst::Raster const* refRasterPtr = 
                m_inputParameters.m_referenceRastersPtr->getCurrentObj();
                
              // Reprojection issues
                
/*              te::rp::Copy2DiskRaster( *refRasterPtr, "refRaster_" +
                boost::lexical_cast< std::string >( refRasterIdx ) + ".tif" );  */              
                
              std::auto_ptr< te::rst::Raster > reprojectedRefRasterPtr;
              
              if( inputRasterPtr->getSRID() != refRasterPtr->getSRID() )
              {
                std::map< std::string, std::string > rInfo;
                rInfo[ "FORCE_MEM_DRIVER" ] = "TRUE";
                
                reprojectedRefRasterPtr.reset( refRasterPtr->transform( 
                  inputRasterPtr->getSRID(), rInfo, te::rst::Interpolator::NearestNeighbor ) );
                
                TERP_TRUE_OR_RETURN_FALSE( reprojectedRefRasterPtr.get(),
                  "Raster reprojection error" );
                
                refRasterPtr = reprojectedRefRasterPtr.get();
              }              
              
//               te::rp::Copy2DiskRaster( *refRasterPtr, "refRaster_" +
//                 boost::lexical_cast< std::string >( refRasterIdx ) + "_reprojected.tif" );
              
              // The reference image position over the input image
              
              const double searchAreaULX = refRasterPtr->getGrid()->getExtent()->getLowerLeftX();
              const double searchAreaULY = refRasterPtr->getGrid()->getExtent()->getUpperRightY();
              
              // The search area over the input image
              
              double searchAreaULRow = 0;
              double searchAreaULCol = 0;
              inputRasterPtr->getGrid()->geoToGrid( searchAreaULX, searchAreaULY, searchAreaULCol, searchAreaULRow );
              
              double searchAreaLRRow = searchAreaULRow - 1.0 + ((double)refRasterPtr->getNumberOfRows());
              double searchAreaLRCol = searchAreaULCol - 1.0 + ((double)refRasterPtr->getNumberOfColumns());
              
              searchAreaULRow += ((double)m_inputParameters.m_inRasterExpectedRowDisplacement);
              searchAreaULRow -= ((double)m_inputParameters.m_inRasterExpectedRowError);
              searchAreaULRow = std::max( 0.0, searchAreaULRow );
              searchAreaULRow = std::min( searchAreaULRow, (double)( inputRasterPtr->getNumberOfRows() - 1 ) );              
              
              searchAreaLRRow += ((double)m_inputParameters.m_inRasterExpectedRowDisplacement);
              searchAreaLRRow += ((double)m_inputParameters.m_inRasterExpectedRowError);
              searchAreaLRRow = std::max( 0.0, searchAreaLRRow );
              searchAreaLRRow = std::min( searchAreaLRRow, (double)( inputRasterPtr->getNumberOfRows() - 1 ) );              
              
              searchAreaULCol += ((double)m_inputParameters.m_inRasterExpectedColDisplacement);
              searchAreaULCol -= ((double)m_inputParameters.m_inRasterExpectedColError);
              searchAreaULCol = std::max( 0.0, searchAreaULCol );
              searchAreaULCol = std::min( searchAreaULCol, (double)( inputRasterPtr->getNumberOfColumns() - 1 ) );  
              
              searchAreaLRCol += ((double)m_inputParameters.m_inRasterExpectedColDisplacement);
              searchAreaLRCol += ((double)m_inputParameters.m_inRasterExpectedColError);
              searchAreaLRCol = std::max( 0.0, searchAreaLRCol );
              searchAreaLRCol = std::min( searchAreaLRCol, (double)( inputRasterPtr->getNumberOfColumns() - 1 ) ); 
              
              const unsigned int searchAreaWidth = (unsigned int)( searchAreaLRCol - searchAreaULCol + 1.0 );
              const unsigned int searchAreaHeight = (unsigned int)( searchAreaLRRow - searchAreaULRow + 1.0 );
              
              if( ( searchAreaWidth > 1 ) && ( searchAreaHeight > 1 ) )
              {
                outParamsPtr->m_matchingResult[ refRasterIdx ].m_searchAreaRowStart =
                  searchAreaULRow;
                outParamsPtr->m_matchingResult[ refRasterIdx ].m_searchAreaColStart =
                  searchAreaULCol;   
                outParamsPtr->m_matchingResult[ refRasterIdx ].m_searchAreaWidth =
                  searchAreaWidth;   
                outParamsPtr->m_matchingResult[ refRasterIdx ].m_searchAreaHeigh =
                  searchAreaHeight;                  
                  
                // Matching the reference image
                
                te::rp::TiePointsLocator::InputParameters locatorInputParams =
                  m_inputParameters.m_locatorParams;
                locatorInputParams.m_inRaster1Ptr = inputRasterPtr;
                locatorInputParams.m_inMaskRaster1Ptr = 0;
                locatorInputParams.m_inRaster1Bands = m_inputParameters.m_inRasterTPLocationBands;
                locatorInputParams.m_raster1TargetAreaLineStart = (unsigned int)searchAreaULRow;
                locatorInputParams.m_raster1TargetAreaColStart = (unsigned int)searchAreaULCol;
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
                        InternalMatchingInfo matchingInfo;
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
                          outParamsPtr->m_matchingResult[ refRasterIdx ].m_tiePoints.push_back(
                            tiePoint );
                        }
                        
                        refRastersMatchingInfo.push_back( matchingInfo );

                        outParamsPtr->m_matchingResult[ refRasterIdx ].m_status = 
                          OutputParameters::MatchingResult::Success;
                      }
                      else
                      {
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
                          
                          outParamsPtr->m_matchingResult[ refRasterIdx ].m_tiePoints.push_back(
                            tiePoint );
                        }
                                                
                        outParamsPtr->m_matchingResult[ refRasterIdx ].m_status = 
                          OutputParameters::MatchingResult::Fail;    
                      }
                      
                      double ulCol = 0;
                      double ulRow = 0;
                      locatorOutputParams.m_transformationPtr->inverseMap(
                        0.0, 0.0, ulCol, ulRow );
                      
                      double lrCol = 0;
                      double lrRow = 0;
                      locatorOutputParams.m_transformationPtr->inverseMap(
                        (double)( refRasterPtr->getNumberOfColumns() - 1 ), 
                        (double)( refRasterPtr->getNumberOfRows() - 1 ), lrCol, lrRow );                      
                      
                      outParamsPtr->m_matchingResult[ refRasterIdx ].m_matchedPositionRowStart = 
                        (unsigned int)std::min( ulRow, lrRow );
                      outParamsPtr->m_matchingResult[ refRasterIdx ].m_matchedPositionColStart = 
                        (unsigned int)std::min( ulCol, lrCol );                        
                        
                      outParamsPtr->m_matchingResult[ refRasterIdx ].m_matchedPositionWidth = 
                        (unsigned int)std::abs( lrCol - ulCol );
                      outParamsPtr->m_matchingResult[ refRasterIdx ].m_matchedPositionHeight = 
                        (unsigned int)std::abs( lrRow - ulRow );                      
                    }
                    else
                    {
                      outParamsPtr->m_matchingResult[ refRasterIdx ].m_tiePoints =
                        locatorOutputParams.m_tiePoints;
                      outParamsPtr->m_matchingResult[ refRasterIdx ].m_status = 
                        OutputParameters::MatchingResult::Fail;                                                
                    }
                  }
                  else
                  {
                    outParamsPtr->m_matchingResult[ refRasterIdx ].m_status = 
                      OutputParameters::MatchingResult::Fail;
                  }
                }
                else
                {
                  outParamsPtr->m_matchingResult[ refRasterIdx ].m_status = 
                    OutputParameters::MatchingResult::Fail;
                }
              }
              else
              {
                outParamsPtr->m_matchingResult[ refRasterIdx ].m_status = 
                  OutputParameters::MatchingResult::Fail;
              }
              
              //skip to the next sector

              sectorIdx = sector.size();                               
              
              // Finding the tie-points in agreement with the choosen geometric transformation model
              
              if( !m_inputParameters.m_processAllReferenceRasters )
              {
                if( getTransformation( refRastersMatchingInfo, baseGeometricTransformPtr,
                  baseTransAgreementTiePoints ) )
                {
                  // No need to precess more reference rasters
                  // Break the loop
                  
                  refRastersIndexesBySectorIdx = refRastersIndexesBySector.size();
                  continueOnLoop = false;
                }
                else
                {
                  baseGeometricTransformPtr.reset();
                  baseTransAgreementTiePoints.clear();
                }
              }
              
              // Progress 
              
              if( m_inputParameters.m_enableProgress )
              {
                progressPtr->pulse();
                if( ! progressPtr->isActive() ) return false;
              }                           
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
      else
      {
        if( baseGeometricTransformPtr.get() == 0 )
        {
          return false;
        }
      }  
      
      outParamsPtr->m_geomTransPtr.reset( baseGeometricTransformPtr->clone() );
      outParamsPtr->m_tiePoints = baseTransAgreementTiePoints;
      
      // Generating the refined output raster
      
      te::rp::Register::InputParameters regInParams;
      regInParams.m_inputRasterPtr = inputRasterPtr;
      regInParams.m_inputRasterBands = m_inputParameters.m_inRasterBands2Process;
      regInParams.m_tiePoints.clear();
      regInParams.m_outputSRID = inputRasterPtr->getSRID();
      regInParams.m_outputResolutionX = inputRasterPtr->getResolutionX();
      regInParams.m_outputResolutionY = inputRasterPtr->getResolutionY();
      regInParams.m_interpMethod = m_inputParameters.m_interpMethod;
      regInParams.m_noDataValue = m_inputParameters.m_outputNoDataValue;
      regInParams.m_geomTransfName.clear();
      regInParams.m_geomTransfPtr = baseGeometricTransformPtr.get();
      
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
      
      // Checking reference rasters weights

      TERP_TRUE_OR_RETURN_FALSE(
        m_inputParameters.m_referenceRastersWeights.empty() ? true :
        m_inputParameters.m_referenceRastersWeights.size() ==
        m_inputParameters.m_referenceRastersPtr->getObjsCount(),
        "Invalid reference rasters weights" );

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
      const std::vector< InternalMatchingInfo >& inTiePoints,
      std::vector< te::gm::GTParameters::TiePoint >& outTiePoints,
      std::vector< double >& outTiePointsWeights ) const
    {
      outTiePoints.clear();
      outTiePointsWeights.clear();
      
      // Guessing limits
      
      double referenceImagesWeightsMin =  std::numeric_limits< double >::max();
      double referenceImagesWeightsMax = -1.0 * referenceImagesWeightsMin;      
      unsigned int inTiePointsIdx = 0;
      unsigned int mInfoTiePointsIdx = 0;
      double minTPNumberByRefRaster = std::numeric_limits< double >::max();
      double maxTPNumberByRefRaster = -1.0 * minTPNumberByRefRaster;
      double minConvexHullAreaPercentByRefRaster = std::numeric_limits< double >::max();
      double maxConvexHullAreaPercentByRefRaster = -1.0 * minConvexHullAreaPercentByRefRaster;
      
      for( inTiePointsIdx = 0 ; inTiePointsIdx < inTiePoints.size() ; ++inTiePointsIdx )
      {
        const InternalMatchingInfo& mInfo = inTiePoints[ inTiePointsIdx ];
        
        if( minTPNumberByRefRaster > ((double)mInfo.m_tiePoints.size()) )
        {
          minTPNumberByRefRaster = ((double)mInfo.m_tiePoints.size());
        }
        
        if( maxTPNumberByRefRaster < ((double)mInfo.m_tiePoints.size()) )
        {
          maxTPNumberByRefRaster = ((double)mInfo.m_tiePoints.size());
        }    
        
        if( minConvexHullAreaPercentByRefRaster > mInfo.m_convexHullAreaPercent )
        {
          minConvexHullAreaPercentByRefRaster = mInfo.m_convexHullAreaPercent;
        }
        
        if( maxConvexHullAreaPercentByRefRaster < mInfo.m_convexHullAreaPercent )
        {
          maxConvexHullAreaPercentByRefRaster = mInfo.m_convexHullAreaPercent;
        }
        
        if( m_inputParameters.m_referenceRastersWeights.size() )
        {
          if( referenceImagesWeightsMin > 
            ((double)m_inputParameters.m_referenceRastersWeights[ mInfo.m_referenceRasterIndex ] ) )
          {
            referenceImagesWeightsMin = 
              ((double)m_inputParameters.m_referenceRastersWeights[ mInfo.m_referenceRasterIndex ] );
          }
          
          if( referenceImagesWeightsMax < 
            ((double)m_inputParameters.m_referenceRastersWeights[ mInfo.m_referenceRasterIndex ] ) )
          {
            referenceImagesWeightsMax = 
              ((double)m_inputParameters.m_referenceRastersWeights[ mInfo.m_referenceRasterIndex ] );
          }  
        }
      }      
      
      double weight = 0;
      const double convexHullAreaPercentGain = ( maxConvexHullAreaPercentByRefRaster == 
        minConvexHullAreaPercentByRefRaster ) ? 0.0 :
        1.0 / ( maxConvexHullAreaPercentByRefRaster - minConvexHullAreaPercentByRefRaster );
      const double convexHullAreaPercentOffset = (convexHullAreaPercentGain == 0.0 ) ? 
        1.0 : -1.0 * minConvexHullAreaPercentByRefRaster;
      const double tiePointsNumberGain = ( maxTPNumberByRefRaster == 
        minTPNumberByRefRaster ) ? 0.0 :
        1.0 / ( maxTPNumberByRefRaster - minTPNumberByRefRaster );
      const double tiePointsNumberOffset = ( tiePointsNumberGain == 0.0 ) ? 1.0 : -1.0 * minTPNumberByRefRaster;      
      
      double referenceImagesWeightsGain = 0.0;
      double referenceImagesWeightsOffset = 0.0;
      if( m_inputParameters.m_referenceRastersWeights.size() )
      {
        referenceImagesWeightsGain = ( referenceImagesWeightsMax == 
          referenceImagesWeightsMin ) ? 0.0 :
          1.0 / ( referenceImagesWeightsMax - referenceImagesWeightsMin );
        referenceImagesWeightsOffset = (referenceImagesWeightsGain == 0.0 ) ? 
          1.0 : -1.0 * referenceImagesWeightsMin;
      }
      
      for( inTiePointsIdx = 0 ; inTiePointsIdx < inTiePoints.size() ; ++inTiePointsIdx )
      {
        const InternalMatchingInfo& mInfo = inTiePoints[ inTiePointsIdx ];
        
        weight = 
          (
            (
              ( mInfo.m_convexHullAreaPercent + convexHullAreaPercentOffset )
              *
              convexHullAreaPercentGain
            )
            +
            (
              ( ((double)mInfo.m_tiePoints.size()) + tiePointsNumberOffset )
              *
              tiePointsNumberGain
            )
            +
            (
              ( m_inputParameters.m_referenceRastersWeights.empty() )
              ?
              (
                1.0
              )
              :
              (
                (
                  ((double)m_inputParameters.m_referenceRastersWeights[ mInfo.m_referenceRasterIndex ] )
                  +
                  referenceImagesWeightsOffset
                )
                *
                referenceImagesWeightsGain
              )
            )
          )
          /
          3.0;          
        
        for( mInfoTiePointsIdx = 0 ; mInfoTiePointsIdx < mInfo.m_tiePoints.size() ; 
          ++mInfoTiePointsIdx )
        {
          outTiePoints.push_back( mInfo.m_tiePoints[ mInfoTiePointsIdx ] );
          outTiePointsWeights.push_back( weight );
        }
      }
    }
    
    double GeometricRefining::getTPConvexHullArea( 
      const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
      const bool useTPSecondCoordPair ) const
    {
      std::auto_ptr< te::gm::Surface > convexHullPtr;
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
      std::auto_ptr< te::gm::Surface >& convexHullPtr ) const
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

        if( dynamic_cast< te::gm::Surface* >( geomPtr.get() ) )
        {
          convexHullPtr.reset( 
            dynamic_cast< te::gm::Surface* >( geomPtr.release() ) );
          return true;
        }
        else
        {
          return false;
        }
      }
    }   
    
    bool GeometricRefining::getTransformation( const std::vector< InternalMatchingInfo >& inTiePoints,
      std::auto_ptr< te::gm::GeometricTransformation >& geometricTransformPtr,
      std::vector< te::gm::GTParameters::TiePoint >& baseTransAgreementTiePoints ) const
    {
      geometricTransformPtr.reset();
      baseTransAgreementTiePoints.clear();
      
      te::gm::GTParameters geoTransParams;
      std::vector< double > tiePointsWeights;
      convert( inTiePoints, geoTransParams.m_tiePoints, tiePointsWeights );
      
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
            tiePointsWeights,
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


