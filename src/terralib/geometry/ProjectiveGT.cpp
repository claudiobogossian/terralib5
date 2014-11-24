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
  \file terralib/geometry/ProjectiveGT.cpp

  \brief 2D Projective Geometric transformation.
*/

// TerraLib
#include "../common/MatrixUtils.h"
#include "ProjectiveGT.h"

// STL
#include <cmath>

te::gm::ProjectiveGT::ProjectiveGT()
{  
}

te::gm::ProjectiveGT::~ProjectiveGT()
{  
}

const std::string& te::gm::ProjectiveGT::getName() const
{
  static std::string name( "Projective" );
  return name;
}

bool te::gm::ProjectiveGT::isValid( const GTParameters& params ) const
{
  return ( ( params.m_directParameters.size() == 8 ) &&
    ( params.m_inverseParameters.size() == 8 ) );
}

void te::gm::ProjectiveGT::directMap( const GTParameters& params, const double& pt1X, 
                                      const double& pt1Y, double& pt2X, double& pt2Y ) const
{
  assert( isValid( params ) );
  
  pt2X = (
            ( params.m_directParameters[0] * pt1X  ) + 
            ( params.m_directParameters[1] * pt1Y ) + 
              params.m_directParameters[2] 
          )
          /
          (
            ( params.m_directParameters[6] * pt1X  ) + 
            ( params.m_directParameters[7] * pt1Y ) + 
            1
          );
          
  pt2Y =  (
            ( params.m_directParameters[3] * pt1X  ) + 
            ( params.m_directParameters[4] * pt1Y ) + 
              params.m_directParameters[5] 
          )
          /
          (
            ( params.m_directParameters[6] * pt1X  ) + 
            ( params.m_directParameters[7] * pt1Y ) +
            1
          );
}

void te::gm::ProjectiveGT::inverseMap( const GTParameters& params, const double& pt2X, 
                                       const double& pt2Y, double& pt1X, double& pt1Y ) const
{
  assert( isValid( params ) );
  
  pt1X = (
            ( params.m_inverseParameters[0] * pt2X  ) + 
            ( params.m_inverseParameters[1] * pt2Y ) + 
              params.m_inverseParameters[2] 
          )
          /
          (
            ( params.m_inverseParameters[6] * pt2X  ) + 
            ( params.m_inverseParameters[7] * pt2Y ) +
            1
          );
          
  pt1Y =  (
            ( params.m_inverseParameters[3] * pt2X  ) + 
            ( params.m_inverseParameters[4] * pt2Y ) + 
              params.m_inverseParameters[5] 
          )
          /
          (
            ( params.m_inverseParameters[6] * pt2X  ) + 
            ( params.m_inverseParameters[7] * pt2Y ) +
            1
          );  
}
          
unsigned int te::gm::ProjectiveGT::getMinRequiredTiePoints() const
{
  return 4;
}

te::gm::GeometricTransformation* te::gm::ProjectiveGT::clone() const
{
  te::gm::ProjectiveGT* newTransPtr = new ProjectiveGT;
  newTransPtr->m_internalParameters = m_internalParameters;
  return newTransPtr;
}
        
bool te::gm::ProjectiveGT::computeParameters( GTParameters& params ) const
{
    /* 
      | u | = | a b c | * | x |
      | v |   | d e f |   | y |
      | 1 |   | g h 1 |   | 1 |
        
        u = a.x + b.y + c
            -------------------
            g.x + h.y + 1
            
        v = d.x + e.y + f
            -------------------
            g.x + h.y + 1
               
        Method adapted from Albertz and Kreiling (1989).
        Reference: Albertz, J.; Kreiling, W. Photogrametriches taschenbuch. 
        Karlsruhe: Wichmann, 1989.
    */

  m_computeParameters_tiepointsSize = params.m_tiePoints.size();
  if( m_computeParameters_tiepointsSize < getMinRequiredTiePoints() ) return false;

  // Direct mapping
  m_computeParameters_L_DM.resize( 2*m_computeParameters_tiepointsSize, 1 );
  m_computeParameters_A_DM.resize( 2*m_computeParameters_tiepointsSize, 8 ); 
  
  //Inverse mapping
  m_computeParameters_L_IM.resize( 2*m_computeParameters_tiepointsSize, 1 );
  m_computeParameters_A_IM.resize( 2*m_computeParameters_tiepointsSize, 8 );  
  
  for ( m_computeParameters_blockOffset = 0 ; (m_computeParameters_blockOffset < m_computeParameters_tiepointsSize) ; 
        ++m_computeParameters_blockOffset) 
  {
    m_computeParameters_index1 = m_computeParameters_blockOffset*2;
    m_computeParameters_index2 = m_computeParameters_index1 + 1;
    
    const Coord2D& x_y = params.m_tiePoints[ m_computeParameters_blockOffset ].first;
    const Coord2D& u_v = params.m_tiePoints[ m_computeParameters_blockOffset ].second;
    
    m_computeParameters_L_DM( m_computeParameters_index1, 0) = u_v.x;
    m_computeParameters_L_DM( m_computeParameters_index2, 0) = u_v.y;
    
    m_computeParameters_A_DM( m_computeParameters_index1, 0 ) = x_y.x ;
    m_computeParameters_A_DM( m_computeParameters_index1, 1 ) = x_y.y ;
    m_computeParameters_A_DM( m_computeParameters_index1, 2 ) = 1 ;
    m_computeParameters_A_DM( m_computeParameters_index1, 3 ) = 0 ;
    m_computeParameters_A_DM( m_computeParameters_index1, 4 ) = 0 ;
    m_computeParameters_A_DM( m_computeParameters_index1, 5 ) = 0 ;
    m_computeParameters_A_DM( m_computeParameters_index1, 6 ) = - x_y.x * u_v.x;
    m_computeParameters_A_DM( m_computeParameters_index1, 7 ) = - x_y.y * u_v.x;
    
    m_computeParameters_A_DM( m_computeParameters_index2, 0 ) = 0;
    m_computeParameters_A_DM( m_computeParameters_index2, 1 ) = 0;
    m_computeParameters_A_DM( m_computeParameters_index2, 2 ) = 0;
    m_computeParameters_A_DM( m_computeParameters_index2, 3 ) = x_y.x ;
    m_computeParameters_A_DM( m_computeParameters_index2, 4 ) = x_y.y ;
    m_computeParameters_A_DM( m_computeParameters_index2, 5 ) = 1 ;
    m_computeParameters_A_DM( m_computeParameters_index2, 6 ) = - x_y.x * u_v.y;
    m_computeParameters_A_DM( m_computeParameters_index2, 7 ) = - x_y.y * u_v.y;
    
    m_computeParameters_L_IM( m_computeParameters_index1, 0) = x_y.x;
    m_computeParameters_L_IM( m_computeParameters_index2, 0) = x_y.y;
    
    m_computeParameters_A_IM( m_computeParameters_index1, 0 ) = u_v.x ;
    m_computeParameters_A_IM( m_computeParameters_index1, 1 ) = u_v.y ;
    m_computeParameters_A_IM( m_computeParameters_index1, 2 ) = 1 ;
    m_computeParameters_A_IM( m_computeParameters_index1, 3 ) = 0 ;
    m_computeParameters_A_IM( m_computeParameters_index1, 4 ) = 0 ;
    m_computeParameters_A_IM( m_computeParameters_index1, 5 ) = 0 ;
    m_computeParameters_A_IM( m_computeParameters_index1, 6 ) = - u_v.x * x_y.x;
    m_computeParameters_A_IM( m_computeParameters_index1, 7 ) = - u_v.y * x_y.x;
    
    m_computeParameters_A_IM( m_computeParameters_index2, 0 ) = 0;
    m_computeParameters_A_IM( m_computeParameters_index2, 1 ) = 0;
    m_computeParameters_A_IM( m_computeParameters_index2, 2 ) = 0;
    m_computeParameters_A_IM( m_computeParameters_index2, 3 ) = u_v.x ;
    m_computeParameters_A_IM( m_computeParameters_index2, 4 ) = u_v.y ;
    m_computeParameters_A_IM( m_computeParameters_index2, 5 ) = 1 ;
    m_computeParameters_A_IM( m_computeParameters_index2, 6 ) = - u_v.x * x_y.y;
    m_computeParameters_A_IM( m_computeParameters_index2, 7 ) = - u_v.y * x_y.y;    
  }
  
  /* At calcule */
  m_computeParameters_A_DM_t = boost::numeric::ublas::trans( m_computeParameters_A_DM );
  m_computeParameters_A_IM_t = boost::numeric::ublas::trans( m_computeParameters_A_IM );

  /* N calcule */
  m_computeParameters_N_DM = boost::numeric::ublas::prod( m_computeParameters_A_DM_t, m_computeParameters_A_DM );
  m_computeParameters_N_IM = boost::numeric::ublas::prod( m_computeParameters_A_IM_t, m_computeParameters_A_IM );

  /* U calcule */
  m_computeParameters_U_DM = boost::numeric::ublas::prod( m_computeParameters_A_DM_t, m_computeParameters_L_DM );
  m_computeParameters_U_IM = boost::numeric::ublas::prod( m_computeParameters_A_IM_t, m_computeParameters_L_IM );

  /* N_inv calcule */
  
  if ( ! te::common::GetInverseMatrix( m_computeParameters_N_DM, m_computeParameters_N_DM_inv ) ) return false;

  m_computeParameters_X_DM = boost::numeric::ublas::prod( m_computeParameters_N_DM_inv, m_computeParameters_U_DM );
      
  params.m_directParameters.resize( 8 );
  params.m_directParameters[0] = m_computeParameters_X_DM(0,0);
  params.m_directParameters[1] = m_computeParameters_X_DM(1,0);
  params.m_directParameters[2] = m_computeParameters_X_DM(2,0);
  params.m_directParameters[3] = m_computeParameters_X_DM(3,0);
  params.m_directParameters[4] = m_computeParameters_X_DM(4,0);
  params.m_directParameters[5] = m_computeParameters_X_DM(5,0);
  params.m_directParameters[6] = m_computeParameters_X_DM(6,0);
  params.m_directParameters[7] = m_computeParameters_X_DM(7,0);
  
  if ( ! te::common::GetInverseMatrix( m_computeParameters_N_IM, m_computeParameters_N_IM_inv ) ) return false;

  m_computeParameters_X_IM = boost::numeric::ublas::prod( m_computeParameters_N_IM_inv, m_computeParameters_U_IM );
      
  params.m_inverseParameters.resize( 8 );
  params.m_inverseParameters[0] = m_computeParameters_X_IM(0,0);
  params.m_inverseParameters[1] = m_computeParameters_X_IM(1,0);
  params.m_inverseParameters[2] = m_computeParameters_X_IM(2,0);
  params.m_inverseParameters[3] = m_computeParameters_X_IM(3,0);
  params.m_inverseParameters[4] = m_computeParameters_X_IM(4,0);
  params.m_inverseParameters[5] = m_computeParameters_X_IM(5,0);
  params.m_inverseParameters[6] = m_computeParameters_X_IM(6,0);
  params.m_inverseParameters[7] = m_computeParameters_X_IM(7,0);  
  
  return true;
}


