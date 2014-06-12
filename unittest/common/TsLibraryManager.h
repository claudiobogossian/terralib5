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
  \file TsLibraryManager.h
 
  \brief Test suite for the Library Manager class.
 */

#ifndef __TERRALIB_UNITTEST_COMMON_INTERNAL_LIBRARYM_H
#define __TERRALIB_UNITTEST_COMMON_INTERNAL_LIBRARYM_H

// TerraLib
//#include "Config.h"
#include "../../src/terralib/common/Library.h"
#include "../../src/terralib/common/LibraryManager.h"

// Boost
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsLibraryManager

  \brief Test suite for the Library Manager Class.

  This test suite will check the following:
  <ul>
  <li>  Attempts to add a Library in the manager. </li>
  <li>  Attempts to find a Library in the manager. </li>
  </ul>
 */

class TsLibraryManager : public CPPUNIT_NS::TestFixture
{

// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsLibraryManager );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcAdd ); 
  CPPUNIT_TEST( tcFind ); 

  CPPUNIT_TEST_SUITE_END();

  public:

// It sets up context before running the test.
    void setUp();

// It cleans up after the test run.
    void tearDown();

  protected:

// Test Cases:

     /*! \brief Test Case: Find a library on the manager. */
    void tcAdd ();

    /*! \brief Test Case: Find a library on the manager. */
    void tcFind ();

  private:

    te::common::Library* lib1;
    te::common::Library* lib2;

    te::common::LibraryPtr ptr_Lib1;
    te::common::LibraryPtr ptr_Lib2;

};

#endif  // __TERRALIB_UNITTEST_COMMON_INTERNAL_LIBRARYM_H
