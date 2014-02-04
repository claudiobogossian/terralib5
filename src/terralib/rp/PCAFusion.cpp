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
  \file terralib/rp/PCAFusion.cpp
  \brief Creation of skeleton imagems.
*/

#include "PCAFusion.h"
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
  
namespace te
{
  namespace rp
  {

    PCAFusion::InputParameters::InputParameters()
    {
      reset();
    }

    PCAFusion::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    PCAFusion::InputParameters::~InputParameters()
    {
      reset();
    }

    void PCAFusion::InputParameters::reset() throw( te::rp::Exception )
    {
      m_lowResRasterPtr = 0;
      m_lowResRasterBands.clear();
      m_highResRasterPtr = 0;
      m_highResRasterBand = 0;
      m_enableProgress = false;
      m_interpMethod = te::rst::Interpolator::NearestNeighbor;
    }

    const PCAFusion::InputParameters& PCAFusion::InputParameters::operator=(
      const PCAFusion::InputParameters& params )
    {
      reset();

      m_lowResRasterPtr = params.m_lowResRasterPtr;
      m_lowResRasterBands = params.m_lowResRasterBands;
      m_highResRasterPtr = params.m_highResRasterPtr;
      m_highResRasterBand = params.m_highResRasterBand;
      m_enableProgress = params.m_enableProgress;
      m_interpMethod = params.m_interpMethod;

      return *this;
    }

    te::common::AbstractParameters* PCAFusion::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    PCAFusion::OutputParameters::OutputParameters()
    {
      reset();
    }

    PCAFusion::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    PCAFusion::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void PCAFusion::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const PCAFusion::OutputParameters& PCAFusion::OutputParameters::operator=(
      const PCAFusion::OutputParameters& params )
    {
      reset();

      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }

    te::common::AbstractParameters* PCAFusion::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    PCAFusion::PCAFusion()
    {
      reset();
    }

    PCAFusion::~PCAFusion()
    {
    }

    bool PCAFusion::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;

      PCAFusion::OutputParameters* outParamsPtr = dynamic_cast<
        PCAFusion::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( 4 );
        
        progressPtr->setMessage( "Fusing images" );
      }        
      
      
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr->pulse();
        if( ! progressPtr->isActive() ) return false;
      }  
      
      return true;
    }

    void PCAFusion::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool PCAFusion::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      PCAFusion::InputParameters const* inputParamsPtr = dynamic_cast<
        PCAFusion::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;

      // Checking the m_lowResRasterPtr and m_lowResRasterBands

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_lowResRasterPtr,
        "Invalid low Resolution Raster Pointer" )

      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_lowResRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );
        
      for( unsigned int lowResRasterBandsIdx = 0 ; lowResRasterBandsIdx <
        inputParamsPtr->m_lowResRasterBands.size() ; ++lowResRasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( 
          inputParamsPtr->m_lowResRasterBands[ lowResRasterBandsIdx ] <
          m_inputParameters.m_lowResRasterPtr->getNumberOfBands(), 
          "Invalid raster band" );   
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

    bool PCAFusion::isInitialized() const
    {
      return m_isInitialized;
    }
    

    
  }
}   // end namespace te

