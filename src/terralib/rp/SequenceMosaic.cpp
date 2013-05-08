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
  \file terralib/rp/SequenceMosaic.cpp
  \brief Create a mosaic from a sequence of overlapped rasters using an automatic tie-points detection method.
*/

#include "SequenceMosaic.h"

#include "Macros.h"
//#include "../raster/Interpolator.h"
//#include "../raster/Enums.h"
//#include "../raster/RasterFactory.h"
#include "../raster/Grid.h"
//#include "../raster/Band.h"
#include "../raster/BandProperty.h"
//#include "../raster/PositionIterator.h"
#include "../raster/Utils.h"
#include "../memory/CachedRaster.h"
#include "../memory/ExpansibleRaster.h"
#include "../geometry/Envelope.h"
//#include "../geometry/GTFactory.h"
//#include "../geometry/GTParameters.h"
#include "../geometry/Polygon.h"
#include "../geometry/LinearRing.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/MultiPoint.h"
//#include "../srs/Converter.h"

//#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

//#include <climits>
//#include <cfloat>
//#include <cmath>
#include <memory>

#define SEQUENCE_RASTER_MAX_MOSAIC_MEM_USE 10

namespace te
{
  namespace rp
  {

    SequenceMosaic::InputParameters::InputParameters()
    {
      reset();
    }

    SequenceMosaic::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    SequenceMosaic::InputParameters::~InputParameters()
    {
      reset();
    }

    void SequenceMosaic::InputParameters::reset() throw( te::rp::Exception )
    {
      m_feederRasterPtr = 0;
      m_inputRastersBands.clear();
      m_geomTransfName = "Affine";
      m_interpMethod = te::rst::Interpolator::NearestNeighbor;
      m_noDataValue = 0.0;
      m_forceInputNoDataValue = false;
      m_blendMethod = te::rp::Blender::NoBlendMethod;
      m_autoEqualize = true;
      m_useRasterCache = true;
      m_enableMultiThread = true;
      m_enableProgress = false;
      m_geomTransfMaxError = 2;
      m_tiePointsLocationBandIndex = 0;
      m_maxTiePoints = 0;
      m_maxRastersOffset = 0;
      m_outDataSetsNamePrefix.clear();
      m_minRequiredTiePointsCoveredAreaPercent = 0.0;
    }

    const SequenceMosaic::InputParameters& SequenceMosaic::InputParameters::operator=(
      const SequenceMosaic::InputParameters& params )
    {
      reset();

      m_feederRasterPtr = params.m_feederRasterPtr;
      m_inputRastersBands = params.m_inputRastersBands;
      m_geomTransfName = params.m_geomTransfName;
      m_interpMethod = params.m_interpMethod;
      m_noDataValue = params.m_noDataValue;
      m_forceInputNoDataValue = params.m_forceInputNoDataValue;
      m_blendMethod = params.m_blendMethod;
      m_autoEqualize = params.m_autoEqualize;
      m_useRasterCache = params.m_useRasterCache;
      m_enableMultiThread = params.m_enableMultiThread;
      m_enableProgress = params.m_enableProgress;
      m_geomTransfMaxError = params.m_geomTransfMaxError;
      m_tiePointsLocationBandIndex = params.m_tiePointsLocationBandIndex;
      m_maxTiePoints = params.m_maxTiePoints;
      m_maxRastersOffset = params.m_maxRastersOffset;
      m_minRequiredTiePointsCoveredAreaPercent = 
        params.m_minRequiredTiePointsCoveredAreaPercent;

      return *this;
    }

    te::common::AbstractParameters* SequenceMosaic::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    SequenceMosaic::OutputParameters::OutputParameters()
    {
      reset();
    }

    SequenceMosaic::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    SequenceMosaic::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void SequenceMosaic::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_outputDSPtr = 0;
    }

    const SequenceMosaic::OutputParameters& SequenceMosaic::OutputParameters::operator=(
      const SequenceMosaic::OutputParameters& params )
    {
      reset();

      m_outputDSPtr = params.m_outputDSPtr;

      return *this;
    }

    te::common::AbstractParameters* SequenceMosaic::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    SequenceMosaic::SequenceMosaic()
    {
      reset();
    }

    SequenceMosaic::~SequenceMosaic()
    {
    }

    bool SequenceMosaic::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;
      
      SequenceMosaic::OutputParameters* outParamsPtr = dynamic_cast<
        SequenceMosaic::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      TERP_TRUE_OR_RETURN_FALSE( outParamsPtr->m_outputDSPtr,
        "Invalid data source" );
      TERP_TRUE_OR_RETURN_FALSE( outParamsPtr->m_outputDSPtr->isValid(),
        "Invalid data source" );
      
      // iterating over all rasters
      
      const std::vector< double > dummyRasterOffsets( m_inputParameters.m_inputRastersBands[ 0 ].size(), 0.0 );
      const std::vector< double > dummyRasterScales( m_inputParameters.m_inputRastersBands[ 0 ].size(), 1.0 );
      
      std::vector< unsigned int > dummyRasterBandsIndexes;
      {
        for( unsigned int bandIdx = 0 ; bandIdx < 
          m_inputParameters.m_inputRastersBands[ 0 ].size(); ++bandIdx )
        dummyRasterBandsIndexes.push_back( bandIdx );
      }
      
      std::auto_ptr< te::mem::ExpansibleRaster > mosaicRasterHandler;
      std::vector< te::rst::BandProperty > mosaicBandProperties;
      
      double mosaicXResolution = 0;
      double mosaicYResolution = 0;
      int mosaicSRID = 0;
      std::vector< double > mosaicTargetMeans;
      std::vector< double > mosaicTargetVariances;
      te::gm::Polygon lastMosaicAddedRasterPol(  0, te::gm::PolygonType, 0 ); //the polygon of the last added raster (mosaic world coords)
      te::gm::Polygon mosaicValidDataPol(  0, te::gm::PolygonType, 0 ); // the polygon delimiting the valid data inside the mosaic (mosaic world coods)
      unsigned int mosaicSequenceCounter = 0;
      
      m_inputParameters.m_feederRasterPtr->reset();
      while( m_inputParameters.m_feederRasterPtr->getCurrentObj() )
      {
        const unsigned int inputRasterIdx = 
          m_inputParameters.m_feederRasterPtr->getCurrentOffset();
        
        te::rst::Raster const* inputRasterPtr = 
          m_inputParameters.m_feederRasterPtr->getCurrentObj();

        std::auto_ptr< te::mem::CachedRaster > cachedInputRasterPtr;
        if( m_inputParameters.m_useRasterCache )
        {
          cachedInputRasterPtr.reset( new te::mem::CachedRaster(
            *( m_inputParameters.m_feederRasterPtr->getCurrentObj() ), 
            SEQUENCE_RASTER_MAX_MOSAIC_MEM_USE, 0 ) );
          inputRasterPtr = cachedInputRasterPtr.get();
        }
        
        // Mosaicking
        
        if( mosaicRasterHandler.get() == 0 )
        {
          mosaicXResolution = inputRasterPtr->getGrid()->getResolutionX();
          mosaicYResolution = inputRasterPtr->getGrid()->getResolutionY();
          mosaicSRID = inputRasterPtr->getGrid()->getSRID();
          
          const double& mosaicLLX = inputRasterPtr->getGrid()->getExtent()->m_llx;
          const double& mosaicLLY = inputRasterPtr->getGrid()->getExtent()->m_lly;
          const double& mosaicURX = inputRasterPtr->getGrid()->getExtent()->m_urx;
          const double& mosaicURY = inputRasterPtr->getGrid()->getExtent()->m_ury;        
          
          te::gm::LinearRing* auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
          auxLinearRingPtr->setPoint( 0, mosaicLLX, mosaicURY );
          auxLinearRingPtr->setPoint( 1, mosaicURX, mosaicURY );
          auxLinearRingPtr->setPoint( 2, mosaicURX, mosaicLLY );
          auxLinearRingPtr->setPoint( 3, mosaicLLX, mosaicLLY );
          auxLinearRingPtr->setPoint( 4, mosaicLLX, mosaicURY );
          lastMosaicAddedRasterPol.clear();
          lastMosaicAddedRasterPol.push_back( auxLinearRingPtr );
          lastMosaicAddedRasterPol.setSRID( mosaicSRID );
          
          mosaicValidDataPol = lastMosaicAddedRasterPol;
          
          mosaicBandProperties.clear();
          
          std::vector< te::rst::BandProperty* > bandsProperties;
          for( std::vector< unsigned int >::size_type inputRastersBandsIdx = 0 ;  
            inputRastersBandsIdx <
            m_inputParameters.m_inputRastersBands[ 0 ].size() ; 
            ++inputRastersBandsIdx )
          {
            te::rst::BandProperty const* const inBandPropPtr =
              inputRasterPtr->getBand( m_inputParameters.m_inputRastersBands[ 
              0 ][ inputRastersBandsIdx ] )->getProperty();
            
            bandsProperties.push_back( new te::rst::BandProperty( *inBandPropPtr ) );
            bandsProperties[ inputRastersBandsIdx ]->m_colorInterp = te::rst::GrayIdxCInt;
            bandsProperties[ inputRastersBandsIdx ]->m_noDataValue = m_inputParameters.m_noDataValue;
            
            mosaicBandProperties.push_back( *bandsProperties[ inputRastersBandsIdx ] );
          }

          mosaicRasterHandler.reset( 
            new te::mem::ExpansibleRaster( SEQUENCE_RASTER_MAX_MOSAIC_MEM_USE,
            new te::rst::Grid( *( inputRasterPtr->getGrid() ) ),
            bandsProperties ) );
          TERP_TRUE_OR_RETURN_FALSE( mosaicRasterHandler.get(),
            "Output raster creation error" );
            
          const unsigned int nBands = mosaicRasterHandler->getNumberOfBands();
          
          mosaicTargetMeans.resize( nBands, 0.0 );
          mosaicTargetVariances.resize( nBands, 0.0 );        
            
          for( unsigned int inputRastersBandsIdx = 0 ; inputRastersBandsIdx <
            nBands ; ++inputRastersBandsIdx )
          {
            const unsigned int inputBandIdx =  
              m_inputParameters.m_inputRastersBands[ 0 ][ inputRastersBandsIdx ] ;
            const double& bandNoDataValue = m_inputParameters.m_forceInputNoDataValue ?
              m_inputParameters.m_noDataValue : inputRasterPtr->getBand( inputBandIdx
              )->getProperty()->m_noDataValue;
            const unsigned int outRowsBound = inputRasterPtr->getNumberOfRows();
            const unsigned int outColsBound = inputRasterPtr->getNumberOfColumns();
            te::rst::Band& outBand =
              (*mosaicRasterHandler->getBand( inputRastersBandsIdx ));
            const te::rst::Band& inBand =
              (*inputRasterPtr->getBand( inputBandIdx ));
            unsigned int validPixelsNumber = 0;
            unsigned int outCol = 0;
            unsigned int outRow = 0;
            double pixelValue = 0;
            
            double& mean = mosaicTargetMeans[ inputRastersBandsIdx ];
            mean = 0;          

            for( outRow = 0 ; outRow < outRowsBound ; ++outRow )
            {
              for( outCol = 0 ; outCol < outColsBound ; ++outCol )
              {
                inBand.getValue( outCol, outRow, pixelValue );

                if( pixelValue != bandNoDataValue )
                {
                  outBand.setValue( outCol, outRow, pixelValue );
                  mean += pixelValue;
                  ++validPixelsNumber;
                }
                
              }
            }

            mean /= ( (double)validPixelsNumber );

            // variance calcule

            if( m_inputParameters.m_autoEqualize )
            {
              double& variance = mosaicTargetVariances[ inputRastersBandsIdx ];
              variance = 0;

              double pixelValue = 0;

              for( outRow = 0 ; outRow < outRowsBound ; ++outRow )
              {
                for( outCol = 0 ; outCol < outColsBound ; ++outCol )
                {
                  outBand.getValue( outCol, outRow, pixelValue );

                  if( pixelValue != bandNoDataValue )
                  {
                    variance += ( ( pixelValue - mean ) * ( pixelValue -
                      mean ) ) / ( (double)validPixelsNumber );
                  }
                }
              }
            }
          }
        }
        else
        {
          // Generating the offset and gain info for eath band from the current raster

          std::vector< double > currentRasterBandsOffsets;
          std::vector< double > currentRasterBandsScales;

          if( m_inputParameters.m_autoEqualize )
          {
            double currentRasterVariance = 0;
            double currentRasterMean = 0;

            for( unsigned int inputRastersBandsIdx = 0 ; inputRastersBandsIdx <
              m_inputParameters.m_inputRastersBands[ inputRasterIdx ].size() ;
              ++inputRastersBandsIdx )
            {
              const unsigned int inputBandIdx = m_inputParameters.m_inputRastersBands[ inputRasterIdx ][
                inputRastersBandsIdx ];

              calcBandStatistics( (*inputRasterPtr->getBand( inputBandIdx ) ),
                m_inputParameters.m_forceInputNoDataValue,
                m_inputParameters.m_noDataValue,
                currentRasterMean,
                currentRasterVariance );

              currentRasterBandsScales.push_back( std::sqrt( mosaicTargetVariances[ inputRastersBandsIdx ] /
                currentRasterVariance ) );
              currentRasterBandsOffsets.push_back( mosaicTargetMeans[ inputRastersBandsIdx ] -
                ( currentRasterBandsScales[ inputRastersBandsIdx ] * currentRasterMean ) );
            }
          }
          else
          {
            currentRasterBandsOffsets = dummyRasterOffsets;
            currentRasterBandsScales = dummyRasterScales;
          } 
          
          // Locating the tie points where te::gm::GTParameters::TiePoint::first
          // are :mosaic indexed coods and te::gm::GTParameters::TiePoint::second are
          // indexed coords from the current raster
          
          te::rp::TiePointsLocator::OutputParameters locatorOutParams;
          
          {
            te::rp::TiePointsLocator::InputParameters locatorInParams;
            
            locatorInParams.m_inRaster1Ptr = mosaicRasterHandler.get();
            
            locatorInParams.m_inMaskRaster1Ptr = 0;
            
            locatorInParams.m_inRaster1Bands.push_back( m_inputParameters.m_inputRastersBands[ 0 ][ 
              m_inputParameters.m_tiePointsLocationBandIndex ] );

            double mosaicLLXIndex = 0;
            double mosaicLLYIndex = 0;
            double mosaicURXIndex = 0;
            double mosaicURYIndex = 0;
            mosaicRasterHandler->getGrid()->geoToGrid(
              lastMosaicAddedRasterPol.getMBR()->getLowerLeftX(),
              lastMosaicAddedRasterPol.getMBR()->getLowerLeftY(),
              mosaicLLXIndex,
              mosaicLLYIndex );
            mosaicRasterHandler->getGrid()->geoToGrid(
              lastMosaicAddedRasterPol.getMBR()->getUpperRightX(),
              lastMosaicAddedRasterPol.getMBR()->getUpperRightY(),
              mosaicURXIndex,
              mosaicURYIndex );            
            locatorInParams.m_raster1TargetAreaColStart = (unsigned int)mosaicLLXIndex;
            locatorInParams.m_raster1TargetAreaLineStart = (unsigned int)mosaicURYIndex;
            locatorInParams.m_raster1TargetAreaWidth = (unsigned int)( mosaicURXIndex -
              mosaicLLXIndex );
            locatorInParams.m_raster1TargetAreaHeight = (unsigned int)( mosaicLLYIndex -
              mosaicURYIndex );
              
            locatorInParams.m_inRaster2Ptr = inputRasterPtr;
            
            locatorInParams.m_inMaskRaster2Ptr = 0;
            
            locatorInParams.m_inRaster2Bands.push_back( 
              m_inputParameters.m_inputRastersBands[ inputRasterIdx ][ 
              m_inputParameters.m_tiePointsLocationBandIndex ] );
              
            locatorInParams.m_raster2TargetAreaLineStart = 0;
            locatorInParams.m_raster2TargetAreaColStart = 0;
            locatorInParams.m_raster2TargetAreaWidth = 0;
            locatorInParams.m_raster2TargetAreaHeight = 0;
              
            locatorInParams.m_enableMultiThread = m_inputParameters.m_enableMultiThread;

            locatorInParams.m_enableProgress = false;
            
            locatorInParams.m_maxTiePoints = m_inputParameters.m_maxTiePoints;
            
            locatorInParams.m_pixelSizeXRelation = mosaicRasterHandler->getResolutionX() /
              inputRasterPtr->getResolutionX();
            locatorInParams.m_pixelSizeYRelation = mosaicRasterHandler->getResolutionY() /
              inputRasterPtr->getResolutionY();
              
            locatorInParams.m_geomTransfName = m_inputParameters.m_geomTransfName;
              
            locatorInParams.m_geomTransfMaxError = m_inputParameters.m_geomTransfMaxError;
            
            locatorInParams.m_maxR1ToR2Offset = m_inputParameters.m_maxRastersOffset;
            
            locatorInParams.m_enableGeometryFilter = true;
            
            locatorInParams.m_interpMethod = m_inputParameters.m_interpMethod;
            
            te::rp::TiePointsLocator locatorInstance;
            TERP_TRUE_OR_RETURN_FALSE( locatorInstance.initialize( 
              locatorInParams ), "Tie points locator init error" );
            TERP_TRUE_OR_RETURN_FALSE( locatorInstance.execute( 
              locatorOutParams ), "Tie points locator exec error" );
          }
        
          // The matching was accomplished successfully
        
          if( 
              ( locatorOutParams.m_transformationPtr.get() == 0 ) 
              ||
              ( 
                locatorOutParams.m_tiePoints.size() 
                < 
                ( 
                  2 
                  *
                  locatorOutParams.m_transformationPtr->getMinRequiredTiePoints() 
                ) 
              ) 
              ||
              (
                (
                  getTPConvexHullArea( locatorOutParams.m_tiePoints, true ) 
                  /
                  (double)(
                    inputRasterPtr->getNumberOfColumns() 
                    *
                    inputRasterPtr->getNumberOfRows()
                  )
                )
                < 
                m_inputParameters.m_minRequiredTiePointsCoveredAreaPercent 
              ) 
            )
          {
            TERP_TRUE_OR_RETURN_FALSE( createRasterDataSet( 
              m_inputParameters.m_outDataSetsNamePrefix + 
              boost::lexical_cast< std::string >( mosaicSequenceCounter++ ),
              *( mosaicRasterHandler.get() ), outParamsPtr->m_outputDSPtr ),
              "Data set creation error" );
            mosaicRasterHandler.reset();
          }
          else
          {
            // locating the current raster over the mosaic
            
            double LLXIndexed = 0;
            double LLYIndexed = 0;
            double URXIndexed = 0;
            double URYIndexed = 0;
            double LLX = 0;
            double LLY = 0;
            double URX = 0;
            double URY = 0;            
            {
              double mappedX1 = 0;
              double mappedY1 = 0;
              double mappedX2 = 0;
              double mappedY2 = 0;
              locatorOutParams.m_transformationPtr->inverseMap( 
                0.0,
                (double)( inputRasterPtr->getNumberOfRows() - 1 ),
                mappedX1,
                mappedY1);
              locatorOutParams.m_transformationPtr->inverseMap( 
                (double)( inputRasterPtr->getNumberOfColumns() - 1 ),
                0.0,
                mappedX2,
                mappedY2);                
              
              LLXIndexed = std::min( mappedX1, mappedX2 );
              LLYIndexed = std::max( mappedY1, mappedY2 );
              URXIndexed = std::max( mappedX1, mappedX2 );
              URYIndexed = std::min( mappedY1, mappedY2 );  
              
              mosaicRasterHandler->getGrid()->gridToGeo( LLXIndexed, LLYIndexed,
                LLX, LLY );
              mosaicRasterHandler->getGrid()->gridToGeo( URXIndexed, URYIndexed,
                URX, URY );              
            }            
            
            // Blending the current raster into the mosaic
            
            {
              te::rp::Blender blenderInstance;

              TERP_TRUE_OR_RETURN_FALSE( blenderInstance.initialize(
                *( mosaicRasterHandler.get() ),
                dummyRasterBandsIndexes,
                *inputRasterPtr,
                m_inputParameters.m_inputRastersBands[ inputRasterIdx ],
                m_inputParameters.m_blendMethod,
                te::rst::Interpolator::NearestNeighbor,
                m_inputParameters.m_interpMethod,
                m_inputParameters.m_noDataValue,
                m_inputParameters.m_forceInputNoDataValue,
                dummyRasterOffsets,
                dummyRasterScales,
                currentRasterBandsOffsets,
                currentRasterBandsScales,
                &mosaicValidDataPol,
                0,
                locatorOutParams.m_transformationPtr.get() ), 
                "Blender initiazing error" );              
                
              for( unsigned int inputRastersBandsIdx = 0 ; inputRastersBandsIdx <
                m_inputParameters.m_inputRastersBands[ inputRasterIdx ].size() ;
                ++inputRastersBandsIdx )
              {                
                te::rst::Band& outputBand = (*mosaicRasterHandler->getBand( inputRastersBandsIdx ));

                double outputBandRangeMin = 0;
                double outputBandRangeMax = 0;
                te::rst::GetDataTypeRanges( outputBand.getProperty()->m_type,
                  outputBandRangeMin, outputBandRangeMax );                
                  
                unsigned int outputRow = 0;
                unsigned int outputCol = 0;
                const unsigned int outputRowsStart = std::max( 0.0, URYIndexed );
                const unsigned int outputColsStart = std::max( 0.0, LLXIndexed );
                const unsigned int outputRowsBound = (unsigned int)std::min( 
                  (double)mosaicRasterHandler->getNumberOfRows(),
                  LLYIndexed );
                const unsigned int outputColsBound = (unsigned int)std::min( 
                  (double)mosaicRasterHandler->getNumberOfColumns(),
                  URXIndexed );                  
                double blendedValue = 0;                  
                
                for( outputRow = outputRowsStart ; outputRow < outputRowsBound ;
                  ++outputRow )
                {
                  for( outputCol = outputColsStart ; outputCol < outputColsBound ;
                    ++outputCol )
                  {
                    blenderInstance.getBlendedValue( outputRow, outputCol, inputRastersBandsIdx,
                      blendedValue );
                    
                    if( blendedValue != m_inputParameters.m_noDataValue )
                    {
                      blendedValue = std::max( blendedValue, outputBandRangeMin );
                      blendedValue = std::min( blendedValue, outputBandRangeMax );

                      outputBand.setValue( outputCol, outputRow, blendedValue );
                    }                      
                  }
                }
              }
            }
            
            // updating the mosaic related polygons
            
            {
              te::gm::LinearRing* auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
              auxLinearRingPtr->setPoint( 0, LLX, URY );
              auxLinearRingPtr->setPoint( 1, URX, URY );
              auxLinearRingPtr->setPoint( 2, URX, LLY );
              auxLinearRingPtr->setPoint( 3, LLX, LLY );
              auxLinearRingPtr->setPoint( 4, LLX, URY );
              lastMosaicAddedRasterPol.clear();
              lastMosaicAddedRasterPol.push_back( auxLinearRingPtr );
              
              // current mosaic area in a multi-polygon form
              std::auto_ptr< te::gm::MultiPolygon > mosaicValidDataPolMultiPolPtr(
                new te::gm::MultiPolygon( 0, te::gm::MultiPolygonType,
                mosaicValidDataPol.getSRID(), 0 ) );
              mosaicValidDataPolMultiPolPtr->add( (te::gm::Polygon*)mosaicValidDataPol.clone() );
              
              // union of the current raster box with the current mosaic valid area under the mosaic SRID
              std::auto_ptr< te::gm::Geometry > unionMultiPolPtr; 
              unionMultiPolPtr.reset( mosaicValidDataPolMultiPolPtr->Union(
                &lastMosaicAddedRasterPol ) );
              TERP_TRUE_OR_THROW( unionMultiPolPtr.get(), "Invalid pointer" );
              unionMultiPolPtr->setSRID( mosaicSRID );
              
              TERP_TRUE_OR_THROW( unionMultiPolPtr->getGeomTypeId() == te::gm::PolygonType,
                "Invalid geometry type")
              mosaicValidDataPol = *( (te::gm::Polygon*)unionMultiPolPtr.get() );
            }            
          }
        }
        
        // Move to the next raster
        
        m_inputParameters.m_feederRasterPtr->moveNext();
      }
      
      TERP_TRUE_OR_RETURN_FALSE( createRasterDataSet( 
        m_inputParameters.m_outDataSetsNamePrefix + 
        boost::lexical_cast< std::string >( mosaicSequenceCounter++ ),
        *( mosaicRasterHandler.get() ), outParamsPtr->m_outputDSPtr ),
        "Data set creation error" );
      
      return true;
    }

    void SequenceMosaic::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool SequenceMosaic::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      SequenceMosaic::InputParameters const* inputParamsPtr = dynamic_cast<
        SequenceMosaic::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;

      // Checking the feeder

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_feederRasterPtr,
        "Invalid m_feederRasterPtr" )

      TERP_TRUE_OR_RETURN_FALSE(
        m_inputParameters.m_feederRasterPtr->getObjsCount() > 0,
        "Invalid number of rasters" )

      // checking m_inputRastersBands

      TERP_TRUE_OR_RETURN_FALSE(
        ((unsigned int)m_inputParameters.m_inputRastersBands.size()) ==
        m_inputParameters.m_feederRasterPtr->getObjsCount(),
        "Bands mismatch" );
        
      m_inputParameters.m_feederRasterPtr->reset();

      for( std::vector< std::vector< unsigned int > >::size_type
        inputRastersBandsIdx = 0 ;  inputRastersBandsIdx <
        m_inputParameters.m_inputRastersBands.size() ; ++inputRastersBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_feederRasterPtr->getCurrentObj()
          != 0, "Invalid raster" )
        
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRastersBands[
          inputRastersBandsIdx ].size() > 0, "Invalid bands number" );

        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRastersBands[
          inputRastersBandsIdx ].size() ==  m_inputParameters.m_inputRastersBands[
          0 ].size(), "Bands number mismatch" );
          
        for( std::vector< unsigned int >::size_type currRasterBandidx = 0 ; 
          currRasterBandidx < m_inputParameters.m_inputRastersBands[
          inputRastersBandsIdx ].size() ; ++currRasterBandidx )
        {
          TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRastersBands[
            inputRastersBandsIdx ][ currRasterBandidx ] <
            m_inputParameters.m_feederRasterPtr->getCurrentObj()->getNumberOfBands(),
            "Invalid band index" );
            
          TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_tiePointsLocationBandIndex < 
            m_inputParameters.m_inputRastersBands[ inputRastersBandsIdx ].size(),
            "Invalid parameter m_tiePointsLocationBandIndex" );
        }
        
        m_inputParameters.m_feederRasterPtr->moveNext();
      }
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_maxTiePoints, 
        "Invalid tie points number" );
        
      TERP_TRUE_OR_RETURN_FALSE( 
        ( ( m_inputParameters.m_minRequiredTiePointsCoveredAreaPercent >= 0.0 ) &&
        ( m_inputParameters.m_minRequiredTiePointsCoveredAreaPercent <= 100.0 ) ),
        "Invalid parameter m_minRequiredTiePointsCoveredAreaPercent" );

      m_isInitialized = true;

      return true;
    }

    bool SequenceMosaic::isInitialized() const
    {
      return m_isInitialized;
    }

    void SequenceMosaic::calcBandStatistics( const te::rst::Band& band,
      const bool& forceNoDataValue,
      const double& noDataValue,
      double& mean, double& variance )
    {
      mean = 0;
      variance = 0;

      double internalNoDataValue = 0;
      if( forceNoDataValue )
        internalNoDataValue = noDataValue;
      else
        internalNoDataValue = band.getProperty()->m_noDataValue;

      const unsigned int nCols = band.getProperty()->m_blkw *
        band.getProperty()->m_nblocksx;
      const unsigned int nLines = band.getProperty()->m_blkh *
        band.getProperty()->m_nblocksy;

      double pixelsNumber = 0;
      double value = 0;
      unsigned int col = 0;
      unsigned int line = 0;

      for( line = 0 ; line < nLines ; ++line )
        for( col = 0 ; col < nCols ; ++col )
        {
          band.getValue( col, line, value );

          if( value != internalNoDataValue )
          {
            mean += value;
            ++pixelsNumber;
          }
        }

      if( pixelsNumber != 0.0 )
      {
        mean /= pixelsNumber;

        for( line = 0 ; line < nLines ; ++line )
          for( col = 0 ; col < nCols ; ++col )
          {
            band.getValue( col, line, value );

            if( value != internalNoDataValue )
            {
              variance += ( ( value - mean ) * ( value - mean ) ) / pixelsNumber;
            }
          }

      }
    }
    
    bool SequenceMosaic::createRasterDataSet( const std::string& dataSetName,
      const te::rst::Raster& sourceRaster, te::da::DataSource* dataSourcePtr ) const
    {
      return false;
    }
    
    double SequenceMosaic::getTPConvexHullArea( 
      const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
      const bool useTPSecondCoordPair ) const
    {
      if( tiePoints.size() < 3 )
      {
        return 0;
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
        
        return ( (te::gm::Surface*)points.convexHull() )->getArea();
      }
    }

  } // end namespace rp
}   // end namespace te

