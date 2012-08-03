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
  \file terralib/rp/TiePointsLocator.cpp
  \brief Tie points locator.
*/

#include "TiePointsLocator.h"
#include "Macros.h"

namespace te
{
  namespace rp
  {
    TiePointsLocator::InputParameters::InputParameters()
    {
      reset();
    }

    TiePointsLocator::InputParameters::~InputParameters()
    {
      reset();
    }

    void TiePointsLocator::InputParameters::reset() throw( te::rp::Exception )
    {
      m_interesPointsLocationStrategy = InputParameters::InvalidStrategyT;
      m_inRaster1Ptr = 0;
      m_inRaster2Ptr = 0;
      m_inRaster1Bands.clear();
      m_inRaster2Bands.clear();
    }

    const TiePointsLocator::InputParameters& TiePointsLocator::InputParameters::operator=(
      const TiePointsLocator::InputParameters& params )
    {
      reset();

      m_interesPointsLocationStrategy = params.m_interesPointsLocationStrategy;
      m_inRaster1Ptr = params.m_inRaster1Ptr;
      m_inRaster2Ptr = params.m_inRaster2Ptr;
      m_inRaster1Bands = params.m_inRaster1Bands;
      m_inRaster2Bands = params.m_inRaster2Bands;

      return *this;
    }

    te::common::AbstractParameters* TiePointsLocator::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    TiePointsLocator::OutputParameters::OutputParameters()
    {
      reset();
    }

    TiePointsLocator::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    TiePointsLocator::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void TiePointsLocator::OutputParameters::reset() throw( te::rp::Exception )
    {
    }

    const TiePointsLocator::OutputParameters& TiePointsLocator::OutputParameters::operator=(
      const TiePointsLocator::OutputParameters& params )
    {
      reset();

      return *this;
    }

    te::common::AbstractParameters* TiePointsLocator::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    TiePointsLocator::TiePointsLocator()
    {
      reset();
    }

    TiePointsLocator::~TiePointsLocator()
    {
    }

    bool TiePointsLocator::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
    }

    void TiePointsLocator::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_outputParametersPtr = 0;
      m_isInitialized = false;
    }

    bool TiePointsLocator::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();
      

      TiePointsLocator::InputParameters const* inputParamsPtr = dynamic_cast<
        TiePointsLocator::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr, "Invalid parameters" );      
      
      // Checking input raster 1

      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inRaster1Ptr,
        "Invalid raster 1 pointer" );
      TERP_TRUE_OR_RETURN_FALSE(
        inputParamsPtr->m_inRaster1Ptr->getAccessPolicy() & te::common::RAccess,
        "Invalid raster 1" );
        
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inRaster1Bands.size() > 0,
        "No given raster 1 band" );

      for( unsigned int inRaster1BandsIdx = 0 ; inRaster1BandsIdx <
        inputParamsPtr->m_inRaster1Bands.size() ; ++inRaster1BandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE(
          inputParamsPtr->m_inRaster1Bands[ inRaster1BandsIdx ] <
          inputParamsPtr->m_inRaster1Ptr->getNumberOfBands(),
          "Invalid input raster 1 band" );
      }
      
      // Checking input raster 2

      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inRaster2Ptr,
        "Invalid raster 2 pointer" );
      TERP_TRUE_OR_RETURN_FALSE(
        inputParamsPtr->m_inRaster2Ptr->getAccessPolicy() & te::common::RAccess,
        "Invalid raster 2" );
        
      TERP_TRUE_OR_RETURN_FALSE( inputParamsPtr->m_inRaster2Bands.size() > 0,
        "No given raster 2 band" );        

      for( unsigned int inRaster2BandsIdx = 0 ; inRaster2BandsIdx <
        inputParamsPtr->m_inRaster2Bands.size() ; ++inRaster2BandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE(
          inputParamsPtr->m_inRaster2Bands[ inRaster2BandsIdx ] <
          inputParamsPtr->m_inRaster2Ptr->getNumberOfBands(),
          "Invalid input raster 2 band" );
      }      

      m_inputParameters = *inputParamsPtr;
      m_isInitialized = true;

      return true;
    }

    bool TiePointsLocator::isInitialized() const
    {
      return m_isInitialized;
    }


  } // end namespace rp
}   // end namespace te

