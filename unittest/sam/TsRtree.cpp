/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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

// Unit-Test TerraLib
#include "TsRtree.h"

// TerraLib
//#include <terralib/common.h>
//#include <terralib/geometry.h>

// STL
//#include <cstdio>
//#include <cstdlib>
//#include <cstring>

int b;

CPPUNIT_TEST_SUITE_REGISTRATION( TsRTree );

void TsRTree::setUp()
{
// Box to be inserted in the RTree
  r15 = new te::gm::Envelope(1.0,1.0, 2.5,2.5);
  r16 = new te::gm::Envelope(3.2,1.0, 7.5,4.0);
  r17 = new te::gm::Envelope(12.5,3.5, 17.5,5.0);
  r18 = new te::gm::Envelope(14.0,1.7, 15,5.9);
  r19 = new te::gm::Envelope(14.5,2.5, 16.5,3.5);
  r12 = new te::gm::Envelope(5.0,4.5, 8.5,6.5);
  r08 = new te::gm::Envelope(3.3,6.5, 4.5,7.5);
  r09 = new te::gm::Envelope(5.2,8.5, 6.3,9.5);
  r10 = new te::gm::Envelope(5.2,7.2, 6.3,8.2);
  r11 = new te::gm::Envelope(8,1.8, 9,10.5);
  r13 = new te::gm::Envelope(11,4.3, 11.9,9);
  r14 = new te::gm::Envelope(10.2,11.5, 6.3,8.0);

//Researching boxes
  r1 = new te::gm::Envelope(3.0,1.5, 9.5,10.6);
  r2 = new te::gm::Envelope(0.5,0.5, 18.0,6.0);
  r3 = new te::gm::Envelope(2.7,6.7, 6.5,9.9);
  r4 = new te::gm::Envelope(4.6,1.7, 9.3,10.8);
  r5 = new te::gm::Envelope(9.5,4.5, 12,9.5);
  r6 = new te::gm::Envelope(0.7,0.7,7.7, 4.3);
  r7 = new te::gm::Envelope(12.5,1.5,18, 6.0);

}

void TsRTree::tearDown()
{
}

void TsRTree::tcRTreeUnsignedInts()
{
//#ifdef TE_COMPILE_ALL
  std::vector<te::gm::Point*> pointVec;  
  te::sam::rtree::Index<size_t, 4> rtree;

  pointVec.reserve(100);

  for(int i = 0; i < 100; ++i)
  {
    te::gm::Point* pt = new te::gm::Point(static_cast<double>(i), static_cast<double>(i), 4326);
    pointVec.push_back(pt);
    rtree.insert(*(pt->getMBR()), i);
  }
  size_t size = rtree.size();

  for(int i = 0; i < 100; ++i)
  {
    std::vector<size_t> report;
    const te::gm::Envelope* mbr = pointVec[i]->getMBR();

    assert(rtree.search(*mbr, report) == 1);
    assert(report[0] == static_cast<size_t>(i));
  }


  te::common::FreeContents(pointVec);
//#endif
}

void TsRTree::tcRTreeInts()
{
//#ifdef TE_COMPILE_ALL
  std::vector<te::gm::Point*> pointVec;  
  te::sam::rtree::Index<int, 4> rtree;

  pointVec.reserve(100);

  for(int i = 0; i < 100; ++i)
  {
    te::gm::Point* pt = new te::gm::Point(i, i, 4326);
    pointVec.push_back(pt);
    rtree.insert(*(pt->getMBR()), i);
  }
  size_t size = rtree.size();

  for(int i = 0; i < 100; ++i)
  {
    std::vector<int> report;
    const te::gm::Envelope* mbr = pointVec[i]->getMBR();

    CPPUNIT_ASSERT(rtree.search(*mbr, report) == 1);
    CPPUNIT_ASSERT(report[0] == i);
  }
  const te::gm::Envelope* mbr1 = new te::gm::Envelope(0.0,0.0,3.5,3.5);
  std::vector<int> report1;
  size_t ss;
  CPPUNIT_ASSERT(rtree.search(*mbr1, report1) == 4);
  ss = report1.size();
  report1.clear();
  const te::gm::Envelope* mbr2 = new te::gm::Envelope(1.5,1.5,7.5,7.5);
  CPPUNIT_ASSERT(rtree.search(*mbr2, report1) == 6);

  for (std::size_t j = 0; j < report1.size(); j++)
  {
    std::cout << std::endl << "Print report RTree points["<< j <<"]: " << report1[j] << std::endl;
  }

  te::common::FreeContents(pointVec);
  delete mbr1;
  delete mbr2;

//#endif
}

void TsRTree::tcRTreePointers()
{
  std::vector<te::gm::Point*> pointVec;  
  te::sam::rtree::Index<int*, 4> rtree;

  pointVec.reserve(100);
  int* id = 0; 

  for(int i = 0; i < 100; ++i)
  {
    te::gm::Point* pt = new te::gm::Point(static_cast<double>(i), static_cast<double>(i), 4326);
    pointVec.push_back(pt);
    //rtree.insert(*(pt->getMBR()), &i);
    rtree.insert(*(pt->getMBR()), id++);

  }
  size_t size = rtree.size();

  for(int i = 0; i < 100; ++i)
  {
    std::vector<int*> report;
    const te::gm::Envelope* mbr = pointVec[i]->getMBR();

    assert(rtree.search(*mbr, report) == 1);
    std::cout << std::endl << "Print report RTree Pointers: " << report[0] << std::endl;
  }

  const te::gm::Envelope* mbr1 = new te::gm::Envelope(0.0,0.0,3.5,3.5);
  std::vector<int*> report1;
  CPPUNIT_ASSERT(rtree.search(*mbr1, report1) == 4);
  for (std::size_t j = 0; j < report1.size(); j++)
  {
    std::cout << std::endl << "Print report["<< j <<"]: " << report1[j] << std::endl;
  }
  te::common::FreeContents(pointVec);

}

void TsRTree::tcRTreeGradeInts()
{
//#ifdef TE_COMPILE_ALL
  std::vector<te::gm::Point*> pointVec;  
  te::sam::rtree::Index<int, 4> rtree;

  pointVec.reserve(100);
  std::size_t j; 
  int k = 0; //id 
  for(int i = 0; i < 10; ++i)
  {
    for (j = 0; j < 10; j++)
    {
      te::gm::Point* pt = new te::gm::Point(i, j, 4326);
      pointVec.push_back(pt);
      rtree.insert(*(pt->getMBR()), k++);
    }
  }

  size_t size = rtree.size();

  for(int i = 0; i < 100; ++i)
  {
    std::vector<int> report;
    const te::gm::Envelope* mbr = pointVec[i]->getMBR();

    CPPUNIT_ASSERT(rtree.search(*mbr, report) == 1);
    CPPUNIT_ASSERT(report[0] == i);
  }
  const te::gm::Envelope* mbr1 = new te::gm::Envelope(0.0,0.0,3.5,3.5);
  std::vector<int> report1;
  size_t ss;
  size_t searchResult =rtree.search(*mbr1, report1);
  CPPUNIT_ASSERT(rtree.search(*mbr1, report1) == 16);
  ss = report1.size();
  report1.clear();
  const te::gm::Envelope* mbr2 = new te::gm::Envelope(1.5,1.5,7.5,7.5);
  CPPUNIT_ASSERT(rtree.search(*mbr2, report1) == 36);

  for (j = 0; j < report1.size(); j++)
  {
    std::cout << std::endl << "Print report["<< j <<"]: " << report1[j] << std::endl;
  }

  te::common::FreeContents(pointVec);
  delete mbr1;
  delete mbr2;

//#endif
}

void TsRTree::tcRTreeBox()
{
//#ifdef TE_COMPILE_ALL

  std::vector<te::gm::Envelope*> envelopeVec;  
  te::sam::rtree::Index<int, 4> rtree;

  envelopeVec.reserve(121);
  std::size_t j; 
  int k = 0; //id 
  for(int i = 0; i < 11; ++i)
  {
    for (j = 0; j < 11; j++)
    {
      te::gm::Envelope* box = new te::gm::Envelope(i, j, i+1,j+1);
      envelopeVec.push_back(box);
      rtree.insert(*(box), k++);
    }
  }
  size_t env_size = envelopeVec.size();
  size_t size = rtree.size();

//Inserting another box
  te::gm::Envelope* boxInsert = new te::gm::Envelope(12, 2, 13,5);
  envelopeVec.push_back(boxInsert);
  rtree.insert(*(boxInsert), k++);
  env_size = envelopeVec.size();
  size = rtree.size();

  for(std::size_t i = 0; i < env_size; ++i)
  {
    std::vector<int> report;
    const te::gm::Envelope* mbr = envelopeVec[i];
    size_t search_result = rtree.search(*mbr, report);

    std::cout << std::endl << "Print search_result [" << i << "] = " << search_result << std::endl;

    for (j = 0; j < report.size(); j++)
    {
      std::cout << std::endl << "    Print report["<< j << "]: " << report[j] << std::endl;
    }
  }
  const te::gm::Envelope* mbr1 = new te::gm::Envelope(0.0,0.0,3.5,3.5);
  std::vector<int> report1;
  size_t ss;
  size_t searchResult = rtree.search(*mbr1, report1);
  CPPUNIT_ASSERT( searchResult == 16);
  ss = report1.size();
  report1.clear();
  const te::gm::Envelope* mbr2 = new te::gm::Envelope(1.5,1.5,7.5,7.5);
  searchResult = rtree.search(*mbr2, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 49);
    
  std::cout << std::endl << "Print report mbr2(x1,y1,x2,y2) "<< mbr2->m_llx << " " << mbr2->m_lly << " " <<mbr2->m_urx << " " << mbr2->m_ury << std::endl;
  std::cout << std::endl << "    Search result = "<< searchResult << std::endl;
  for (j = 0; j < report1.size(); j++)
  {
    std::cout << std::endl << "    Print report["<< j <<"]: " << report1[j] << std::endl;
  }
  report1.clear();

// Testing search to catch the last box
  const te::gm::Envelope* mbr3 = new te::gm::Envelope(8.5,3.5,12.5,4.5);
  searchResult = rtree.search(*mbr3, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 7);
    
  std::cout << std::endl << "Print report mbr3(x1,y1,x2,y2) "<< mbr3->m_llx << " " << mbr3->m_lly << " " <<mbr3->m_urx << " " << mbr3->m_ury << std::endl;
  std::cout << std::endl << "    Search result = "<< searchResult << std::endl;
  for (j = 0; j < report1.size(); j++)
  {
    std::cout << std::endl << "    Print report["<< j <<"]: " << report1[j] << std::endl;
  }

  te::common::FreeContents(envelopeVec);
  delete mbr1;
  delete mbr2;
  delete mbr3;

//#endif
}

void TsRTree::tcRTreeBox_4()
{
  //#ifdef TE_COMPILE_ALL

  te::sam::rtree::Index<int, 4> rtree;

  std::vector<te::gm::Envelope*> envelopeVec;
  envelopeVec.reserve(12);

  std::size_t j; 
  int k = 0; //id 
  {
    envelopeVec.push_back(r15);
    rtree.insert(*(r15), k++);

    envelopeVec.push_back(r16);
    rtree.insert(*(r16), k++);

    envelopeVec.push_back(r17);
    rtree.insert(*(r17), k++);

    envelopeVec.push_back(r18 );
    rtree.insert(*(r18), k++);

    envelopeVec.push_back(r19 );
    rtree.insert(*(r19), k++);

    envelopeVec.push_back(r12 );
    rtree.insert(*(r12), k++);

    envelopeVec.push_back(r08 );
    rtree.insert(*(r08), k++);

    envelopeVec.push_back(r09 );
    rtree.insert(*(r09), k++);

    envelopeVec.push_back(r10 );
    rtree.insert(*(r10), k++);

    envelopeVec.push_back(r11 );
    rtree.insert(*(r11), k++);

    envelopeVec.push_back(r13 );
    rtree.insert(*(r13), k++);

    envelopeVec.push_back(r14 );
    rtree.insert(*(r14), k++);
  }
  size_t env_size = envelopeVec.size();
  size_t size = rtree.size();

//Inserting another box 
  te::gm::Envelope* boxInsert = new te::gm::Envelope(12, 2, 13,5);
  envelopeVec.push_back(boxInsert);
  rtree.insert(*(boxInsert), k++);

  for(std::size_t i = 0; i < env_size; ++i)
  {
    std::vector<int> report;
    const te::gm::Envelope* mbr = envelopeVec[i];
    size_t search_result = rtree.search(*mbr, report);

    std::cout << std::endl << "Printing search_result using mbr(x1,y1,x2,y2) " << mbr->m_llx << " " << mbr->m_lly << " " <<mbr->m_urx << " " << mbr->m_ury << std::endl;
    std::cout << "    Search Result = " << search_result << std::endl;

    for (j = 0; j < report.size(); j++)
    {
      std::cout << std::endl << "    Print report["<< j << "]: " << report[j] << std::endl;
    }
  }

// Searching in the RTree...
  std::vector<int> report1;
  size_t ss;
  size_t searchResult = rtree.search(*r1, report1);
  CPPUNIT_ASSERT( searchResult == 6);
  ss = report1.size();
  report1.clear();
  searchResult = rtree.search(*r2, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 9);
  report1.clear();
  searchResult = rtree.search(*r3, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 3);
  report1.clear();
  searchResult = rtree.search(*r4, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 5);
  report1.clear();
  searchResult = rtree.search(*r5, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 2);
  report1.clear();
  searchResult = rtree.search(*r6, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 2);
  report1.clear();
  searchResult = rtree.search(*r7, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 4);

// Inserting a box 
  te::gm::Envelope* boxInsert1 = new te::gm::Envelope(2, 1, 4,4);
  envelopeVec.push_back(boxInsert1);
  rtree.insert(*(boxInsert1), k++);
  env_size = envelopeVec.size();
  size = rtree.size();

// Searching again ...
  report1.clear();
  searchResult = rtree.search(*r6, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 3);

  te::common::FreeContents(envelopeVec);

//#endif
}
void TsRTree::tcRTreeBox_3()
{
  //#ifdef TE_COMPILE_ALL

  te::sam::rtree::Index<int, 3> rtree;

  std::vector<te::gm::Envelope*> envelopeVec;
  envelopeVec.reserve(12);

  std::size_t j; 
  int k = 0; //id 
  {
    envelopeVec.push_back(r15);
    rtree.insert(*(r15), k++);

    envelopeVec.push_back(r16);
    rtree.insert(*(r16), k++);

    envelopeVec.push_back(r17);
    rtree.insert(*(r17), k++);

    envelopeVec.push_back(r18 );
    rtree.insert(*(r18), k++);

    envelopeVec.push_back(r19 );
    rtree.insert(*(r19), k++);

    envelopeVec.push_back(r12 );
    rtree.insert(*(r12), k++);

    envelopeVec.push_back(r08 );
    rtree.insert(*(r08), k++);

    envelopeVec.push_back(r09 );
    rtree.insert(*(r09), k++);

    envelopeVec.push_back(r10 );
    rtree.insert(*(r10), k++);

    envelopeVec.push_back(r11 );
    rtree.insert(*(r11), k++);

    envelopeVec.push_back(r13 );
    rtree.insert(*(r13), k++);

    envelopeVec.push_back(r14 );
    rtree.insert(*(r14), k++);
  }
  size_t env_size = envelopeVec.size();
  size_t size = rtree.size();

//Inserting another box that causes a splitNode
  te::gm::Envelope* boxInsert = new te::gm::Envelope(12, 2, 13,5);
  envelopeVec.push_back(boxInsert);
  rtree.insert(*(boxInsert), k++);
  CPPUNIT_ASSERT(rtree.size() == size +1 );

  for(std::size_t i = 0; i < env_size; ++i)
  {
    std::vector<int> report;
    const te::gm::Envelope* mbr = envelopeVec[i];
    size_t search_result = rtree.search(*mbr, report);

    std::cout << std::endl << "Printing search_result using mbr(x1,y1,x2,y2) " << mbr->m_llx << " " << mbr->m_lly << " " <<mbr->m_urx << " " << mbr->m_ury << std::endl;
    std::cout << "    Search Result = " << search_result << std::endl;

    for (j = 0; j < report.size(); j++)
    {
      std::cout << std::endl << "    Print report["<< j << "]: " << report[j] << std::endl;
    }
  }

// Searching in the RTree...
  std::vector<int> report1;
  size_t ss;
  size_t searchResult = rtree.search(*r1, report1);
  CPPUNIT_ASSERT( searchResult == 6);
  ss = report1.size();
  report1.clear();
  searchResult = rtree.search(*r2, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 9);
  report1.clear();
  searchResult = rtree.search(*r3, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 3);
  report1.clear();
  searchResult = rtree.search(*r4, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 5);
  report1.clear();
  searchResult = rtree.search(*r5, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 2);
  report1.clear();
  searchResult = rtree.search(*r6, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 2);
  report1.clear();
  searchResult = rtree.search(*r7, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 4);

// Inserting a box which should be added in node[0], branch [0]
  te::gm::Envelope* boxInsert1 = new te::gm::Envelope(2, 1, 4,4);
  envelopeVec.push_back(boxInsert1);
  rtree.insert(*(boxInsert1), k++);
  env_size = envelopeVec.size();
  size = rtree.size();

// Searching again ...
  report1.clear();
  searchResult = rtree.search(*r6, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 3);

  te::common::FreeContents(envelopeVec);

//#endif
}

void TsRTree::tcRTreeBox_2()
{
//#ifdef TE_COMPILE_ALL

  te::sam::rtree::Index<int, 2> rtree;

  std::vector<te::gm::Envelope*> envelopeVec;  
  envelopeVec.reserve(12);
  {
    envelopeVec.push_back(r15);
    envelopeVec.push_back(r16);
    envelopeVec.push_back(r17);
    envelopeVec.push_back(r18 );
    envelopeVec.push_back(r19 );
    envelopeVec.push_back(r12 );
    envelopeVec.push_back(r08 );
    envelopeVec.push_back(r09 );
    envelopeVec.push_back(r10 );
    envelopeVec.push_back(r11 );
    envelopeVec.push_back(r13 );
    envelopeVec.push_back(r14 );
  }
  size_t env_size = envelopeVec.size();

  int k = 0; //id
  for (std::size_t j = 0; j < env_size; j++)
  {
    te::gm::Envelope* mbr = envelopeVec[j];
    rtree.insert(*(mbr), k++);
  }      
  //Inserting another box 
  te::gm::Envelope* boxInsert = new te::gm::Envelope(12, 2, 13,5);
  envelopeVec.push_back(boxInsert);
  rtree.insert(*(boxInsert), k++);
  env_size = envelopeVec.size();

  for(std::size_t i = 0; i < env_size; ++i)
  {
    std::vector<int> report;
    const te::gm::Envelope* mbr = envelopeVec[i];
    size_t search_result = rtree.search(*mbr, report);

    std::cout << std::endl << "Printing search_result using mbr(x1,y1,x2,y2) " << mbr->m_llx << " " << mbr->m_lly << " " <<mbr->m_urx << " " << mbr->m_ury << std::endl;
    std::cout << "    Search Result = " << search_result << std::endl;

    for (std::size_t n = 0; n < report.size(); n++)
    {
      std::cout << std::endl << "    Print report["<< n << "]: " << report[n] << std::endl;
    }
  }

// Searching in the RTree ...
  std::vector<int> report1;
  size_t ss;
  size_t searchResult = rtree.search(*r1, report1);
  CPPUNIT_ASSERT( searchResult == 6);
  ss = report1.size();
  report1.clear();
  searchResult = rtree.search(*r2, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 9);
  report1.clear();
  searchResult = rtree.search(*r3, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 3);
  report1.clear();
  searchResult = rtree.search(*r4, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 5);
  report1.clear();
  searchResult = rtree.search(*r5, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 2);
  report1.clear();
  searchResult = rtree.search(*r6, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 2);
  report1.clear();
  searchResult = rtree.search(*r7, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 4);

// Inserting a box
  te::gm::Envelope* boxInsert1 = new te::gm::Envelope(2, 1, 4,4);
  envelopeVec.push_back(boxInsert1);
  rtree.insert(*(boxInsert1), k++);
  env_size = envelopeVec.size();
  size_t size = rtree.size();

// Searching again ...
  report1.clear();
  searchResult = rtree.search(*r6, report1);
  ss = report1.size();
  CPPUNIT_ASSERT(searchResult == 3);

  te::common::FreeContents(envelopeVec);

//#endif
}
