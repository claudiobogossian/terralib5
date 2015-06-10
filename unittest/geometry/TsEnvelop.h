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
  \file TsEnvelop.h
 
  \brief Test suite for the Envelop class.
 */

#ifndef __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_ENVELOPE_H
#define __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_ENVELOPE_H

// STL
#include <string>
#include <vector>

// STL
#include <cstdio>
#include <cstdlib>
#include <cstring>

//TerraLib 
#include <terralib/common.h>
#include <terralib/geometry.h>


// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsEnvelop

  \brief Test suite for the Envelop Class.

  This test suite will check the following:
  <ul>
  <li> </li>
  <li></li>
  <li></li>
  <li></li>
  </ul>
 */
class TsEnvelop : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsEnvelop );

// It registers the class methods as Test Cases belonging to the suit 
  
  CPPUNIT_TEST( tcEnvelopInvalid );
  CPPUNIT_TEST( tcEnvelopValid );
  CPPUNIT_TEST( tcEnvelopCopy );
  CPPUNIT_TEST( tcEnvelopInit );
  CPPUNIT_TEST( tcEquals );
  CPPUNIT_TEST( tcGetLowerLeftX );
  CPPUNIT_TEST( tcGetLowerLeftY );
  CPPUNIT_TEST( tcGetUpperRightX );
  CPPUNIT_TEST( tcGetUpperRightY );
  CPPUNIT_TEST( tcGetLowerLeftCoord2D );
  CPPUNIT_TEST( tcGetUpperRightCoord2D );
  CPPUNIT_TEST( tcMakeInvalid );
  CPPUNIT_TEST( tcIsValidRect );
  CPPUNIT_TEST( tcGetWidth_Right_Area );
  CPPUNIT_TEST( tcIntersects );
  CPPUNIT_TEST( tcDisjoint );
  CPPUNIT_TEST( tcTouches );
  CPPUNIT_TEST( tcIntersection );
  CPPUNIT_TEST( tcContains );
  CPPUNIT_TEST( tcWithin );
  CPPUNIT_TEST( tcUnion );


  //CPPUNIT_TEST( tcSpatialOperations );
  CPPUNIT_TEST( tcSpatialOperations1 );

  //CPPUNIT_TEST( tcOperatorCompare );
  //CPPUNIT_TEST( tcOperatorEqual );
  //CPPUNIT_TEST( tcTransform );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: Constructs an envelope with invalid coordinates. */
    void tcEnvelopInvalid();

    /*! \brief Test Case: Constructs an envelope with valid coordinates.. */
    void tcEnvelopValid();

    /*! \brief Test Case: Copy a envelop. */
    void tcEnvelopCopy();

    /*! \brief Test Case: Initialize a envelop. */
    void tcEnvelopInit();

    /*! \brief Test Case: Check if the envelopes are "spatially equal". */
    void tcEquals();

    /*! \brief Test Case: Get the lower left X coordinate. */
    void tcGetLowerLeftX();

    /*! \brief Test Case: Get the lower left Y coordinate. */
    void tcGetLowerLeftY();

    /*! \brief Test Case: Get the lower left X coordinate. */
    void tcGetUpperRightX();

    /*! \brief Test Case: Get the lower left X coordinate. */
    void tcGetUpperRightY();

    /*! \brief Test Case: Get the lower left Coord2D. */
    void tcGetLowerLeftCoord2D();

    /*! \brief Test Case: Get the lower left X coordinate. */
    void tcGetUpperRightCoord2D();

    /*! \brief Test Case: Invalidate the envelop. */
    void tcMakeInvalid();

    void tcIsValidRect();

    void tcGetWidth_Right_Area();
    ////void tcGetWidth();
    ////void tcGetHeight();
    ////void tcGetArea();

    void tcSpatialOperations();
    void tcSpatialOperations1(); //box of Ouro Preto

    /////*! \brief Test Case: Assign operator. */
    ////void tcOperatorAssign();

    /////*! \brief Test Case: Compare operator */
    ////void tcOperatorCompare();

    /////*! \brief Test Case:  Equals */
    ////void tcOperatorEqual();

    /*! \brief Test Case:  Disjoint */
    void tcDisjoint();

    /*! \brief Test Case:  Intersects */
    void tcIntersects();

    /*! \brief Test Case:  Touches */
    void tcTouches();

    /*! \brief Test Case:  Within */
    void tcWithin();

    /*! \brief Test Case:  Contains */
    void tcContains();

    /*! \brief Test Case:  Intersection */
    void tcIntersection();

    /*! \brief Test Case: Updates the envelop with another envelop. */
    void tcUnion();


  private:

    te::gm::Envelope* boxA; 
    te::gm::Envelope* boxAEqual; 

    te::gm::Envelope* boxB; 
    te::gm::Envelope* boxC;
    te::gm::Envelope* boxD;
    te::gm::Envelope* boxE;
    te::gm::Envelope* boxF;
    te::gm::Envelope* boxG; 
    te::gm::Envelope* boxH;

    te::gm::Envelope* boxAeB; 
    te::gm::Envelope* boxAeF; 
    te::gm::Envelope* boxAexpandedB;
    te::gm::Envelope* boxGexpandedA;
};

#endif  // __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_ENVELOPE_H

