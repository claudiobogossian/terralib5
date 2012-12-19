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
  \file terralib/geometry/ProjectiveGT.cpp

  \brief 2D Geometric transformation.
*/

// TerraLib
#include "../common/MatrixUtils.h"
#include "ProjectiveGT.h"

// STL
#include <cmath>

// Boost
#include <boost/numeric/ublas/matrix.hpp>

#define TETRANSFORMATIONNAME "Projective"

te::gm::ProjectiveGT::ProjectiveGT()
{  
}

te::gm::ProjectiveGT::~ProjectiveGT()
{  
}

const std::string& te::gm::ProjectiveGT::getName() const
{
  static std::string name( TETRANSFORMATIONNAME );
  return name;
}

bool te::gm::ProjectiveGT::isValid( const GTParameters& params ) const
{
  return ( ( params.m_directParameters.size() == 8 ) &&
    ( params.m_inverseParameters.size() == 8 ) );
}

void te::gm::ProjectiveGT::directMap( const GTParameters& params, const Coord2D& pt1, 
  Coord2D& pt2 ) const
{
  assert( isValid( params ) );
  
  pt2.x = (
            ( params.m_directParameters[0] * pt1.x  ) + 
            ( params.m_directParameters[1] * pt1.y ) + 
              params.m_directParameters[2] 
          )
          /
          (
            ( params.m_directParameters[6] * pt1.x  ) + 
            ( params.m_directParameters[7] * pt1.y ) + 
            1
          );
          
  pt2.y =  (
            ( params.m_directParameters[3] * pt1.x  ) + 
            ( params.m_directParameters[4] * pt1.y ) + 
              params.m_directParameters[5] 
          )
          /
          (
            ( params.m_directParameters[6] * pt1.x  ) + 
            ( params.m_directParameters[7] * pt1.y ) +
            1
          );
}

void te::gm::ProjectiveGT::inverseMap( const GTParameters& params, const Coord2D& pt2, 
  Coord2D& pt1 ) const
{
  assert( isValid( params ) );
  
  pt1.x = (
            ( params.m_inverseParameters[0] * pt2.x  ) + 
            ( params.m_inverseParameters[1] * pt2.y ) + 
              params.m_inverseParameters[2] 
          )
          /
          (
            ( params.m_inverseParameters[6] * pt2.x  ) + 
            ( params.m_inverseParameters[7] * pt2.y ) +
            1
          );
          
  pt1.y =  (
            ( params.m_inverseParameters[3] * pt2.x  ) + 
            ( params.m_inverseParameters[4] * pt2.y ) + 
              params.m_inverseParameters[5] 
          )
          /
          (
            ( params.m_inverseParameters[6] * pt2.x  ) + 
            ( params.m_inverseParameters[7] * pt2.y ) +
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
};
        
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

  const unsigned int tiepointsSize = params.m_tiePoints.size();
  if( tiepointsSize < 4 ) return false;

  /* L and A calcule */

  boost::numeric::ublas::matrix< double > L( 2*tiepointsSize, 1 );
  boost::numeric::ublas::matrix< double > A( 2*tiepointsSize, 8 );  
  unsigned int index1 = 0;
  unsigned int index2 = 0;
  
  for ( unsigned int blockOffset = 0 ; (blockOffset < tiepointsSize) ; 
        ++blockOffset) 
  {
    index1 = blockOffset*2;
    index2 = index1 + 1;
    
    const Coord2D& x_y = params.m_tiePoints[ blockOffset ].first;
    const Coord2D& u_v = params.m_tiePoints[ blockOffset ].second;
    
    L( index1, 0) = u_v.x;
    L( index2, 0) = u_v.y;
    
    A( index1, 0 ) = x_y.x ;
    A( index1, 1 ) = x_y.y ;
    A( index1, 2 ) = 1 ;
    A( index1, 3 ) = 0 ;
    A( index1, 4 ) = 0 ;
    A( index1, 5 ) = 0 ;
    A( index1, 6 ) = - x_y.x * u_v.x;
    A( index1, 7 ) = - x_y.y * u_v.x;
    
    A( index2, 0 ) = 0;
    A( index2, 1 ) = 0;
    A( index2, 2 ) = 0;
    A( index2, 3 ) = x_y.x ;
    A( index2, 4 ) = x_y.y ;
    A( index2, 5 ) = 1 ;
    A( index2, 6 ) = - x_y.x * u_v.y;
    A( index2, 7 ) = - x_y.y * u_v.y;
  }
  
//  std::cout << std::endl << "L:" << std::endl << L << std::endl;
//  std::cout << std::endl << "A:" << std::endl << A << std::endl;

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
      
//    std::cout << std::endl << "X:" << std::endl << X << std::endl;
    
    params.m_directParameters.resize( 8 );
    params.m_directParameters[0] = X(0,0);
    params.m_directParameters[1] = X(1,0);
    params.m_directParameters[2] = X(2,0);
    params.m_directParameters[3] = X(3,0);
    params.m_directParameters[4] = X(4,0);
    params.m_directParameters[5] = X(5,0);
    params.m_directParameters[6] = X(6,0);
    params.m_directParameters[7] = X(7,0);
    
    /* inverse parameters calcule */
    
    boost::numeric::ublas::matrix< double > XExpanded( 3, 3 );
    XExpanded( 0, 0 ) = X(0,0);
    XExpanded( 0, 1 ) = X(1,0);
    XExpanded( 0, 2 ) = X(2,0);
    XExpanded( 1, 0 ) = X(3,0);
    XExpanded( 1, 1 ) = X(4,0);
    XExpanded( 1, 2 ) = X(5,0);
    XExpanded( 2, 0 ) = X(6,0);
    XExpanded( 2, 1 ) = X(7,0);
    XExpanded( 2, 2 ) = 1;
    
//    std::cout << std::endl << "XExpanded:" << std::endl << XExpanded << std::endl;
    
    boost::numeric::ublas::matrix< double > XExpandedInv;

    if( te::common::getInverseMatrix( XExpanded, XExpandedInv ) ) 
    {
//      std::cout << std::endl << "XExpandedInv:" << std::endl << XExpandedInv << std::endl;
      
      params.m_inverseParameters.resize( 8 );
      params.m_inverseParameters[0] = XExpandedInv(0,0);
      params.m_inverseParameters[1] = XExpandedInv(0,1);
      params.m_inverseParameters[2] = XExpandedInv(0,2);
      params.m_inverseParameters[3] = XExpandedInv(1,0);
      params.m_inverseParameters[4] = XExpandedInv(1,1);
      params.m_inverseParameters[5] = XExpandedInv(1,2);
      params.m_inverseParameters[6] = XExpandedInv(2,0);
      params.m_inverseParameters[7] = XExpandedInv(2,1);
      
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

te::gm::ProjectiveGTFactory::ProjectiveGTFactory()
  : GTFactory( TETRANSFORMATIONNAME )
{
}

te::gm::ProjectiveGTFactory::~ProjectiveGTFactory()
{
}

te::gm::GeometricTransformation* te::gm::ProjectiveGTFactory::build()
{
  return new ProjectiveGT();
}

namespace
{
  te::gm::ProjectiveGTFactory ProjectiveGTFactoryInstance;
}

