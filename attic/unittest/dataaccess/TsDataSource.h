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
  \file TsDataSource.h
 
  \brief A test suit for the DataSource interface.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_H

// TerraLib
#include <terralib/dataaccess_fw.h>
#include <terralib/geometry_fw.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>  //por algum motivo se nao colocar essa linha pega o antigo.

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
  CPPUNIT_TEST( tcGetId );

  CPPUNIT_TEST( tcClose );

  CPPUNIT_TEST( tcExist );

  CPPUNIT_TEST( tcSetConnectionInfo );
  CPPUNIT_TEST( tcOpenWithMap ); 
  CPPUNIT_TEST( tcGetType );
  CPPUNIT_TEST( tcIsOpened );
  CPPUNIT_TEST( tcIsValid );

  //getDataset methods...
  CPPUNIT_TEST( tcGetDataSetByName);
  CPPUNIT_TEST( tcGetDataSetByEnvRec );
  CPPUNIT_TEST( tcGetDataSetByEnvRec1 );
  CPPUNIT_TEST( tcGetDataSetByEnvRec2 );
  CPPUNIT_TEST( tcGetDataSetByGeometry );
  CPPUNIT_TEST( tcGetDataSetByProperty );
  CPPUNIT_TEST( tcGetDataSetByOids );

  CPPUNIT_TEST( tcGetCatalog );
  CPPUNIT_TEST( tcGetCatalogFull );
  CPPUNIT_TEST( tcGetCapabilities );
  CPPUNIT_TEST( tcSQLDialect );

  CPPUNIT_TEST( tcGetTransactor );
  CPPUNIT_TEST( tcGetConnectionInfo );
  //CPPUNIT_TEST( tcGetConnection );
  CPPUNIT_TEST( tcClose );

  CPPUNIT_TEST( tcSetConnectionInfo );
  CPPUNIT_TEST( tcCloseConnection );

  CPPUNIT_TEST( tcCreate );
  CPPUNIT_TEST( tcDrop );
  CPPUNIT_TEST( tcGetEncodings );
  //CPPUNIT_TEST( tcGetDataSources );

  CPPUNIT_TEST_SUITE_END();

  public:
// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:
    void tcOpenWithMap();//
    void tcOpenWithStr();//?
    void tcGetType();//
    void tcGetId();//
    void tcSetId();

    void tcClose();
    void tcIsOpened();
    void tcIsValid(); 
    void tcGetCatalog();//??
    void tcGetCatalogFull(); //??
    void tcGetCapabilities();
    void tcGetTransactor(); 
    void tcGetConnectionInfo();
    //void tcGetConnection();
    void tcSQLDialect();


    void tcSetConnectionInfo();
    void tcCloseConnection();
    void tcExist();
    void tcCreate();
    void tcDrop();
    void tcGetEncodings();
    ////void tcGetDataSources();

// Transaction Methods...ponly ion transactor will be tested
    //void tcIsIntransaction();
    //void tcBegin();
    //void tcCommit();
    //void tcEnd();
    //void tcRollBack();

// Methods  for retrieving data from the datasource (old DataSourceTranstator)

    void tcGetDataSetByName();
    void tcGetDataSetByEnvRec();
    void tcGetDataSetByEnvRec1();
    void tcGetDataSetByEnvRec2();
    void tcGetDataSetByGeometry();
    void tcGetDataSetByProperty();
    void tcGetDataSetByOids();

    void tcQueryByString(); 
    void tcQueryBySelect();

    void tcExecuteQuery();
    void tcExecuteString();
    void tcCancel();

// Methods for retrieving the metadata about datasets from the dataSource
    void tcGetDataSetNames();
    void tcGetNumberOfDataSets();

    void tcGetDataSetType();
    void tcGetProperties();
    void tcGetPropertyByName();//dataset name and property name
    void tcGetPropertyNames();
    void tcGetNumberOfProperties();
    void tcPropertyExists();
    void tcGetPropertyByPos(); //dataset name and property position

 
    void tcGetPrimaryKey(); 
    void tcPrimaryKeyExists();
    void tcGetForeingKey(); 
    void tcGetForeignKeyNames();
    void tcForeignKeyExists();

    void tcGetUniqueKey();
    void tcGetUniqueKeyNames();
    void tcUniqueKeyExists();

    void tcGetCheckConstraint();
    void tcGetCheckConstraintNames();
    void tcCheckConstraintExists();

    void tcGetIndex();
    void tcGetIndexNames();
    void tcIndexExists();

    void tcGetSequence();
    void tcGetSequenceNames();
    void tcSequenceExists();

    void tcGetExtentByPropName();
    void tcGetExtentByPropPos();

    void tcGetNumberOfItems();

// Auxiliary commands
    void tcEscape();
    void tcIsDataSetNameValid();
    void tcIsPropertyNameValid ();

// Changing schema and content 
    
    void tcAddProperty();
    void tcDropProperty();
    void tcRenameProperty();

    void tcAddPrimaryKey();
    void tcDropPrimaryKey();

    void tcAddForeignKey();
    void tcDropForeignKey();

    void tcAddUniqueKey();
    void tcDropUniqueKey();

    void tcAddCheckConstraint();
    void tcDropCheckConstraint();

    void tcAddIndex();
    void tcDropIndex();

    void tcAddSequence();
    void tcDropSequence();

// DataSource inquire (colocar junto com os metodos do inicio)

    void tcHasDataSets();
    void tcDataSetExists();

// Dataset Schema Persistence Methods
// Methods for dealing with datasource and dataset schema changes

    void tcCreateDataSet();
    void tcCloneDataSet();
    void tcDropDataSet();
    void tcRenameDataSet();

    void tcAddDataSet();
    void tcRemoveOids();
    void tcUpdateDataSet();
    //void tcOptimize();

// Methods to deal with repository 

    void tcCreateNewDS();
    void tcDropDS();
    void tcExistsDS();
    void tcGetDataSourceNames();
    void tcGetEncodingsOfDS();

  public:
// It will come from the setUp of the derived database class (see:TsPostGIS, TsSQLite, etc)
    te::da::DataSource* m_ds;
    //std::auto_ptr<te::da::DataSource> m_ds;
    te::da::DataSourceCapabilities  m_capabilit;
    std::map<std::string, std::string> m_connInfo;
    std::map<std::string, std::string> m_connInfoExist;

    std::string m_connStr;
    std::string m_dsType;
    std::map<std::string, std::string> m_connInfoNewDs;
    std::map<std::string, std::string> m_connInfoDropDs;
   
// Now I need these vars to use in methods of the datasource directly...(without  transactor) 
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

