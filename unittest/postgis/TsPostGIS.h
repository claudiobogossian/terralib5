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
  \file TsPostGIS.h
 
  \brief A test suit for the PostGIS class.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_POSTGIS_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_POSTGIS_H

// STL
#include <string>
#include <vector>

#include "../dataaccess/TsDataSource.h"

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

class TsPostGIS : public TsDataSource
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsPostGIS );

// It registers the class methods as Test Cases belonging to the suit. 

// Adding the macro with all Test Cases of TsDataSource
  INSERT_DATASOURCE_TC

// Adding specific Test cases for PostGIS (TsPostGIS)
  CPPUNIT_TEST( tcGetConnPool );
  CPPUNIT_TEST( tcGetDialect );
  CPPUNIT_TEST( tcPGTransactor );
  CPPUNIT_TEST( tcGetGeomTypeId );
  CPPUNIT_TEST( tcGetCurrentSchema );
  CPPUNIT_TEST( tcCreatePostgres );
  CPPUNIT_TEST( tcDropPostgresDb );

  CPPUNIT_TEST_SUITE_END();  
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

// Adding  specific Test cases for PostGIS
    void tcCreatePostgres();
    void tcDropPostgresDb();
    void tcGetConnPool();
    void tcGetCurrentSchema();
    void tcGetDialect();
    void tcGetGeomTypeId(); //??
    void tcGetPGISVersion();
    void tcGetPGVersion();
    void tcPGTransactor();

// Test Cases not implemented yet - passar p/cima qdo implementar, em ordem alfabetic
    void tcSetDialect();
    void tcSetCapabilities();

// Specific variables to use here
    std::map<std::string, std::string> m_connInfoNewPostgresDb;
};

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_POSTGIS_H

