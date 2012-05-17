/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

// Unit-Test TerraLib
#include "TsGeometricTransformations.h"
#include "../Config.h"

// TerraLib
#include <terralib/geometry/AffineGT.h>
#include <terralib/geometry/GTParameters.h>
#include <terralib/geometry/GTFactory.h>
//#include <terralib/geometry.h>

// STL
//#include <cstdio>
//#include <cstdlib>
//#include <cstring>
#include <memory>

#include <cmath>

CPPUNIT_TEST_SUITE_REGISTRATION( TsGeometricTransformations );

void TsGeometricTransformations::setUp()
{
}

void TsGeometricTransformations::tearDown()
{
}

void TsGeometricTransformations::testDirectMapping( 
  te::gm::GeometricTransformation const * transfPtr,
  const te::gm::Coord2D& pt1, const te::gm::Coord2D& pt2, 
  const double& maxError )
{
  te::gm::Coord2D mappedCoord;
  
  transfPtr->directMap( pt1, mappedCoord );
  
  const double diffx = mappedCoord.x - pt2.x;
  const double diffy = mappedCoord.y - pt2.y;
  const double error = sqrt( ( diffx * diffx ) + ( diffy * diffy ) );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, error, maxError );
}


void TsGeometricTransformations::testInverseMapping( 
  te::gm::GeometricTransformation const * transfPtr,
  const te::gm::Coord2D& pt1, const te::gm::Coord2D& pt2, 
  const double& maxError )
{
  te::gm::Coord2D mappedCoord;
  
  transfPtr->inverseMap( pt2, mappedCoord );
  
  const double diffx = mappedCoord.x - pt1.x;
  const double diffy = mappedCoord.y - pt1.y;
  const double error = sqrt( ( diffx * diffx ) + ( diffy * diffy ) );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, error, maxError );
}

void TsGeometricTransformations::tcAffine()
{
  std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
    te::gm::GTFactory::make( "Affine" ) );
  CPPUNIT_ASSERT( transfPtr.get() != 0 );
    
  te::gm::GTParameters transfParams;
  
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0, 0 ), te::gm::Coord2D( 0, 0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 10, 0 ), te::gm::Coord2D( 10, 0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0, 10 ), te::gm::Coord2D( 0, 10 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 10, 10 ), te::gm::Coord2D( 10, 10 ) ) );

  CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
  
  CPPUNIT_ASSERT( transfPtr->getName() == "affine" );
  
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0000001 );
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 10, 0 ), 0.0000001 );        
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 0, 10 ), 
    te::gm::Coord2D( 0, 10 ), 0.0000001 );    
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 10, 10 ), 
    te::gm::Coord2D( 10, 10 ), 0.0000001 );    
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 5, 5 ), 
    te::gm::Coord2D( 5, 5 ), 0.0000001 );    
    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0000001 );
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 10, 0 ), 0.0000001 );        
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 0, 10 ), 
    te::gm::Coord2D( 0, 10 ), 0.0000001 );    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 10, 10 ), 
    te::gm::Coord2D( 10, 10 ), 0.0000001 );    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 5, 5 ), 
    te::gm::Coord2D( 5, 5 ), 0.0000001 );    
    
  double translationX = 0;
  double translationY = 0;
  double scalingFactorX = 0;
  double scalingFactorY = 0;
  double skew = 0;
  double squeeze = 0;
  double scaling = 0;
  double rotation = 0;
  
  CPPUNIT_ASSERT( te::gm::AffineGT::decompose( 
    transfPtr->getParameters().m_directParameters,
    translationX, translationY, scalingFactorX, scalingFactorY, skew, 
    squeeze, scaling, rotation ) );
    
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, translationX, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, translationY, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, scalingFactorX, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, scalingFactorY, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, skew, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, squeeze, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, scaling, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, rotation, 0.00000000001 );
}

void TsGeometricTransformations::tcSecondDegreePolynomialGT()
{
  std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
    te::gm::GTFactory::make( "SecondDegreePolynomial" ) );
  CPPUNIT_ASSERT( transfPtr.get() != 0 );
    
  te::gm::GTParameters transfParams;
  
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0, 0 ), te::gm::Coord2D( 0, 0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 10, 0 ), te::gm::Coord2D( 10, 0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0, 10 ), te::gm::Coord2D( 0, 10 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 10, 10 ), te::gm::Coord2D( 10, 10 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 5, 5 ), te::gm::Coord2D( 5, 5 ) ) );    
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 5, 10 ), te::gm::Coord2D( 5, 10 ) ) );
    

  CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
  
  CPPUNIT_ASSERT( transfPtr->getName() == "SecondDegreePolynomial" );
  
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0000001 );
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 10, 0 ), 0.0000001 );        
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 0, 10 ), 
    te::gm::Coord2D( 0, 10 ), 0.0000001 );    
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 10, 10 ), 
    te::gm::Coord2D( 10, 10 ), 0.0000001 );    
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 5, 5 ), 
    te::gm::Coord2D( 5, 5 ), 0.0000001 );    
  testDirectMapping( transfPtr.get(), te::gm::Coord2D( 5, 10 ), 
    te::gm::Coord2D( 5, 10 ), 0.0000001 );      
    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0000001 );
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 10, 0 ), 0.0000001 );        
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 0, 10 ), 
    te::gm::Coord2D( 0, 10 ), 0.0000001 );    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 10, 10 ), 
    te::gm::Coord2D( 10, 10 ), 0.0000001 );    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 5, 5 ), 
    te::gm::Coord2D( 5, 5 ), 0.0000001 );    
  testInverseMapping( transfPtr.get(), te::gm::Coord2D( 5, 10 ), 
    te::gm::Coord2D( 5, 10 ), 0.0000001 );    
   
}

