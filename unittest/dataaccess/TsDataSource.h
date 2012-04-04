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
  protected:

// Test Cases:

    virtual void tcGetType();
    virtual void tcGetConnectionInfo();
    virtual void tcSetConnectionInfo();
    virtual void tcGetConnectionStr();
    virtual void tcSetConnectionStr();
    virtual void tcGetCapabilities();
    virtual void tcSQLDialect();
    virtual void tcOpen();
    virtual void tcOpenWithMap();
    virtual void tcOpenWithString();
    virtual void tcClose();
    virtual void tcIsOpened();
    virtual void tcIsValid();

    virtual void tcGetCatalog();
    virtual void tcGetCatalogFull();

    virtual void tcGetTransactor();
    virtual void tcCreate();
    virtual void tcDrop();

// It will come from the setUp of the derived database class (see:TsPostGIS, TsSQLite, etc)
    te::da::DataSource* m_ds;
    std::map<std::string, std::string> m_capabilit;
    std::map<std::string, std::string> m_connInfo;
    std::string m_connInfoStr;
    std::string m_dsType;
    std::map<std::string, std::string> m_connInfoNewDs;
    std::map<std::string, std::string> m_connInfoDropDs;
    te::da::DataSourceCatalog* m_dsCatalog;
    size_t m_nroDataSets;

};

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

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_H

