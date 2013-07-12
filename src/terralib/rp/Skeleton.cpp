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
  \file terralib/rp/Skeleton.cpp
  \brief Creation of skeleton imagems.
*/

#include "Skeleton.h"
#include "Macros.h"


namespace te
{
  namespace rp
  {

    Skeleton::InputParameters::InputParameters()
    {
      reset();
    }

    Skeleton::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    Skeleton::InputParameters::~InputParameters()
    {
      reset();
    }

    void Skeleton::InputParameters::reset() throw( te::rp::Exception )
    {
      m_inputRasterPtr = 0;
      m_inputRasterBands.clear();
    }

    const Skeleton::InputParameters& Skeleton::InputParameters::operator=(
      const Skeleton::InputParameters& params )
    {
      reset();

      m_inputRasterPtr = params.m_inputRasterPtr;
      m_inputRasterBands = params.m_inputRasterBands;

      return *this;
    }

    te::common::AbstractParameters* Skeleton::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }

    Skeleton::OutputParameters::OutputParameters()
    {
      reset();
    }

    Skeleton::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    Skeleton::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void Skeleton::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_rType.clear();
      m_rInfo.clear();
      m_outputRasterPtr.reset();
    }

    const Skeleton::OutputParameters& Skeleton::OutputParameters::operator=(
      const Skeleton::OutputParameters& params )
    {
      reset();

      m_rType = params.m_rType;
      m_rInfo = params.m_rInfo;

      return *this;
    }

    te::common::AbstractParameters* Skeleton::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }

    Skeleton::Skeleton()
    {
      reset();
    }

    Skeleton::~Skeleton()
    {
    }

    bool Skeleton::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;

      Skeleton::OutputParameters* outParamsPtr = dynamic_cast<
        Skeleton::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      te::rp::Matrix< double > magnitudes;
      magnitudes.reset( te::rp::Matrix< double >::AutoMemPol ); 
      TERP_TRUE_OR_RETURN_FALSE( buildEdgeMap( magnitudes ),
        "Edge map build error" );      
        
      te::rp::Matrix< double > gradVecField;
      gradVecField.reset( te::rp::Matrix< double >::AutoMemPol ); 
      TERP_TRUE_OR_RETURN_FALSE( buildGradientVectorField( magnitudes, gradVecField ),
        "Edge map build error" );   
        
      magnitudes.reset();

      return true;
    }

    void Skeleton::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool Skeleton::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      Skeleton::InputParameters const* inputParamsPtr = dynamic_cast<
        Skeleton::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;

      // Checking the input raster

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRasterPtr,
        "Invalid m_inputRasterPtr" )

      TERP_TRUE_OR_RETURN_FALSE( 
        m_inputParameters.m_inputRasterPtr->getAccessPolicy() & te::common::RAccess, 
        "Invalid raster" );

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inputRasterBands.size() > 0,
        "Invalid raster bands number" );
        
      for( unsigned int inRasterBandsIdx = 0 ; inRasterBandsIdx < 
        m_inputParameters.m_inputRasterBands.size() ; ++inRasterBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( 
          m_inputParameters.m_inputRasterBands[ inRasterBandsIdx ] <
          m_inputParameters.m_inputRasterPtr->getNumberOfBands(), 
          "Invalid raster bands" );
      }        

      m_isInitialized = true;

      return true;
    }

    bool Skeleton::isInitialized() const
    {
      return m_isInitialized;
    }

    bool Skeleton::buildEdgeMap( te::rp::Matrix< double >& magnitudes ) const
    {
      const unsigned int nRows = m_inputParameters.m_inputRasterPtr->getNumberOfRows();
      const unsigned int nCols = m_inputParameters.m_inputRasterPtr->getNumberOfColumns();
      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;
      
      if( ! magnitudes.reset( nRows,nCols ) )
        return false;
      
      unsigned int row = 0;
      unsigned int col = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        magnitudes[ row ][ 0 ] = 0;
        magnitudes[ row ][ lastColIdx ] = 0;
      }
      
      for( col = 0 ; col < nCols ; ++col )
      {
        magnitudes[ row ][ col ] = 0;
        magnitudes[ lastRowIdx ][ col ] = 0;
      }     
      
      double diffX = 0;
      double diffY = 0;
      
      for( row = 1 ; row < lastRowIdx ; ++row )
        for( col = 1 ; col < lastColIdx ; ++col )
        {
          diffX = magnitudes[ col + 1 ][ row ] - magnitudes[ col - 1 ][ row ];
          diffY = magnitudes[ col ][ row - 1 ] - magnitudes[ col ][ row + 1 ];
          magnitudes[ row ][ col ] = std::sqrt( ( diffX * diffX ) + ( diffY *
            diffY ) );
        }
    }
    
    bool Skeleton::buildGradientVectorField( const te::rp::Matrix< double >& magnitudes,
      te::rp::Matrix< double >& gradVecField ) const
    {
      const unsigned int nRows = magnitudes.getLinesNumber();
      const unsigned int nCols = magnitudes.getColumnsNumber();
      const unsigned int lastRowIdx = nRows - 1;
      const unsigned int lastColIdx = nCols - 1;
      
      if( ! gradVecField.reset( nRows,nCols ) )
        return false;
      
      unsigned int row = 0;
      unsigned int col = 0;
      
      for( row = 0 ; row < nRows ; ++row )
      {
        gradVecField[ row ][ 0 ] = 0;
        gradVecField[ row ][ lastColIdx ] = 0;
      }
      
      for( col = 0 ; col < nCols ; ++col )
      {
        gradVecField[ row ][ col ] = 0;
        gradVecField[ lastRowIdx ][ col ] = 0;
      }     
      
      for( row = 1 ; row < lastRowIdx ; ++row )
        for( col = 1 ; col < lastColIdx ; ++col )
        {

        }
    }    

  } // end namespace rp
}   // end namespace te

