/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/GeometricTransformation.cpp

  \brief 2D Geometric transformation base class.
*/

// TerraLib
#include "GeometricTransformation.h"

#include <cmath>

te::gm::GeometricTransformation::GeometricTransformation()
{  
}

te::gm::GeometricTransformation::~GeometricTransformation()
{  
}

bool te::gm::GeometricTransformation::initialize( const GTParameters& newParameters )
{
  /* If previous calculated parameters were supplied, no need to do calcules */

  if( isValid( newParameters ) ) 
  {
    m_internalParameters_ = newParameters;
    
    return true;
  } 
  else 
  {
    /* No previous parameters given - Need to calculate the new transformation
       parameters */
    
    return computeParameters( m_internalParameters_ );
  }
}

double te::gm::GeometricTransformation::getMaxDirectMappingError( 
  const GTParameters& params ) const
{
  assert( isValid( params ) );
    
  const unsigned int tiepointsSize = (unsigned int)params.m_tiePoints.size();
  
  double maxError = 0;
  double currentError = 0;
  
  for( unsigned int tpIndex = 0 ; tpIndex < tiepointsSize ; ++tpIndex ) 
  {
    currentError = getDirectMappingError( params.m_tiePoints[ tpIndex ], 
      params );
    
    if( currentError > maxError ) 
    {
      maxError = currentError;
    }
  }
  
  return maxError;
}

double te::gm::GeometricTransformation::getMaxInverseMappingError( 
  const GTParameters& params ) const
{
  assert( isValid( params ) );
    
  const unsigned int tiepointsSize = (unsigned int)params.m_tiePoints.size();
  
  double maxError = 0;
  double currentError = 0;
  
  for( unsigned int tpIndex = 0 ; tpIndex < tiepointsSize ; ++tpIndex ) 
  {
    currentError = getInverseMappingError( params.m_tiePoints[ tpIndex ], 
      params );
    
    if( currentError > maxError ) 
    {
      maxError = currentError;
    }
  }
  
  return maxError;
}

double te::gm::GeometricTransformation::getDirectMapRMSE( 
  const GTParameters& params ) const
{
  assert( isValid( params ) );
    
  const unsigned int tiepointsSize = (unsigned int)params.m_tiePoints.size();
  
  if( tiepointsSize == 0 )
  {
    return 0;
  }
  else
  {
    double error2Sum = 0;
    double currentError = 0;
    
    for( unsigned int tpIndex = 0 ; tpIndex < tiepointsSize ; ++tpIndex ) 
    {
      currentError = getDirectMappingError( params.m_tiePoints[ tpIndex ], 
        params );
        
      error2Sum += ( currentError * currentError );
    }
    
    return sqrt( error2Sum / ( (double)tiepointsSize ) );
  }
}

double te::gm::GeometricTransformation::getInverseMapRMSE( 
  const GTParameters& params ) const
{
  assert( isValid( params ) );
    
  const unsigned int tiepointsSize = (unsigned int)params.m_tiePoints.size();
  
  if( tiepointsSize == 0 )
  {
    return 0;
  }
  else
  {
    double error2Sum = 0;
    double currentError = 0;
    
    for( unsigned int tpIndex = 0 ; tpIndex < tiepointsSize ; ++tpIndex ) 
    {
      currentError = getInverseMappingError( params.m_tiePoints[ tpIndex ], 
        params );
        
      error2Sum += ( currentError * currentError );
    }
    
    return sqrt( error2Sum / ( (double)tiepointsSize ) );
  }  
}

double te::gm::GeometricTransformation::getDirectMappingError( 
  const GTParameters::TiePoint& tiePoint, const GTParameters& params ) const
{
  assert( isValid( params ) );
    
  Coord2D directMappedPoint;

  directMap( params, tiePoint.first, directMappedPoint );
    
  double diffX = tiePoint.second.x - directMappedPoint.x;
  double diffY = tiePoint.second.y - directMappedPoint.y;
    
  return hypot( diffX, diffY );
}

double te::gm::GeometricTransformation::getInverseMappingError( 
  const GTParameters::TiePoint& tiePoint, const GTParameters& params ) const
{
  assert( isValid( params ) );
    
  Coord2D inverseMappedPoint;

  inverseMap( params, tiePoint.second, inverseMappedPoint );
    
  double diffX = tiePoint.first.x - inverseMappedPoint.x;
  double diffY = tiePoint.first.y - inverseMappedPoint.y;
    
  return hypot( diffX, diffY );
}
