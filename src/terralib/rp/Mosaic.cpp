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
    }

    const Mosaic::InputParameters& Mosaic::InputParameters::operator=(
      const Mosaic::InputParameters& params )
    {
      reset();
      
      m_feederRasterPtr = params.m_feederRasterPtr;

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
      m_outputRasterPtr.clear();
    }

    const Mosaic::OutputParameters& Mosaic::OutputParameters::operator=(
      const Mosaic::OutputParameters& params )
    {
      reset();
      
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
      
        
      m_isInitialized = true;

      return true;
    }

    bool Mosaic::isInitialized() const
    {
      return m_isInitialized;
    }

  } // end namespace rp
}   // end namespace te

