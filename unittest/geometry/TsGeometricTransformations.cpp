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

#include <boost/lexical_cast.hpp> 

#include <memory>

#include <cmath>

CPPUNIT_TEST_SUITE_REGISTRATION( TsGeometricTransformations );

void TsGeometricTransformations::setUp()
{
}

void TsGeometricTransformations::tearDown()
{
}

void TsGeometricTransformations::testTiePoints( 
  const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
  const te::gm::GeometricTransformation& transformation,
  double maxError ) const
{
  CPPUNIT_ASSERT( transformation.isValid() );
  
  te::gm::Coord2D mappedCoord;
  double diffx = 0;
  double diffy = 0;
  double derror = 0;
  double ierror = 0;
  
  for( unsigned int tpIdx = 0 ; tpIdx < tiePoints.size() ; ++tpIdx )
  {
    transformation.directMap( tiePoints[ tpIdx ].first, mappedCoord );
    diffx = mappedCoord.x - tiePoints[ tpIdx ].second.x;
    diffy = mappedCoord.y - tiePoints[ tpIdx ].second.y;
    derror = sqrt( ( diffx * diffx ) + ( diffy * diffy ) );

    transformation.inverseMap( tiePoints[ tpIdx ].second, mappedCoord );
    diffx = mappedCoord.x - tiePoints[ tpIdx ].first.x;
    diffy = mappedCoord.y - tiePoints[ tpIdx ].first.y;
    ierror = sqrt( ( diffx * diffx ) + ( diffy * diffy ) );
    
    if( ( derror > maxError ) || ( ierror > maxError ) )
    {
      CPPUNIT_FAIL( 
        "["
        + boost::lexical_cast<std::string>( tiePoints[ tpIdx ].first.x )
        + ","
        + boost::lexical_cast<std::string>( tiePoints[ tpIdx ].first.y )
        + "]->["
        + boost::lexical_cast<std::string>( tiePoints[ tpIdx ].second.x )
        + ","
        + boost::lexical_cast<std::string>( tiePoints[ tpIdx ].second.y )
        + "] derror:"
        + boost::lexical_cast<std::string>( derror )
        + " ierror:"
        + boost::lexical_cast<std::string>( ierror )
      );
    }    
  }  
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
//  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
//    te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( 0.0, 1.0 ) ) );
//  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
//    te::gm::Coord2D( 1.0, 0.0 ), te::gm::Coord2D( 1.0, 0.0 ) ) );

  CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
  
  CPPUNIT_ASSERT( transfPtr->getName() == "RST" );
  
  testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );
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
//  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
//    te::gm::Coord2D( 1.0, 0.0 ), te::gm::Coord2D( 1.0, 2.0 ) ) );
    

  CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
  
  CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
  
testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );  
}

void TsGeometricTransformations::tcAffineDecompose()
{
  // translation X+1
  
  {
    std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
      te::gm::GTFactory::make( "Affine" ) );
    CPPUNIT_ASSERT( transfPtr.get() != 0 );    
    
    te::gm::GTParameters transfParams;
    
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 1.0, 0.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( 1.0, 1.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( 2.0, 1.0 ) ) );
      

    CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
    CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
    
    testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );  
      
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
      
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, translationX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, skew, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, squeeze, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scaling, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, rotation, 0.00000000001 );
  }
  
  // translation X+1
  
  {
    std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
      te::gm::GTFactory::make( "Affine" ) );
    CPPUNIT_ASSERT( transfPtr.get() != 0 );    
    
    te::gm::GTParameters transfParams;
    
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( -1.0, 0.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( -1.0, 1.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( 0.0, 1.0 ) ) );
      

    CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
    CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
    
    testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );  
      
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
      
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, translationX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, skew, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, squeeze, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scaling, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, rotation, 0.00000000001 );
  }  
  
  // translation Y+1
  
  {
    std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
      te::gm::GTFactory::make( "Affine" ) );
    CPPUNIT_ASSERT( transfPtr.get() != 0 );    
    
    te::gm::GTParameters transfParams;
    
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 0.0, 1.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( 0.0, 2.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( 1.0, 2.0 ) ) );
      

    CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
    CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
    
    testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );  
      
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
      
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, translationY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, skew, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, squeeze, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scaling, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, rotation, 0.00000000001 );
  }  
  
  // translation Y-1
  
  {
    std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
      te::gm::GTFactory::make( "Affine" ) );
    CPPUNIT_ASSERT( transfPtr.get() != 0 );    
    
    te::gm::GTParameters transfParams;
    
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 0.0, -1.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( 0.0, 0.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( 1.0, 0.0 ) ) );
      

    CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
    CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
    
    testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );  
      
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
      
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, translationY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, skew, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, squeeze, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scaling, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, rotation, 0.00000000001 );
  }  
  
  // 90 deg clock wise rotation around the center
  
  {
    std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
      te::gm::GTFactory::make( "Affine" ) );
    CPPUNIT_ASSERT( transfPtr.get() != 0 );    
    
    te::gm::GTParameters transfParams;
    
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 0.0, 0.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( 1.0, 0.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( 1.0, -1.0 ) ) );
      

    CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
    CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
    
    testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );  
      
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
      
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, skew, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, squeeze, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scaling, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.570796, rotation, 0.001 );
  }  
  
  // 90 deg counter clock wise rotation around the center
  
  {
    std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
      te::gm::GTFactory::make( "Affine" ) );
    CPPUNIT_ASSERT( transfPtr.get() != 0 );    
    
    te::gm::GTParameters transfParams;
    
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 0.0, 0.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( -1.0, 0.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( -1.0, 1.0 ) ) );
      

    CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
    CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
    
    testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );  
      
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
      
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, skew, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, squeeze, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scaling, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.570796, rotation, 0.001 );
  }   
  
  // scale +2X
  
  {
    std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
      te::gm::GTFactory::make( "Affine" ) );
    CPPUNIT_ASSERT( transfPtr.get() != 0 );    
    
    te::gm::GTParameters transfParams;
    
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 0.0, 0.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( 0.0, 1.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( 2.0, 1.0 ) ) );
      

    CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
    CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
    
    testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );  
      
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
      
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, scalingFactorX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, skew, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.414213, squeeze, 0.0001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.414213, scaling, 0.0001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, rotation, 0.00000000001 );
  }  
  
  // scale -2X
  
  {
    std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
      te::gm::GTFactory::make( "Affine" ) );
    CPPUNIT_ASSERT( transfPtr.get() != 0 );    
    
    te::gm::GTParameters transfParams;
    
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 0.0, 0.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( 0.0, 1.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( -2.0, 1.0 ) ) );
      

    CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
    CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
    
    testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );  
      
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
      
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, scalingFactorX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, skew, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.707106, squeeze, 0.0001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.414213, scaling, 0.0001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, rotation, 0.00000000001 );
  }  
  
  // scale 2Y
  
  {
    std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
      te::gm::GTFactory::make( "Affine" ) );
    CPPUNIT_ASSERT( transfPtr.get() != 0 );    
    
    te::gm::GTParameters transfParams;
    
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 0.0, 0.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( 0.0, 2.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( 1.0, 2.0 ) ) );
      

    CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
    CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
    
    testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );  
      
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
      
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, scalingFactorY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, skew, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.707106, squeeze, 0.0001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.414213, scaling, 0.0001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, rotation, 0.00000000001 );
  }   
  
  // scale -2Y
  
  {
    std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
      te::gm::GTFactory::make( "Affine" ) );
    CPPUNIT_ASSERT( transfPtr.get() != 0 );    
    
    te::gm::GTParameters transfParams;
    
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 0.0, 0.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( 0.0, -2.0 ) ) );
    transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
      te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( 1.0, -2.0 ) ) );
      

    CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
    CPPUNIT_ASSERT( transfPtr->getName() == "Affine" );
    
    testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );  
      
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
      
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, translationY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, scalingFactorX, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, scalingFactorY, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, skew, 0.00000000001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.414213, squeeze, 0.0001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.414213, scaling, 0.0001 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, rotation, 0.00000000001 );
  }   
}

void TsGeometricTransformations::tcSecondDegreePolynomialGT()
{
  std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
    te::gm::GTFactory::make( "SecondDegreePolynomial" ) );
  CPPUNIT_ASSERT( transfPtr.get() != 0 );
    
  te::gm::GTParameters transfParams;
  
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 1.0, -1.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 0.0 ), te::gm::Coord2D( 0.0, 0.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 2.0, 0.0 ), te::gm::Coord2D( 1.0, 1.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 2.0, 2.0 ), te::gm::Coord2D( -1.0, 1.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 2.0 ), te::gm::Coord2D( -2.0, 0.0 ) ) );    
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0.0, 2.0 ), te::gm::Coord2D( -1.0, -1.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( -1.0, 0.0 ) ) );
    

  CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
  
  CPPUNIT_ASSERT( transfPtr->getName() == "SecondDegreePolynomial" );
  
  testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );
}

void TsGeometricTransformations::tcThirdDegreePolynomialGT()
{
  std::auto_ptr< te::gm::GeometricTransformation > transfPtr( 
    te::gm::GTFactory::make( "ThirdDegreePolynomial" ) );
  CPPUNIT_ASSERT( transfPtr.get() != 0 );
    
  te::gm::GTParameters transfParams;
  
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0.0, 2.0 ), te::gm::Coord2D( 0.0, 3.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 2.0 ), te::gm::Coord2D( 1.0, 2.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 2.0, 2.0 ), te::gm::Coord2D( 2.0, 2.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 3.0, 2.0 ), te::gm::Coord2D( 3.0, 3.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0.0, 1.0 ), te::gm::Coord2D( 0.0, 2.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 1.0 ), te::gm::Coord2D( 1.0, 1.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 2.0, 1.0 ), te::gm::Coord2D( 2.0, 1.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 0.0, 0.0 ), te::gm::Coord2D( 0.0, 1.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 1.0, 0.0 ), te::gm::Coord2D( 1.0, 0.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 2.0, 0.0 ), te::gm::Coord2D( 2.0, 0.0 ) ) );
  transfParams.m_tiePoints.push_back( te::gm::GTParameters::TiePoint( 
    te::gm::Coord2D( 3.0, 0.0 ), te::gm::Coord2D( 3.0, 1.0 ) ) );

  CPPUNIT_ASSERT( transfPtr->initialize( transfParams ) );
  
  CPPUNIT_ASSERT( transfPtr->getName() == "ThirdDegreePolynomial" );
  
  testTiePoints( transfParams.m_tiePoints, *transfPtr, 1.0 );
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
  
  testTiePoints( transfParams.m_tiePoints, *transfPtr, 0.00001 );    
}
