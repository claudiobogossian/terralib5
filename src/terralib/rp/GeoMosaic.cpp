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
  \file terralib/rp/GeoMosaic.cpp
  \brief Create a mosaic from a set of geo-referenced rasters.
*/

#include "GeoMosaic.h"

#include "Macros.h"
#include "Functions.h"
#include "../raster/Interpolator.h"
#include "../raster/Enums.h"
#include "../raster/RasterFactory.h"
#include "../raster/Grid.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/PositionIterator.h"
#include "../raster/Utils.h"
#include "../memory/CachedRaster.h"
#include "../geometry/Envelope.h"
#include "../geometry/GTFactory.h"
#include "../geometry/Polygon.h"
#include "../geometry/LinearRing.h"
#include "../geometry/MultiPolygon.h"
#include "../srs/Converter.h"
#include "../common/progress/TaskProgress.h"

#include <boost/shared_array.hpp>
#include <boost/filesystem.hpp>

#include <climits>
#include <cfloat>
#include <cmath>
#include <memory>
#include <cstdio>

namespace te
{
  namespace rp
  {

    GeoMosaic::InputParameters::InputParameters()
    {
      reset();
    }

    GeoMosaic::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    GeoMosaic::InputParameters::~InputParameters()
    {
      reset();
    }

    void GeoMosaic::InputParameters::reset() throw( te::rp::Exception )
    {
      m_feederRasterPtr = 0;
      m_inputRastersBands.clear();
      m_interpMethod = te::rst::Interpolator::NearestNeighbor;
      m_noDataValue = 0.0;
      m_forceInputNoDataValue = false;
      m_blendMethod = te::rp::Blender::NoBlendMethod;
      m_autoEqualize = true;
      m_useRasterCache = true;
      m_enableProgress = false;
      m_enableMultiThread = true;
    }

    const GeoMosaic::InputParameters& GeoMosaic::InputParameters::operator=(
      const GeoMosaic::InputParameters& params )
    {
      reset();

      m_feederRasterPtr = params.m_feederRasterPtr;
      m_inputRastersBands = params.m_inputRastersBands;
      m_interpMethod = params.m_interpMethod;
      m_noDataValue = params.m_noDataValue;
      m_forceInputNoDataValue = params.m_forceInputNoDataValue;
      m_blendMethod = params.m_blendMethod;
      m_autoEqualize = params.m_autoEqualize;
      m_useRasterCache = params.m_useRasterCache;
      m_enableProgress = params.m_enableProgress;
      m_enableMultiThread = params.m_enableMultiThread;

      return *this;
    }

    te::common::AbstractParameters* GeoMosaic::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    GeoMosaic::OutputParameters::OutputParameters()
    {
      reset();
    }

    GeoMosaic::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    GeoMosaic::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void GeoMosaic::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const GeoMosaic::OutputParameters& GeoMosaic::OutputParameters::operator=(
      const GeoMosaic::OutputParameters& params )
    {
      reset();

      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;
      m_outputRasterPtr.reset();;

      return *this;
    }

    te::common::AbstractParameters* GeoMosaic::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    GeoMosaic::GeoMosaic()
    {
      reset();
    }

    GeoMosaic::~GeoMosaic()
    {
    }

    bool GeoMosaic::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;
      
      GeoMosaic::OutputParameters* outParamsPtr = dynamic_cast<
        GeoMosaic::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      assert( m_inputParameters.m_feederRasterPtr->getObjsCount() > 1 );
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( 2 + m_inputParameters.m_feederRasterPtr->getObjsCount() );
        
        progressPtr->setMessage( "Mosaic" );
      }       
      
      // First pass: getting global mosaic info

      double mosaicXResolution = 0.0;
      double mosaicYResolution = 0.0;
      double mosaicLLX = DBL_MAX; // world coords
      double mosaicLLY = DBL_MAX; // world coords
      double mosaicURX = -1.0 * DBL_MAX; // world coords
      double mosaicURY = -1.0 * DBL_MAX; // world coords
      int mosaicSRID = 0;
      std::vector< double > mosaicBandsRangeMin;
      std::vector< double > mosaicBandsRangeMax;       
      te::rst::BandProperty mosaicBaseBandProperties( 0, 0, "" );
      std::vector< te::gm::Polygon > rastersBBoxes; // all rasters bounding boxes (under the first raster world coords.

      {
        te::gm::Polygon auxPolygon( 0, te::gm::PolygonType, 0 );
        te::gm::LinearRing* auxLinearRingPtr = 0;        
        te::rst::Raster const* inputRasterPtr = 0;
        unsigned int inputRasterIdx = 0;
        te::srs::Converter convInstance;
        te::gm::Coord2D llCoord1;
        te::gm::Coord2D urCoord1;
        te::gm::Coord2D llCoord2;
        te::gm::Coord2D urCoord2;

        m_inputParameters.m_feederRasterPtr->reset();
        while( ( inputRasterPtr = m_inputParameters.m_feederRasterPtr->getCurrentObj() ) )
        {
          inputRasterIdx = m_inputParameters.m_feederRasterPtr->getCurrentOffset();
          TERP_TRUE_OR_RETURN_FALSE(
            inputRasterPtr->getAccessPolicy() & te::common::RAccess,
            "Invalid raster" );
            
          // Defining the base mosaic info

          if( inputRasterIdx == 0 )
          {
            mosaicXResolution = inputRasterPtr->getGrid()->getResolutionX();
            mosaicYResolution = inputRasterPtr->getGrid()->getResolutionY();

            mosaicLLX = inputRasterPtr->getGrid()->getExtent()->m_llx;
            mosaicLLY = inputRasterPtr->getGrid()->getExtent()->m_lly;
            mosaicURX = inputRasterPtr->getGrid()->getExtent()->m_urx;
            mosaicURY = inputRasterPtr->getGrid()->getExtent()->m_ury;

            mosaicSRID = inputRasterPtr->getGrid()->getSRID();

            mosaicBaseBandProperties = *inputRasterPtr->getBand( 0 )->getProperty();

            // finding the current raster bounding box polygon (first raster world coordinates)
            auxPolygon.clear();
            auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
            auxLinearRingPtr->setPoint( 0, mosaicLLX, mosaicURY );
            auxLinearRingPtr->setPoint( 1, mosaicURX, mosaicURY );
            auxLinearRingPtr->setPoint( 2, mosaicURX, mosaicLLY );
            auxLinearRingPtr->setPoint( 3, mosaicLLX, mosaicLLY );
            auxLinearRingPtr->setPoint( 4, mosaicLLX, mosaicURY );
            auxPolygon.push_back( auxLinearRingPtr );
            auxPolygon.setSRID( mosaicSRID );
            rastersBBoxes.push_back( auxPolygon );
          }
          else
          {
            if( mosaicSRID == inputRasterPtr->getGrid()->getSRID() )
            {
              urCoord1.x = inputRasterPtr->getGrid()->getExtent()->m_urx;
              urCoord1.y = inputRasterPtr->getGrid()->getExtent()->m_ury;
              llCoord1.x = inputRasterPtr->getGrid()->getExtent()->m_llx;
              llCoord1.y = inputRasterPtr->getGrid()->getExtent()->m_lly;
            }
            else
            {
              convInstance.setSourceSRID( inputRasterPtr->getGrid()->getSRID() );
              convInstance.setTargetSRID( mosaicSRID );

              convInstance.convert(
                inputRasterPtr->getGrid()->getExtent()->m_urx,
                inputRasterPtr->getGrid()->getExtent()->m_ury,
                urCoord1.x,
                urCoord1.y );
              convInstance.convert(
                inputRasterPtr->getGrid()->getExtent()->m_llx,
                inputRasterPtr->getGrid()->getExtent()->m_lly,
                llCoord1.x,
                llCoord1.y );
            }

            // expanding mosaic area

            mosaicLLX = std::min( mosaicLLX, urCoord1.x );
            mosaicLLX = std::min( mosaicLLX, llCoord1.x );

            mosaicLLY = std::min( mosaicLLY, urCoord1.y );
            mosaicLLY = std::min( mosaicLLY, llCoord1.y );

            mosaicURX = std::max( mosaicURX, urCoord1.x );
            mosaicURX = std::max( mosaicURX, llCoord1.x );

            mosaicURY = std::max( mosaicURY, urCoord1.y );
            mosaicURY = std::max( mosaicURY, llCoord1.y );

            // finding the current raster bounding box polygon (first raster world coordinates)

            auxPolygon.clear();
            auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
            auxLinearRingPtr->setPoint( 0, llCoord1.x, urCoord1.y );
            auxLinearRingPtr->setPoint( 1, urCoord1.x, urCoord1.y );
            auxLinearRingPtr->setPoint( 2, urCoord1.x, llCoord1.y );
            auxLinearRingPtr->setPoint( 3, llCoord1.x, llCoord1.y );
            auxLinearRingPtr->setPoint( 4, llCoord1.x, urCoord1.y );
            auxPolygon.push_back( auxLinearRingPtr );
            auxPolygon.setSRID( mosaicSRID );
            rastersBBoxes.push_back( auxPolygon );
          }

          // checking the input bands

          for( std::vector< unsigned int >::size_type inputRastersBandsIdx = 0 ;
            inputRastersBandsIdx <
            m_inputParameters.m_inputRastersBands[ inputRasterIdx ].size() ;
            ++inputRastersBandsIdx )
          {
            const unsigned int& currBand =
              m_inputParameters.m_inputRastersBands[ inputRasterIdx ][ inputRastersBandsIdx ];

            TERP_TRUE_OR_RETURN_FALSE( currBand < inputRasterPtr->getNumberOfBands(),
              "Invalid band" )
          }


          m_inputParameters.m_feederRasterPtr->moveNext();
        }
      }
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }        

      // creating the output raster
      
      te::rst::Raster* outputRasterPtr = 0;

      {
        mosaicBandsRangeMin.resize( 
          m_inputParameters.m_inputRastersBands[ 0 ].size(), 0 );
        mosaicBandsRangeMax.resize( 
          m_inputParameters.m_inputRastersBands[ 0 ].size(), 0 );           
        
        std::vector< te::rst::BandProperty* > bandsProperties;
        for( std::vector< unsigned int >::size_type bandIdx = 0 ;  bandIdx <
          m_inputParameters.m_inputRastersBands[ 0 ].size() ; ++bandIdx )
        {
          bandsProperties.push_back( new te::rst::BandProperty( mosaicBaseBandProperties ) );
          bandsProperties[ bandIdx ]->m_colorInterp = te::rst::GrayIdxCInt;
          bandsProperties[ bandIdx ]->m_noDataValue = m_inputParameters.m_noDataValue;
          
          te::rst::GetDataTypeRanges( bandsProperties[ bandIdx ]->m_type,
            mosaicBandsRangeMin[ bandIdx ],
            mosaicBandsRangeMax[ bandIdx ]);           
        }

        te::rst::Grid* outputGrid = new te::rst::Grid( mosaicXResolution,
          mosaicYResolution,  new te::gm::Envelope( mosaicLLX, mosaicLLY, mosaicURX,
          mosaicURY ), mosaicSRID );

        outParamsPtr->m_outputRasterPtr.reset(
          te::rst::RasterFactory::make(
            outParamsPtr->m_rType,
            outputGrid,
            bandsProperties,
            outParamsPtr->m_rInfo,
            0,
            0 ) );
        TERP_TRUE_OR_RETURN_FALSE( outParamsPtr->m_outputRasterPtr.get(),
          "Output raster creation error" );
          
        outputRasterPtr = outParamsPtr->m_outputRasterPtr.get();
      }

      std::auto_ptr< te::mem::CachedRaster > cachedRasterInstancePtr;
      
      if( m_inputParameters.m_useRasterCache )
      {
        cachedRasterInstancePtr.reset( new te::mem::CachedRaster(
          *(outParamsPtr->m_outputRasterPtr.get()), 20, 0 ) );   
          
        outputRasterPtr = cachedRasterInstancePtr.get();
      }

      // fill output with no data values

      {
        const unsigned int nBands = outputRasterPtr->getNumberOfBands();
        const unsigned int nRows = outputRasterPtr->getNumberOfRows();
        const unsigned int nCols = outputRasterPtr->getNumberOfColumns();
        unsigned int col = 0;
        unsigned int row = 0;
        unsigned int bandIdx = 0;

        for( bandIdx = 0 ; bandIdx < nBands ; ++bandIdx )
        {
          te::rst::Band& outBand = ( *( outputRasterPtr->getBand( bandIdx ) ) );

          for( row = 0 ; row < nRows ; ++row )
          {
            for( col = 0 ; col < nCols ; ++col )
            {
              outBand.setValue( col, row, m_inputParameters.m_noDataValue );
            }
          }
        }
      }
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }        
      
      // Copying the first image data to the output mosaic
      // and find the base mosaic mean and offset values

      std::vector< double > mosaicTargetMeans( outputRasterPtr->getNumberOfBands(), 0 );
      std::vector< double > mosaicTargetVariances( outputRasterPtr->getNumberOfBands(), 0 );

      {
        m_inputParameters.m_feederRasterPtr->reset();

        te::rst::Raster const* inputRasterPtr =
          m_inputParameters.m_feederRasterPtr->getCurrentObj();
        TERP_DEBUG_TRUE_OR_RETURN_FALSE( inputRasterPtr, "Invalid raster pointer" );

        double inXStartGeo = 0;
        double inYStartGeo = 0;
        inputRasterPtr->getGrid()->gridToGeo( 0.0, 0.0, inXStartGeo, inYStartGeo );
        double outRowStartDouble = 0;
        double outColStartDouble = 0;
        outputRasterPtr->getGrid()->geoToGrid( inXStartGeo, inYStartGeo,
          outColStartDouble, outRowStartDouble );

        const unsigned int outRowStart = (unsigned int)std::max( 0.0, outRowStartDouble );
        const unsigned int outColStart = (unsigned int)std::max( 0.0, outColStartDouble );
        const unsigned int outRowsBound = std::min( outRowStart +
          inputRasterPtr->getNumberOfRows(),
          outputRasterPtr->getNumberOfRows() );
        const unsigned int outColsBound = std::min( outColStart +
          inputRasterPtr->getNumberOfColumns(),
          outputRasterPtr->getNumberOfColumns() );

        const unsigned int nBands = (unsigned int)
          m_inputParameters.m_inputRastersBands[ 0 ].size();
        unsigned int outCol = 0;
        unsigned int outRow = 0;
        double inCol = 0;
        double inRow = 0;
        double bandNoDataValue = -1.0 * DBL_MAX;
        std::complex< double > pixelCValue = 0;
        te::rst::Interpolator interpInstance( inputRasterPtr,
          m_inputParameters.m_interpMethod );
        unsigned int inputBandIdx = 0;

        for( unsigned int inputRastersBandsIdx = 0 ; inputRastersBandsIdx <
          nBands ; ++inputRastersBandsIdx )
        {
          inputBandIdx =  m_inputParameters.m_inputRastersBands[ 0 ][
            inputRastersBandsIdx ] ;
          bandNoDataValue = m_inputParameters.m_forceInputNoDataValue ?
            m_inputParameters.m_noDataValue : inputRasterPtr->getBand( inputBandIdx
            )->getProperty()->m_noDataValue;
          te::rst::Band& outBand =
            (*outputRasterPtr->getBand( inputRastersBandsIdx ));
          unsigned int validPixelsNumber = 0;

          double& mean = mosaicTargetMeans[ inputRastersBandsIdx ];
          mean = 0;

          for( outRow = outRowStart ; outRow < outRowsBound ; ++outRow )
          {
            inRow = ((double)outRow) - outRowStartDouble;

            for( outCol = outColStart ; outCol < outColsBound ; ++outCol )
            {
              inCol = ((double)outCol) - outColStartDouble;

              interpInstance.getValue( inCol, inRow, pixelCValue, inputBandIdx );

              if( pixelCValue.real() != bandNoDataValue )
              {
                outBand.setValue( outCol, outRow, pixelCValue );
                mean += pixelCValue.real();
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

            for( outRow = outRowStart ; outRow < outRowsBound ; ++outRow )
            {
              for( outCol = outColStart ; outCol < outColsBound ; ++outCol )
              {
                outBand.getValue( outCol, outRow, pixelValue );

                if( pixelValue != m_inputParameters.m_noDataValue )
                {
                  variance += ( ( pixelValue - mean ) * ( pixelValue -
                    mean ) ) / ( (double)validPixelsNumber );
                }
              }
            }
          }
        }
      }
      
      TERP_DEBUG_TRUE_OR_THROW( rastersBBoxes.size() ==
        m_inputParameters.m_feederRasterPtr->getObjsCount(),
        "Rasters bounding boxes number mismatch" );
        
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }          

      // Initiating the mosaic bounding boxes union

      std::auto_ptr< te::gm::MultiPolygon > mosaicBBoxesUnionPtr(
        new te::gm::MultiPolygon( 0, te::gm::MultiPolygonType,
        outputRasterPtr->getSRID(), 0 ) );
      mosaicBBoxesUnionPtr->add( (te::gm::Polygon*)rastersBBoxes[ 0 ].clone() );

      // skipping the first raster

      m_inputParameters.m_feederRasterPtr->reset();
      m_inputParameters.m_feederRasterPtr->moveNext();

      // iterating over the other rasters
      
      te::rst::Raster const* originalInputRasterPtr = 0;

      std::vector< unsigned int > outputRasterBands;
      std::vector< double > dummyRasterOffsets;
      std::vector< double > dummyRasterScales;
      for( unsigned int outputRasterBandsIdx = 0 ; outputRasterBandsIdx <
        outputRasterPtr->getNumberOfBands() ; ++outputRasterBandsIdx )
      {
        outputRasterBands.push_back( outputRasterBandsIdx );
        dummyRasterOffsets.push_back( 0.0 );
        dummyRasterScales.push_back( 1.0 );
      }      

      while( ( originalInputRasterPtr = m_inputParameters.m_feederRasterPtr->getCurrentObj() ) )
      {
        const unsigned int inputRasterIdx = m_inputParameters.m_feederRasterPtr->getCurrentOffset();
        
//         Copy2DiskRaster( outputRaster, boost::lexical_cast< std::string >( inputRasterIdx ) +
//           "_output_raster_begininng.tif" );

        TERP_DEBUG_TRUE_OR_THROW( rastersBBoxes[ inputRasterIdx ].getSRID() == outputRasterPtr->getSRID(),
          "Invalid boxes SRID" );
          
        te::rst::Raster const* inputRasterPtr = originalInputRasterPtr;
        
//         Copy2DiskRaster( *inputRasterPtr, boost::lexical_cast< std::string >( inputRasterIdx ) +
//           "_input_raster.tif" );

        // reprojection / caching issues
        
        std::auto_ptr< te::rst::Raster > reprojectedInputRasterPtr;
        
        std::string reprojectedRasterFileName;

        if( outputRasterPtr->getSRID() != inputRasterPtr->getSRID() )
        {
          reprojectedRasterFileName = boost::filesystem::unique_path( 
            boost::filesystem::temp_directory_path() /= 
            boost::filesystem::path( "TerralibRGeoMosaic_%%%%-%%%%-%%%%-%%%%" ) ).string();
          TERP_TRUE_OR_RETURN_FALSE( !reprojectedRasterFileName.empty(),
            "Invalid temporary raster file name" );
          reprojectedRasterFileName += ".tif";
          
          std::map< std::string, std::string > rInfo;
          rInfo[ "URI" ] = reprojectedRasterFileName;
          
          reprojectedInputRasterPtr.reset( inputRasterPtr->transform( 
            outputRasterPtr->getSRID(), rInfo,
            m_inputParameters.m_interpMethod) );
          inputRasterPtr = reprojectedInputRasterPtr.get();
          TERP_TRUE_OR_RETURN_FALSE( inputRasterPtr, "Reprojection error" );
          
//           Copy2DiskRaster( *inputRasterPtr, boost::lexical_cast< std::string >( inputRasterIdx ) +
//             "reprojected_input_raster.tif" );
        }
        
        // Caching issues 
        
        std::auto_ptr< te::mem::CachedRaster > cachedInputRasterPtr;
        
        if( m_inputParameters.m_useRasterCache )
        {
          cachedInputRasterPtr.reset( new te::mem::CachedRaster(
            *inputRasterPtr, 20, 0 ) );
          inputRasterPtr = cachedInputRasterPtr.get();
        }         
        
        // The transformation mapping outputRaster pixels 
        // ( te::gm::GTParameters::TiePoint::first ) to input pixels 
        // ( te::gm::GTParameters::TiePoint::second ) 
        // (Note: all coords are indexed by lines/columns).
        
        std::auto_ptr< te::gm::GeometricTransformation > transPtr;
        
        {
          te::gm::GTParameters transParams;
          double auxX;
          double auxY;
          te::gm::GTParameters::TiePoint auxTP;
          
          auxTP.first.x = 0;
          auxTP.first.y = 0;
          outputRasterPtr->getGrid()->gridToGeo( auxTP.first.x, auxTP.first.y, auxX, auxY );
          inputRasterPtr->getGrid()->geoToGrid( auxX, auxY, auxTP.second.x, auxTP.second.y );
          transParams.m_tiePoints.push_back( auxTP );
          
          auxTP.first.x = 0;
          auxTP.first.y = (double)( outputRasterPtr->getNumberOfRows() - 1 );
          outputRasterPtr->getGrid()->gridToGeo( auxTP.first.x, auxTP.first.y, auxX, auxY );
          inputRasterPtr->getGrid()->geoToGrid( auxX, auxY, auxTP.second.x, auxTP.second.y );          
          transParams.m_tiePoints.push_back( auxTP );
          
          auxTP.first.x = (double)( outputRasterPtr->getNumberOfColumns() - 1);
          auxTP.first.y = 0;
          outputRasterPtr->getGrid()->gridToGeo( auxTP.first.x, auxTP.first.y, auxX, auxY );
          inputRasterPtr->getGrid()->geoToGrid( auxX, auxY, auxTP.second.x, auxTP.second.y );
          transParams.m_tiePoints.push_back( auxTP );
          
          auxTP.first.x = (double)( outputRasterPtr->getNumberOfColumns() - 1 );
          auxTP.first.y = (double)( outputRasterPtr->getNumberOfRows() - 1 );
          outputRasterPtr->getGrid()->gridToGeo( auxTP.first.x, auxTP.first.y, auxX, auxY );
          inputRasterPtr->getGrid()->geoToGrid( auxX, auxY, auxTP.second.x, auxTP.second.y );                    
          transParams.m_tiePoints.push_back( auxTP );
          
          transPtr.reset( te::gm::GTFactory::make( "RST" ) );
          TERP_TRUE_OR_RETURN_FALSE( transPtr.get(), "Could not instantiate a geometric transformation" );
          
          TERP_TRUE_OR_RETURN_FALSE( transPtr->initialize( transParams ),
            "Could not initialize a geometric transformation" );
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
          currentRasterBandsOffsets = dummyRasterOffsets;
          currentRasterBandsScales = dummyRasterScales;
        }

         // blending

        te::rp::Blender blenderInstance;

        TERP_TRUE_OR_RETURN_FALSE( blenderInstance.initialize(
          *outputRasterPtr,
          outputRasterBands,
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
          mosaicBBoxesUnionPtr.get(),
          0,
          *transPtr,
          m_inputParameters.m_enableMultiThread ? 0 : 1,
          false ), "Blender initiazing error" );
        
        TERP_TRUE_OR_RETURN_FALSE( blenderInstance.blendIntoRaster1(), 
          "Error blending images" );

//           Copy2DiskRaster( outputRaster, boost::lexical_cast< std::string >( inputRasterIdx ) +
//              "output_raster_after_blending_" + 
//             boost::lexical_cast< std::string >( mosaicBBoxesUnionIdx ) + ".tif" );

        // updating the  gloabal mosaic boxes

        std::auto_ptr< te::gm::Geometry > boxesUnionResultPtr; // under the mosaic SRID
        boxesUnionResultPtr.reset( mosaicBBoxesUnionPtr->Union(
          &( rastersBBoxes[ inputRasterIdx ] ) ) );
        TERP_TRUE_OR_THROW( boxesUnionResultPtr.get(), "Invalid pointer" );
        boxesUnionResultPtr->setSRID( outputRasterPtr->getSRID() );

        if( boxesUnionResultPtr->getGeomTypeId() == te::gm::MultiPolygonType )
        {
          mosaicBBoxesUnionPtr.reset( (te::gm::MultiPolygon*)boxesUnionResultPtr.release() );
        }
        else if( boxesUnionResultPtr->getGeomTypeId() == te::gm::PolygonType )
        {
          // transforming it into a te::gm::MultiPolygon
          te::gm::MultiPolygon* auxMultiPol = new te::gm::MultiPolygon( 0,
            te::gm::MultiPolygonType, boxesUnionResultPtr->getSRID(), 0 );
          auxMultiPol->add( boxesUnionResultPtr.release() );

          mosaicBBoxesUnionPtr.reset( auxMultiPol );
        }
        else
        {
          TERP_LOG_AND_RETURN_FALSE( "Invalid union geometry type" );
        }
        
        // Reseting the input cache
        
        cachedInputRasterPtr.reset();
        
        // deleting the reprojected raster
        
        if( reprojectedInputRasterPtr.get() )
        {
          reprojectedInputRasterPtr.reset();
          remove( reprojectedRasterFileName.c_str() );
        }

        // moving to the next raster

        m_inputParameters.m_feederRasterPtr->moveNext();
        
        if( m_inputParameters.m_enableProgress )
        {
          progressPtr->pulse();
          if( ! progressPtr->isActive() ) return false;
        }          
      }
      
      // reseting the output cache
      
      if( cachedRasterInstancePtr.get() ) cachedRasterInstancePtr.reset();
      
      return true;
    }

    void GeoMosaic::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool GeoMosaic::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      GeoMosaic::InputParameters const* inputParamsPtr = dynamic_cast<
        GeoMosaic::InputParameters const* >( &inputParams );
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

      for( std::vector< std::vector< unsigned int > >::size_type
        inputRastersBandsIdx = 0 ;  inputRastersBandsIdx <
        m_inputParameters.m_inputRastersBands.size() ; ++inputRastersBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRastersBands[
          inputRastersBandsIdx ].size() > 0, "Invalid bands number" );

        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRastersBands[
          inputRastersBandsIdx ].size() ==  m_inputParameters.m_inputRastersBands[
          0 ].size(), "Bands number mismatch" );
      }

      m_isInitialized = true;

      return true;
    }

    bool GeoMosaic::isInitialized() const
    {
      return m_isInitialized;
    }

    void GeoMosaic::calcBandStatistics( const te::rst::Band& band,
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

