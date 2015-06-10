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
  \file TsUnitsOfMeasure.h
 
  \brief Test suite for the Units of measure  class.
 */

#ifndef __TERRALIB_UNITTEST_COMMON_INTERNAL_UOM_H
#define __TERRALIB_UNITTEST_COMMON_INTERNAL_UOM_H

// TerraLib
//#include "Config.h"

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsUnitsOfMeasure

  \brief Test suite for the Units of Measure Class.

  This test suite will check the following:
  <ul>
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

class TsUnitOfMeasure : public CPPUNIT_NS::TestFixture
{

  // It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsUnitOfMeasure);

  // It registers the class methods as Test Cases belonging to the suit
  CPPUNIT_TEST (tcGetId);
  CPPUNIT_TEST (tcGetName);
  CPPUNIT_TEST (tcGetDescription);
  CPPUNIT_TEST (tcGetSymbol);
  CPPUNIT_TEST (tcGetType);
  CPPUNIT_TEST (tcIsBaseUnit);
  CPPUNIT_TEST (tcGetBaseUnitId);
  CPPUNIT_TEST (tcGetConversionFactors);
  CPPUNIT_TEST (tcGetConversionValue);

  CPPUNIT_TEST_SUITE_END();
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleans up after the test run.
    void tearDown();

  protected:

    // Test Cases:

    /*! \brief Test Case: Checks the unit of measure identification */
    void tcGetId();

    /*! \brief Test Case: Checks the unit of measure name */
    void tcGetName();

    /*! \brief Test Case: Checks the unit of measure description */
    void tcGetDescription();

    /*! \brief Test Case: Checks the unit of measure symbol */
    void tcGetSymbol();

    /*! \brief Test Case: Checks the unit of measure type */
    void tcGetType();

    /*! \brief Test Case: Checks if it is returning true for a base unit and false for non-base units. */
    void tcIsBaseUnit();

    /*! \brief Test Case: Checks the base unit id from which this unit derives of */
    void tcGetBaseUnitId();

    /*! \brief Test Case: Checks if the conversion factors to convert the unit to its base unit are correct */
    void tcGetConversionFactors();

    /*! \brief Test Case:  Checks the multiplicative value to convert the unit to its base unit */
    void tcGetConversionValue();

  private:

  te::common::UnitOfMeasure* uomA;
  te::common::UnitOfMeasure* uomB;
  te::common::UnitOfMeasure* uomC;
  te::common::UnitOfMeasure* uomD;

};

#endif  // __TERRALIB_UNITTEST_COMMON_INTERNAL_UOM_H