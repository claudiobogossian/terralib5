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
  \file TsDataSourceCatalogLoader.h
 
  \brief Test suite for the DataSourceCatalogLoader and DataSourceCatalog classes
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASOURCECATALOGLOADER_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASOURCECATALOGLOADER_H

// TerraLib
#include <terralib/dataaccess_fw.h>
#include <terralib/geometry.h>
#include <terralib/dataaccess/datasource_h.h>

// STL
#include <string>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsDataSourceCatalogLoader

  \brief Test suite for the DataSourceCatalogLoader class.

  This test suite will perform checks in the DataSourceCatalogLoader Classes
  using an associated DataSource.
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

class TsDataSourceCatalogLoader : public CPPUNIT_NS::TestFixture
{
  // It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsDataSourceCatalogLoader );

// It registers the class methods as Test Cases belonging to the suit 

  CPPUNIT_TEST( tcHasDataSets );
  CPPUNIT_TEST( tcGetDataSets );
  CPPUNIT_TEST( tcGetDataSetType );
  CPPUNIT_TEST( tcGetDataSetTypeTrue );
  CPPUNIT_TEST( tcIdxExists );
  CPPUNIT_TEST( tcGetProperties );
  //CPPUNIT_TEST( tcGetProperty ); //Not implemented yet by Postgis
  CPPUNIT_TEST( tcDataSetExists );
  CPPUNIT_TEST( tcGetDataSetTypeInvalid );
  CPPUNIT_TEST( tcGetPrimaryKey );
  CPPUNIT_TEST( tcGetUniqueKeys );
  CPPUNIT_TEST( tcGetIndexes );
  CPPUNIT_TEST( tcGetCheckConstraints );

  CPPUNIT_TEST( tcLoadCatalog );
  CPPUNIT_TEST( tcGetExtent );
  CPPUNIT_TEST( tcGetExtentAll );
  CPPUNIT_TEST( tcGetSequences  );
  CPPUNIT_TEST( tcGetTransactor  );
  CPPUNIT_TEST( tcPkExists );
  CPPUNIT_TEST( tcUkExists );
  CPPUNIT_TEST( tcCcExists );
  CPPUNIT_TEST( tcIdxExists );
  CPPUNIT_TEST( tcFkExists );
  CPPUNIT_TEST( tcAllGets );
  CPPUNIT_TEST( tcGetFks );
  CPPUNIT_TEST( tcLoadCatalogFull );

  CPPUNIT_TEST_SUITE_END();

  public:
// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases for DataSourceCatalogLoader:

    // Protected Methods 
    // ...

    /*!
      \brief Test Case: Testing DataSourceCatalogLoader of an given datasource.
    */
    void tcLoadCatalog();
    void tcLoadCatalogFull();
    void tcGetDataSets();
    void tcGetDataSetType();
    void tcGetDataSetTypeTrue();
    void tcGetProperties();
    void tcGetProperty();
    void tcGetPrimaryKey();
    void tcGetUniqueKeys();
    void tcGetForeignKeys();
    void tcGetForeignKey();
    void tcGetIndexes();
    void tcGetCheckConstraints();
    void tcGetSequences();
    void tcGetSequence();
    void tcGetExtent();
    void tcHasDataSets();
    void tcDataSetExists();

    void tcAllGets();
    void tcGetDataSetTypeInvalid();

    /*!
      \brief Test Case: get the envelop of all dataSets defined in the m_vecDtNamesAndEnvelops.
  
      This test case will get the envelop (rectangle) of all dataSets defined in m_vecDtNamesAndEnvelops.
      The vector of pairs contains the dataSet names and its envelops. The envelop returned is compared 
      with the given envelop.
     */
    void tcGetExtentAll();

    void tcPkExists();
    void tcUkExists();
    void tcCcExists();
    void tcIdxExists();
    void tcFkExists();
    void tcGetFks();
    void tcIndexExists();
    void tcSequenceExists();
    void tcGetTransactor();

  public:
// It will come from the setUp of the derived database class (see:TsPostGISCatalogLoader, TsSQLiteCatalogLoader, etc)
    te::da::DataSource* m_ds;
    std::map<std::string, std::string> m_connInfo;
    te::da::DataSourceCapabilities  m_capabilit;
    std::string m_dsType;
    std::vector<std::string> m_vecDtNames;
    std::vector<std::string> m_vecPkNames;    
    std::vector<std::string> m_vecUkNames;
    std::vector<std::string> m_vecIdxNames;
    std::vector<std::string> m_vecFkNames;
    std::vector<std::string> m_vecCcNames;
    std::vector<std::string> m_vecSeqNames;
    std::vector<std::pair<std::string, te::gm::Envelope> > m_vecDtNamesAndEnvelops;
};
#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASOURCECATALOGLOADER_H