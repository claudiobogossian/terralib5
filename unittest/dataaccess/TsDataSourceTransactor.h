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
  \file TsDataSourceTransactor.h
 
  \brief A test suit for the DataSourceTransactor interface.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASOURCETRANSACTOR_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASOURCETRANSACTOR_H

// TerraLib
#include <terralib/dataaccess_fw.h>
#include <terralib/geometry_fw.h>

// STL
#include <string>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsDataSourceTransactor

  \brief A test suit for the DataSourceTransactor interface.

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
class TsDataSourceTransactor : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsDataSourceTransactor );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcGetDataSource ); 
  CPPUNIT_TEST( tcGetDataSet );
  CPPUNIT_TEST( tcGetDataSetByGeometry );
  CPPUNIT_TEST( tcGetDataSetByProperty );
  CPPUNIT_TEST( tcGetDataSetByEnvRec );
  CPPUNIT_TEST( tcGetDataSetByEnvRec1 );
  CPPUNIT_TEST( tcGetDataSetByEnvRec2 );
  CPPUNIT_TEST( tcQueryByString );

  CPPUNIT_TEST_SUITE_END();

  public:
// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases for DataSourceTransactor:

    // Protected Methods 
    // ...

    /*!
      \brief Test Case: Testing DataSourceTransactor of an given datasource.
    */
    void tcBegin();
    void tcComit();
    void tcRollback();
    void tcExecuteQuery();
    void tcExecuteCommand();
    void tcGetDataSource();

    /*!
      \brief Test Case: get the dataSet using the i-th position in the DataSourceCatalog.
  
      This test case will get all the dataSets using the i-th position in the DataSourceCatalog.
     */    
    void tcGetDataSet(); //10 API

    /*!
      \brief Test Case: get only the objects that intercepts a given rectangle.
  
      This test case will get all the objects that intercepts a given rectangle defined m_vecNamesAndRecs.
      The vector of pairs contain the dataSets names and the rectangles to be used as a spatial filter when retrieving datasets.
     */
    void tcGetDataSetByEnvRec();
    void tcGetDataSetByEnvRec1();

    /*!
      \brief Test Case: get only the objects that intercepts a given rectangle.
  
      This test case will get all the objects that intercepts a given rectangle defined m_vecNamesAndRecs.
      The vector of pairs contain the dataSets names and the rectangles to be used as a spatial filter when retrieving datasets.
      The number of objects returned is compared with the size in the vector of pairs m_vecNamesSizesRec.
     */
    void tcGetDataSetByEnvRec2();

    /*!
    \brief Test Case: get only the objects that intercepts a given geometry (point, box, pol, geom).

    This test case will get all the objects that intercepts a given geometry (point, box, pol, geom).
   */
    void tcGetDataSetByGeometry();

    /*!
    \brief Test Case: It gets the DataSet identified by the given name using a spatial filter over the given geometric property.

    This test case will gets the DataSet identified by the given name using a spatial filter over the given geometric property.
   */
    void tcGetDataSetByProperty();

   /*!
    \brief Test Case: It gets a dataset identified by the given name using the set of objects identification.

    This test case will the gets the dataset identified by the given name using the set of objects identification.
   */
    void tcGetDataSetByOids();

    void tcQueryByString(); 
    void tcQueryBySelect();


    // New ones...
    void tcGetPreparedStmt();
    void tcGetBatchExecutor();
    void tcGetCatalogLoader();
    void tcCancel();

    //It will come from the setUp of the derived database class (see:TsPostGIS or TsSQLite or ...)
    te::da::DataSource* m_ds;
    te::da::DataSourceCapabilities  m_capabilit;
    std::map<std::string, std::string> m_connInfo;
    std::string m_dsType;
    std::map<std::string, std::string> m_connInfoNewDs;

    te::da::DataSourceCatalog* m_dsCatalog;

    std::string m_dsToString;

    size_t m_nroDataSets;

    std::vector<std::string> m_vecDtNames;
    std::vector<te::gm::Envelope> m_vecEnvelops;
    std::vector<std::pair<std::string, te::gm::Envelope> > m_vecDtNamesAndEnvelops;
    std::vector<std::pair<std::string, te::gm::Envelope> > m_vecNamesAndRecs;
    std::vector<std::pair<std::string, size_t> > m_vecNamesSizes;
    std::vector<std::pair<std::string, size_t> > m_vecNamesSizesRec;

    te::gm::Envelope m_box;
    te::gm::Point   m_pt;
    te::gm::LinearRing* m_linearRing;
    te::gm::Polygon* m_pol;
    te::gm::Geometry* m_geom;

};
/*
// Calling order of DATASOURCETRANSACTOR Test Cases                                
                          
#define INSERT_DATASOURCETRANSACTOR_TC CPPUNIT_TEST( tcGetDataSource ); \
                               CPPUNIT_TEST( tcGetDataSet ); \
                               CPPUNIT_TEST( tcGetDataSetByGeometry ); \
                               CPPUNIT_TEST( tcGetDataSetByProperty ); \
                               CPPUNIT_TEST( tcGetDataSetByEnvRec ); \
                               CPPUNIT_TEST( tcGetDataSetByEnvRec1 ); \
                               CPPUNIT_TEST( tcGetDataSetByEnvRec2 ); \
                               CPPUNIT_TEST( tcQueryByString ); \
                               CPPUNIT_TEST( tcGetCatalogLoader ); \
                               CPPUNIT_TEST( tcGetDataSetTypePersistence ); \
                               CPPUNIT_TEST( tcGetDataSetPersistence ); \
*/
#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASOURCETRANSACTOR_H
