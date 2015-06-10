/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/geometry/ThirdDegreePolynomialGT.cpp

  \brief Third Degree Polynomial Geometric transformation.
*/

// TerraLib
#include "../common/MatrixUtils.h"
#include "ThirdDegreePolynomialGT.h"

// STL
#include <cmath>


te::gm::ThirdDegreePolynomialGT::ThirdDegreePolynomialGT()
{  
}

te::gm::ThirdDegreePolynomialGT::~ThirdDegreePolynomialGT()
{  
}

const std::string& te::gm::ThirdDegreePolynomialGT::getName() const
{
  static std::string name( "ThirdDegreePolynomial" );
  return name;
}

bool te::gm::ThirdDegreePolynomialGT::isValid( const GTParameters& params ) const
{
  return ( ( params.m_directParameters.size() == 20 ) &&
    ( params.m_inverseParameters.size() == 20 ) );
}

void te::gm::ThirdDegreePolynomialGT::directMap( const GTParameters& params, const double& pt1X, 
                                                 const double& pt1Y, double& pt2X, double& pt2Y ) const
{
  assert( isValid( params ) );
  
  pt2X =
      params.m_directParameters[ 0 ] +
    ( params.m_directParameters[ 1 ] * pt1X ) +
    ( params.m_directParameters[ 2 ] * pt1Y ) +
    ( params.m_directParameters[ 3 ] * pt1X * pt1X ) +
    ( params.m_directParameters[ 4 ] * pt1X * pt1Y ) +
    ( params.m_directParameters[ 5 ] * pt1Y * pt1Y ) +
    ( params.m_directParameters[ 6 ] * pt1X * pt1X * pt1X ) +
    ( params.m_directParameters[ 7 ] * pt1X * pt1X * pt1Y ) +
    ( params.m_directParameters[ 8 ] * pt1X * pt1Y * pt1Y ) +
    ( params.m_directParameters[ 9 ] * pt1Y * pt1Y * pt1Y );

  pt2Y =
      params.m_directParameters[ 10 ] +
    ( params.m_directParameters[ 11 ] * pt1X ) +
    ( params.m_directParameters[ 12 ] * pt1Y ) +
    ( params.m_directParameters[ 13 ] * pt1X * pt1X ) +
    ( params.m_directParameters[ 14 ] * pt1X * pt1Y ) +
    ( params.m_directParameters[ 15 ] * pt1Y * pt1Y ) +
    ( params.m_directParameters[ 16 ] * pt1X * pt1X * pt1X ) +
    ( params.m_directParameters[ 17 ] * pt1X * pt1X * pt1Y ) +
    ( params.m_directParameters[ 18 ] * pt1X * pt1Y * pt1Y ) +
    ( params.m_directParameters[ 19 ] * pt1Y * pt1Y * pt1Y );
}

void te::gm::ThirdDegreePolynomialGT::inverseMap( const GTParameters& params, const double& pt2X, 
                                                  const double& pt2Y, double& pt1X, double& pt1Y ) const
{
  assert( isValid( params ) );
  
  pt1X =
      params.m_inverseParameters[ 0 ] +
    ( params.m_inverseParameters[ 1 ] * pt2X ) +
    ( params.m_inverseParameters[ 2 ] * pt2Y ) +
    ( params.m_inverseParameters[ 3 ] * pt2X * pt2X ) +
    ( params.m_inverseParameters[ 4 ] * pt2X * pt2Y ) +
    ( params.m_inverseParameters[ 5 ] * pt2Y * pt2Y ) +
    ( params.m_inverseParameters[ 6 ] * pt2X * pt2X * pt2X ) +
    ( params.m_inverseParameters[ 7 ] * pt2X * pt2X * pt2Y ) +
    ( params.m_inverseParameters[ 8 ] * pt2X * pt2Y * pt2Y ) +
    ( params.m_inverseParameters[ 9 ] * pt2Y * pt2Y * pt2Y );

  pt1Y =
      params.m_inverseParameters[ 10 ] +
    ( params.m_inverseParameters[ 11 ] * pt2X ) +
    ( params.m_inverseParameters[ 12 ] * pt2Y ) +
    ( params.m_inverseParameters[ 13 ] * pt2X * pt2X ) +
    ( params.m_inverseParameters[ 14 ] * pt2X * pt2Y ) +
    ( params.m_inverseParameters[ 15 ] * pt2Y * pt2Y ) +
    ( params.m_inverseParameters[ 16 ] * pt2X * pt2X * pt2X ) +
    ( params.m_inverseParameters[ 17 ] * pt2X * pt2X * pt2Y ) +
    ( params.m_inverseParameters[ 18 ] * pt2X * pt2Y * pt2Y ) +
    ( params.m_inverseParameters[ 19 ] * pt2Y * pt2Y * pt2Y );  

}
          
unsigned int te::gm::ThirdDegreePolynomialGT::getMinRequiredTiePoints() const
{
  return 11;
}

te::gm::GeometricTransformation* te::gm::ThirdDegreePolynomialGT::clone() const
{
  te::gm::ThirdDegreePolynomialGT* newTransPtr = new ThirdDegreePolynomialGT;
  newTransPtr->m_internalParameters = m_internalParameters;
  return newTransPtr;
}
        
bool te::gm::ThirdDegreePolynomialGT::computeParameters( GTParameters& params ) const
{
  // Creating the equation system parameters
  
  m_computeParameters_tiepointsSize = params.m_tiePoints.size();
  if( m_computeParameters_tiepointsSize < getMinRequiredTiePoints() ) return false;
  
  m_computeParameters_W.resize( m_computeParameters_tiepointsSize, 10 );
  m_computeParameters_WI.resize( m_computeParameters_tiepointsSize, 10 );
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
    m_computeParameters_W( m_computeParameters_tpIdx, 3 ) = pt1.x * pt1.x;
    m_computeParameters_W( m_computeParameters_tpIdx, 4 ) = pt1.x * pt1.y;
    m_computeParameters_W( m_computeParameters_tpIdx, 5 ) = pt1.y * pt1.y;
    m_computeParameters_W( m_computeParameters_tpIdx, 6 ) = pt1.x * pt1.x * pt1.x;
    m_computeParameters_W( m_computeParameters_tpIdx, 7 ) = pt1.x * pt1.x * pt1.y;
    m_computeParameters_W( m_computeParameters_tpIdx, 8 ) = pt1.x * pt1.y * pt1.y;
    m_computeParameters_W( m_computeParameters_tpIdx, 9 ) = pt1.y * pt1.y * pt1.y;
    
    const Coord2D& pt2 = params.m_tiePoints[ m_computeParameters_tpIdx ].second;
    
    m_computeParameters_WI( m_computeParameters_tpIdx, 0 ) = 1;
    m_computeParameters_WI( m_computeParameters_tpIdx, 1 ) = pt2.x;
    m_computeParameters_WI( m_computeParameters_tpIdx, 2 ) = pt2.y;
    m_computeParameters_WI( m_computeParameters_tpIdx, 3 ) = pt2.x * pt2.x;
    m_computeParameters_WI( m_computeParameters_tpIdx, 4 ) = pt2.x * pt2.y;
    m_computeParameters_WI( m_computeParameters_tpIdx, 5 ) = pt2.y * pt2.y;
    m_computeParameters_WI( m_computeParameters_tpIdx, 6 ) = pt2.x * pt2.x * pt2.x;
    m_computeParameters_WI( m_computeParameters_tpIdx, 7 ) = pt2.x * pt2.x * pt2.y;
    m_computeParameters_WI( m_computeParameters_tpIdx, 8 ) = pt2.x * pt2.y * pt2.y;
    m_computeParameters_WI( m_computeParameters_tpIdx, 9 ) = pt2.y * pt2.y * pt2.y;
    
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
  
  params.m_directParameters.resize( 20 );
  params.m_directParameters[ 0 ] = m_computeParameters_A( 0, 0 );
  params.m_directParameters[ 1 ] = m_computeParameters_A( 1, 0 );
  params.m_directParameters[ 2 ] = m_computeParameters_A( 2, 0 );
  params.m_directParameters[ 3 ] = m_computeParameters_A( 3, 0 );
  params.m_directParameters[ 4 ] = m_computeParameters_A( 4, 0 );
  params.m_directParameters[ 5 ] = m_computeParameters_A( 5, 0 );
  params.m_directParameters[ 6 ] = m_computeParameters_A( 6, 0 );
  params.m_directParameters[ 7 ] = m_computeParameters_A( 7, 0 );
  params.m_directParameters[ 8 ] = m_computeParameters_A( 8, 0 );
  params.m_directParameters[ 9 ] = m_computeParameters_A( 9, 0 );
  params.m_directParameters[ 10 ] = m_computeParameters_B( 0, 0 );
  params.m_directParameters[ 11 ] = m_computeParameters_B( 1, 0 );
  params.m_directParameters[ 12 ] = m_computeParameters_B( 2, 0 );
  params.m_directParameters[ 13 ] = m_computeParameters_B( 3, 0 );
  params.m_directParameters[ 14 ] = m_computeParameters_B( 4, 0 );
  params.m_directParameters[ 15 ] = m_computeParameters_B( 5, 0 );
  params.m_directParameters[ 16 ] = m_computeParameters_B( 6, 0 );
  params.m_directParameters[ 17 ] = m_computeParameters_B( 7, 0 );
  params.m_directParameters[ 18 ] = m_computeParameters_B( 8, 0 );
  params.m_directParameters[ 19 ] = m_computeParameters_B( 9, 0 );
  
  params.m_inverseParameters.resize( 20 );
  params.m_inverseParameters[ 0 ] = m_computeParameters_AI( 0, 0 );
  params.m_inverseParameters[ 1 ] = m_computeParameters_AI( 1, 0 );
  params.m_inverseParameters[ 2 ] = m_computeParameters_AI( 2, 0 );
  params.m_inverseParameters[ 3 ] = m_computeParameters_AI( 3, 0 );
  params.m_inverseParameters[ 4 ] = m_computeParameters_AI( 4, 0 );
  params.m_inverseParameters[ 5 ] = m_computeParameters_AI( 5, 0 );
  params.m_inverseParameters[ 6 ] = m_computeParameters_AI( 6, 0 );
  params.m_inverseParameters[ 7 ] = m_computeParameters_AI( 7, 0 );
  params.m_inverseParameters[ 8 ] = m_computeParameters_AI( 8, 0 );
  params.m_inverseParameters[ 9 ] = m_computeParameters_AI( 9, 0 );
  params.m_inverseParameters[ 10 ] = m_computeParameters_BI( 0, 0 );
  params.m_inverseParameters[ 11 ] = m_computeParameters_BI( 1, 0 );
  params.m_inverseParameters[ 12 ] = m_computeParameters_BI( 2, 0 );
  params.m_inverseParameters[ 13 ] = m_computeParameters_BI( 3, 0 );
  params.m_inverseParameters[ 14 ] = m_computeParameters_BI( 4, 0 );
  params.m_inverseParameters[ 15 ] = m_computeParameters_BI( 5, 0 );
  params.m_inverseParameters[ 16 ] = m_computeParameters_BI( 6, 0 );
  params.m_inverseParameters[ 17 ] = m_computeParameters_BI( 7, 0 );
  params.m_inverseParameters[ 18 ] = m_computeParameters_BI( 8, 0 );
  params.m_inverseParameters[ 19 ] = m_computeParameters_BI( 9, 0 );

  return true;
}

