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
 
  \brief A test suit for the DataSource interface.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_H

// TerraLib
#include <terralib/dataaccess_fw.h>
#include <terralib/geometry_fw.h>

// STL
#include <string>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsDataSource

  \brief A test suit for the DataSource interface.

  ....
  <br>
  This test suite will check the following:
  <ul>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  <li>;</li>
  </ul>
 */
class TsDataSource : public CPPUNIT_NS::TestFixture
{

// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsDataSource );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcOpenWithStr );
  CPPUNIT_TEST( tcClose );

  CPPUNIT_TEST( tcExist );

  CPPUNIT_TEST( tcSetConnectionInfo );
  CPPUNIT_TEST( tcOpenWithMap ); 
  CPPUNIT_TEST( tcGetType );
  CPPUNIT_TEST( tcIsOpened );
  CPPUNIT_TEST( tcIsValid );
  CPPUNIT_TEST( tcGetCatalog );
  CPPUNIT_TEST( tcGetCatalogFull );
  CPPUNIT_TEST( tcGetCapabilities );
  CPPUNIT_TEST( tcGetTransactor );
  CPPUNIT_TEST( tcGetConnectionInfo );
  CPPUNIT_TEST( tcGetConnectionStr );
  CPPUNIT_TEST( tcClose );

  CPPUNIT_TEST( tcSetConnectionInfo );
  CPPUNIT_TEST( tcSetConnectionStr );

  CPPUNIT_TEST( tcCreate );
  CPPUNIT_TEST( tcDrop );

  CPPUNIT_TEST_SUITE_END();

  public:
// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:
    void tcOpenWithMap();
    void tcOpenWithStr();
    void tcGetType();
    void tcClose();
    void tcIsOpened();
    void tcIsValid();
    void tcGetCatalog();
    void tcGetCatalogFull();
    void tcGetCapabilities();
    void tcGetTransactor();
    void tcGetConnectionInfo();
    void tcGetConnectionStr();
    void tcSQLDialect();


    void tcSetConnectionInfo();
    void tcSetConnectionStr();
    void tcExist();
    void tcCreate();
    void tcDrop();
/*
    virtual void tcGetConnectionInfo();
    virtual void tcSetConnectionInfo();
    virtual void tcGetConnectionStr();
    virtual void tcSetConnectionStr();
    virtual void tcSQLDialect();
    virtual void tcOpen();
    virtual void tcOpenWithString();
    virtual void tcCreate();
    virtual void tcDrop();
*/
  public:
// It will come from the setUp of the derived database class (see:TsPostGIS, TsSQLite, etc)
    te::da::DataSource* m_ds;
    std::map<std::string, std::string> m_capabilit;
    std::map<std::string, std::string> m_connInfo;
    std::string m_connStr;
    std::string m_dsType;
    std::map<std::string, std::string> m_connInfoNewDs;
    std::map<std::string, std::string> m_connInfoDropDs;


/*
    std::string m_connInfoStr;
    std::map<std::string, std::string> m_connInfoDropDs;
    te::da::DataSourceCatalog* m_dsCatalog;
    size_t m_nroDataSets;
*/
};
/*
#define INSERT_DATASOURCE_TC CPPUNIT_TEST( tcOpenWithMap ); \
                             CPPUNIT_TEST( tcGetTransactor ); \
                             CPPUNIT_TEST( tcIsValid ); \
                             CPPUNIT_TEST( tcIsOpened ); \
                             CPPUNIT_TEST( tcGetCapabilities ); \
                             CPPUNIT_TEST( tcClose ); \
                             CPPUNIT_TEST( tcSetConnectionInfo ); \
                             CPPUNIT_TEST( tcGetConnectionInfo ); \
                             CPPUNIT_TEST( tcGetConnectionStr ); \
                             CPPUNIT_TEST( tcSQLDialect ); \
                             CPPUNIT_TEST( tcOpen ); \
                             CPPUNIT_TEST( tcIsOpened ); \
                             CPPUNIT_TEST( tcGetType ); \
                             CPPUNIT_TEST( tcGetTransactor ); \
                             CPPUNIT_TEST( tcGetCapabilities ); \
                             CPPUNIT_TEST( tcClose ); \
                             CPPUNIT_TEST( tcOpen ); \
                             CPPUNIT_TEST( tcGetCatalogFull ); \
                             CPPUNIT_TEST( tcCreate ); \
                             CPPUNIT_TEST( tcDrop ); \
*/
#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_H

