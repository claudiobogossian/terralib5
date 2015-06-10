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
  \file TsRTree.h
 
  \brief Test suite for the RTree class.
 */

#ifndef __TERRALIB_UNITTEST_SAM_INTERNAL_RTREE_H
#define __TERRALIB_UNITTEST_SAM_INTERNAL_RTREE_H

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
#include <terralib/sam.h>


// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsRTree

  \brief Test suite for the RTree Class.

  This test suite will check the following:
  <ul>
  <li> </li>
  <li></li>
  <li></li>
  <li></li>
  </ul>
 */

class TsRTree : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsRTree );

// It registers the class methods as Test Cases belonging to the suit 
  
  CPPUNIT_TEST( tcRTreeInts );
  CPPUNIT_TEST( tcRTreeUnsignedInts );
  CPPUNIT_TEST( tcRTreePointers );
  CPPUNIT_TEST( tcRTreeGradeInts );
  CPPUNIT_TEST( tcRTreeBox );
  CPPUNIT_TEST( tcRTreeBox_4 );
  CPPUNIT_TEST( tcRTreeBox_3 );
  CPPUNIT_TEST( tcRTreeBox_2 );
 
  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: Constructs an RTree using int as DATATYPE . */
    void tcRTreeInts();
   /*! \brief Test Case: Constructs an RTree using unsigned int as DATATYPE . */
    void tcRTreeUnsignedInts();
   /*! \brief Test Case: Constructs an RTree using pointer as DATATYPE . */
    void tcRTreePointers();
   /*! \brief Test Case: Constructs an RTree inserting a gride of ints . */
    void tcRTreeGradeInts();
   /*! \brief Test Case: Constructs an RTree inserting a gride of unitary box . */
    void tcRTreeBox();
   /*! \brief Test Case: Constructs an RTree (MAXNODE = 4) inserting the defined rectangles and search the RTree. */
    void tcRTreeBox_4();
   /*! \brief Test Case: Constructs an RTree (MAXNODE = 3) inserting the defined rectangles and search the RTree. */
    void tcRTreeBox_3();
   /*! \brief Test Case: Constructs an RTree (MAXNODE = 2) inserting the defined rectangles and search the RTree. */
    void tcRTreeBox_2();

// Boxes to be inserted in a RTree in the last 3 test cases
    te::gm::Envelope* r15;  // new te::gm::Envelope(1.0,1.0, 2.5,2.5);
    te::gm::Envelope* r16;  // new te::gm::Envelope(3.2,1.0, 7.5,4.0);
    te::gm::Envelope* r17;  // new te::gm::Envelope(12.5,3.5, 17.5,5.0);
    te::gm::Envelope* r18;  // new te::gm::Envelope(14.0,1.7, 15,5.9);
    te::gm::Envelope* r19;  // new te::gm::Envelope(14.5,2.5, 16.5,3.5);
    te::gm::Envelope* r12;  // new te::gm::Envelope(5.0,4.5, 8.5,6.5);
    te::gm::Envelope* r08;  // new te::gm::Envelope(3.3,6.5, 4.5,7.5);
    te::gm::Envelope* r09;  // new te::gm::Envelope(5.2,8.5, 6.3,9.5);
    te::gm::Envelope* r10;  // new te::gm::Envelope(5.2,7.2, 6.3,8.2);
    te::gm::Envelope* r11;  // new te::gm::Envelope(8,1.8, 9,10.5);
    te::gm::Envelope* r13;  // new te::gm::Envelope(11,4.3, 11.9,9);
    te::gm::Envelope* r14;  // new te::gm::Envelope(10.2,11.5, 6.3,8.0);

//Researching boxes 
    te::gm::Envelope* r1;  // new te::gm::Envelope(3.0,1.5, 9.5,10.6);
    te::gm::Envelope* r2;  // new te::gm::Envelope(0.5,0.5, 18.0,6.0);
    te::gm::Envelope* r3;  // new te::gm::Envelope(2.7,6.7, 6.5,9.9);
    te::gm::Envelope* r4;  // new te::gm::Envelope(4.6,1.7, 9.3,10.8);
    te::gm::Envelope* r5;  // new te::gm::Envelope(9.5,4.5, 12,9.5);
    te::gm::Envelope* r6;  // new te::gm::Envelope(0.7,0.7,7.7, 4.3);
    te::gm::Envelope* r7;  // new te::gm::Envelope(12.5,1.5,18, 6.0);


  private:

};

#endif  // __TERRALIB_UNITTEST_SAM_INTERNAL_RTREE_H

