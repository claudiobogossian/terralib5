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
  \file TsLayerExplorer.h
 
  \brief Test suite for the LayerExplorer framework.
 */

#ifndef __TERRALIB_UNITTEST_QTWIDGETS_INTERNAL_LAYEREXPLORER_H
#define __TERRALIB_UNITTEST_QTWIDGETS_INTERNAL_LAYEREXPLORER_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsLayerExplorer

  \brief Test suite for the LayerExplorer framework.

  This test suite will check the following:
  <ul>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  </ul>
 */
class TsLayerExplorer : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsLayerExplorer );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcCreateExplorer );
  CPPUNIT_TEST( tcPerformanceExplorer );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: creating a simple explorer for a set of in-memory layers. */
    void tcCreateExplorer();

    /*! \brief Test Case: do some tests for performance measurement. */
    void tcPerformanceExplorer();

};

#endif  // __TERRALIB_UNITTEST_QTWIDGETS_INTERNAL_LAYEREXPLORER_H

