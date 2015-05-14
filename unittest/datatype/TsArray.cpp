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

//TerraLib

//...

// Unit-Test TerraLib
#include "TsArray.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TsArray );

void TsArray::setUp()
{
// An Array is defined to hold 5 points at the positions (1,1) to (4,4) and (5,1).
// After inserting the points, the dimensions size should be 6 in lines and 5 in columns.

  myarray = new te::dt::Array(2, te::dt::GEOMETRY_TYPE);
  mypoint1 = new te::gm::Point(10.0, 20.0);
  mypoint2 = new te::gm::Point(30.0, 20.0);
  mypoint3 = new te::gm::Point(40.0, 20.0);
  mypoint4 = new te::gm::Point(50.0, 20.0);
  mypoint5 = new te::gm::Point(80.0, 80.0);

  pos.push_back(1); // position (1, 1)
  pos.push_back(1);
  myarray->insert(mypoint1, pos);

  pos.clear();
  pos.push_back(2); // position (2, 2)
  pos.push_back(2);
  myarray->insert(mypoint2, pos);

  pos.clear();
  pos.push_back(3); // position (3, 3)
  pos.push_back(3);
  myarray->insert(mypoint3, pos);

  pos.clear();
  pos.push_back(4); // position (4, 4)
  pos.push_back(4);
  myarray->insert(mypoint4, pos);

  pos.clear();
  pos.push_back(5); // position (5, 1)
  pos.push_back(1);
  myarray->insert(mypoint5, pos);

  std::cout << std::endl;
  std::cout << "Printing array initialized = " << myarray->toString() << std::endl;
}

void TsArray::tearDown()
{  
  pos.clear();
  delete myarray;
}

void TsArray::tcArray()
{
//#ifdef TE_COMPILE_ALL

  te::dt::Array* myarray1 = new te::dt::Array(3, te::dt::GEOMETRY_TYPE);

  CPPUNIT_ASSERT(myarray1->getElementsTypeCode() == te::dt::GEOMETRY_TYPE); 
  CPPUNIT_ASSERT(myarray1->getDimension() == 3);
  CPPUNIT_ASSERT(myarray1->getTypeCode() == te::dt::ARRAY_TYPE);

  std::vector<std::size_t> pos;

  te::gm::Point* mypoint = new te::gm::Point(10.0, 20.0);

  te::gm::LinearRing* s = new te::gm::LinearRing(5, te::gm::LineStringType);
  s->setPoint(0, 10, 20); // lower left 
  s->setPoint(1, 10, 10); // upper left
  s->setPoint(2, 20, 10); // upper rigth
  s->setPoint(3, 20, 20); // lower rigth
  s->setPoint(4, 10, 20); // closing
  te::gm::Polygon* mypolygon = new te::gm::Polygon(0, te::gm::PolygonType);
  mypolygon->push_back(s);

  pos.push_back(1); // position (1, 1, 1)
  pos.push_back(1);
  pos.push_back(1);
  myarray1->insert(mypoint, pos);
  te::gm::Point* p1 = static_cast<te::gm::Point*>(myarray1->getData(pos));
  CPPUNIT_ASSERT(p1 = mypoint);

  pos.clear();
  pos.push_back(1); // position (1, 3, 2)
  pos.push_back(3);
  pos.push_back(2);
  myarray1->insert(mypolygon, pos);
  te::gm::Polygon* pol1 = static_cast<te::gm::Polygon*>(myarray1->getData(pos));
  CPPUNIT_ASSERT(pol1 = mypolygon);

  std::cout <<"Printing array with point and polygon " << myarray1->toString() << std::endl;

  delete myarray1; //it also deletes mypolygon and mypoint

//#endif
}

void TsArray::tcGetDimension()
{
//#ifdef TE_COMPILE_ALL
  te::dt::Array b(3,te::dt::INT16_TYPE);

  CPPUNIT_ASSERT(b.getDimension() == 3);

  CPPUNIT_ASSERT(b.getTypeCode() == te::dt::ARRAY_TYPE);

  CPPUNIT_ASSERT(b.getElementsTypeCode() == te::dt::INT16_TYPE);

  pos.push_back(1); // position (1, 1, 1)
  pos.push_back(1);
  pos.push_back(1);

  // Inserting INT32 elements in a 1-dimension array 
  te::dt::Array b1(1,te::dt:: INT32_TYPE);

  CPPUNIT_ASSERT(b.getDimension() == 1);

  CPPUNIT_ASSERT(b.getTypeCode() == te::dt::ARRAY_TYPE);

  CPPUNIT_ASSERT(b.getElementsTypeCode() == te::dt::INT32_TYPE);

  pos.push_back(1); // position (1)
  //int i = 25;
  //b1.insert(i, pos); //integer is incompatible with te::dt::AbstractData

  CPPUNIT_ASSERT_MESSAGE("It is allowed to declare 'te::dt::Array b(3,te::dt::INT32_TYPE)', but it does not allow insert elements of type te::dt::INT32_TYPE", 1 == 2);
//#endif
}

void TsArray::tcGetElementsTypeCode()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(myarray->getElementsTypeCode() == te::dt::GEOMETRY_TYPE); 
//#endif
}

void TsArray::tcGetDimensionSize()
{
//#ifdef TE_COMPILE_ALL
  std::cout << std::endl;
  std::cout << "Dimension 1 size= " << myarray->getDimensionSize(0) << " Dimension 2 size= " << myarray->getDimensionSize(1) << std::endl;
  CPPUNIT_ASSERT(myarray->getDimensionSize(0) == 6);
  CPPUNIT_ASSERT(myarray->getDimensionSize(1) == 5);
  //#endif
}

void TsArray::tcInsert()
{
//#ifdef TE_COMPILE_ALL
  CPPUNIT_ASSERT(myarray->getElementsTypeCode() == te::dt::GEOMETRY_TYPE); 
  CPPUNIT_ASSERT(myarray->getDimension() == 2);
  CPPUNIT_ASSERT(myarray->getTypeCode() == te::dt::ARRAY_TYPE);

  std::cout << std::endl;
  std::cout << "Dimension 1 size= " << myarray->toString() << std::endl;

  // inserting a new point at a new position (6,1)
  std::vector<std::size_t> npos;

  te::gm::Point* newpoint = new te::gm::Point(111.0, 111.0);
  npos.push_back(6); // position (6, 2)
  npos.push_back(2);
  myarray->insert(newpoint, npos);

  // getting the new point
  te::gm::Point* p5 = static_cast<te::gm::Point*>(myarray->getData(pos));
  CPPUNIT_ASSERT(p5 = newpoint);

  // checking dimension size after insert
  std::cout << std::endl;
  std::cout << "After insert: Dimension 1 size= " << myarray->getDimensionSize(0) << " Dimension 2 size= " << myarray->getDimensionSize(1) << std::endl;
  CPPUNIT_ASSERT(myarray->getDimensionSize(0) == 7);
  CPPUNIT_ASSERT(myarray->getDimensionSize(1) == 5);
//#endif
}

void TsArray::tcGetData()
{
//#ifdef TE_COMPILE_ALL

// getting first point 
  pos.clear();
  pos.push_back(1); // position (1, 1)
  pos.push_back(1);
  te::gm::Point* p1 = static_cast<te::gm::Point*>(myarray->getData(pos));
  CPPUNIT_ASSERT(p1 = mypoint1);
  CPPUNIT_ASSERT(p1->equals(mypoint1));

// getting last point 
  pos.clear();
  pos.push_back(5); // position (5, 1)
  pos.push_back(1);
  te::gm::Point* p5 = static_cast<te::gm::Point*>(myarray->getData(pos));
  CPPUNIT_ASSERT(p5 = mypoint5);
 
//#endif
}

void TsArray::tcOperatorBraket()
{
//#ifdef TE_COMPILE_ALL

// getting first point 
  pos.clear();
  pos.push_back(1); // position (1, 1)
  pos.push_back(1);
  
  te::gm::Point p1 = static_cast<te::gm::Point&>(myarray->operator[](pos));
  te::gm::Point p1_alternative = static_cast<te::gm::Point&>((*myarray)[pos]);
  
  CPPUNIT_ASSERT(p1.getX() == mypoint1->getX());
  CPPUNIT_ASSERT(p1.getY() == mypoint1->getY());
  CPPUNIT_ASSERT(p1.equals(mypoint1));

  CPPUNIT_ASSERT(p1_alternative.getX() == mypoint1->getX());
  CPPUNIT_ASSERT(p1_alternative.getY() == mypoint1->getY());
  CPPUNIT_ASSERT(p1_alternative.equals(mypoint1));
//#endif
}

void TsArray::tcClone()
{
//#ifdef TE_COMPILE_ALL 
  te::dt::AbstractData* myclone =  myarray->clone();
  std::cout << std::endl;
  std::cout << "Cloned Array= " << myclone->toString() << std::endl;
  CPPUNIT_ASSERT(strcmp((myclone->toString()).c_str(), (myarray->toString()).c_str()) == 0);
//#endif
}

void TsArray::tcGetTypeCode()
{
//#ifdef TE_COMPILE_ALL
    CPPUNIT_ASSERT(myarray->getTypeCode() == te::dt::ARRAY_TYPE);
//#endif
}

void TsArray::tcToString()
{
//#ifdef TE_COMPILE_ALL
   CPPUNIT_ASSERT_NO_THROW(myarray->toString());
//#endif
}