/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file TsLibrary.h
 
  \brief Test suite for the Library  class.
 */

#ifndef __TERRALIB_UNITTEST_COMMON_INTERNAL_LIBRARY_H
#define __TERRALIB_UNITTEST_COMMON_INTERNAL_LIBRARY_H

// TerraLib
//#include "Config.h"
#include "../../src/terralib/common/Library.h"

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsLibrary

  \brief Test suite for the library class.

  This test suite will check the following:
  <ul>
  <li> Inserting new unit of measure. </li>
  </ul>
 */

class TsLibrary : public CPPUNIT_NS::TestFixture
{

  // It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsLibrary);

  // It registers the class methods as Test Cases belonging to the suit
  CPPUNIT_TEST (tcLoad);
  CPPUNIT_TEST (tcUnload);
  CPPUNIT_TEST (tcGetFileName);
  CPPUNIT_TEST (tcGetNativeName);
  CPPUNIT_TEST (tcAddSearchDir);
  CPPUNIT_TEST (tcResetSearchPath);
  CPPUNIT_TEST (tcGetAddress);

  CPPUNIT_TEST_SUITE_END();
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleans up after the test run.
    void tearDown();

  protected:

    // Test Cases:

    /*! \brief Test Case: Attempt to load a library  */
    void tcLoad();

    /*! \brief Test Case:*/
    void tcUnload();

    /*! \brief Test Case:*/
    void tcGetFileName();

    /*! \brief Test Case:*/
    void tcGetNativeName();

    /*! \brief Test Case:*/
    void tcAddSearchDir();

    /*! \brief Test Case:*/
    void tcResetSearchPath();

    /*! \brief Test Case:*/
    void tcGetAddress();

  private:

    te::common::Library* lib1;
    te::common::Library* lib2;
    te::common::Library* lib3;

};

#endif  // __TERRALIB_UNITTEST_COMMON_INTERNAL_LIBRARY_H