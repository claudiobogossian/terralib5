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
  \file TsDataSetTypePersistence.h
 
  \brief Test suite for the DataSetTypePersistence implemention.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASETTYPEPERSISTENCE_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASETTYPEPERSISTENCE_H

// TerraLib
#include <terralib/dataaccess_fw.h>
#include <terralib/geometry_fw.h>
//#include <terralib/dataset_fw.h>
// STL
#include <string>
#include <vector>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

namespace te { namespace da { class DataSource; } }

/*!
  \class TsDataSetTypePersistence

  \brief Test suite for the DataSetTypePersistence implemention.

  This test suite will perform checks in the DataSetTypePersistence.
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
class TsDataSetTypePersistence : public CPPUNIT_NS::TestFixture
{
  // It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsDataSetTypePersistence );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcCreateDataSetTypeBasedOnCapabilities ); 
  CPPUNIT_TEST( tcCreateDataSetType );
  CPPUNIT_TEST( tcAddPrimaryKey );
  CPPUNIT_TEST( tcAddUniqueKey );
  CPPUNIT_TEST( tcAddCheckConstraint );
  CPPUNIT_TEST( tcAddIndex );
  CPPUNIT_TEST( tcAddAutoIncrementProperty );
  CPPUNIT_TEST( tcAddArrayProperty );
  CPPUNIT_TEST( tcRenameProperty );
  CPPUNIT_TEST( tcUpdateProperty );
  CPPUNIT_TEST( tcCloneTable );
  CPPUNIT_TEST( tcRemoveIndex );
  CPPUNIT_TEST( tcCreateDataSetTypeWithPkUkIdxCc );
  CPPUNIT_TEST( tcAddForeignKey );
  CPPUNIT_TEST( tcRemovePrimaryKey );
  CPPUNIT_TEST( tcRemoveUniqueKey );
  CPPUNIT_TEST( tcRemoveProperty );
  CPPUNIT_TEST( tcRemoveCheckConstraint );
  CPPUNIT_TEST( tcRemoveDataSetType );
  CPPUNIT_TEST( tcRenameDataSetTypeCloned );
  CPPUNIT_TEST( tcRemoveDataSetTypeWithFk );

  CPPUNIT_TEST_SUITE_END();

  public:
// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    /*! \brief Test Case: create datasettype checking datasource capabilities. */
    void tcCreateDataSetTypeBasedOnCapabilities();

    /*! \brief Test Case: create several dataset types with/without constraints and indexes. */
    void tcCreateDataSetType();

    /*! \brief Test Case: adding a primary key. */
    void tcAddPrimaryKey();

    /*! \brief Test Case: adding a unique key. */
    void tcAddUniqueKey();

    /*! \brief Test Case: adding an index. */
    void tcAddIndex();

    /*! \brief Test Case: adding a new autoincrement property. */
    void tcAddAutoIncrementProperty();

    /*! \brief Test Case: adding an array property. */
    void tcAddArrayProperty();

    /*! \brief Test Case: Renaming an property. */
    void tcRenameProperty();

    /*! \brief Test Case: Updating an property. */
    void tcUpdateProperty();

    /*! \brief Test Case: Clonning a dataset type. */
    void tcCloneTable();

    /*! \brief Test Case: drop properties. */
    void tcRemoveProperty();

    /*! \brief Test Case: drop pk. */
    void tcRemovePrimaryKey();

    /*! \brief Test Case: drop uk. */
    void tcRemoveUniqueKey();

    /*! \brief Test Case: drop index. */
    void tcRemoveIndex();

    /*! \brief Test Case: drop dataset type. */
    void tcRemoveDataSetType();

    /*! \brief Test Case: Rename dataset type cloned. */
    void tcRenameDataSetTypeCloned();

    /*! \brief Test Case: drop dataset type cloned. */
    void tcRemoveDataSetTypeCloned();

    /*! \brief Test Case: create dataset type with pk,uk,idx,cc. */
    void tcCreateDataSetTypeWithPkUkIdxCc();

    void tcRemoveDataSetTypeWithFk();

    void tcAddCheckConstraint();

    void tcRemoveCheckConstraint(); 

    void tcAddForeignKey ();

// It will come from the setUp (values from json file for each driver)

    te::da::DataSource* m_ds;
    std::map<std::string, std::string> m_capabilit;
    te::da::DataSetType* m_newdt;
    te::da::DataSetType* m_newdt3;
    te::da::ForeignKey* m_fk;
    std::string m_dtcloned;
    std::map<std::string, std::string> m_connInfo;
    std::string m_dsType;
    std::map<std::string, std::string> m_connInfoNewDs;

    te::da::DataSourceCatalog* m_dsCatalog;

    std::string m_dsToString;
    size_t m_maxTransactors;

    size_t m_nroDataSets;

    std::vector<std::string> m_vecDtNames;
    std::vector<std::string> m_vecSeqNames;
    std::vector<te::gm::Envelope> m_vecEnvelops;
    std::vector<std::pair<std::string, te::gm::Envelope> > m_vecDtNamesAndEnvelops;
    std::vector<std::pair<std::string, te::gm::Envelope> > m_vecNamesAndRecs;
    std::vector<std::pair<std::string, size_t> > m_vecNamesSizes;
    std::vector<std::pair<std::string, size_t> > m_vecNamesSizesRec;

// Specific variables to use in DataSetTypePersistence tests
    std::string m_ccName;
    te::da::CheckConstraint* m_cc;

    std::string m_newDataSetType; //define always 8 attributes
    std::string m_NameAttr1;
    std::string m_Attr1;
    std::string m_NameAttr2; 
    std::string m_NameAttr3; 
    std::string m_NameAttr4; 
    std::string m_NameAttr5; 
    std::string m_NameAttr6; 
    std::string m_NameAttr7; 
    std::string m_NameAttr8;

// Properties to be created based on datasource capabities
    std::string m_attr_bit;
    std::string m_attr_char;  
    std::string m_attr_uchar;
    std::string m_attr_int16;
    std::string m_attr_uint16;
    std::string m_attr_int32;
    std::string m_attr_uint32;
    std::string m_attr_int64;
    std::string m_attr_uint64;

    std::string m_attr_bool;
    std::string m_attr_float;
    std::string m_attr_double ;
    std::string m_attr_numeric;
    std::string m_attr_string;
    std::string m_attr_byte_array;
    std::string m_attr_geometry;
    std::string m_attr_array;
    std::string m_attr_datetime;
    std::string m_attr_composite;
    std::string m_attr_raster;
// end of properties based on datasource capabities

    std::string m_Attr6Pk;
    std::string m_Attr6Fk;
    std::string m_Attr3e4Uk;
    std::string m_Attr5idx; 
    std::string m_Attr2idx; 
    std::string m_Attr7_seq;

    std::string m_newCapabilitiesDt;
    std::string m_newDataSetType2;
    std::string m_newDataSetType3; //define datasettype with pk

    std::string m_oldNameProp; 
    std::string m_newNameProp;

// End of Specification of  variables to use in DataSetTypePersistence tests

};

/*
// It registers the class methods as Test Cases in a macro 
#define INSERT_DATASETTYPEPERSISTENCE_TC CPPUNIT_TEST( tcCreateDataSetTypeBasedOnCapabilities ); \
                CPPUNIT_TEST( tcCreateDataSetType ); \
                CPPUNIT_TEST( tcAddPrimaryKey ); \
                CPPUNIT_TEST( tcAddUniqueKey ); \
                CPPUNIT_TEST( tcAddCheckConstraint ); \
                CPPUNIT_TEST( tcAddIndex ); \
                CPPUNIT_TEST( tcAddAutoIncrementProperty ); \
                CPPUNIT_TEST( tcAddArrayProperty ); \
                CPPUNIT_TEST( tcRenameProperty ); \
                CPPUNIT_TEST( tcUpdateProperty ); \
                CPPUNIT_TEST( tcCloneTable ); \
                CPPUNIT_TEST( tcRemoveIndex ); \
                CPPUNIT_TEST( tcCreateDataSetTypeWithPkUkIdxCc ); \
                CPPUNIT_TEST( tcAddForeignKey ); \
                CPPUNIT_TEST( tcRemovePrimaryKey ); \
                CPPUNIT_TEST( tcRemoveUniqueKey ); \
                CPPUNIT_TEST( tcRemoveProperty ); \
                CPPUNIT_TEST( tcRemoveCheckConstraint ); \
                CPPUNIT_TEST( tcRemoveDataSetType ); \
                CPPUNIT_TEST( tcRenameDataSetTypeCloned ); \
                CPPUNIT_TEST( tcRemoveDataSetTypeCloned ); \
                CPPUNIT_TEST( tcRemoveDataSetTypeWithFk ); \
*/
#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASETTYPEPERSISTENCE_H

