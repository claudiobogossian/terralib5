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
//#include "../raster/Utils.h"
#include "../memory/CachedRaster.h"
#include "../memory/ExpansibleRaster.h"
//#include "../geometry/Envelope.h"
#include "../geometry/GTFactory.h"
//#include "../geometry/Polygon.h"
//#include "../geometry/LinearRing.h"
//#include "../geometry/MultiPolygon.h"
//#include "../srs/Converter.h"

//#include <boost/shared_ptr.hpp>

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
      m_locatorParams = params.m_locatorParams;

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
      m_dataSetsNamePrefix.clear();
    }

    const SequenceMosaic::OutputParameters& SequenceMosaic::OutputParameters::operator=(
      const SequenceMosaic::OutputParameters& params )
    {
      reset();

      m_outputDSPtr = params.m_outputDSPtr;
      m_dataSetsNamePrefix = params.m_dataSetsNamePrefix;

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

      // creating the first output mosaic raster
      // definig the output mosaic raster properties
      // Copying the first image data to the output mosaic
      // Finding the base mosaic mean and offset values
      
      std::auto_ptr< te::mem::ExpansibleRaster > mosaicRasterHandler;
      std::vector< te::rst::BandProperty > mosaicBandProperties;
      double mosaicXResolution = 0;
      double mosaicYResolution = 0;
      int mosaicSRID = 0;
      std::vector< double > mosaicTargetMeans;
      std::vector< double > mosaicTargetVariances;

      {
        m_inputParameters.m_feederRasterPtr->reset();
        te::rst::Raster const * const firstRasterPtr = 
          m_inputParameters.m_feederRasterPtr->getCurrentObj();
          
        mosaicXResolution = firstRasterPtr->getGrid()->getResolutionX();
        mosaicYResolution = firstRasterPtr->getGrid()->getResolutionY();
        mosaicSRID = firstRasterPtr->getGrid()->getSRID();
        
        std::vector< te::rst::BandProperty* > bandsProperties;
        for( std::vector< unsigned int >::size_type inputRastersBandsIdx = 0 ;  
          inputRastersBandsIdx <
          m_inputParameters.m_inputRastersBands[ 0 ].size() ; 
          ++inputRastersBandsIdx )
        {
          te::rst::BandProperty const* const inBandPropPtr =
            firstRasterPtr->getBand( m_inputParameters.m_inputRastersBands[ 
            0 ][ inputRastersBandsIdx ] )->getProperty();
          
          bandsProperties.push_back( new te::rst::BandProperty( *inBandPropPtr ) );
          bandsProperties[ inputRastersBandsIdx ]->m_colorInterp = te::rst::GrayIdxCInt;
          bandsProperties[ inputRastersBandsIdx ]->m_noDataValue = m_inputParameters.m_noDataValue;
          
          mosaicBandProperties.push_back( *bandsProperties[ inputRastersBandsIdx ] );
        }

        mosaicRasterHandler.reset( 
          new te::mem::ExpansibleRaster( SEQUENCE_RASTER_MAX_MOSAIC_MEM_USE,
          new te::rst::Grid( *( firstRasterPtr->getGrid() ) ),
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
            m_inputParameters.m_noDataValue : firstRasterPtr->getBand( inputBandIdx
            )->getProperty()->m_noDataValue;
          const unsigned int outRowsBound = firstRasterPtr->getNumberOfRows();
          const unsigned int outColsBound = firstRasterPtr->getNumberOfColumns();
          te::rst::Band& outBand =
            (*mosaicRasterHandler->getBand( inputRastersBandsIdx ));
          const te::rst::Band& inBand =
            (*firstRasterPtr->getBand( inputBandIdx ));
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
      
      // iterating over the other rasters
      
      unsigned int mosaicSequenceCounter = 0;
      
      m_inputParameters.m_feederRasterPtr->reset();
      m_inputParameters.m_feederRasterPtr->moveNext();

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
          currentRasterBandsOffsets.resize( mosaicTargetMeans.size(), 0.0 );
          currentRasterBandsScales.resize( mosaicTargetMeans.size(), 0.0 );
        }        
        
        m_inputParameters.m_feederRasterPtr->moveNext();
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
        }
        
        m_inputParameters.m_feederRasterPtr->moveNext();
      }

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

  } // end namespace rp
}   // end namespace te

