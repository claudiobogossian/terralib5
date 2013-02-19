/*  Copyright (C) 2001-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file TsTreeItem.cpp
 
  \brief Test suite for the TreeItem.
 */

// Unit-Test TerraLib
#include "TsTreeItem.h"

// TerraLib
#include <terralib/common.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TsTreeItem );

void TsTreeItem::setUp()
{
}

void TsTreeItem::tearDown()
{
}

void TsTreeItem::noParent()
{
  te::common::TreeItem parent;
  te::common::TreeItem* parentParent = parent.getParent();
  CPPUNIT_ASSERT(parentParent == 0);
}

void TsTreeItem::parentChildConnection()
{
  te::common::TreeItem* parent = new te::common::TreeItem();
  te::common::TreeItem child(parent);
  te::common::TreeItem* p = child.getParent();
  CPPUNIT_ASSERT(parent == p);
}

void TsTreeItem::simblings()
{
  te::common::TreeItem* parent = new te::common::TreeItem();
  te::common::TreeItem* child1 = new te::common::TreeItem(parent);
  te::common::TreeItem* child2 = new te::common::TreeItem(parent);
  CPPUNIT_ASSERT( child1->isSibling(child2) == true);
  CPPUNIT_ASSERT( child2->isSibling(child1) == true);

  delete parent;
}

void TsTreeItem::checkNumberOfChildren()
{
  te::common::TreeItem* parent = new te::common::TreeItem();
  CPPUNIT_ASSERT( parent->getChildrenCount() == 0);
  te::common::TreeItem* child1 = new te::common::TreeItem(parent);
  te::common::TreeItem* child2 = new te::common::TreeItem(parent);
  te::common::TreeItem* child3 = new te::common::TreeItem(parent);
  te::common::TreeItem* child4 = new te::common::TreeItem(parent);
  CPPUNIT_ASSERT( parent->getChildrenCount() == 4);
  te::common::TreeItem* child5 = new te::common::TreeItem(parent);
  CPPUNIT_ASSERT( parent->getChildrenCount() == 5);

  delete parent;
}

void TsTreeItem::getChildAndCheckItsIndex()
{
  te::common::TreeItem* parent = new te::common::TreeItem();
  te::common::TreeItem* child1 = new te::common::TreeItem(parent);
  te::common::TreeItem* child2 = new te::common::TreeItem(parent);

  te::common::TreeItemPtr child1Ref = parent->getChild(0);
  te::common::TreeItemPtr child2Ref = parent->getChild(1);

  CPPUNIT_ASSERT( child1Ref->getIndex() == child1->getIndex());
  CPPUNIT_ASSERT( child2Ref->getIndex() == child2->getIndex());

  delete parent;
}

void TsTreeItem::childDisconnects()
{
  te::common::TreeItem* parent = new te::common::TreeItem();
  CPPUNIT_ASSERT( parent->getChildrenCount() == 0);

  te::common::TreeItem* child1 = new te::common::TreeItem(parent);
  te::common::TreeItem* child2 = new te::common::TreeItem(parent);

  CPPUNIT_ASSERT( child1->getChildrenCount() == 0);
  CPPUNIT_ASSERT( parent->getChildrenCount() == 2);

  //child1->disconnect(); // isso estranhamente apaga o parent
  //CPPUNIT_ASSERT( parent->getChildrenCount() == 1);  // <---- quebra, pois parent nao existe mais
  //delete parent;
}

void TsTreeItem::iterateChildren()
{
  te::common::TreeItem* parent = new te::common::TreeItem();
  te::common::TreeItem* child0 = new te::common::TreeItem(parent);
  te::common::TreeItem* child1 = new te::common::TreeItem(parent);
  te::common::TreeItem* child2 = new te::common::TreeItem(parent);
  te::common::TreeItem* child3 = new te::common::TreeItem(parent);

  int indexes[4] = { child0->getIndex(), child1->getIndex(), child2->getIndex(), child3->getIndex() }; 
  
  int idx = 0;
  for(te::common::TreeItem::const_iterator it = parent->begin(); it != parent->end(); ++it)
  {
    te::common::TreeItemPtr item = *it;
    CPPUNIT_ASSERT(item->getIndex() == indexes[idx]);
    idx++;
  }
  delete parent;
}

void TsTreeItem::countItems()
{
  te::common::TreeItem* parent = new te::common::TreeItem();
  
  // parent has 3 children
  te::common::TreeItem* childA = new te::common::TreeItem(parent);
  te::common::TreeItem* childB = new te::common::TreeItem(parent);
  te::common::TreeItem* childC = new te::common::TreeItem(parent);

  // childA has 2 children
  te::common::TreeItem* childA1 = new te::common::TreeItem(childA);
  te::common::TreeItem* childA2 = new te::common::TreeItem(childA);

  // childC has 1 child
  te::common::TreeItem* childC1 = new te::common::TreeItem(childC);

  // childC1 has 5 children
  te::common::TreeItem* childC1A = new te::common::TreeItem(childC1);
  te::common::TreeItem* childC1B = new te::common::TreeItem(childC1);
  te::common::TreeItem* childC1C = new te::common::TreeItem(childC1);
  te::common::TreeItem* childC1D = new te::common::TreeItem(childC1);
  te::common::TreeItem* childC1E = new te::common::TreeItem(childC1);

  std::size_t count = 0;
  parent->getDescendantsCount(count);

  // total = 3 + 2 + 1 + 5 = 11 descendants
  CPPUNIT_ASSERT(count == 11);

  delete parent;
}


//void add(const TreeItemPtr& childItem);
//const TreeItemPtr& operator[](std::size_t i) const;
//void insert(std::size_t i, const TreeItemPtr& childItem);
//TreeItemPtr remove(std::size_t i);
//std::list<TreeItemPtr> remove(std::size_t i, std::size_t count);
//TreeItemPtr replace(std::size_t i, const TreeItemPtr& childItem);
//void swap(const TreeItemPtr& firstChild, const TreeItemPtr& secondChild);
//void swap(const TreeItemPtr& sibling);