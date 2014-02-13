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
  \file terralib/rp/GeometricRefining.cpp
  \brief Geometric (and positioning) correction of a already geo-referenced raster using a set of reference rasters.
*/

#include "GeometricRefining.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"

namespace te
{
  namespace rp
  {

    GeometricRefining::InputParameters::InputParameters()
    {
      reset();
    }

    GeometricRefining::InputParameters::InputParameters( const InputParameters& other )
    {
      reset();
      operator=( other );
    }

    GeometricRefining::InputParameters::~InputParameters()
    {
      reset();
    }

    void GeometricRefining::InputParameters::reset() throw( te::rp::Exception )
    {
      m_RasterPtr = 0;
      m_inRasterBands2Process.clear();
      m_inRasterTPLocationBands.clear();
      m_referenceRastersPtr = 0;
      m_referenceTPLocationBands.clear();
      m_enableMultiThread = true;
      m_enableProgress = false;
      m_locatorParams.reset();
      m_minRequiredTiePointsCoveredAreaPercent = 0.0;
    }

    const GeometricRefining::InputParameters& GeometricRefining::InputParameters::operator=(
      const GeometricRefining::InputParameters& params )
    {
      reset();

      m_RasterPtr = params.m_RasterPtr;
      m_inRasterBands2Process = params.m_inRasterBands2Process;
      m_inRasterTPLocationBands = params.m_inRasterTPLocationBands;
      m_referenceRastersPtr = params.m_referenceRastersPtr;
      m_referenceTPLocationBands = params.m_referenceTPLocationBands;
      m_enableMultiThread = params.m_enableMultiThread;
      m_enableProgress = params.m_enableProgress;
      m_locatorParams = params.m_locatorParams;
      m_minRequiredTiePointsCoveredAreaPercent = params.m_minRequiredTiePointsCoveredAreaPercent;

      return *this;
    }

    te::common::AbstractParameters* GeometricRefining::InputParameters::clone() const
    {
      return new InputParameters( *this );
    }
    
    // ----------------------------------------------------------------------

    GeometricRefining::OutputParameters::OutputParameters()
    {
      reset();
    }

    GeometricRefining::OutputParameters::OutputParameters( const OutputParameters& other )
    {
      reset();
      operator=( other );
    }

    GeometricRefining::OutputParameters::~OutputParameters()
    {
      reset();
    }

    void GeometricRefining::OutputParameters::reset() throw( te::rp::Exception )
    {
      m_tiePoints.clear();
      m_transformationPtr.reset();
    }

    const GeometricRefining::OutputParameters& GeometricRefining::OutputParameters::operator=(
      const GeometricRefining::OutputParameters& params )
    {
      reset();

      m_tiePoints = params.m_tiePoints;
      m_transformationPtr.reset( params.m_transformationPtr->clone() );

      return *this;
    }

    te::common::AbstractParameters* GeometricRefining::OutputParameters::clone() const
    {
      return new OutputParameters( *this );
    }
    
    // ----------------------------------------------------------------------

    GeometricRefining::GeometricRefining()
    {
      reset();
    }

    GeometricRefining::~GeometricRefining()
    {
    }

    bool GeometricRefining::execute( AlgorithmOutputParameters& outputParams )
      throw( te::rp::Exception )
    {
      if( ! m_isInitialized ) return false;
      
      GeometricRefining::OutputParameters* outParamsPtr = dynamic_cast<
        GeometricRefining::OutputParameters* >( &outputParams );
      TERP_TRUE_OR_THROW( outParamsPtr, "Invalid paramters" );
      
      // progress
      
      std::auto_ptr< te::common::TaskProgress > progressPtr;
      if( m_inputParameters.m_enableProgress )
      {
        progressPtr.reset( new te::common::TaskProgress );
        
        progressPtr->setTotalSteps( 1 );
        
        progressPtr->setMessage( "Mosaicking" );
      }          
      
      // iterating over all reference rasters
      
      return true;
    }

    void GeometricRefining::reset() throw( te::rp::Exception )
    {
      m_inputParameters.reset();
      m_isInitialized = false;
    }

    bool GeometricRefining::initialize( const AlgorithmInputParameters& inputParams )
      throw( te::rp::Exception )
    {
      reset();

      GeometricRefining::InputParameters const* inputParamsPtr = dynamic_cast<
        GeometricRefining::InputParameters const* >( &inputParams );
      TERP_TRUE_OR_THROW( inputParamsPtr, "Invalid paramters pointer" );

      m_inputParameters = *inputParamsPtr;
      
      // checking input raster
      
      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_RasterPtr,
         "Invalid raster pointer" );
         
      TERP_TRUE_OR_RETURN_FALSE( 
        ( m_inputParameters.m_RasterPtr->getAccessPolicy() & te::common::RAccess ),
         "Invalid raster" );  
         
      // Checking input raster bands
      
      for( unsigned int inRasterBands2ProcessIdx = 0 ; inRasterBands2ProcessIdx <
        m_inputParameters.m_inRasterBands2Process.size() ; 
        ++inRasterBands2ProcessIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRasterBands2Process[
          inRasterBands2ProcessIdx ] < m_inputParameters.m_RasterPtr->getNumberOfBands(),
          "Invalid raster bands" ); 
      }
      
      for( unsigned int inRasterTPLocationBandsIdx = 0 ; inRasterTPLocationBandsIdx <
        m_inputParameters.m_inRasterTPLocationBands.size() ; 
        ++inRasterTPLocationBandsIdx )
      {
        TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_inRasterTPLocationBands[
          inRasterTPLocationBandsIdx ] < m_inputParameters.m_RasterPtr->getNumberOfBands(),
          "Invalid raster bands" ); 
      }      

      // Checking the feeder

      TERP_TRUE_OR_RETURN_FALSE( m_inputParameters.m_referenceRastersPtr,
        "Invalid reference rasters feeder pointer" )

      TERP_TRUE_OR_RETURN_FALSE(
        m_inputParameters.m_referenceRastersPtr->getObjsCount() > 0,
        "Invalid number of reference rasters" )

      // checking other parameters
        
      TERP_TRUE_OR_RETURN_FALSE( 
        ( ( m_inputParameters.m_minRequiredTiePointsCoveredAreaPercent >= 0.0 ) &&
        ( m_inputParameters.m_minRequiredTiePointsCoveredAreaPercent <= 100.0 ) ),
        "Invalid parameter m_minRequiredTiePointsCoveredAreaPercent" );

      m_isInitialized = true;

      return true;
    }

    bool GeometricRefining::isInitialized() const
    {
      return m_isInitialized;
    }

    double GeometricRefining::getTPConvexHullArea( 
      const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
      const bool useTPSecondCoordPair ) const
    {
      if( tiePoints.size() < 3 )
      {
        return 0;
      }
      else
      {
        te::gm::MultiPoint points( 0, te::gm::MultiPointType );
        
        for( unsigned int tiePointsIdx = 0 ; tiePointsIdx < tiePoints.size() ;
          ++tiePointsIdx )
        {
          if( useTPSecondCoordPair )
            points.add( new te::gm::Point( tiePoints[ tiePointsIdx ].second.x,
              tiePoints[ tiePointsIdx ].second.y ) );
          else
            points.add( new te::gm::Point( tiePoints[ tiePointsIdx ].first.x,
              tiePoints[ tiePointsIdx ].first.y ) );
        }
        
        std::auto_ptr< te::gm::Polygon > convexHullPolPtr( 
          dynamic_cast< te::gm::Polygon* >( points.convexHull() ) );
        
        if( convexHullPolPtr.get() )
          return convexHullPolPtr->getArea();
        else
          return 0.0;
      }
    }

  } // end namespace rp
}   // end namespace te

