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
  \file TsGTFilter.h
 
  \brief Test suite for Geometric Transformations Filter.
 */

#ifndef __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GTFILTER_H
#define __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GTFILTER_H

#include <terralib/geometry.h>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

#include <vector>

/*!
  \class TsGTFilter

  \brief Test suite for Geometric Transformations Filter.
 */
class TsGTFilter : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsGTFilter );

  CPPUNIT_TEST( tcApplyRansacWithTPSet1 );
  
  CPPUNIT_TEST( tcApplyRansacWithTPSet2 );
  
  CPPUNIT_TEST( tcApplyRansacMultiThread );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:
      
    /*! \brief Generate a tie-points set 1. */  
    void generateTestTPSet1( std::vector< te::gm::GTParameters::TiePoint >& 
      tiePoints );
      
    /*! \brief Generate a tie-points set 2. */  
    void generateTestTPSet2( std::vector< te::gm::GTParameters::TiePoint >& 
      tiePoints );      
    
// Test Cases:

    /*! \brief Test Case: ApplyRansac */
    void tcApplyRansacWithTPSet1();
    
    /*! \brief Test Case: ApplyRansac */
    void tcApplyRansacWithTPSet2();
    
    /*! \brief Test Case: tcApplyRansacMultiThread */
    void tcApplyRansacMultiThread();    
};

#endif  // __TERRALIB_UNITTEST_GEOMETRY_INTERNAL_GTFILTER_H

