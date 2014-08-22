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
  \file terralib/rp/WisperFusion.cpp
  \brief Creation of skeleton imagems.
*/

#include "WisperFusion.h"
#include "Macros.h"
#include "Functions.h"

#include "../raster/BandProperty.h"
#include "../raster/RasterFactory.h"
#include "../raster/Band.h"
#include "../raster/Grid.h"
#include "../geometry/Envelope.h"
#include "../common/progress/TaskProgress.h"

#include <cmath>
#include <limits>

#ifndef M_PI
  #define M_PI       3.14159265358979323846
#endif
  
namespace te
{
  namespace rp
  {

    WisperFusion::InputParameters::InputParameters()
    {
      reset();
    }

    WisperFusion::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    WisperFusion::InputParameters::~InputParameters()
    {
      reset();
    }

    void WisperFusion::InputParameters::reset() throw( te::rp::Exception )
    {
      m_lowResRasterPtr = 0;
      m_lowResRasterBands.clear();
      m_highResRasterPtr = 0;
      m_highResRasterBand = 0;
      m_enableProgress = false;
      m_interpMethod = te::rst::Interpolator::NearestNeighbor;
      m_filterType = te::rp::B3SplineFilter;
    }

    const WisperFusion::InputParameters& WisperFusion::InputParameters::operator=(
      const WisperFusion::InputParameters& params )
    {
      reset();

      m_lowResRasterPtr = params.m_lowResRasterPtr;
      m_lowResRasterBands = params.m_lowResRasterBands;
      m_highResRasterPtr = params.m_highResRasterPtr;
      m_highResRasterBand = params.m_highResRasterBand;
      m_enableProgress = params.m_enableProgress;
      m_interpMethod = params.m_interpMethod;
      m_filterType = params.m_filterType;

      return *this;
    }

    te::common::AbstractParameters* WisperFusion::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    WisperFusion::OutputParameters::OutputParameters()
    {
      reset();
    }

    WisperFusion::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    WisperFusion::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void WisperFusion::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const WisperFusion::OutputParameters& WisperFusion::OutputParameters::operator=(
      const WisperFusion::OutputParameters& params )
    {
      reset();

      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }

    te::common::AbstractParameters* WisperFusion::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    WisperFusion::WisperFusion()
    {
      reset();
    }

    WisperFusion::~WisperFusion()
    {
    }

    bool WisperFusion::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;

      WisperFusion::OutputParameters* outParamsPtr = dynamic_cast<
        WisperFusion::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( 4 );
        
        progressPtr->setMessage( "Fusing images" );
      }        
      
      // defining the number of blocks each memory raster must have
      
      unsigned int maxNumberOfRasterBlocks = 1;
      
      {
        
      }
      
      // creating the ressampled input raster
      
      std::auto_ptr< te::rst::Raster > resampledLlowResRasterPtr;
      
      {
        std::map< std::string, std::string > rinfo;
        rinfo["MAXNUMBEROFRAMBLOCKS"] = boost::lexical_cast< std::string >(
          maxNumberOfRasterBlocks );  
        
        TERP_TRUE_OR_RETURN_FALSE( te::rp::RasterResample(
          *m_inputParameters.m_lowResRasterPtr,
          m_inputParameters.m_lowResRasterBands,
          m_inputParameters.m_interpMethod,
          0,
          0,
          m_inputParameters.m_lowResRasterPtr->getNumberOfRows(),
          m_inputParameters.m_lowResRasterPtr->getNumberOfColumns(),
          m_inputParameters.m_highResRasterPtr->getNumberOfRows(),
          m_inputParameters.m_highResRasterPtr->getNumberOfColumns(),  
          rinfo,
          "EXPANSIBLE",
          resampledLlowResRasterPtr ), 
          "Low resolution raster resample error" );
      }


      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }              
      
      return true;
    }

    void WisperFusion::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool WisperFusion::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      WisperFusion::InputParameters const* inputParamsPtr = dynamic_cast<
        WisperFusion::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;

      // Checking the m_lowResRasterPtr and m_lowResRasterBands

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_lowResRasterPtr,
        "Invalid low Resolution Raster Pointer" )

      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_lowResRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );
      
      for( unsigned int lowResRasterBandsIdx = 0; lowResRasterBandsIdx <
        m_inputParameters.m_lowResRasterBands.size() ; 
        ++lowResRasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_lowResRasterBands[ lowResRasterBandsIdx ] <
          m_inputParameters.m_lowResRasterPtr->getNumberOfBands(), 
          "Invalid low resolution raster band" );   
      }
        
      // Checking the m_highResRasterPtr and m_highResRasterBand

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_highResRasterPtr,
        "Invalid high resolution Raster Pointer" )

      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_highResRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );
        
      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_highResRasterBand <
        m_inputParameters.m_highResRasterPtr->getNumberOfBands(), 
        "Invalid raster band" );   
        
      m_isInitialized = true;

      return true;
    }

    bool WisperFusion::isInitialized() const
    {
      return m_isInitialized;
    }
    

    
  }
}   // end namespace te

