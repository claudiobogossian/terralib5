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
  \file terralib/rp/Mosaic.cpp
  \brief Create a mosaic from a set of rasters.
*/

#include "Mosaic.h"

#include "Macros.h"
#include "../raster/Enums.h"
#include "../raster/RasterFactory.h"
#include "../raster/Grid.h"
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../memory/CachedRaster.h"
#include "../geometry/Envelope.h"
#include "../geometry/GTFactory.h"
#include "../geometry/Polygon.h"
#include "../geometry/LinearRing.h"
#include "../srs/Converter.h"

#include <boost/shared_ptr.hpp>

#include <climits>
#include <cfloat>

namespace te
{
  namespace rp
  {

    Mosaic::InputParameters::InputParameters()
    {
      reset();
    }
    
    Mosaic::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }    

    Mosaic::InputParameters::~InputParameters()
    {
      reset();
    }

    void Mosaic::InputParameters::reset() throw( te::rp::Exception )
    {
      m_feederRasterPtr = 0;
      m_inputRastersBands.clear();
      m_tiePoints.clear();
      m_geomTransfName = "Affine";
      m_interpMethod = te::rst::Interpolator::NearestNeighbor;
      m_noDataValue = 0.0;
      m_blendMethod = te::rp::Blender::NoBlendMethod;
    }

    const Mosaic::InputParameters& Mosaic::InputParameters::operator=(
      const Mosaic::InputParameters& params )
    {
      reset();
      
      m_feederRasterPtr = params.m_feederRasterPtr;
      m_inputRastersBands = params.m_inputRastersBands;
      m_tiePoints = params.m_tiePoints;
      m_geomTransfName = params.m_geomTransfName;
      m_interpMethod = params.m_interpMethod;
      m_noDataValue = params.m_noDataValue;
      m_blendMethod = params.m_blendMethod;

      return *this;
    }

    te::common::AbstractParameters* Mosaic::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    Mosaic::OutputParameters::OutputParameters()
    {
      reset();
    }

    Mosaic::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    Mosaic::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void Mosaic::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const Mosaic::OutputParameters& Mosaic::OutputParameters::operator=(
      const Mosaic::OutputParameters& params )
    {
      reset();
      
      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;
      m_outputRasterPtr = params.m_outputRasterPtr;
      
      return *this;
    }

    te::common::AbstractParameters* Mosaic::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    Mosaic::Mosaic()
    {
      reset();
    }

    Mosaic::~Mosaic()
    {
    }
    
    bool Mosaic::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;
      
      Mosaic::OutputParameters* outParamsPtr = dynamic_cast<
        Mosaic::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      // First pass: getting global mosaic info
      
      const bool useTiePoints = ( m_inputParameters.m_tiePoints.size() > 0 );
      double mosaicXResolution = 0.0;
      double mosaicYResolution = 0.0;
      double mosaicLLX = DBL_MAX; // world coords
      double mosaicLLY = DBL_MAX; // world coords
      double mosaicURX = -1.0 * DBL_MAX; // world coords
      double mosaicURY = -1.0 * DBL_MAX; // world coords
      int mosaicSRID = 0;
      te::rst::BandProperty mosaicBaseBandProperties( 0, 0, "" );
      std::vector< boost::shared_ptr< te::gm::GeometricTransformation > >
        mosaicGeomTransfms; // Mapping indexed points (line/coluns) from each raster to the first raster indexed points (lines/columns)..
      std::vector< te::gm::Polygon > rastersBBoxes; // all rasters bounding boxes (under the first raster world coords.
      te::gm::Polygon auxPolygon( 0, te::gm::PolygonType, 0 );
      te::gm::LinearRing* auxLinearRingPtr = 0;
      {
        te::rst::Raster const* inputRasterPtr = 0;
        unsigned int inputRasterIdx = 0;
        te::srs::Converter convInstance;
        boost::shared_ptr< te::gm::GeometricTransformation > auxTransPtr;
        te::gm::Coord2D llCoord1;
        te::gm::Coord2D lrCoord1;
        te::gm::Coord2D ulCoord1;
        te::gm::Coord2D urCoord1;
        te::gm::Coord2D llCoord2;
        te::gm::Coord2D lrCoord2;
        te::gm::Coord2D ulCoord2;
        te::gm::Coord2D urCoord2;    
        te::rst::Grid firstRasterGrid( 1u, 1u, new te::gm::Envelope( 0, 0, 0, 0), 
          -1 );
        
        m_inputParameters.m_feederRasterPtr->reset();
        while( ( inputRasterPtr = m_inputParameters.m_feederRasterPtr->getCurrentObj() ) )
        {
          inputRasterIdx = m_inputParameters.m_feederRasterPtr->getCurrentOffset();
          TERP_TRUE_OR_RETURN_FALSE( 
            inputRasterPtr->getAccessPolicy() & te::common::RAccess, 
            "Invalid raster" );          
            
          // Defining the base mosaic info
          
          if( useTiePoints )
          {
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
              
              firstRasterGrid = (*inputRasterPtr->getGrid());
              
              // finding the current raster bounding box polygon (first raster world coordinates)
              
              auxPolygon.clear();
              auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
              auxLinearRingPtr->setPoint( 0, mosaicLLX, mosaicURY );
              auxLinearRingPtr->setPoint( 1, mosaicURX, mosaicURY );
              auxLinearRingPtr->setPoint( 2, mosaicURX, mosaicLLY );
              auxLinearRingPtr->setPoint( 3, mosaicLLX, mosaicLLY );
              auxLinearRingPtr->setPoint( 4, mosaicLLX, mosaicURY );
              auxPolygon.push_back( auxLinearRingPtr );
              rastersBBoxes.push_back( auxPolygon );
              
            }
            else
            {
              te::gm::GTParameters transParams;
              
              if( inputRasterIdx == 1 )
              {
                transParams.m_tiePoints = m_inputParameters.m_tiePoints[ 0 ];
              }
              else
              {
                // converting the current tie-points to map coords from the
                // current raster to the first one
                
                te::gm::GTParameters::TiePoint auxTP;
                const std::vector< te::gm::GTParameters::TiePoint >& inputTPs =
                  m_inputParameters.m_tiePoints[ inputRasterIdx - 1 ];
                const unsigned int inputTPsSize = inputTPs.size();
                const te::gm::GeometricTransformation& lastTransf = 
                  (*mosaicGeomTransfms[ inputRasterIdx - 2 ].get());
                
                for( unsigned int inputTPsIdx = 0 ; inputTPsIdx < inputTPsSize ;
                  ++inputTPsIdx )
                {
                  auxTP.second = inputTPs[ inputTPsIdx - 1 ].second;
                  lastTransf.inverseMap( inputTPs[ inputTPsIdx - 1 ].first, auxTP.first  );
                  transParams.m_tiePoints.push_back( auxTP );
                }
              }
                
              auxTransPtr.reset( te::gm::GTFactory::make( 
                m_inputParameters.m_geomTransfName ) );  
              TERP_TRUE_OR_RETURN_FALSE( auxTransPtr.get() != 0,
                "Geometric transformation instatiation error" );
              TERP_TRUE_OR_RETURN_FALSE( auxTransPtr->initialize( transParams ),
                "Geometric transformation parameters calcule error" );
              mosaicGeomTransfms.push_back( auxTransPtr );
                
              // current raster corner coords (line/column)
              
              ulCoord2.x = -0.5;
              ulCoord2.y = -0.5;
              urCoord2.x = ((double)inputRasterPtr->getGrid()->getNumberOfColumns() 
                - 0.5);
              urCoord2.y = -0.5;
              lrCoord2.x = urCoord2.x;
              lrCoord2.y = ((double)inputRasterPtr->getGrid()->getNumberOfRows() 
                - 0.5);
              llCoord2.x = ulCoord2.x;
              llCoord2.y = lrCoord2.y;
              
              // current raster corner coords (line/column) over the 
              // first raster coords system (lines/columns)
              
              auxTransPtr->inverseMap( ulCoord2, ulCoord1 );
              auxTransPtr->inverseMap( urCoord2, urCoord1 );
              auxTransPtr->inverseMap( lrCoord2, lrCoord1 );
              auxTransPtr->inverseMap( llCoord2, llCoord1 );              
              
              // the respective coords in world space (first raster)
              
              firstRasterGrid.gridToGeo( ulCoord1.x, ulCoord1.y, ulCoord2.x, 
                ulCoord2.y );                  
              firstRasterGrid.gridToGeo( urCoord1.x, urCoord1.y, urCoord2.x, 
                urCoord2.y );  
              firstRasterGrid.gridToGeo( lrCoord1.x, lrCoord1.y, lrCoord2.x, 
                lrCoord2.y );                
              firstRasterGrid.gridToGeo( llCoord1.x, llCoord1.y, llCoord2.x, 
                llCoord2.y );
              
              // expanding mosaic area
              
              mosaicLLX = std::min( mosaicLLX, ulCoord2.x );
              mosaicLLX = std::min( mosaicLLX, urCoord2.x );
              mosaicLLX = std::min( mosaicLLX, lrCoord2.x );
              mosaicLLX = std::min( mosaicLLX, llCoord2.x );
              
              mosaicLLY = std::min( mosaicLLY, ulCoord2.y );
              mosaicLLY = std::min( mosaicLLY, urCoord2.y );
              mosaicLLY = std::min( mosaicLLY, llCoord2.y );
              mosaicLLY = std::min( mosaicLLY, lrCoord2.y );
              
              mosaicURX = std::max( mosaicURX, ulCoord2.x );
              mosaicURX = std::max( mosaicURX, urCoord2.x );
              mosaicURX = std::max( mosaicURX, lrCoord2.x );
              mosaicURX = std::max( mosaicURX, llCoord2.x );
              
              mosaicURY = std::max( mosaicURY, ulCoord2.y );
              mosaicURY = std::max( mosaicURY, urCoord2.y );
              mosaicURY = std::max( mosaicURY, lrCoord2.y );
              mosaicURY = std::max( mosaicURY, llCoord2.y );
              
              // finding the current raster bounding box polygon (first raster world coordinates)
              
              auxPolygon.clear();
              auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
              auxLinearRingPtr->setPoint( 0, ulCoord2.x, ulCoord2.y );
              auxLinearRingPtr->setPoint( 1, urCoord2.x, urCoord2.y );
              auxLinearRingPtr->setPoint( 2, lrCoord2.x, lrCoord2.y );
              auxLinearRingPtr->setPoint( 3, llCoord2.x, llCoord2.y );
              auxLinearRingPtr->setPoint( 4, ulCoord2.x, ulCoord2.y );
              auxPolygon.push_back( auxLinearRingPtr );
              rastersBBoxes.push_back( auxPolygon );
                            
            }
          }
          else
          {
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
              rastersBBoxes.push_back( auxPolygon );
            }
            else
            {
              if( mosaicSRID == inputRasterPtr->getGrid()->getSRID() )
              {
                ulCoord1.x = inputRasterPtr->getGrid()->getExtent()->m_llx;
                ulCoord1.y = inputRasterPtr->getGrid()->getExtent()->m_ury;            
                urCoord1.x = inputRasterPtr->getGrid()->getExtent()->m_urx;
                urCoord1.y = inputRasterPtr->getGrid()->getExtent()->m_ury;            
                lrCoord1.x = inputRasterPtr->getGrid()->getExtent()->m_urx;
                lrCoord1.y = inputRasterPtr->getGrid()->getExtent()->m_lly;
                llCoord1.x = inputRasterPtr->getGrid()->getExtent()->m_llx;
                llCoord1.y = inputRasterPtr->getGrid()->getExtent()->m_lly;
              }
              else
              {
                convInstance.setSourceSRID( inputRasterPtr->getGrid()->getSRID() );
                convInstance.setTargetSRID( mosaicSRID );

                convInstance.convert( 
                  inputRasterPtr->getGrid()->getExtent()->m_llx,
                  inputRasterPtr->getGrid()->getExtent()->m_ury,
                  ulCoord1.x,
                  ulCoord1.y );                
                convInstance.convert( 
                  inputRasterPtr->getGrid()->getExtent()->m_urx,
                  inputRasterPtr->getGrid()->getExtent()->m_urx,
                  urCoord1.x,
                  urCoord1.y );
                convInstance.convert( 
                  inputRasterPtr->getGrid()->getExtent()->m_urx,
                  inputRasterPtr->getGrid()->getExtent()->m_lly,
                  lrCoord1.x,
                  lrCoord1.y );
                convInstance.convert( 
                  inputRasterPtr->getGrid()->getExtent()->m_llx,
                  inputRasterPtr->getGrid()->getExtent()->m_llx,
                  llCoord1.x,
                  llCoord1.y );
              }
              
              // expanding mosaic area
              
              mosaicLLX = std::min( mosaicLLX, ulCoord1.x );
              mosaicLLX = std::min( mosaicLLX, urCoord1.x );
              mosaicLLX = std::min( mosaicLLX, lrCoord1.x );
              mosaicLLX = std::min( mosaicLLX, llCoord1.x );
              
              mosaicLLY = std::min( mosaicLLY, ulCoord1.y );
              mosaicLLY = std::min( mosaicLLY, urCoord1.y );
              mosaicLLY = std::min( mosaicLLY, llCoord1.y );
              mosaicLLY = std::min( mosaicLLY, lrCoord1.y );
              
              mosaicURX = std::max( mosaicURX, ulCoord1.x );
              mosaicURX = std::max( mosaicURX, urCoord1.x );
              mosaicURX = std::max( mosaicURX, lrCoord1.x );
              mosaicURX = std::max( mosaicURX, llCoord1.x );
              
              mosaicURY = std::max( mosaicURY, ulCoord1.y );
              mosaicURY = std::max( mosaicURY, urCoord1.y );
              mosaicURY = std::max( mosaicURY, lrCoord1.y );
              mosaicURY = std::max( mosaicURY, llCoord1.y );
              
              // finding the current raster bounding box polygon (first raster world coordinates)
              
              auxPolygon.clear();
              auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
              auxLinearRingPtr->setPoint( 0, ulCoord1.x, ulCoord1.y );
              auxLinearRingPtr->setPoint( 1, urCoord1.x, urCoord1.y );
              auxLinearRingPtr->setPoint( 2, lrCoord1.x, lrCoord1.y );
              auxLinearRingPtr->setPoint( 3, llCoord1.x, llCoord1.y );
              auxLinearRingPtr->setPoint( 4, ulCoord1.x, ulCoord1.y );
              auxPolygon.push_back( auxLinearRingPtr );
              rastersBBoxes.push_back( auxPolygon );              
            }
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
      
      // creating the output raster
      
      {
        std::vector< te::rst::BandProperty* > bandsProperties;
        for( std::vector< unsigned int >::size_type bandIdx = 0 ;  bandIdx < 
          m_inputParameters.m_inputRastersBands[ 0 ].size() ; ++bandIdx )
        {
          bandsProperties.push_back( new te::rst::BandProperty( mosaicBaseBandProperties ) );
          bandsProperties[ bandIdx ]->m_colorInterp = te::rst::GrayIdxCInt;
          bandsProperties[ bandIdx ]->m_noDataValue = m_inputParameters.m_noDataValue;
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
      }      
      
      // Processing each raster
      
      te::mem::CachedRaster cachedRasterInstance( 
        *(outParamsPtr->m_outputRasterPtr.get()), 25, 0 );
      
      if( useTiePoints )
        return executeTiePointsMosaic( mosaicGeomTransfms, rastersBBoxes,
          cachedRasterInstance );
      else
        return executeGeoMosaic( rastersBBoxes, cachedRasterInstance);
    }

    void Mosaic::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool Mosaic::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();
      
      Mosaic::InputParameters const* inputParamsPtr = dynamic_cast<
        Mosaic::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" ); 
      
      m_inputParameters = *inputParamsPtr;
      
      // Checking the feeder
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_feederRasterPtr, 
        "Invalid m_feederRasterPtr" )
      
      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_feederRasterPtr->getObjsCount() > 1, 
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
      
      // checking other parameters
      
      TERP_TRUE_OR_RETURN_FALSE( 
        ( m_inputParameters.m_tiePoints.size() == 0 ) ? true :
        ( m_inputParameters.m_tiePoints.size() ==
        ( m_inputParameters.m_feederRasterPtr->getObjsCount() - 1 ) ),
        "Bands mismatch" );      
        
      m_isInitialized = true;

      return true;
    }

    bool Mosaic::isInitialized() const
    {
      return m_isInitialized;
    }
    
    bool Mosaic::executeGeoMosaic( 
      const std::vector< te::gm::Polygon >& rastersBBoxes,
      te::rst::Raster& outputRaster )
    {
      // Copyting the first image data to the output mosaic
      
      m_inputParameters.m_feederRasterPtr->reset();
      
      {
        te::rst::Raster const* inputRasterPtr = 
          m_inputParameters.m_feederRasterPtr->getCurrentObj();
        TERP_DEBUG_TRUE_OR_RETURN_FALSE( inputRasterPtr, "Invalid raster pointer" );
        
        const unsigned int nBands = (unsigned int)
          m_inputParameters.m_inputRastersBands[ 0 ].size();
        const unsigned int nRows = inputRasterPtr->getNumberOfRows();
        const unsigned int nCols = inputRasterPtr->getNumberOfColumns();
        unsigned int col = 0;
        unsigned int row = 0;
        
        for( unsigned int inputRastersBandsIdx = 0 ; inputRastersBandsIdx <
          nBands ; ++inputRastersBandsIdx )
        {
          const unsigned int& bandIdx = m_inputParameters.m_inputRastersBands[ 0 ][ 
            inputRastersBandsIdx ];
          
          for( row = 0 ; row < nRows ; ++row )
          {
            for( col = 0 ; col < nCols ; ++col )
            {
            
            }
          }
        }
        
      }
      
      return true;
    }
    
    bool Mosaic::executeTiePointsMosaic( 
      const std::vector< boost::shared_ptr< te::gm::GeometricTransformation > >&
      mosaicGeomTransfms, const std::vector< te::gm::Polygon >& rastersBBoxes,
      te::rst::Raster& outputRaster )
    {
      return false;
    } 
    
    

  } // end namespace rp
}   // end namespace te

