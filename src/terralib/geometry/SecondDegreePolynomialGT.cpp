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

// Boost
#include <boost/numeric/ublas/matrix.hpp>

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
};
        
bool te::gm::SecondDegreePolynomialGT::computeParameters( GTParameters& params ) const
{
    /* Reference: Remote Sensing - Models and Methods For Image Processing
                  Second Edition
                  Robert A. Schowengerdt
                  Academic Press
    */

  // Creating the equation system parameters
  
  const unsigned int tiepointsSize = params.m_tiePoints.size();
  if( tiepointsSize < getMinRequiredTiePoints() ) return false;
  
  boost::numeric::ublas::matrix< double > W( tiepointsSize, 6 );
  boost::numeric::ublas::matrix< double > WI( tiepointsSize, 6 );
  boost::numeric::ublas::matrix< double > X( tiepointsSize, 1 );
  boost::numeric::ublas::matrix< double > XI( tiepointsSize, 1 );
  boost::numeric::ublas::matrix< double > Y( tiepointsSize, 1 );
  boost::numeric::ublas::matrix< double > YI( tiepointsSize, 1 );
  
  for ( unsigned int tpIdx = 0 ; tpIdx < tiepointsSize ; ++tpIdx ) 
  {
    const Coord2D& pt1 = params.m_tiePoints[ tpIdx ].first;
    
    W( tpIdx, 0 ) = 1;
    W( tpIdx, 1 ) = pt1.x;
    W( tpIdx, 2 ) = pt1.y;
    W( tpIdx, 3 ) = pt1.x * pt1.y;
    W( tpIdx, 4 ) = pt1.x * pt1.x;
    W( tpIdx, 5 ) = pt1.y * pt1.y;
    
    const Coord2D& pt2 = params.m_tiePoints[ tpIdx ].second;
    
    WI( tpIdx, 0 ) = 1;
    WI( tpIdx, 1 ) = pt2.x;
    WI( tpIdx, 2 ) = pt2.y;
    WI( tpIdx, 3 ) = pt2.x * pt2.y;
    WI( tpIdx, 4 ) = pt2.x * pt2.x;
    WI( tpIdx, 5 ) = pt2.y * pt2.y;    
    
    X( tpIdx, 0 ) = pt2.x;
    
    XI( tpIdx, 0 ) = pt1.x;
    
    Y( tpIdx, 0 ) = pt2.y;
    
    YI( tpIdx, 0 ) = pt1.y;    
  }
    
  // Solving...
  
  boost::numeric::ublas::matrix< double > PinvW;
  if( ! te::common::GetPseudoInverseMatrix( W, PinvW ) ) return false;
  
  boost::numeric::ublas::matrix< double > PinvWI;
  if( ! te::common::GetPseudoInverseMatrix( WI, PinvWI ) ) return false;  
  
  boost::numeric::ublas::matrix< double > A( boost::numeric::ublas::prod( PinvW, X ) );
    
  boost::numeric::ublas::matrix< double > AI( boost::numeric::ublas::prod( PinvWI, XI ) );
        
  boost::numeric::ublas::matrix< double > B( boost::numeric::ublas::prod( PinvW, Y ) );
    
  boost::numeric::ublas::matrix< double > BI( boost::numeric::ublas::prod( PinvWI, YI ) );
    
  // Copying the parameters to output
  
  params.m_directParameters.resize( 12 );
  params.m_directParameters[ 0 ] = A( 0, 0 );
  params.m_directParameters[ 1 ] = A( 1, 0 );
  params.m_directParameters[ 2 ] = A( 2, 0 );
  params.m_directParameters[ 3 ] = A( 3, 0 );
  params.m_directParameters[ 4 ] = A( 4, 0 );
  params.m_directParameters[ 5 ] = A( 5, 0 );
  params.m_directParameters[ 6 ] = B( 0, 0 );
  params.m_directParameters[ 7 ] = B( 1, 0 );
  params.m_directParameters[ 8 ] = B( 2, 0 );
  params.m_directParameters[ 9 ] = B( 3, 0 );
  params.m_directParameters[ 10 ] = B( 4, 0 );
  params.m_directParameters[ 11 ] = B( 5, 0 );
  
  params.m_inverseParameters.resize( 12 );
  params.m_inverseParameters[ 0 ] = AI( 0, 0 );
  params.m_inverseParameters[ 1 ] = AI( 1, 0 );
  params.m_inverseParameters[ 2 ] = AI( 2, 0 );
  params.m_inverseParameters[ 3 ] = AI( 3, 0 );
  params.m_inverseParameters[ 4 ] = AI( 4, 0 );
  params.m_inverseParameters[ 5 ] = AI( 5, 0 );
  params.m_inverseParameters[ 6 ] = BI( 0, 0 );
  params.m_inverseParameters[ 7 ] = BI( 1, 0 );
  params.m_inverseParameters[ 8 ] = BI( 2, 0 );
  params.m_inverseParameters[ 9 ] = BI( 3, 0 );
  params.m_inverseParameters[ 10 ] = BI( 4, 0 );
  params.m_inverseParameters[ 11 ] = BI( 5, 0 );
  
  return true;
}


