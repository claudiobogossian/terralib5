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

#define TESTDIRECTMAPPING( transfPtr, pt1, pt2, maxError ) \
{ \
  te::gm::Coord2D mappedCoord; \
  transfPtr->directMap( pt1, mappedCoord ); \
  const double diffx = mappedCoord.x - pt2.x; \
  const double diffy = mappedCoord.y - pt2.y; \
  const double error = sqrt( ( diffx * diffx ) + ( diffy * diffy ) ); \
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, error, maxError ); \
}

#define TESTINVERSEMAPPING( transfPtr, pt1, pt2, maxError ) \
{ \
  te::gm::Coord2D mappedCoord; \
  transfPtr->inverseMap( pt2, mappedCoord ); \
  const double diffx = mappedCoord.x - pt1.x; \
  const double diffy = mappedCoord.y - pt1.y; \
  const double error = sqrt( ( diffx * diffx ) + ( diffy * diffy ) ); \
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, error, maxError ); \
}

void TsGeometricTransformations::setUp()
{
}

void TsGeometricTransformations::tearDown()
{
}

void TsGeometricTransformations::tcRSTGT()
{
  std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
    te::gm::GTFactory::make( "RST" ) );
  CPPUNIT_ASSERT( transfPtr.get() != 0 );
    
  te::gm::GTParameters transfParams;
  
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 0.0, -1.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( -1.0, 0.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( 0.0, 1.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 0.0 ), te::gm::Coord2D( 1.0, 0.0 ) ) );

  CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
  
  CPPUNIT_ASSERT( transfPtr->getName() == "RST" );
  
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 0.0 ), 
    te::gm::Coord2D( 0.0, -1.0 ), 0.0000001 );
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 1.0 ), 
    te::gm::Coord2D( -1.0, 0.0 ), 0.0000001 );        
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 1.0 ), 
    te::gm::Coord2D( 0.0, 1.0 ), 0.0000001 );    
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 0.0 ), 
    te::gm::Coord2D( 1.0, 0.0 ), 0.0000001 );    
    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 0.0 ), 
    te::gm::Coord2D( 0.0, -1.0 ), 0.0000001 );
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 1.0 ), 
    te::gm::Coord2D( -1.0, 0.0 ), 0.0000001 );        
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 1.0 ), 
    te::gm::Coord2D( 0.0, 1.0 ), 0.0000001 );    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 0.0 ), 
    te::gm::Coord2D( 1.0, 0.0 ), 0.0000001 );    
}

void TsGeometricTransformations::tcAffine()
{
  std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
    te::gm::GTFactory::make( "Affine" ) );
  CPPUNIT_ASSERT( transfPtr.get() != 0 );
    
  te::gm::GTParameters transfParams;
  
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 1.0, 0.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( -1.0, -2.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( -1.0, 0.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 0.0 ), te::gm::Coord2D( 1.0, 2.0 ) ) );
    

  CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
  
  CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
  
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 0.0 ), 
    te::gm::Coord2D( 1.0, 0.0 ), 0.0000001 );
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 1.0 ), 
    te::gm::Coord2D( -1.0, -2.0 ), 0.0000001 );        
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 1.0 ), 
    te::gm::Coord2D( -1.0, 0.0 ), 0.0000001 );    
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 0.0 ), 
    te::gm::Coord2D( 1.0, 2.0 ), 0.0000001 );    
    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 0.0 ), 
    te::gm::Coord2D( 1.0, 0.0 ), 0.0000001 );
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 1.0 ), 
    te::gm::Coord2D( -1.0, -2.0 ), 0.0000001 );        
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 1.0 ), 
    te::gm::Coord2D( -1.0, 0.0 ), 0.0000001 );    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 0.0 ), 
    te::gm::Coord2D( 1.0, 2.0 ), 0.0000001 );    
    
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
    
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.999999999999, translationX, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, translationY, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, scalingFactorX, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, scalingFactorY, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, skew, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, squeeze, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, scaling, 0.00000000001 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5707963267948966, rotation, 0.00000000001 );
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
    te::gm::Coord2D( 10, 0 ), te::gm::Coord2D( 20, 0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0, 10 ), te::gm::Coord2D( 0, 20 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 10, 10 ), te::gm::Coord2D( 20, 20 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 5, 5 ), te::gm::Coord2D( 10, 10 ) ) );    
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 5, 10 ), te::gm::Coord2D( 10, 20 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 35, 7 ), te::gm::Coord2D( 70, 14 ) ) );
    

  CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
  
  CPPUNIT_ASSERT( transfPtr->getName() == "SecondDegreePolynomial" );
  
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0000001 );
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 20, 0 ), 0.0000001 );        
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 0, 10 ), 
    te::gm::Coord2D( 0, 20 ), 0.0000001 );    
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 10, 10 ), 
    te::gm::Coord2D( 20, 20 ), 0.0000001 );    
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 5, 5 ), 
    te::gm::Coord2D( 10, 10 ), 0.0000001 );    
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 5, 10 ), 
    te::gm::Coord2D( 10, 20 ), 0.0000001 );      
    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0000001 );
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 20, 0 ), 0.0000001 );        
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 0, 10 ), 
    te::gm::Coord2D( 0, 20 ), 0.0000001 );    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 10, 10 ), 
    te::gm::Coord2D( 20, 20 ), 0.0000001 );    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 5, 5 ), 
    te::gm::Coord2D( 10, 10 ), 0.0000001 );    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 5, 10 ), 
    te::gm::Coord2D( 10, 20 ), 0.0000001 );    
   
}

void TsGeometricTransformations::tcThirdDegreePolynomialGT()
{
  std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
    te::gm::GTFactory::make( "ThirdDegreePolynomial" ) );
  CPPUNIT_ASSERT( transfPtr.get() != 0 );
    
  te::gm::GTParameters transfParams;
  
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0, 0 ), te::gm::Coord2D( 0, 0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 10, 0 ), te::gm::Coord2D( 20, 0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0, 3 ), te::gm::Coord2D( 0, 6 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 11, 11 ), te::gm::Coord2D( 22, 22 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 5, 0 ), te::gm::Coord2D( 10, 0 ) ) );    
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 9, 10 ), te::gm::Coord2D( 18, 20 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 3, 14 ), te::gm::Coord2D( 6, 28 ) ) );    
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 4, 11 ), te::gm::Coord2D( 8, 22 ) ) );    
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 50, 14), te::gm::Coord2D( 100, 28 ) ) );    
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 50, 200), te::gm::Coord2D( 100, 400 ) ) );    
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 3, 450), te::gm::Coord2D( 6, 900 ) ) );     

  CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
  
  CPPUNIT_ASSERT( transfPtr->getName() == "ThirdDegreePolynomial" );
  
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0001 );
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 20, 0 ), 0.0001 );        
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 0, 3 ), 
    te::gm::Coord2D( 0, 6 ), 0.0001 );    
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 11, 11 ), 
    te::gm::Coord2D( 22, 22 ), 0.0001 );    
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 5, 0 ), 
    te::gm::Coord2D( 10, 0 ), 0.0001 );    
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 9, 10 ), 
    te::gm::Coord2D( 18, 20 ), 0.0001 );      
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 3, 14 ), 
    te::gm::Coord2D( 6, 28 ), 0.0001 ); 
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 4, 11 ), 
    te::gm::Coord2D( 8, 22 ), 0.0001 ); 
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 50, 14 ), 
    te::gm::Coord2D( 100, 28 ), 0.0001 ); 
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 50, 200 ), 
    te::gm::Coord2D( 100, 400 ), 0.0001 );    
    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 0, 0 ), 
    te::gm::Coord2D( 0, 0 ), 0.0001 );
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 10, 0 ), 
    te::gm::Coord2D( 20, 0 ), 0.0001 );        
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 0, 3 ), 
    te::gm::Coord2D( 0, 6 ), 0.0001 );    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 11, 11 ), 
    te::gm::Coord2D( 22, 22 ), 0.0001 );    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 5, 0 ), 
    te::gm::Coord2D( 10, 0 ), 0.0001 );    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 9, 10 ), 
    te::gm::Coord2D( 18, 20 ), 0.0001 );      
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 3, 14 ), 
    te::gm::Coord2D( 6, 28 ), 0.0001 ); 
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 4, 11 ), 
    te::gm::Coord2D( 8, 22 ), 0.0001 ); 
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 50, 14 ), 
    te::gm::Coord2D( 100, 28 ), 0.0001 ); 
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 50, 200 ), 
    te::gm::Coord2D( 100, 400 ), 0.0001 );     
}

void TsGeometricTransformations::tcProjectiveGT()
{
  std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
    te::gm::GTFactory::make( "Projective" ) );
  CPPUNIT_ASSERT( transfPtr.get() != 0 );
    
  te::gm::GTParameters transfParams;
  
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 1.0, -2.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( -1.0, -1.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( -1.0, 1.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 0.0 ), te::gm::Coord2D( 1.0, 2.0 ) ) );
    

  CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
  
  CPPUNIT_ASSERT( transfPtr->getName() == "Projective" );
  
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 0.0 ), 
    te::gm::Coord2D( 1.0, -2.0 ), 0.0000001 );        
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 1.0 ), 
    te::gm::Coord2D( -1.0, -1.0 ), 0.0000001 );        
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 1.0 ), 
    te::gm::Coord2D( -1.0, 1.0 ), 0.0000001 );    
  TESTDIRECTMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 0.0 ), 
    te::gm::Coord2D( 1.0, 2.0 ), 0.0000001 );    
    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 0.0 ), 
    te::gm::Coord2D( 1.0, -2.0 ), 0.0000001 );        
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 0.0, 1.0 ), 
    te::gm::Coord2D( -1.0, -1.0 ), 0.0000001 );        
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 1.0 ), 
    te::gm::Coord2D( -1.0, 1.0 ), 0.0000001 );    
  TESTINVERSEMAPPING( transfPtr.get(), te::gm::Coord2D( 1.0, 0.0 ), 
    te::gm::Coord2D( 1.0, 2.0 ), 0.0000001 );      
}
