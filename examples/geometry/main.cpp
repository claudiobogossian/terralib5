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
  \file main.cpp

  \brief TerraLib Geometry module examples.
 */

// Examples
#include "GeometryExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/common/Globals.h>
#include <terralib/geometry.h>

// STL
#include <cassert>
#include <cstdlib>
#include <iostream>

//@{ Geometries

// Points
te::gm::Point* pt1;
te::gm::Point* pt2;

// Lines
te::gm::LineString* l1;
te::gm::LineString* l2; 
te::gm::LineString* l3;

// Polygons
te::gm::Polygon* p; 
te::gm::Polygon* pHole;

//@}

void createGeometries()
{
  std::cout << "Creating geometries..." << std::endl;

// Create points
  pt1 = createPoint(0.0, 0.0);
  assert(pt1);
  std::cout << pt1->asText() << std::endl;
  
  pt2 = createPoint(5.0, 5.0);
  assert(pt2);  
  std::cout << pt2->asText() << std::endl;

// Create lines
  l1 = createLineString(0.0, 0.0, 10.0, 10.0);
  assert(l1);
  std::cout << l1->asText() << std::endl;
  
  l2 = createLineString(0.0, 10.0, 10.0, 0.0);
  assert(l2);
  std::cout << l2->asText() << std::endl;
  
  l3 = createLineString(0.0, 20.0, 20.0, 20.0);
  assert(l3);
  std::cout << l3->asText() << std::endl;

// Create polygons
  p = createPolygon();
  assert(p);
  std::cout << p->asText() << std::endl;
  
  pHole = createPolygonWithHole();
  assert(pHole);
  std::cout << pHole->asText() << std::endl;
}

void spatialRelationsExamples()
{
  std::cout << "Calculating spatial relations..." << std::endl;

// Verifies some spatial relations
  bool result;

// Points
  result = spatialRelation(pt1, pt2, te::gm::DISJOINT); // true
  result = spatialRelation(pt1, pt2, te::gm::TOUCHES); // false

// Points and lines //???
result = spatialRelation(pt1, l1, te::gm::TOUCHES); // true
result = spatialRelation(pt1, l2, te::gm::TOUCHES); // false

// Points and polygons
  result = spatialRelation(pt2, p, te::gm::INTERSECTS); // true
  result = spatialRelation(pt2, pHole, te::gm::INTERSECTS); // false

// Lines
  result = spatialRelation(l1, l1, te::gm::EQUALS); // true
  result = spatialRelation(l1, l2, te::gm::CROSSES); // true
  result = spatialRelation(l1, l3, te::gm::CROSSES); // false

// Lines and polygons
  result = spatialRelation(l1, p, te::gm::INTERSECTS); // true
  result = spatialRelation(l3, p, te::gm::INTERSECTS); // false

// Polygons
  result = spatialRelation(p, pHole, te::gm::COVERS); // true
}

void setOperationsExamples()
{
  std::cout << "Executing set operations..." << std::endl;
  std::cout << "Results:" << std::endl;

// Applies set operations
  te::gm::Geometry* mpt = Union(pt1, pt2); // return a multi point
  assert(mpt);
  std::cout << mpt->asText() << std::endl;
  delete mpt;

  te::gm::Geometry* mgeom = Union(pt2, l3); // return a geometry collection
  assert(mgeom);
  std::cout << mgeom->asText() << std::endl;
  delete mgeom;

  te::gm::Geometry* ml = Union(l1, l2); // return a multi line
  assert(ml);
  std::cout << ml->asText() << std::endl;
  delete ml;

  te::gm::Geometry* ptInt = intersection(l1, l2); // return a point
  assert(ptInt);
  std::cout << ptInt->asText() << std::endl;
  delete ptInt;

  te::gm::Geometry* pDiff = difference(p, pHole); // return a polygon
  assert(pDiff);
  std::cout << pDiff->asText() << std::endl;
  delete pDiff;

  te::gm::Geometry* lInt = intersection(l1, pHole); // return a multi line
  assert(lInt);
  std::cout << lInt->asText() << std::endl;
  delete lInt;
}

void wkbConversionExamples()
{
  std::cout << "Executing wkb conversion..." << std::endl;
  std::cout << "Results:" << std::endl;
  char* wkb = new char[pt1->getWkbSize()];

  te::gm::WKBWriter::write(pt1, wkb);
  te::gm::Geometry* g(te::gm::WKBReader::read(wkb));
  assert(g->equals(pt1));
  std::cout << g->asText() << std::endl;

  delete g;
  delete[] wkb;

  wkb = new char[pt2->getWkbSize()]; 
  te::gm::WKBWriter::write(pt2, wkb);
  g = te::gm::WKBReader::read(wkb);
  assert(g->equals(pt2));
  std::cout << g->asText() << std::endl;

  delete g;
  delete[] wkb;

  wkb = new char[l1->getWkbSize()]; 
  te::gm::WKBWriter::write(l1, wkb); 
  g = te::gm::WKBReader::read(wkb);
  assert(g->equals(l1));
  std::cout << g->asText() << std::endl;

  delete g;
  delete[] wkb;

  wkb = new char[l2->getWkbSize()]; 
  te::gm::WKBWriter::write(l2, wkb); 
  g = te::gm::WKBReader::read(wkb);
  assert(g->equals(l2));
  std::cout << g->asText() << std::endl;

  delete g;
  delete[] wkb;

  wkb = new char[l3->getWkbSize()]; 
  te::gm::WKBWriter::write(l3, wkb); 
  g = te::gm::WKBReader::read(wkb);
  assert(g->equals(l3));
  std::cout << g->asText() << std::endl;

  delete g;
  delete[] wkb;

  wkb = new char[p->getWkbSize()]; 
  te::gm::WKBWriter::write(p, wkb); 
  g = te::gm::WKBReader::read(wkb);
  assert(g->equals(p));
  std::cout << g->asText() << std::endl;

  delete g;
  delete[] wkb;

  wkb = new char[pHole->getWkbSize()]; 
  te::gm::WKBWriter::write(pHole, wkb); 
  g = te::gm::WKBReader::read(wkb);
  assert(g->equals(pHole));
  std::cout << g->asText() << std::endl;

  delete g;
  delete[] wkb;
  
}

void deleteGeometries()
{
  delete pt1;
  delete pt2;
  delete l1;
  delete l2;
  delete l3;
  delete p;
  delete pHole;
}

int main(int /*argc*/, char** /*argv*/)
{

  TerraLib::getInstance().initialize();

// Call of examples methods
  createGeometries();
  spatialRelationsExamples();
  setOperationsExamples();
  wkbConversionExamples();
  readWkts("./geometries.wkt");
  //readWkts("./wkt_geom.txt");

  deleteGeometries();


  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
