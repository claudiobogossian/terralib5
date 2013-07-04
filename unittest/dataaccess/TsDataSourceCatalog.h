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
  \file TsDataSourceCatalog.h
 
  \brief Test suite for the DataSourceCatalog class.
 */

#ifndef __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASOURCECATALOG_H
#define __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASOURCECATALOG_H

// STL
#include <string>
#include <vector>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsDataSourceCatalog

  \brief Test suite for the DataSourceCatalog class.

  This test suite will perform checks in the DataSourceCatalog class
  without using any associated DataSource.
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
class TsDataSourceCatalog : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsDataSourceCatalog );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( tcCreateEmptyCatalog );
  CPPUNIT_TEST( tcOneDataSetType );
  CPPUNIT_TEST( tcDataSetTypes );
  //Not implemented yet!
  //CPPUNIT_TEST( tcOneSequence );
  //CPPUNIT_TEST( tcSequences );
  //CPPUNIT_TEST( tcDataSetTypesAndSequences );
  //CPPUNIT_TEST( tcDataSetTypesWithForeignKeys );
  //CPPUNIT_TEST( tcSequencesOwnedByDataSetTypes );

  CPPUNIT_TEST_SUITE_END();    
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:

    void tcGetId();
    void tcSetId();

    void tcGetDataSource();
    void tcSetDataSource();
    void tcClear();
    void tcGetNumberOfDataSets();
    void tcDataSetExists();
    void tcAddDataSetType(); // tcAddOneDataSetType();
    void tcRemove();
    void tcRename();
    void tcGetDataSetTypeByIdx();
    void tcGetDataSetTypeByName();

    void tcGetNumberOfSequences();
    void tcAddSequence();
    //void tcAddSequences();
    void tcRemoveSequence();
    //void tcChangesSequenceId();
    void tcDetachSequence();
    void tcGetSequenceByIdx();
    void tcGetSequenceByName();
    void tcGetSequenceById();
    //void tcGetSequencePos();

    void tcAddRefFk();
    void tcRemoveRefFk();
    void tcgetRefFk();
    void tcDropDependentSequencesOnProp();

    //Protected Methods
    void tcCheckSequenceDependency();
    void tcIndexSequenceDependency();
    void tcDropDependentSequencesOnDataSetType();
    void tcDropDependentSequenceEntry();
    void tcCheckFKsDependency();
    void tcIndexFKs();
    void tcDropDependentFKs();
    // End of proptected methods
    // ...


    /*!
      \brief Test Case: creating an empty catalog.
      
      This test case will try to create a new instance of a DataSourceCatalog and destroy it
      without adding any object to it.
     */
    void tcCreateEmptyCatalog();

    /*! \brief Test Case: adding and removing just one DataSetType (without foreign key). */
    void tcOneDataSetType();

    /*! \brief Test Case: adding and removing lots of DataSetTypes (without foreign key). */
    void tcDataSetTypes();

    /*! \brief Test Case: adding and removing just one Sequence (not owned). */
    void tcOneSequence();

    /*! \brief Test Case: adding and removing lots of Sequences (not owned). */
    void tcSequences();

    /*! \brief Test Case: adding lots of DataSetTypes (without foreign key) and Sequences (not owned). */
    void tcDataSetTypesAndSequences();

    /*! \brief Test Case: adding and removing dataset types with associated foreign keys. */
    void tcDataSetTypesWithForeignKeys();

    /*! \brief Test Case: adding sequences owned by DataSetTypes. */
    void tcSequencesOwnedByDataSetTypes();

    /*!
      \brief Test Case: performance monitor.

      This test case will add thousands of objects to the catalog.
      The main goal is to measure:
      <ul>
      <li>the performance of adding DataSetTypes not associated to foreign keys;</li>
      <li>the performance of adding DataSetTypes associated to foreign keys;</li>
      <li>the performance of adding Sequences not owned by dataset types;</li>
      <li>the performance of adding Sequences owned by dataset types;</li>
      <li>the performance of renaming DataSetTypes;</li>
      <li>the performance of renaming Sequences;</li>
      <li>the performance of changing DataSetTypes identification;</li>
      <li>the performance of changing Sequences identification;</li>
      <li>the performance of removing DataSetTypes not associated to foreign keys;</li>
      <li>the performance of removing DataSetTypes associated to foreign keys;</li>
      <li>the performance of removing Sequences not owned by dataset types;</li>
      <li>the performance of removing Sequences owned by dataset types;</li>
      </ul>
     */
    void tcPerformance();
};

#endif  // __TERRALIB_UNITTEST_DATAACCESS_INTERNAL_DATASOURCECATALOG_H

