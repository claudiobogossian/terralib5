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
  \file TsDataSetTypePersistencePostGIS.h
 
  \brief Test suite for the PostGIS DataSetTypePersistence implemention.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASETTYPEPERSISTENCEPOSTGIS_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASETTYPEPERSISTENCEPOSTGIS_H

// STL
#include <string>
#include <vector>

// Terralib
#include "../dataaccess/TsDataSetTypePersistence.h"

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

//namespace te { namespace da { class DataSource; } }

/*!
  \class TsDataSetTypePersistencePostGIS

  \brief Test suite for the PostGIS DataSetTypePersistence implemention.

  This test suite will perform checks in the PostGIS DataSetTypePersistence.
  <br>
  It will check the following:
  <ul>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  </ul>
 */
class TsDataSetTypePersistencePostGIS : public TsDataSetTypePersistence
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsDataSetTypePersistencePostGIS );

// It registers the base class methods as Test Cases belonging to the suit.  
  INSERT_DATASETTYPEPERSISTENCE_TC ;

// It registers the derived class methods as Test Cases belonging to the suit 
   //there is no specific method for PostGis...

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

// Adding specific Test cases for PostGIS DataSetTypePersistence 
   //there is no specific method for PostGis...

};

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASETTYPEPERSISTENCEPOSTGIS_H

