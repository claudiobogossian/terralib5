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
  \file TsArray.h
 
  \brief Test suite for the Array class.
 */

#ifndef __TERRALIB_UNITTEST_DATATYPE_INTERNAL_ARRAY_H
#define __TERRALIB_UNITTEST_DATATYPE_INTERNAL_ARRAY_H

// STL
#include <string>
#include <vector>

// STL
#include <cstdio>
#include <cstdlib>
#include <cstring>

//TerraLib 
#include <terralib/datatype.h>
#include <terralib/geometry.h>
#include <terralib/datatype/Array.h>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsArray

  \brief Test suite for the Array Class.

  This test suite will check the following:
  <ul>
  <li> </li>
  <li></li>
  <li></li>
  <li></li>
  </ul>
 */
class TsArray : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsArray );

// It registers the class methods as Test Cases belonging to the suit 
  
  CPPUNIT_TEST( tcArray );
  CPPUNIT_TEST( tcGetDimension );
  CPPUNIT_TEST( tcGetElementsTypeCode);
  CPPUNIT_TEST( tcGetDimensionSize );
  CPPUNIT_TEST( tcInsert );
  CPPUNIT_TEST( tcGetData );
  CPPUNIT_TEST( tcOperatorBraket ); // []
  CPPUNIT_TEST( tcClone);
  CPPUNIT_TEST( tcGetTypeCode);
  CPPUNIT_TEST( tcToString);
 
  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: Multi-dimensional array constructor. */
    void tcArray();

    /*! \brief Test Case: Returns the number of dimensions of the array*/
    void tcGetDimension();

    /*! \brief Test Case: Returns the data type of the elements of the array. */
    void tcGetElementsTypeCode();

    /*! \brief Test Case: Gets the number of elements in the i-th dimension. */
    void tcGetDimensionSize();

    /*! \brief Test Case: Inserts data into specified position. */
    void tcInsert();

     /*! \brief Test Case: Returns data from specified position. */
    void tcGetData();

    /*! \brief Test Case: Access data in i index. */
    void tcOperatorBraket();

    /*! \brief Test Case: It creates a new clone of the array. */
    void tcClone();

    /*! \brief Test Case: Copy Constructor. */
//    void tcArrayConstructor(); 

    /*! \brief Test Case: Returns the type code for array data: te::dt::ARRAY_TYPE. */
    void tcGetTypeCode();

    /*! \brief Test Case: It returns an string representation of Array. */
    void tcToString();

  private: 

    te::dt::Array* myarray;
    std::vector<std::size_t> pos;
    te::gm::Point* mypoint1;
    te::gm::Point* mypoint2;
    te::gm::Point* mypoint3;
    te::gm::Point* mypoint4;
    te::gm::Point* mypoint5;
};

#endif  // __TERRALIB_UNITTEST_DATATYPE_INTERNAL_ARRAY_H

