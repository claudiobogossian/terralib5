/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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

/*!
  \file TsTranslator.h
 
  \brief Test suite for the Translator framework.
 */

#ifndef __TERRALIB_UNITTEST_COMMON_INTERNAL_TRANSLATOR_H
#define __TERRALIB_UNITTEST_COMMON_INTERNAL_TRANSLATOR_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsTranslator

  \brief Test suite for the Translator framework.

  This test suite will check the following:
  <ul>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  </ul>
 */
class TsTranslator : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsTranslator );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcLocalePtBR );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: setting a pt_BR locale. */
    void tcLocalePtBR();

};

#endif  // __TERRALIB_UNITTEST_COMMON_INTERNAL_TRANSLATOR_H

