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
// STL
#include <string>
//#include <vector>

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
  protected:

// Test Cases for DataSourceCatalogLoader:

    // Protected Methods 
    // ...

    /*!
      \brief Test Case: Testing DataSourceCatalogLoader of an given datasource.
    */

    virtual void tcLoadCatalog();
    virtual void tcLoadCatalogFull();
    virtual void tcGetDataSets();
    virtual void tcGetDataSetType();
    virtual void tcAllGets();
    virtual void tcGetDataSetTypeInvalid();
    virtual void tcDataSetExists();
    virtual void tcGetSequences();

    virtual void tcGetExtent();

    /*!
      \brief Test Case: get the envelop of all dataSets defined in the m_vecDtNamesAndEnvelops.
  
      This test case will get the envelop (rectangle) of all dataSets defined in m_vecDtNamesAndEnvelops.
      The vector of pairs contains the dataSet names and its envelops. The envelop returned is compared 
      with the given envelop.
     */
    virtual void tcGetExtentAll();

    virtual void tcGetTransactor();
    virtual void tcPkExists();
    virtual void tcUkExists();
    virtual void tcCcExists();
    virtual void tcIdxExists();
    virtual void tcFkExists();
    virtual void tcGetFks();

// It will come from the setUp of the derived database class (see:TsPostGISCatalogLoader, TsSQLiteCatalogLoader, etc)
    te::da::DataSource* m_ds;
    std::map<std::string, std::string> m_connInfo;
    std::map<std::string, std::string> m_capabilit;
    std::string m_dsType;
    std::vector<std::string> m_vecDtNames;
    std::vector<std::string> m_vecPkNames;    
    std::vector<std::string> m_vecUkNames;
    std::vector<std::string> m_vecIdxNames;
    std::vector<std::string> m_vecFkNames;
    std::vector<std::string> m_vecCcNames;
    std::vector<std::string> m_vecSeqNames;
    std::vector<std::pair<std::string, te::gm::Envelope> > m_vecDtNamesAndEnvelops;

    size_t m_nroDataSets;
    std::string m_newDataSetType;
};

#define INSERT_DATASOURCECATALOGLOADER_TC \
                             CPPUNIT_TEST( tcGetDataSetTypeInvalid ); \
                             CPPUNIT_TEST( tcLoadCatalog  ); \
                             CPPUNIT_TEST( tcGetDataSets  ); \
                             CPPUNIT_TEST( tcGetDataSetType  ); \
                             CPPUNIT_TEST( tcDataSetExists  ); \
                             CPPUNIT_TEST( tcGetSequences  ); \
                             CPPUNIT_TEST( tcGetTransactor  ); \
                             CPPUNIT_TEST( tcGetExtent ); \
                             CPPUNIT_TEST( tcGetExtentAll ); \
                             CPPUNIT_TEST( tcPkExists ); \
                             CPPUNIT_TEST( tcUkExists ); \
                             CPPUNIT_TEST( tcCcExists ); \
                             CPPUNIT_TEST( tcIdxExists ); \
                             CPPUNIT_TEST( tcFkExists ); \
                             CPPUNIT_TEST( tcAllGets ); \
                             CPPUNIT_TEST( tcGetFks ); \
                             CPPUNIT_TEST( tcLoadCatalogFull ); \

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASOURCECATALOGLOADER_H