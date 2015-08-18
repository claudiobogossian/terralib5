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
      m_interpMethod = te::rst::NearestNeighbor;
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
      m_minRequiredTiePointsCoveredAreaPercent = 25.0;
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
      unsigned int prevRasterMosaicFirstCol = 0; // The last raster added to the mosaic position
      unsigned int prevRasterMosaicFirstRow = 0; // The last raster added to the mosaic position
      unsigned int prevRasterMosaicLastCol = 0; // The last raster added to the mosaic position
      unsigned int prevRasterMosaicLastRow = 0; // The last raster added to the mosaic position
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
          
          prevRasterMosaicFirstCol = 0;
          prevRasterMosaicLastRow = inputRasterPtr->getNumberOfRows() - 1;
          prevRasterMosaicLastCol = inputRasterPtr->getNumberOfColumns() - 1;
          prevRasterMosaicFirstRow = 0;              
          
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

          // fill the output mosaic raster 
            
          {
            const unsigned int nBands = mosaicRasterHandler->getNumberOfBands();
            const unsigned int inputRasterNRows = inputRasterPtr->getNumberOfRows();
            const unsigned int inputRasterNCols = inputRasterPtr->getNumberOfColumns();            
            const unsigned int mosaicNRows = mosaicRasterHandler->getNumberOfRows();
            const unsigned int mosaicNCols = mosaicRasterHandler->getNumberOfColumns();              
            unsigned long int validPixelsNumber = 0;
            unsigned int outCol = 0;
            unsigned int outRow = 0;
            double pixelValue = 0;            
            
            mosaicTargetMeans.resize( nBands, 0.0 );
            mosaicTargetVariances.resize( nBands, 0.0 );        
              
            for( unsigned int inputRastersBandsIdx = 0 ; inputRastersBandsIdx <
              nBands ; ++inputRastersBandsIdx )
            {
              const unsigned int inputBandIdx =  
                m_inputParameters.m_inputRastersBands[ inputRasterIdx ][ inputRastersBandsIdx ] ;
              te::rst::Band& outBand =
                (*mosaicRasterHandler->getBand( inputRastersBandsIdx ));
              const te::rst::Band& inBand =
                (*inputRasterPtr->getBand( inputBandIdx ));                
              const double& inputBandNoDataValue = m_inputParameters.m_forceInputNoDataValue ?
                m_inputParameters.m_noDataValue : inputRasterPtr->getBand( inputBandIdx
                )->getProperty()->m_noDataValue;
              const double& outputBandNoDataValue = outBand.getProperty()->m_noDataValue;
              
              double mean = 0.0;

              for( outRow = 0 ; outRow < mosaicNRows ; ++outRow )
              {
                for( outCol = 0 ; outCol < mosaicNCols ; ++outCol )
                {
                  if( ( outRow < inputRasterNRows ) && ( outCol < inputRasterNCols ) )
                  {
                    inBand.getValue( outCol, outRow, pixelValue );

                    if( pixelValue != inputBandNoDataValue )
                    {
                      outBand.setValue( outCol, outRow, pixelValue );
                      
                      mean += pixelValue;
                      ++validPixelsNumber;
                    }
                  }
                  else
                  {
                    outBand.setValue( outCol, outRow, outputBandNoDataValue );
                  }
                }
              }

              // variance calcule

              if( m_inputParameters.m_autoEqualize && ( validPixelsNumber > 0 ) )
              {
                mean /= ( (double)validPixelsNumber );
                mosaicTargetMeans[ inputRastersBandsIdx ] = mean;
                
                double& variance = mosaicTargetVariances[ inputRastersBandsIdx ];
                variance = 0;

                double pixelValue = 0;

                for( outRow = 0 ; outRow < inputRasterNRows ; ++outRow )
                {
                  for( outCol = 0 ; outCol < inputRasterNCols ; ++outCol )
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

            locatorInParams.m_raster1TargetAreaColStart = prevRasterMosaicFirstCol;
            locatorInParams.m_raster1TargetAreaLineStart = prevRasterMosaicFirstRow;
            locatorInParams.m_raster1TargetAreaWidth = (unsigned int)( 
              prevRasterMosaicLastCol - prevRasterMosaicFirstCol + 1 );
            locatorInParams.m_raster1TargetAreaHeight = (unsigned int)( 
              prevRasterMosaicLastRow - prevRasterMosaicFirstRow + 1 );
              
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
            
            if( mosaicRasterHandler->getSRID() == inputRasterPtr->getSRID() )
            {
              locatorInParams.m_pixelSizeXRelation = mosaicRasterHandler->getResolutionX() /
                inputRasterPtr->getResolutionX();
              locatorInParams.m_pixelSizeYRelation = mosaicRasterHandler->getResolutionY() /
                inputRasterPtr->getResolutionY();              
            }
            else
            {
              te::gm::Envelope inputRasterExtent( *inputRasterPtr->getGrid()->getExtent() );
              inputRasterExtent.transform( inputRasterPtr->getSRID(),
                mosaicRasterHandler->getSRID() );
              
              locatorInParams.m_pixelSizeXRelation = mosaicRasterHandler->getResolutionX() /
                ( inputRasterExtent.getWidth() / ((double)inputRasterPtr->getNumberOfColumns()) );
              locatorInParams.m_pixelSizeYRelation = mosaicRasterHandler->getResolutionY() /
                ( inputRasterExtent.getHeight() / ((double)inputRasterPtr->getNumberOfRows()) );                
            }
              
            locatorInParams.m_geomTransfName = m_inputParameters.m_geomTransfName;
              
            locatorInParams.m_enableGeometryFilter = true;
            
            locatorInParams.m_interpMethod = m_inputParameters.m_interpMethod;
            
            te::rp::TiePointsLocator locatorInstance;
            TERP_TRUE_OR_RETURN_FALSE( locatorInstance.initialize( 
              locatorInParams ), "Tie points locator init error" );
            if( ! locatorInstance.execute( locatorOutParams ) )
            {
              locatorInParams.reset();
            }
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
                  te::rp::GetTPConvexHullArea( locatorOutParams.m_tiePoints, true ) 
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

            std::vector< double > currentRasterBandsOffsets = dummyRasterOffsets;
            std::vector< double > currentRasterBandsScales = dummyRasterScales;

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
                  
                if( 
                    ( mosaicTargetMeans[ inputRastersBandsIdx ] != 0.0 )
                    &&
                    ( mosaicTargetVariances[ inputRastersBandsIdx ] != 0.0 )
                  )
                {                  
                  calcBandStatistics( (*inputRasterPtr->getBand( inputBandIdx ) ),
                    m_inputParameters.m_forceInputNoDataValue,
                    m_inputParameters.m_noDataValue,
                    currentRasterMean,
                    currentRasterVariance );
                  
                  currentRasterBandsScales[ inputRastersBandsIdx ] = 
                    ( 
                      std::sqrt( mosaicTargetVariances[ inputRastersBandsIdx ] )
                       /
                      std::sqrt( currentRasterVariance )
                    );
                  currentRasterBandsOffsets[ inputRastersBandsIdx ] =
                    ( 
                      mosaicTargetMeans[ inputRastersBandsIdx ]
                      -
                      ( 
                        currentRasterBandsScales[ inputRastersBandsIdx ] 
                        * 
                        currentRasterMean 
                      ) 
                    );
                }
              }
            }
            
            // Updating the mosaic sequence info
            
            currentMosaicSquenceInfo.m_tiePoints.push_back( 
              locatorOutParams.m_tiePoints );            
            currentMosaicSquenceInfo.m_rasterFeederIndexes.push_back( 
              inputRasterIdx );
            
            // The indexed detailed extent of input raster
            
            te::gm::LinearRing inRasterIndexedDetailedExtent( te::gm::LineStringType, 0, 0 );
            TERP_TRUE_OR_RETURN_FALSE( te::rp::GetIndexedDetailedExtent(
              *inputRasterPtr->getGrid(), inRasterIndexedDetailedExtent ),
              "Error creating the raster detailed extent" );            
            
            // Expanding the mosaic to fit the extent requirement allowing the 
            // merge of the current raster.
            // Fiding the new geometric transformation between then.
            
            std::auto_ptr< te::gm::GeometricTransformation > geoTransPtr(
              locatorOutParams.m_transformationPtr->clone() );
              
            {
              // guessing the limits of the new blended raster over the old mosaic
              
              double mappedLRX = std::numeric_limits< double >::min();
              double mappedLRY = std::numeric_limits< double >::min();
              double mappedULX = std::numeric_limits< double >::max();
              double mappedULY = std::numeric_limits< double >::max();
              double mappedX = 0;
              double mappedY = 0;
              for( unsigned int inRasterDetExtentIdx = 0 ; inRasterDetExtentIdx <
                inRasterIndexedDetailedExtent.size() ; ++inRasterDetExtentIdx )
              {
                locatorOutParams.m_transformationPtr->inverseMap( 
                  inRasterIndexedDetailedExtent.getX( inRasterDetExtentIdx ),
                  inRasterIndexedDetailedExtent.getY( inRasterDetExtentIdx ),
                  mappedX,
                  mappedY);
                
                if( mappedLRX < mappedX ) mappedLRX = mappedX;
                if( mappedLRY < mappedY ) mappedLRY = mappedY;
                if( mappedULX > mappedX ) mappedULX = mappedX;
                if( mappedULY > mappedY ) mappedULY = mappedY;
              }
              
              // calc of old reference for future use
              
              double oldMosaicReferenceX = 0;
              double oldMosaicReferenceY = 0;
              mosaicRasterHandler->getGrid()->gridToGeo( 0.0, 0.0, oldMosaicReferenceX,
                oldMosaicReferenceY );
              
              // expanding
              
              double mosaicReferenceHasChanged = false;              
              
              if( mappedULX < 0.0 )
              {
                TERP_TRUE_OR_RETURN_FALSE( mosaicRasterHandler->addLeftColumns( 
                  (unsigned int)std::ceil( -1.0 * mappedULX ) ),
                  "Mosaic expansion error" );
                mosaicReferenceHasChanged = true;
              }
              
              if( mappedULY < 0.0 )
              {
                TERP_TRUE_OR_RETURN_FALSE( mosaicRasterHandler->addTopLines( 
                  (unsigned int)std::ceil( -1.0 * mappedULY ) ),
                  "Mosaic expansion error" );
                mosaicReferenceHasChanged = true;
              }      
              
              if( mappedLRX > ((double)(mosaicRasterHandler->getNumberOfColumns() - 1) ) )
              {
                TERP_TRUE_OR_RETURN_FALSE( mosaicRasterHandler->addRightColumns( 
                  (unsigned int)std::ceil( mappedLRX - 
                  ((double)(mosaicRasterHandler->getNumberOfColumns() - 1)) ) ),
                  "Mosaic expansion error" );
              }
              
              if( mappedLRY > ((double)(mosaicRasterHandler->getNumberOfRows() - 1) ) )
              {
                TERP_TRUE_OR_RETURN_FALSE( mosaicRasterHandler->addBottomLines( 
                  (unsigned int)std::ceil( mappedLRY - 
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
            
            // Blending the current raster into the mosaic
        
            {
              te::rp::Blender blenderInstance;
              
              TERP_TRUE_OR_RETURN_FALSE( blenderInstance.initialize(
                *( mosaicRasterHandler.get() ),
                dummyRasterBandsIndexes,              
                *inputRasterPtr,
                m_inputParameters.m_inputRastersBands[ inputRasterIdx ],
                m_inputParameters.m_blendMethod,
                te::rst::NearestNeighbor,
                m_inputParameters.m_interpMethod,
                m_inputParameters.m_noDataValue,
                false,
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
            
            // The input rasters detailed extent over the expanded mosaic
            
            te::gm::LinearRing inRasterDetailedExtent( 
                inRasterIndexedDetailedExtent.size(), te::gm::LineStringType,
                mosaicRasterHandler->getSRID(), (te::gm::Envelope*)0 );
            
            {
              double mappedX = 0;
              double mappedY = 0;              

              for( unsigned int inRasterDetExtentIdx = 0 ; inRasterDetExtentIdx <
                inRasterIndexedDetailedExtent.size() ; ++inRasterDetExtentIdx )
              {
                locatorOutParams.m_transformationPtr->inverseMap( 
                  inRasterIndexedDetailedExtent.getX( inRasterDetExtentIdx ),
                  inRasterIndexedDetailedExtent.getY( inRasterDetExtentIdx ),
                  mappedX,
                  mappedY);
                
                inRasterDetailedExtent.setPoint( inRasterDetExtentIdx, mappedX, mappedY );
              }            
            }
            
            // Updating the info about the position of the last blended raster
            
            {
              prevRasterMosaicLastCol = 0;
              prevRasterMosaicLastRow = 0;
              prevRasterMosaicFirstCol = std::numeric_limits< unsigned int >::max();
              prevRasterMosaicFirstRow = std::numeric_limits< unsigned int >::max();

              double mappedX = 0;
              double mappedY = 0;
              for( unsigned int inRasterDetExtentIdx = 0 ; inRasterDetExtentIdx <
                inRasterDetailedExtent.size() ; ++inRasterDetExtentIdx )
              {
                geoTransPtr->inverseMap( 
                  inRasterDetailedExtent.getX( inRasterDetExtentIdx ),
                  inRasterDetailedExtent.getY( inRasterDetExtentIdx ),
                  mappedX,
                  mappedY);
                
                mappedX = std::max( mappedX, 0.0 );
                mappedX = std::min( mappedX, 
                  (double)( mosaicRasterHandler->getNumberOfColumns() - 1 ) );

                mappedY = std::max( mappedY, 0.0 );
                mappedY = std::min( mappedY, 
                  (double)( mosaicRasterHandler->getNumberOfRows() - 1 ) );                
                
                if( prevRasterMosaicLastCol < mappedX ) prevRasterMosaicLastCol = mappedX;
                if( prevRasterMosaicLastRow < mappedY ) prevRasterMosaicLastRow = mappedY;
                if( prevRasterMosaicFirstCol > mappedX ) prevRasterMosaicFirstCol = mappedX;
                if( prevRasterMosaicFirstRow > mappedY ) prevRasterMosaicFirstRow = mappedY;
              }              
            }            
            
            // updating the mosaic related polygons
            
            {
              // building a polygon of the blended raster over the mosaic (world coods)
              
              te::gm::Polygon lastMosaicAddedRasterPol(  0, te::gm::PolygonType, 0 ); //the polygon of the last added raster (mosaic world coords)
              lastMosaicAddedRasterPol.push_back( new te::gm::LinearRing( 
                inRasterDetailedExtent ) );
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

  } // end namespace rp
}   // end namespace te

