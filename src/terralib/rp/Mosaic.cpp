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
#include "../geometry/Envelope.h"
#include "../srs/Converter.h"

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
      
      double mosaicXResolution = 0.0;
      double mosaicYResolution = 0.0;
      double mosaicLLX = DBL_MAX;
      double mosaicLLY = DBL_MAX;
      double mosaicURX = -1.0 * DBL_MAX;
      double mosaicURY = -1.0 * DBL_MAX;
      int mosaicSRID = 0;
      te::rst::BandProperty mosaicBaseBandProperties( 0, 0, "" );
      {
        te::rst::Raster const* inputRasterPtr = 0;
        unsigned int inputRasterIdx = 0;
        double currentLLX = 0;
        double currentLLY = 0;
        double currentURX = 0;
        double currentURY = 0;
        te::srs::Converter convInstance;
        
        m_inputParameters.m_feederRasterPtr->reset();
        while( ( inputRasterPtr = m_inputParameters.m_feederRasterPtr->operator++() ) )
        {
          inputRasterIdx = m_inputParameters.m_feederRasterPtr->getCurrentOffset();
          TERP_TRUE_OR_RETURN_FALSE( 
            inputRasterPtr->getAccessPolicy() & te::common::RAccess, 
            "Invalid raster" );          
            
          // Defining the base mosaic info
          
          if( m_inputParameters.m_tiePoints.size() > 0 )
          {
            
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
            }
            else
            {
              if( mosaicSRID == inputRasterPtr->getGrid()->getSRID() )
              {
                currentLLX = inputRasterPtr->getGrid()->getExtent()->m_llx;
                currentLLY = inputRasterPtr->getGrid()->getExtent()->m_lly;
                currentURX = inputRasterPtr->getGrid()->getExtent()->m_urx;
                currentURY = inputRasterPtr->getGrid()->getExtent()->m_ury;            
              }
              else
              {
                convInstance.setSourceSRID( inputRasterPtr->getGrid()->getSRID() );
                convInstance.setTargetSRID( mosaicSRID );
                
                convInstance.convert( 
                  inputRasterPtr->getGrid()->getExtent()->m_llx,
                  inputRasterPtr->getGrid()->getExtent()->m_llx,
                  currentLLX,
                  currentLLY );
                convInstance.convert( 
                  inputRasterPtr->getGrid()->getExtent()->m_urx,
                  inputRasterPtr->getGrid()->getExtent()->m_urx,
                  currentURX,
                  currentURY );                                   
              }
              
              mosaicLLX = std::min( mosaicLLX, currentLLX );
              mosaicLLY = std::min( mosaicLLY, currentLLY );
              mosaicURX = std::max( mosaicURX, currentURX );
              mosaicURY = std::max( mosaicURY, currentURY );
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
      
 
      
      return true;
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

  } // end namespace rp
}   // end namespace te

