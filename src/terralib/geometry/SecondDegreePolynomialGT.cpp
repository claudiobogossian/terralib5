/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/SecondDegreePolynomialGT.cpp

  \brief Second Degree Polynomial Geometric transformation.
*/

// TerraLib
#include "../common/MatrixUtils.h"
#include "SecondDegreePolynomialGT.h"

// STL
#include <cmath>


te::gm::SecondDegreePolynomialGT::SecondDegreePolynomialGT()
{  
}

te::gm::SecondDegreePolynomialGT::~SecondDegreePolynomialGT()
{  
}

const std::string& te::gm::SecondDegreePolynomialGT::getName() const
{
  static std::string name( "SecondDegreePolynomial" );
  return name;
}

bool te::gm::SecondDegreePolynomialGT::isValid( const GTParameters& params ) const
{
  return ( ( params.m_directParameters.size() == 12 ) &&
    ( params.m_inverseParameters.size() == 12 ) );
}

void te::gm::SecondDegreePolynomialGT::directMap( const GTParameters& params, const double& pt1X, 
                                                  const double& pt1Y, double& pt2X, double& pt2Y ) const
{
  assert( isValid( params ) );
  
  pt2X =
      params.m_directParameters[ 0 ] +
    ( params.m_directParameters[ 1 ] * pt1X ) +
    ( params.m_directParameters[ 2 ] * pt1Y ) +
    ( params.m_directParameters[ 3 ] * pt1X * pt1Y ) +
    ( params.m_directParameters[ 4 ] * pt1X * pt1X ) +
    ( params.m_directParameters[ 5 ] * pt1Y * pt1Y );
  
  pt2Y =
      params.m_directParameters[ 6 ] +
    ( params.m_directParameters[ 7 ] * pt1X ) +
    ( params.m_directParameters[ 8 ] * pt1Y ) +
    ( params.m_directParameters[ 9 ] * pt1X * pt1Y ) +
    ( params.m_directParameters[ 10 ] * pt1X * pt1X ) +
    ( params.m_directParameters[ 11 ] * pt1Y * pt1Y );  
}

void te::gm::SecondDegreePolynomialGT::inverseMap( const GTParameters& params, const double& pt2X, 
                                                   const double& pt2Y, double& pt1X, double& pt1Y ) const
{
  assert( isValid( params ) );

  pt1X =
      params.m_inverseParameters[ 0 ] +
    ( params.m_inverseParameters[ 1 ] * pt2X ) +
    ( params.m_inverseParameters[ 2 ] * pt2Y ) +
    ( params.m_inverseParameters[ 3 ] * pt2X * pt2Y ) +
    ( params.m_inverseParameters[ 4 ] * pt2X * pt2X ) +
    ( params.m_inverseParameters[ 5 ] * pt2Y * pt2Y );
  
  pt1Y =
      params.m_inverseParameters[ 6 ] +
    ( params.m_inverseParameters[ 7 ] * pt2X ) +
    ( params.m_inverseParameters[ 8 ] * pt2Y ) +
    ( params.m_inverseParameters[ 9 ] * pt2X * pt2Y ) +
    ( params.m_inverseParameters[ 10 ] * pt2X * pt2X ) +
    ( params.m_inverseParameters[ 11 ] * pt2Y * pt2Y );
}
          
unsigned int te::gm::SecondDegreePolynomialGT::getMinRequiredTiePoints() const
{
  return 7;
}

te::gm::GeometricTransformation* te::gm::SecondDegreePolynomialGT::clone() const
{
  te::gm::SecondDegreePolynomialGT* newTransPtr = new SecondDegreePolynomialGT;
  newTransPtr->m_internalParameters = m_internalParameters;
  return newTransPtr;
}
        
bool te::gm::SecondDegreePolynomialGT::computeParameters( GTParameters& params ) const
{
    /* Reference: Remote Sensing - Models and Methods For Image Processing
                  Second Edition
                  Robert A. Schowengerdt
                  Academic Press
    */

  // Creating the equation system parameters
  
  m_computeParameters_tiepointsSize = params.m_tiePoints.size();
  if( m_computeParameters_tiepointsSize < getMinRequiredTiePoints() ) return false;
  
  m_computeParameters_W.resize( m_computeParameters_tiepointsSize, 6 );
  m_computeParameters_WI.resize( m_computeParameters_tiepointsSize, 6 );
  m_computeParameters_X.resize( m_computeParameters_tiepointsSize, 1 );
  m_computeParameters_XI.resize( m_computeParameters_tiepointsSize, 1 );
  m_computeParameters_Y.resize( m_computeParameters_tiepointsSize, 1 );
  m_computeParameters_YI.resize( m_computeParameters_tiepointsSize, 1 );
  
  for ( m_computeParameters_tpIdx = 0 ; m_computeParameters_tpIdx < m_computeParameters_tiepointsSize ; ++m_computeParameters_tpIdx ) 
  {
    const Coord2D& pt1 = params.m_tiePoints[ m_computeParameters_tpIdx ].first;
    
    m_computeParameters_W( m_computeParameters_tpIdx, 0 ) = 1;
    m_computeParameters_W( m_computeParameters_tpIdx, 1 ) = pt1.x;
    m_computeParameters_W( m_computeParameters_tpIdx, 2 ) = pt1.y;
    m_computeParameters_W( m_computeParameters_tpIdx, 3 ) = pt1.x * pt1.y;
    m_computeParameters_W( m_computeParameters_tpIdx, 4 ) = pt1.x * pt1.x;
    m_computeParameters_W( m_computeParameters_tpIdx, 5 ) = pt1.y * pt1.y;
    
    const Coord2D& pt2 = params.m_tiePoints[ m_computeParameters_tpIdx ].second;
    
    m_computeParameters_WI( m_computeParameters_tpIdx, 0 ) = 1;
    m_computeParameters_WI( m_computeParameters_tpIdx, 1 ) = pt2.x;
    m_computeParameters_WI( m_computeParameters_tpIdx, 2 ) = pt2.y;
    m_computeParameters_WI( m_computeParameters_tpIdx, 3 ) = pt2.x * pt2.y;
    m_computeParameters_WI( m_computeParameters_tpIdx, 4 ) = pt2.x * pt2.x;
    m_computeParameters_WI( m_computeParameters_tpIdx, 5 ) = pt2.y * pt2.y;    
    
    m_computeParameters_X( m_computeParameters_tpIdx, 0 ) = pt2.x;
    
    m_computeParameters_XI( m_computeParameters_tpIdx, 0 ) = pt1.x;
    
    m_computeParameters_Y( m_computeParameters_tpIdx, 0 ) = pt2.y;
    
    m_computeParameters_YI( m_computeParameters_tpIdx, 0 ) = pt1.y;    
  }
    
  // Solving...
  
  if( ! te::common::GetPseudoInverseMatrix( m_computeParameters_W, m_computeParameters_PinvW ) ) return false;
  
  if( ! te::common::GetPseudoInverseMatrix( m_computeParameters_WI, m_computeParameters_PinvWI ) ) return false;  
  
  m_computeParameters_A = boost::numeric::ublas::prod( m_computeParameters_PinvW, m_computeParameters_X );
    
  m_computeParameters_AI = boost::numeric::ublas::prod( m_computeParameters_PinvWI, m_computeParameters_XI );
        
  m_computeParameters_B = boost::numeric::ublas::prod( m_computeParameters_PinvW, m_computeParameters_Y );
    
  m_computeParameters_BI = boost::numeric::ublas::prod( m_computeParameters_PinvWI, m_computeParameters_YI );
    
  // Copying the parameters to output
  
  params.m_directParameters.resize( 12 );
  params.m_directParameters[ 0 ] = m_computeParameters_A( 0, 0 );
  params.m_directParameters[ 1 ] = m_computeParameters_A( 1, 0 );
  params.m_directParameters[ 2 ] = m_computeParameters_A( 2, 0 );
  params.m_directParameters[ 3 ] = m_computeParameters_A( 3, 0 );
  params.m_directParameters[ 4 ] = m_computeParameters_A( 4, 0 );
  params.m_directParameters[ 5 ] = m_computeParameters_A( 5, 0 );
  params.m_directParameters[ 6 ] = m_computeParameters_B( 0, 0 );
  params.m_directParameters[ 7 ] = m_computeParameters_B( 1, 0 );
  params.m_directParameters[ 8 ] = m_computeParameters_B( 2, 0 );
  params.m_directParameters[ 9 ] = m_computeParameters_B( 3, 0 );
  params.m_directParameters[ 10 ] = m_computeParameters_B( 4, 0 );
  params.m_directParameters[ 11 ] = m_computeParameters_B( 5, 0 );
  
  params.m_inverseParameters.resize( 12 );
  params.m_inverseParameters[ 0 ] = m_computeParameters_AI( 0, 0 );
  params.m_inverseParameters[ 1 ] = m_computeParameters_AI( 1, 0 );
  params.m_inverseParameters[ 2 ] = m_computeParameters_AI( 2, 0 );
  params.m_inverseParameters[ 3 ] = m_computeParameters_AI( 3, 0 );
  params.m_inverseParameters[ 4 ] = m_computeParameters_AI( 4, 0 );
  params.m_inverseParameters[ 5 ] = m_computeParameters_AI( 5, 0 );
  params.m_inverseParameters[ 6 ] = m_computeParameters_BI( 0, 0 );
  params.m_inverseParameters[ 7 ] = m_computeParameters_BI( 1, 0 );
  params.m_inverseParameters[ 8 ] = m_computeParameters_BI( 2, 0 );
  params.m_inverseParameters[ 9 ] = m_computeParameters_BI( 3, 0 );
  params.m_inverseParameters[ 10 ] = m_computeParameters_BI( 4, 0 );
  params.m_inverseParameters[ 11 ] = m_computeParameters_BI( 5, 0 );
  
  return true;
}


