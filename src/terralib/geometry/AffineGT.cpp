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
  \file terralib/geometry/AffineGT.cpp

  \brief 2D Geometric transformation.
*/

// TerraLib
#include "../common/MatrixUtils.h"
#include "AffineGT.h"

// STL
#include <cmath>

// Boost
#include <boost/numeric/ublas/matrix.hpp>

#define TETRANSFORMATIONNAME "Affine"

te::gm::AffineGT::AffineGT()
{  
}

te::gm::AffineGT::~AffineGT()
{  
}

const std::string& te::gm::AffineGT::getName() const
{
  static std::string name( TETRANSFORMATIONNAME );
  return name;
}

bool te::gm::AffineGT::isValid( const GTParameters& params ) const
{
  return ( ( params.m_directParameters.size() == 6 ) &&
    ( params.m_inverseParameters.size() == 6 ) );
}

void te::gm::AffineGT::directMap( const GTParameters& params, const Coord2D& pt1, 
  Coord2D& pt2 ) const
{
  assert( isValid( params ) );
  
  pt2.x = ( params.m_directParameters[0] * pt1.x  ) + 
          ( params.m_directParameters[1] * pt1.y ) + 
            params.m_directParameters[2];
  pt2.y = ( params.m_directParameters[3] * pt1.x ) + 
          ( params.m_directParameters[4] * pt1.y ) + 
            params.m_directParameters[5];  
}

void te::gm::AffineGT::inverseMap( const GTParameters& params, const Coord2D& pt2, 
  Coord2D& pt1 ) const
{
  assert( isValid( params ) );

  pt1.x = ( params.m_inverseParameters[0] * pt2.x  ) +
          ( params.m_inverseParameters[1] * pt2.y  ) + 
            params.m_inverseParameters[2];
  pt1.y = ( params.m_inverseParameters[3] * pt2.x ) + 
          ( params.m_inverseParameters[4] * pt2.y ) + 
            params.m_inverseParameters[5]; 
}
          
unsigned int te::gm::AffineGT::getMinRequiredTiePoints() const
{
  return 3;
}
        
bool te::gm::AffineGT::computeParameters( GTParameters& params ) const
{
    /*
            u = a1.x + a2.y + a3
            v = b1.x + b2.y + b3

            A . X + L = 0
            N = At . A
            U = At . L
            X = - (N ^ -1) . U

            A = |x1 y1 1  0  0  0|     Xt = |a1 a2 a3 b1 b2 b3|
                |0  0  0  x1 y1 1|
                |x2 y2 1  0  0  0|
                |0  0  0  x2 y2 1|     Lt = |u1 v1 u2 v2 ... un vn|
                |.  .  .  .  .  .|
                |xn yn 1  0  0  0|
                |0  0  0  xn yn 1|

    */

  const unsigned int tiepointsSize = params.m_tiePoints.size();
  assert( tiepointsSize > 2 );

  /* L calcule */
  
  boost::numeric::ublas::matrix< double > L( 2*tiepointsSize, 1 );

  for ( unsigned int L_block_offset = 0 ; ( L_block_offset < tiepointsSize ); 
    ++L_block_offset ) 
  {
    const Coord2D& u_v = params.m_tiePoints[ L_block_offset ].second;
    
    L( L_block_offset*2     , 0) = u_v.x;
    L( L_block_offset*2 + 1 , 0) = u_v.y;
  }
  
  /* A calcule */

  boost::numeric::ublas::matrix< double > A( 2*tiepointsSize, 6 );
  
  for ( unsigned int A_block_offset = 0 ; (A_block_offset < tiepointsSize) ; 
        ++A_block_offset) 
  {
        
    const Coord2D& x_y = params.m_tiePoints[ A_block_offset ].first;    
    
    A( A_block_offset*2  , 0 ) = x_y.x ;
    A( A_block_offset*2  , 1 ) = x_y.y ;
    A( A_block_offset*2  , 2 ) = 1 ;
    A( A_block_offset*2  , 3 ) = 0 ;
    A( A_block_offset*2  , 4 ) = 0 ;
    A( A_block_offset*2  , 5 ) = 0 ;
    A( A_block_offset*2+1, 0 ) = 0 ;
    A( A_block_offset*2+1, 1 ) = 0 ;
    A( A_block_offset*2+1, 2 ) = 0 ;
    A( A_block_offset*2+1, 3 ) = x_y.x ;
    A( A_block_offset*2+1, 4 ) = x_y.y ;
    A( A_block_offset*2+1, 5 ) = 1 ; 
  }

  /* At calcule */
  boost::numeric::ublas::matrix< double > At( boost::numeric::ublas::trans( A ) ) ;

  /* N calcule */
  boost::numeric::ublas::matrix< double > N( boost::numeric::ublas::prod( At, A ) );

  /* U calcule */
  boost::numeric::ublas::matrix< double > U( boost::numeric::ublas::prod( At, L ) );

  /* N_inv calcule */
  boost::numeric::ublas::matrix< double > N_inv;
  
  if ( te::common::getInverseMatrix( N, N_inv ) ) 
  {
    /* direct parameters calcule */

    boost::numeric::ublas::matrix< double > X( 
      boost::numeric::ublas::prod( N_inv, U ) );
    
    params.m_directParameters.resize( 6 );
    params.m_directParameters[0] = X(0,0);
    params.m_directParameters[1] = X(1,0);
    params.m_directParameters[2] = X(2,0);
    params.m_directParameters[3] = X(3,0);
    params.m_directParameters[4] = X(4,0);
    params.m_directParameters[5] = X(5,0);
    
    /* inverse parameters calcule */
    
    boost::numeric::ublas::matrix< double > XExpanded( 3, 3 );
    XExpanded( 0, 0 ) = X(0,0);
    XExpanded( 0, 1 ) = X(1,0);
    XExpanded( 0, 2 ) = X(2,0);
    XExpanded( 1, 0 ) = X(3,0);
    XExpanded( 1, 1 ) = X(4,0);
    XExpanded( 1, 2 ) = X(5,0);
    XExpanded( 2, 0 ) = 0;
    XExpanded( 2, 1 ) = 0;
    XExpanded( 2, 2 ) = 1;
    
    boost::numeric::ublas::matrix< double > XExpandedInv;

    if( te::common::getInverseMatrix( XExpanded, XExpandedInv ) ) 
    {
      params.m_inverseParameters.resize( 6 );
      params.m_inverseParameters[0] = XExpandedInv(0,0);
      params.m_inverseParameters[1] = XExpandedInv(0,1);
      params.m_inverseParameters[2] = XExpandedInv(0,2);
      params.m_inverseParameters[3] = XExpandedInv(1,0);
      params.m_inverseParameters[4] = XExpandedInv(1,1);
      params.m_inverseParameters[5] = XExpandedInv(1,2);
      
      return true;
    } 
    else 
    {
      return false;
    }
  } else {
    return false;   
  }
}


bool te::gm::AffineGT::decompose( const std::vector< double >& transfParams,
  double& translationX, double& translationY,
  double& scalingFactorX, double& scalingFactorY, double& skew,
  double& squeeze, double& scaling, double& rotation )
{
  assert( transfParams.size() == 6 );
  
  double APar = transfParams[ 0 ];
  double BPar = transfParams[ 1 ];
  double CPar = transfParams[ 3 ];
  double DPar = transfParams[ 4 ];
  
  double determinant = ( APar * DPar ) - ( BPar * CPar );
  
  if( determinant == 0.0 )
  {
    return false;
  }
  else if( determinant < 0.0 )
  {
    APar = transfParams[ 1 ];
    BPar = transfParams[ 0 ];
    CPar = transfParams[ 4 ];
    DPar = transfParams[ 3 ];
    
    determinant = ( APar * DPar ) - ( BPar * CPar );
  }  
  
  const double FVar = 1.0 / ( ( APar * APar ) +
    ( CPar * CPar ) );
    
  skew = ( ( APar * BPar ) + ( CPar * DPar ) ) * FVar;
  
  squeeze = 1.0 / sqrt( FVar * determinant );
    
  scaling = sqrt( determinant );
  
  scalingFactorX = scaling * squeeze;
  
  scalingFactorY = scaling / squeeze;
  
  translationX = transfParams[ 2 ];
  
  translationY = transfParams[ 5 ];
  
  rotation = atan( CPar / APar );
  
  return true;
}

te::gm::AffineGTFactory::AffineGTFactory()
  : GTFactory( TETRANSFORMATIONNAME )
{
}

te::gm::AffineGTFactory::~AffineGTFactory()
{
}

te::gm::GeometricTransformation* te::gm::AffineGTFactory::build()
{
  return new AffineGT();
}

namespace
{
  te::gm::AffineGTFactory AffineGTFactoryInstance;
}

