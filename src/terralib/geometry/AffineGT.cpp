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
  \file terralib/geometry/AffineGT.cpp

  \brief 2D Geometric transformation.
*/

// TerraLib
#include "../common/MatrixUtils.h"
#include "AffineGT.h"

// STL
#include <cmath>


te::gm::AffineGT::AffineGT()
{  
}

te::gm::AffineGT::~AffineGT()
{  
}

const std::string& te::gm::AffineGT::getName() const
{
  static std::string name( "Affine" );
  return name;
}

bool te::gm::AffineGT::isValid( const GTParameters& params ) const
{
  return ( ( params.m_directParameters.size() == 6 ) &&
    ( params.m_inverseParameters.size() == 6 ) );
}

void te::gm::AffineGT::directMap( const GTParameters& params, const double& pt1X, 
                                  const double& pt1Y, double& pt2X, double& pt2Y ) const
{
  assert( isValid( params ) );
  
  pt2X = ( params.m_directParameters[0] * pt1X  ) + 
          ( params.m_directParameters[1] * pt1Y ) + 
            params.m_directParameters[2];
  pt2Y = ( params.m_directParameters[3] * pt1X ) + 
          ( params.m_directParameters[4] * pt1Y ) + 
            params.m_directParameters[5];  
}

void te::gm::AffineGT::inverseMap( const GTParameters& params, const double& pt2X, 
                                   const double& pt2Y, double& pt1X, double& pt1Y ) const
{
  assert( isValid( params ) );

  pt1X = ( params.m_inverseParameters[0] * pt2X  ) +
          ( params.m_inverseParameters[1] * pt2Y  ) + 
            params.m_inverseParameters[2];
  pt1Y = ( params.m_inverseParameters[3] * pt2X ) + 
          ( params.m_inverseParameters[4] * pt2Y ) + 
            params.m_inverseParameters[5]; 
}
          
unsigned int te::gm::AffineGT::getMinRequiredTiePoints() const
{
  return 3;
}

te::gm::GeometricTransformation* te::gm::AffineGT::clone() const
{
  te::gm::AffineGT* newTransPtr = new AffineGT;
  newTransPtr->m_internalParameters = m_internalParameters;
  return newTransPtr;
}
        
bool te::gm::AffineGT::computeParameters( GTParameters& params ) const
{
  m_computeParameters_tiepointsSize = static_cast<unsigned int>(params.m_tiePoints.size());

  if( m_computeParameters_tiepointsSize < getMinRequiredTiePoints() )
    return false;

  m_computeParameters_L.resize( 2*m_computeParameters_tiepointsSize, 1 );
  m_computeParameters_A.resize( 2*m_computeParameters_tiepointsSize, 6 );
  m_computeParameters_index1 = 0;
  m_computeParameters_index2 = 0;
  
  for ( m_computeParameters_tpIdx = 0 ; m_computeParameters_tpIdx < m_computeParameters_tiepointsSize ; ++m_computeParameters_tpIdx)
  {
    const Coord2D& x_y = params.m_tiePoints[ m_computeParameters_tpIdx ].first;
    const Coord2D& u_v = params.m_tiePoints[ m_computeParameters_tpIdx ].second;
    
    m_computeParameters_index1 = m_computeParameters_tpIdx*2;
    m_computeParameters_index2 = m_computeParameters_index1 + 1;

    m_computeParameters_A( m_computeParameters_index1, 0 ) = x_y.x ;
    m_computeParameters_A( m_computeParameters_index1, 1 ) = x_y.y ;
    m_computeParameters_A( m_computeParameters_index1, 2 ) = 1 ;
    m_computeParameters_A( m_computeParameters_index1, 3 ) = 0 ;
    m_computeParameters_A( m_computeParameters_index1, 4 ) = 0 ;
    m_computeParameters_A( m_computeParameters_index1, 5 ) = 0 ;
    
    m_computeParameters_A( m_computeParameters_index2, 0 ) = 0 ;
    m_computeParameters_A( m_computeParameters_index2, 1 ) = 0 ;
    m_computeParameters_A( m_computeParameters_index2, 2 ) = 0 ;
    m_computeParameters_A( m_computeParameters_index2, 3 ) = x_y.x ;
    m_computeParameters_A( m_computeParameters_index2, 4 ) = x_y.y ;
    m_computeParameters_A( m_computeParameters_index2, 5 ) = 1 ; 
    
    m_computeParameters_L( m_computeParameters_index1, 0) = u_v.x;
    m_computeParameters_L( m_computeParameters_index2, 0) = u_v.y;
  }

  /* At calcule */
  m_computeParameters_At = boost::numeric::ublas::trans( m_computeParameters_A );

  /* N calcule */
  m_computeParameters_N = boost::numeric::ublas::prod( m_computeParameters_At, m_computeParameters_A );

  /* U calcule */
  m_computeParameters_U = boost::numeric::ublas::prod( m_computeParameters_At, m_computeParameters_L );

  /* N_inv calcule */
  
  if ( te::common::GetInverseMatrix( m_computeParameters_N, m_computeParameters_N_inv ) ) 
  {
    /* direct parameters calcule */

    m_computeParameters_X = boost::numeric::ublas::prod( m_computeParameters_N_inv, 
      m_computeParameters_U );
    
    params.m_directParameters.resize( 6 );
    params.m_directParameters[0] = m_computeParameters_X(0,0);
    params.m_directParameters[1] = m_computeParameters_X(1,0);
    params.m_directParameters[2] = m_computeParameters_X(2,0);
    params.m_directParameters[3] = m_computeParameters_X(3,0);
    params.m_directParameters[4] = m_computeParameters_X(4,0);
    params.m_directParameters[5] = m_computeParameters_X(5,0);
    
    /* inverse parameters calcule */
    
    m_computeParameters_XExpanded.resize( 3, 3 );
    m_computeParameters_XExpanded( 0, 0 ) = m_computeParameters_X(0,0);
    m_computeParameters_XExpanded( 0, 1 ) = m_computeParameters_X(1,0);
    m_computeParameters_XExpanded( 0, 2 ) = m_computeParameters_X(2,0);
    m_computeParameters_XExpanded( 1, 0 ) = m_computeParameters_X(3,0);
    m_computeParameters_XExpanded( 1, 1 ) = m_computeParameters_X(4,0);
    m_computeParameters_XExpanded( 1, 2 ) = m_computeParameters_X(5,0);
    m_computeParameters_XExpanded( 2, 0 ) = 0;
    m_computeParameters_XExpanded( 2, 1 ) = 0;
    m_computeParameters_XExpanded( 2, 2 ) = 1;
    
    if( te::common::GetInverseMatrix( m_computeParameters_XExpanded, m_computeParameters_XExpandedInv ) ) 
    {
      params.m_inverseParameters.resize( 6 );
      params.m_inverseParameters[0] = m_computeParameters_XExpandedInv(0,0);
      params.m_inverseParameters[1] = m_computeParameters_XExpandedInv(0,1);
      params.m_inverseParameters[2] = m_computeParameters_XExpandedInv(0,2);
      params.m_inverseParameters[3] = m_computeParameters_XExpandedInv(1,0);
      params.m_inverseParameters[4] = m_computeParameters_XExpandedInv(1,1);
      params.m_inverseParameters[5] = m_computeParameters_XExpandedInv(1,2);
      
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
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
    
    const double FVar = 1.0 / ( ( APar * APar ) +
      ( CPar * CPar ) );
      
    skew = ( ( APar * BPar ) + ( CPar * DPar ) ) * FVar;
    
    squeeze = 1.0 / sqrt( FVar * determinant );
      
    scaling = sqrt( determinant );
    
    scalingFactorX = scaling / squeeze;
    
    scalingFactorY = scaling * squeeze;    
    
    rotation = atan( APar / CPar );
  }
  else
  {
    const double FVar = 1.0 / ( ( APar * APar ) +
      ( CPar * CPar ) );
      
    skew = ( ( APar * BPar ) + ( CPar * DPar ) ) * FVar;
    
    squeeze = 1.0 / sqrt( FVar * determinant );
      
    scaling = sqrt( determinant );
    
    scalingFactorX = scaling * squeeze;
    
    scalingFactorY = scaling / squeeze;    
    
    rotation = atan( CPar / APar );
  }
  
  translationX = transfParams[ 2 ];
  
  translationY = transfParams[ 5 ];
  
  return true;
}


