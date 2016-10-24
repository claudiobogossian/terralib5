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
  \file TsUnitsOfMeasureManager.h
 
  \brief Test suite for the Units of Measure Manager class.
 */

#ifndef __TERRALIB_UNITTEST_COMMON_INTERNAL_UOMM_H
#define __TERRALIB_UNITTEST_COMMON_INTERNAL_UOMM_H

// TerraLib
//#include "Config.h"
#include "../../src/terralib/common/UnitsOfMeasureManager.h"
#include "../../src/terralib/common/UnitOfMeasure.h"

// STL
#include <map>
#include <string>
#include <vector>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsUnitsOfMeasureManager

  \brief Test suite for the Units of Measure Manager Class.

  This test suite will check the following:
  <ul>
  <li>  Initializes the manager with data from a json file. </li>
  <li> Inserting new unit of measure. </li>
  <li> Inserting new unit of measure with alternative names. </li>
  <li> Removing an unit of measure from the manager. </li>
  <li> Finding an unit of measure identified by it's identificaton number. </li>
  <li> Finding an unit of measure identified by it's name. </li>
  <li> Finding an unit of measure identified by one of it's alternative names. </li>
  <li> Finding an unit of measure identified either by it's name or one of it's alternative names. </li>
  <li> Returning a pair of iterators over the managed units of measure. </li>
  <li> Clearing all the managed units. </li>
  <li> Returning the alternative names for a unit of measure. </li>
  <li> Calculating a multiplicative factor to convert from a given unit to its base unit and vice-versa. </li>
  </ul>
 */

class TsUnitsOfMeasureManager : public CPPUNIT_NS::TestFixture
{

// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsUnitsOfMeasureManager );

// It registers the class methods as Test Cases belonging to the suit 
//  CPPUNIT_TEST( tcFindUomById ); 
  CPPUNIT_TEST( tcFindUomByName );
  CPPUNIT_TEST( tcFindUomByAltName );
  CPPUNIT_TEST( tcFindUomByNames );
  CPPUNIT_TEST( tcGeAltNames );
  CPPUNIT_TEST( tcConvert );

  CPPUNIT_TEST_SUITE_END();

  public:

// It sets up context before running the test.
    void setUp();

// It cleans up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: Find an unit of using it's identificaton number. 
    void tcFindUomById ();
    */

    /*! \brief Test Case: Find an unit of using it's name. */
    void tcFindUomByName ();

    /*! \brief Test Case: Find an unit of using one of it's alternative name. */
    void tcFindUomByAltName ();

    /*! \brief Test Case: Find an unit of using either it's name or on e of it's alternative names. */
    void tcFindUomByNames ();

    /* ! \brief Test Case: Returns the alternative names for a unit of measure.*/
    void tcGeAltNames ();

    /* ! \brief Test Case: Calculates a multiplicative factor to convert from a given unit to its base unit and vice-versa.*/
    void tcConvert ();

  private:

  te::common::UnitOfMeasurePtr uomA;
  te::common::UnitOfMeasurePtr uomB;
  te::common::UnitOfMeasurePtr uomC;
  te::common::UnitOfMeasurePtr uomD;

  std::vector<std::string> alternativeNames;

};

#endif  // __TERRALIB_UNITTEST_COMMON_INTERNAL_UOMM_H
