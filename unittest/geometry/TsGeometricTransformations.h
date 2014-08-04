/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file TsGeometricTransformations.h
 
  \brief Test suite for Geometric Transformations.
 */

#ifndef __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GEOMETRICTRANSFORMATIONS_H
#define __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GEOMETRICTRANSFORMATIONS_H

#include <terralib/geometry/GeometricTransformation.h>

// STL
//#include <string>
//#include <vector>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsGeometricTransformations

  \brief Test suite for Geometric Transformations.
 */
class TsGeometricTransformations : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsGeometricTransformations );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcRSTGT );
  CPPUNIT_TEST( tcAffine );
  CPPUNIT_TEST( tcAffineDecompose );
  CPPUNIT_TEST( tcSecondDegreePolynomialGT );
  CPPUNIT_TEST( tcThirdDegreePolynomialGT );
  CPPUNIT_TEST( tcProjectiveGT );  

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:
    
    void testTiePoints( const std::vector< te::gm::GTParameters::TiePoint >& tiePoints,
      const te::gm::GeometricTransformation& transformation,
      double maxError ) const;
      
// Test Cases:

    /*! \brief Test Case: RST transformation. */
    void tcRSTGT();    

    /*! \brief Test Case: Affine transformation. */
    void tcAffine();
    
    /*! \brief Test Case: Affine transformation decompose method. */
    void tcAffineDecompose();    
    
    /*! \brief Test Case: Second degree polynomial transformation. */
    void tcSecondDegreePolynomialGT();
    
    /*! \brief Test Case: Third degree polynomial transformation. */
    void tcThirdDegreePolynomialGT();    
    
    /*! \brief Test Case: Projective transformation. */
    void tcProjectiveGT();
};

#endif  // __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GEOMETRY_H

