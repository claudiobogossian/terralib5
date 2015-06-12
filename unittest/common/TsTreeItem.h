/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file TsTreeItem.h
 
  \brief Test suite for the TreeItem.
 */

#ifndef __TERRALIB_UNITTEST_COMMON_INTERNAL_TREEITEM_H
#define __TERRALIB_UNITTEST_COMMON_INTERNAL_TREEITEM_H

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

// TerraLib
#include <terralib/common/TreeItem.h>

/*!
  \class TsTreeItem

  \brief Test suite for the TreeItem.
 */
class TsTreeItem : public CPPUNIT_NS::TestFixture
{
// It registers this class as a Test Suit
  CPPUNIT_TEST_SUITE( TsTreeItem );

// It registers the class methods as Test Cases belonging to the suit 
  CPPUNIT_TEST( noParent );
  CPPUNIT_TEST( parentChildConnection );
  CPPUNIT_TEST( simblings );
  CPPUNIT_TEST( checkNumberOfChildren );
  CPPUNIT_TEST( getChildAndCheckItsIndex );
  //CPPUNIT_TEST( childDisconnects ); // disconnect deletes all the structure!
  CPPUNIT_TEST( iterateChildren );
  CPPUNIT_TEST( countItems );
  CPPUNIT_TEST( getChildByIndex );
  //CPPUNIT_TEST( addingItems ); // adding and inserting fails when asserting (TreeItem, lines 83 and 91)
  //CPPUNIT_TEST( removingItems ); // disconnect deletes all the structure!
  //CPPUNIT_TEST( removingSequence ); // disconnect deletes all the structure!
  CPPUNIT_TEST( childSwap );
  CPPUNIT_TEST( siblingSwap );
  //CPPUNIT_TEST(  );

  CPPUNIT_TEST_SUITE_END();
  
  public:

// It sets up context before running the test.
    void setUp();

// It cleann up after the test run.
    void tearDown();

  protected:

// Test Cases:
    void noParent();
    void parentChildConnection();
    void simblings();
    void checkNumberOfChildren();
    void getChildAndCheckItsIndex();
    void childDisconnects();
    void iterateChildren();
    void countItems();
    void getChildByIndex();
    void addingItems();
    void removingItems();
    void removingSequence();
    void childSwap();
    void siblingSwap();
};

#endif  // __TERRALIB_UNITTEST_COMMON_INTERNAL_TREEITEM_H