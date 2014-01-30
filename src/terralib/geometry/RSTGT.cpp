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
  \file terralib/geometry/RSTGT.cpp

  \brief 2D Rotation/scale/translation(rigid body) Geometric transformation.
*/

// TerraLib
#include "../common/MatrixUtils.h"
#include "RSTGT.h"

// STL
#include <cmath>

// Boost
#include <boost/numeric/ublas/matrix.hpp>

te::gm::RSTGT::RSTGT()
{  
}

te::gm::RSTGT::~RSTGT()
{  
}

const std::string& te::gm::RSTGT::getName() const
{
  static std::string name( "RST" );
  return name;
}

bool te::gm::RSTGT::isValid( const GTParameters& params ) const
{
  return ( ( params.m_directParameters.size() == 4 ) &&
    ( params.m_inverseParameters.size() == 4 ) );
}

void te::gm::RSTGT::directMap( const GTParameters& params, const double& pt1X, 
                               const double& pt1Y, double& pt2X, double& pt2Y ) const
{
  assert( isValid( params ) );
  
  pt2X = ( params.m_directParameters[0] * pt1X  ) - 
          ( params.m_directParameters[1] * pt1Y ) + 
            params.m_directParameters[2];
  pt2Y = ( params.m_directParameters[1] * pt1X ) + 
          ( params.m_directParameters[0] * pt1Y ) + 
            params.m_directParameters[3];  
}

void te::gm::RSTGT::inverseMap( const GTParameters& params, const double& pt2X, 
                                const double& pt2Y, double& pt1X, double& pt1Y ) const
{
  assert( isValid( params ) );

  pt1X = ( params.m_inverseParameters[0] * pt2X  ) -
          ( params.m_inverseParameters[1] * pt2Y  ) + 
            params.m_inverseParameters[2];
  pt1Y = ( params.m_inverseParameters[1] * pt2X ) + 
          ( params.m_inverseParameters[0] * pt2Y ) + 
            params.m_inverseParameters[3]; 
}
          
unsigned int te::gm::RSTGT::getMinRequiredTiePoints() const
{
  return 2;
}

te::gm::GeometricTransformation* te::gm::RSTGT::clone() const
{
  te::gm::RSTGT* newTransPtr = new RSTGT;
  newTransPtr->m_internalParameters = m_internalParameters;
  return newTransPtr;
};
        
bool te::gm::RSTGT::computeParameters( GTParameters& params ) const
{
  const unsigned int tiepointsSize = params.m_tiePoints.size();
  if( tiepointsSize < 2 ) return false;
  
  boost::numeric::ublas::matrix< double > A( 2 * tiepointsSize, 4 );
  boost::numeric::ublas::matrix< double > L( 2 * tiepointsSize, 1 );
  unsigned int index1 = 0;
  unsigned int index2 = 0;  
  
  for ( unsigned int tpIdx = 0 ; tpIdx < tiepointsSize ; ++tpIdx ) 
  {
    index1 = tpIdx*2;
    index2 = index1 + 1;
        
    const Coord2D& x_y = params.m_tiePoints[ tpIdx ].first;
    const Coord2D& u_v = params.m_tiePoints[ tpIdx ].second;
    
    A( index1, 0 ) = x_y.x;
    A( index1, 1 ) = -1.0 * x_y.y;
    A( index1, 2 ) = 1.0;
    A( index1, 3 ) = 0.0;
    
    A( index2, 0 ) = x_y.y;
    A( index2, 1 ) = x_y.x;
    A( index2, 2 ) = 0.0;
    A( index2, 3 ) = 1.0;    
    
    L( index1, 0 ) = u_v.x;
    L( index2, 0 ) = u_v.y;
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
    
  if ( te::common::GetInverseMatrix( N, N_inv ) ) 
  {
    /* direct parameters calcule */

    boost::numeric::ublas::matrix< double > X( 
      boost::numeric::ublas::prod( N_inv, U ) );
      
//    std::cout << std::endl << "X:" << std::endl << X << std::endl;      
    
    params.m_directParameters.resize( 4 );
    params.m_directParameters[0] = X(0,0);
    params.m_directParameters[1] = X(1,0);
    params.m_directParameters[2] = X(2,0);
    params.m_directParameters[3] = X(3,0);
    
    /* inverse parameters calcule */
    
    boost::numeric::ublas::matrix< double > XExpanded( 3, 3 );
    XExpanded( 0, 0 ) = X(0,0);
    XExpanded( 0, 1 ) = -1.0 * X(1,0);
    XExpanded( 0, 2 ) = X(2,0);
    XExpanded( 1, 0 ) = X(1,0);
    XExpanded( 1, 1 ) = X(0,0);
    XExpanded( 1, 2 ) = X(3,0);
    XExpanded( 2, 0 ) = 0;
    XExpanded( 2, 1 ) = 0;
    XExpanded( 2, 2 ) = 1;
    
//    std::cout << std::endl << "XExpanded:" << std::endl << XExpanded << std::endl;    
    
    boost::numeric::ublas::matrix< double > XExpandedInv;

    if( te::common::GetInverseMatrix( XExpanded, XExpandedInv ) ) 
    {
//      std::cout << std::endl << "XExpandedInv:" << std::endl << XExpandedInv << std::endl;

      params.m_inverseParameters.resize( 4 );
      params.m_inverseParameters[0] = XExpandedInv(0,0);
      params.m_inverseParameters[1] = XExpandedInv(1,0);
      params.m_inverseParameters[2] = XExpandedInv(0,2);
      params.m_inverseParameters[3] = XExpandedInv(1,2);
      
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


