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
  \file TsPostGISInfo.h
 
  \brief A test suit for the PostGISInfo class.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_POSTGISINFO_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_POSTGISINFO_H

// TerraLib Unit-test
#include "../dataaccess/TsDataSourceInfo.h"

class TsPostGISInfo : public TsDataSourceInfo
{

// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsPostGISInfo );

// It registers the class methods as Test Cases belonging to the suit 
  INSERT_DATASOURCEINFO_TC

// Adding specific Test cases for PostGIS (TsPostGISInfo)
  CPPUNIT_TEST( tcGetType );
  CPPUNIT_TEST( tcGetDbName );
  CPPUNIT_TEST( tcToString );
  CPPUNIT_TEST( tcAllGetsRelatedWithInfo );
  CPPUNIT_TEST( tcFromString );
  CPPUNIT_TEST( tcToConnectionString );
  
//...
  CPPUNIT_TEST_SUITE_END(); 
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

// Adding specific Test cases for PostGIS 
    void tcGetType();
    void tcGetDbName();
    void tcToString();
    void tcAllGetsRelatedWithInfo ();

// Not implemented yet! - passar p/ cima em ordem alphabetica e 
    //incluir em CPPUNIT_TEST Specific Test Casses for Postgis na ordem de chamada
    void tcFromString ();
    void tcToConnectionString ();
    void tcClone ();

  protected: 

     te::pgis::DataSourceInfo* m_pgisInfoChk; //used for static cast in pgis methods
     te::pgis::DataSourceInfo* m_pgisInfo; //used for static cast in pgis methods
};

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_POSTGISINFO_H
