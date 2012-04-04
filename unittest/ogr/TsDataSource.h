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
  \file TsDataSource.h
 
  \brief Test suite for TerraLib OGR driver.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_OGR_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_OGR_H

// STL
#include <string>
#include <vector>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

// Forward declarations
namespace te
{
  namespace da
  {
    class DataSource;
  }
}

/*!
  \class TsDataSource

  \brief Test suite for TerraLib OGR driver.

  This test suite will check the following:
  <ul>
  <li>Openning a DataSource;</li>
  <li>Getting features;</li>
  <li>Getting features that obey a spatial restriction;</li>
  </ul>
 */
class TsDataSource : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsDataSource );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcOpen );
  CPPUNIT_TEST( tcGetFeature );
  CPPUNIT_TEST( tcGetFeatureWithSpatialRestriction );


  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

    te::da::DataSource* connectDataSource();

// Test Cases:

    /*! \brief Test Case: openning an OGR data source. */
    void tcOpen();

    /*! \brief Test Case: getting a feature from an OGR data source. */
    void tcGetFeature();

    /*! \brief Test Case: getting a feature from an OGR data source using a spatial restriction. */
    void tcGetFeatureWithSpatialRestriction();
};

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_OGR_H

