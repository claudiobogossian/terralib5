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
  \brief Create mosaics from a sequence of overlapped rasters using an automatic tie-points detection method. 
*/

#include "SequenceMosaic.h"

#include "RasterHandler.h"
#include "Macros.h"
#include "Functions.h"
#include "../raster/Grid.h"
#include "../raster/BandProperty.h"
#include "../raster/Utils.h"
#include "../raster/RasterFactory.h"
#include "../memory/CachedRaster.h"
#include "../memory/ExpansibleRaster.h"
#include "../geometry/Envelope.h"
#include "../geometry/GeometricTransformation.h"
#include "../geometry/Polygon.h"
#include "../geometry/LinearRing.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/Point.h"
#include "../common/progress/TaskProgress.h"
#include "../common/PlatformUtils.h"

#include <boost/lexical_cast.hpp>

#include <memory>
#include <cmath>

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
      m_tiePointsLocationBandIndex = 0;
      m_outDataSetsNamePrefix.clear();
      m_outDataSetsNameSufix.clear();
      m_minRequiredTiePointsCoveredAreaPercent = 0.0;
      m_locatorParams.reset();
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
      m_tiePointsLocationBandIndex = params.m_tiePointsLocationBandIndex;
      m_outDataSetsNamePrefix = params.m_outDataSetsNamePrefix;
      m_outDataSetsNameSufix = params.m_outDataSetsNameSufix;
      m_minRequiredTiePointsCoveredAreaPercent = 
        params.m_minRequiredTiePointsCoveredAreaPercent;
      m_locatorParams = params.m_locatorParams;

      return *this;
    }

    te::common::AbstractParameters* SequenceMosaic::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }
    
    // ----------------------------------------------------------------------

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
      m_sequencesInfo.clear();
    }

    const SequenceMosaic::OutputParameters& SequenceMosaic::OutputParameters::operator=(
      const SequenceMosaic::OutputParameters& params )
    {
      reset();

      m_outputDSPtr = params.m_outputDSPtr;
      m_sequencesInfo = params.m_sequencesInfo;

      return *this;
    }

    te::common::AbstractParameters* SequenceMosaic::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }
    
    // ----------------------------------------------------------------------
    
    SequenceMosaic::MosaicSequenceInfo::MosaicSequenceInfo()
    {
    }    
    
    SequenceMosaic::MosaicSequenceInfo::~MosaicSequenceInfo()
    {
    }    
    
    void SequenceMosaic::MosaicSequenceInfo::clear()
    {
      m_dataSetName.clear();
      m_rasterFeederIndexes.clear();
      m_tiePoints.clear();
    }      
    
    // ----------------------------------------------------------------------

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
        
      outParamsPtr->m_sequencesInfo.clear();
        
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( m_inputParameters.m_feederRasterPtr->getObjsCount() );
        
        progressPtr->setMessage( "Mosaicking" );
      }          
      
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
      std::vector< double > mosaicTargetMeans;
      std::vector< double > mosaicTargetVariances;
      te::gm::MultiPolygon mosaicValidAreaPols(  0, te::gm::MultiPolygonType, 0 ); // the polygons delimiting the valid data inside the mosaic (mosaic world coods)
      std::vector< double > mosaicBandsRangeMin;
      std::vector< double > mosaicBandsRangeMax;        
      unsigned int lastInputRasterBBoxLLXIndexed = 0; // The last raster added to the mosaic position
      unsigned int lastInputRasterBBoxLLYIndexed = 0; // The last raster added to the mosaic position
      unsigned int lastInputRasterBBoxURXIndexed = 0; // The last raster added to the mosaic position
      unsigned int lastInputRasterBBoxURYIndexed = 0; // The last raster added to the mosaic position
      MosaicSequenceInfo currentMosaicSquenceInfo;
      
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
        
        // saving the current mosaic (debug purposes)
        
//         if( mosaicRasterHandler.get() )
//         {
//           if( ! createDiskRasterCopy( "Mosaic_" + 
//             boost::lexical_cast< std::string >( inputRasterIdx - 1 ) + ".tif", 
//             *mosaicRasterHandler  ) )
//           {
//             return false;
//           }
//         }
        
        // Mosaicking
        
        if( mosaicRasterHandler.get() == 0 )
        {
          // calculating the initial mosaic blocking scheme          
          
          unsigned int mosaicNBlocksX = 4;
          unsigned int mosaicNBlocksY = 4;
          if( m_inputParameters.m_enableMultiThread )
          {
            const unsigned int nProc = te::common::GetPhysProcNumber();
            
            if( ( mosaicNBlocksX * mosaicNBlocksY ) < nProc )
            {
              mosaicNBlocksX = mosaicNBlocksY = (unsigned int)std::ceil( 
                std::sqrt( (double)nProc ) );
            }            
          }
          
          unsigned int mosaicBlockW = (unsigned int)std::ceil( ((double) inputRasterPtr->getNumberOfColumns()) 
            / ((double)mosaicNBlocksX) );
          unsigned int mosaicBlockH = (unsigned int)std::ceil( ((double)inputRasterPtr->getNumberOfRows()) 
            / ((double)mosaicNBlocksY ) );          
          
          unsigned int mosaicNCols = mosaicBlockW * mosaicNBlocksX;
          unsigned int mosaicNRows = mosaicBlockH * mosaicNBlocksY;
          
          // creating the raster mosaic
          
          std::vector< te::rst::BandProperty* > bandsProperties;
          
          for( std::vector< unsigned int >::size_type inputRastersBandsIdx = 0 ; 
            inputRastersBandsIdx < m_inputParameters.m_inputRastersBands[ inputRasterIdx ].size() ; 
            ++inputRastersBandsIdx )
          {
            te::rst::BandProperty const* const inBandPropPtr =
              inputRasterPtr->getBand( m_inputParameters.m_inputRastersBands[ 
              inputRasterIdx ][ inputRastersBandsIdx ] )->getProperty();
            
            bandsProperties.push_back( new te::rst::BandProperty( *inBandPropPtr ) );
            bandsProperties[ inputRastersBandsIdx ]->m_colorInterp = te::rst::GrayIdxCInt;
            bandsProperties[ inputRastersBandsIdx ]->m_noDataValue = m_inputParameters.m_noDataValue;
            bandsProperties[ inputRastersBandsIdx ]->m_blkw = mosaicBlockW;
            bandsProperties[ inputRastersBandsIdx ]->m_blkh = mosaicBlockH;
            bandsProperties[ inputRastersBandsIdx ]->m_nblocksx = mosaicNBlocksX;
            bandsProperties[ inputRastersBandsIdx ]->m_nblocksy = mosaicNBlocksY;              
          }
          
          te::gm::Coord2D uLC( inputRasterPtr->getGrid()->getExtent()->getLowerLeftX(),
            inputRasterPtr->getGrid()->getExtent()->getUpperRightY() );
          
          te::rst::Grid* mosaicGridPtr = new te::rst::Grid( 
            mosaicNCols, 
            mosaicNRows,
            inputRasterPtr->getGrid()->getResolutionX(),
            inputRasterPtr->getGrid()->getResolutionY(),
            &uLC,
            inputRasterPtr->getSRID() );

          mosaicRasterHandler.reset( 
            new te::mem::ExpansibleRaster( SEQUENCE_RASTER_MAX_MOSAIC_MEM_USE,
            mosaicGridPtr,
            bandsProperties ) );
          TERP_TRUE_OR_RETURN_FALSE( mosaicRasterHandler.get(),
            "Output raster creation error" );
          
          // updating the global mosaic variables
          
          mosaicBandsRangeMin.resize( 
            mosaicRasterHandler->getNumberOfBands(), 0 );
          mosaicBandsRangeMax.resize( 
            mosaicRasterHandler->getNumberOfBands(), 0 );          
          for( unsigned int mosaicBandidx = 0 ; mosaicBandidx <
            mosaicRasterHandler->getNumberOfBands() ;  ++mosaicBandidx )
          {
            te::rst::GetDataTypeRanges( 
              mosaicRasterHandler->getBand( mosaicBandidx )->getProperty()->m_type,
              mosaicBandsRangeMin[ mosaicBandidx ],
              mosaicBandsRangeMax[ mosaicBandidx ]);             
          }          
          
          lastInputRasterBBoxLLXIndexed = 0;
          lastInputRasterBBoxLLYIndexed = inputRasterPtr->getNumberOfRows() - 1;
          lastInputRasterBBoxURXIndexed = inputRasterPtr->getNumberOfColumns() - 1;
          lastInputRasterBBoxURYIndexed = 0;              
          
          const double& mosaicLLX = mosaicRasterHandler->getGrid()->getExtent()->m_llx;
          const double& mosaicLLY = mosaicRasterHandler->getGrid()->getExtent()->m_lly;
          const double& mosaicURX = mosaicRasterHandler->getGrid()->getExtent()->m_urx;
          const double& mosaicURY = mosaicRasterHandler->getGrid()->getExtent()->m_ury;        
          
          te::gm::LinearRing* auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
          auxLinearRingPtr->setPoint( 0, mosaicLLX, mosaicURY );
          auxLinearRingPtr->setPoint( 1, mosaicURX, mosaicURY );
          auxLinearRingPtr->setPoint( 2, mosaicURX, mosaicLLY );
          auxLinearRingPtr->setPoint( 3, mosaicLLX, mosaicLLY );
          auxLinearRingPtr->setPoint( 4, mosaicLLX, mosaicURY );             
          auxLinearRingPtr->setSRID( mosaicRasterHandler->getGrid()->getSRID() );
          
          te::gm::Polygon* outPolPtr = new te::gm::Polygon( 0, te::gm::PolygonType, 
            mosaicRasterHandler->getGrid()->getSRID(), 0 ); 
          outPolPtr->add( auxLinearRingPtr ); 
          
          mosaicValidAreaPols.clear();
          mosaicValidAreaPols.add( outPolPtr );
          mosaicValidAreaPols.setSRID( mosaicRasterHandler->getGrid()->getSRID() );

          // copying the raster data 
            
          const unsigned int nBands = mosaicRasterHandler->getNumberOfBands();
          
          mosaicTargetMeans.resize( nBands, 0.0 );
          mosaicTargetVariances.resize( nBands, 0.0 );        
            
          for( unsigned int inputRastersBandsIdx = 0 ; inputRastersBandsIdx <
            nBands ; ++inputRastersBandsIdx )
          {
            const unsigned int inputBandIdx =  
              m_inputParameters.m_inputRastersBands[ inputRasterIdx ][ inputRastersBandsIdx ] ;
            const double& inputBandNoDataValue = m_inputParameters.m_forceInputNoDataValue ?
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

                if( pixelValue != inputBandNoDataValue )
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

                  if( pixelValue != inputBandNoDataValue )
                  {
                    variance += ( ( pixelValue - mean ) * ( pixelValue -
                      mean ) ) / ( (double)validPixelsNumber );
                  }
                }
              }
            }
          }
          
          // Updating the mosaic sequence info
          
          currentMosaicSquenceInfo.m_rasterFeederIndexes.push_back(
            inputRasterIdx );
          currentMosaicSquenceInfo.m_tiePoints.push_back( 
            std::vector< te::gm::GTParameters::TiePoint >() );
          
          // Move to the next raster
          
          cachedInputRasterPtr.reset();
          
          m_inputParameters.m_feederRasterPtr->moveNext();          
          
          if( m_inputParameters.m_enableProgress )
          {
            progressPtr->pulse();
            if( ! progressPtr->isActive() ) return false;
          }            
        }
        else
        {
          // Locating the tie points where te::gm::GTParameters::TiePoint::first
          // are :mosaic indexed coods and te::gm::GTParameters::TiePoint::second are
          // indexed coords from the current raster
          
          te::rp::TiePointsLocator::OutputParameters locatorOutParams;
          
          {
            te::rp::TiePointsLocator::InputParameters locatorInParams;
            locatorInParams = m_inputParameters.m_locatorParams;
            
            locatorInParams.m_inRaster1Ptr = mosaicRasterHandler.get();
            
            locatorInParams.m_inMaskRaster1Ptr = 0;
            
            locatorInParams.m_inRaster1Bands.clear();
            locatorInParams.m_inRaster1Bands.push_back( 
              m_inputParameters.m_tiePointsLocationBandIndex );

            locatorInParams.m_raster1TargetAreaColStart = lastInputRasterBBoxLLXIndexed;
            locatorInParams.m_raster1TargetAreaLineStart = lastInputRasterBBoxURYIndexed;
            locatorInParams.m_raster1TargetAreaWidth = (unsigned int)( 
              lastInputRasterBBoxURXIndexed - lastInputRasterBBoxLLXIndexed + 1 );
            locatorInParams.m_raster1TargetAreaHeight = (unsigned int)( 
              lastInputRasterBBoxLLYIndexed - lastInputRasterBBoxURYIndexed + 1 );
              
            locatorInParams.m_inRaster2Ptr = inputRasterPtr;
            
            locatorInParams.m_inMaskRaster2Ptr = 0;
            
            locatorInParams.m_inRaster2Bands.clear();
            locatorInParams.m_inRaster2Bands.push_back( 
              m_inputParameters.m_inputRastersBands[ inputRasterIdx ][ 
              m_inputParameters.m_tiePointsLocationBandIndex ] );
              
            locatorInParams.m_raster2TargetAreaLineStart = 0;
            locatorInParams.m_raster2TargetAreaColStart = 0;
            locatorInParams.m_raster2TargetAreaWidth = 0;
            locatorInParams.m_raster2TargetAreaHeight = 0;
              
            locatorInParams.m_enableMultiThread = m_inputParameters.m_enableMultiThread;

            locatorInParams.m_enableProgress = false;
            
            locatorInParams.m_pixelSizeXRelation = mosaicRasterHandler->getResolutionX() /
              inputRasterPtr->getResolutionX();
            locatorInParams.m_pixelSizeYRelation = mosaicRasterHandler->getResolutionY() /
              inputRasterPtr->getResolutionY();
              
            locatorInParams.m_geomTransfName = m_inputParameters.m_geomTransfName;
              
            locatorInParams.m_enableGeometryFilter = true;
            
            locatorInParams.m_interpMethod = m_inputParameters.m_interpMethod;
            
            te::rp::TiePointsLocator locatorInstance;
            TERP_TRUE_OR_RETURN_FALSE( locatorInstance.initialize( 
              locatorInParams ), "Tie points locator init error" );
            TERP_TRUE_OR_RETURN_FALSE( locatorInstance.execute( 
              locatorOutParams ), "Tie points locator exec error" );
          }
          
          // The matching was accomplished successfully ?
        
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
                (
                  m_inputParameters.m_minRequiredTiePointsCoveredAreaPercent 
                  /
                  100.0
                )
              ) 
             )
          {
            // Saving the mosaic sequence info
            
            currentMosaicSquenceInfo.m_dataSetName = m_inputParameters.m_outDataSetsNamePrefix + 
              boost::lexical_cast< std::string >( outParamsPtr->m_sequencesInfo.size() ) +
              m_inputParameters.m_outDataSetsNameSufix;            
              
            outParamsPtr->m_sequencesInfo.push_back( currentMosaicSquenceInfo );
            
            // Create the output data set
            
            assert( mosaicRasterHandler.get() );
            TERP_TRUE_OR_RETURN_FALSE( createRasterDataSet( 
              currentMosaicSquenceInfo.m_dataSetName,
              *( mosaicRasterHandler.get() ), outParamsPtr->m_outputDSPtr ),
              "Data set creation error" );
            mosaicRasterHandler.reset();
            
            currentMosaicSquenceInfo.clear();
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
            
            // Updating the mosaic sequence info
            
            currentMosaicSquenceInfo.m_tiePoints.push_back( 
              locatorOutParams.m_tiePoints );            
            currentMosaicSquenceInfo.m_rasterFeederIndexes.push_back( 
              inputRasterIdx );
            
            // Expanding the mosaic to fit the extent requirement allowing the 
            // merge of the current raster.
            // Fiding the new geometric transformation between then.
            
            std::auto_ptr< te::gm::GeometricTransformation > geoTransPtr(
              locatorOutParams.m_transformationPtr->clone() );
              
            {
              const double oldLLYIndexed = 
                (double)( inputRasterPtr->getNumberOfRows() - 1 );
              const double oldURXIndexed =
                (double)( inputRasterPtr->getNumberOfColumns() - 1 );
              
              double mappedLLX = 0;
              double mappedLLY = 0;
              double mappedLRX = 0;
              double mappedLRY = 0;
              double mappedURX = 0;
              double mappedURY = 0;
              double mappedULX = 0;
              double mappedULY = 0;
              locatorOutParams.m_transformationPtr->inverseMap( 
                0.0,
                oldLLYIndexed,
                mappedLLX,
                mappedLLY);
              locatorOutParams.m_transformationPtr->inverseMap( 
                oldURXIndexed,
                oldLLYIndexed,
                mappedLRX,
                mappedLRY);                
              locatorOutParams.m_transformationPtr->inverseMap( 
                oldURXIndexed,
                0.0,
                mappedURX,
                mappedURY);                
              locatorOutParams.m_transformationPtr->inverseMap( 
                0.0,
                0.0,
                mappedULX,
                mappedULY);                
              
              const double newLLXIndexed = std::min( mappedLLX, std::min( mappedLRX, std::min( mappedURX, mappedULX ) ) );
              const double newLLYIndexed = std::max( mappedLLY, std::max( mappedLRY, std::max( mappedURY, mappedULY ) ) );
              const double newURXIndexed = std::max( mappedLLX, std::max( mappedLRX, std::max( mappedURX, mappedULX ) ) );
              const double newURYIndexed = std::min( mappedLLY, std::min( mappedLRY, std::min( mappedURY, mappedULY ) ) );
              
              double oldMosaicReferenceX = 0;
              double oldMosaicReferenceY = 0;
              mosaicRasterHandler->getGrid()->gridToGeo( 0.0, 0.0, oldMosaicReferenceX,
                oldMosaicReferenceY );
              
              double mosaicReferenceHasChanged = false;
              
              
              if( newLLXIndexed < 0.0 )
              {
                TERP_TRUE_OR_RETURN_FALSE( mosaicRasterHandler->addLeftColumns( 
                  (unsigned int)std::ceil( -1.0 * newLLXIndexed ) ),
                  "Mosaic expansion error" );
                mosaicReferenceHasChanged = true;
              }
              
              if( newURYIndexed < 0.0 )
              {
                TERP_TRUE_OR_RETURN_FALSE( mosaicRasterHandler->addTopLines( 
                  (unsigned int)std::ceil( -1.0 * newURYIndexed ) ),
                  "Mosaic expansion error" );
                mosaicReferenceHasChanged = true;
              }      
              
              if( newURXIndexed > ((double)(mosaicRasterHandler->getNumberOfColumns() - 1) ) )
              {
                TERP_TRUE_OR_RETURN_FALSE( mosaicRasterHandler->addRightColumns( 
                  (unsigned int)std::ceil( newURXIndexed - 
                  ((double)(mosaicRasterHandler->getNumberOfColumns() - 1)) ) ),
                  "Mosaic expansion error" );
              }
              
              if( newLLYIndexed > ((double)(mosaicRasterHandler->getNumberOfRows() - 1) ) )
              {
                TERP_TRUE_OR_RETURN_FALSE( mosaicRasterHandler->addBottomLines( 
                  (unsigned int)std::ceil( newLLYIndexed - 
                  ((double)(mosaicRasterHandler->getNumberOfRows() - 1) ) ) ),
                  "Mosaic expansion error" );
              }              
              
              // updating the geometric transformation between both rasters
              
              if( mosaicReferenceHasChanged )
              {
                double expansionOffSetX = 0;
                double expansionOffSetY = 0;
                mosaicRasterHandler->getGrid()->geoToGrid( oldMosaicReferenceX,
                  oldMosaicReferenceY, expansionOffSetX, expansionOffSetY );
                  
                te::gm::GTParameters newTransParams;
                newTransParams.m_tiePoints = 
                  geoTransPtr->getParameters().m_tiePoints;
                
                for( unsigned int tpIdx = 0 ; tpIdx < 
                  newTransParams.m_tiePoints.size() ; ++tpIdx )
                {
                  newTransParams.m_tiePoints[ tpIdx ].first.x += expansionOffSetX;
                  newTransParams.m_tiePoints[ tpIdx ].first.y += expansionOffSetY;  
                }  
                
                TERP_TRUE_OR_RETURN_FALSE( geoTransPtr->initialize( newTransParams ),
                  "Geometric transformation parameters calcule error" );
              }            
            }
            
            // Locating the current raster over the expanded mosaic
            
            double inputRasterMappedLLXIndexed = 0;
            double inputRasterMappedLLYIndexed = 0;
            double inputRasterMappedLRXIndexed = 0;
            double inputRasterMappedLRYIndexed = 0;
            double inputRasterMappedURXIndexed = 0;
            double inputRasterMappedURYIndexed = 0;
            double inputRasterMappedULXIndexed = 0;
            double inputRasterMappedULYIndexed = 0;            
            {
              const double lastRowIdx = 
                (double)( inputRasterPtr->getNumberOfRows() - 1 );
              const double lastColIdx =
                (double)( inputRasterPtr->getNumberOfColumns() - 1 );

              geoTransPtr->inverseMap( 
                0.0,
                lastRowIdx,
                inputRasterMappedLLXIndexed,
                inputRasterMappedLLYIndexed);
              geoTransPtr->inverseMap( 
                lastColIdx,
                lastRowIdx,
                inputRasterMappedLRXIndexed,
                inputRasterMappedLRYIndexed);                
              geoTransPtr->inverseMap( 
                lastColIdx,
                0.0,
                inputRasterMappedURXIndexed,
                inputRasterMappedURYIndexed); 
              geoTransPtr->inverseMap( 
                0.0,
                0.0,
                inputRasterMappedULXIndexed,
                inputRasterMappedULYIndexed);  
              
              lastInputRasterBBoxLLXIndexed = (unsigned int)
                std::max( 0.0,
                  std::min( (double)( mosaicRasterHandler->getNumberOfColumns() - 1 ),              
                    std::floor( 
                      std::min( inputRasterMappedLLXIndexed, 
                        std::min( inputRasterMappedLRXIndexed, 
                          std::min( inputRasterMappedURXIndexed, 
                                      inputRasterMappedULXIndexed
                          ) 
                        ) 
                      )
                    )
                  )
                );
                
              lastInputRasterBBoxLLYIndexed = (unsigned int)
                std::max( 0.0,
                  std::min( (double)( mosaicRasterHandler->getNumberOfRows() - 1 ),                
                    std::ceil( 
                      std::max( inputRasterMappedLLYIndexed, 
                        std::max( inputRasterMappedLRYIndexed, 
                          std::max( inputRasterMappedURYIndexed, 
                                      inputRasterMappedULYIndexed
                          ) 
                        ) 
                      ) 
                    )
                  )
                );
                
              lastInputRasterBBoxURXIndexed = (unsigned int)
                std::max( 0.0,
                  std::min( (double)( mosaicRasterHandler->getNumberOfColumns() - 1 ),                
                    std::ceil( 
                      std::max( inputRasterMappedLLXIndexed, 
                        std::max( inputRasterMappedLRXIndexed, 
                          std::max( inputRasterMappedURXIndexed, 
                                      inputRasterMappedULXIndexed
                          ) 
                        ) 
                      )
                    )
                  )
                );
                
              lastInputRasterBBoxURYIndexed = (unsigned int)
                std::max( 0.0,
                  std::min( (double)( mosaicRasterHandler->getNumberOfRows() - 1 ),               
                    std::floor( 
                      std::min( inputRasterMappedLLYIndexed, 
                        std::min( inputRasterMappedLRYIndexed, 
                          std::min( inputRasterMappedURYIndexed, 
                                      inputRasterMappedULYIndexed
                          ) 
                        ) 
                      ) 
                    )
                  )
                );
                
              assert( lastInputRasterBBoxLLXIndexed >= 0 );
              assert( lastInputRasterBBoxLLXIndexed <= 
                ( mosaicRasterHandler->getNumberOfColumns() - 1 ) );    
              assert( lastInputRasterBBoxLLYIndexed >= 0 );
              assert( lastInputRasterBBoxLLYIndexed <= 
                ( mosaicRasterHandler->getNumberOfRows() - 1 ) );                 
              assert( lastInputRasterBBoxURXIndexed >= 0 );
              assert( lastInputRasterBBoxURXIndexed <= 
                ( mosaicRasterHandler->getNumberOfColumns() - 1 ) ); 
              assert( lastInputRasterBBoxURYIndexed >= 0 );
              assert( lastInputRasterBBoxURYIndexed <= 
                ( mosaicRasterHandler->getNumberOfRows() - 1 ) );
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
                &mosaicValidAreaPols,
                0,                
                *geoTransPtr,
                m_inputParameters.m_enableMultiThread ? 0 : 1,
                false ),
                "Blender initiazing error" );   
              
              TERP_TRUE_OR_RETURN_FALSE( blenderInstance.blendIntoRaster1(), 
                 "Error blending images" );
            }
            
            // updating the mosaic related polygons
            
            {
              
              double inputRasterMappedLLX = 0;
              double inputRasterMappedLLY = 0;
              mosaicRasterHandler->getGrid()->gridToGeo( 
                (double)inputRasterMappedLLXIndexed, (double)inputRasterMappedLLYIndexed,
                inputRasterMappedLLX, inputRasterMappedLLY );              
              
              double inputRasterMappedLRX = 0;
              double inputRasterMappedLRY = 0;
              mosaicRasterHandler->getGrid()->gridToGeo( 
                (double)inputRasterMappedLRXIndexed, (double)inputRasterMappedLRYIndexed,
                inputRasterMappedLRX, inputRasterMappedLRY );
                
              double inputRasterMappedURX = 0;
              double inputRasterMappedURY = 0;
              mosaicRasterHandler->getGrid()->gridToGeo( 
                (double)inputRasterMappedURXIndexed, (double)inputRasterMappedURYIndexed,
                inputRasterMappedURX, inputRasterMappedURY );                
                
              double inputRasterMappedULX = 0;
              double inputRasterMappedULY = 0;
              mosaicRasterHandler->getGrid()->gridToGeo( 
                (double)inputRasterMappedULXIndexed, (double)inputRasterMappedULYIndexed,
                inputRasterMappedULX, inputRasterMappedULY );                  
              
              te::gm::LinearRing* auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
              auxLinearRingPtr->setPoint( 0, inputRasterMappedULX, inputRasterMappedULY );
              auxLinearRingPtr->setPoint( 1, inputRasterMappedURX, inputRasterMappedURY );
              auxLinearRingPtr->setPoint( 2, inputRasterMappedLRX, inputRasterMappedLRY );
              auxLinearRingPtr->setPoint( 3, inputRasterMappedLLX, inputRasterMappedLLY );
              auxLinearRingPtr->setPoint( 4, inputRasterMappedULX, inputRasterMappedULY );
              auxLinearRingPtr->setSRID( mosaicRasterHandler->getSRID() );
              te::gm::Polygon lastMosaicAddedRasterPol(  0, te::gm::PolygonType, 0 ); //the polygon of the last added raster (mosaic world coords)
              lastMosaicAddedRasterPol.push_back( auxLinearRingPtr );
              lastMosaicAddedRasterPol.setSRID( mosaicRasterHandler->getSRID() );
              
              // union of the current raster box with the current mosaic valid area under the mosaic SRID
              
              std::auto_ptr< te::gm::Geometry > unionMultiPolPtr(
                mosaicValidAreaPols.Union( &lastMosaicAddedRasterPol ) );
              TERP_TRUE_OR_THROW( unionMultiPolPtr.get(), "Invalid pointer" );
              unionMultiPolPtr->setSRID( mosaicRasterHandler->getSRID() );
              
              if( unionMultiPolPtr->getGeomTypeId() == te::gm::MultiPolygonType )
              {
                mosaicValidAreaPols = *( (te::gm::MultiPolygon*)unionMultiPolPtr.get() );
              }
              else if( unionMultiPolPtr->getGeomTypeId() == te::gm::PolygonType )
              {
                mosaicValidAreaPols.clear();
                mosaicValidAreaPols.setSRID( unionMultiPolPtr->getSRID() );
                mosaicValidAreaPols.add( (te::gm::Polygon*)unionMultiPolPtr.release() );
              }
              else
              {
                TERP_LOG_AND_RETURN_FALSE( "Invalid union geometry type" );
              }
            }       
            
            // Move to the next raster
            
            cachedInputRasterPtr.reset();
            
            m_inputParameters.m_feederRasterPtr->moveNext();  
            
            if( m_inputParameters.m_enableProgress )
            {
              progressPtr->pulse();
              if( ! progressPtr->isActive() ) return false;
            }             
          }
        }
      }
      
      if( mosaicRasterHandler.get() )
      {
        // Saving the mosaic sequence info
        
        currentMosaicSquenceInfo.m_dataSetName = m_inputParameters.m_outDataSetsNamePrefix + 
          boost::lexical_cast< std::string >(  outParamsPtr->m_sequencesInfo.size() ) +
          m_inputParameters.m_outDataSetsNameSufix;            
          
        outParamsPtr->m_sequencesInfo.push_back( currentMosaicSquenceInfo );        
        
        TERP_TRUE_OR_RETURN_FALSE( createRasterDataSet( 
          currentMosaicSquenceInfo.m_dataSetName,
          *( mosaicRasterHandler.get() ), outParamsPtr->m_outputDSPtr ),
          "Data set creation error" );
          
        currentMosaicSquenceInfo.clear();
      }
      
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
      if( dataSetName.empty() ) return false;
      if( ! ( sourceRaster.getAccessPolicy() & te::common::RAccess ) ) return false;
      if( dataSourcePtr == 0 ) return false;
      if( ! dataSourcePtr->isValid() ) return false;
      
      const unsigned int nRows = sourceRaster.getNumberOfRows();
      const unsigned int nCols = sourceRaster.getNumberOfColumns();
      const unsigned int nBands = sourceRaster.getNumberOfBands();
      
      std::vector< te::rst::BandProperty* > bandsProperties;
      unsigned int bandIdx = 0;
      for( bandIdx = 0 ; bandIdx < nBands ; ++bandIdx )
      {
        bandsProperties.push_back( new te::rst::BandProperty( 
          *( sourceRaster.getBand( bandIdx )->getProperty()) ) );
      }
      
      te::rp::RasterHandler outRasterHandler;
      if( ! te::rp::CreateNewRaster( *( sourceRaster.getGrid() ),
        bandsProperties, dataSetName, *dataSourcePtr, outRasterHandler) )
        return false;      
      
      unsigned int col = 0;
      unsigned int row = 0;
      double value = 0;
      
      for( bandIdx = 0 ; bandIdx < nBands ; ++bandIdx )
      {
        const te::rst::Band& inBand = *(sourceRaster.getBand( bandIdx ));
        te::rst::Band& outBand = *(outRasterHandler.getRasterPtr()->getBand( bandIdx ));
        
        for( row = 0 ; row < nRows ; ++row )
        {
          for( col = 0 ; col < nCols ; ++col )
          {
            inBand.getValue( col, row, value );
            outBand.setValue( col, row, value );
          }
        }
      }
      
      return true;
    }
    
    bool SequenceMosaic::createDiskRasterCopy( const std::string& fileName,
       const te::rst::Raster& sourceRaster ) const
    {
      std::map<std::string, std::string> rInfo;
      rInfo["URI"] = fileName;
      
      const unsigned int nBands = sourceRaster.getNumberOfBands();
      
      std::vector<te::rst::BandProperty*> bandsProperties;
      unsigned int bandIdx = 0;
      for( bandIdx = 0 ; bandIdx < nBands ; ++bandIdx )
        bandsProperties.push_back(new te::rst::BandProperty( 
          *sourceRaster.getBand( bandIdx )->getProperty() ) );

      te::rst::Grid* newgrid = new te::rst::Grid( *sourceRaster.getGrid() );

      std::auto_ptr< te::rst::Raster > outputRasterPtr(
        te::rst::RasterFactory::make( "GDAL", newgrid, bandsProperties, rInfo, 0, 0));
      if( outputRasterPtr.get() == 0 ) return false;
          
      unsigned int line = 0;
      unsigned int col = 0;
      const unsigned int nLines = sourceRaster.getNumberOfRows();
      const unsigned int nCols = sourceRaster.getNumberOfColumns();

      double value = 0;
      
      for( bandIdx = 0 ; bandIdx < nBands ; ++bandIdx )
      {
        const te::rst::Band& inBand = *sourceRaster.getBand( bandIdx );
        te::rst::Band& outBand = *outputRasterPtr->getBand( bandIdx );
        
        for( line = 0 ; line < nLines ; ++line )
          for( col = 0 ; col < nCols ; ++col )
          {
            inBand.getValue( col, line, value );
            outBand.setValue( col, line, value );
          }          
      }
      
      return true;
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
        
        std::auto_ptr< te::gm::Geometry > convexHullPolPtr( points.convexHull() );
        
        if( dynamic_cast< te::gm::Surface* >( convexHullPolPtr.get() ) )
        {
          return dynamic_cast< te::gm::Surface* >( convexHullPolPtr.get() )->getArea();
        }
        else
        {
          return 0.0;
        }
      }
    }

  } // end namespace rp
}   // end namespace te

